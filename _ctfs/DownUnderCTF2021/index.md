---
title: "CTF - DownUnderCTF 2021"
categories: [ctf]
date: 2021-09-25
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - DUCTF 2021
---

# DownUnderCTF 2021

<p align="center">
  <img src="/images/writeups/DUCTF2021/logo.png" width="30%"/>
</p>

Welcome! I've participated on my own in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! Loved the OSINT ones!

---

Challenge index:


### Misc
- [Twitter](#twitter)


### OSINT
- [Who goes there?](#who-goes-there)
- [Get over it!](#get-over-it)
- [Apartment Views](#apartment-views)
- [(back) On the rails](#back-on-the-rails)
- [eyespy](#eyespy)
- [Sharing is Caring](#sharing-is-caring)


### Forensic
- [Retro](#retro)
- [Do the loop!](#do-the-loop)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# Misc

## Twitter
100
socials
If you have been paying attention to our Twitter, we have been using flags in the background of our graphics this year. However, one of these flags stands out a bit more than the rest! Can you find it?

While you're there, why don't you give us a follow to keep up to date with DownUnderCTF!

On the twitter they have several images. The one about the prices have a highlighted flag:

When over the images in full screen and I was able to notice the lighter green in the bottom right of [this tweet](https://twitter.com/DownUnderCTF/status/1436141450509500428)

<p align="center">
  <img src="/images/writeups/DUCTF2021/Misc/1_flag.jpeg" width="70%"/>
</p>

``` DUCTF{EYES_ON_THE_PRIZES_TWITTER} ```


---

# OSINT

## Who goes there?
100
beginner
Disclaimer: Please note that this storyline, including any previous or future additions are all fictional and created solely for this challenge as part of DownUnder CTF. These are real places however they have no association/affiliation to the event, you are not required to call any place or make contact with anyone, doing so may disqualify you from the event.

Welcome to the team, glad you chose to join us - hopefully you’ll like it here and want to stay. Let me tell you about your first task:

We’ve observed an underground criminal RaaS operation calling back to this domain, can you find the number of the individual who registered the domain?

646f776e756e646572.xyz

Flag format is DUCTF{+61<number>}

Author: xXl33t_h@x0rXx

If we are looking for information related to a domain name, we need to take a look at the WHOIS information. It usually contains names, telephones, emails related with the ownership of a certain domain.

A quick [who.is](https://who.is/whois/646f776e756e646572.xyz) query will get our information:

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/0_whois.png" width="70%"/>
</p>

``` DUCTF{+61420091337} ```


## Get over it!
100
easy

Bridget loves bridges, this one is her favourite.

What is the name of it and the length of its main span to the nearest metre?

Flag format; DUCTF{the_bridge_name-1337m}


We get a photo of a place in the riverside with a cable bridge:

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/get-over-it.jpg" width="70%"/>
</p>

Quick exiftool just in case there is some interesting metadata:

```
kali@kali:~/Desktop/CTFs/DUCTF2021/OSINT$ exiftool get-over-it.jpg 
ExifTool Version Number         : 12.16
File Name                       : get-over-it.jpg
Directory                       : .
File Size                       : 2.2 MiB
File Modification Date/Time     : 2021:09:24 10:47:47-04:00
File Access Date/Time           : 2021:09:24 10:48:04-04:00
Y Resolution                    : 96
Exif Byte Order                 : Big-endian (Motorola, MM)
XP Comment                      : TRY HARDER!
Padding                         : (Binary data 2060 bytes, use -b option to extract)
Image Width                     : 2268
Image Height                    : 4032
```

Nothing but try harder. So I use to use Google Lens as a first approach with the mobile phone and found some similarities with the _Eleanor Schonell Bridge_ and it's on Australia, so could be right!

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/1_0_lens.jpg" width="50%"/>
</p>

Now on Google Maps I tried to have the same perspective of the bridge and had a look to other years images, just in case there was a better similarity in the colors of the image and check out this one from Mar 2016. Same structure, base, cables, riverside look:

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/1_2_maps.png" width="80%"/>
</p>

Now that we have the name, I did a quick google search with the keywords: _eleanor schonell bridge dimensions_ and found the height:

```
The bridge consists of a single main span of 185 m and multiple approach spans at each end of up to 75 m, giving a total length of 390 m. The deck is about 11 m above the Brisbane River's high tide level.
```

``` DUCTF{eleanor_schonell-185m} ```


## Apartment Views
108
medium

We received intel that the accomplice we have been following is meeting with the ringleader of RaaS operation, they shared this photo online a few years back - believed to be the view from their apartment building (hideout).

Between you and me, our sources say the ringleader was testing the ransomware they were going to sell and instead infected their own laptop.

We think we may be able to setup a sting to apprehend the ringleader and retrieve the decryption key when they meet the accomplice at a dead drop in a nearby alleyway. Can you work out what the name of the street is?

Flag format: DUCTF{street_name}


They provide us the following photo (I've made the markings):

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/image.png" width="80%"/>
</p>

In the photo we see a set of buildings, two of them have letters on them and also the brown one has an unique look.
Trying to see whats in the letters by enhancing the image on Photoshop didn't help too much. By zooming in the original photo we cann see that two of three letters on white are **AN.** so with that information I use Google Lens again but limiting the search to that building since the rest look normal and the results will be biased.

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_0_lens.png" width="70%"/>
</p>

It really make the difference when croping the photo to highlight the most peculiar sights, buildings or objects. In this challenge didn't find any results using the complete photo, so that was a game changer. Finally we identify the tower and the bank (**ANZ**) so we searched on Google for _ANZ_ and two buildings showed up once in Sydney and the other in Melbourne, obviously, ours is the one in Melbourne.

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_1.png" width="70%"/>
</p>

Now with the 3D maps we can get [the perspective of the image](https://www.google.es/maps/@-37.8098054,144.9596064,162a,35y,165h,72.04t/data=!3m1!1e3!5m1!1e4) to look in the correct way. We can see the corresponding buildings with the ones in the original photo. To activate the 3D view from Google Maps in the menu by activating Globe View:

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_hint.png" width="40%"/>
</p>

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_2_buildings.png" width="70%"/>
</p>

Looking to the yellow building with squared windows, we can see the apartments in front of them where the photo was taken (red dot):

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_4_view.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/3_3_alley.png" width="70%"/>
</p>

It could be Little Lonsdale St or McLean Alley, turn out to be McLean:

``` DUCTF{mclean_alley} ```


## (back) On the rails
352
easy
We intercepted further communications between the two. This text was sent shortly before we lost track of one of the suspects, with an image attached. Can you work out what they're talking about?

Okay, please promise not to judge me, but I might have ended up catching the wrong train again. Though I think I'm still in Australia this time (at least it isn't like that time in Norway LOL). I managed to snap a picture before we went past this station… you have any ideas where I might be?

Please tell us the name of the station, with any spaces replaced by underscores.

Flag format: DUCTF{station_name}

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/image2.png" width="70%"/>
</p>

double track railways australia
Pedestrian bridge

Could not get it :(
Key of this challenge was to look for abandoned, overgrown, derelict station in Australia, if you were great enought, you could use the previous challenge to narrow down your search by locating the suspect in "Victoria" state.

[Full writeup](https://github.com/DownUnderCTF/Challenges_2021_Public/blob/main/osint/)



## eyespy
404
medium

With the phone number you found, we were able to see their phone pinging off a cell tower. We intercepted their call and need to track down the registration number of the plane that their accomplice left on and and where the flight was heading. We recorded their call and were given the following transcript, can you analyse it for us?

Flag format: DUCTF{plane-registration-number_flight destination}

Transcript:


Settling in well since moving from SA? 14:31 _Moving from SA - South Australia_

IH - I don’t know, he was supposed to meet me here 8 minutes ago. 14:35 _Meet at 14:27_

UM - Damn his flight leaves very soon, are you sure he’ll make it? 14:36 

IH - I’ve got to bounce, he is here, I’ll call you back in a few minutes 14:37 _Meets at 14:37_

--

IH - Yeah, one that’ll touch down in about an hour 15:10 _Airplane landing 16:10~_

Could not get it since I didn't solve a previous challlenge and missed some important hints in the series.


## Sharing is Caring
427
hard

We have identified a username associated with one of the suspects: alex_elgato93. They seem pretty active on social media. Surely with all these posts they must have slipped up somewhere and given us a way to work out their location?

Please tell us the name of the closest street, with any spaces replaced by underscores.

Flag format: DUCTF{street_name}

So Alex is on [Twitter](https://twitter.com/alex_elgato93).
I used twint to retrieve all his tweets and try to see if any of those had location or something but hit a dead end. 

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/4_0_twitter.png" width="70%"/>
</p>

```
twint -u alex_elgato93 -o ../file.json --json
```

```json
{"id": 1438063287002861572, "conversation_id": "1438063287002861572", "created_at": "2021-09-15 04:53:00 EDT", "date": "2021-09-15", "time": "04:53:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "being on the run kinda sucks tbh... 🥱🥱", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1438063287002861572", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436869923708362753, "conversation_id": "1436869923708362753", "created_at": "2021-09-11 21:51:00 EDT", "date": "2021-09-11", "time": "21:51:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "What's a secret agent's go-to fashion? Spyware.", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436869923708362753", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436334646598508547, "conversation_id": "1436334646598508547", "created_at": "2021-09-10 10:24:00 EDT", "date": "2021-09-10", "time": "10:24:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "Why are people afraid of computers? They byte.", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436334646598508547", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436321560604790787, "conversation_id": "1436321560604790787", "created_at": "2021-09-10 09:32:00 EDT", "date": "2021-09-10", "time": "09:32:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "could anyone give me some tips on using msfvenom? the script i found isnt doing anything", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436321560604790787", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436291371900440576, "conversation_id": "1436291310357401607", "created_at": "2021-09-10 07:32:02 EDT", "date": "2021-09-10", "time": "07:32:02", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "my favourite character", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436291371900440576", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436291310357401607, "conversation_id": "1436291310357401607", "created_at": "2021-09-10 07:31:48 EDT", "date": "2021-09-10", "time": "07:31:48", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": " https://t.co/K2Hslixi3G", "language": "und", "mentions": [], "urls": [], "photos": ["https://pbs.twimg.com/media/E-67Q_yVEAcaLG7.png"], "replies_count": 1, "retweets_count": 0, "likes_count": 3, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436291310357401607", "retweet": false, "quote_url": "", "video": 1, "thumbnail": "https://pbs.twimg.com/media/E-67Q_yVEAcaLG7.png", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436290614044217348, "conversation_id": "1436290614044217348", "created_at": "2021-09-10 07:29:02 EDT", "date": "2021-09-10", "time": "07:29:02", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "my friend was telling me about this really niche hacker show, irobot or something?", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436290614044217348", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436208566118109184, "conversation_id": "1436208566118109184", "created_at": "2021-09-10 02:03:00 EDT", "date": "2021-09-10", "time": "02:03:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "way more puns coming your way, don't worry.", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436208566118109184", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436192207904469013, "conversation_id": "1436192207904469013", "created_at": "2021-09-10 00:58:00 EDT", "date": "2021-09-10", "time": "00:58:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": ".-- .... -.--/.-- .- .../- .... ./.... .- -.-. -.- . .-.  .../.... .. -.- .. -. --./- .-. .. .--./.. -. - . .-. .-. ..- .--. - . -.. ..--../- .... . .-. ./.-- .- .../.-/..-. .. .-. . .-- .- .-.. .-.. .-.-.-", "language": "und", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 0, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436192207904469013", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436183400071716866, "conversation_id": "1436183400071716866", "created_at": "2021-09-10 00:23:00 EDT", "date": "2021-09-10", "time": "00:23:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "57687920646964207468652062616e64206e65766572206765742061206769673f204974207761732063616c6c656420313032334d422e20", "language": "it", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 4, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436183400071716866", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436128035267358721, "conversation_id": "1436128035267358721", "created_at": "2021-09-09 20:43:00 EDT", "date": "2021-09-09", "time": "20:43:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "Maybe I'll start encoding all my puns...", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436128035267358721", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1436021835552858120, "conversation_id": "1436021835552858120", "created_at": "2021-09-09 13:41:00 EDT", "date": "2021-09-09", "time": "13:41:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "I can't resists. Here, see if you can decode the answer! I bet you can't: VXNlIGEgYm90bmV0Lg==", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1436021835552858120", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435996921785557000, "conversation_id": "1435996921785557000", "created_at": "2021-09-09 12:02:00 EDT", "date": "2021-09-09", "time": "12:02:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "Oops. Forgot to post the follow up. Noone liked my post anyway 😢", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435996921785557000", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435963888479076353, "conversation_id": "1435963685034291201", "created_at": "2021-09-09 09:50:44 EDT", "date": "2021-09-09", "time": "09:50:44", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "I'll post the next part once I hit 100 likes!", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 2, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435963888479076353", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435963685034291201, "conversation_id": "1435963685034291201", "created_at": "2021-09-09 09:49:56 EDT", "date": "2021-09-09", "time": "09:49:56", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "What’s the best way to catch a runaway robot?", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 1, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435963685034291201", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435962387098570754, "conversation_id": "1435962387098570754", "created_at": "2021-09-09 09:44:46 EDT", "date": "2021-09-09", "time": "09:44:46", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "new pun coming up in 5 minutes", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 1, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435962387098570754", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435961438762266625, "conversation_id": "1435961438762266625", "created_at": "2021-09-09 09:41:00 EDT", "date": "2021-09-09", "time": "09:41:00", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "anyone got any hacker puns to share?", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 2, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435961438762266625", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435958888113737742, "conversation_id": "1435958888113737742", "created_at": "2021-09-09 09:30:52 EDT", "date": "2021-09-09", "time": "09:30:52", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "I hired a guy to teach me all about the dark web.   He was just my tormentor.", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 2, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435958888113737742", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
{"id": 1435922409060638720, "conversation_id": "1435922409060638720", "created_at": "2021-09-09 07:05:55 EDT", "date": "2021-09-09", "time": "07:05:55", "timezone": "-0400", "user_id": 1435905683354841090, "username": "alex_elgato93", "name": "Alexandros Elgato", "place": "", "tweet": "Hello World", "language": "en", "mentions": [], "urls": [], "photos": [], "replies_count": 0, "retweets_count": 0, "likes_count": 2, "hashtags": [], "cashtags": [], "link": "https://twitter.com/alex_elgato93/status/1435922409060638720", "retweet": false, "quote_url": "", "video": 0, "thumbnail": "", "near": "", "geo": "", "source": "", "user_rt_id": "", "user_rt": "", "retweet_id": "", "reply_to": [], "retweet_date": "", "translate": "", "trans_src": "", "trans_dest": ""}
```

Wrapping up:

- [Twitter](https://www.reddit.com/user/alexandros-elgato/) - Twitter search for _alex_elgato93_
https://www.reddit.com/user/alexandros-elgato/
- [Reddit](https://www.reddit.com/user/alexandros-elgato/) - Google by searching for his Twitter name: _Alexandros Elgato_
- [Interesting screenshot](https://i.redd.it/d8tjy7453om71.png) - On reddit profile
- MAC Address: de:73:2c:6c:1b:c1 - On reddit [screenshot](https://i.redd.it/d8tjy7453om71.png)
- [GitHub](https://github.com/alexandros-elgato) - Search on GitHub for his reddit user _alexandros-elgato_

Actions:
- [Waybackmachine over Twitter profile](https://web.archive.org/web/20210909134800/https://twitter.com/alex_elgato93) shows one result, but no track on there
- Wayback on reddit profile had no results
- GitHub searches:
  https://github.com/alexandros-elgato.keys
  https://github.com/alexandros-elgato.gpg
  https://api.github.com/users/alexandros-elgato/gpg_keys
  https://api.github.com/users/alexandros-elgato/events/public
  https://github.com/alexandros-elgato/Hello-World/commit/87cd5d6cd9c12aabfb6eadcf743be491cf2dff10.patch

I got stucked for some time so I went over all findings again.
Then the MAC address cought my attetion, so made some searches. When talking about networks, always give wigle a try.

Look what ye had with that MAC!

[Wigle query](https://wigle.net/search?netid=de%3A73%3A2c%3A6c%3A1b%3Ac1)

<p align="center">
  <img src="/images/writeups/DUCTF2021/OSINT/4_0_mac.png" width="70%"/>
</p>

His location: Charles Mcintosh PKWY

``` DUCTF{charles_mcintosh_pkwy} ```

---

# Forensic

## Retro

Our original logo was created in paint, I wonder what other secrets it hides?

They provide us the og.jpg

<p align="center">
  <img src="/images/writeups/DUCTF2021/Forensic/og.jpg" width="80%"/>
</p>

Using exiftool we can see the details

```
exiftool og.jpg 

Color Transform                 : YCbCr
Exif Byte Order                 : Big-endian (Motorola, MM)
Artist                          : DUCTF{sicc_paint_skillz!}
XP Author                       : DUCTF{sicc_paint_skillz!}
Padding                         : (Binary data 2060 bytes, use -b option to extract)
X Resolution                    : 300
Displayed Units X               : inches
```

``` DUCTF{sicc_paint_skillz!} ```


## Do the loop!
100
easy

Do the loop doop doop...

Flag format: The STRING you end up with after solving challenge, case insensitive.

We got a WAV file with a morse code on the background. As it is, morse2ascii tool cannot decode it properly since the morse beeps are low against the music and voices. Then I tried to apply low and high filters in Audacity to try to isolate 1000Hz-1010HZ but morse was still difused with the overall noise. So I went to SonicVisualizer and get the spectrogram for getting some hints or ideas.
By appliying the following visualization settings to the spectrogram I could get a great visualization of the dots and lines:

<p align="center">
  <img src="/images/writeups/DUCTF2021/Forensic/2_1_filter.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/writeups/DUCTF2021/Forensic/2_0_sonic.png" width="80%"/>
</p>

See morse code format at the 1000Hz. I found it to be a short message so I stopped the idea of isolation and started to write them down as I could. Some of the beeps were missed due to interferences with other noises, but looking to the overall message we could get the flag:

```
.. -.-. --- ..- .-.. -.. .-.. .. ... - . -. - --- - .... .. ... --- -. .-.. --- --- .--. .- .-.. .-.. -.. .- -.--
```

``` ICOULDLISTENTOTHISONLOOPALLDAY ```


