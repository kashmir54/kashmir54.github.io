---
title: "CTF - NACTF2020"
categories: [ctf]
date: 2020-10-16
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: CTF - NACTF
---

# NACTF 2020

<p align="center">
  <img src="/images/writeups/NACTF/1_logo.jpeg" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges:

<p align="center">
  <img src="/images/writeups/NACTF/2_rank.png" width="500"/>
</p>


Interesting CTF, with entry level challenges on web and forensic (more like stego) and other hard ones, specially some programming ones that I tried. Let's get into it!

---

# Forensic

## Gummies
50

Kylie is obsessed with gummies. With her collection of miscellaneous gummy bears, she took this incredible picture which is now her phone's wallpaper. Can you find her flag?

A simple _zsteg -a_ will retrieve the flag:

<p align="center">
  <img src="/images/writeups/NACTF/Forensic/1_flag.png" width="500"/>
</p>

``` nactf{5t3gan0graphy_rul35!} ```


## Meta-morphosis
75

Mikey really likes Metamorphosis by Franz Kafka, so much so that he sent this meme to the class.

Check _exiftools_
```
kali@kali:~/Desktop/CTFs/NACTF/Forensic/Meta$ exiftool meme-3.jpg 
ExifTool Version Number         : 12.06
File Name                       : meme-3.jpg
Directory                       : .
File Size                       : 52 kB
File Modification Date/Time     : 2020:10:30 20:52:48-04:00
File Access Date/Time           : 2020:10:30 20:53:00-04:00
File Inode Change Date/Time     : 2020:10:30 20:53:00-04:00
File Permissions                : rw-r--r--
File Type                       : JPEG
File Type Extension             : jpg
MIME Type                       : image/jpeg
JFIF Version                    : 1.01
X Resolution                    : 1
Y Resolution                    : 1
Exif Byte Order                 : Big-endian (Motorola, MM)
Resolution Unit                 : None
Artist                          : nactf{m3ta_m3ta_m3ta_d3f4j}
Y Cb Cr Positioning             : Centered
Image Width                     : 500
Image Height                    : 461
Encoding Process                : Baseline DCT, Huffman coding
```

``` nactf{m3ta_m3ta_m3ta_d3f4j} ```


##  Turnips
100

Dr. J loves his ch0nky turnips, can you find his ch0nky flag?


Seems like a broken image... Let's fix it with hexedit:

Ahhhh... Nope, but there is like some kind of strange pattern on the file... Let's go for strings to see any hint:

```
kali@kali:~/Desktop/CTFs/NACTF/Forensic/Turnips$ strings turnip-for-what.jpg 
...
m4~9
Y@?X(
`."7
9#W.
5};D
x<4<?
kmS[
|6z}
byu;k
nactf{turn1p_f0r_h3x_f3j52}
```
Wops, got the flag:

``` nactf{turn1p_f0r_h3x_f3j52} ```


##  Secret Message
100

Monica loves inventing secret languages. So much so that she claims to be the only one to know the message in this recording. What does it say?

We got an mp3 file. I played it and it's morse, let's get it:

```
ffmpeg -i input.mp3 output.wav 
kali@kali:~/Desktop/CTFs/NACTF/Forensic/SecretMessage$ morse2ascii -m output.wav 

MORSE2ASCII 0.2
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org

- open output.wav
  wave size      1200478
  format tag     1
  channels:      1
  samples/sec:   48000
  avg/bytes/sec: 96000
  block align:   2
  bits:          16
  samples:       600239
  bias adjust:   -191
  volume peaks:  -30194 30194
  normalize:     2573
  resampling to  8000hz

- decoded morse data:
--.-..-...--...---./-----..-./.-......--.--...-....---....--...
```

This time I couldn't get the morse right with the _morse2ascii_, so let's head over to [Morsecode](https://morsecode.world/international/decoder/audio-decoder-adaptive.html):

<p align="center">
  <img src="/images/writeups/NACTF/SecretMessage/1_flag.png" width="500"/>
</p>

``` nactf{QU33N_0F_L4NGU4G3S} ```

##  Turnips 2
150

Uh oh.. Parth's file seems to have been corrupted. Can you figure out how to find his flag?

I run a bunch of commands over the file.txt but this was the final check. Look over with xxd and find some interesting fragments:

```
.lvG........I9D0
.......}.....Oi.
....WiCCPICC Pro
file..X..y.TT]..
...0...%........
..*).*.. ......!
b!.("...H...*(..
.....y..}.;k.{..
```

I looked in google de _WiCCPICC Profile_ and [checked](https://www.google.com/search?client=firefox-b-e&ei=AL-cX5S4MtL4aLPKiKgG&q=wiccpicc+profile&oq=wiccpicc+profile&gs_lcp=CgZwc3ktYWIQAzoHCAAQsAMQE1DiL1jiL2D7MWgAcAB4AIABb4gByAGSAQMxLjGYAQCgAQGqAQdnd3Mtd2l6yAECwAEB&sclient=psy-ab&ved=0ahUKEwiUuOmL2N3sAhVSPBoKHTMlAmUQ4dUDCAw&uact=5) that is something related with a PNG IHDR file, therefore I hexedit the file and place the correct magic bytes and headers according to the [specification](https://parsiya.net/blog/2018-02-25-extracting-png-chunks-with-go/):

```
00000000   89 50 4E 47  0D 0A 1A 0A  .PNG....
00000008   00 00 00 0D  49 48 44 52  ....IHDR
00000010   00 00 04 A3  00 00 03 7D  .......}
00000018   08 06 00 00  00 4F 69 BA  .....Oi.
00000020   C0 00 00 18  57 69 43 43  ....WiCC
00000028   50 49 43 43  20 50 72 6F  PICC Pro
00000030   66 69 6C 65  00 00 58 85  file..X.
```

<p align="center">
  <img src="/images/writeups/NACTF/Turnips2/file" width="500"/>
</p>

``` nactf{th3_turn1p5_ar3_tak17g_0v35_skf9} ```


##  Static
250

Juliet just airdropped me this really wierd photo that looks like tv static. She said this would be easier than passing notes in class, but I can't understand what she's trying to say. Help!


Rip like 5 hours... I was close, but you never know.

---

# Web

##  Login
175

Vyom has learned his lesson about client side authentication and his soggy croutons will now be protected. There's no way you can log in to this secure portal!

https://login.challenges.nactf.com/

Let's try some SQL Injections:

<p align="center">
  <img src="/images/writeups/NACTF/Web/1_sql.png" width="500"/>
</p>

``` nactf{sQllllllll_1m5qpr8x} ```


##  Cookie Recipe
150

Arjun owns a cookie shop serving warm, delicious, oven-baked cookies. He sent me his ages-old family recipe dating back four generations through this link, but, for some reason, I can't get the recipe. Only cookie lovers are allowed!


We can see a cookie into the page. Let's log in with any username:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/1_path.png" width="500"/>
</p>

Ok, no admin works, so let's try to change the cookie for the auth.php path:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/2_log.png" width="500"/>
</p>

Reload the website and there we go:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/3_flag.png" width="500"/>
</p>

``` nactf{c00kie_m0nst3r_5bxr16o0z} ```


---

# Crypto

##  Caesar's Challenge
25

Zabelo wrote this message on a note he passed to me. anpgs{q3p1cu3e1at_e0px5!} He also told me his favorite number was 13. What could this mean?

Ez ROT13: ``` nactf{d3c1ph3r1ng_r0ck5!} ```


##  YAMS
100

Instead of turnips, Yavan loves YAMS. Day and night, he sings about YAMS, dreams about YAMS and runs to the store to catch the newest released batch of YAMS. Hes cryptic too. I wonder what this could mean.

Uexummq lm Vuycnqjc. Hqjc ie qmud xjas: fycfx{waY5_sp3_Y0yEw_w9vU91}

YAMS.... Is YAMS a key? Vigenere?

Welcome to Vigenere. Here is your flag: ``` nactf{yaM5_ar3_Y0mMy_w9jC91} ```



##  Oligar's Tricky RSA
175

The crypto master Oligar just sent this file with three numbers. What do they mean?

I got the file with RSA parameters:

```
c = 97938185189891786003246616098659465874822119719049
e = 65537
n = 196284284267878746604991616360941270430332504451383
```

For this I have a little roadmap with [RsaCtfTool](https://github.com/Ganapati/RsaCtfTool). Find it down below: 


```
kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --createpub -n 196284284267878746604991616360941270430332504451383 -e 65537 > ~/Desktop/CTFs/NACTF/Crypto/RSA/pub.key

kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --publickey ~/Desktop/CTFs/NACTF/Crypto/RSA/pub.key --private > ~/Desktop/CTFs/NACTF/Crypto/RSA/key

[*] Performing factordb attack on /home/kali/Desktop/CTFs/NACTF/Crypto/RSA/pub.key.

Results for /home/kali/Desktop/CTFs/NACTF/Crypto/RSA/pub.key:

Private key :
-----BEGIN RSA PRIVATE KEY-----
MHgCAQACFgCGTZu3ODf6xVEntw2V1WILWDz/KTcCAwEAAQIVJ4WzK0nZ/2wNo9Es
si23/pijS3ihAgsIev/y8NsVH+p3vwILD9Y2EXS3Fv4f7IkCCwexQq/kyGVuEOnT
AgsONFZepc8qwlij8QILCB0K8fjbLQ1JbCE=
-----END RSA PRIVATE KEY-----


kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --dumpkey --key  ~/Desktop/CTFs/NACTF/Crypto/RSA/key         
private argument is not set, the private key will not be displayed, even if recovered.                        
n: 196284284267878746604991616360941270430332504451383  
e: 65537                                              
d: 57761855232773998001493320508606360547602423576737 
p: 10252256693298561414756287                   
q: 19145471103565027335990409
```

Now use this script developed over other CTFs:

{% highlight python %}
#!/usr/bin/env python3

import codecs

e = 65537
n = 196284284267878746604991616360941270430332504451383
d = 57761855232773998001493320508606360547602423576737 

msg = 97938185189891786003246616098659465874822119719049
flag = []

hexa = str(hex(pow(msg,d,n)))[2:]
print(hexa)

flag = codecs.decode(hexa, 'hex').decode('utf-8')
print(flag)
{% endhighlight %}


We can retrieve the flag:

```
kali@kali:~/Desktop/CTFs/NACTF/Crypto/RSA$ python3 text.py 
6e616374667b736e33616b795f6331706833727d0a
nactf{sn3aky_c1ph3r}
```

``` nactf{sn3aky_c1ph3r} ```
