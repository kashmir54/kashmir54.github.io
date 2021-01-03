---
title: "CTF - Brixel CTF 2020"
categories: [ctf]
date: 2020-10-16
comments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: CTF - BrixelCTF 2020
---

# Brixel CTF 2020

<p align="center">
  <img src="/images/writeups/Brixel/brixel_logo.jpeg" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) with my mate [RazviOverflow](https://ctftime.org/user/72894) and finished the complete CTF for our first time! 

<p align="center">
  <img src="/images/writeups/Brixel/brixel_rank.png" width="500"/>
</p>


This was an entry level CTF with challenges I really liked. This is the index:


- [Internet/Web](#internet)

- [Easy](#easy)
- [Hidden Code](#hidden-code)
- [robotopia](#robotopia)
- [Discord](#discord)
- [login1](#login1)
- [login2](#login2)
- [login3](#login3)
- [login4](#login4)
- [Browsercheck](#browsercheck)
- [SnackShack awards](#snackshack-awards)
- [Flat earth](#flat-earth)
- [Dadjokes](#dadjokes)
- [Pathfinders 1](#pathfinders-1)
- [Pathfinders 2](#pathfinders-2)


- [Crypto](#crypto)

- [Sea code](#sea-code)
- [Merde](#merde)
- [Merda](#merda)
- [shit](#shit)
- [Scheibe](#scheibe)
- [flawed](#flawed)
- [Dont be salty](#dont-be-salty)


- [OSINT](#osint)

- [A quick search](#a-quick-search)
- [Manhunt1](#manhunt1)
- [Manhunt2](#Manhunt2)
- [Manhunt3](#manhunt3)
- [Manhunt4](#manhunt4)
- [Manhunt5](#manhunt5)
- [Manhunt6](#manhunt6)
- [Manhunt7](#manhunt7)
- [Manhunt8](#manhunt8)
- [Manhunt9](#manhunt9)
- [Physical pentest setup](#physical-pentest-setup)
- [Visit Limburg 1](#visit-Limburg-1)
- [Visit Limburg 2](#visit-Limburg-2)
- [Visit Limburg 3](#visit-Limburg-3)
- [Bird call](#bird-call)


- [Forensic](#forensic)

- [A message from space](#a-message-from-space)
- [Lottery ticket](#lottery-ticket)
- [Lost Evidence](#lost-evidence)


- [Programming](#programming)

- [Keep walking](#keep-walking)
- [A song](#a-song)
- [An arduino project](#an-arduino-project)


- [Old Tech](#old-tech)

- [Goodbye old friend](#goodbye-old-friend)
- [The tape](#the-tape)


---

# Internet

##  Easy
5

On the homepage there is a hidden flag. It's a Source of easy points!

The flag was on the index page, CTRL+F to look for the work flag will do the work:

<p align="center">
  <img src="/images/writeups/Brixel/Web/1_easy.png" width="500"/>
</p>

``` brixelCTF{notsosecret} ```


##  Hidden Code
5

Something strange happens on the brixel website when you enter the konami code

flag = the character you see floating by

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


Google Fu shows that the konami code is a set of keys introduced on konami games to so cheat or something. 

<p align="center">
  <img src="/images/writeups/Brixel/Web/2_konami.png" width="500"/>
</p>

Entering that secuence of keys on the webside shows Mario running from left to right.

<p align="center">
  <img src="/images/writeups/Brixel/Web/2_mario.png" width="500"/>
</p>

``` mario ```


##  robotopia
5

I found this cool website, it claims to be 100% robot-free!

There's nothing there yet at the moment, but at least it's robots-free. I wonder how they keep it that way?

http://timesink.be/robotopia/


Robots.... Something like robots.txt

http://timesink.be/robotopia/robots.txt

<p align="center">
  <img src="/images/writeups/Brixel/Web/3_robots.png" width="500"/>
</p>

``` brixelCTF{sadr0b0tz} ```


##  Discord
5

Join our Discord and read the rules.


So, join discord and on the rules we can see the flag:

<p align="center">
  <img src="/images/writeups/Brixel/Web/4_discord.png" width="500"/>
</p>


```  brixelCTF{th4nk5_f0r_r34d1ng_th3_rulz} ```


##  login1
5

My buddy is trying to become a web developer, he made this little login page. Can you get the password?

http://timesink.be/login1/index.html


On the website, we inspect on the HTML code and we can see a JS function with the login and the hardcoded password:

``` brixelCTF{w0rst_j4v4scr1pt_3v3r!} ```


##  login2
5

Cool, you found the first password! He secured it more, could you try again?

We can find another login and a verify function with the following code:

{% highlight javascript %}

function verify() {
	password = document.getElementById("the_password").value;
	split = 6;
	if (password.substring(0, split) == 'brixel') 
	{
		if (password.substring(split*6, split*7) == '180790') 
		{
			if (password.substring(split, split*2) == 'CTF{st') 
			{
				if (password.substring(split*4, split*5) == '5cr1pt') 
				{
					if (password.substring(split*3, split*4) == 'd_j4v4') 
					{
						if (password.substring(split*5, split*6) == '_h3r3.') 
						{
							if (password.substring(split*2, split*3) == '1ll_b4') 
							{
								if (password.substring(split*7, split*8) == '54270}') 
								{
									alert("Password Verified")
								}
							}
						}
					}
				}
			}
		}
	}
	else 
	{
	alert("Incorrect password");
	}
}

{ % endhighlight % }

I reversed the code, just assembling the strings on the right side in the order of the splits:
(0, split) -> (split, split\*2) -> (split\*3, split\*4)...

``` brixelCTF{st1ll_b4d_j4v45cr1pt_h3r3.18079054270} ```

##  login3
5

Nice! you found another one! He changed it up a bit again, could you try again?


Again, a JS function to verify. We can see the JS use the readTextFile to retrieve the username and the password. So lets go to the corresponding URL to retrieve both:


{% highlight javascript %}

function verify() {
	username = document.getElementById("the_username").value;
	password = document.getElementById("the_password").value;
	if(username == readTextFile("username.txt"))
	{
		if(password == readTextFile("password.txt"))
		{
			alert("Password Verified");
		} else {
			alert("Incorrect password");
		}
	}else{
		alert("Incorrect username");
	}
	
}

function readTextFile(filePath) 
{      
	var result = null;
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.open("GET", filePath, false);
	xmlhttp.send();
	if (xmlhttp.status==200) {
		result = xmlhttp.responseText;
	}
	return result;
} 

{ % endhighlight % }

_http://timesink.be/login3/username.txt_

admin

_http://timesink.be/login3/password.txt_

``` brixelCTF{n0t_3v3n_cl05e_t0_s3cur3!} ```



##  login4
5

Whow, another one! You're good! So I told my buddy how you managed to get the password last time, and he fixed it. Could you check again please?


We have a similar code, but in this time the password is 'encoded':

{% highlight javascript %}

if(password == atob(readTextFile("password.txt")))

{ % endhighlight % }

The atob() function is used in JS to decode base64. As the previous login, it retrieves the base64 code from the password.txt file. Go to the URL, retrieve the base64 and then I used [CyberChef](https://gchq.github.io/CyberChef) to decode:

_http://timesink.be/login4/password.txt_

YnJpeGVsQ1RGe2V2ZW5fYmFzZTY0X3dvbnRfbWFrZV95b3Vfc2VjdXJlfQ==

_Decode base64:_

``` brixelCTF{even_base64_wont_make_you_secure} ```


##  Browsercheck
10

I found this weird website, but it will only allow 'ask jeeves crawler' to enter?

Can you get me in?


Maybe we need to change the User-Agent for something related to jeeves... Google Fu shows this answer on [StackExchange](https://webmasters.stackexchange.com/questions/6205/what-user-agent-should-i-set). 

Let's try to change the User-Agent and use: *Mozilla/2.0 (compatible; Ask Jeeves/Teoma)*

I use User-Agent Switcher and Manager Addon for Firefox and configure it with the User-Agent:

<p align="center">
  <img src="/images/writeups/Brixel/Web/8_ua.png" width="500"/>
</p>

Refrest the website and it shows the flag:

<p align="center">
  <img src="/images/writeups/Brixel/Web/8_flag.png" width="500"/>
</p>

``` brixelCTF{askwho?} ```


##  SnackShack awards
10

A friend of mine owns a snackbar and is entered in a competition to win an award.

It seems he is not going to win because he has a low amount of votes :(

Do you think you can boost his votes? adding 5000 votes at once should do the trick!

His snackbar is called Cafetaria 't pleintje


We got into a blog website were we can cast votes, I used burp to intercept the POST method and check which is the data:

<p align="center">
  <img src="/images/writeups/Brixel/Web/10_burp.png" width="500"/>
</p>

We modify the data and send 5000 vote to the store, then forward the request:

<p align="center">
  <img src="/images/writeups/Brixel/Web/10_5000.png" width="500"/>
</p>

Then the browser will display the flag:

``` Well done! The flag is brixelCTF{bakpau}  ```



##  Flat earth
10

These idiots... I heard there is a rally of flat earth believers tomorrow

We should access their admin panel and stop that rally from happening!
http://timesink.be/flatearth/ 


On the HTML we can see a link to the *admin.php* path:

<p align="center">
  <img src="/images/writeups/Brixel/Web/11_admin.png" width="500"/>
</p>

Now we get into the login. admin:admin or admin:1234 didn't work, lets go for an easy SQLi:

With _' OR 1=1 --_ we got in and the webpage displayed the following text:

<p align="center">
  <img src="/images/writeups/Brixel/Web/11_slqi.png" width="500"/>
</p>

``` That should do the trick, the flag is brixelCTF{aroundtheglobe} ```


## Dadjokes
15

Darn! Some idiot scriptkiddy broke my favorite website full of dad jokes!

I can't seem to contact the owner to fix the site

Can you bring it back and remove the defaced page?

http://timesink.be/dadjokes/jokes/

For this chall we could see that there is a defaced page and a comment within it:

<p align="center">
  <img src="/images/writeups/Brixel/Web/defaced.png" width="500"/>
</p>

``` 
Hey bozo! I left your original index file under index_backup.html so you can see how your site looked before I used my l33t skillz to deface it. 
```

``` http://timesink.be/dadjokes/index_backup.html ```

<p align="center">
  <img src="/images/writeups/Brixel/Web/dad.png" width="500"/>
</p>

We can still submit submit jokes and we can spot the following URL while uploading the joke:


http://timesink.be/dadjokes/jokes/submit.php?filename=asdf.php&title=asdf&content=asdf&submit=true

```
You're on the right track, but I'm not going to allow this in here, try that somewhere else!
```

http://timesink.be/dadjokes/jokes/submit.php?filename=../asdf.php&title=asdf&content=asdf&submit=true

```
You're on the right track, but are you trying to be a bit naughty?
```

http://timesink.be/dadjokes/jokes/submit.php?filename=../index.html&title=asdf&content=asdf&submit=true

```
You are definitely on the right track here... but what are you trying to accomplish?
```

At this time I tried to do literally what the chall says and I write into the index.html the original content of the website encoded:

{% highlight html%}

<html><head><title>DadJokes, your source of lame dad jokes</title></head><body><div align="center"><h1>DadJokes</h1><hr><img src="images/banner.png" alt="dadjokes"><br><br><a href="jokes/read.php">Read dad jokes</a><br><br><a href="jokes/submit.php">submit your own jokes</a></div></body></html>

{% endhighlight %}


```
http://timesink.be/dadjokes/jokes/submit.php?filename=../index.html&title=index.html&content=%0A%3Chtml%3E%3Ctitle%3EDadJokes%2C%20your%20source%20of%20lame%20dad%20jokes%3C%2Ftitle%3E%3Cbody%3E%3Cdiv%20align%3D%22center%22%3E%3Ch1%3EDadJokes%3C%2Fh1%3E%3Chr%3E%3Cimg%20src%3D%22images%2Fbanner.png%22%20alt%3D%22dadjokes%22%3E%3Cbr%3E%3Cbr%3E%3Ca%20href%3D%22jokes%2Fread.php%22%3ERead%20dad%20jokes%3C%2Fa%3E%3Cbr%3E%3Cbr%3E%3Ca%20href%3D%22jokes%2Fsubmit.php%22%3Esubmit%20your%20own%20jokes%3C%2Fa%3E%3C%2Fdiv%3E%3C%2Fhtml%3E&submit=true
```

``` brixelCTF{lamejoke} ```


## Pathfinders 1
15

These f\*cking religious sects!

These guys brainwashed my niece into their demeted world of i-readings and other such nonsense.

The feds recently closed their churches, but it seems they are preparing for a new online platform to continue their malicious activities.

can you gain access to their admin panel to shut them down?

Their website is: http://timesink.be/pathfinder/


Since the chall is called pathfinder, maybe we can use some tool to find a file within the server. We also know by inspecting the website that there is an /admin page. I used WFUZZ for this task:

```
wfuzz -c -z file,/opt/SecLists/Discovery/Web-Content/raft-medium-files.txt http://timesink.be/pathfinder/index.php?page=admin/FUZZ
```

I found the following file with different length:

<p align="center">
  <img src="/images/writeups/Brixel/Web/passwd.png" width="500"/>
</p>

http://timesink.be/pathfinder/index.php?page=admin/.htpasswd

```
#normally you would brute force this, but that is not in scope of this challenge. The flag is: brixelCTF{unsafe_include}
admin:$apr1$941ydmlw$aPUW.gCFcvUbIcP0ptVQF0 
```

``` brixelCTF{unsafe_include} ```



## Pathfinder 2
20
It seems they updated their security. can you get the password for their admin section on their new site?

http://timesink.be/pathfinder2/

oh yeah, let's assume they are running a php version below 5.3.4 here...


So it seems to have a .php filter on the path:

<p align="center">
  <img src="/images/writeups/Brixel/Web/12_pathfinder2.png" width="500"/>
</p>

So the target is to bypass that filter. Knowing the PHP version is under 5.3.4, we can check on the [PHP changelog](https://www.php.net/releases/5_3_4.php) to check if there is any security patch. We can see the following entry:
- Paths with NULL in them (foo\0bar.txt) are now considered as invalid (CVE-2006-7243).

So maybe if we use this to trick the filepath and to bypass the filter..

http://timesink.be/pathfinder2/index.php?page=admin/.htpasswd%00.php

We can retrieve the flag!

``` Great work! the flag is brixelCTF{outdated_php} ```

---

# Crypto

##  Sea code
5

beep beep beeeep...

This one should be fairly straight forward

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


We have a morse code audio

```
morse2ascii -m message.wav 
-...../..-..-...---./..-.---.-./-........./-.-......-.-...-...-.--../...../.....---...-.-...-..
```

We can go to [morsecode world](https://morsecode.world/international/decoder/audio-decoder-adaptive.html) and introduce the audio file. The output for this message is:

``` THE FLAG FOR THIS CHALLENGE IS SEAGULL ```


##  Merde
5

A french messenger was caught during the war

He was carrying a piece of paper that read: Vvr ktdk vl jvtzsyHBI{fnzcievs}

Upon torturing the messenger for an explaination, he only shouted 'confidentiel'!!!

Too bad he died, I bet something good was in that message :(


French and that encode format... Sounds like Vigenere Encoder. Let's decode that on CyberChef and use that 'confidentiel' as a key:

<p align="center">
  <img src="/images/writeups/Brixel/Crypto/2_cyber.png" width="500"/>
</p>

``` The flag is brixelCTF{baguette} ```



##  Merda
5

An Italian messenger was caught during the war

He was carrying a piece of paper that read: ymj kqfl nx gwncjqHYK{uneefsfutqn}

Upon torturing the messenger for an explaination, he gestured a V with his fingers. The english guard took it as an insult and killed him right on the spot.

Maybe he just wanted some peace?


Italian and that format... Sounds like Caesar Cipher. Head over to CyberChef and use ROT13 with 21 rotations:


``` the flag is brixelCTF{pizzanapoli} ```


## shit
(s̸͖̾̀͊͠h̸̜̒ï̷̧̲͙̭̤͛͒̋t̷̢̲͚͖̑͜)
10

A messenger droid was caught during the intergalactic war

Upon investigating his memory banks, we found this message:
MDExMTAxMDAgMDExMDEwMDAgMDExMDAxMDEgMDAxMDAwMDAgMDExMDAxMTAgMDExMDExMDAgMDExMDAwMDEgMDExMDAxMTEgMDAxMDAwMDAgMDExMDEwMDEgMDExMTAwMTEgMDAxMDAwMDAgMDExMDAwMTAgMDExMTAwMTAgMDExMDEwMDEgMDExMTEwMDAgMDExMDAxMDEgMDExMDExMDAgMDEwMDAwMTEgMDEwMTAxMDAgMDEwMDAxMTAgMDExMTEwMTEgMDExMTAwMTAgMDExMDExMTEgMDExMDAwMTAgMDExMDExMTEgMDExMDAwMTEgMDExMDExMTEgMDExMTAwMDAgMDExMTExMDE=

We are lucky we found him, he was only 64 parsecs from his base


Seems like a base64 message, let's decode it con CyberChef again. We got binary code:

01110100 01101000 01100101 00100000 01100110 01101100 01100001 01100111 00100000 01101001 01110011 00100000 01100010 01110010 01101001 01111000 01100101 01101100 01000011 01010100 01000110 01111011 01110010 01101111 01100010 01101111 01100011 01101111 01110000 01111101

Using binary decoder we get the following message:

``` the flag is brixelCTF{robocop} ```


## Scheibe 
Scheiße
10

A german messenger was caught during WW2

He was carrying a piece of paper that read: qbhbh zrmua gfbld ocqbv

He was nice enough to give us all we wanted, except the decoded message, he needs a special machine for that, and we don't have it.

He DID give us the settings for the machine.

The settings for this machine are:

Model: G-312

Reflector: UKW 11/26

Rotor 1: 2/12/6

Rotor 2: 1/17/16

Rotor 3: 3/12/1

The flag is ONE word, no spaces. do NOT translate the flag!

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


I found a [great tool](https://cryptii.com/pipes/enigma-machine) to decrypt this ENIGMA machine.


<p align="center">
  <img src="/images/writeups/Brixel/Crypto/4_enigma.png" width="500"/>
</p>

derfl agist sauer kraut

It's german "der flag ist sauerkraut" meaning "the flag is sauerkraut"

``` sauerkraut ```


## flawed
10

Our l33t hackers hacked a bulletin board and gained access to the database. We need to find the admin password.

The user's database info is:

Username:admin

Passwordhash:d269ce15f9c44bc3992a5f4e5f273e06

The flag is the plaintext password

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format



We went to [CrackStation](https://crackstation.net/) and introduced the hash. The password is:

``` notsecure ```



##  Dont be salty
20

Our l33t hackers hacked a bulletin board and gained access to the database. We need to find the admin password.

The user's database info is:

Username:admin

Passwordhash:2bafea54caf6f8d718be0f234793a9be

Salt:04532@#!!

We know from the source code that the salt is put AFTER the password, then hashed. We also know the user likes to use lowercase passwords of only 5 characters long.

The flag is the plaintext password.

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


Following the instructions we can use hashcat to guess the hash. We placed the hash with the hashcat format (we know it's MD5 with a salt after the password, therefore we need mode 10) in the file chall.hash:

chall.hash:
```
2bafea54caf6f8d718be0f234793a9be:04532@#!!
```

Then with the following command we indicate -a 3 (bruteforce), -m 10 (md5 hash(password:hash)), the file, the rule (l? is a lowercase letter), --show to display the password

```
hashcat -a 3 -m 10 chall.hash l?l?l?l?l? --show
2bafea54caf6f8d718be0f234793a9be:04532@#!!:brute
```

``` brute ```

---

# OSINT

##  A quick search
5

Here's an easy one to start off the OSINT challenges.

I took this photo but forgot the name of this tower.

Can you give me the name?

I remember it started with an E.

This flag is not in the usual format, you can enter it with or without the brixelCTF{fl-ag} format


<p align="center">
  <img src="/images/writeups/Brixel/OSINT/image.jpg" width="500"/>
</p>

We can use Google Images to do a reverse lookup with the photo:


<p align="center">
  <img src="/images/writeups/Brixel/OSINT/1_tower.png" width="500"/>
</p>

``` Eben-Ezer ```


## Manhunt1
5

My dad is pissed off! He was told by my mother NOT to buy ice cream but he did anyway when she wasn't looking.

Someone posted this picture on the internet and my mother saw it, man, he is in so much trouble!

I want to know WHO posted this picture, let's hunt this guy down!

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format, replace spaces with underscores ('\_')

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


<p align="center">
  <img src="/images/writeups/Brixel/OSINT/icecream.jpg" width="500"/>
</p>

We used exiftool in order to find any hint about the location, the event, the mobile phone... And we found the owner:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/1_hunt.png" width="500"/>
</p>

``` Johnny_Dorfmeister ```


## Manhunt2
5

Ah, now you know his name, good. Can you tell me the name of his last employer?

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.

We used Google Fu and found its LinkedIn profile and checked where he worked. Current work is independent, so we use the previous company called:

``` pishapasha ```

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/2_hunt.png" width="500"/>
</p>



## Manhunt3
5

Ah, now you know his name, good. Can you tell me his favorite food?

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


On LinkedIn contact info we found its twitter and with that username also its instagram:

_https://twitter.com/johnnydorfmeis1_
_https://www.instagram.com/johnnydorfmeister/_

I got in a rabbit hole with the droste.zip file and the cacao image on one of the tweets... Then I found the instagram and check out all the images and finally, I found this one with the description:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/3_insta.jpg" width="500"/>
</p>

According to the description, his favorite food is:

``` macaroni ```



## Manhunt4
5

Ah, now you know his name, good. Can you tell me his birthday? (DD/MM/YYYY)

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.

There is one tweet saying:

```
I can't seem to open this zipfile. I bet there's something interesting in it. https://alf.nu/s/droste.zip
```

Inside it has an image and a recursive zip:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/droste.jpg" width="500"/>
</p>

Rabbit hole, just ignore the zip. I followed him on Twitter and his birth date showed up:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/manhunt4.png" width="500"/>
</p>


``` 18/12/1985 ```


## Manhunt5
5

What is he talking about with that deleted page on his twitter account? can you retrieve it somehow?

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/3_hunt.jpg" width="500"/>
</p>

Oh... A tweet telling to delete a website and archive... [WaybackMachine](https://web.archive.org) time! The webpage to look for is **http://howitshould.be/test-page**

Looking into Jan 15th we found a screenshot on web archive with the message:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/3_way.jpg" width="500"/>
</p>

``` Howdy! the flag is: w@yb@ck! ```

## Manhunt6
5
So he's a webdesigner huh? I wonder what his customers have to say about his work. In english please!

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


This could be related with the twitter account we saw early with his website [howitshould.be](http://www.howitshould.be/). Let's visit it to find further information:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/9_manhunt6.png" width="500"/>
</p>

Oh! We saw in the chall "In english pls!" let's translate that rusky text:

Есть свободный флаг: поэзия
There is a free flag: poetry

``` poetry ```


## Manhunt7
10

Can you tell me where he lives? Then I can drop some kind words in his mailbox!

Format: brixelCTF{STREET_NUMBER_POSTALCODE_CITY} e.g brixelCTF{examplestreet_15_8500_kortrijk}

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


Well... After some time looking into his social media (Twitter, Instagram, LinkedIn and GitHub) I just looked over the contact form at his website and try to send an email. Suddenly I got this:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/manhunt7.png" width="500"/>
</p>

Johnny Dorfmeister
Melkvoetstraat 48
3500 Hasselt

``` brixelCTF{Melkvoetstraat_48_3500_Hasselt} ```


## Manhunt8
5

so now you know where he lives, can you tell me what was written on his wall in 2013?

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format, replace spaces with underscores ('\_')

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


I head over to GMaps, used the previous addres and reach the house. I also change the date of GMaps to be on the year 2013 and saw the message in the wall:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/manhunt8.png" width="500"/>
</p>

``` JUST_MARRIED ```


## Manhunt9
15

Auth. You're on your own for this :)

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

This is part of a series, more challenges will be unlocked once you solve this one. While most challenges will be OSINT, some of them may not be.


Auth... Okey, looking to the web I found a little auth link....

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/10_auth.png" width="500"/>
</p>

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/10_page.png" width="500"/>
</p>


https://github.com/JohnnyDorfmeister/authentication-requests


On the github we found the code of the Auth webpage:

{% highlight php %}

<?php
if(!isset($_POST['username']))
{
	die("Eat shit and die...");
}
if($_POST['username'] == "johnny" && $_POST['password'] == removed for security reasons)
{
	$_SESSION["loggedin"] = "true";
	include("flag.php");
	die();
}else{
	echo "<form method=\"POST\" action=\"" . $_SERVER['PHP_SELF'] . "\">\n";
	echo "<table align=\"center\">\n";
	echo "	<tr><td>Username:&nbsp;<td><input type=\"text\" name=\"username\"></tr>\n";
	echo "	<tr><td>Password:&nbsp;<td><input type=\"password\" name=\"password\"></tr>\n";
	echo "	<tr><td colspan=2 align=\"right\"><input type=\"submit\" name=\"submit\" value=\"log in\"></tr>\n";
	echo "</table>\n";
	echo "</form>\n";
	die();
}
?>

{% endhighlight %}

Well... We don't know the password... But maybe on [previous commits](https://github.com/JohnnyDorfmeister/authentication-requests/commit/2ae62707c1b1ab8266ddc3329ae07da163dfd97a)...

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/10_commit.png" width="500"/>
</p>

Use the following command with the POST fields set with the found creds and retrieve the flag:

```
kali@kali:~$ curl -d "username=johnny&password=letmein" -X POST http://authentication.howitshould.be/auth.php
<html>
<body>
<title>Auth</title>
<!-- Authentication script by Johnny Dorfmeister. https://github.com/JohnnyDorfmeister/authentication-requests !-->
congratulations, the flag is g1ttern00b
```

``` g1ttern00b ```



##  Physical pentest setup
10

So we're going to try to install a 'bug' at the local police office of the city of Halen (BE)

The plan is to disguise ourselves as technicians of their telecom provider, but who is that?

❗ do NOT call them to get the information, you can find everything you need online! It's a criminal offence to bother them with nonsense like this and you could get in trouble for it. 📵

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

Replace all spaces with underscores (\_) while entering the flag!


We start by looking into the city and search for a police station, but nothing showed up, only two stations on the nearby cities. On Google,  I searched for "police station halen belgium" and results didn't look too promising.

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/6_gmaps.jpg" width="500"/>
</p>

I decided to go for duckduckgo with no language preference and use the query "halen belgium politie" since politie is the name of the police there. I found an [interesting result](https://www.halen.be/politie-lrh) from the council website.

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/6_council.jpg" width="500"/>
</p>

There we could find interesting information about the station:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/6_info.jpg" width="800"/>
</p>

Halen district office

- Address: Sportlaan 2A, 3545 Halen
- Phone: 011 93 89 80
- Email: halen@politielrh.be
- Website: http://www.lokalepolitie.be/5907

Looking for it at the Google Street View, I supposed that the station will be blurred since it's forbidden to take photos of police buildings but this time Google has to improve their algorithm. We confirm that the result is the local police station:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/6_station.jpg" width="800"/>
</p>

Since we need to know their telecom provider and we currently know their phone number (011 93 89 80, international syntax: +32 11938980), we can look for the carrier at the Internet:

- [FreeCarrierLookup](https://freecarrierlookup.com/)
- [Telnyx](https://telnyx.com/number-lookup)

Well.. One of the tools says _Orange Business Belgium_ and the other one _COLT Telecom_ I tried Orange and didn't work, so there is one option left:

``` COLT_Telecom ```


## Visit Limburg 1
10
Welcome to Limburg! This beatiful province of Belgium offers so much to see and do!

Let's say you want to take a nice bike ride from one side to the other, but don't want to drive too much on roads where there are a lot of cars driving.

For this purpose we have a bike network! It tries to guide you as much as possible over smaller roads and bike paths where cars are not allowed to drive.

Let's start here: [Google maps link](https://www.google.com/maps/place/Maastrichtersteenweg+201,+3770+Riemst/@50.8333603,5.637255,15.25z/data=!4m5!3m4!1s0x47c0e88b020d50a5:0x81a9e3b139856acb!8m2!3d50.8316796!4d5.6442194) and end here: [Google maps link](https://www.google.com/maps/place/Steenstortstraat,+3581+Beringen/@51.078121,5.2380936,16.5z/data=!4m5!3m4!1s0x47c13016b7f3699f:0xcf71d3a220658e79!8m2!3d51.0784705!4d5.2415842).

The flag is the sum of all the nodes you pass on your trip (including the start and end nodes!)

Happy cycling!

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


For this challenge I was searching for "Limburg Cycle Routes Network map" on google and found this [Wordpress](https://bicycledutch.wordpress.com/2019/09/25/the-numbered-junction-network-for-recreational-cycling/) with some webpages describing this network map. Within that blog I found [Fietsnet](https://fietsnet.be/routeplanner/default.aspx) which is a web app to configure your route and show the nodes.

Since we have the GMaps directions, I set the route by cycle on GMaps and looked for the origin and the destination nodes on the Fietsnet app. 

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/7_gmaps.png" width="800"/>
</p>

The starting node is at Maastrichtersteenweg, node 341. The destination is Steenstortstraat, node 557. 

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/7_close.jpg" width="800"/>
</p>

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/7_fiet.jpg" width="800"/>
</p>

With all the nodes displayed by the app we have to sum them up:

341+310+518+307+311+316+300+305+79+74+548+73+509+250+30+29+534+251+64+532+533+65+558+11+557 = 7405

``` 7405 ```


## Visit Limburg 2
20

I'm supposed to meet up with my buddy who supplies me with Twilight cd's but he's very cryptic about the meeting location.

He told me he will meet under a bridge that sits in the middle of a triangle of 3 locations.

The first two locations are the two mineshafts of the coalmines in the province that are the furthest appart from each other.

The third location is where the provided (hazy) picture was taken

Please provide the date that was written under the bridge (format #.##.## where # is a digit) in 2013

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format



This challenge took me a lot of time to solve it. I'm currently a Limburg expert haha. Now to the serious part.

Limburg has two sides, one on The Nederlands and other one in Belgium. I was digging around the Nederlands part which was useless since the challenge is meant to be done within the Belgium part.

I started by collecting the first clear location which is the one on the photo:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/IMG_1243.jpg" width="800"/>
</p>

In the image we can see like a chuck made of layers and could be metal. I used "metal church limburg" query on Google and found the monument. It is called "The See-Through Church of Borgloon":

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_church.jpg" width="500"/>
</p>

We locate the church in GMaps to start drawing the triangle. For this chall I added the places on the favorite list.

Now to the fun part, finfing the two coal mine shafts. Those should be the ones which are the furthest appart from each other. I used [Mindat](https://www.mindat.org/) and Google Fu in order to find the shafts. I came across this [MiningHeritage](http://www.miningheritage.org/en/general/history-Limburg-coalmines) website which shows a map with the main coal concessions in Limburg:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_coal.png" width="800"/>
</p>

Well, if we pick the furthest appart, we find the number 2 and 10-11. Number 2 corresponds to be-Mine. This one still has a shaft, so it is a candidate:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_bemine.jpg" width="800"/>
</p>

Looking for the coal mine 10-11 I found the [Eisden Mine](https://www.mindat.org/loc-194086.html) which also had a shaft. This is the second candidate and it was added into the GMaps list:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_eisden.jpg" width="800"/>
</p>

Now we boild a triangle with those apexes and calculate the center (approx):

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_triangle.png" width="800"/>
</p>

So we will take a look into two of the brigdes which are close to that location. Remember, we need a date written under the bridge on 2013. We can check the date with the Google Map Street Viewer. To do so, look into the history on the upper left cornet and select the pictures taken on June 2013:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_bridge.jpg" width="800"/>
</p>

Wait, what's in there? Zoom in and there you have the date:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/11_date.jpg" width="800"/>
</p>


``` brixelCTF{7.05.09} ```




## Visit Limburg 3
25
While hiking on the BE-MINE hill on 18 december 2020 I noticed a big cargo plane with 4 props flying over, it looked like it had militairy markings.

I tried to find out what plane it was but failed. Can you help me out? I bet it came from one of the military airbases nearby.

I need the registration number, model, and month & year of registration.

Syntax: brixelCTF{registrationnumber+model+month(MM)+year(YY)}


First let's see where the airplane was seen. Be-MINE hill... That could be close to the mine we saw in the previous chall. Anyway, a quick search on Google tells you that there are hills and mountain parks on the old mine placement, so we know where it is:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_skate.jpg" width="600"/>
</p>


For this chall I tried to search for an airplane with those features: 4 props, military and cargo. I had two cadidates the A400M from Airbus and the C-130 from Lockheed:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_planes.png" width="600"/>
</p>

I used [RadarBox](https://www.radarbox.com/) to check the flights history. Other friends at the CTF used [ADS-B](https://www.ads-b.nl/index.php) which was specialized on military flights. I used the free-trial from radarbox and searched for A400 and C-130 aircrafts flight on the 18th December:

- A400:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_a400.png" width="600"/>
</p>

- C-130:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_c130.png" width="600"/>
</p>

The only one that has a route over the target location (be-Mine) is that C-130 CH-09. We can see the flight information on [RadarBox](https://www.radarbox.com/data/flights/baf677):

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_radar.png" width="600"/>
</p>

Now that we know the model (C130) and the registration number (CH-09), we can look for the registration date. Googling "ch-09 c130 registration information" I found a [C-130 website](http://www.c-130.net/aircraft-database/C-130/serials-and-inventory/airforce/BAF/) with that information:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/12_date.png" width="600"/>
</p>

We can see the delivery date: 03/1993

``` brixelCTF{CH-09+C130+03+73} ```



## Bird call
20
I heard this birdcall when walking the other day

Can you give me the LATIN name of this bird species?

Here's a dutch joke for you: Het is niet de proxi-mus📱🤣

replace (if any) spaces with underscores ('\_')

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


For this chall I downloaded the file and used an app called BirdNET for Android. This app allows us to record sound, select a frame of the spectrogram and analyze it. The results shows a White stork species:

<p align="center">
  <img src="/images/writeups/Brixel/OSINT/8_birdcall.jpg" width="500"/>
</p>


``` White_Stork ```

Well... Read after send. it was on latin...

``` Ciconia_ciconia ```



---

# Forensic

## A message from space
10

I received a message from space

Beam me up scottie1!

We got a recording.wav audio. Listening to that and the space hint, experience tell us that this is SSTV. For this matter I always use [RXSSTV](http://users.belgacom.net/hamradio/rxsstv.htm) app on Windows to load the file and retrieve the image.


<p align="center">
  <img src="/images/writeups/Brixel/Forensic/1_sstv.jpg" width="500"/>
</p>

``` brixelCTF{SP4C3L4B} ```


## Lottery ticket
10

Someone is trying to sell this lottery ticket online, it has the winning numbers but I suspect foul play

Can you tell me which the new numbers are that are photoshopped?

Add them all up, the resulting number is the flag

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


We have a ticket:

<p align="center">
  <img src="/images/writeups/Brixel/Forensic/lotteryticket.jpg" width="500"/>
</p>

Taking a closer look we can see that some numbers have noise around them and some other dont. Those with no noise are probably the photoshopped one since that noise is harder to reproduce.

<p align="center">
  <img src="/images/writeups/Brixel/Forensic/1_lot.png" width="500"/>
</p>


The numbers with no noise are:

42+88+25+48 = 203

``` 203 ```


## Lost evidence
30

A buddy of mine is in serious trouble. He works for the feds and accidentally deleted a pendrive containing crucial evidence

Can you get it back and tell us what the evidence is?

We need to know what the suspect bought

The flag is in this format: brixelCTF{name_of_product_bought}

First we used FTK Imager to analyze the content of the image. Looking into all the files, we found a file with RIFF WAVE header, which corresponds to .wav format.

<p align="center">
  <img src="/images/writeups/Brixel/Forensic/3_ftk.png" width="500"/>
</p>

Extracting the file and listening to it, we can hear a conversation with a phone bot of a bank. In the conversation, the client is using the phone pad to introduce numbers. We can know that from the [DTMF](https://onlinetonegenerator.com/dtmf.html) (Dual Tone Multi Frequency) on the sequence. 

In order to extract that sequence I used multimon-ng tool to extract all the DTMF detected and get the corresponding numbers. The result is the following sequence and hearing the audio, we can set the different parts that the client is introducing:

```
multimon-ng -t wav -a DTMF 001416.wav

DTMF: 2
DTMF: 1
DTMF: 2
DTMF: 5
DTMF: 5
DTMF: 5
DTMF: 4
DTMF: 2
DTMF: 4
DTMF: 0
----
DTMF: 5
DTMF: 4
DTMF: 6
DTMF: 6
DTMF: 6
DTMF: 9
---
DTMF: 1
DTMF: 6
DTMF: 0
DTMF: 9
---
DTMF: 2 Transfer
---
DTMF: 5 Acc number
DTMF: 3
DTMF: 3
DTMF: 2
DTMF: 6
DTMF: 6
---
DTMF: 5 Amount
DTMF: 0
DTMF: 0
DTMF: 0
---
DTMF: 1 Personal msg
---
Message:

DTMF: 8 
DTMF: 4
DTMF: 4
DTMF: 9
DTMF: 9
DTMF: 0
DTMF: 3
DTMF: 3
DTMF: 3
DTMF: 6
DTMF: 6
DTMF: 6
DTMF: 7
DTMF: 7
DTMF: 7
DTMF: 0
DTMF: 8
DTMF: 4
DTMF: 4
DTMF: 3
DTMF: 3
DTMF: 0
DTMF: 2
DTMF: 2
DTMF: 2
DTMF: 6
DTMF: 6
DTMF: 6
DTMF: 2
DTMF: 2
DTMF: 2
DTMF: 2
DTMF: 4
DTMF: 4
DTMF: 4
DTMF: 6
DTMF: 6
DTMF: 3
DTMF: 3
DTMF: 0
DTMF: 2
DTMF: 2
DTMF: 7
DTMF: 7
DTMF: 7
DTMF: 8
DTMF: 8
DTMF: 4
DTMF: 4
DTMF: # End of message
DTMF: 2 Nothing more
```
The message is:

8449903336667770844330222666222244466330227778844

And decoding it with the movile text pad we get:

THX FOR THE COCAINE BRUH

``` brixelCTF{cocaine} ```


---

# Programming

## Keep walking
10

This is a challenge to test your basic programming skills.

Pseudo code:

Set X = 1

Set Y = 1

Set previous answer = 1

answer = X * Y + previous answer + 3

After that => X + 1 and Y + 1 ('answer' becomes 'previous answer') and repeat this till you have X = 525.

The final answer is the value of 'answer' when X = 525. Fill it in below.

Example:

5 = 1 * 1 + 1 + 3

12 = 2 * 2 + 5 + 3

24 = 3 * 3 + 12 + 3

........................

........................

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format

We implemented the pseudocode in python and solve the challenge

{% highlight python %}

x = 1
y = 1
prev_ans = 1

while x != 526:
	ans = x*y+prev_ans+3
	print('{} = {} * {} + {} + 3'.format(ans, x, y, prev_ans))
	x += 1
	y += 1
	prev_ans = ans

print(ans)

{% endhighlight %}

```
47550112 = 522 * 522 + 47277625 + 3
47823644 = 523 * 523 + 47550112 + 3
48098223 = 524 * 524 + 47823644 + 3
48373851 = 525 * 525 + 48098223 + 3
ans = 48373851
```
 
``` 48373851 ```


## A song
10
I wrote this song

it seems I'm pretty bad at it, but hey! it could get you a flag :)


For this chall I cound find that there is a programming language called [Rockstar](https://esolangs.org/wiki/Rockstar) which can be also a (pretty bad) song lyrics. I could retireve the flag using [this website](https://esolangs.org/wiki/Rockstar).

``` brixelCTF{5667236346614} ```


## An arduino project
15
I once made this arduino program but forgot what it does.

Unfortunately I lost the schematic that comes with it.

Maybe you can get it running?

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


They provide an arduino project. I have one, so... Hands on! I compile and upload the sketch and hook up the cables as intended. If you take a look, those comments in the pinMode are probably the input pins of a 7-segments LED display, in this case, comon anode (because it connects the common positive to 5V):


<p align="center">
  <img src="/images/writeups/Brixel/Programming/5_7.jpg" width="500"/>
</p>


<p align="center">
  <img src="/images/writeups/Brixel/Programming/5_arduino.png" width="500"/>
</p>


<p align="center">
  <img src="/images/writeups/Brixel/Programming/5_cables.png" width="500"/>
</p>

We can see a sequence showing up:

<video width="394" height="700" controls>
  <source src="/images/writeups/Brixel/Programming/5_video.mp4" type="video/mp4">
</video>

```
406798190332....
```

Don't use the dots to define the flag...

``` 406798190332 ```




---

# Old Tech

## Goodbye old friend
15
On 31/12/2020 support for flash will end

Therefor we made you a farewell animation

Can you get the flag?

Beware headphone users! the music is loud.


We got a swf file. Using Internet explorer we can see an f on the left side. Seems like the frame is fixed and there is more image on the left site.
In order to check the complete flash, I used [SWF File Player](http://www.swffileplayer.com/). I load the file and then expand the window to display the flag:

<p align="center">
  <img src="/images/writeups/brixel/Old/1_flash.png" width="500"/>
</p>

``` brixelCTF{n0_m0r3_5upp0rt} ```



## The tape
20
I found this cassette tape from the '80s. I bet it has some cool games on it or something.

Better start looking for someone who grew up in that era... :)

This flag is not in the usual format, you can enter it with or without the brixelCTF{flag} format


In this chall I tried to figure out which technology was involved. I came to the conclusion of being a tape for Commodore64 or Sinclair Spectrum. Also the TAP capital letters on the filename were a hint to reach the conclusion. 
I looked for a emulator of those computers and found that they need a file with .TAP or .TZX files in order to work properly. I found this program called [AudioTap](http://wav-prg.sourceforge.net/audiotap.html) which converts a WAV file into TAP file. 

<p align="center">
  <img src="/images/writeups/brixel/Old/4_audiotap.png" width="500"/>
</p>

Then it was time to look for an emulator. The TAP file didn't work on Sinclair emulators so I also tried on C64 emulators, until I found one were it worked: [C64Emulator](https://c64emulator.111mb.de/index.php?site=pp_javascript&lang=en&group=c64).
I uploaded the file and retrieve the flag:

<p align="center">
  <img src="/images/writeups/brixel/Old/4_tape.png" width="500"/>
</p>


