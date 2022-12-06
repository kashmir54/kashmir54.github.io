---
title: "HTB - Blocky"
categories: [box]
date: 2022-12-06
comments: true
image: /images/walkthroughs/hackthebox/blocky/logo.png
favicon: /images/walkthroughs/hackthebox/blocky/logo.png
description: HTB - Blocky walkthrough
---

# Blocky 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/banner.png" width="70%"/>
</p>

## Summary

Blocky is an easy box where we need to find the plugins web directory to find two minecraft plugins in .jar files. Decompiling those plugins we can see some hardcoded credentials for the SQL database. Using those credentials we can log into wordpress and PHPMyAdmin but also log into the SSH for user notch.

Once inside we run sudo -l to see that the user can run all commands with the password, so sudo su will give us the root shell. I don't know it that is intended, but that is the easy solution.


## Enumeration

- nmap

```bash
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-06 07:08 EST
Stats: 0:03:47 elapsed; 0 hosts completed (1 up), 1 undergoing Service Scan
Service scan Timing: About 75.00% done; ETC: 07:12 (0:00:41 remaining)
Nmap scan report for 10.129.228.45
Host is up (0.041s latency).
Not shown: 65530 filtered tcp ports (no-response)
PORT      STATE  SERVICE   VERSION
21/tcp    open   ftp?
22/tcp    open   ssh       OpenSSH 7.2p2 Ubuntu 4ubuntu2.2 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   2048 d6:2b:99:b4:d5:e7:53:ce:2b:fc:b5:d7:9d:79:fb:a2 (RSA)
|   256 5d:7f:38:95:70:c9:be:ac:67:a0:1e:86:e7:97:84:03 (ECDSA)
|_  256 09:d5:c2:04:95:1a:90:ef:87:56:25:97:df:83:70:67 (ED25519)
80/tcp    open   http      Apache httpd 2.4.18
|_http-title: Did not follow redirect to http://blocky.htb
|_http-server-header: Apache/2.4.18 (Ubuntu)
8192/tcp  closed sophos
25565/tcp open   minecraft Minecraft 1.11.2 (Protocol: 127, Message: A Minecraft Server, Users: 0/20)
Service Info: Host: 127.0.1.1; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

We can see the hostname blocky.htb, let's add it to /etc/hosts

- gobuster

```bash
 gobuster dir -u "http://blocky.htb" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20 -x php,html
===============================================================
Gobuster v3.3
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://blocky.htb
[+] Method:                  GET
[+] Threads:                 20
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.3
[+] Extensions:              html,php
[+] Timeout:                 10s
===============================================================
2022/12/06 07:10:18 Starting gobuster in directory enumeration mode
===============================================================
/.hta.php             (Status: 403) [Size: 293]
/.hta.html            (Status: 403) [Size: 294]
/.hta                 (Status: 403) [Size: 289]
/.htaccess            (Status: 403) [Size: 294]
/.htaccess.php        (Status: 403) [Size: 298]
/.htaccess.html       (Status: 403) [Size: 299]
/.htpasswd.php        (Status: 403) [Size: 298]
/.htpasswd            (Status: 403) [Size: 294]
/.htpasswd.html       (Status: 403) [Size: 299]
/index.php            (Status: 301) [Size: 0] [--> http://blocky.htb/]
/javascript           (Status: 301) [Size: 313] [--> http://blocky.htb/javascript/]
/phpmyadmin           (Status: 301) [Size: 313] [--> http://blocky.htb/phpmyadmin/]
/plugins              (Status: 301) [Size: 310] [--> http://blocky.htb/plugins/]
/readme.html          (Status: 200) [Size: 7413]
Progress: 10958 / 14142 (77.49%)[ERROR] 2022/12/06 07:10:44 [!] Get "http://blocky.htb/index.php": context deadline exceeded (Client.Timeout exceeded while awaiting headers)
/server-status        (Status: 403) [Size: 298]
/wiki                 (Status: 301) [Size: 307] [--> http://blocky.htb/wiki/]
/wp-admin             (Status: 301) [Size: 311] [--> http://blocky.htb/wp-admin/]
/wp-content           (Status: 301) [Size: 313] [--> http://blocky.htb/wp-content/]
/wp-config.php        (Status: 200) [Size: 0]
/wp-blog-header.php   (Status: 200) [Size: 0]
/wp-cron.php          (Status: 200) [Size: 0]
/wp-includes          (Status: 301) [Size: 314] [--> http://blocky.htb/wp-includes/]
/wp-load.php          (Status: 200) [Size: 0]
/wp-links-opml.php    (Status: 200) [Size: 219]
/wp-mail.php          (Status: 403) [Size: 3444]
/wp-login.php         (Status: 200) [Size: 2397]
/wp-settings.php      (Status: 500) [Size: 0]
/wp-signup.php        (Status: 302) [Size: 0] [--> http://blocky.htb/wp-login.php?action=register]
/wp-trackback.php     (Status: 200) [Size: 135]
/xmlrpc.php           (Status: 405) [Size: 42]
/xmlrpc.php           (Status: 405) [Size: 42]
```

- wpscan

```bash
wpscan --url "http://blocky.htb" -e u,ap --api-token <api-key>

# A lot of vulnerabilities and also the twenty plugins that allow us to change templates and get RCE.

[i] User(s) Identified:

[+] notch
 | Found By: Author Posts - Author Pattern (Passive Detection)
 | Confirmed By:
 |  Wp Json Api (Aggressive Detection)
 |   - http://blocky.htb/index.php/wp-json/wp/v2/users/?per_page=100&page=1
 |  Author Id Brute Forcing - Author Pattern (Aggressive Detection)
 |  Login Error Messages (Aggressive Detection)
```


We land into a wordpress website that seems like a blog:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/1_0_web.png" width="90%"/>
</p>


We can also see the phpmyadmin and wp-admin logins:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/1_1_admin.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/1_2_wp.png" width="90%"/>
</p>


On gobuster's output, we can see the _/plugins_ directory, where we can find some minecraft plugins in .jar, let's decompile them:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/1_3_plugins.png" width="90%"/>
</p>

Using jd-gui decompiler we can see the following credentials:

```java
package com.myfirstplugin;

public class BlockyCore {
  public String sqlHost = "localhost";
  
  public String sqlUser = "root";
  
  public String sqlPass = "8YsqfCTnvxAUeduzjNSXe22";
  
  public void onServerStart() {}
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/1_4_creds.png" width="90%"/>
</p>


**root:8YsqfCTnvxAUeduzjNSXe22**

That password is the same as this hash found on the wp_users table:

**notch:$P$BiVoTj899ItS1EZnMhqeqVbrZI4Oq0/**

So trying to log into the box with that credential actually worked and we get the user flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/3_0_user.png" width="80%"/>
</p>


# Privesc

Since we have the password, let's try the sudo -l command. We see that we can run all comands, so sudo su will give us the root shell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/blocky/4_0_root.png" width="80%"/>
</p>