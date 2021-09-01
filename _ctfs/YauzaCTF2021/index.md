---
title: "CTF - YauzaCTF 2021"
categories: [ctf]
date: 2021-09-02
comments: true
description: CTF - YauzaCTF 2021
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# YauzaCTF 2021

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/logo.png" width="50%"/>
</p>


Welcome! I've participated in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some OSINT challenges! This CTF was hard, we got some Forensic challenges with the help of my teammates, but I focused on the OSINT ones. I could only get one of those, but I was close to get the other one. Some great GitHub OSINT techniques in [_Get in touch_](#get-in-touch).

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/rank.png" width="70%"/>
</p>

---

Challenge index:

### OSINT

- [Stolen Capitalism](#stolen-capitalism)
- [Get in touch](#get-in-touch)

<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# OSINT

## Stolen Capitalism

English Someone stole bitcoins from the wallet of our Party bc1qtmyn49urgfu27n30zrghl8499sx42pqrhpm8xt.

Find the real name of the attacker. The flag matches the specified format.


We can see [here](https://www.blockchain.com/btc/address/bc1qtmyn49urgfu27n30zrghl8499sx42pqrhpm8xt) that they stole 0.00046552BTC since it's the only [outgoing transaction](https://www.blockchain.com/btc/tx/05e3d1eaec7d263e80d4cc03958bf9c0136888a1acd99661cba7dd86fe36d44b).

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/1_0_bit.png" width="70%"/>
</p>

- bc1qtmyn49urgfu27n30zrghl8499sx42pqrhpm8xt Original

- bc1q0lpcsvr6p2lg0wrmwge548x0yau943n9vljzsr
- bc1qcrf4najrlj6pwynxhywlp3h3qzazvthrdp5ta3

It was sent to this address: bc1q0lpcsvr6p2lg0wrmwge548x0yau943n9vljzsr and there is a chain of addresses that lead me to these two:

- bc1qz3n89hjq4rrlzsna3pdrygslum7v4slf8qpm8h
- bc1qeaxw4s92ajf939z280rmycq2vw39scvqv573yu

I search for them on Google to try luck and the _bc1qz3n89hjq4rrlzsna3pdrygslum7v4slf8qpm8h_ one shows this search result:

https://antichat.com/members/413903/


<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/1_1_profile.png" width="80%"/>
</p>


- Info:

For donats: bc1qz3n89hjq4rrlzsna3pdrygslum7v4slf8qpm8h
For messages: tovarishch123@protonmail.com

Okey, this can be out target.

I used ProtOSINT to retrieve more info about the email account:


```
   ___           _            _       _   
  / _ \_ __ ___ | |_ ___  ___(_)_ __ | |_ 
 / /_)/ '__/ _ \| __/ _ \/ __| | '_ \| __|
/ ___/| | | (_) | || (_) \__ \ | | | | |_ 
\/    |_|  \___/ \__\___/|___/_|_| |_|\__|
                                                  

Protonmail API is ONLINE
Protonmail VPN is ONLINE

Let's take a look at your target:
1 - Test the validity of one protonmail account
2 - Try to find if your target have a protonmail account
3 - Find if your IP is currently affiliate to ProtonVPN

Choose a program: 1
You want to know if a protonmail email is real ?
Give me your email: tovarishch123@protonmail.com
Protonmail email is valid
Date and time of the creation: 2021-08-24 12:34:43
Do you want to download the public key attached to the email ?
Please enter "yes" or "no": yes
-----BEGIN PGP PUBLIC KEY BLOCK-----
Version: ProtonMail

xsBNBGElH6MBCAC8xJSVy7p2zaP0+BpxKqm8gXwLllK10xpnCqFCMap0I2tJ
OZ7w77brAhDHgvrgFUZKVq6TwL67+Opagvw+FOXIM9jMY1tIegDT4ClOvAq0
qu4xl1eUngkVjof2nZOXOGWmOMMucg85kW/KbICrKMS4qmkMX/SDNvdGgG2T
EfFLFk5L36LNRkrdcldfMO1pBuC0ryYcpkln3ECghU7C5IQ/to1YmWAdTBrj
nWIsHy3tx5CDc8kfjHFwWPTPD6ohVl9Nn04r/BBvIGSXHHOrF1H0mYl3+mX/
6QrNcGfv0nNKNjEh68uagQh5kvQbgpaHJ8Mfp10hhrAS1TaWmMJt6bHvABEB
AAHNO3RvdmFyaXNoY2gxMjNAcHJvdG9ubWFpbC5jb20gPHRvdmFyaXNoY2gx
MjNAcHJvdG9ubWFpbC5jb20+wsCNBBABCAAgBQJhJR+jBgsJBwgDAgQVCAoC
BBYCAQACGQECGwMCHgEAIQkQqNhcv6+LqtAWIQRB82GX5+lCXfUDmYOo2Fy/
r4uq0JvgB/9RSO6oEGDm5vR6/AARczZ438r+/V6xabT4kbGv/kfLmVEbKbLN
MR9DJbM2aQHoOcgrM6FcYN3JQrtQyCAeAwoNrcvZdaGMX0J+IchBkViUnCad
uWOaoAlLAdBj5ouGmrOJ6qXsiWLy0nM3JytSLYishhGi8V5ZjXZkJrQMcdj5
vZse1o7Oy0LBH1aP845w5clNChQYkkqU79voPPOLqipzcN+UfrmwYGnhCp9w
dRTnT56OcX+4HOQ7gPddx1UgaF7Zk9SGTJGDaxdSK49X44NgXSjWtPN9shYw
oAaQJFgqonukdIDsBD7k4zaywlUpD70Ecpcivu9Iw4xKa5U4XvTxzsBNBGEl
H6MBCAC4xc84HEQWerKr5Ae4QvU1lGmnzYMoPCRyAnXgCLSdx2GcQxziDXKV
fGSWP+LfVZBSiD8S0r6KMK5LM4oyzhCJBS7CFAje5lP/YVfvH/RRUtXzIP0O
ie/6diltjGjtiAnS25Rrwxk5oavSIHAtvVwiY8op1WLoXTeE4ef/MAiftRsT
iAEUs+NntH52fz+9h9Ppcq7Rj7iyKKGxm/6Ls/sLLjFAf8E3cQ18Tagq+A/U
kNowUfXx9/ShEk02MBBcYS1XboyjxXkr66DUabllUtwhjGLX5vrcESTtlhZj
YsqEjQY+/uBBHXqatoNeg8YFa/8+dwUj45Fqe7/tqiBCN3arABEBAAHCwHYE
GAEIAAkFAmElH6MCGwwAIQkQqNhcv6+LqtAWIQRB82GX5+lCXfUDmYOo2Fy/
r4uq0LhlB/9gP5JvRscfKoCUjsiGyOsp7Z0PmiLq8EwTPnqthkRj+qru56uQ
Fp4bFtNAOWC1NtM1LtFKV33DrMYgbfWLwSKWraMOMepFZmnQaxE+YvovDrfx
ncUJIyTtWtVzP52zr9FaSV7G4BtNyef5a6ddRvAWMGt5bq5hZ4hNhWLQK3I2
lKWm0yuFxfZhNqh5JAaNpFzOTaZDrGlFql6y8+3rEkVjkzKiByaJqA4eHffz
P4TJ4TvSJnsVf75eVArWKvoOibq5pbE27QoD+zSGJqXADgRb7Oo38eROQo72
4Gt0T7hHr7hiVEh9hlZskhTDY4iC+CU3pGhhkMvtCsLCKYPZgz/r
=OxJJ
-----END PGP PUBLIC KEY BLOCK-----

```

Okey, a creation date is interesting : **2021-08-24 12:34:43**
Used the PGP key to get extra information from Kleopatra, maybe I could find the name there if it was configured, but didn't:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/1_2_kleo.png" width="70%"/>
</p>

Nothing on protonmail contact information as well:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/1_3_proton.png" width="70%"/>
</p>


- His posts:

Title: Reliable mixers
Msg: Recommend a good proven bitcoin mixer
Msg (rusky): Получите отличный миксер для битков

This post is talking about a bitcoin mixer, so he is trying to lose track of his bitcoins (since they are stolen), so this is definitely our target.

Also other message is in a thread talking about what they are listening at the moment.
He has listen on Tuesday 24/08/2021 at 8:13 PM to this [youtube video](https://www.youtube.com/watch?v=DMoCM_FgLP8) as he says on [here](https://antichat.com/threads/424516/page-162#post-4472293) 

I checked all comments in YouTube from that date and checked all YouTube accounts but nothing interesting popped up.

There is a comment that looked suspicious, but nothing more.

```
[COMMENT]
Ivan Ivanoff
youtube.com/channel/UCPx-OC_YhRca7ipQpXvmawQ

youtube.com/watch?v=DMoCM_FgLP8&lc=UgxOnZBgw8uDKc5LmWp4AaABAg
4 days ago | like: 0 | reply: 0

Toska is the word you're looking for

```

Nah I got stucked....

Okey, I have to go bad to all findings. First, the dates of the different occurences:

- Stole bitcoin       : 2021/08/22 - 20:54
- Stole bitcoin 2     : 2021/08/22 - 21:41

- Stole bitcoin 3     : 2021/08/23 - 09:03

- Created email acc   : 2021/08/24 - 12:34
- Youtube song post   : 2021/08/24 - 20:13 
- Bitcoin mixer post  : 2021/08/24 - 20:19 


Sad, I couldn't get it, I tried many thing but nothing worked. 
The solution? It was all about going to Skype and search for the email I found (tovarishch123@protonmail.com), nothing about keybase or vk as I tried...




## Get in touch

Our best agent is going to send us a message from an enemy country. But instead, he just posted some stupid player on his page! Find his public key immediately and send us the first 42 symbols in an appropriate form so we can be sure it's him. For example, YAUZActf{Tgj8t6gbK9zlv4Xmivyhttjzvfbbp7nuqkce3uuomF}

tasks.yauzactf.com:30010

The provided webpage has the following HTML code:

```html
<!DOCTYPE html>
<html>
<body style="background-color: red;">
    <link rel="stylesheet" href="https://cdn.plyr.io/3.6.8/plyr.css" />
    <script src="https://cdn.plyr.io/3.6.8/plyr.polyfilled.js"></script>

<div hidden class="plyr__video-embed" id="player1">
    <iframe
        src="https://www.youtube.com/embed/DMoCM_FgLP8"
        allowfullscreen
        allowtransparency
        allow="autoplay"
    ></iframe>
</div>

<div hidden class="plyr__video-embed" id="player2">
    <iframe
        src="https://www.youtube.com/embed/Bwf9f498Yqs"
        allowfullscreen
        allowtransparency
        allow="autoplay"
    ></iframe>
</div>

<div hidden class="plyr__video-embed" id="player3">
    <iframe
        src="https://www.youtube.com/embed/o2mD5hv0eMc"
        allowfullscreen
        allowtransparency
        allow="autoplay"
    ></iframe>
</div>

<script>
function which_playlist_i_want_to_listen() {
    var currenthour = new Date().getHours(); 

    switch(currenthour % 3) {
        case 0:
            var elem = document.getElementById("player1");
            elem.removeAttribute("hidden");
            break;
        case 1:
            var elem = document.getElementById("player2");
            elem.removeAttribute("hidden");
            break;
        case 2:
            var elem = document.getElementById("player3");
            elem.removeAttribute("hidden");
            break;
    }
}

which_playlist_i_want_to_listen();

</script>

</body>
</html>
```

There are 3 YouTube videos in different iframes at the website. First thought was about... Can I find an interesting playlist on YouTube with the 3 of them? Could it be that the owner of that playlist is the guy I'm looking for...

I tried with different techniques and using YouTube filters for playlists but I could not find any suspicious user.


<div align="center" display="grid">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_0_1_obs.png" width="30%"/>
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_0_2_obs.png" width="30%"/>
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_0_3_obs.png" width="30%"/>
</div>

I downloaded the comments and replies from the three videos using [YCS - YouTube Comment Search](https://chrome.google.com/webstore/detail/ycs-youtube-comment-searc/pmfhcilikeembgbiadjiojgfgcfbcoaa) for Chrome and search with a regex for a public key _\w{42,64}_


But there was nothing. Then an idea came in... Could it be that the user left comments in the three videos? Maybe parts of the public key that should be reasembled? Or even I can check the user YouTube channel to look for more information. Hands on.

First I got the 3 txt with the comments from the YCS addon, and created this Python script to get all users commenting and replying in the 3 videos, then I looked for the intersection between the sets of users:

```python
import json
import re

with open('Obscurity.txt', 'r') as in_file:
  obs = in_file.read()

with open('Gagarin.txt', 'r') as in_file:
  gaga = in_file.read()

with open('OurDream.txt', 'r') as in_file:
  dream = in_file.read()

obs_users = []
gaga_users = []
dream_users = []

res = re.findall(r'[\]]\n((\S+| )*)', obs)
for found in res:
  obs_users.append(found[0])

res = re.findall(r'[\]]\n((\S+| )*)', gaga)
for found in res:
  gaga_users.append(found[0])

res = re.findall(r'[\]]\n((\S+| )*)', dream)
for found in res:
  dream_users.append(found[0])

common = set.intersection(set(obs_users), set(gaga_users), set(dream_users))

print(common)

```

Results:

```json
{
  "0bserver1": "youtube.com/channel/UC_RHiNvDCFuI2tLYr4zFDOw", 
  "Simon Young": "youtube.com/channel/UC06U9X3aT55uPhbcP3xVBFw", 
  "Александр с Альтаира": "youtube.com/channel/UCI-3DgqtpkA4FxnwkQ4au9w", 
  "Stas Vas": "youtube.com/channel/UC1H23Ka3vEqqGzpqSb9nXRg", 
  "Денис": "youtube.com/channel/UC-T6OqkstWfMPiV0Qom4aUA", 
  "Ромашка": "youtube.com/channel/UCtknBn6ZkD-hGvL1KuVlUyA", 
  "Nostalgic memories": "youtube.com/channel/UCz7A7jiQI5HA22h3a1NiB4w", 
  "welcome to the Real": "youtube.com/channel/UCH4EvOS7Hz_vlCqwCNebAMQ"
}
```

Great, next step, check all those profiles. Found nothing and almost OSINTed innocent people ^^'
I got stucked and came back to the website to look something more...

I looked on GitHub for the name function. Why? I was thinking for 1 hour and this idea came like the previous 10 silly ideas came in... But this time, it worked ^^'

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_1_github.png" width="70%"/>
</p>

That name is the quality mark of the URSS. I will also use its translation to cyrillic alphabet (znak-kachestva = знак качества)

Then I had the GitHub profile which was created 4 days ago and only has that repository:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_2_gitprofile.png" width="70%"/>
</p>

I was focused on getting something on the YouTube videos (spoiler, it was an outstanding rabbit hole). In my mind I thought I could get its YouTube account and the get an e-mail from that, so I used all the comments in the video looking for _знак качества_ or znak-kachestva and similars, but nothing showed up, also the profile photo of any YouTube account matches with the one on GitHub.

Lots of other OSINT work was done, using VK, Yandex reverse image search to the user avatar, still nothing, only a worthless account on something called kwork.

I used sherlock for get more information on other social:

- GitHub : https://github.com/znak-kachestva
- kwork : https://kwork.ru/user/znak-kachestva


I closed all YouTube and website variables, it was time for some great GitHub OSINT. I started by inspecting the complete repo, all the commits, comments, atc. But nothing there.

First I tried to retrieve the email from the GitHub information. To do so, I used the API with the following GET request. You can use it by replacing it with the GitHub username:

```
https://api.github.com/users/<github-username>/events/public
```

In this case:

https://api.github.com/users/znak-kachestva/events/public


<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_5_api.png" width="70%"/>
</p>

The email didn't showed up, that is because they have the privacy setting by default:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_5_api.png" width="70%"/>
</p>

Then a favolous idea came to my mind, what if the email from the first challenge is the one used here?
Can we retrieve a GitHub user by its email? Yes, we do.
I didn't find any post about this (I'm sure there is one), so I will explain how it works:


GitHub account discovery by email:

1. Create a dummy GitHub repo
2. Make a commit, but replace the author and use the target email for GitHub to resolve it.
3. Check the commit information to find the GitHub user.


That's the idea, we will take advantage of GitHub resolving the emails to get an account (if it exists), so in this case, if _tovarishch123@protonmail.com_ is znak-kachestva on GitHub, it will resolve it and its profile will come up. This is the steps:


1. Create a dummy GitHub repo

```
$ git clone <email_guess repo>

$ cd email_guess/
/email_guess$ ls
foo.txt  README.md

/email_guess$ subl foo2.txt

/email_guess$ git add *

/email_guess$ git commit -m "bar" --author="foo <tovarishch123@protonmail.com>"
[master 6a09937] bar
 Author: foo <tovarishch123@protonmail.com>
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 foo2.txt

/email_guess$ git push origin master
```

See who is here:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_4_attack.png" width="70%"/>
</p>

On the .patch of the commit hash (https://github.com/kashmir54/email_guess/commit/6a09937ed880e8201aec387aedf815ab8cbce418.patch) we can see that the email is related with the account:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_7_patch.png" width="70%"/>
</p>

```
From 6a09937ed880e8201aec387aedf815ab8cbce418 Mon Sep 17 00:00:00 2001
From: foo <tovarishch123@protonmail.com>
Date: Sat, 28 Aug 2021 20:58:48 -0400
Subject: [PATCH] bar

---
 foo2.txt | 0
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 foo2.txt

diff --git a/foo2.txt b/foo2.txt
new file mode 100644
index 0000000..e69de29
```

Using ProtonMail web interface I exported the cert (asc file) and imported it in kleopatra to retrieve the information:

<p align="center">
  <img src="/images/writeups/YauzaCTF2021/OSINT/2_5_pgp.png" width="70%"/>
</p>


```
-----BEGIN PGP PUBLIC KEY BLOCK-----
Comment: ID de usuario: tovarishch123@protonmail.com <tovarishch123@protonmail.com>
Comment: Creado:  24/08/2021 18:34
Comment: Tipo:  2.048-bit RSA
Comment: Uso: Firmado, Cifrado, ID de la certificación
Comment: Huella digital:  41F36197E7E9425DF5039983A8D85CBFAF8BAAD0

mQENBGElH6MBCAC8xJSVy7p2zaP0+BpxKqm8gXwLllK10xpnCqFCMap0I2tJOZ7w
77brAhDHgvrgFUZKVq6TwL67+Opagvw+FOXIM9jMY1tIegDT4ClOvAq0qu4xl1eU
ngkVjof2nZOXOGWmOMMucg85kW/KbICrKMS4qmkMX/SDNvdGgG2TEfFLFk5L36LN
RkrdcldfMO1pBuC0ryYcpkln3ECghU7C5IQ/to1YmWAdTBrjnWIsHy3tx5CDc8kf
jHFwWPTPD6ohVl9Nn04r/BBvIGSXHHOrF1H0mYl3+mX/6QrNcGfv0nNKNjEh68ua
gQh5kvQbgpaHJ8Mfp10hhrAS1TaWmMJt6bHvABEBAAG0O3RvdmFyaXNoY2gxMjNA
cHJvdG9ubWFpbC5jb20gPHRvdmFyaXNoY2gxMjNAcHJvdG9ubWFpbC5jb20+iQFN
BBABCAAgBQJhJR+jBgsJBwgDAgQVCAoCBBYCAQACGQECGwMCHgEAIQkQqNhcv6+L
qtAWIQRB82GX5+lCXfUDmYOo2Fy/r4uq0JvgB/9RSO6oEGDm5vR6/AARczZ438r+
/V6xabT4kbGv/kfLmVEbKbLNMR9DJbM2aQHoOcgrM6FcYN3JQrtQyCAeAwoNrcvZ
daGMX0J+IchBkViUnCaduWOaoAlLAdBj5ouGmrOJ6qXsiWLy0nM3JytSLYishhGi
8V5ZjXZkJrQMcdj5vZse1o7Oy0LBH1aP845w5clNChQYkkqU79voPPOLqipzcN+U
frmwYGnhCp9wdRTnT56OcX+4HOQ7gPddx1UgaF7Zk9SGTJGDaxdSK49X44NgXSjW
tPN9shYwoAaQJFgqonukdIDsBD7k4zaywlUpD70Ecpcivu9Iw4xKa5U4XvTxuQEN
BGElH6MBCAC4xc84HEQWerKr5Ae4QvU1lGmnzYMoPCRyAnXgCLSdx2GcQxziDXKV
fGSWP+LfVZBSiD8S0r6KMK5LM4oyzhCJBS7CFAje5lP/YVfvH/RRUtXzIP0Oie/6
diltjGjtiAnS25Rrwxk5oavSIHAtvVwiY8op1WLoXTeE4ef/MAiftRsTiAEUs+Nn
tH52fz+9h9Ppcq7Rj7iyKKGxm/6Ls/sLLjFAf8E3cQ18Tagq+A/UkNowUfXx9/Sh
Ek02MBBcYS1XboyjxXkr66DUabllUtwhjGLX5vrcESTtlhZjYsqEjQY+/uBBHXqa
toNeg8YFa/8+dwUj45Fqe7/tqiBCN3arABEBAAGJATYEGAEIAAkFAmElH6MCGwwA
IQkQqNhcv6+LqtAWIQRB82GX5+lCXfUDmYOo2Fy/r4uq0LhlB/9gP5JvRscfKoCU
jsiGyOsp7Z0PmiLq8EwTPnqthkRj+qru56uQFp4bFtNAOWC1NtM1LtFKV33DrMYg
bfWLwSKWraMOMepFZmnQaxE+YvovDrfxncUJIyTtWtVzP52zr9FaSV7G4BtNyef5
a6ddRvAWMGt5bq5hZ4hNhWLQK3I2lKWm0yuFxfZhNqh5JAaNpFzOTaZDrGlFql6y
8+3rEkVjkzKiByaJqA4eHffzP4TJ4TvSJnsVf75eVArWKvoOibq5pbE27QoD+zSG
JqXADgRb7Oo38eROQo724Gt0T7hHr7hiVEh9hlZskhTDY4iC+CU3pGhhkMvtCsLC
KYPZgz/r
=Vupe
-----END PGP PUBLIC KEY BLOCK-----
```

Tried this one and the first version of the key I found with the ProtOSINT tool and none of them worked :(

```
YAUZActf{xsBNBGElH6MBCAC8xJSVy7p2zaP0+BpxKqm8gXwLll}
YAUZActf{mQENBGElH6MBCAC8xJSVy7p2zaP0+BpxKqm8gXwLll}
```

I was about to quit, the [RazviOverflow](https://razvioverflow.github.io/) came in and suggested to go for the GitHub SSH, PGP and GPG keys that could be accessed from the API instead of using the e-mail. 

You have different ways of accessing user keys:

- https://github.com/<github-username>.keys
- https://github.com/<github-username>.gpg
- https://api.github.com/users/<github-username>/events/public
- https://api.github.com/users/<github-username>/gpg_keys

From the last one I could get this output:

```json
[
  {
    "id": 1444378,
    "primary_key_id": null,
    "key_id": "FDB8EDCF2AC7E589",
    "raw_key": "-----BEGIN PGP PUBLIC KEY BLOCK-----\r\nVersion: Keybase OpenPGP v1.0.0\r\nComment: YuazaCTF{u53_p6p_f0r_53cr37_m3554635}\r\n\r\nxo0EYSVYlQEEANaoJXa6DarxrVc8OWpexZxAq0z8a7/7wPu3whJxDEoj8CKeyjdE\r\nzCyceXhxcAogNzG+sTWsEtU5kSVEQTezCgvfbmmgO6dV0zvWotYBYS/gC3zpd4fZ\r\nGsewxhC48RkRzCu1x96iEntUL3QHpB2FqfuT0ezyL07qaqXz5oGhdNCZABEBAAHN\r\nVXpuYWsta2FjaGVzdHZhIChZdWF6YUNURnt1NTNfcDZwX2Ywcl81M2NyMzdfbTM1\r\nNTQ2MzV9KSA8dG92YXJpc2hjaDEyM0Bwcm90b25tYWlsLmNvbT7CrQQTAQoAFwUC\r\nYSVYlQIbLwMLCQcDFQoIAh4BAheAAAoJEP247c8qx+WJuOYD/Rhpj7TeflHpl7q0\r\n6RH49WCPRNek2lRWk3xui7opU8n/Nl578eeiJbZspgnfjGObNBLnlHrj/sAJhGGM\r\nIoPHcX6gAptZsJgbT56+h3AWBgK6VwkFDI/rt89PCL6eQ3cHzSja6xzLwLjXk6Ms\r\nbLTXrq7hb0q8JCLzM5WhUtB3qTa0zo0EYSVYlQEEAMDjWFpuU/tgaf4dX0O0paDk\r\nFsbTAsiYwlZlVWJEQoP+jbvyrEIJkCYhkXgs1csly95gas+P8+NG2YzytVJy9KtQ\r\ncjHxRWCO7JfXpmAIDAi+299aKlqOopabuViGOaFX8bVRsK2KI7RHAhW5vB262n5C\r\naCinsfc4+VIW5Kk7DeuNABEBAAHCwIMEGAEKAA8FAmElWJUFCQ8JnAACGy4AqAkQ\r\n/bjtzyrH5YmdIAQZAQoABgUCYSVYlQAKCRC4/NIsU9Q4+ouwA/sF2Thq4VPLfdfw\r\nVI/DzPphF8mEobyakjoxzl3rK7WYDJwfu5o4XlCsCQj7U6tTeAvu1TSNyzVCFot+\r\nJ0XGrqw3D7Kypz/XNQecB8rQla12uKFoMbQzOBkYVmJZ7GuiAaaVBHl+RM35H4m/\r\nNPKKZjQ02X3fGRtUDkX1xkVkG4oQ5SYSBACbGYcO8+Or9OMWXNOJZDf5jqwuwa7/\r\nZlfVxsomtbxE6TixrMQRtUzz3So8h9oYO4o6MpLESHju4rtIqlOf7H7oQ+dlA18b\r\nr69dsE1NjE+nbBExHao42LI2UIGxtPJYMFl0vir+/LW0xdnIPk7gymn7bE7+xz53\r\nELygfI7UQ8xew86NBGElWJUBBAC/VnjzIe8YOGb//pOfkb0jY5grGMvxKHINgpst\r\nxvgQTkRXjUsQTIOn+E65w59IqbtbXYGdzyZ/wOT1vEy+9C6OIan1ogBDGhhifVoB\r\nitSnyl9Gmo5aCXvU9QHSamywAAM60USWbh1loa7c2nkGYsngwMh3OgEsybwRASuC\r\ng2mpowARAQABwsCDBBgBCgAPBQJhJViVBQkPCZwAAhsuAKgJEP247c8qx+WJnSAE\r\nGQEKAAYFAmElWJUACgkQIV3WhrucA/5wYgP7B2fefe9GbIHg517szsLet0l28rxz\r\nZNUl7hu29XTCWkQij64AM9PJ8Jlf9pd5VKcqpr/tMNMkU4/pBoG3PMab0jcjNToE\r\n3KH1OS1RH7xDIVU3o9fqji90xopFCNGIoJtaEnlOjtwCGk2G42X5CLipZumTU/Yx\r\nthvx/Nc4XtJqALYu6wP/VASHBpw2kXkPI+mC7Lo/czqGQp4p0QzQyeIemyxp7Frp\r\nvKCoCJmU4buu/4kbB4J+Ocu6jfTT07RSTqGic061zzYBaOTqY++s4BmeCSA9GASQ\r\njo7lVzprh43aDQOhAV08M6QJrWAjIsBQLb4xpQIzcFkYcBqIQJ2ZAUeF5BHlWyo=\r\n=COnb\r\n-----END PGP PUBLIC KEY BLOCK-----\r\n",
    "public_key": "xo0EYSVYlQEEANaoJXa6DarxrVc8OWpexZxAq0z8a7/7wPu3whJxDEoj8CKeyjdEzCyceXhxcAogNzG+sTWsEtU5kSVEQTezCgvfbmmgO6dV0zvWotYBYS/gC3zpd4fZGsewxhC48RkRzCu1x96iEntUL3QHpB2FqfuT0ezyL07qaqXz5oGhdNCZABEBAAE=",
    "emails": [
      {
        "email": "tovarishch123@protonmail.com",
        "verified": true
      }
    ],
    "subkeys": [
      {
        "id": 1444379,
        "primary_key_id": 1444378,
        "key_id": "B8FCD22C53D438FA",
        "raw_key": null,
        "public_key": "zo0EYSVYlQEEAMDjWFpuU/tgaf4dX0O0paDkFsbTAsiYwlZlVWJEQoP+jbvyrEIJkCYhkXgs1csly95gas+P8+NG2YzytVJy9KtQcjHxRWCO7JfXpmAIDAi+299aKlqOopabuViGOaFX8bVRsK2KI7RHAhW5vB262n5CaCinsfc4+VIW5Kk7DeuNABEBAAE=",
        "emails": [

        ],
        "subkeys": [

        ],
        "can_sign": true,
        "can_encrypt_comms": true,
        "can_encrypt_storage": true,
        "can_certify": false,
        "created_at": "2021-08-25T20:15:55.000Z",
        "expires_at": null
      },
      {
        "id": 1444380,
        "primary_key_id": 1444378,
        "key_id": "215DD686BB9C03FE",
        "raw_key": null,
        "public_key": "zo0EYSVYlQEEAL9WePMh7xg4Zv/+k5+RvSNjmCsYy/Eocg2Cmy3G+BBORFeNSxBMg6f4TrnDn0ipu1tdgZ3PJn/A5PW8TL70Lo4hqfWiAEMaGGJ9WgGK1KfKX0aajloJe9T1AdJqbLAAAzrRRJZuHWWhrtzaeQZiyeDAyHc6ASzJvBEBK4KDaamjABEBAAE=",
        "emails": [

        ],
        "subkeys": [

        ],
        "can_sign": true,
        "can_encrypt_comms": true,
        "can_encrypt_storage": true,
        "can_certify": false,
        "created_at": "2021-08-25T20:15:55.000Z",
        "expires_at": null
      }
    ],
    "can_sign": true,
    "can_encrypt_comms": true,
    "can_encrypt_storage": true,
    "can_certify": true,
    "created_at": "2021-08-25T20:15:55.000Z",
    "expires_at": null
  }
]
```

Some funny guy placing that fake flag :))))))), anyway I used _https://github.com/znak-kachestva.gpg_ to get a clean key instead of that JSON format and got his public key:


-----BEGIN PGP PUBLIC KEY BLOCK-----

xo0EYSVYlQEEANaoJXa6DarxrVc8OWpexZxAq0z8a7/7wPu3whJxDEoj8CKeyjdE
zCyceXhxcAogNzG+sTWsEtU5kSVEQTezCgvfbmmgO6dV0zvWotYBYS/gC3zpd4fZ
GsewxhC48RkRzCu1x96iEntUL3QHpB2FqfuT0ezyL07qaqXz5oGhdNCZABEBAAHN
VXpuYWsta2FjaGVzdHZhIChZdWF6YUNURnt1NTNfcDZwX2Ywcl81M2NyMzdfbTM1
NTQ2MzV9KSA8dG92YXJpc2hjaDEyM0Bwcm90b25tYWlsLmNvbT7CrQQTAQoAFwUC
YSVYlQIbLwMLCQcDFQoIAh4BAheAAAoJEP247c8qx+WJuOYD/Rhpj7TeflHpl7q0
6RH49WCPRNek2lRWk3xui7opU8n/Nl578eeiJbZspgnfjGObNBLnlHrj/sAJhGGM
IoPHcX6gAptZsJgbT56+h3AWBgK6VwkFDI/rt89PCL6eQ3cHzSja6xzLwLjXk6Ms
bLTXrq7hb0q8JCLzM5WhUtB3qTa0zo0EYSVYlQEEAMDjWFpuU/tgaf4dX0O0paDk
FsbTAsiYwlZlVWJEQoP+jbvyrEIJkCYhkXgs1csly95gas+P8+NG2YzytVJy9KtQ
cjHxRWCO7JfXpmAIDAi+299aKlqOopabuViGOaFX8bVRsK2KI7RHAhW5vB262n5C
aCinsfc4+VIW5Kk7DeuNABEBAAHCwIMEGAEKAA8FAmElWJUFCQ8JnAACGy4AqAkQ
/bjtzyrH5YmdIAQZAQoABgUCYSVYlQAKCRC4/NIsU9Q4+ouwA/sF2Thq4VPLfdfw
VI/DzPphF8mEobyakjoxzl3rK7WYDJwfu5o4XlCsCQj7U6tTeAvu1TSNyzVCFot+
J0XGrqw3D7Kypz/XNQecB8rQla12uKFoMbQzOBkYVmJZ7GuiAaaVBHl+RM35H4m/
NPKKZjQ02X3fGRtUDkX1xkVkG4oQ5SYSBACbGYcO8+Or9OMWXNOJZDf5jqwuwa7/
ZlfVxsomtbxE6TixrMQRtUzz3So8h9oYO4o6MpLESHju4rtIqlOf7H7oQ+dlA18b
r69dsE1NjE+nbBExHao42LI2UIGxtPJYMFl0vir+/LW0xdnIPk7gymn7bE7+xz53
ELygfI7UQ8xew86NBGElWJUBBAC/VnjzIe8YOGb//pOfkb0jY5grGMvxKHINgpst
xvgQTkRXjUsQTIOn+E65w59IqbtbXYGdzyZ/wOT1vEy+9C6OIan1ogBDGhhifVoB
itSnyl9Gmo5aCXvU9QHSamywAAM60USWbh1loa7c2nkGYsngwMh3OgEsybwRASuC
g2mpowARAQABwsCDBBgBCgAPBQJhJViVBQkPCZwAAhsuAKgJEP247c8qx+WJnSAE
GQEKAAYFAmElWJUACgkQIV3WhrucA/5wYgP7B2fefe9GbIHg517szsLet0l28rxz
ZNUl7hu29XTCWkQij64AM9PJ8Jlf9pd5VKcqpr/tMNMkU4/pBoG3PMab0jcjNToE
3KH1OS1RH7xDIVU3o9fqji90xopFCNGIoJtaEnlOjtwCGk2G42X5CLipZumTU/Yx
thvx/Nc4XtJqALYu6wP/VASHBpw2kXkPI+mC7Lo/czqGQp4p0QzQyeIemyxp7Frp
vKCoCJmU4buu/4kbB4J+Ocu6jfTT07RSTqGic061zzYBaOTqY++s4BmeCSA9GASQ
jo7lVzprh43aDQOhAV08M6QJrWAjIsBQLb4xpQIzcFkYcBqIQJ2ZAUeF5BHlWyo=
=COnb
-----END PGP PUBLIC KEY BLOCK-----

And use it as input for the flag, we got it:

YAUZActf{xo0EYSVYlQEEANaoJXa6DarxrVc8OWpexZxAq0z8a7}

HAHAHAHA so the challenge description said to use this format YAUZActf{} and we couldn't validate it because thay changes the description with the new flag format. FFFFFFFFFFFFFFFFFFFFFFFF:

```YauzaCTF{xo0EYSVYlQEEANaoJXa6DarxrVc8OWpexZxAq0z8a7}```

That's it, hope you liked those OSINT challs, great learning for the GitHub accounts.