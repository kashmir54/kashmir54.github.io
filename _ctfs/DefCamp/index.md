---
title: "CTF - DefCamp 2020"
categories: [ctf]
date: 2020-12-07
comments: true
image: /images/writeups/DefCamp/1_logo.png
description: CTF - DefCamp 2020
---

# DefCamp 2020

<p align="center">
  <img src="/images/writeups/DefCamp/1_logo.png" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) with my mates [RazviOverflow](https://razvioverflow.github.io/) and [liti0s](https://litios.github.io/) got some challenges!

<p align="center">
  <img src="/images/writeups/DefCamp/2_rank.png" width="500"/>
</p>


Solved challs:

- [alien-inclusion (Web)](#alien-inclusion)
- [stug-reference (Stego)](#stug-reference)
- [basic-coms (Forensic)](#basic-coms)
- [why-xor (Crypto)](#why-xor)
- [bro64 (Crypto)](#bro64)

Close to the flag:

- [am-I-crazy (Web)](#am-i-crazy)
- [qr-mania (networking)](#qr-mania)

---

# Web

## alien-inclusion

Keep it local and you should be fine. The flag is in /var/www/html/flag.php.

Flag format: CTF{sha256}


Accessing to the main page we get:

{% highlight php %}

<?php

  if (!isset($_GET['start'])){
      show_source(__FILE__);
      exit;
  } 

  include ($_POST['start']);
  echo $secret; 
?>

{% endhighlight %}

The code suggests 2 main approaches: First, set the GET parameter called start to avoid the exit instruction. And the last thing is to use the include() function to include the file where the flag is. We can do this by indicating the filename on the POST parameter called start.

Since the flag file is at /var/www/html/flag.php, we assume that the index file we have accessed is on the same html directory, therefore we don't need to indicate it. By using curl command, we can set the POST parameter after -d option with the syntax 'param_name=param_value' and the GET parameter with the syntax after the URL '?param_name=param_value'. With this being said, the final command to execute is:

```
kali@kali:DefCamp$ curl -d 'start=flag.php' http://34.89.211.188:32294?start=123
```

``` ctf{b513ef6d1a5735810bca608be42bda8ef28840ee458df4a3508d25e4b706134d} ```


## am-I-crazy

You might see but you cannot feel.

Flag format: CTF{sha256}


Okey, we land into a website which ask for some text:

<p align="center">
  <img src="/images/writeups/DefCamp/Web/1_page.png" width="500"/>
</p>

For 'asdfasdf' we get the following output:

{% highlight php %}
  10 <?=
  define('WORKING_DIRECTORY', getcwd());

  $var = <<<xd
  0
  xd;

  echo $var;

  register_shutdown_function(function() {
      chdir(WORKING_DIRECTORY);
      if (empty($_GET['tryharder'])) {
          $_GET['tryharder'] = 0;
          show_source(__FILE__);
      }
      if (strlen($_GET['tryharder']) > 15){
          $_GET['tryharder'] = 0;
      }
      $contents = file_get_contents(__FILE__);
      $search_pattern = '/\$var = <<<xd\s*(.*)\s*xd/im';
      preg_match($search_pattern, $contents, $matches);
      
      $new_contents = preg_replace_callback($search_pattern, 
      	function($matches) {
          	return str_replace($matches[1], $_GET['tryharder'], $matches[0]);
      }, $contents);
      file_put_contents(__FILE__, $new_contents, LOCK_EX);
  }); 
{% endhighlight %}

We can see that a GET parameter called **tryharder** is requested with two different behabiours:

If no **tryharder** parameter, show the sourcefile else:

A regex is used to retrieve a certain fragment from the php code. Then it is used at the _preg_match_ function, where it process the actual PHP file as content as well as the regex to output the result at _$matches_ variable.

Now we move into a _preg_replace_callback_ function, which takes the regular expresion _$search_pattern_ and the anonymous function to execute. This function will take as input the obtained result of the regex and finally the _$content_ with the php. Finally it will save into _$new_contents_ the operations made on the function.

This function replaces the inner group of the matched expression. So, what happens next? 
The regex is made up of two groups in this case, first (group 0) the overall match, which will be the whole blue part on the following image. And the second group (group 1) is related to the part within the parenthesis and tinted on green.

<p align="center">
  <img src="/images/writeups/DefCamp/Web/2_regex.png" width="500"/>
</p>


Therefore it will replace that 0 with the content of **tryharder** parameter and return the page generated after replacing it with what we want for then echo it. 
Remember, we have the 15 chars limitation... so we have to manage the payload to get something in there.

What's that **<<< xd xd;** syntax at PHP? That is [Heredoc](https://www.php.net/manual/en/language.types.string.php#language.types.string.syntax.heredoc). There you can read variables and other kind of things. 

{% highlight php %}
echo <<<EOT
My name is "$name". I am printing some $foo->foo.
Now, I am printing some {$foo->bar[1]}.
This should print a capital 'A': \x41
EOT;
{% endhighlight %}

I couldn't get this task, but we could learn what "backticks" are on PHP. Anything within backticks (\`anything\`) will be executed as a shell command without using _shell_exec()_ function. Therefore the solution could be to use as payload some of the following commands:

```
${`cat *>2`}
${print`cat 2`}
```

---

# Stego

## stug-reference

Do you have your own stug pass hidden within?

Flag format: ctf{sha256}

<p align="center">
  <img src="/images/writeups/DefCamp/Stego/stug.jpg" width="500"/>
</p>

We can see on the image the text in upper case: STEG HIDDE. Which sounds like the famous stego tool steghide. This tool will require a password to extract the embedded secret. The main though could be to use rockyou.txt wordlist to bruteforce the algorithm. stegcracker is a tool which will help us with this task. It will retrieve the wordlist from the default path in Kali systems (/usr/share/wordlists/rockyou.txt). You can specify the wordlist to use by setting the filepath at the end of stegcracker command.

```
stegcracker stug.jpg
```

Okey, the bruteforce is taking too long. Let's move for hints with the chall statement or the image. We can see a tank going out, the model of that tank could be 'stug'. So candidate to passwords may be 'tank', 'stug' and 'cave' for example. Using steghide command with extract operation and indicating the stego file (-st) we introduce the candidates. The password was 'stug' and steghide extracted a file called flag.txt with the flag inside.

```
kali@kali:~/Desktop/CTFs/DefCamp/Stego$ steghide extract -sf stug.jpg 
Enter passphrase: stug
wrote extracted data to "flag.txt".
```
``` ctf{32849dd9d7e7b313c214a7b1d004b776b4af0cedd9730e6ca05ef725a18e38e1} ```

---

# Forensic

## basic-coms

Look for it and you shall find the flag.

Flag format: CTF{sha256}

For this chall we have a pcap file with a set of TCP and UDP packets. We used Expert Information tool on Wireshark's Analisys tab and find out some HTTP taffic within the trace.

<p align="center">
  <img src="/images/writeups/DefCamp/Forenic/1_basic.jpg" width="500"/>
</p>

One of the requests is an HTTP GET, with a curious text on the 'important' parameter. Let's decode the URL using an [online URL decoder](https://meyerweb.com/eric/tools/dencoder/)

Raw request:

```
GET /?important=The%20content%20of%20the%20f%20l%20a%20g%20is%20ca314be22457497e81a08fc3bfdbdcd3e0e443c41b5ce9802517b2161aa5e993%20and%20respects%20the%20format HTTP/1.1\r\n
```

Decoded request:

```
/?important=The content of the f l a g is ca314be22457497e81a08fc3bfdbdcd3e0e443c41b5ce9802517b2161aa5e993 and respects the format
```

As the request states, let't craft the flag and introduce it:

``` ctf{ca314be22457497e81a08fc3bfdbdcd3e0e443c41b5ce9802517b2161aa5e993} ```

---

# Crypto

## why-xor

Let's be fair, we all start with XOR, and we keep enjoying it.

Flag format: CTF{sha256}

A python file is provided:

```
{% highlight python %}

xored = ['\x00', '\x00', '\x00', '\x18', 'C', '_', '\x05', 'E', 'V', 'T', 'F', 'U', 'R', 'B', '_', 'U', 'G', '_', 'V', '\x17', 'V', 'S', '@', '\x03', '[', 'C', '\x02', '\x07', 'C', 'Q', 'S', 'M', '\x02', 'P', 'M', '_', 'S', '\x12', 'V', '\x07', 'B', 'V', 'Q', '\x15', 'S', 'T', '\x11', '_', '\x05', 'A', 'P', '\x02', '\x17', 'R', 'Q', 'L', '\x04', 'P', 'E', 'W', 'P', 'L', '\x04', '\x07', '\x15', 'T', 'V', 'L', '\x1b']
s1 = ""
s2 = ""
# ['\x00', '\x00', '\x00'] at start of xored is the best hint you get
a_list = [chr(ord(a) ^ ord(b)) for a,b in zip(s1, s2)]
print(a_list)
print("".join(a_list))

{% endhighligh %}
```

Check out the hint... since the flag starts with **ctf**, the first 3 chars at the xored list are 0, therefore, the key starts with 'ctf' and it could be the whole key. Why we know that? Remember XOR properties, when something is XORed with its own (for example c xor c) the result is 0.
We developed a little script to go over all the xored list and using the 3 char key 'ctf'. Since the list xored is much bigger than our key, we use itertools.cycle(key) to pad the remaining positions by repeating the key like the following image:

<p align="center">
  <img src="/images/writeups/DefCamp/Crypto/1_xor.jpg" width="500"/>
</p>

```
{% highlight python %}
import itertools

xored = ['\x00', '\x00', '\x00', '\x18', 'C', '_', '\x05', 'E', 'V', 'T', 'F', 'U', 'R', 'B', '_', 'U', 'G', '_', 'V', '\x17', 'V', 'S', '@', '\x03', '[', 'C', '\x02', '\x07', 'C', 'Q', 'S', 'M', '\x02', 'P', 'M', '_', 'S', '\x12', 'V', '\x07', 'B', 'V', 'Q', '\x15', 'S', 'T', '\x11', '_', '\x05', 'A', 'P', '\x02', '\x17', 'R', 'Q', 'L', '\x04', 'P', 'E', 'W', 'P', 'L', '\x04', '\x07', '\x15', 'T', 'V', 'L', '\x1b']
key = 'ctf'
flag = [chr(ord(a) ^ ord(b)) for a,b in zip(xored, itertools.cycle(key))]
print("".join(flag))

{% endhighligh %}
```

By executing the script, we retrive the result with the flag

``` ctf{79f107231696395c004e87dd7709d3990f0d602a57e9f56ac428b31138bda258} ```


## bro64

Betaflash let’s go in Cuba and dance amigo !!

Flag format: CTF{sha256}
{% highlight json %}
{
	"nonce": "wpUq2dKfUzs=", 
	"ciphertext": "oy5LG9jXkyS3xVVeCJ/mWhjxYDFMql0vu4CUryzziKc46PjrdEzqETLdnYU5TeM2ykJsu+16GjF2ZFi7DIJ7eMeU0g3j", "key": "Fidel_Alejandro_Castro_Ruz_Cuba!"
}
{% endhighlight %}

Okey, after long time figuring out what cipher was used on this, we came to a conclusion that it has to be Salsa20 or Chacha20, first by the properties (the use a 12 bytes nonce and 32 bytes key) and also the chall statement, since salsa and chachacha are latin dance types. Both are stream ciphers.

This turned out to be pretty easy, but the unknown has difficulties, we tried both (Salsa20 and Chacha20) implementations on python3 with Crypto packages. Beware, nonce and cyphertext has to be base64 decoded to introduce it into the cypher since they were presented on base64 and not the byte representation.

Further doc can be found at the [PyCryptodome](https://pycryptodome.readthedocs.io/en/latest/src/cipher/salsa20.html) and [asecuritysite](https://asecuritysite.com/encryption/salsa20), very usefull.

{% highlight python %}

from Crypto.Cipher import Salsa20
from Crypto.Cipher import ChaCha20
from base64 import b64encode, b64decode

nonce = b'wpUq2dKfUzs='
secret = b'Fidel_Alejandro_Castro_Ruz_Cuba!'
ciphertext = b'oy5LG9jXkyS3xVVeCJ/mWhjxYDFMql0vu4CUryzziKc46PjrdEzqETLdnYU5TeM2ykJsu+16GjF2ZFi7DIJ7eMeU0g3j'

cipher = Salsa20.new(key=secret, nonce=b64decode(nonce))
plaintext = cipher.decrypt(b64decode(ciphertext))
print(f'Salsa20 \n{plaintext}\n')

cipher = ChaCha20.new(key=secret, nonce=b64decode(nonce))
plaintext = cipher.decrypt(b64decode(ciphertext))
print(f'Chacha20 \n{plaintext}')

{% endhighligh%}


```
kali@kali:~/Desktop/CTFs/DefCamp/Crypto$ python3 bro2.py 
Salsa20 
b'$d\x1fd\xb8Oo\xc8\xb7\x96B\x98cWY\xea\xf2A\x13\x95\xe8d\x0e6\xdc\xf1\x01\xd3-H\x17\x892\xdc\x90\xc5\r\n\xee\xcc\x88Y\x9fP\x01oX\xcb\xfe\xb8}O\x96\xfc\xce\xbb\x88\xc4\x83m|J!\xd7\xbda;`G'

Chacha20 
b'ctf{f38deb0782c0f252090a52b2f1a5b05bf2964272f65d5c3580be631f52f4b3e0}'
```

``` ctf{f38deb0782c0f252090a52b2f1a5b05bf2964272f65d5c3580be631f52f4b3e0} ```

---

# Networking

## qr-mania

We intercepted some weird requests. See if you can extract some useful information.

Flag format: CTF{sha256}

We can see a lot of qr codes on the GET requests by using the HTTP export option from Wireshark.

<p align="center">
  <img src="/images/writeups/DefCamp/Net/1_qrs.png" width="700"/>
</p>

Once all objects are retrieved, we use the following script to change the color to white and black and use the python library to decode them. 

<p align="center">
  <img src="/images/writeups/DefCamp/Net/2_qrs.png" width="700"/>
</p>

{% highlight python %}
from pyzbar.pyzbar import decode
from PIL import Image
import os

def to_black_n_white(img_path):

  im = Image.open(img_path)
  im = im.convert("RGBA")
  pix = im.load()
  background_color = pix[1,1]
  qr_color = ()
  for y in range(im.size[1]):
      for x in range(im.size[0]):
        #If background color then set to white, else black:
          if pix[x, y] == background_color:
              pix[x, y] = (255, 255, 255, 255)
          else:
            qr_color = pix[x, y]
            pix[x, y] = (0, 0, 0, 255)

  im.save('./out/'+img_path)
  return im, background_color, qr_color

text_sum = []

files = [x for x in os.listdir('HTTPObj') if 'png' in x]
files.sort()
for png_file in files:
  image, bg, qr_color = to_black_n_white('HTTPObj/'+ png_file)
  decoded_char = str(decode(image)[0].data)[2]
  text_sum.append(decoded_char)
print(text_sum)
print(''.join(text_sum))

{% endhighlight %}

Once decoded, we bring them up together and get the flag, but it's scrambled. No alphabetical sorting works, filenames are not usefull, the rest of HTTP trafic don't show ideas or information further than the HTML with al the files listed in alphabetical order.

```
kali@kali:~/Desktop/CTFs/DefCamp/Net$ python3 script.py 

C: suvwivhtpjkcdpcdurty.png (72, 154, 141, 255)	(71, 92, 209, 255)
T: wbuhqpnwfuovgdwoedoc.png (79, 161, 63, 255)	(85, 71, 81, 255)
F: hchwxnsotuqrtbrdmbmg.png (123, 88, 69, 255)	(203, 195, 91, 255)
{: lmmfdbfmheysbhbgjazn.png (194, 211, 222, 255)(160, 128, 65, 255)
}: czguxctbmqgfgxhvnwzr.png (196, 81, 205, 255)	(90, 196, 192, 255)
['6', '0', '4', 'd', '2', '}', '7', '5', 'd', '1', '7', '1', 'c', 'b', '2', 'e', '2', '2', '3', 'F', '6', 'f', 'a', 'f', 'f', 'a', 'e', 'f', 'b', '5', '1', 'f', 'e', '9', '3', '{', '6', 'b', 'd', 'b', '0', '9', 'd', '8', '6', '8', 'b', '2', 'a', 'b', '2', 'f', 'C', '7', '6', 'c', '3', 'T', 'd', '8', 'c', '6', '5', '9', '5', '7', 'f', '9', '5']
604d2}75d171cb2e223F6faffaefb51fe93{6bdb09d868b2ab2fC76c3Td8c65957f95

```

I tried many things like looking for the RGB components to order the QRs, and many other methods that I cannot remember. 
The final solution was to look for a comment using _exiftool_ command. This comment has the position for each QR code and that could give us the flag... Well, better to go over the simple steps first and don't stick to crazy ideas at the beginning!

That's all. Thanks for reading!!!