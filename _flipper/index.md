---
title: "Kashmir54 - Flipper Zero"
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

Flipper Zero, we all know what this marvelous RF Swiss tool is capable of and if not, let me introduce you to it. This compact device is a mix of chips and antennas that can work on different RF scenarios, such as SubGHz frequencies, NFC, 125kHz RFID, WiFi, Bluetooth, and Infrared but it can also work as an Arduino thanks to its GPIO pins, so basically, the combinations and expansions are limitless. Since there are many GPIO apps and external modules developed by the community, I decided to design a PCB to connect the different modules to the flipper and take advantage of these developments. On previous days I was using a fistful set of dangling cables to connect the different chips but with this little PCB I managed to prepare a plug-and-play connector, which save time and space. At the moment I only prepared a multipurpose PCB, specifically to connect the NRF24 and CC1101 chips.

<br>

---

<br>


## Current PCBs and bundles

If you are interested on any item or bundle, contact me through [email](mailto:kashmir_54@hotmail.com). Click on any of the following cards to check further information.

<br>

<div class="grid-two">


<a class="box" href="/flipper/voyagerrf_micro">
  <div class="box-two">
    <p class="rodden">VoyagerRF Micro</p>
    <p>Micro WiFi DevBoard with ESP32-C3 with Marauder/Ghost_ESP firmware, GPS, antenna, and MicroSD socket.</p>
    <img class="main" src="/images/flipper/micro/thumb_micro.JPG">
  </div>
</a>

<a class="box" href="/flipper/voyagerrf_v2">
  <div class="box-two">
    <p class="rodden">VoyagerRFv2</p>
    <p>WiFi DevBoard with ESP32-C3 with Marauder/Ghost_ESP firmware, GPS, antenna, and MicroSD socket + CC1101/nRF24 socket.</p>
    <img class="main" src="/images/flipper/v2/thumbs.png">
  </div>
</a>

<a class="box" href="/flipper/newhorizonsrf">
  <div class="box-two">
    <p class="rodden">NewHorizonsRF</p>
    <p>UHF RFID Experimental DevBoard based on YRM1003 module for Flipper Zero.</p>
    <span></span>
    <img class="main" src="/images/flipper/newhorizons/thumbs.JPG">
  </div>
</a>

<a class="box" href="/flipper/wifi">
  <div class="box-two">
    <p class="rodden">VoyagerRFv1</p>
    <p>WiFi DevBoard with ESP32-C3 with Marauder/Ghost_ESP firmware, antenna, and MicroSD socket + CC1101/nRF24 socket.</p>
    <img class="main" src="/images/flipper/voyager_white_1.jpg">
  </div>
</a>

<a class="box" href="/flipper/nrf24_board#other-stuff">
  <div class="box-two">
    <p class="rodden">MegaPack RF</p>
    <p>Go beyond! A starter pack to increase your Flipper capabilities.</p>
    <img class="main" src="/images/flipper/megapack_rf.jpg">
  </div>
</a>


</div>

<br>

---

<br>

## Board comparison

Here you have a set of features and the different board to have all information

| Board          | ESP32C3 + SD Card Socket       |   GPS (Marauder/Ghost_ESP)  |  NeoPixel LED (Marauder/Ghost_ESP) | CC1101 / nRF24 Socket | UHF Reader  | 
| -----------    | --------------------     |  -----------      |  -------------- | --------------------  | ------------------- |
| VoyagerRFv2    |             ✅     |              ✅         |      ✅         |      ✅       |      ❌     |
| VoyagerRFv1    |             ✅     |              ✅         |      ❌         |      ✅       |      ❌     |
| VoyagerRF Micro|             ✅     |              ✅         |      ✅         |      ❌       |      ❌     |
| New Horizons   |             ❌     |              ❌         |      ❌         |      ❌       |      ✅     |



<br>

---

