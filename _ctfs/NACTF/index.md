---
title: "CTF - NACTF2020"
categories: [ctf]
date: 2020-10-16
hasComments: true
image: /images/walkthroughs/hackthebox/traceback/traceback_logo.png
description: CTF - NACTF
---

# NACTF 2020

<p align="center">
  <img src="/images/writeups/NACTF/1_logo.jpeg" width="300"/>
</p>

I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges:

<p align="center">
  <img src="/images/writeups/NACTF/2_rank.png" width="500"/>
</p>


Interesting CTF, with entry level challenges on web and forensic (more like stego) and other hard ones, specially some programming ones that I tried. Let's get into it!

---

# Forensic

## Gummies
50

Kylie is obsessed with gummies. With her collection of miscellaneous gummy bears, she took this incredible picture which is now her phone's wallpaper. Can you find her flag?

A simple _zsteg -a_ will retrieve the flag:

<p align="center">
  <img src="/images/writeups/NACTF/Forensic/1_flag.png" width="500"/>
</p>

``` nactf{5t3gan0graphy_rul35!} ```


## Meta-morphosis
75

Mikey really likes Metamorphosis by Franz Kafka, so much so that he sent this meme to the class.

Check _exiftools_
```
kali@kali:~/Desktop/CTFs/NACTF/Forensic/Meta$ exiftool meme-3.jpg 
ExifTool Version Number         : 12.06
File Name                       : meme-3.jpg
Directory                       : .
File Size                       : 52 kB
File Modification Date/Time     : 2020:10:30 20:52:48-04:00
File Access Date/Time           : 2020:10:30 20:53:00-04:00
File Inode Change Date/Time     : 2020:10:30 20:53:00-04:00
File Permissions                : rw-r--r--
File Type                       : JPEG
File Type Extension             : jpg
MIME Type                       : image/jpeg
JFIF Version                    : 1.01
X Resolution                    : 1
Y Resolution                    : 1
Exif Byte Order                 : Big-endian (Motorola, MM)
Resolution Unit                 : None
Artist                          : nactf{m3ta_m3ta_m3ta_d3f4j}
Y Cb Cr Positioning             : Centered
Image Width                     : 500
Image Height                    : 461
Encoding Process                : Baseline DCT, Huffman coding
```

``` nactf{m3ta_m3ta_m3ta_d3f4j} ```


##  Turnips
100

Dr. J loves his ch0nky turnips, can you find his ch0nky flag?


Seems like a broken image... Let's fix it with hexedit:

Ahhhh... Nope, but there is like some kind of strange pattern on the file... Let's go for strings to see any hint:

```
kali@kali:~/Desktop/CTFs/NACTF/Forensic/Turnips$ strings turnip-for-what.jpg 
...
m4~9
Y@?X(
`."7
9#W.
5};D
x<4<?
kmS[
|6z}
byu;k
nactf{turn1p_f0r_h3x_f3j52}
```
Wops, got the flag:

``` nactf{turn1p_f0r_h3x_f3j52} ```


##  Secret Message
100

Monica loves inventing secret languages. So much so that she claims to be the only one to know the message in this recording. What does it say?

We got an mp3 file. I played it and it's morse, let's get it:

```
ffmpeg -i input.mp3 output.wav 
kali@kali:~/Desktop/CTFs/NACTF/Forensic/SecretMessage$ morse2ascii -m output.wav 

MORSE2ASCII 0.2
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org

- open output.wav
  wave size      1200478
  format tag     1
  channels:      1
  samples/sec:   48000
  avg/bytes/sec: 96000
  block align:   2
  bits:          16
  samples:       600239
  bias adjust:   -191
  volume peaks:  -30194 30194
  normalize:     2573
  resampling to  8000hz

- decoded morse data:
--.-..-...--...---./-----..-./.-......--.--...-....---....--...
```

This time I couldn't get the morse right with the _morse2ascii_, so let's head over to [Morsecode](https://morsecode.world/international/decoder/audio-decoder-adaptive.html):

<p align="center">
  <img src="/images/writeups/NACTF/SecretMessage/1_flag.png" width="500"/>
</p>

``` nactf{QU33N_0F_L4NGU4G3S} ```

##  Turnips 2
150

Uh oh.. Parth's file seems to have been corrupted. Can you figure out how to find his flag?

I run a bunch of commands over the file.txt but this was the final check. Look over with xxd and find some interesting fragments:

```
.lvG........I9D0
.......}.....Oi.
....WiCCPICC Pro
file..X..y.TT]..
...0...%........
..*).*.. ......!
b!.("...H...*(..
.....y..}.;k.{..
```

I looked in google de _WiCCPICC Profile_ and [checked](https://www.google.com/search?client=firefox-b-e&ei=AL-cX5S4MtL4aLPKiKgG&q=wiccpicc+profile&oq=wiccpicc+profile&gs_lcp=CgZwc3ktYWIQAzoHCAAQsAMQE1DiL1jiL2D7MWgAcAB4AIABb4gByAGSAQMxLjGYAQCgAQGqAQdnd3Mtd2l6yAECwAEB&sclient=psy-ab&ved=0ahUKEwiUuOmL2N3sAhVSPBoKHTMlAmUQ4dUDCAw&uact=5) that is something related with a PNG IHDR file, therefore I hexedit the file and place the correct magic bytes and headers according to the [specification](https://parsiya.net/blog/2018-02-25-extracting-png-chunks-with-go/):

```
00000000   89 50 4E 47  0D 0A 1A 0A  .PNG....
00000008   00 00 00 0D  49 48 44 52  ....IHDR
00000010   00 00 04 A3  00 00 03 7D  .......}
00000018   08 06 00 00  00 4F 69 BA  .....Oi.
00000020   C0 00 00 18  57 69 43 43  ....WiCC
00000028   50 49 43 43  20 50 72 6F  PICC Pro
00000030   66 69 6C 65  00 00 58 85  file..X.
```

<p align="center">
  <img src="/images/writeups/NACTF/Turnips2/file" width="500"/>
</p>

``` nactf{th3_turn1p5_ar3_tak17g_0v35_skf9} ```


##  Static
250

Juliet just airdropped me this really wierd photo that looks like tv static. She said this would be easier than passing notes in class, but I can't understand what she's trying to say. Help!


Rip like 5 hours... I was close, but you never know.

---

# Web

##  Login
175

Vyom has learned his lesson about client side authentication and his soggy croutons will now be protected. There's no way you can log in to this secure portal!

https://login.challenges.nactf.com/

Let's try some SQL Injections:

<p align="center">
  <img src="/images/writeups/NACTF/Web/1_sql.png" width="500"/>
</p>

``` nactf{sQllllllll_1m5qpr8x} ```


##  Cookie Recipe
150

Arjun owns a cookie shop serving warm, delicious, oven-baked cookies. He sent me his ages-old family recipe dating back four generations through this link, but, for some reason, I can't get the recipe. Only cookie lovers are allowed!


We can see a cookie into the page. Let's log in with any username:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/1_path.png" width="500"/>
</p>

Ok, no admin works, so let's try to change the cookie for the auth.php path:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/2_log.png" width="500"/>
</p>

Reload the website and there we go:

<p align="center">
  <img src="/images/writeups/NACTF/Web/Cookie/3_flag.png" width="500"/>
</p>

``` nactf{c00kie_m0nst3r_5bxr16o0z} ```


---

# Crypto

##  Caesar's Challenge
25

Zabelo wrote this message on a note he passed to me. anpgs{q3p1cu3e1at_e0px5!} He also told me his favorite number was 13. What could this mean?

Ez ROT13: ``` nactf{d3c1ph3r1ng_r0ck5!} ```


##  YAMS
100

Instead of turnips, Yavan loves YAMS. Day and night, he sings about YAMS, dreams about YAMS and runs to the store to catch the newest released batch of YAMS. Hes cryptic too. I wonder what this could mean.

Uexummq lm Vuycnqjc. Hqjc ie qmud xjas: fycfx{waY5_sp3_Y0yEw_w9vU91}

YAMS.... Is YAMS a key? Vigenere?

Welcome to Vigenere. Here is your flag: ``` nactf{yaM5_ar3_Y0mMy_w9jC91} ```



##  Oligar's Tricky RSA
175

The crypto master Oligar just sent this file with three numbers. What do they mean?

I got the file with RSA parameters:

```
c = 97938185189891786003246616098659465874822119719049
e = 65537
n = 196284284267878746604991616360941270430332504451383
```

For this I have a little roadmap with [RsaCtfTool](https://github.com/Ganapati/RsaCtfTool). Find it down below: 


```
kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --createpub -n 196284284267878746604991616360941270430332504451383 -e 65537 > ~/Desktop/CTFs/NACTF/Crypto/RSA/pub.key

kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --publickey ~/Desktop/CTFs/NACTF/Crypto/RSA/pub.key --private > ~/Desktop/CTFs/NACTF/Crypto/RSA/key

[*] Performing factordb attack on /home/kali/Desktop/CTFs/NACTF/Crypto/RSA/pub.key.

Results for /home/kali/Desktop/CTFs/NACTF/Crypto/RSA/pub.key:

Private key :
-----BEGIN RSA PRIVATE KEY-----
MHgCAQACFgCGTZu3ODf6xVEntw2V1WILWDz/KTcCAwEAAQIVJ4WzK0nZ/2wNo9Es
si23/pijS3ihAgsIev/y8NsVH+p3vwILD9Y2EXS3Fv4f7IkCCwexQq/kyGVuEOnT
AgsONFZepc8qwlij8QILCB0K8fjbLQ1JbCE=
-----END RSA PRIVATE KEY-----


kali@kali:/opt/RsaCtfTool$ python3 RsaCtfTool.py --dumpkey --key  ~/Desktop/CTFs/NACTF/Crypto/RSA/key         
private argument is not set, the private key will not be displayed, even if recovered.                        
n: 196284284267878746604991616360941270430332504451383  
e: 65537                                              
d: 57761855232773998001493320508606360547602423576737 
p: 10252256693298561414756287                   
q: 19145471103565027335990409
```

Now use this script developed over other CTFs:

```
{% highlight python %}
#!/usr/bin/env python3

import codecs

e = 65537
n = 196284284267878746604991616360941270430332504451383
d = 57761855232773998001493320508606360547602423576737 

msg = 97938185189891786003246616098659465874822119719049
flag = []

hexa = str(hex(pow(msg,d,n)))[2:]
print(hexa)

flag = codecs.decode(hexa, 'hex').decode('utf-8')
print(flag)
{% endhighlight %}
```

We can retrieve the flag:

```
kali@kali:~/Desktop/CTFs/NACTF/Crypto/RSA$ python3 text.py 
6e616374667b736e33616b795f6331706833727d0a
nactf{sn3aky_c1ph3r}
```

``` nactf{sn3aky_c1ph3r} ```



##  Random Number Generator
250

Dr. J created a fast pseudorandom number generator (prng) to randomly assign pairs for the upcoming group test. Austin really wants to know the pairs ahead of time... can you help him and predict the next output of Dr. J's prng?

Beginner foothold: The server script uses the following line:

```
{% highlight python %}
random.seed(round(time.time() / 100, 5))
{% endhighlight %}
```

The random library will generate numbers from that seed. Imagine that you use the number _1_ instead of the time as seed. You will get the same random numbers in the same order for different executions. That is why they are called pseudo-random, because they aren't real random numbers. [Read more](https://www.geeksforgeeks.org/pseudo-random-number-generator-prng/).

For this one I try to find the same seed as the server. For this task I picked various seeds within a given timeframe. I set this timeframe with my machine time and 2 offsets, -100000 and +100000. The following script will find the correct seed by comparing the first generated number. When found, we go ahead and send the following two numbers:

```
{% highlight python %}
import random, time
from pwn import *

magic_number_1, magic_number_2 = 0, 0
r = remote('challenges.ctfd.io', 30264)

print(r.recv())
r.send("r\n")
number = int(r.recv()[:-2])
print(number)
print("-----------")

#Cover a wide frame of time, checking wether I found the correct seed or not:
for i in range(-100000,100000):

	random.seed(round(time.time() / 100 + i/100000, 5))
	number_2 = random.randint(1, 100000000)

	if number_2 == number:
		print(str(number_2), i)
		magic_number_1 = random.randint(1, 100000000)
		magic_number_2 = random.randint(1, 100000000)

r.send("g\n")
print(r.recv())
r.send("{}\n".format(magic_number_1))
print(r.recv())
r.send("{}\n".format(magic_number_2))
print(r.recv())
{% endhighlight %}
```

```
kali@kali:~/Desktop/CTFs/NACTF/Crypto/RandomNumberGenerator$ python3 test.py 
[+] Opening connection to challenges.ctfd.io on port 30264: Done
b"Welcome to Dr. J's Random Number Generator!\n[r] Print a new random number\n[g] Guess the next two random numbers and receive the flag!\n[q] Quit\n\n> "
31242721
-----------
31242721 399
b'Guess the next two random numbers for a flag!\nGood luck!\nEnter your first guess:\n> '
b"Wow, lucky guess... You won't be able to guess right a second time\nEnter your second guess:\n> "
b"What? You must have psychic powers... Well here's your flag: \nnactf{ch000nky_turn1ps_1674973}\n"
```

``` nactf{ch000nky_turn1ps_1674973} ```


## Error 1
350

Pranay has decided that the previous error detection scheme is a little bit too inefficient... While eating his delicious HAM-filled Italian Sub at lunch, he came up with a new idea. Fortunately, he has also invested in a less noisy communication channel.


Worked on Hamming Code. I went deep into the basics in order to refresh the very basics. I end up using the following script and [this lib](https://github.com/dominiccarrano/hamming):

```
{% highlight python %}
from hamming import decode as dec
from bitarray import bitarray
data_str = '010011011010011010100011011001111110111101000101010011110111010101110110100110001100000111011101100100101111011010110001010011001110011010101111010111111010111010110111010111110110100110011011001101101101011101000111101000110100001010110100100001110110011110111011111101111000001100100011011010010111101100100100000011001101000001001010100000100111001011111101'
def chunks(l, n):
    # For item i in a range that is a length of l,
    for i in range(0, len(l), n):
        # Create an index range for l of n items:
        yield l[i:i+n]

#Divide in chunks of 15 bits
chunk_list = list(chunks(data_str, 15))
print(chunk_list)
final_data = bitarray()

'''
X 0 1 0
0 1 1 0
1 1 1 1 
0 0 1 1 
'''

for item in chunk_list:

	item = list(item)
	print(item)
	#This is key. The library gets 16 bit chunks, there is one misses, the first one. We calculate
	#if it's 0 or 1. We know that there is AT LEAST ONE ERROR. Therefore, we set this bit the other way around.
	if item.count('1') % 2 == 0:
		item.insert(0,'1')
	else:
		item.insert(0,'0')
	print(item)
	print('--------')
	data = bitarray(''.join(item))
	print(data)
	print(dec(data))
	final_data.extend(dec(data))

print(final_data)
{% endhighlight %}
```

I obtained the following bitarray, I introduced it into [CyberChef](https://gchq.github.io/CyberChef/) and got the flag:

```
011011100110000101100011011101000110011001111011011010000110000101101101011011010011000101101110011001110101111101100011011011110110010000110011011100110101111100110101001101000011011001101101011101100011001101110001001110010110000100110000011101000110010101111101
```

``` nactf{hamm1ng_cod3s_546mv3q9a0te} ```


##  Error 2
550

Kayla decided she wants to use the previous error detection scheme for cryptography! After computing the normal error bits, she switched them around according to a secret key.

Worked on permutations and ways to check which permutation is the right one. What I mean is, the parity bits have been permutated, they are normally located at positions [1,2,4,8], but since this implementation only uses 15 bits, they are found at positions i-1 = [0,1,3,7]. In this challenge, they picked those positions and place it at any of the other 15 positions. The foothold is to **find the right permutation**. We generate all permutations of 4 length arrays with the possible 15 positions:
```
{% highlight python %}
itertools.permutations(iterable, r=4)
{% endhighligh %}
```
For each permutation (aka. "the secret key" on the challenge description) we will pick the bits from the positions and correctly place the into their original positions in this challenge: [0,1,3,7]. For example, we have the following permutation key: (5, 7, 8, 13). The workflow is:


bit_chunk = '011010100100111'

POP position 5 from the chunk and insert it at position 0:
'01101**0**100100111' -> '**0**01101100100111'

POP position 7 from the chunk and insert it at position 1:
'0011011**0**0100111' -> '0**0**0110110100111'

POP position 8 from the chunk and insert it at position 3:
'00011011**0**100111' -> '000**0**11011100111'

POP position 13 from the chunk and insert it at position 7:
'000011011100**1**11' -> '0000110**1**1110011'


To sum up:

Positions (5, 7, 8, 13) to (0, 1, 3, 7):
'011010100100111' -> '000011011110011'

Once we change the chunk, we introduce it into the library and try to solve the error using hamming code. REMEMBER, this lib uses 16 bit chunks, we need to insert the remaining parity bit at position 0!!

We compare the output with the expected first letter, in this case the one of the flag 'n'.

This workflow is implemented on the following script along with the usage of previous hamming lib:

```
{% highlight python %}

from hamming import decode as dec
from bitarray import bitarray
import time
import binascii
import itertools

data_str = '011010100100111010010011110010110110110010010010011100001111101101101000110100110000010011100010001001110000110111100100110111111110101000101011011010000111100001000111001110010111001101011100011101111011100111111000101001110000011101011110010111111110001101110000011010010011101010010110101010001000011100101110100000101110000010110010100010101111010110001101001100001000101100100011111000100001110001100110001110101011010001111001111001101001110000110000111011001000010110001001010111111010101100010011011001110110111001100111101001001100110100100110001000101010111011101010110000111001011100111001'

def chunks(l, n):
    # For item i in a range that is a length of l,
    for i in range(0, len(l), n):
        # Create an index range for l of n items:
        yield l[i:i+n]


def text_from_bits(bits, encoding='utf-8', errors='surrogatepass'):
    n = int(bits, 2)
    return n.to_bytes((n.bit_length() + 7) // 8, 'big').decode(encoding, errors) or '\0'


def swap(bits, comb):
	#Pick the bits from the combination and insert them into the right position for parity bits
	for i, j in zip([0,1,3,7], [0,1,2,3]):
		ex = bits.pop(comb[j])
		bits.insert(i, ex)
	return bits



#Divide in chunks of 15 bits
chunk_list = list(chunks(data_str, 15))
secret_key = []

# Get all permutations of length 4 with the 15 posible positions:
iterable = [x for x in range(0,14)]
combinations = list(itertools.permutations(iterable, r=4))
print('Total permutations: ', len(list(combinations)))

#Try all permutations that could lead into the right first two bytes
for combination in combinations:

	final_data = bitarray()

	for item in chunk_list[:6]:

		item = swap(list(item), combination)
		
		#Add the first parity bit so the lib can get the 16 bits needed. 
		#We know there is one wrong, therefore we will apply it the other way around.
		if item.count('1') % 2 == 0:
			item.insert(0,'1')
		else:
			item.insert(0,'0')

		data = bitarray(''.join(item))
		#Get the result from fixing error:
		final_data.extend(dec(data))

	msg = ''.join(final_data.decode({'1':bitarray('1'), '0':bitarray('0')}))
	if msg[:48] == '011011100110000101100011011101000110011001111011':
		print(msg[:48], '= 011011100110000101100011011101000110011001111011')
		print('Combination = ', combination)
		secret_key = combination
		#The we could find 2 combinations, but only the first showed a reasonable flag.
		#We stop when find the first permutation
		break


# Now that we know the permutations, retrieve the flag
final_data = bitarray()

for item in chunk_list:
	item = swap(list(item), secret_key)
	
	if item.count('1') % 2 == 0:
		item.insert(0,'1')
	else:
		item.insert(0,'0')
	data = bitarray(''.join(item))
	final_data.extend(dec(data))

msg = ''.join(final_data.decode({'1':bitarray('1'), '0':bitarray('0')}))
n = text_from_bits(msg, encoding='utf-8', errors='surrogatepass')
print(n)

{% endhighligh %}
```

After execution we can get out flag :D

```
kali@kali:~/Desktop/CTFs/NACTF/Crypto/Error2/hamming$ python3 script.py 
Total permutations:  24024
011011100110000101100011011101000110011001111011 = 011011100110000101100011011101000110011001111011
Combination =  (5, 7, 8, 13)
nactf{err0r_c0rr3cti0n_w1th_th3_c0rr3ct_f1le_q73xer7k9}
```

---

# General

##  Dr. J's Vegetable Factory #1 🥕
50

After years of collecting plush vegetable toys, Dr. J decided to take on his true passion: starting a vegetable factory. Dr. J is incredibly organized, so he likes all of his vegetables to be in the proper order. In fact, he built a robot "Turnipinator-1000" to alphabetize his vegetables for him! Unfortunately, Dr. J doesn't know what instructions to give Turnipinator-1000. Can you help him out? 🥬🥕🌽🍆🥦🥒🥑🍄

nc challenges.ctfd.io 30267

Give instructions in the form of numbers separated by spaces. Entering the number x will swap the vegetable in position x with the vegetable in position x+1. Positions start at zero, not one. (Dr. J is a programmer after all.) For example, given the following vegetables: Avocado, Brocolli, Eggplant, Daikon Radish, Carrot, one possible solution is "3 2 3"

Avocado, Brocolli, Eggplant, Daikon Radish, Carrot

    (swap 3 and 4)

Avocado, Brocolli, Eggplant, Carrot, Daikon Radish

    (swap 2 and 3)

Avocado, Brocolli, Carrot, Eggplant, Daikon Radish

    (swap 3 and 4)

Avocado, Brocolli, Carrot, Daikon Radish, Eggplant


The problem is like [bubble sort algorithm](https://www.toptal.com/developers/sorting-algorithms). The following algorithm retrieves the information from the server, gets the list, sortit alphabet-wise and gets the positions of each sorted element. Then on the original unsorted list we place the position it has to reach. Then the bubble sort algorithm does the trick, and each position change it does is collected into a list. Then the list is the input for the challenge.

This algorithm can be used in the next challenge, only change the option in the send line:
```
{% highlight python %} 
r.send("1\n") 
{% endhighlight %}
```

```
{% highlight python %}

from pwn import *
import re
import time

def bubbleSort(arr): 

    n = len(arr) 
    sequence = []
    for i in range(n-1): 
        for j in range(0, n-i-1): 
            if arr[j] > arr[j+1] : 
                sequence.append(str(j))
                arr[j], arr[j+1] = arr[j+1], arr[j] 
    return sequence


def get_sequence(current_list):

    unordered = current_list.split(', ')
    sorted_list = sorted(unordered)

    for desired_index, item in enumerate(sorted_list):
        current_index = unordered.index(item)
        unordered[current_index] = desired_index

    sequence = bubbleSort(unordered) 
    print('Sequence: ', ' '.join(sequence))
    return ' '.join(sequence)


r = remote('challenges.ctfd.io', 30267)
received = r.recv().decode('utf-8')
print(received)
r.send("1\n")
stage = 0
while True:
   
    msg = r.recv().decode('utf-8')
    
    print("-----------")
    print(msg)
    print("-----------")
    if 'nactf' in msg:
        print(msg)
        break
    if not stage:
        re_obj = re.search(r'\n\n(.*)\n\n', msg)
        vege = re_obj.groups(1)[0]
    else:
        re_obj = re.search(r'STAGE [0-9] 🥬🥕🌽🍆🥦🥒🥑🍄\n(.*)\n', msg)
        vege = re_obj.groups(1)[0]
    print("+++++")
    print(str(vege))
    print("+++++")
    sequence = get_sequence(vege)
    r.send("{}\n".format(sequence))
    stage += 1

{% endhighlight %}
```

Execute it and retrieve the flag:

``` nactf{1f_th3r3s_4_pr0b13m_13ttuce_kn0w_db4d736fd28f0ea39ec} ```

##  Dr. J's Vegetable Factory #2 🥕
150

Dr. J expanded his vegetable factory! Now he's got hundreds of vegetables. Same problem as last time: can you give Turnipinator-1000 the right instructions to sort Dr. J's vegetables? 🥬🥕🌽🍆🥦🥒🥑🍄

nc challenges.ctfd.io 30267

Use previous algorithm and change the option in the send line:
```
{% highlight python %} 
r.send("2\n") 
{% endhighlight %}
```

``` nactf{d0n7_w0rry_p34_h4ppy_f27ae283dd72cb62f685} ```

##  Dr. J's Vegetable Factory #3 🥕
175

Rahul hates vegetables. Rahul hates vegetables so much that he snuck into Dr. J's factory at night to sabotage Dr. J's vegetable production! He brought a sledgehammer and broke the wheels of Dr. J's robot! 😓 Now the robot is stuck in place, and instead of being able to swap any adjacent elements, it can only swap the elements in positions 0 and 1!

But Dr. J won't let this incident stop him from giving the people the vegetables they deserve! Dr. J is a problem solver 🧠. He organized his vegetables in a circle, and added a conveyor-belt that allows him shift the positions of the vegetables. He thinks that the conveyor belt should make it possible to sort his vegetables, but he's not 100% sure. Can you help him out?

nc challenges.ctfd.io 30267

Enter letters separated by spaces to sort Dr. J's vegetables. Entering "c" will activate the conveyor belt and shift all vegetables left one position. Entering "s" will swap the vegetable in position 0 with the vegetable in position 1.

The same as before, but now we fix the positions at 1 and 0, adding the conveyor belt function (pop first element and append it).

When position 0 is better than 1, swap, else, move conveyor. The exception case is if position 0 is the final element and the position 1 is the fisrt element, in this case, we move the conveyor.

```
{% highlight python %}
from pwn import *
import re
import time

def bubbleSort(arr, sorted_arr): 

    n = len(arr) 
    sequence = []
    bot, top = min(sorted_arr), max(sorted_arr)
    #While the array is not sorted...
    while arr != sorted_arr: 
        #Exceptional case
        if arr[0] == top and arr[1] == bot:
            # Move conveyor-belt
            first = arr.pop(0)
            arr.append(first)
            sequence.append('c')
            continue
        #Swap elements if 0 is greater than 1
        if arr[0] > arr[1]: 
            sequence.append('s')
            arr[0], arr[1] = arr[1], arr[0] 
        else:
        # Move conveyor-belt    
            first = arr.pop(0)
            arr.append(first)
            sequence.append('c')

    return sequence


def get_sequence(current_list):

    unordered = current_list.split(', ')
    sorted_list = sorted(unordered)

    #Set objectives and arrays
    for desired_index, item in enumerate(sorted_list):
        current_index = unordered.index(item)
        unordered[current_index] = desired_index
    sorted_numbers = sorted(unordered)
    sequence = bubbleSort(unordered, sorted_numbers) 
    #print('Sequence: ', ' '.join(sequence))
    return ' '.join(sequence)



r = remote('challenges.ctfd.io', 30267)
received = r.recv().decode('utf-8')
print(received)
r.send("3\n")
stage = 0
while True:
   
    msg = r.recv().decode('utf-8')
    print("-----------")
    print(msg)
    print("-----------")
    if 'nactf' in msg:
        break
    if not stage:
        re_obj = re.search(r'order:\n\n(.*)', msg)
        vege = re_obj.groups(1)[0]
    print("+++++")
    print(str(vege))
    print("+++++")
    sequence = get_sequence(vege)
    r.send("{}\n".format(sequence))
    stage += 1
{% endhighlight %}
```

``` nactf{1t_t4k35_tw0_t0_m4n90_8a51c7b47fbe227} ```


##  Dr. J's Vegetable Factory #4 🥕
350

Thanks to you solving Dr. J's conveyor belt conundrum, Dr. J's vegetable factory is up and running once again. But this time, Juliet breaks into the factory to finish what Rahul started. For some reason, she doesn't break Turnipinator-1000 completely. First, she spins Turnipinator-1000 in a random direction. Then, she pulls the arms of Turnipinator-1000 a random distance apart so it can no longer swap adjacent elements! Now, it can only swap the vegetable in position x with the vegetable in position y!

Initially, Dr. J is very sad. 😥 Even with the conveyor belt, he doesn't think he can sort his vegetables anymore. But then, Dr. J has a brilliant revelation! 💡 He scribbles on scratch paper for a few minutes, then he gets up and removes a few of the vegetables from the conveyor belt.

Proudly, he declares, "Dr. J's Vegetable Factory is back in business!"

nc challenges.ctfd.io 30267

Same instructions as in part #3. Enter letters separated by spaces to sort Dr. J's vegetables. Entering "c" will activate the conveyor belt and shift all vegetables left one position. Entering "s" will swap the vegetable in position x with the vegetable in position y. (x and y will be given.)

Nah. I couldn't get it :(



That's all I could do! Thanks for reading!
