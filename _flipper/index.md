---
title: "Kashmir54 - FlipperZero"
categories: [flipper]
date: 2023-03-10
comments: true
description: FlipperZero modules by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# Flipper Zero 🐬📻

<p align="center">
  <img class="head-logo" src="/images/flipper/flipper.png" width="30%"/>
</p>

Flipper Zero, we all know what this marvellous RF swiss tool is capable of and if not, let me introduce you to it. This compact device is a mix of chips and antennas that can work on different RF scenarios, such as SubGHz frequencies, NFC, 125kHz RFID, WiFi, Bluetooth and Infrared but it can also work as an Arduino thanks to its GPIO pins, so basically, the combinations and expansions are limitless. Since there are many GPIO apps and external modules developed by the community, I decided to design a PCB to connect the different modules to the flipper and take advantage of these developements. On previous days I was using a fistful set of dangling cables to connect the different chips but with this little PCB I managed to prepare a plug-and-play conenctor, which save time and space. At the moment I only prepared a multipurpose PCB, especifically to connect the NRF24 and CC1101 chips.


## Current PCBs and bundles

If you are interested on any item or bundle, contact me through [email](mailto:kashmir_54@hotmail.com)

<br>

<div class="grid-two">

<a class="box" href="#nrf24">
  <div class="box-two">
    <p class="rodden">Pack NRF24</p>
    <p>Get the connection board and the NRF24.</p>
    <img class="orange-border" src="/images/flipper/nrf24.jpg">
  </div>
</a>

<a class="box" href="#cc1101">
  <div class="box-two">
    <p class="rodden">Pack CC1101</p>
    <p>Get the connection board and the CC1101.</p>
    <img class="orange-border" src="/images/flipper/cc1101_1.jpg">
  </div>
</a>

<a class="box" href="#other-stuff">
  <div class="box-two">
    <p class="rodden">MegaPack RF</p>
    <p>Go beyond! A starter pack to increase your Flipper capabilities.</p>
    <img class="orange-border" src="/images/flipper/megapack_rf.jpg">
  </div>
</a>

<a class="box" href="">
  <div class="box-two">
    <p class="rodden">VoyagerRF</p>
    <p>WiFi DevBoard with ESP32-C3, proper antenna, Micro SD Card socket and Marauder firmware. Includes a socket for NRF24/CC1101 connection.</p>
    <p>Coming soon...</p>
  </div>
</a>

<a class="box" href="">
  <div class="box-two">
    <p class="rodden">Standalone PCB</p>
    <p>Plug-and-play PCB for those who already own the NRF24/CC1101 chip.</p>
    <img class="orange-border" src="/images/flipper/pcb_v1_1.jpg">
  </div>
</a>

<a class="box" href="">
  <div class="box-two">
    <p class="rodden">USB-C Keychain</p>
    <p>Avoid carrying a spare USB cable on your pocket.</p>
    <img class="orange-border" src="/images/flipper/usbc.jpeg">
  </div>
</a>

</div>

In the following sections I will go over each of them, to summarize their basic capabilities.

<br>

---

<br>

## NRF24

This module that operates on the 2.4GHz band, which allows the user to interact send and receive data in that band. Unlike the ESP32 (which also operates on the 2.4GHz band) this chip does not implement the TPC/IP stack, therefore, control of the raw physical layer is provided. This module is used by Flipper's GPIO plugins such as [MouseJacker](https://github.com/RogueMaster/flipperzero-firmware-wPlugins/blob/420/documentation/NRF24.md). This plugin is used in combination with the NRF24 Sniffer, which allow us to grab interesting information from devices working in this band.

<p align="center">
  <img class="schema" src="/images/flipper/nrf24_dia.png" width="40%"/>
</p>
<div align="center">
NRF24L01 pinout, similar to NRF24L01+PA/LNA. Courtesy of [UberGuidoZ](https://github.com/UberGuidoZ).
</div>

<p align="center">
  <img src="/images/flipper/nrf24_sch.png" width="60%"/>
</p>
<div align="center">
NRF24L01 wiring diagram.
</div>

Probably you already know which devices use this band, that's right, wireless keyboards and mice, well, some of them. There ar some well-known brands and models that implement "unencrypted" packets to communicate the device with the USB dongle, therefore, we can sniff the spectrum to grab the address of the device then use it to send our curated payload to the dongle. Further information can be found in the original [blog post](http://travisgoodspeed.blogspot.com/2011/02/promiscuity-is-nrf24l01s-duty.html) or in [mousejack.com](https://www.mousejack.com/).

What implies this vulnerability? Mainly, that we can impersonate the keyboard or the mouse and send [HID codes](https://gist.github.com/MightyPork/6da26e382a7ad91b5496ee55fdc73db2), aka, keystrokes or mouse movements. Known products affected by this vulnerability can be found in the [JackIt repo](https://github.com/insecurityofthings/jackit). You can check them out and play around with them.

I have developed some PCBs to connect this module. I added some decoupling capacitors for stable power delivery and noise reduction on the signal. Here you have some photos of the PCBs in action:


<div display="grid" align="center">
  <img src="/images/flipper/nrf24.jpg" width="40%"/>
  <img src="/images/flipper/nrf24_1.jpg" width="47%"/>
</div>

<br>

---

<br>

## CC1101

CC1101 is an RF transceiver that works on the sub GHz frequencies. It works on 300-348Mhz, 387-464Mhz, and 779-928Mhz specifically and you can use it on  your flipper as an external module for the SubGHz application. With the corresponding antenna, this module allows the researcher to reach further transmission distances (up to 500m), with is a major improvement over the stock Flipper Zero antenna. 


<p align="center">
  <img class="schema" src="/images/flipper/cc1101_pinout.png" width="30%"/>
</p>
<div align="center">
CC1101 wiring diagram. Courtesy of [quen0n](https://github.com/quen0n).
</div>

The board that I've designed has the same pinout required for the [Unleashed SubGHz application](https://github.com/quen0n/flipperzero-ext-cc1101) to use the external module, therefore, you can use the same board for the NRF24 and the CC1101! 

Some tests that I performed at home, showed that the stock SubGHz module could grab a 433MHz keyfob at 5 meters distance while with the external module I could grab the keyfob up to 20 meters, which is a great improvement. I will arrange more tests in order to get some accurate measurements about this performance.

Here you have the CC1101 external module in action with the same PCB:

<div display="grid" align="center">
  <img src="/images/flipper/cc1101.jpg" width="50%"/>
  <img src="/images/flipper/cc1101_1.jpg" width="46%"/>
</div>

<br>

---

<br>

## Other stuff

I also have other interesting stuff, such as USBC keychains, which comes pretty handy on ground pentest operations and have the same orange tone as the flipper do! These gadget allow the researcher to bring the Flipper Zero and the keychain attached without the worry of bringing a spare USBC cable. A perfect combination for the Flipper! I created a "MegaPack RF" which is a combination of all these items, for beginners (and also experienced) FlipperZero users to explore new boundaries for this outstanding tool. 

<p align="center">
  <img src="/images/flipper/megapack_rf.jpg" width="60%"/>
</p>


That's all for now. In further updates I will explain how to play around with these modules. 