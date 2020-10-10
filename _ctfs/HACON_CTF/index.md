---
title: "CTF - HACON_CTF"
categories: [ctf]
date: 2020-09-03
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: CTF - HACON_CTF
---

# HACON_CTF

<p align="left">
  <img src="/images/writeups/HACON_CTF/1_logo.png" width="100"/>
</p>

I participate in this CTF for my team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges:

- [EAsy WEb](#easy-web)
- [Robo Returns]
- [MISC 2]
- [MISC 3]
- [Sn0w M4N]
- [BEEP]
- [Wizard]
- [the_matrix]

<p align="left">
  <img src="/images/writeups/HACON_CTF/2_rank.png" width="600"/>
</p>

Let's get into it!

---

# Web

## EAsy WEb
70

Go on, step by step,
And you have the flag!!

https://murmuring-caverns-26740.herokuapp.com/

Flag Format HACSEC{...}


By visiting the website we can check a login form. If we submit it empty it shows a hint:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Web/easy_web/1_web.png" width="600"/>
</p>

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImtleW1hc3RlciIsInBhc3N3b3JkIjoic2VjcmV0a2V5In0.oJOVw-DOUGSNjGDRWe5_kZm3MAFq_Y9YZa0QyXDbvlM
```

It seems like a JSON web token ([JWT](https://jwt.io/)). Let's decode it:

```
{
	"alg":"HS256",
	"typ":"JWT"
}{
	"username":"keymaster",
	"password":"secretkey"
}
($åp.å.HØÆ
..æFfÜÀ.©.Xe..ÉpÛ¾S
```

Entering that username and password we get:

```
here's your next hint -> syntsvyr
```

Having some fun ideas I came across [ROT13](https://www.dcode.fr/caesar-cipher):

```
syntsvyr -> ROT13 -> flagfile
```

Ok... Visiting _https://murmuring-caverns-26740.herokuapp.com/flagfile_ it shows a text. I search for the start of the flag (HACSEC) and find it in the same color as the background:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Web/easy_web/2_flag.png" width="500"/>
</p>

```flag: HACSEC{Y0u_g0T_I7}```


## Robo Returns
75

A spy of China has been arrested from India yesterday..
While he was taking to the jail he some how ran away..
We have his photo but his name is not being known by any of the person..
From our sources, we have come to know that the name of the person is in this webpage...
You are a CBI agent and you have to find the name..

http://web.hac-security.com/

Flag Format HACSEC{...}


We have an Amazon like website...?

<p align="center">
  <img src="/images/writeups/HACON_CTF/Web/robo_returns/1_web.png" width="500"/>
</p>

I found a comment with this but it's not base64...

```whfh-HprSE8wvsRZA+A+hbz3z8vR9ETHCpoTm+R4ZOgm0O3/LwVwzHD25/A== rid-Q6GBNFEGMMP73XFA9PCP```

Stucked.... Robo like robots.txt?

```
User-agent: *
Disallow: */s?k=*&rh=n*p_*p_*p_
Disallow: /dp/product-availability/
Disallow: /dp/rate-this-item/
Disallow: /exec/obidos/account-access-login
Disallow:/exec/obidos/hac-sec/johnwick/admin-login/
User-agent: AmazonAdBot
Disallow:*
```
We can check out the name **johnwick**

``` flag: HACSEC{johnwick} ```


## D4m4ged C4lc
80

This calculator does more than javascript operation
Check out for errors

https://hidden-journey-18965.herokuapp.com/

Flag Format HACSEC{...}


{% highlight javascript %}
document.cookie = "username=dev";
	
function myfunc() {
	var a = document.getElementById('first').value;
	var b = document.getElementById('second').value;

	document.getElementById('plus').addEventListener("click", function() {
		var c = a+b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page1",data)
			.done(function(result) {
				//error1
			})
	})

	document.getElementById('minus').addEventListener("click", function() {
		var c = a-b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page2",data)
			.done(function(result) {
				//error2
			})
	})

	document.getElementById('multiply').addEventListener("click", function() {
		var c = a*b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page3",data)
			.done(function(result) {
				//error3
			})
	})

	document.getElementById('divide').addEventListener("click", function() {
		var c = a/b;
		document.getElementById('result').innerHTML = c;
		let data = {
			value: c
		}
		$.post("/page4",data)
			.done(function(result) {
				//error4
			})
	})	
}	
{% endhighlight %}

I didn't get anything in a certain time, so I skipped, I will come back to it.

---

# Network

## BAsic
50

What is the port used by IPPS ?

Flag Format HACSEC{portnumber}


``` flag: HACSEC{631}```

---

# Misc

## MISC 2
50

Very Easy .....Basic skills and you got it.

Flag Format HACSEC{string}


We had a zip file, I used these commands:

```
kali@kali:~/Desktop/CTFs/HACON_CTF/Misc/misc2$ binwalk -e zippppp.zip

kali@kali:~/Desktop/CTFs/HACON_CTF/Misc/misc2/_zippppp.zip.extracted/zippppp$ grep '' *
flag237:HACSEC{GoTCHA-AgAiN}
flag324:HACSEC{BetterLuck next time}
flag430:HACSEC{Youare too noob}
flag532:HACSEC{RookiesHEre--Huh!}
flag68:HACSEC{GOTCHA!}
flag710:HACON{naoo}
flag721:HACSEC{NOPESs}
flag733:Hacon{notyour flag}
flag757:HACSEC{z1ppppitnice}
flag907:HACSEC{Flag is not here}
```

``` flag: HACSEC{z1ppppitnice}```


## MISC 3
75

Zips and Your skills , easy enough ....huh!

Flag Format HACSEC{string}


Commands:

```
$ binwalk -e zipv2.zip 
$ strings zipv2.zip > text.txt
$ grep 'HACSEC' text.txt

$ grep 'HACSEC' text.txt 
HACSEC{Z1ppv2}
...
```

``` flag: HACSEC{Z1ppv2}```

---

# Forensic

## Sn0w M4N
50

Who doesnt        love to build
              a snow man during
              winter.

Flag Format HACSEC{strings}
Note Words are separated by underscore()


We can find out some whitespace stego on the txt file:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/snow_man/1_stego.png" width="500"/>
</p>

We can use a tool called [stegsnow](http://manpages.ubuntu.com/manpages/bionic/man1/stegsnow.1.html):

```
$ stegsnow -C chall.txt
HACSEC{s7eg_5n0W_F7W}
```

``` flag: HACSEC{s7eg_5n0W_F7W}```


## BEEP
75

Whats that noise, does it sound familiar?

Flag Format HACSEC{string}
Note Words separated by underscore()


It's a wav audio, so we will use a tool called morse2ascii:

```
morse2ascii -m audio.wav 
..../.-/-.-./..././-.-./--/-----/.-./.../...--/-.-./-----/-../...--/.----/.../.--/....-/-.--/-/-----/---/-.-./---/-./...-/...--/-./--.../../---/-./....-/.-../ 
```

Now we can introduce it into the [CyberChef](https://gchq.github.io/CyberChef/) website and get the flag:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/beep/1_flag.png" width="500"/>
</p>

Check the hint to get it right with the underscores: 1st 2 words are a single word ;P

``` flag: HACSEC{M0RS3C0D3_1S_W4Y_T0O_CONV3N7ION4L} ```


## Wizard
75

Wizard has the magical powers to bring people from the dead.
They use a very special spell, figure it out.

Flag Format HACSEC{strings}
Note Words are separated by underscore()


First we can see a png file that cannot be read. We can see that there is something going on with the magic numbers:

```
$ file wizard.png 
wizard.png: data

$ xxd wizard.png | head
00000000: 00aa 4e47 0d0a 1a0a 0000 000d 4948 4452  ..NG........IHDR
00000010: 0000 0780 0000 0438 0802 0000 0067 b156  .......8.....g.V
00000020: 1400 0000 0970 4859 7300 000e c400 000e  .....pHYs.......
00000030: c401 952b 0e1b 0000 2000 4944 4154 78da  ...+.... .IDATx.
00000040: ecbd d9af 24c9 95e6 7716 3377 8ff5 2e79  ....$...w.3w...y
```

Lets fix it, use the expected magic bytes for the PNG file: _80 50 4e 47_

```
hexedit wizard.png
```

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/wizard/2_hexedit.png" width="500"/>
</p>

And now check the flag in the image:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/wizard/3_flag.png" width="500"/>
</p>

``` flag: HACSEC{w1Z4rD_u53_M4giC_byT3S} ```


## the_matrix

We got an image:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/the_matrix/the_matrix.png" width="500"/>
</p>


Let's analyze it with binwalk:

```
kali@kali:HACON_CTF/Forensic/the_matrix$ binwalk -e the_matrix.png 

kali@kali:HACON_CTF/Forensic/the_matrix/_the_matrix.png.extracted$ ls
4F8CBA.zip  83  83.zlib  D3D4F.zip  world.zip
```

We can see a zip called world.zip, seems to be locked with a password, let's crack it with a wordlist:

```
kali@kali:HACON_CTF/Forensic/the_matrix/_the_matrix.png.extracted$ fcrackzip -u -D -p /usr/share/wordlists/rockyou.txt world.zip 


PASSWORD FOUND!!!!: pw == steven

kali@kali:HACON_CTF/Forensic/the_matrix/_the_matrix.png.extracted$ unzip world.zip 
Archive:  world.zip
[world.zip] captured_message.pcapng password: 
  inflating: captured_message.pcapng  
  inflating: take_me.cpython-37.pyc 
```

A pcap file showed up, let's take a look on Wireshark:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/the_matrix/1_wire.png" width="500"/>
</p>

An HTTP POST is sent and we can see this data:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/the_matrix/2_ana.png" width="500"/>
</p>

```
"I think you know what to do with this Neo ->LedbRKTnteMyLeNnAYzdueMfuqT5t3NqAYRkQOuyKWXVw3covOXqRPJgROFgEZFdtbTmRP0(hml wimsd kayf afklwsv gx tjsucwlk sfv lzak lwpl, sfv lzwf vg kgewlzafy oalz al)"
```

The last part seems like rot. Not 13 but I check other values until reaching 34:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/the_matrix/3_rot.png" width="500"/>
</p>

```
TmljZSBvbmUgTmVvIGhlcmUncyB5b3VyIGZsYWcgSEFDe3kwdWFyZXRoZWNoMHNlbjBuZX0(put equal sign instead of brackets and this text, and then do something with it)
```

Let's go what it says, but an equal could be some base64. Before putting the equal I could see the message and the flag:

<p align="center">
  <img src="/images/writeups/HACON_CTF/Forensic/the_matrix/4_flag.png" width="500"/>
</p>

```flag: HAC{y0uarethech0sen0ne}```

Thanks for reading!!