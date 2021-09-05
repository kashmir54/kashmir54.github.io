---
title: "CTF - GrabCON CTF 2021"
categories: [ctf]
date: 2021-09-05
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - GrabCON 2021
---

# GrabCON CTF 2021

<p align="center">
  <img src="/images/writeups/GrabCON/logo.png" width="30%"/>
</p>

Welcome! I've participated on my own in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! I focused on the web ones, but had two from crypto and one from misc. I enjoyed the web ones, learned a lot from graphql and javascript and its spread syntax.

<p align="center">
  <img src="/images/writeups/GrabCON/rank.png" width="70%"/>
</p>

---

Challenge index:

### OSINT

- [Victim 1](#victim-1)
- [Victim 2](#victim-2)
- [The Tour(1)](#the-tour1)
- [The Tour(2)](#the-tour2)

### Forensic

- [Tampered](#tampered)
- [Confession](#confession)

### Web

- [E4sy Pe4sy](#e4sy-pe4sy)


### Crypto

- [Warm-up](#warm-up)

<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>


---

# OSINT

## Victim 1
150
We got to know our victims is hiding somewhere. We got access to live CCTV camera of that place. Can you find zip code of that location?

[Live Camera](http://31.207.115.133:8080/cgi-bin/faststream.jpg?stream=half&fps=15&rand=COUNTER)

GrabCON{zipcode}

On the live camera we can see a building on contruction or being reformed:

<div display="grid">
  <img src="/images/writeups/GrabCON/OSINT/1_2.png" width="30%"/>
  <img src="/images/writeups/GrabCON/OSINT/1_3.png" width="69%"/>
</div>

From [IP geolocation lookup](https://www.iplocation.net/ip-lookup) we can see a hint of its location:

```
IP Address      Country   Region        City
31.207.115.133    Italy     Trentino-Alto Adige Brunico
```

The city is set to _39031_ zipcode, but could be different depending on the part of the city. To make sure of that, we can see that in the photo there is a yellow cableway. On Google Maps we can see that the cableway is on the south of the city and still has the _39031_ zipcode:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/brunico.png" width="95%"/>
</p>


``` GrabCON{39031} ```


## Victim 2

We've managed to get into that place in Trentino-Alto Adige, but we saw a diary left behind in that place owned by him. Go throughing that diary we've got this photo. Locals said it was a sceneric view of a mountain from a hotel. Can you find the location of the hotel located near to this place?

File

GrabCON{hotelnameinlowercase}

Author: CETACEAN

They provide us the following photo:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_0.jpg" width="70%"/>
</p>

A quick analysis let us with the following clues:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_0.png" width="70%"/>
</p>

- In Trentino-Alto Adige (challenge description and previous challenge)
- In the photo we can see the word _Schennerhof_ in a building (purple)
- A tower/monument with a clock (orange)

Let's locate the village. First, I looked for the [Schennerhof in Trentino-Alto Adige](https://www.google.es/maps/place/Schennerhof/@46.6890374,11.185739,767m/data=!3m3!1e3!4b1!5s0x4782bbc3ea26d3fb:0xd34d1298bb30ef0f!4m8!3m7!1s0x4782bbc1e23f7a2b:0xa930ded9e1a1f4c5!5m2!4m1!1i2!8m2!3d46.6890337!4d11.1879277) in Google Maps and found it a place called Schenna:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_11.png" width="95%"/>
</p>


The I lookewd for the tower name and seems like its the [Schloss Schenna](https://www.google.com/maps/place/Schloss+Schenna/@46.6913315,11.1875181,3a,75y,90t/data=!3m8!1e2!3m6!1sAF1QipOHA9u_2PHNhG_XPm3PzFnFaj5eoLICZfiCumeC!2e10!3e12!6shttps:%2F%2Flh5.googleusercontent.com%2Fp%2FAF1QipOHA9u_2PHNhG_XPm3PzFnFaj5eoLICZfiCumeC%3Dw203-h152-k-no!7i4032!8i3024!4m13!1m7!3m6!1s0x4782bbc1459b71c9:0xa3a02fd65b81ef62!2s39017+Schenna,+South+Tyrol,+Italy!3b1!8m2!3d46.6869305!4d11.1888008!3m4!1s0x4782bbc45938005d:0x5b295816c9c3b673!8m2!3d46.6914341!4d11.1875624) 

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_1.png" width="95%"/>
</p>

Now that we have located the two elements in the photo, let's try to retrieve the perpective:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_3_view.png" width="95%"/>
</p>

Check this view align with the elements in the photo... Seems like its so close to [this place](https://www.google.com/maps/@46.6891552,11.1888635,3a,75y,274.74h,78.66t/data=!3m6!1e1!3m4!1sCVfUuRyGMrtG7YbLelP-gA!2e0!7i13312!8i6656):

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_4_street.png" width="95%"/>
</p>

Behind of that point, we can see the Hotel Hohenwart, but trying the flag didn't work...

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_5_hotel.png" width="95%"/>
</p>

Then I just start thinking about the perpective and checked the reviews and photos from some apartments in the sourroundings of my calculated location. Check the following photos taken from the [_Apartments Prairerhof_](https://www.google.com/maps/place/Apartments+Prairerhof/@46.6883906,11.1889964,3a,75y,90t/data=!3m8!1e2!3m6!1sAF1QipPQcqFNJQYGVjzvWVZcflPzMS4eaHrQPtgK5Zk4!2e10!3e12!6shttps:%2F%2Flh5.googleusercontent.com%2Fp%2FAF1QipPQcqFNJQYGVjzvWVZcflPzMS4eaHrQPtgK5Zk4%3Dw203-h152-k-no!7i3968!8i2976!4m8!3m7!1s0x4782bb471bbcef65:0xaf70408505f3ffe1!5m2!4m1!1i2!8m2!3d46.6883034!4d11.1888382):

<div display="grid">
  <img src="/images/writeups/GrabCON/OSINT/2_6_close.png" width="49%"/>
  <img src="/images/writeups/GrabCON/OSINT/2_7_almost.png" width="49%"/>
</div>

No it couldn't be from the _Apartments Prairerhof_, those reviews photos are taken a little bit to the left of out reference photo... I recalculate the perspective with the points in the reference photo:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_0.png" width="95%"/>
</p>

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_8.png" width="95%"/>
</p>

It has to be taken from that purple arrow or from the Hotel Hohenwart for sure:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/2_9_takenfrom.png" width="95%"/>
</p>

And yes, they fixed the challenge and the flag worked:

``` GrabCON{hotelhohenwart} ```



## The Tour(1)
374
w0nd3r50uL! I know her but she did something horrible! She recently switched to some free and open-source software for running self-hosted social networking services. Check out her profile and find the last location she visited when she felt hungry?


I started by looking for the username in Google: 

[https://www.reddit.com/user/w0nd3r50uL/](https://www.reddit.com/user/w0nd3r50uL/)

Reddit profile is not available. Maybe is on wayback machine?

https://web.archive.org/web/20210904191920/https://www.reddit.com/user/w0nd3r50uL/

Nope.

Then I executed [Sherlock](https://github.com/sherlock-project/sherlock):

https://mastodon.social/@w0nd3r50uL

Great, the mastodon profile is real. We can see her photo and some kind of tweets:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_0.png" width="95%"/>
</p>

The challenge is asking for a place where she ate:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_1.png" width="95%"/>
</p>

So she was at Belgrade, Serbia because of the flight pass exposed on a photo (yes, we solved the Tour 2 chall before the 1 ^^').

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_2.png" width="50%"/>
</p>

Now things started to get hard. We analyzed the photos on his social media from a restaurant and the photo of the food:

<div display="grid">
  <img src="/images/writeups/GrabCON/OSINT/3_3.jpg" width="48%"/>
  <img src="/images/writeups/GrabCON/OSINT/3_33.png" width="48%"/>
</div>

Clues we have until now from the photos:

- A restaurant in the riverside.
- A wide river in Belgrade (Danube).
- Wood chairs at most and some metal ones.
- Little plants on tables and table wooden pads.
- Maybe second lane of buildings since there is another row of buildings in front.
- Tall chimeney holded by metal cables.
- Greenland in the other side of the river.
- Hanged light bulbs in the terrace.

Using Google Lens or reverse search didn't worked for me. So it was time to look for other posts. This blurred image shows a tower at the bottom. I had some clues for that tower in Belgrade by searching for it at [Google](https://www.google.com/search?q=danube+tower+belgrade&tbm=isch&ved=2ahUKEwi0h9jaw-byAhXl6OAKHYSjDoMQ2-cCegQIABAA&oq=danube+tower+belgrade&gs_lcp=CgNpbWcQAzoFCAAQgAQ6BAgAEB46BAgAEBhQ-uQBWO3xAWCO8wFoAHAAeACAAU-IAegEkgEBOZgBAKABAaoBC2d3cy13aXotaW1nwAEB&sclient=img&ei=zQg0YfTsL-XRgweEx7qYCA&bih=937&biw=1920&rlz=1C1GIWA_enES726ES726#imgrc=sa2jVaHtfl_MSM) with keywords _danube belgrade tower_:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_333.png" width="60%"/>
</p>

[RazviOverflow](https://razvioverflow.github.io/) came in and worked on finding [that blurry tower](https://www.google.com/maps/place/%D0%A6%D1%80%D0%BA%D0%B2%D0%B0+%D0%A3%D0%B7%D0%BD%D0%B5%D1%81%D0%B5%D1%9A%D0%B0+%D0%91%D0%BB%D0%B0%D0%B6%D0%B5%D0%BD%D0%B5+%D0%94%D1%98%D0%B5%D0%B2%D0%B8%D1%86%D0%B5+%D0%9C%D0%B0%D1%80%D0%B8%D1%98%D0%B5/@44.8466798,20.4120867,471m/data=!3m1!1e3!4m5!3m4!1s0x475a65aeca57330d:0xa77696182233311!8m2!3d44.8458888!4d20.4135727) and got its location and name:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_5_raz.png" width="70%"/>
</p>

Then I started looking around in that place for the different restaurants, key point here is that the photo shows that it's a place in second row of the riverside:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_6_rests.png" width="95%"/>
</p>

One by one, with the analysis we did at the beginning, looking at the first photo of each restaurant, I came across [Gardos Coffee](https://www.google.com/maps/place/Cafeteria+Gardos/@44.8479579,20.411394,793m/data=!3m1!1e3!4m5!3m4!1s0x475a6598bfd255ad:0x7472368da91d33bf!8m2!3d44.8479657!4d20.4127181) and check out the details in the photo, those chairs look familiar to me:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_7_chairs.png" width="95%"/>
</p>

Woooo-wooo, mate, that is the photo she posted on social media, flag if close:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_9.png" width="95%"/>
</p>

Then on the [reviews](https://www.google.com/maps/place/Cafeteria+Gardos/@44.8479671,20.4124115,396m/data=!3m1!1e3!4m7!3m6!1s0x475a6598bfd255ad:0x7472368da91d33bf!8m2!3d44.8479657!4d20.4127181!9m1!1b1) we find the flag, great challenge:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_8_flag.png" width="95%"/>
</p>

``` GrabCON{Oops_I_forgot_to_hide} ```


## The Tour(2)
488
Can you find the flight number and the flight operator of the last flight that took her to the final destination? E.g. GrabCON{AF226_Air_France}

On social media we found the flight pass:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_2.png" width="70%"/>
</p>

We can see that she redacted the information, but forgot the barcode... It's PDF417 as shows in the [cognex website](https://cmbdn.cognex.com/free-barcode-scanner) that I use for these things.

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/bars.png" width="70%"/>
</p>

Then I cropped the barcode, some adjustments on photoshop and ready to dance:

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_32.jpg" width="40%"/>
</p>

I used the [cognex website](https://cmbdn.cognex.com/free-barcode-scanner) but didn't worked, so an alternative is this [online tool to decode it.](https://online-barcode-reader.inliteresearch.com/)

<p align="center">
  <img src="/images/writeups/GrabCON/OSINT/3_4.png" width="70%"/>
</p>

The scanner shows:

```
PERKOVIC/MELANI MS      EYCBKAV BEGISTJU 0802 300Y009F0022 162>5
32
2MO0300BJU                                        2A115212207326
7 0                          N
```

It's information, but to know what it represent, I when to [this website](https://shaun.net/notes/whats-contained-in-a-boarding-pass-barcode/) and realized that each field has its format and significance. To get the flight codes: [Airline codes](https://airlinecodes.info/JU)

I wrapped up the information from the actual pass and got the following fields and data:

```
Format : 		PE
Name: 			RKOVIC/MELANI MS (girl)
Booking ref: 	EYCBKAV
Flight:			BEGISTJU
	From: 	BEG - Nikola Tesla Airport, Belgrade, Serbia
	To: 	IST - Istanbul New Airport
	On: 	JU 	- Air Serbia
Flight N: 		0802
Julian date: 	October 27
Cabin:			Y (Economy)
Seat: 			009F
Sequence:		0022
Passenger status:1
Size:			62
Version num:	5
```

With that information we can build up the flag:

``` GrabCON{JU802_Air_Serbia} ```

---

# Forensic

## Tampered
332
In our company we caught one of the employee tampering a file so we took a some backup from his computer and now we need your help to figure few things.

Name of the new file which our employee was tampering. Example : "important note.txt"

Which tool he was using ? Example : random.exe

What was the changed timestamp on the new file? Example : 2001-01-27_23:12:56 (YYYY-MM-DD)

Content inside the file? Example : e977656fea7ea5b9a8887ecf730860af

Example Flag : GrabCON{important_note.txt_random.exe_2001-01-27_23:12:56_e977656fea7ea5b9a8887ecf730860af}


I started by looking into all documents and I saw a file called ConsoleHost_history.txt

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/2_1_console.png" width="95%"/>
</p>

In the file we can see the following commands being executed by the user:

```
cd .\Downloads
.\timestomp.exe 'C:\Users\nick\AppData\Local\Firefox Backup\don''t open it.txt' -z "Monday 07/10/1969 6:33:33 PM"
.\timestomp.exe 'C:\Users\nick\AppData\Local\Firefox Backup\don''t open it.txt' -z "Monday 07/10/1969 6:33:33 PM"
.\timestomp.exe 'C:\Users\nick\AppData\Local\Firefox Backup\don''t open it.txt' -z "Friday 26/08/1969 7:33:33 PM"
.\timestomp.exe 'C:\Users\nick\AppData\Local\Firefox Backup\don''t open it.txt' -z "Friday 26/08/1969 7:33:33 PM"
.\timestomp.exe 'C:\Users\nick\AppData\Local\Firefox Backup\don''t open it.txt' -z "Friday 08/10/1969 7:33:33 PM"
cd .\AppData\
cd .\Local\
cd '.\Firefox Backup\'
mv '.\don''t open it.txt' '.\don''t open it.hidden'
```

That filename definitely looks suspicious, doesn't it? We have the file _don't open it.hidden_ that was a parameter for the program called _timestomp.exe_. Those could be parts of our flag. Let's try to get its content by searching for its name:

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/2_2_cont.png" width="95%"/>
</p>

We have the content: _6b751689f3cdaed05e552eff51115684_, now let's get the time 

We also have the Master File Table (MFT) in the hits for the filename, we can use them to retrieve the timestamp. For that I made a couple searches on Google and found the following [article by SANS](https://www.sans.org/blog/digital-forensics-detecting-time-stamp-manipulation/) and they were using [dkovar's MFT analyze](https://github.com/dkovar/analyzeMFT) tool:

```
python2 analyzeMFT.py -f MFT -o output.csv
```

Then I opened the output from the tool in csv and parsed it on Excel to extract the modification date _10/08/1969 16:33:33_:

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/2_3_mod.png" width="95%"/>
</p>

We can now build up the flag, replacing the spaces with underscores:

``` GrabCON{don't_open_it.hidden_timestomp.exe_1969-08-10_16:33:33_6b751689f3cdaed05e552eff51115684} ```


## Confession
497
Our Employee finally realized his wrong doings and he confessed that he was going to attack the company infrastructure from a remote computer we need you to further investigate this matter and figure out what was he doing on the remote computer.

Note : Challenge File is same as Tampered Challenge.

Since I got the Tampered challenge and I couldn't make volatility to work for the other challenges, I try myself on this one.
I kept working on Autopsy and searched for different tools for remote conections such as Teamviewer, Anydesk, etc.

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/3_1_search.png" width="95%"/>
</p>

We can see that the user has various applications downloaded and exe files from Anydesk, TeamViewer and Remote Desktop. At this time I was looking for DFIR techniques over the three apps, and I remembered a [challenge writeup](https://redpwn.net/writeups/tjctf2020/cookiemonster/) from previous year TJCTF featuring redpwn team. In there, they used volatility to extract images from the Remote Desktop cache to solve a puzzle with the bitmap (_.bmc_) files. They didn't finished the writeup but [this posts](https://cbtgeeks.com/2018/05/22/digital-forensics-on-rdp-cache/) has an outstanding explanation on it.

The cache is located in the following user path: _/img_grab2.E01/Users/nick/AppData/Local/Microsoft/Terminal Server Client/Cache/_

Now we have to parse the bmc files, for that there are tools such as [BMC-Viewer](https://github.com/0xTowel/BMC-Viewer-Backup) in Windows or [bmc-tools](https://github.com/ANSSI-FR/bmc-tools). BMC Tools is a simple python script so let's use it. I exported the Cache files to Kali and execute the tool over the directory:

```
kali@kali:~/Desktop/CTFs/GrabCON/Forensic$ python3 bmc-tools.py -s cache/ -d results/
[+++] Processing a directory...
[===] 1062 tiles successfully extracted in the end.
[===] Successfully exported 1062 files.
[===] 1052 tiles successfully extracted in the end.
[===] Successfully exported 1052 files.
[!!!] Unable to retrieve file contents; aborting.
```

Great! We have a thousand small images. I can see some traces of pastebin, and also from the flag:

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/3_2_images.png" width="95%"/>
</p>

I manage to get some of the fragments:

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/3_5_files.png" width="95%"/>
</p>

And to order them in Paint xd:

<p align="center">
  <img src="/images/writeups/GrabCON/Forensic/3_6_flag.png" width="70%"/>
</p>


``` GrabCON{RDP_Cach3_4nalysis_1s_Aw3s0me} ```

---

# Web

## E4sy Pe4sy
100
Hack admin user!
Link

A website login and a SQL inyection:

```
admin
' OR 1=1 #
```

<p align="center">
  <img src="/images/writeups/GrabCON/Web/1_0_flag.png" width="70%"/>
</p>

``` GrabCON{E4sy_pe4sy_SQL_1nj3ct10n} ```

---

# Crypto

## Warm-up
50
Mukesh used to drink and then smoke 5 times a day. He is now suffering form cancer his drink was 64 rupees and 32 rupees cigarette that costs to cheap for him. And he has this much of cancer now.

```
S01ZRENXU1NJVkhGUVZKUkpaRkZNMlNLSTVKVENWU0xLVlZYQVlLVElaTkVVVlRNS0pIRkc2U1dKVkpHV01LWEtaQ1VVVENXTlJORlNVS1dOUkdGS01EVUs1S0dXVlNLS1pDWFFUQ1ROUllFT1VUTE1STFZDTUxFSk5MR1c0Q0lLWVlEQ1RDWEtWMkZNVjJWTkJKRk1SS09MQktHV05EWktKV0U0VlNOTlJTRVdWVEtLSkxWR01CUklOTEdXNUNQS05XRkVSQ1dLWkhFSVVaUk9CTUZFMjJXS1ZKVENXU0tLWlZWVVJTVUdGTEZLVkNGR0ZIVTRSU1ZQRkdXWVRTTUtOVlRLVEtTR0ZSVEFVSlFNUkdGSVZUTUs1SkZNVExaS1pWWElWMk5OTkxFNFZUS0pKTFZJUkxRSlZKR1dNS1RLTVlEU1RDVk5OMkVRVjJXSlpLRk1WTFVKTkpUQ1VTRUtaV0U0VjJTR0JORklVU1dNUkxGTVJMVUpSSldXNkNYS0lZVlVSQ1RHQVlVT1ZDRk1STUU0VktPS0ZKVENTU0VLWldGTVZLUkdGU0U2VkxMTVJNVktNS1dNRktXV09LREtFWUZVVUNWTk4yRVFVWlJPQkdGTVJLT0s1S1RBM0NNS0pWWFFTQ1hLWlNFSVZMTE1SRFZJMjIySlJMR1dUU0hLSVlWVVMyV05OU0ZHVTMySkpGRksyM0VLVklWTVVTVUtSS1hJVDJXR0JORk1WTDJKSk1GRU1DMktOTEVLUlNXS05XRU1VQ1JOTlNFT1ZEMkpGNUZPVkxMR0ZKVEFXU09LWVlVNFJTVEdGWUVJVlJRTlJKRTIyWlZKNUtHV05LR0tWTEZVVkNXTk00VUdVM0xMSkZGSzIzWUtKSldXU1NMS0lZV0dNS1JHRkpFWVZMTEpaTVZLVlNPSlJLVEE1Q1NLNUtWRVZDV0tWSEZNVVJRSVVZRk0yMkdLWkpXVVZTSktaS0RBT0tRS1FZRFNVQ1JIVTZRPT09PQ==
```

As showed in the challenge description, we can guess that it's (base64 + base32) * 5

``` GrabCON{dayuum_s0n!} ```


Cool CTF! I didn't have all the time I wanted to spend on it and also had issues with the volatility3 that drove me crazy. Thanks to my team and specially [RazviOverflow](https://razvioverflow.github.io/) who throwed some light on the OSINT challs ;)

Thanks for reading!