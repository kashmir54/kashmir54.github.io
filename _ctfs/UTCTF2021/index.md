---
title: "CTF - UTCTF 2021"
categories: [ctf]
date: 2021-02-20
comments: true
image: /images/writeups/UTCTF2021/logo.png
description: CTF - UTCTF 2021
---

# UTCTF 2021

<p align="center">
  <img src="/images/writeups/UTCTF2021/logo.png" width="20%"/>
</p>


I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges!


<p align="center">
  <img src="/images/writeups/UTCTF2021/rank.png" width="70%"/>
</p>

---

Challenge index:

### Beginner

- [HTML](#html)
- [Stringy Things](#stringy-things)
- [Magic Bytes](#magic-bytes)
- [Run-ELF](#run-elf)
- [Cipher Gauntlet](#cipher-gauntlet)
- [Sizzling Bacon](#sizzling-bacon)
- [Various Vernacular](#various-vernacular)

### Web

- [Source it!](#source-it)
- [Cutest Cookie Clicker Rip-Off](#cutest-cookie-clicker-rip-off)
- [Oinker](#oinker)

### Forensic

- [SHIFT](#shift)
- [Doubly Deleted Data](#doubly-deleted-data)
- [OSINT Part 1](#osint-part-1)
- [OSINT Part 2](#osint-part-1)

### Misc

- [Emoji Encryption](#emoji-encryption)


---

# Beginner

## HTML
814
I hid a flag somewhere in the code of the UTCTF website, can you find it?

``` utflag{you_found_me_0123959} ```


## Stringy Things
850
I know there's a string in this binary somewhere.... Now where did I leave it?

A basic strings on the binary file will show the flag

```
strings calc
```

``` utflag{strings_is_op} ```


## Magic Bytes
859

My friend sent me this text file and said they put a flag inside. I opened it and it doesn't look like a normal text file to me! Maybe you'll be able to find it.

With xxd we can see that is a PNG file:

```
kali@kali:~/Desktop/CTFs/UTCTF2021/Beginner$ xxd out.txt | head
00000000: 8950 4e47 0d0a 1a0a 0000 000d 4948 4452  .PNG........IHDR
00000010: 0000 0780 0000 0438 0806 0000 00e8 d3c1  .......8........
00000020: 4300 0001 8369 4343 5049 4343 2070 726f  C....iCCPICC pro
```

Open it as image:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Beginner/out.png" width="50%"/>
</p>

``` utflag{file_extensions_mean_nothing} ``` 


## Run-ELF
859

Anyone know how to run an ELF file? I bet you could figure it out.

```
kali@kali:~/Desktop/CTFs/UTCTF2021/Beginner$ chmod +x run
kali@kali:~/Desktop/CTFs/UTCTF2021/Beginner$ ./run 
```
``` utflag{run_run_binary_9312854} ```


## Cipher Gauntlet
867

Can you make it through all of the encodings and ciphers

From the given txt we can see binary code. 

We translated it to ascii and we got:

```
Uh-oh, looks like we have another block of text, with some sort of special encoding. Can you figure out what this encoding is? (hint: if you look carefully, you'll notice that there only characters present are A-Z, a-z, 0-9, and sometimes / and +. See if you can find an encoding that looks like this one.)
TmV3IGNoYWxsZW5nZSEgQ2FuIHlvdSBmaWd1cmUgb3V0IHdoYXQncyBnb2luZyBvbiBoZXJlPyBJdCBsb29rcyBsaWtlIHRoZSBsZXR0ZXJzIGFyZSBzaGlmdGVkIGJ5IHNvbWUgY29uc3RhbnQuIChoaW50OiB5b3UgbWlnaHQgd2FudCB0byBzdGFydCBsb29raW5nIHVwIFJvbWFuIHBlb3BsZSkuCm15eHFia2Rldmtkc3l4YyEgaXllIHJrZm8gcHN4c2Nyb24gZHJvIGxvcXN4eG9iIG1iaXpkeXFia3pyaSBtcmt2dm94cW8uIHJvYm8gc2MgayBwdmtxIHB5YiBrdnYgaXllYiBya2JuIG9wcHliZGM6IGVkcHZrcXt4eWdfaXllYm9fenZraXN4cV9nc2RyX21iaXpkeX0uIGl5ZSBnc3Z2IHBzeG4gZHJrZCBrIHZ5ZCB5cCBtYml6ZHlxYmt6cmkgc2MgbGVzdm5zeHEgeXBwIGRyc2MgY3liZCB5cCBsa2NzbSB1eHlndm9ucW8sIGt4biBzZCBib2t2dmkgc2MgeHlkIGN5IGxrbiBrcGRvYiBrdnYuIHJ5em8gaXllIG94dHlpb24gZHJvIG1ya3Z2b3hxbyE=
```

Second part seems like base64:

```
New challenge! Can you figure out what's going on here? It looks like the letters are shifted by some constant. (hint: you might want to start looking up Roman people).
myxqbkdevkdsyxc! iye rkfo psxscron dro loqsxxob mbizdyqbkzri mrkvvoxqo. robo sc k pvkq pyb kvv iyeb rkbn oppybdc: edpvkq{xyg_iyebo_zvkisxq_gsdr_mbizdy}. iye gsvv psxn drkd k vyd yp mbizdyqbkzri sc lesvnsxq ypp drsc cybd yp lkcsm uxygvonqo, kxn sd bokvvi sc xyd cy lkn kpdob kvv. ryzo iye oxtyion dro mrkvvoxqo!
```

Use a ROT13 (or caesar cipher) to get the flag:

``` utflag{now_youre_playing_with_crypto} ```


## Sizzling Bacon
913

My buddy Francis is really into Bacon. He loves it so much that he gave me this encoded bacon-themed flag (he said he was inspired by the sound of sizzling bacon).

We have this string:

```
sSsSSsSSssSSsSsSsSssSSSSSSSssS{SSSsSsSSSsSsSSSsSSsSSssssssSSSSSSSsSSSSSSSSsSSsssSSssSsSSSsSSsSSSSssssSSsssSSsSSsSSSs}
```

Translate it to binary, s is 1 and S is 0 for example:

```
10100 10011 00101 01011 00000 00110 { 00010 10001 01000 10010 
01111 11000 00001 00000 00010 01110 01101 00010 01000 01111 
00111 00100 10001 }
```

2^5 = 32 ... Maybe they represent the 26 alphabe, fromb binary to numbers:

```
20 19 5 11 0 6 { 2 17 8 18 15 24 1 0 2 14 13 2 8 15 7 4 17 }
```

Alphabet correlation:

``` utflag{crispybaconcipher} ```


## Various Vernacular
988

This flag was intercepted. wmysau{foeim_Tfusoli}

Unfortunately, it seems to be encrypted. Additional encrypted text was also found.
Hkgxologflutleiaymt xgf Azutgkrftmtf ltmntf ERW wfr ELW wfmtk Rkweq.

I used [quipquip](https://quipqiup.com/) for the puzzle using this clue ```wmysau=utflag```.

``` utflag{nicht_English} ```


---

# Web

## Source it!
793

Can you see how this page handles authentication?

http://web1.utctf.live:8778

We can see that the credentials are hashed and embedded into the JS code:

{% highlight javascript %}

function checkPassword(form) { 
    password1 = form.password1.value; 
    name = form.name.value;
    var username = "admin";
    var hash = "1bea3a3d4bc3be1149a75b33fb8d82bc"; 
    var hashedPasswd = CryptoJS.MD5(password1);

    if (password1 == '') 
        alert ("Please enter Password"); 
  
    else if (username != name) { 
        alert ("\nYou lack access privlages...") 
        return false; 
    }
         
    else if (hash != hashedPasswd) { 
        alert ("\nIncorrect password...") 
        return false; 
    } 

    else{ 
        alert("Access Granted\n" + text) 
        return true; 
    } 
} 

{% endhighlight %}

I used [crackstation](https://crackstation.net/) to "reverse" the hash:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Web/1_crack.png" width="70%"/>
</p>

After introducing the pass:

``` utflag{b33n_th3r3_s0uRc3d_th4t} ```


## Cutest Cookie Clicker Rip-Off
835

I built this awesome game based off of cookie clicker! Bet you'll never beat my high score. Hehehe!

http://web1.utctf.live:4270

Set the cookie to a high number and wait for the timer:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Web/2_flag.png" width="80%"/>
</p>

``` utflag{numnum_cookies_r_yumyum} ```


## Oinker
879

I found this cool more private alternative to twitter.

http://web2.utctf.live:5320/

We can post a "tweet" and see that there is an id on the URL. Looking for different messages and ids on the URL, we can see that the second one was the flag:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Web/3_flag.png" width="70%"/>
</p>

``` utflag{traversal_bad_dude}  ```


---

# Forensic

## SHIFT
967

I just tried to download this flag, but it looks like the image got messed up in transit...

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/SHIFT.png" width="80%"/>
</p>

We can take a closer look and conclude that each row is shifted 6 pixels to the left.

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/shifted.png" width="40%"/>
</p>

I have implemented the following script to unshift the image:

{% highlight python %}
from PIL import Image

im = Image.open('SHIFT.png','r')
im2 = Image.open('SHIFT2.png','r')

width, height = im.size

pixels = im.load()
pixels2 = im2.load()

for y in range(0, height):
    for x in range(0, width):
        pixels2[x,y] = pixels[(x+(height-y)*6)%width, y]

im2.show()
im2.save('SHIFT2.png')
{% endhighlight %}

Running the script we obtain the output image:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/SHIFT2.png" width="80%"/>
</p>

``` utflag{not_when_i_shift_into_maximum_overdrive} ```


## Doubly Deleted Data
978

We got a copy of an elusive hacker's home partition and gave it to someone back in HQ to analyze for us. We think the hacker deleted the file with the flag, but before our agent could find it, they accidentally deleted the copy of the partition! Now we'll never know what that flag was. :(

We get an .img file. Let's open it on Autopsy to analyze it. We can see a txt called plan:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/plan.png" width="70%"/>
</p>

Then on the deleted files we can see one with the flag:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/flag.png" width="70%"/>
</p>

``` utflag{d@t@_never_dis@ppe@rs} ```


## OSINT Part 1
976

Can you find any OpSec vulnerabilities?

'http://misc.utctf.live:8756/'

We can see the description of the last member saying that he is on social media, so let's find him out:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/1_osint.png" width="70%"/>
</p>

I found him on twitter with his name and one of the photos contained the flag:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/1_flag.png" width="70%"/>
</p>

``` utflag{g0t_y0ur_b4dg3} ```


## OSINT Part 2
983

Find the origins of the linked file found in part 1.

We have the following image:

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/2_image.jpg" width="70%"/>
</p>

Using Google Lens we can see that is a theme park on Shiroshi City, Miyagi, Japan.
There was nothing on GMaps so let's do a reverse search on Google.
Finally I found that photo in this [blog](https://tobiasmaier.blogspot.com/2021/03/blog-post_6.html):

<p align="center">
  <img src="/images/writeups/UTCTF2021/Forensic/2_flag.png" width="60%"/>
</p>

``` utflag{r3v3rs3d_t0_0r1g1nal} ```

---

# Misc

## Emoji Encryption
100

I came up with this rad new encryption. Bet no one can break it

☂️🦃🔥🦁🍎🎸{🐘🥭🧅🤹🧊☀️\_💣🐘\_🌋🐘🌈☀️🍎🦃🧊🦁🐘}

Use the first letter of what th emoji represents

``` utflag{emojis_be_versatile} ```

---

And that's all for this CTF! Thanks for reading!
