///////////////////////////////////////////////////////////////////////////////
// VAV to TZX (or TAP) converter
//
// Adapted from the original VOC2TZX converter by:                      v0.54b
// (c) 1997 Tomaz Kac and Martijn van der Heide
//
// (c) 2008 Miguel Angel Rodriguez Jodar 
//    - WAV loading routine (VOC deprecated)
//    - Butterworth filtering
//    - Clenaup code to compile under MacOS and Linux
//
// Also endian depandable (Intel compatible), possible fix:
// Search for ENDIAN word to find out where it should be changed (probably
// forgot some of the places though :( ) Most of the time the WORD (2 byte)
// values are being written to disk ... so you should first swap them and
// then write them ...

#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned char *raw;			// Address of the loaded .WAV file (converted to RAW)
int rawlen;			// Length of the .WAV file in the memory
int rate;			// Sample Rate
int silence=0;		// Are Silence blocks in the .VOC file ?  Not used for WAV (MARJ)
unsigned char temp[65536*2];	// converted data
unsigned char tstr[14];
unsigned char tzxbuf[10]={ 'Z','X','T','a','p','e','!', 0x1A, 1, 10 };
unsigned char cpcbuf[5]={ 0x33, 1, 0, 0x15, 1 };
FILE *ofh;			// Output File Handle

// Standard Timings for ROM loading routines
int HP_PILOT=	2168;	// Half-Period of Pilot pulse
int HP_SYNC1=	667;	//                First Sync
int HP_SYNC2=	735;	//                Second Sync
int HP_BIT0=	855;	//                Bit 0
int HP_BIT1=	1710;	//                Bit 1
int STD_PAUSE=	1000;	// Standard Pause in milliseconds (ms)
int PILOT_HEAD= 8064;	// Length of Pilot when Header Block (ID==00, Len:19)
int PILOT_DATA= 3220;	// Length of Pilot when Data Block   (ID!=00)
int STD_TRES=	20;		// Treshold in % for a value to be recognised as
						// Standard
int TRES_FULL=	35;		// [35] Max Treshold in % between two HPs
int TRES_01=	40;		// [30] Min Treshold in % between 0 and 1 bit HP
int TRES_END=  	60;		// [60] Min Treshold in % between two HPs to end the Load
int MIN_PILOT=	50;		// [50] Min Number of PILOT HPs

int tape=0;				// Are we making .TAP file ?
int cpc=0;				// Creating a Amstrad CPC .CDT file ?
int rom=0;				// Should we use the ROM values for 0 and 1 timing ?
int ignore=0;			// Ignore Last Byte if it contains less than 8 bits ?
int aprox=1;			// Are we approximating Bit 1 HP == 2* Bit 0 HP ?
int force=0;			// Force Custom Loader Length to forcep ?
int forcep;
int fsync=0;			// Force SYNC of Custom blocks to Standard SYNC ?
int alter=0;			// Alternative way to recognise 0 and 1 bit (with averaging them)
int difend=0;			// Use alternative way to find END-OF-BLOCK ... with TRES_END as Treshold
						// Value
int middle=0;			// Use Middle value to determine 0 and 1 bit
int slock=0;			// Use Speedlock conversion Algorythm (0=NO, 1,2,3,4 - YES)
int diload=0;			// Use Digital Integration Algorythm

int dofilter=0;         // Apply Butterworth filter?

int cur=0;					// Current position in the sample
int last;					// Position of last Pulse in the sample
int plen=0;					// Current Pulse length
int lplen;					// Length of Last Pulse
int pilot,num0,num1;		// Number of Pilot, 0 and 1 bit HP's
double total,total0,total1;	// Total length of Pilot, 0 and 1 tones
double cycle;				// How much Z80 cycles is one Sample (byte)
int block=0;				// Number of the block to recognise
int start;					// Start of the block in the sample
int endb;					// End of the last block in the sample
int sync1,sync2;			// Length of both SYNC in T States
int hp0,hp1,hpt;			// Length of 0 and 1 bit HP's (first occurances)
int hp01;					// Middle value of bit 0 and 1
int datastart;				// Start of DATA in the sample
int apilot,ahp0,ahp1;		// Average Length of Pilot, 0 and 1 bit HP's in T
int posbyte,posbit;			// Position in : Byte in block, Bit in Byte
int currbit;				// Value of the Current Bit of Data read
float pausev;				// Value of the pause (in seconds)
unsigned char buf2[255];				// Buffer for file writes
int custom=' ';				// Are we saving a Custom Loading block ?
int lst_pilot;				// Previous Pilot length
int dif;
int PilotOK;				// Was the Pilot tone recognised OK ?
int NoData;					// Was there any data in the data block ?
int bleep=0;				// Using algorythm to recognise Firebirds' BleepLoad?
int naslnorm=0;				// For bleepload algorythm...

unsigned char finp[255];   // Input File  (First Command Line Option)
unsigned char fout[255];   // Output File (Second Command Line Option or First with .TZX)
unsigned char errstr[255]; // Error String
int n=0;
int files=0;	  // Number of Files on the command line

unsigned short slock12sync[14]=	{
								3150,3150,
								1130,1130,1130,1130,1130,1130,565,565,1130,1130,565,565
								};	// 2 SYNC pulses + 6 bits of data (111010) of Speedlock 2
									// for Speedlock 1 these 6 bits of data MUST be read from the tape
unsigned short slock34sync[28]=	{
								667,735,
								714,714,1428,1428,2144,2144,2144,2144,2144,2144,1428,1428,714,714,
								0,0,0,0,0,0,0,0,0,0,0,0
								};	// 2 Standard SYNC, 14 Composed SYNC pulses and 6 bits of Data for Type 2,3
unsigned short slockmidsync[21*2];	// SYNC between two Sub-blocks in second turbo block of 2,3

int subblocknum=1;	// Number of Sub-Blocks in Speedlock 2 and 3 second turbo block
int totallen;		// Total length of Speedlock 2 and 3 second turbo block
unsigned char totalcheck;	// Checksum of all sub-blocks ... THIS ONE ISN'T RIGHT !
int slockvar=0;		// Speedlock Variation for Speedlock 3 : 1 - Red/Black  Decryption Stripes
					//										 2 - Multicolor Decryption Stripes
int slockskip=0xFFFF; 	// How many blocks to skip before encounter first Turbo block
int ClickLen;		// Length of Click in Pulses
int ClickPulses[10];	// Pulses of the Click
int PilotPulse;		// Pulse of the Pilot signal
int NumPilotParts;	// Number of Pilot Parts in Speedlock 1,2 pilot
int PilotParts[50];	// Table of Pilot Parts Length
int slock1flag=0;	// Are we reading 6 bits of the Speedlock 1 flag byte ?

int dipilot;		// Digital Integration PRE-PILOT length in pulses
double ditotal;		// Total length of DI Pre-Pilot
int adipilot;		// Average pilot length of di pre-pilot

void Error(char *errstr)
{
// exits with an error message *errstr

printf("\n-- Error: %s\n",errstr);
exit(0);
}

void ChangeFileExtension(char *str,char *ext)
{
// Changes the File Extension of String *str to *ext
int n;

n=strlen(str); while (str[n]!='.') n--;
n++; str[n]=0; strcat(str,ext);
}

/* From: http://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript 
Band-pass filter: 400 to 4000 Hz. Butterworth, 2nd order */
#define NZEROS 4
#define NPOLES 4
#define GAIN   2.067801344e+01

void Filtrar (unsigned char *raw, int lraw)
{
  double xv[NZEROS+1], yv[NPOLES+1], sample;
  int i;

  printf ("Filtering ...\n");
  for (i=0;i<5;i++)
  {
	  xv[i]=(raw[i]-128.0)/256;
	  yv[i]=(raw[i]-128.0)/256;
  }

  for (;i<lraw;i++)
  {
        xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; 
        xv[4] = ((raw[i]-128.0)/256) / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; 
        yv[4] =   (xv[0] + xv[4]) - 2 * xv[2]
                     + ( -0.4847923389 * yv[0]) + (  2.2258314555 * yv[1])
                     + ( -3.9809841336 * yv[2]) + (  3.2391830490 * yv[3]);
		sample = (yv[4]*1000);
		raw[i]=(sample>127)?255:(sample<-128)?0:sample+128;
  }
}

void ConvertWAVtoRAW(unsigned char *buffer, int lbuffer, unsigned char *raw, int *iraw, int numchan, int bitspersample)
{
  signed short *sbuffer=(signed short *)buffer;
  int nsamples=lbuffer/(numchan*bitspersample/8);
  int i,temp;

  for (i=0;i<nsamples;i++)
  {
    if (numchan==1 && bitspersample==8)
      raw[*iraw]=buffer[i];
    else if (numchan==1 && bitspersample==16)
      raw[*iraw]=128+(sbuffer[i]/256);
    else if (numchan==2 && bitspersample==8)
    {
      temp=(int)buffer[i*2]+buffer[i*2+1]-256;
      temp=(temp<-128)? -128:(temp>127)?127:temp;
      raw[*iraw]=temp+128;
    }
    else if (numchan==2 && bitspersample==16)
    {
      temp=(int)sbuffer[i*2]+sbuffer[i*2+1];
      temp=(temp<-32768)? -32768:(temp>32767)?32767:temp;
      raw[*iraw]=(temp/256)+128;
    }
    else
      Error ("Invalid conversion detected. Should not happen!!\n");

    (*iraw)++;
  }
}

int LoadWAV(char *filename)
{
// Loads a .WAV file to the memory
// Input   : filename
// Outputs : raw    - address of the loaded file
//  remark : samples on the raw buffer have to be 8 bits, unsigned, as VOC samples were. (MARJ)
//           rawlen - length of the loaded file
//           rate   - sample rate of the file (Hz)
// Returns : 0 - when everything went OK
//			 1 - when input file is not found
//           2 - when there is not enough memory to load file
//           3 - when the file is not the right type or corrupt

FILE *fh;
int flen;
unsigned char buffer[1024];
int iraw=0;
int leido;
unsigned int samplerate;
unsigned short wavetype, numchan, bitspersample;

if ((fh=fopen(filename,"rb"))==NULL) return(1);
fseek(fh,0,SEEK_END);
flen=ftell(fh);
rewind(fh);
raw=(char *) malloc(flen);
if (raw==NULL) { fclose(fh); return(2); }
fread(buffer,1,44,fh);   // read RIFF, fmt and begining of data chunks

// ENDIAN dependent!!!
wavetype=*(unsigned short *)(buffer+20);
numchan=*(unsigned short *)(buffer+22);
samplerate=*(unsigned int *)(buffer+24);
bitspersample=*(unsigned short *)(buffer+34);

if (strncmp(buffer,"RIFF",4) || strncmp(buffer+8,"WAVE",4) || strncmp(buffer+12,"fmt ",4) || wavetype!=1 /* linear PCM */ ||
    (numchan!=1 && numchan!=2) || (bitspersample!=8 && bitspersample!=16) )
{
  free(raw);
  fclose(fh);
  if (!strncmp(buffer,"RIFX",4))
     printf("-- Error: Big endian WAV format not supported (RIFX)\n");
  if (wavetype!=1)
    printf ("-- Error: Not a linear PCM file\n");
  if (numchan!=1 && numchan!=2)
    printf ("-- Error: # of channels in WAV file not supported (%d channels found)\n", numchan);
  if (bitspersample!=8 && bitspersample!=16)
    printf ("-- Error: Bits per sample in WAV file not supported (%d bits found)\n", bitspersample);
  return 3;
}

printf ("Loaded WAVE file. Sample rate: %d Hz, %d channels, %d bits per sample.\n", samplerate, numchan, bitspersample);

leido=fread (buffer, 1, 1024, fh);
while(1)
{
  if (leido<1024)
    break;

  //Do conversion to RAW
  ConvertWAVtoRAW(buffer, 1024, raw, &iraw, numchan, bitspersample);
  leido=fread (buffer, 1, 1024, fh);
}
ConvertWAVtoRAW(buffer, leido, raw, &iraw, numchan, bitspersample);
iraw--;

if (dofilter)
{
	if (samplerate!=44100)
		printf("-- Warning: filter is only effective at a sampling rate of 44100 Hz. Yours is %d Hz\n", samplerate);
	Filtrar(raw, iraw);
}

fclose(fh);
rawlen = iraw;
rate = samplerate;

return 0;
}

int GetPulse()
{
// Finds next Pulse and returns its length
#define PT 127		// Treshold value ...

last=cur;
if (raw[cur]>PT)	while(raw[cur]> PT && cur<rawlen) cur++;
else				while(raw[cur]<=PT && cur<rawlen) cur++;
return(cur-last);
}

int Diff(int a,int b)
{
// Returns the Difference (in %) between a and b (a+(a*(d/100))=b) d=diff
float aa,bb;

if (!a && !b) return(0);
if (a>b) { aa=(float) a; bb=(float) b;}
else     { aa=(float) b; bb=(float) a;}
return(abs((int) (0.5+(100.0*((bb-aa)/aa)))));
}

char *CheckSum(int len)
{
// Calculates CheckSum for the data in temp[], length len
unsigned char c=0;
int n=0;

while (n<len-1) { c^=temp[n]; n++; }
if (c==temp[len-1]) return("Y "); else return("N ");
}

char *Identify(int len)		
{
// Returns string pointer to what the block in Temp is (Program: ... , Bytes..)
int n;

if (cpc)
	{
	if (temp[0]==44)
		{
		for (n=0; n<12; n++)
			{
			if (temp[n+1])	tstr[n]=temp[n+1];
			else			tstr[n]=' ';
			}
		tstr[n]=0;
		}
	else strcpy(tstr,"------------");
	return(tstr);
	}

if (temp[0]==0 && (len==19 || len==20) && temp[1]<4)
	{
	switch (temp[1])
		{
		case 0x00: strcpy(tstr,"P:"); break;
		case 0x01: strcpy(tstr,"N:"); break;
		case 0x02: strcpy(tstr,"C:"); break;
		case 0x03: strcpy(tstr,"B:"); break;
		}
	 for (n=0; n<10; n++)
	 	{
	 	if (temp[n+2]>31 && temp[n+2]<127) tstr[n+2]=temp[n+2];
		else                               tstr[n+2]=32;
		}
	tstr[13]=0;
	}
else strcpy(tstr,"------------");
return(tstr);
}

void SavePureData(unsigned char *data, int len, int sync)
	{
	// This will save the Pure data of Speedlock (or other) tapes... also with the Speedlock SYNC
	buf2[0]=0x14;
	buf2[1]=(unsigned char) (ahp0&0xff); buf2[2]=(unsigned char) (ahp0>>8);
	buf2[3]=(unsigned char) (ahp1&0xff); buf2[4]=(unsigned char) (ahp1>>8);
	buf2[5]=(unsigned char) posbit;
	buf2[6]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))&0xff);
	buf2[7]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))>>8);
	buf2[8]=(unsigned char) (len&0xff); buf2[9]=(unsigned char) ((len>>8)&0xff);
	buf2[10]=(unsigned char) ((len>>8)>>8);
	fwrite(buf2,1,11,ofh); fwrite(data,1,len,ofh);
	if (sync)	// Write the Mid-Block SYNC ?
		{
		buf2[0]=0x13;
		buf2[1]=21*2;		// 21 'waves' or 42 pulses
		for (n=0; n<21; n++)
			{	// the mid-block SYNC is   000001111111111100000
			if (n<5 || n>15)	slockmidsync[(n*2)]=slockmidsync[1+(n*2)]=875;
			else				slockmidsync[(n*2)]=slockmidsync[1+(n*2)]=1750;;
			}
		fwrite(buf2,1,2,ofh);	// Write the mid-block SYNC , ENDIAN depandable !!!
		fwrite(slockmidsync,1,21*2*2,ofh);  // ENDIAN depandable !!!
		}
	}

void SaveGroupEnd()
{
buf2[0]=0x22;
fwrite(buf2,1,1,ofh);
}

void SaveTZXBlock()
{
// Saves the .TZX block to the file

// If bleepload then DON'T save the blocks with Pilot ONLY and make the next block after the pilot
// only signal a 'NORMAL' block (so it has a bigger pilot signal ... it loads in better)
// This will NOT show on screen ! (the screen will show the empty blocks, but they are not saved!
if (bleep)
	{
	if (!posbit) { naslnorm=1; return; }
	if (naslnorm)
		{
		naslnorm=0;
		apilot=HP_PILOT;
		lst_pilot=PILOT_DATA;
		}
	}
// Save the SpeedLock PURE DATA blocks
if (slock && ((block==slockskip+1 || block==slockskip+2) ||
              (slock==1 && slockvar==2 && block>slockskip)))
	{
	SavePureData(temp,posbyte,0);
	SaveGroupEnd();
	return;
	}
// Save the additional PRE-PILOT pulse for DI loader , Normal PILOT,
// ONE SYNC pulse and the data as Pure Data Block
if (diload && (block==3 || block==4))
	{
	// Save the GROUP Name
	buf2[0]=0x21;
	buf2[1]=10;
	sprintf(&buf2[2],"DI Block %d",block-2);
	fwrite(buf2,1,12,ofh);
	// Save the PRE-PILOT
	adipilot=(int) ((ditotal/(double) dipilot)/cycle + 0.5);
	buf2[0]=0x12;
	buf2[1]=(unsigned char) (adipilot&0xff); buf2[2]=(unsigned char) (adipilot>>8);
	buf2[3]=(unsigned char) (dipilot&0xff); buf2[4]=(unsigned char) (dipilot>>8);
	fwrite(buf2,1,5,ofh);
	// Save the PILOT
	buf2[0]=0x12;
	buf2[1]=(unsigned char) (apilot&0xff); buf2[2]=(unsigned char) (apilot>>8);
	buf2[3]=(unsigned char) (lst_pilot&0xff); buf2[4]=(unsigned char) (lst_pilot>>8);
	fwrite(buf2,1,5,ofh);
	// Save the SYNC
	buf2[0]=0x13;
	buf2[1]=1;
	buf2[2]=(unsigned char) (sync1&0xff); buf2[3]=(unsigned char) (sync1>>8);
    fwrite(buf2,1,4,ofh);
	// Save the DATA
	SavePureData(temp,posbyte,0);
	// And the group end
	SaveGroupEnd();
	return;
	}

// First check if this is a Standard Loading block
if ((Diff(ahp0,HP_BIT0)<=STD_TRES) && (Diff(ahp1,HP_BIT1)<=STD_TRES) &&
	(Diff(apilot,HP_PILOT)<=STD_TRES) && (posbyte<65536) &&
	((temp[0] && (Diff(lst_pilot,PILOT_DATA)<=STD_TRES)) ||
	(!temp[0] && (Diff(lst_pilot,PILOT_HEAD)<=STD_TRES))))
	{
	// Yep, this is a Standard Loading Block (regardless what
	// SYNC pulses it has (they are most of the time recognised
	// as wrong when they are really right)) ... Lets save it ...
	buf2[0]=0x10;
	buf2[1]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))&0xff);
	buf2[2]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))>>8);
	buf2[3]=(unsigned char) (posbyte&0xff); buf2[4]=(unsigned char) (posbyte>>8);
	fwrite(buf2,1,5,ofh); fwrite(temp,1,posbyte,ofh);
	}
else
	{
	// NON-Standard Loading Block...
	buf2[0]=0x11;
	buf2[1]=(unsigned char) (apilot&0xff); buf2[2]=(unsigned char) (apilot>>8);
	if (fsync) { sync1=HP_SYNC1; sync2=HP_SYNC2; }	// Force SYNC values
	buf2[3]=(unsigned char) (sync1&0xff); buf2[4]=(unsigned char) (sync1>>8);
	buf2[5]=(unsigned char) (sync2&0xff); buf2[6]=(unsigned char) (sync2>>8);
	buf2[7]=(unsigned char) (ahp0&0xff); buf2[8]=(unsigned char) (ahp0>>8);
	buf2[9]=(unsigned char) (ahp1&0xff); buf2[10]=(unsigned char) (ahp1>>8);
	if (force) lst_pilot=forcep;					// Force PILOT Length
	buf2[11]=(unsigned char) (lst_pilot&0xff); buf2[12]=(unsigned char) (lst_pilot>>8);
	buf2[13]=(unsigned char) posbit;
	buf2[14]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))&0xff);
	buf2[15]=(unsigned char) (((int) ((pausev+0.0005)*1000.0))>>8);
	buf2[16]=(unsigned char) (posbyte&0xff); buf2[17]=(unsigned char) ((posbyte>>8)&0xff);
	buf2[18]=(unsigned char) ((posbyte>>8)>>8);
	fwrite(buf2,1,19,ofh); fwrite(temp,1,posbyte,ofh);
	}
}

void GetStandardPilot()
{
// Find PILOT signal (first find two pulses that are almost identical)

do	{
	lplen=plen; plen=GetPulse();
	} while (Diff(plen,lplen)>TRES_FULL && cur<rawlen);
start=cur-(plen+lplen);	// Start of the block - so the Pause can be calculated
pilot=0;
total=0.0;
plen=(int) (((float)plen+(float)lplen+0.5)/2.0);
do	{	// OK, found the possible PILOT ... try and load it ...
	lplen=GetPulse();
	pilot++; total+=(double) lplen;
	} while(Diff(plen,lplen)<=TRES_FULL && cur<rawlen);
pilot--; total-=(double) lplen;
//if (lplen>plen) pilot=0;
if (pilot>=MIN_PILOT)	PilotOK=1;	// YES, found PILOT ... lets try the SYNC & DATA
else					PilotOK=0;
// Check if this is bleeploader then first pilot tones must be over 1000 long
if (bleep && block<3 && pilot<1000) PilotOK=0;
}

void GetDIPilot()
{
// Find PILOT signal (first find two pulses that are almost identical)
do	{
	lplen=plen; plen=GetPulse();
	} while (Diff(plen,lplen)>TRES_FULL && cur<rawlen);
start=cur-(plen+lplen);	// Start of the block - so the Pause can be calculated
dipilot=2;
ditotal=(double) (plen+lplen);
plen=(int) (((float)plen+(float)lplen+0.5)/2.0);
do	{	// OK, found the possible PRE-PILOT signal... try and load it ...
	lplen=GetPulse();
	dipilot++; ditotal+=(double) lplen;
	} while(Diff(plen,lplen)<=TRES_FULL && cur<rawlen);
dipilot--; ditotal-=(double) lplen;
pilot=1;
total=(double) lplen;
plen=GetPulse();
plen=(int) (((float)plen+(float)lplen+0.5)/2.0);
do	{	// After the PRE-PILOT the normal PILOT begins ...
	lplen=GetPulse();
	pilot++; total+=(double) lplen;
	} while(Diff(plen,lplen)<=TRES_FULL && cur<rawlen);
//pilot--; total-=(double) lplen;
if (pilot>=MIN_PILOT)	PilotOK=1;	// YES, found PILOT ... lets try the SYNC & DATA
else					PilotOK=0;
}

void GetClickingPilot()
{
double totalpulses;
int numpulses;
int avgpilot;
double totalclickpulses[10];
int currclick;

ClickLen=2;	// it seems it is always 2 anyway :)

// Find PILOT signal (first find two pulses that are almost identical)
do	{
	lplen=plen; plen=GetPulse();
	} while (Diff(plen,lplen)>TRES_FULL && cur<rawlen);
pilot=numpulses=2;	// We found atleast two pulses already
totalpulses=(double) (plen+lplen);
start=cur-(plen+lplen);	// Start of the block - so the Pause can be calculated
NumPilotParts=0;
for (n=0; n<10; n++) totalclickpulses[n]=0.0;
currclick=0;

	readnextpilot:

avgpilot=(int) (0.5+(totalpulses/(double) numpulses));
do	{	// OK, found the possible PILOT ... try and load it ...
	plen=lplen;
	lplen=GetPulse();
	if (lplen>avgpilot*1.3) goto endofpilot; // 1.3 , Sometimes 1.2 or 1.4 better
	if (lplen<avgpilot*0.8) goto endofpart;	 // 0.8 , Sometimes 0.6 or 0.7 better
	totalpulses+=(double) lplen; numpulses++;
	avgpilot=(int) (0.5+(totalpulses/(double) numpulses));
	pilot++;
	} while(cur<rawlen);
PilotOK=0; return;
endofpart:						// Found a CLICK ...
if (pilot<180 || pilot>250) { PilotOK=0; return; }
PilotParts[NumPilotParts]=pilot;
NumPilotParts++;
totalclickpulses[currclick]+=(double) lplen;
currclick++;
do	{
	lplen=GetPulse();
	if (currclick==ClickLen) goto endofclick;	// Read enough clicks ?
	totalclickpulses[currclick]+=(double) lplen;
	currclick++;
	} while(cur<rawlen);
PilotOK=0; return;

	endofclick:

currclick=0;
pilot=1;
totalpulses+=(double) lplen; numpulses++;
goto readnextpilot;

	endofpilot:				    	// Found the SYNC

if (PilotParts[NumPilotParts-4]<pilot)	// Go back a pulse if we read one too
	{ pilot--; cur-=lplen; }			// many (strange thing, since FIRST pulse is)
PilotParts[NumPilotParts]=pilot;		// Always very short compared to the 2nd one
NumPilotParts++;
PilotPulse=(int) (0.5+((double) avgpilot/cycle));	// Length of Pilot pulse
if (NumPilotParts<20) { PilotOK=0; return; }
ClickPulses[0]=ClickPulses[1]=715;
PilotOK=1; return;
}

void SaveClickingPilot()
{
unsigned char buf[256];
int m;

for (n=0; n<NumPilotParts; n++)
	{
	buf[0]=0x12;
	buf[1]=(unsigned char) (PilotPulse&0xFF); buf[2]=(unsigned char) (PilotPulse>>8);
	buf[3]=(unsigned char) (PilotParts[n]&0xFF); buf[4]=(unsigned char) (PilotParts[n]>>8);
	fwrite(buf,1,5,ofh);
	if (n!=NumPilotParts-1)
		{
		buf[0]=0x13;
		buf[1]=(unsigned char) ClickLen;
		for (m=0; m<ClickLen; m++)
			{
			buf[2+(m*2)]=(unsigned char) (ClickPulses[m]&0xFF);
			buf[3+(m*2)]=(unsigned char) (ClickPulses[m]>>8);
			}
		fwrite(buf,1,2+(ClickLen*2),ofh);
		}
	}
}

void GetStandardSync()
{
// lplen holds first HP of SYNC ... get the second one (plen)

plen=GetPulse();
sync1=(int) ((double) lplen/cycle + 0.5);
sync2=(int) ((double) plen/cycle + 0.5);
}

void GetDISync()
{
// lplen holds the value of the ONE DI Sync pulse ...
sync1=(int) ((double) lplen/cycle + 0.5);
}

void GetSpeedlock2Sync()
{
// Just skip 2 SYNC pulses, since they are ALWAYS the same
// Skip next 6 Bits of DATA too ...
// (first pulse of SYNC has already been read - lplen)
int n;

for (n=0; n<1+(6*2); n++) plen=GetPulse();
}

void GetSpeedlock1Sync()
{
// Just skip 2 SYNC pulses, since they are ALWAYS the same
// (first pulse of SYNC has already been read - lplen)

GetPulse();
}


void GetSpeedlock34Sync()
{
// Just skip ALL SYNC pulses, since they are ALWAYS the same
// Actually there are 2 Standard SYNC pulses + 14 Composed SYNC pulses and 6 Bits of DATA
// (first pulse of Standard SYNC has already been read - lplen
int n;

for (n=0; n<1+(7*2)+(6*2); n++) plen=GetPulse();
}

void GetSpeedlock234MiddleSync()
{
// Just skip ALL SYNC pulses, since they are ALWAYS the same
int n;

for (n=0; n<21*2; n++) plen=GetPulse();
}

void Get01Timings()
{
if (!rom)
	{
	plen=GetPulse();
	lplen=GetPulse();
	hp0=(int) (((double)plen+(double)lplen+0.5)/2.0);
	do	{
		plen=GetPulse();
		lplen=GetPulse();
		hpt=(int) (((double)plen+(double)lplen+0.5)/2.0);
		} while(Diff(hpt, hp0)<=TRES_01 && cur<rawlen);
	if (hpt<hp0) { hp1=hp0; hp0=hpt; } else hp1=hpt;
	}
else
	{
	hp0=(int) ((double) 855*cycle);		// When /rom is used
	hp1=(int) ((double) 1710*cycle);
	}

/*
if (block>1)
	{
	hp0=(int) ((double) 752*cycle);		// SU Tapes : 580, 1160
	hp1=(int) ((double) 1172*cycle);	// Alkatraz : 565, 1128
	}
*/

hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value for /middle
}

int GetActualSpeedlock234Data()
{
int syncs[42];
int tempcur;
int endofsubblock=0;
posbyte=posbit=num0=num1=temp[0]=0; total0=total1=0.0;
do	{
	if (!posbit && posbyte)			// Recognise end-of-subblock (if it fits the description ;) )
		{
		tempcur=cur;
		for (n=0; n<42; n++) syncs[n]=(int) (0.5+((double) GetPulse()/cycle));	// Read in 42 pulses
		cur=tempcur;
		endofsubblock=1;
		// sometimes >30 and sometimes >40 works better
		for (n=0; n<10; n++) if (Diff(syncs[n],875)>30) endofsubblock=0;
		for (n=0; n<10; n++) if (Diff(syncs[32+n],875)>30) endofsubblock=0;
		for (n=0; n<22; n++) if (Diff(syncs[10+n],1750)>30) endofsubblock=0;
//		if (posbyte==15966) endofsubblock=1; 	// Ping Pong for-last block
		if (endofsubblock) goto out2;
		}
	lplen=GetPulse();
	plen=GetPulse();
	if (difend && (Diff(plen,lplen)>=TRES_END)) goto out2;
	// Here it is 2* just in case...
	if (!difend && ((plen>(int) (2*hp1)) || (lplen>(int) (2*hp1)))) goto out2;
	// Check if the two pulses together were smaller than ONE pulse of Bit 0
	// If so then there MUST be something wrong and consider this as a
	// mistake - therefore read in what should be the proper pulse !
	if (plen+lplen<hp0)
		{ plen+=lplen; lplen=GetPulse(); plen+=lplen; lplen=GetPulse(); }
	if (!middle)	hpt=(int) (((double) (plen+lplen)+0.5)/2.0);
	else 			hpt=(int) (0.5 +((double) (plen+lplen))/2.0);
	if ((!middle && (abs(hpt-hp0)>abs(hpt-hp1))) || (middle && hpt>hp01))
		{ 
		currbit=1; total1+=(double) (plen+lplen); num1+=2;
		// If alternative way is used then do the recognition with averages !!!
		if (alter && num1>2)
			{
			hp1=(int) (0.5+(total1/ (double) num1));
			if (middle) hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value
			}
		}
	else 
		{
		currbit=0; total0+=(double) (plen+lplen); num0+=2;
		if (alter && num0>2)
			{
			hp0=(int) (0.5+(total0/ (double) num0));
			if (middle) hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value
			}
		}
	temp[posbyte]<<=1; temp[posbyte]+=currbit; posbit++;
	if (posbit==8) { posbyte++; temp[posbyte]=0; posbit=0; }
	} while(cur<rawlen);
out2:
if (posbit) posbyte++; else posbit=8;
if (posbit<8) temp[posbyte-1]<<=(8-posbit);		// Shift the last byte properly
if (posbit<8 && ignore) { posbyte--; posbit=8; }
printf("   Sub-Block %2d                                             L-%5d\n",subblocknum,posbyte);
NoData=0;				
if (endofsubblock) return(1); else return(0);	// return 1 if there are some more sub-blocks !!!
}

void GetActualStandardData()
{
posbyte=posbit=num0=num1=temp[0]=0; total0=total1=0.0;
do	{
	if (slock1flag && posbit==6) goto out;
	lplen=GetPulse();
	plen=GetPulse();
	// Check if the two pulses together were smaller than ONE pulse of Bit 0
	// If so then there MUST be something wrong and consider this as a
	// mistake - therefore read in what should be the proper pulse !
	if (plen+lplen<hp0)
		{ plen+=lplen; lplen=GetPulse(); plen+=lplen; lplen=GetPulse(); }
	// If bleeploader is used then first block is Program: !
	if (bleep && posbyte>18 && block==0 && !strcmp(CheckSum(posbyte),"û ")) goto out;
	// If /end parameter is specified then look if difference between
	// the two read half-periods is greater or equal to specified one
	if (difend && (Diff(plen,lplen)>=TRES_END)) goto out;
	// if no /end parameter is specified look if either one of the
	// read half-periods is greater than 1.4*half-period of bit 1
//	if (posbyte>18 && block==0 && !strcmp(CheckSum(posbyte),"û ")) goto out;
	if (!difend && ((plen>(int) (1.4*hp1)) || (lplen>(int) (1.4*hp1))) && !slock1flag) goto out;
	if (!middle)	hpt=(int) (((double) (plen+lplen)+0.5)/2.0);
	else 			hpt=(int) (0.5 +((double) (plen+lplen))/2.0);
	if ((!middle && (abs(hpt-hp0)>abs(hpt-hp1))) || (middle && hpt>hp01))
		{ 
		currbit=1; total1+=(double) (plen+lplen); num1+=2;
		// If alternative way is used then do the recognition with averages !!!
		if (alter && num1>2)
			{
			hp1=(int) (0.5+(total1/ (double) num1));
			if (middle) hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value
			}
		}
	else
		{
		currbit=0; total0+=(double) (plen+lplen); num0+=2;
		if (alter && num0>2)
			{
			hp0=(int) (0.5+(total0/ (double) num0));
			if (middle) hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value
			}
		}
	temp[posbyte]<<=1; temp[posbyte]+=currbit; posbit++;
	if (posbit==8) { posbyte++; temp[posbyte]=0; posbit=0; }
	} while(cur<rawlen);
out:
if (!posbit && !posbyte) { NoData=1; return; }	// NO DATA read whatsoever
if (posbit) posbyte++; else posbit=8;
if (posbit<8) temp[posbyte-1]<<=(8-posbit);		// Shift the last byte properly
if (posbit<8 && ignore) { posbyte--; posbit=8; }
NoData=0;				
}

void GetSpeedlock1Data()
{
int numblock;
unsigned char flag[10];

ahp0=565; ahp1=1130;	// Speedlock 1 bit 0/1 timings
hp0=(int) ((double) ahp0*cycle);	
hp1=(int) ((double) ahp1*cycle);
hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value for /middle
// Save the Group Name
numblock=block-slockskip+1;
buf2[0]=0x21;
buf2[1]=19;
sprintf(&buf2[2],"SpeedLock 1 Block %d",numblock);
fwrite(buf2,1,21,ofh);
SaveClickingPilot();
// Save the long 2-pulse SYNCs and 6 Bits of Data as Sequence of Pulses
// First read the 6 bits of data in ... this is a MUST for Speedlock1 ...
slock1flag=1;
GetActualStandardData();
slock1flag=0;
for (n=0; n<6; n++)
	{
	if (temp[0]&0x80) { slock12sync[2+(n*2)]=slock12sync[3+(n*2)]=ahp1; flag[n]='1'; }
	else			  { slock12sync[2+(n*2)]=slock12sync[3+(n*2)]=ahp0; flag[n]='0'; }
	temp[0]<<=1;
	}
flag[n]=0;
buf2[0]=0x13;
buf2[1]=14;
fwrite(buf2,1,2,ofh); fwrite(slock12sync,1,14<<1,ofh);   // ENDIAN depandable !!!
//
GetActualStandardData();
// the Data will be saved later with the SaveTZXBlock ... (so the pause will be right)
printf(" Speedlock 1 block %d   Flag:%s                       B%d L-%5d C%s",numblock,flag,posbit,posbyte,CheckSum(posbyte));
block++;
cur=cur-(plen+lplen); // End of the block - for Pause ...
endb=cur;
}

void GetSpeedlock234Data()
{
int numblock;
int noiselen;
int noisepulse;

switch (slock)
	{
	case 2: ahp0=565; ahp1=1130; break; 	// Speedlock 2 bit 0/1 timings
	case 3: ahp0=551; ahp1=1102; break;		// Speedlock 3 bit 0/1 timings
	case 4: ahp0=753; ahp1=1508; break;		// Speedlock 4 bit 0/1 timings
  	}
hp0=(int) ((double) ahp0*cycle);	
hp1=(int) ((double) ahp1*cycle);
hp01=hp1-((int) (0.5+ (double) (hp1-hp0)/2.0));	// Treshold value for /middle
// Fill in the correct timings for the 6 bits in the SYNC pulse ( 111010 ) for Speedlock 3 and 4
if (slock!=2)
	{
	for (n=0; n<6; n++)
		{
		if (n==3 || n==5)	slock34sync[16+(n*2)]=slock34sync[17+(n*2)]=ahp0;
		else				slock34sync[16+(n*2)]=slock34sync[17+(n*2)]=ahp1;
		}
	}
// Save the Group Name
if (block==slockskip)		// FIRST Speedlock turbo block ?
		numblock=1;
else	numblock=2;
// Calculate the noise length for speedlock 3 and get out if there is some more noise
if (slock==3 && slockvar==2 && numblock==1)
	{
	noiselen=(start-endb)/(rate/1000);	// Length of the noise in ms
	if (noiselen>24000) noiselen=24000;
	noisepulse=3000;
	if (noiselen<20000) { NoData=1; return; }
	}
buf2[0]=0x21;
buf2[1]=19;
sprintf(&buf2[2],"SpeedLock %d Block %d",slock,numblock);
fwrite(buf2,1,21,ofh);
// Save the Pure Tone for the Decryption Algorythm of Speedlock 3 Variant 2 to WORK !
if (slock==3 && slockvar==2 && numblock==1)
	{
	printf(" Speedlock 3 variant 2 Simulated Noise for %1.3f seconds\n",(double) noiselen/1000.0);
	noiselen=(int) (((double) noiselen/0.000285714)/noisepulse);
	buf2[0]=0x12;
	buf2[1]=(unsigned char) (noisepulse&0xff); buf2[2]=(unsigned char) (noisepulse>>8);
	buf2[3]=(unsigned char) (noiselen&0xff); buf2[4]=(unsigned char) (noiselen>>8);
	fwrite(buf2,1,5,ofh);
	buf2[0]=0x20;
	buf2[1]=(unsigned char) (300&0xff); buf2[2]=(unsigned char) (300>>8);
	fwrite(buf2,1,3,ofh);
	}
if (slock!=2)
	{
	// Save TYPE 3 and 4 NORMAL Pilot and SYNC
	apilot=(int) ((total/(double) pilot)/cycle + 0.5);
	buf2[0]=0x12;
	buf2[1]=(unsigned char) (apilot&0xff); buf2[2]=(unsigned char) (apilot>>8);
	buf2[3]=(unsigned char) (pilot&0xff); buf2[4]=(unsigned char) (pilot>>8);
	fwrite(buf2,1,5,ofh);
	// Save the big SYNC (plus 6 bits of first byte) as Sequence of Pulses
	buf2[0]=0x13;
	buf2[1]=28;
	fwrite(buf2,1,2,ofh); fwrite(slock34sync,1,28<<1,ofh);  // ENDIAN depandable !!!
	}
else
	{
	// Save TYPE 2 Clicking Pilot and SYNC
	SaveClickingPilot();
	// Save the long 2-pulse SYNCs and 6 Bits of Data as Sequence of Pulses
	buf2[0]=0x13;
	buf2[1]=14;
	fwrite(buf2,1,2,ofh); fwrite(slock12sync,1,14<<1,ofh);  // ENDIAN depandable !!!
	}
if (numblock==1)	// FIRST Speedlock turbo block
	{
	GetActualStandardData();			// Get the encrypted data of the first turbo block
	// the Data will be saved later with the SaveTZXBlock ... (so the pause will be right)
	printf(" Speedlock %d block 1                                     B%d L-%5d C%s",slock,posbit,posbyte,CheckSum(posbyte));
	}
else													// SECOND Speedlock turbo block
	{
	printf(" Speedlock %d block 2 :\n",slock);
	totallen=0;
	totalcheck=0;
	while(GetActualSpeedlock234Data())	// Get the data until there are no more sub-blocks
		{
		for (n=0; n<posbyte; n++) totalcheck^=temp[n];	// Calc checksum
		GetSpeedlock234MiddleSync();
		subblocknum++;
		pausev=0;
		SavePureData(temp,posbyte,1);	// Save the data with the  mid-SYNC  pulse
		totallen+=posbyte;
		}
	for (n=0; n<posbyte-1; n++) totalcheck^=temp[n];	// Calc checksum
	// Last block will be saved later in SaveTZX with the correct Pause value and no mid-SYNC ...
	totallen+=posbyte;
	if (totalcheck==temp[posbyte-1]) totalcheck='û'; else totalcheck='ù';
	printf(" Speedlock %d block 2 Total                               B%d L-%5d C%c ",slock,posbit,totallen,totalcheck);
	}
block++;
cur=cur-(plen+lplen); // End of the block - for Pause ...
endb=cur;
}

void GetDIData()
{
datastart=cur;
Get01Timings();
cur=datastart;
GetActualStandardData();		// OK, (hopefully) load in the data ...
if (NoData) return;
apilot=(int) ((total/(double) pilot)/cycle + 0.5);
ahp0=(int) ((total0/(double) num0)/cycle + 0.5);
ahp1=(int) ((total1/(double) num1)/cycle + 0.5);
// Do we need to make an approximation that HP of Bit 1 == 2*HP of Bit 0
if (aprox) { dif=ahp1-(ahp0*2); ahp0+=dif/4; ahp1-=dif/2; }

printf(
" Digital Integration Block %d          0-%4d,1-%4d      B%d L-%5d    ",
block-1, ahp0, ahp1, posbit, posbyte);
block++;
cur=cur-(plen+lplen); // End of the block - for Pause ...
endb=cur;
}

void GetStandardData()
{
// If BleepLoader is used then permit the blocks with PILOT ONLY !!!
if (bleep && (sync1>=3*HP_SYNC1 || sync2>=3*HP_SYNC2))
	{ NoData=posbit=sync1=sync2=0; posbyte=1; }
else
	{
	datastart=cur;
	Get01Timings();
	cur=datastart;
	GetActualStandardData();		// OK, (hopefully) load in the data ...
	// If BeepLoader then Force SYNC timings !!!
	if (bleep)	{ sync1=HP_SYNC1; sync2=HP_SYNC2; }
	}
if (NoData) return;
apilot=(int) ((total/(double) pilot)/cycle + 0.5);
ahp0=(int) ((total0/(double) num0)/cycle + 0.5);
ahp1=(int) ((total1/(double) num1)/cycle + 0.5);
// If BeepLoader then Force Bit 1 & 2 timings !!!
if (bleep) { ahp0=HP_BIT0; ahp1=HP_BIT1; apilot=HP_PILOT; }
// Do we need to make an approximation that HP of Bit 1 == 2*HP of Bit 0
if (aprox) { dif=ahp1-(ahp0*2); ahp0+=dif/4; ahp1-=dif/2; }
if ((Diff(ahp0,HP_BIT0)<=STD_TRES) && (Diff(ahp1,HP_BIT1)<=STD_TRES) &&
	(Diff(apilot,HP_PILOT)<=STD_TRES) && (posbyte<65536) &&
	((temp[0] && (Diff(pilot,PILOT_DATA)<=STD_TRES)) ||
	(!temp[0] && (Diff(pilot,PILOT_HEAD)<=STD_TRES))))
		custom=32;
else	custom='*';
printf(
"%c%s P-%4d,%4d S-%4d/%4d 0-%4d,1-%4d F-%2.2X B%d L-%5d C%s",
custom,	Identify(posbyte), apilot, pilot, sync1, sync2,	ahp0, ahp1,
temp[0], posbit, posbyte, CheckSum(posbyte));
block++;
cur=cur-(plen+lplen); // End of the block - for Pause ...
endb=cur;
}

static void Convert()
{
// This one will (hopefully) convert the .voc file to .tzx one :)

ofh=fopen(fout,"wb");
if (!tape)
	{
	fwrite(tzxbuf,1,10,ofh);
	if (cpc) fwrite(cpcbuf,1,5,ofh);
	}
cycle=(double) rate/3500000.0;
if (tape) printf("Creating .TAP file ...\n\n");
else
	{
	printf("Creating 1.02 version ");
	if (cpc)	printf("Amstrad CPC .CDT file ... \n\n");
	else		printf(".TZX file ... \n\n");
	}
NoData=1;
// Check the Speedlocks and Skip values ...
if (slock && slockskip==0xFFFF)		// Use default SKIP block values ?
	switch (slock)
		{
		case 1:
		case 4: slockskip=2; break;	// Speedlock 1 & 4 have ONE Basic block
		case 2:
		case 3: slockskip=4; break;	// Speedlock 2 & 3 have TWO Standard blocks
		}
while (cur<rawlen)
	{
	// Get the PILOT - Speedlock 1 and 2 have CLICKING Pilot
	if ((block==slockskip || block==slockskip+1) ||
		(slock==1 && slockvar==2 && block>slockskip))	// Speedlock Turbo Block ?
		switch (slock)
			{
			case  1:
			case  2: GetClickingPilot(); break;
			default: GetStandardPilot(); break;
			}
	else
		{
		if (diload && (block==2 || block==3))	GetDIPilot();
		else									GetStandardPilot();
		}
	if (PilotOK)
		{
		// Calc the Pause between this and last block ... (if it was OK)
 		if (!NoData)
			{
			// Print the Pause before this block (after the previous block)
	    	pausev=((float) start-(float) endb)/(float) rate;
			// if Speedlock 3 , Variant 2 then there should be NOISE in this pause !
			if (slock==3 && slockvar==2 && block==slockskip) pausev=0.0;
			if (pausev<0.0) pausev=0.0;
			if (block) printf("P-%1.3f\n",pausev);
//			printf("Start: %d   End: %d\n",start,endb);
			// Here Save the previous block with PAUSE=pausev
			if (!tape && block) SaveTZXBlock();
			}
	 	// Get the SYNC - ofcourse Speedlokcs have different ones
		if ((block==slockskip || block==slockskip+1) ||
			(slock==1 && slockvar==2 && block>slockskip))	// Speedlock Turbo Block ?
			switch (slock)
				{
				case  1: GetSpeedlock1Sync(); break;
				case  2: GetSpeedlock2Sync(); break;
				case  3:
				case  4: GetSpeedlock34Sync(); break;
				default: GetStandardSync(); break;
				}
		else
			{
			if (diload && (block==2 || block==3))	GetDISync();
			else									GetStandardSync();
			}
		// Get the DATA - Again Speedlocks have different format
		if ((block==slockskip || block==slockskip+1) ||
			(slock==1 && slockvar==2 && block>slockskip))	// Speedlock Turbo Block ?
			switch (slock)
				{
				case  1: GetSpeedlock1Data(); break;
				case  2:
				case  3:
				case  4: GetSpeedlock234Data(); break;
				default: GetStandardData(); break;
				}
		else
			{
			if (diload && (block==2 || block==3))	GetDIData();
			else									GetStandardData();
			}
		if (!NoData)			// Got some Actual Data from conversion
			{
			if (tape)		// Write the .TAP block
				{
				buf2[0]=posbyte&0xff; buf2[1]=posbyte>>8;
				fwrite(buf2,1,2,ofh); fwrite(temp,1,posbyte,ofh);
				}
			lst_pilot=pilot;	// Remember the previous pilot length for saving
			}
		}
	}
// Lets Save the Last block to disk ....
if (!tape && block && !NoData) { pausev=0.0; SaveTZXBlock(); }
if (block)	printf("\n");
else		printf("-- Error: No blocks found... please consult the documentation!\n");
fclose(ofh);
}

void invalidoption(char *s)
{
// Prints the Invalid Option error

sprintf(errstr,"Invalid Option %s !",s);
Error(errstr);
}

int getnumber(char *s)
{
// Returns the INT number contained in string *s
int i;

sscanf(s,"%d",&i); return(i);
}

int main(int argc, char *argv[])
{
printf("\nZXTape Utilities - WAV to TZX Converter v0.6\n");
printf("(c)1997 Tomaz Kac and Martijn van der Heide\n");
printf("(c)2008 Miguel Angel Rodriguez Jodar\n");
if (argc<2)
	{
	printf("\nUsage: WAV2TZX [switches] INPUT.WAV [OUTPUT.TZX | OUTPUT.TAP]\n");
	printf("       Switches:  -tap      Create a .TAP file instead of .TZX\n");
	printf("                  -cpc      Create an Amstrad CPC .CDT file\n");
	printf("                  -rom      Use ROM timings for 0 and 1 bit\n");
	printf("                  -ignore   Ignore Last Byte if it has less than 8 bits\n");
	printf("                  -filter   Apply a Butterworth BP filter from 400Hz to 4KHz\n");
	printf("                  -noaprox  DON'T make Bit1 h-p == 2*Bit0 h-p\n");
	printf("                  -maxp  n  In %% - Max. Treshold between Two HPs of Pilot\n");
	printf("                  -diff  n  In %% - Min. Treshold between 0 and 1 bit HPs\n");
	printf("                  -end   n  In %% - Min. Treshold between Two HPs to END block\n");
	printf("                  -pilot n  In HPs - Min. Number of Pilot HPs for a block\n");
	printf("                  -std   n  In %% - Max. Treshold for Timing to be Standard\n");
	printf("                  -force n  In HPs - Force Length of Custom Pilot Tone to n\n");
	printf("                  -bleep    Use the BleepLoad algorythm for conversion\n");
	printf("                  -alter    Use Alternative way to recognise 0 or 1 bit!\n");
	printf("                  -sync     Force SYNC pulses to Standard values!\n");
	printf("                  -middle   Use Middle Value of bit 0 and 1 !\n");
	printf("                  -slockX n SpeedLock X (1-4), Variant n (Type 1 and 3 only!)\n");
	printf("                  -slskip n Skip n Blocks to get to first Speedlock Turbo block\n");
	printf("                  -diload   Use Digital Integration Loader Algorythm\n");
	exit(0);
	}
for (n=1; n<argc; n++)
	{
	if (argv[n][0]=='-')
		{
		switch (argv[n][1])
			{
		    case 'f':   if (!strcmp(argv[n],"-filter"))
                          dofilter=1;
			        	else if (!strcmp(argv[n],"-force"))
						{
					 	  forcep=getnumber(argv[n+1]); 
						  force=1; 
						  n++; 
						  break;
						}
						else
						 invalidoption(argv[n]);
						break;
			case 't':	if (strcmp(argv[n],"-tap")) invalidoption(argv[n]);
						tape=1; break;
			case 'a':	if (strcmp(argv[n],"-alter")) invalidoption(argv[n]);
						alter=1; break;
			case 'b':	if (strcmp(argv[n],"-bleep")) invalidoption(argv[n]);
						bleep=1; break;
			case 'c':	if (strcmp(argv[n],"-cpc")) invalidoption(argv[n]);
						cpc=1; break;
			case 'r':	if (strcmp(argv[n],"-rom")) invalidoption(argv[n]);
					 	rom=1; break;
			case 'i':	if (strcmp(argv[n],"-ignore")) invalidoption(argv[n]);
					 	ignore=1; break;
			case 'n':	if (strcmp(argv[n],"-noaprox")) invalidoption(argv[n]);
					 	aprox=0; break;
			case 'm':	if (!strcmp(argv[n],"-maxp")) { TRES_FULL=getnumber(argv[n+1]); n++; }
						else {
							if (strcmp(argv[n],"-middle")) invalidoption(argv[n]);
								middle=1;
							}
						break;
			case 'd':	if (!strcmp(argv[n],"-diff")) { TRES_01=getnumber(argv[n+1]); n++; }
						else {
							if (strcmp(argv[n],"-diload")) invalidoption(argv[n]);
								diload=1;
							}
						break;
			case 'e':	if (strcmp(argv[n],"-end")) invalidoption(argv[n]);
					 	TRES_END=getnumber(argv[n+1]); difend=1; n++; break;
			case 'p':	if (strcmp(argv[n],"-pilot")) invalidoption(argv[n]);
					 	MIN_PILOT=getnumber(argv[n+1]); n++; break;
			case 's':	switch (argv[n][2])
							{
							case 't':	if (strcmp(argv[n],"-std")) invalidoption(argv[n]);
										STD_TRES=getnumber(argv[n+1]); n++; break;
							case 'y':	if (strcmp(argv[n],"-sync")) invalidoption(argv[n]);
										fsync=1; break;
							case 'l':	switch (argv[n][6])
											{
											case '1':	if (strcmp(argv[n],"-slock1")) invalidoption(argv[n]);
														slock=1;
														slockvar=getnumber(argv[n+1]); n++;
														break;
											case '2':	if (strcmp(argv[n],"-slock2")) invalidoption(argv[n]);
														slock=2; break;
											case '3':	if (strcmp(argv[n],"-slock3")) invalidoption(argv[n]);
														slock=3;
														slockvar=getnumber(argv[n+1]); n++;
														MIN_PILOT=1400;
														break;
											case '4':	if (strcmp(argv[n],"-slock4")) invalidoption(argv[n]);
														slock=4; break;
											case 'p':	if (strcmp(argv[n],"-slskip")) invalidoption(argv[n]);
														slockskip=getnumber(argv[n+1]); n++;
														break;
											default:	invalidoption(argv[n]);
											}
										break;
							default:	invalidoption(argv[n]);
							}
						break;
			default :	invalidoption(argv[n]);
			}
		}
	else
		{
		files++;
		switch (files)
			{
			case 1: strcpy(finp,argv[n]); break;
			case 2: strcpy(fout,argv[n]); break;
			default:Error("Too Many files on command line!");
			}
		}
	}
if (files==0) Error("No Files specified !");
if (files==1)
	{
	strcpy(fout,finp);
	if (tape) ChangeFileExtension(fout,"TAP");
	else
		{
		if (cpc)	ChangeFileExtension(fout,"CDT");
		else		ChangeFileExtension(fout,"TZX");
		}
	}

switch (LoadWAV(finp))
	{
	case 1: Error("Input file not found!"); break;
	case 2: Error("Not enough memory to load input file!"); break;
	case 3: free(raw);
	        Error("Input file corrupt or in a wrong format!"); break;
	}
Convert();
free(raw);

return 0;
}
