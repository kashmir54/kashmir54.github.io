---
title: "CTF - CyberApocalypse 2021"
categories: [ctf]
date: 2021-02-20
comments: true
image: /images/kashmir.png
favicon: /images/favicon.ico
description: CTF - CyberApocalypse 2021
---

# CyberApocalypse 2021

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/logo.webp" width="70%"/>
</p>


I participate in this CTF for team [ISwearIGoogledIt](https://ctftime.org/team/109689) and got some challenges! I focused on the hardware ones, I loved them!


<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/rank.png" width="70%"/>
</p>

---

Challenge index:

### Hardware

- [Serial Logs](#serial-logs)
- [Compromised](#compromised)
- [Secure](#secure)
- [Off the grid](#off-the-grid)

### Misc

- [Alien Camp](#alien-camp)
- [Input as a Service](#input-as-a-service)

### Forensic

- [Oldest trick in the book](#oldest-trick-in-the-book)

---

# Hardware

## Serial Logs

We have gained physical access to the debugging interface of the Access Control System which is based on a Raspberry Pi-based IoT device. We believe that the log messages of this device contain valuable information of when our asset was abducted.

We got a .sal file. First guess by the content could be a logic analyzer, so let's use Logic2 to inspect its content:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/1_logic.png" width="80%"/>
</p>

We can see that there is only one channel with signals so probably we are talking about [UART protocol](https://en.wikipedia.org/wiki/Universal_asynchronous_receiver-transmitter), so let's analyze it. For doing that, first we have to know the baud rate also known as the transfer speed. It's standard metric is bits/s, so to calculate the baud rate in this signal we zoom into the very first signal and see the minimun period of the signal.

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/2_baud.png" width="80%"/>
</p>

We can see that the smallest period between two high values is 8.5us. So, if we have 1000000us (1 second) and 1 bit is being transfered every 8.5us we can do a quick operation to get hoy many bits/seconds are sent:

```
1 bit   1000000us
----- x --------- = 117.647,0588 bits/second
8.5us       1s
```

Now with the list of [standard baud rates](https://electronics.stackexchange.com/questions/9264/what-standard-uart-rates-are-there), we found that the nearest baud rate to the obtained baud rate is 115.200, so we will take it as a valid result, and configure the analyzer with this standard value:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/3_decode.png" width="70%"/>
</p>

In the next image we can see that Logic2 have decoded correctly the communication and se can see some messages on the signals:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/3_message.png" width="80%"/>
</p>

Now we export it into a csv and with a quick script to then retrieve the message:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/5_export.png" width="50%"/>
</p>

With the following code we can extract the data from the csv.

{% highlight python %}
import csv

message = []

with open('flag') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')

	for row in csv_reader:
		message.append(row[4])

	all_msg = ''.join(message)

with open('output_115200.txt', 'w') as out_file:
	out_file.write(all_msg)
{% endhighlight %}

All was looking good and we had some logs on the file but suddenly we can see that the baud rate changed:

```
...
[LOG] Connection from 316636cf0500c22f97fa261585b72a48c4625aca7868f0f6ee253937620ac15c
[LOG] Connection from 4b1186d29d6b97f290844407273044e5202ddf8922163077b4a82615fdb22376
[LOG] Connection from 4b1186d29d6b97f290844407273044e5202ddf8922163077b4a82615fdb22376
[LOG] Connection from 4b1186d29d6b97f290844407273044e5202ddf8922163077b4a82615fdb22376
[ERR] Noise detected in channel. Swithcing baud to backup value
î.ì~..ò.|`~.î|.....b..|.|....r.|...|pà|....âp..ppâp....âpp|..|..p..|..pp..|â.p..p......||.|..|..pp..|p.|àà|..pp.p...p.|..p..p.î.ì~..ò.|`~.î|.....b..|.|....r.|p.|..p|.p..|â.|p.|p.....|......|...pp..pr.p.à|p.|p.|..||pâ.|p.||.......|...|r..|â..|r.....|.î.ì~..ò.|`~.î|.....b..|.|....r.|à...âpp..pà.p|p..p......p...p|..|..................p..|.....à|â.p..|p..p.p..|p.|p
...
```

Check the last readable message. It says that is switching the baud rate and then we have problems on decoding the signal. We are going to export the csv again but setting the correct baud rate for that segment. Go to the end of the transmission, calculate the baud rate of the final segment and get the message again:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/4_baud2.png" width="80%"/>
</p>

We have the smallest period of 13.48us: 1000000 / 13.48 = 74183 bits/s. As we don't have a common baud rate near to the calculated one, we will set it anyway to 74183 and let's see if we can decode it. Export the table as csv and rerun the script:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/SerialLogs/5_export.png" width="40%"/>
</p>

Bingo:

```
[LOG] Connection from a7e6ec5bb39a554e97143d19d3bfa28a9bbef68fa6ecab3b3ef33919547278d4
[LOG] Connection from 099319f700d8d5f287387c81e6f20384c368a9de27f992f71c1de363c597afd4
[LOG] Connection from ab290d3a380f04c2f0db98f42d5b7adea2bd0723fa38e0621fb3d7c1c2808284
[LOG] Connection from CHTB{wh47?!_f23qu3ncy_h0pp1n9_1n_4_532141_p2070c01?!!!52}
[LOG] Connection from CHTB{wh47?!_f23qu3ncy_h0pp1n9_1n_4_532141_p2070c01?!!!52}
```

``` CHTB{wh47?!_f23qu3ncy_h0pp1n9_1n_4_532141_p2070c01?!!!52} ```

<br>

## Compromised

An embedded device in our serial network exploited a misconfiguration which resulted in the compromisation of several of our slave devices in it, leaving the base camp exposed to intruders. We must find what alterations the device did over the network in order to revert them before its too late

We have another sal file. Let's open it with Logic2:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Compromised/1_logic.png" width="80%"/>
</p>

We have 2 channels with signals in both... Sounds like I2C protocol. Using the analyzer, we configure the data and clock channel:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Compromised/2_i2c.png" width="80%"/>
</p>

We were right, it's I2C. Now let's understand what is in there:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Compromised/3_ack.png" width="80%"/>
</p>

We export it as the previous chall. First select ascii in the protocol display and then export the csv:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Compromised/3_ascii.png" width="40%"/>
</p>

I used the previous scripts with some modifications:

{% highlight python %}

import csv

message = []

with open('flag') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	first_row = 0

	for row in csv_reader:
		if first_row == 0:
			first_row = 1
			continue
		message.append(row[4])
			
	all_msg = ''.join(message)

with open('output.txt', 'w') as out_file:
	out_file.write(all_msg)
{% endhighlight %}

And I had the following output:

```
set_maCxH_lTimB{itn_tuo1:110_se73t_2mimn1_nli4mi70t_2to5:_1c0+.]<+/4~nr^_yz82Gb3b"4#kU_..4+J_5.
3M.2B1\04B.1dV_5. yS.5B7k3..1V.Qxm.!j.@`Q52yq)t%# @5%md}S.
```

We can see traces of the flag, but is scrambled within other data. I though that maybe some of the flags have special values when reading the flag of expected data, so I looked at the table and check this out. Address is different when flag chars came in so let's filter and retrieve only those packets comming from 0x32 address:

{% highlight python %}
import csv

message = []

with open('flag') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	first_row, save_addr = 0, 0

	for row in csv_reader:
		if first_row == 0:
			first_row = 1
			continue
		if save_addr == ',':
			message.append(row[4])
		save_addr = row[6]
			
	all_msg = ''.join(message)

with open('output.txt', 'w') as out_file:
	out_file.write(all_msg)
{% endhighlight %}

``` CHTB{nu11_732m1n47025_c4n_8234k_4_532141_5y573m!@52)#@%} ```

<br>

## Secure

We need to find cover before the invasion begins but unfortunately, the bunker is secured by a smart door lock. The keys of the device are stored in an external microSD connected with wiring with the unsecured part of the device enabling us to capture some traces while trying random combinations. Can you recover the key?

We have another sal file. We have 4 channels... For the different signals we can guess it's SPI protocol. Now we have to identify which channel is which:
- The easier one is the master clock, identified as the _Channel 3_ (yellow) since we can see it's high and almost constant frequency.
- Then the slave select (SS) could be the _Channel 2_ due to its low activity during the transmission.
- Finally the data lines, Master-In-Slave-Out (MISO) and Master-Out-Slave-I (MOSI). For this task may not be as relevant, so we set the _Channel 0_ as MISO and _Channel 1_ as MOSI.

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Secure/1_logic.png" width="90%"/>
</p>

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Secure/2_config.png" width="70%"/>
</p>

As we can see on the table, we made the assignation right, so looking at the table in ascii mode we can see some readable text. _NO NAME FAT16_, that could be the SD card we are looking for; let's export it as csv and use the previous script to export the MISO data channel to a text file.

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Secure/3_hint.png" width="50%"/>
</p>

Using the previous script to extract the row[4], we exported the following txt:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/Secure/4_flag.png" width="70%"/>
</p>

``` CHTB{5P1_15_c0mm0n_0n_m3m02y_d3v1c35_!@52} ```

<br>

## Off the grid

One of our agents managed to store some valuable information in an air-gapped hardware password manage and delete any trace of them in our network before it got compromised by the invaders but the device got damaged during transportation and its OLED screen broke. We need help to recover the information stored in it!


We got another sal file with an schematic attached:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/schematic.png" width="70%"/>
</p>

By opening the file we can see up to 5 channels with signals this time. Thanks to the schematic, we can see the model of the OLED display. For the amount of pins and some first impresions on google images, we can asume its an SPI protocol for that display. In case that it was a 4 pin display, it would be an I2C based display. The pinout and the channel correspondance its listed bellow. For further reading, check this [wiring guide](https://www.14core.com/wiring-the-128x64-1-3-inch-oled-display-on-4-wire-3-wire-i2c-interface/). Final settings are displayed in the image below:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/1_settings.png" width="70%"/>
</p>

We don't have to asign the RST (reset) pin, since it's only used to clear and turn on the display. Once we have set the different channels to the SPI protocol, we take a look into the general image of the transmission. Experience tell us that each transmission (with 1 second between them) is an image for the display to load in its memory and to represent it. Therefore, we will try to extract an image from each transmission. 

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/2_logic.png" width="90%"/>
</p>

But first, what are we looking at? How do those display work? For representing an image, usually we have different arduino libraries which are used to take images into a hexadecimal vector for monochrome displays. So we are looking into the MOSI (Master-Out-Slave-In) hexadecimal since those are the values that can build up the image. So I developed this script for extracting each image:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/4_export.png" width="60%"/>
</p>

```
name,type,start_time,"mosi","miso"
"SPI","enable",1.8286046,,
"SPI","result",1.82861364,0x00,0xAE
"SPI","disable",1.8286212,,
"SPI","enable",1.82866414,,
"SPI","result",1.8286685,0x00,0x02
"SPI","disable",1.82867564,,
"SPI","enable",1.82870758,,
"SPI","result",1.82871154,0x00,0x10
```

To extract the array of values for an specific frame (one per second) I developed the following script:

{% highlight python %}
import csv

message = []
second = '4' 		# Set the second for each image

with open('flag2') as csv_file:
	csv_reader = csv.reader(csv_file, delimiter=',')
	first_row, save_addr = 0, 0

	for row in csv_reader:
		if first_row == 0:
			first_row = 1
			continue

		# Get only the data in that second
		if row[2][0] == second and len(row[4]) == 4:
			message.append(row[4])
			
	all_msg = ','.join(message)

with open('output.txt', 'w') as out_file:
	out_file.write(all_msg)
{% endhighlight %}

To retrieve the hexadecimal value for the image:

```
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0x08,0x04,0xF4,0xF4,0x04,0xF4,0xF4,0x04,0xF4,0xF4,0x04,0xFC,0x00,
0xB1,0x02,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xA0,0xA0,0xB0,0xB0,0xB0,
0xB0,0xB0,0xA0,0x00,0x00,0x00,0x00,0x80,0xB0,0xB0,0x00,0x00,0x00,0x80,0xB0,0xB0,0x00,
0x00,0x00,0x00,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0,0xB0...
```

Now we have the hexadecimal list and we can use the online tool [image2cpp](https://javl.github.io/image2cpp/) to decode the images. Remember to set the resolution to _131 x 64 px_ and read as vertical:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/5_image2cpp.png" width="70%"/>
</p>

The output images are these:

<div display="grid">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/1.png" width="19%"/>
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/2.png" width="19%"/>
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/3.png" width="19%"/>
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/5.png" width="19%"/>
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/6.png" width="19%"/>
</div>

There was one of them with the flag:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/HW/OffTheGrid/4.png" width="30%"/>
</p>


``` CHTB{013d_h4ck1n9_f7w!2^25#} ```

<br>

---

# Misc

## Alien Camp

The Ministry of Galactic Defense now accepts human applicants for their specialised warrior unit, in exchange for their debt to be erased. We do not want to subject our people to this training and to be used as pawns in their little games. We need you to answer 500 of their questions to pass their test and take them down from the inside.


We have a netcat ip with this output:

```
Alien camp 👾

1. ❓
2. Take test!
> 1
Here is a little help:

🌞 -> 6 🍨 -> 15 ❌ -> 3 🍪 -> 16 🔥 -> 44 ⛔ -> 45 🍧 -> 85 👺 -> 70 👾 -> 23 🦄 -> 67 

1. ❓
2. Take test!
> 2

You must answer 500 questions. You have only a few seconds for each question! Be fast! ⏰

Question 1:

👾 * 👾 + 🔥  = ?

Answer: 
```

I made this script to solve this automatically:

{% highlight python %}
from pwn import *
import re

io = remote('206.189.121.131',32068)
f = io.recvuntil('>')
print(f.decode('utf-8'))
io.sendline('1')
f = io.recvuntil('\n>')
print(f.decode('utf-8'))
f = f.decode('utf-8')
objs = re.findall(r'(([🌞🍨❌🍪🔥⛔🍧👺👾🦄])....([\d]+))', f)

rela = {}
for m in objs:
	rela[m[1]] = m[2]

io.send('2\n')
f = io.recvuntil('Answer:')
print(f.decode('utf-8'))

for i in range(0, 500):
	f = f.decode('utf-8')

	eq = re.search(r'([🌞🍨❌🍪🔥⛔🍧👺👾🦄]..?[*+-]?.)+', f)
	real_eq = eq.group(0)

	for emoji in rela:
		real_eq = real_eq.replace(emoji, rela[emoji])

	io.sendline(str(eval(real_eq[:-1])))
	if i == 499:
		print('Get it!')
		flag = io.recvrepeat(5)
		print(flag.decode('utf-8'))

	f = io.recvuntil('Answer:')
	print(f.decode('utf-8'))
{% endhighlight %}

```
Question 500:

🌞 * ❌ * 🌞 - 🍧 * ⛔ + 🌞 - 🔥  = ?

Answer:
498
Get it!
 
Time: 0.05
Correct! ✔
Congratulations! 🎉
You are one of us now! 😎!
Here is a 🎁 for you: CHTB{3v3n_4l13n5_u53_3m0j15_t0_c0mmun1c4t3}
```

``` CHTB{3v3n_4l13n5_u53_3m0j15_t0_c0mmun1c4t3} ```

<br>

## Input as a Service

In order to blend with the extraterrestrials, we need to talk and sound like them. Try some phrases in order to check if you can make them believe you are one of them.


```
kali@kali:~/Desktop/CTFs/CyberApocalypse2021/Forensic$ rlwrap nc 138.68.148.149 30738
2.7.18 (default, Apr 20 2020, 19:51:05) 
[GCC 9.2.0]
Do you sound like an alien?
>>> 
open('/app/input_as_a_service.py', 'r').read()
ls

 #!/usr/bin/python2.7
from sys import version

'''
Descr:
In order to blend with the extraterrestrials, we need to talk and sound like them. Try some phrases in order to check if you can make them believe you are one of them. 
'''

def main():
    print version + '\nDo you sound like an alien?\n>>> \n'
    for _ in range(2):
        text = input(' ')
        print text

if __name__ == "__main__":
    main()
 Traceback (most recent call last):
  File "/app/input_as_a_service.py", line 16, in <module>
    main()
  File "/app/input_as_a_service.py", line 12, in main
    text = input(' ')
  File "<string>", line 1, in <module>
NameError: name 'ls' is not defined
kali@kali:~/Desktop/CTFs/CyberApocalypse2021/Forensic$ rlwrap nc 138.68.148.149 30738
2.7.18 (default, Apr 20 2020, 19:51:05) 
[GCC 9.2.0]
Do you sound like an alien?
>>> 
open('flag.txt', 'r').read()
121

 CHTB{4li3n5_us3_pyth0n2.X?!}

 121
```

``` CHTB{4li3n5_us3_pyth0n2.X?!} ```
<br>

---

# Forensic

## Oldest trick in the book
A data breach has been identified. The invaders have used the oldest trick in the book. Make sure you can identify what got stolen from us.


With this challenge we received a pcap file. Let's analyze it. At the first look, we can see a TCP transmission, some DNS and ICMP packets. Since TCP is being used with TLS 1.2 and 1.3 we can assume that there is nothing usefull there, so I started looking at ICMP packets:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/1_wire.png" width="70%"/>
</p>

There were so much of them so this was my first option to look up for the solution. I applied the _icmp_ filter on wireshark to clean up the transmission and we have up to 20.000 ICMP packets which is not normal... It could be some kind of data exfiltration through this protocol. 
Looking into the first ICMP packet we can see that it has a payload. All ICMPs in this pcap have a constant payload:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/2_pk.png" width="70%"/>
</p>

We can se PK.. 3 times in the payload... That PK reminds me of a [zip/rar file header](https://en.wikipedia.org/wiki/List_of_file_signatures) so we can talk about a zip file being transfered using ICMP payloads(?). 

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/3_pkfile.png" width="70%"/>
</p>

The main problem is that it looks like they are repeating the content 3 times in each payload and ICMP request ECHO and ICMP request REPLY are transmiting the same data, so we will have to make some adjustments to the data. First let's get rid of half of the packets by using only the REPLY packets with the filter _ip.src == 192.168.1.8_:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/4_packets.gif" width="70%"/>
</p>

Now, let's retrieve the payloads. I don't know if wireshark has a method to extract them, I found no option. If you know it, let me know in the comments! For this task I always use **tshark**, so the command to filter and get the data from these packets is _-Y_ with the actual filter and _-T fields -e data.data_ since it's the target field:

```
tshark -r older_trick.pcap -Y "ip.dst == 192.168.1.8" -T fields -e data.data > flag2.txt
```

```
b7ae040000000000504b0304140000000000729e8d52659b504b0304140000000000729e8d52659b504b030414000000
ead10400000000004c6b18000000180000001000000066694c6b18000000180000001000000066694c6b180000001800
99e80400000000006e692f6164646f6e732e6a736f6e7b226e692f6164646f6e732e6a736f6e7b226e692f6164646f6e
cafb040000000000736368656d61223a362c226164646f6e736368656d61223a362c226164646f6e736368656d61223a
281805000000000073223a5b5d7d504b030414000000080073223a5b5d7d504b030414000000080073223a5b5d7d504b
6d330500000000001da38d52ec0fbbb6d0080000670a00001da38d52ec0fbbb6d0080000670a00001da38d52ec0fbbb6
884f0500000000001a00000066696e692f6164646f6e53741a00000066696e692f6164646f6e53741a00000066696e69
...
```

Nice! We have the payloads distributed in lines. Now, I implemented an small script to retrieve only one of the three segments within the payloads, so we extract bytes from position 16 to 48 for example:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/5_script.png" width="70%"/>
</p>


{% highlight python %}
data = []

with open('flag2.txt', 'r') as in_file:
	text = in_file.readlines()

for payload in text:
	data.append(bytearray.fromhex(payload[16:48]))

with open('flag.zip', 'wb') as out_file:
	out_file.write(b''.join(data))

{% endhighlight %}

Look what we have... A valid zip file. We are on the right path:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/6_zip.png" width="60%"/>
</p>

Looking into the zip we can see some .sqlite, .json and .db files. Also the names (cookies, extensions **logins**...) make me think about a Firefox profile! So intuition tells me that there is a password or a cookie with the flag. To see them, I restored the Firefox profile into my actual Firefox folder
Restore Mozilla profile. To do so, you can find the Firefox files at Appdata>Roaming>Mozilla>Firefox>profiles in Windows. Then I replaced the files and use the ones in the zipfile:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/8_appdata.png" width="70%"/>
</p>

Finally, open Firefox, go to _Settings_ and _Saved logins_, there we can see one saved password with the flag:

<p align="center">
  <img src="/images/writeups/CyberApocalypse2021/Forensic/9_flag.png" width="70%"/>
</p>

``` CHTB{long_time_no_s33_icmp} ```


That's all! Thank you for reading!