---
title: "Kashmir54 - FlipperZero NRF24 board"
categories: [flipper]
date: 2023-04-01
comments: true
description: FlipperZero modules by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# NRF24

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

# CC1101

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

## 3D printed case

Thanks to James Martin, you can now print your own 3D case for the NRF24 board. Find it in the following [link](https://www.tinkercad.com/things/4Re4xlWwcfE-nrf24-by-kashmir54-with-case-uses-m2-screws). Looks pretty dope!

<p align="center">
  <img src="/images/flipper/nrf24_case.png" width="70%"/>
</p>

<br>

---

<br>

## Megapacks and combos

I also have other interesting stuff, such as USBC keychains, which comes pretty handy on ground pentest operations and have the same orange tone as the flipper do! These gadget allow the researcher to bring the Flipper Zero and the keychain attached without the worry of bringing a spare USBC cable. A perfect combination for the Flipper! I created a "MegaPack RF" which is a combination of all these items, for beginners (and also experienced) FlipperZero users to explore new boundaries for this outstanding tool. 

<p align="center">
  <img src="/images/flipper/megapack_rf.jpg" width="60%"/>
</p>