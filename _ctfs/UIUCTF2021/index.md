---
title: "CTF - UIUCTF 2021"
categories: [ctf]
date: 2021-02-20
comments: true
description: CTF - UIUCTF 2021
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# UIUCTF 2021

<p align="center">
  <img src="/images/writeups/UIUCTF2021/logo2.png" width="40%"/>
</p>


Hello everybody, I've participated in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got all the OSINT challenges! I had few hours for this CTF, but enjoyed it and stablished some OSINT methodology. Hope these writeups help you!

<p align="center">
  <img src="/images/writeups/UIUCTF2021/rank.png" width="70%"/>
</p>

---

Challenge index:


### OSINT

- [OSINT The Creator](#osint-the-creator)
- [Chaplin's PR Nightmare - 1](#chaplins-pr-nightmare---1)
- [Chaplin's PR Nightmare - 2](#chaplins-pr-nightmare---2)
- [Chaplin's PR Nightmare - 3](#chaplins-pr-nightmare---3)
- [Chaplin's PR Nightmare - 4](#chaplins-pr-nightmare---4)
- [Chaplin's PR Nightmare - 5](#chaplins-pr-nightmare---5)
- [Chaplin's PR Nightmare - 6](#chaplins-pr-nightmare---6)
- [Chaplin's PR Nightmare - 7](#chaplins-pr-nightmare---7)
- [Chaplin's PR Nightmare - 8](#chaplins-pr-nightmare---8)

### Web

- [wasmbaby](#wasmbaby)

### Meta

- [Welcome to UIUCTF'21](#welcome-to-uiuctf21)

### Jail

- [phpfuck](#phpfuck)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>


---


# OSINT

## OSINT The Creator
50
osint

There is a flag on a few of the organizer's profiles. Find it!

author: Thomas

I looked for Thomas on Discord and found his username, then on his profile an spoiler text will display the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/0_1_flag.png" width="40%"/>
</p>

``` uiuctf{@b0uT_m3_suppOrT5_maRkD0wN} ```



## Chaplin's PR Nightmare - 1
53
osint beginner

Charlie Chaplin has gotten into software development, coding, and the like... He made a company, but it recently came under fire for a PR disaster. He got all over the internet before he realized the company's mistake, and is now scrambling to clean up his mess, but it may be too late!! Find his Twitter Account and investigate! NOTE THAT THESE CHALLENGES DO NOT HAVE DO BE DONE IN ORDER!

The inner content of this flag begins with "pe"


With a quick search on Twitter with its name and software or coding, the result poped up (https://twitter.com/ChaplinCoding):

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/1_1_tw.png" width="40%"/>
</p>


The last tweet says something about twitter lists, so let's check the out:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/1_2_lists.png" width="50%"/>
</p>

The second lists have the flag on the description!

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/1_3_flag.png" width="50%"/>
</p>


I will place here all information found and where:

[Twitter account](https://twitter.com/ChaplinCoding)
[YouTube account](https://www.youtube.com/channel/UCxPyHVMa8TyKrOj05x86osA) - On Twitter bio
[Dev website](https://www.charliechaplin.dev/) - On Youtube Channel description
[Imgur](https://imgur.com/user/ChaplinDevelopment) - On an imgur link in the about page
[LinkedIn](https://www.linkedin.com/in/charlie-chaplin-dev/detail/recent-activity/) - Contact form on the website: _You can also find us on Linkedin, but you will have to search for us I forget what our name is, either like "Charlie Chaplin Coding or Development" or "C3D" something like that._
[GitHub](https://github.com/charliechaplindev) - Searching for Charlie Chaplin on Github
Email: charliechaplin.dev@gmail.com - On a commit message on GitHub


``` uiuctf{pe@k_c0medy!} ```


## Chaplin's PR Nightmare - 2
50
osint beginner

Charlie made an advertisement to promote his company, he is using the modern media platform YouTube to present it! Can you find it?

The inner content of this flag begins with "ch"


We found its [ad](https://www.youtube.com/watch?v=LniQBHja9bw) on twitter bio. Watched the video to see if any flag is in there and yes, the flag showed off at the end:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/2_1_flag.png" width="70%"/>
</p>

We also found its [website](https://www.charliechaplin.dev/) on the Youtube channel description.

``` uiuctf{ch@plin_oN_th3_tV!!} ```



## Chaplin's PR Nightmare - 3
50
osint

Charlie even has a website!!! Find it and look around!

The inner content of this flag begins with "ch"


We saw its [website link](https://www.charliechaplin.dev/) on the Youtube channel description.

There we can see 3 pages, home, about and contact:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/3_1_web.png" width="70%"/>
</p>

Oh wait... What is in this photo?

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/3_2_flag.png" width="70%"/>
</p>

Check it closely:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/3_3_flag.png" width="70%"/>
</p>

``` uiuctf{ch@pl1n_i5_eL337} ```


## Chaplin's PR Nightmare - 4
50
osint

Charlie left another flag on his company's website. Maybe you have to reach out to him???

The inner content of this flag begins with "w3"

I went to the contact form, the only way to reach him (?)

On the contact Google Form we saw this note: _Authors Note (Because Yall Deserve to learn): You can enumerate email lists from contact forms sometimes in a way functionally identical to this. Its not common but I've seen it more than once._

I filled up the contact form with random inputs and got the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/4_1_flag.png" width="70%"/>
</p>

``` uiuctf{w3_d0_nOt_v@lu3_yoUR_1nPuT} ```


## Chaplin's PR Nightmare - 5
50
osint

Charlie also has an imgur account that may hold the secret to his PR nightmare... Can you find it?

The inner content of this flag begins with "tH"


I started looking to any imgur link on the website and found one on the about page:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/5_1_about.png" width="70%"/>
</p>

I check his photos on imgur and [one of those](https://imgur.com/gallery/AKY3PUS) had a comment with the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/5_2_flag.png" width="70%"/>
</p>


``` uiuctf{tH3_pR_p0Lic3_h@vE_cAugHt_Up?!} ```



## Chaplin's PR Nightmare - 6
85
osint

Wow Charlie even set up a linkedin account, but not well it is kind of a mess. Is the PR nightmare here??

The inner content of this flag begins with "pr"

Yes, we knew the LinkedIn account from previous challenges: Contact form on the website: _You can also find us on Linkedin, but you will have to search for us I forget what our name is, either like "Charlie Chaplin Coding or Development" or "C3D" something like that._


I found a group searching for C3D:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/6_0_c3d.png" width="70%"/>
</p>

In his profile it says that it hosted other event in the past called Top hat developement night!

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/6_1_profile.png" width="40%"/>
</p>

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/6_2_hosted.png" width="40%"/>
</p>

Here is the [Linkedin page](https://www.linkedin.com/events/6822753659445743616/)


<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/6_3_flag.png" width="50%"/>
</p>


``` uiuctf{pr0f3s5iOn@l_bUs1n3sS_envIroNm3n7} ```



## Chaplin's PR Nightmare - 7
56
osint

Chaplin left some code up on GitHub which may be the actual PR nightmare I hope not though... Leaked code is bad. Find it!!!

The inner content of this flag begins with "th"


I looked for his GitHub and found it witht he follofing quote: "Charlie Chaplin":

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/7_0_git.png" width="80%"/>
</p>

I was looking around its repositories and the commits, and found and interesting one with "Secret keys" and "Ok phew resolved issue":

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/7_1_commits.png" width="80%"/>
</p>

On the secret keys I found a private RSA key:

```
-----BEGIN RSA PRIVATE KEY-----
MIICWwIBAAKBgQDCpNBvn8Bi8Hn/eWMaB22NlzX5Bj2NdJC/iQn4BDfV4F5TwouZ
R09PTNVff4p+mZj9mTbqHb80PAH3RITGeKidyHLdjm9Lbgzq1qJT7tUw4L6Nh2bb
9k2mXk415zY/08HFNFKrR4dZUA89qmsIC9FrVltV5T8ZzIzKOex3/BSffQIDAQAB
AoGAG3szpp7yhcWYI/bahCFKWXRg/XDhHei4SyKpEqhdzceo6cCM/y4vR238rkgO
oMNE84E/XJyok1JpoKS+7lq4y5b/a6Ssf8iKyaGjhm4ProcFOB9wZyb0q/3oP2oH
sfZeg5BchUDpcr78lI/IIds5WB/YdyipNrjLxTQaNJAJZ00CQQDwBRB615KBBByf
ncOKDf78yoxAyZyCgW5fCc82sw/J5K0ccC/K0izBUtA0dVkndp4uX9qut6e4Kd2U
w8T8qUWrAkEAz5paujmrrCPi7OwMqAFObE055DByLuKFAVXnvzplkmK55EGcdDHS
b2FxvTW92Y8jHIMTYFnQCRW05ngAUci3dwJAUQl0sfT8uuCXhXCvp3noQJNmXS10
0T8/84iNVKuKRiEWLYbXLARgJfl+4GdKbZ+H8qJcZCcP0ocYD/U1GhVQtwJASHVd
WUYrrwdPetXgjPr3vkEuBYELOWYCejbbjhj5vqAKtapYyw1gn1NpeVq6iqa04Iy3
RJfUTndvDkmwAtuFfQJAWidPSpezXMcD7tyGiz4UZNOkImyApevveM+aOytndNK1
GWeWiM4oGV3rlOFg1iMu8F9JBcJEj+OniBekFnRULQ==
-----END RSA PRIVATE KEY-----
```

And on the other I found the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/7_3_flag.png" width="80%"/>
</p>

I used the hint: OUtlNDQ4ME1pY1U= from base64: 9Ke4480MicU

``` uiuctf{th3_TrUe_pR_N1gHtm@r3} ```



## Chaplin's PR Nightmare - 8
390
osint extreme

Straightup doxx Charlie by finding the email he set all these accounts up with, and investigate it.

The inner content of this flag begins with "b0"

We found the email on a GitHub commit: *charliechaplin.dev@gmail.com*
Flag? With Gmail? I like it. Let's retrieve the Google ID and check if it has any contribution on GMaps:

To do it, we can use the Google Hangouts contacts to get it. First, open the developer tool and load the network information. Then search for the email account and look for the batchexecute requests and check the response. You will find the ID on one of the data structure received:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/8_0_googleid.png" width="90%"/>
</p>

I used to do this Hangouts/Contact method to retrieve the Google ID, but you can use this [tools](https://tools.epieos.com/email.php) to get it, which is outstanding, but for your knowledge, I described it previously and you can find futher information in this [outstanding post](https://medium.com/week-in-osint/getting-a-grasp-on-googleids-77a8ab707e43):

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/8_1_id.png" width="60%"/>
</p>

Checked the Google Maps profile (which you can craft with the Google ID) and saw a [contribution](https://www.google.com/maps/contrib/117833363030761934622/photos/@41.9667449,-87.6385934,3a,104.2y,90t/data=!3m7!1e2!3m5!1sAF1QipOyi7R_ApFvR6ZeSYVpLYt4p_2wDjLu6r9V3T3T!2e10!6shttps:%2F%2Flh5.googleusercontent.com%2Fp%2FAF1QipOyi7R_ApFvR6ZeSYVpLYt4p_2wDjLu6r9V3T3T%3Dw545-h260-k-no!7i956!8i456!4m3!8m2!3m1!1e1) with a photo:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/8_2_contrib.png" width="90%"/>
</p>

A closer look to write down the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/OSINT/8_3_flag.png" width="90%"/>
</p>

``` uiuctf{b0rk_b0rk_1_lOv3_mY_d0g<3} ```

---


# Meta

## Welcome to UIUCTF'21
1
beginner

Welcome to UIUCTF'21! Your flag can be found on this very page.


I looked for the backgound picture in the HTML:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/Meta/1_1_back.png" width="70%"/>
</p>

And saw the flag

<p align="center">
  <img src="/images/writeups/UIUCTF2021/Meta/1_2_flag.png" width="70%"/>
</p>


``` uiuctf{secret_pictures} ```


---

# Web

## wasmbaby
50
web beginner

wasm's a cool new technology! http://wasmbaby.chal.uiuc.tf

I took a look into the Web Assembly file (wasm), but the developer tools could not retrieve it:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/Web/1_1_wb.png" width="70%"/>
</p>

So I downloaded it from its [URL](wasmbaby.chal.uiuc.tf/index.wasm). Then I use the strings command to check its content and obtained the flag:

```

kali@kali:~/Desktop/CTFs/UIUCTF2021/Web$ strings index.wasm 
...
jkA~j 
jApq"
jApq$
kApq"
hello, world!
-+   0X0x
(null)
0123456789ABCDEF-0X+0X 0X-0x+0x 0x
uiuctf{welcome_to_wasm_e3c3bdd1}
```

``` uiuctf{welcome_to_wasm_e3c3bdd1} ```


---

# Jail

## phpfuck
50
jail php beginner

i hate php

http://phpfuck.chal.uiuc.tf

We can see the code of the website that has LFI. Then it says that the flag is on /flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/Jail/1_1_source.png" width="70%"/>
</p>

So we set the path and get the flag:

<p align="center">
  <img src="/images/writeups/UIUCTF2021/Jail/1_2_flag.png" width="70%"/>
</p>

``` uiuctf{pl3as3_n0_m0rE_pHpee} ```
