---
title: "VoyagerRF Pro for Flipper Zero"
categories: [flipper]
comments: true
description: VoyagerRF Pro module by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

<p class="vera">VoyagerRF Pro</p>
A new version of the VoyagerRF (ESP32 WiFi/BLE Devboard for Flipper Zero) with GPS, MicroSD Card socket and 2G4M27SX chip (nRF24).

<br>

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_both.JPG" data-sub-html="VoyagerRF Pro black and white version">
      <img src="/images/flipper/pro/pro_both.JPG" class="main" width="95%"/>
    </a>
  </div>
</div>

<br>

---

<br>

## Page index

- [Intro](#intro)
- [Bundles and Boards](#bundles-and-boards)
- [3D Case files](#3d-case-files)
- [Tutorials](#tutorials)
- [Troubleshooting](#troubleshooting)


<br>

## Related tools

<div class="grid-one">
  <a class="box" href="https://kashmir54.github.io/voyagerrf/v2.html">
    <div class="main-box" >
      <div style="padding-left: 10%; padding-right: 10%;">
        <p>Update or switch your firmware on the VoyagerRF Pro board easily! (Marauder / Ghost_ESP)</p>
        <img class="micro-icon" src="/images/flipper/pixel.png">
        <p>Click here!</p>
      </div>
    </div>
  </a>
</div>

<br>

---

<br>


## Intro

During the developement of the [VoyagerRFv1](https://kashmir54.github.io/flipper/wifi/) and [VoyagerRFv2](https://kashmir54.github.io/flipper/voyagerrf_v2/) I focused on flexibility, so I can use nRF24 or CC1101 whenever I needed. Using more and more the Flipper, I realized that I used much more the nRF24 instead of the CC1101 because of the jobs I do, so I wanted to upgrade that nRF24 part and let go the CC1101. That's why I developed the VoyagerRF Pro, a board focused on the 2.4GHz, WiFi, BLE 5.0 and propietary protocols with an upgraded nRF24 module, the [E01-2G4M27SX chip](https://www.cdebyte.com/products/E01-2G4M27SX). The board is compatible with all firmwares: Unleashed, Momentum, Roguemaster, Official, etc.

Main features of this VoyagerRF Pro version:

- ⚡️ **Ready for Ghost_ESP Firmware:** The VoyagerRF Pro comes with Marauder preflashed by default, but the board supports Ghost_ESP, you can use the [Web Flasher](https://kashmir54.github.io/voyagerrf/) to easily change/update your firmware with 3 clicks.
- ⚡️ **ESP32-C3: A low-consumption WiFi and Bluetooth** chip with the latest Marauder firmware preflashed. This chip will allow you to perform all WiFi and Bluetooth tools deployed on marauder (including wifi scans, deauths, evilportal, wardriving, sour apple, ble spam, etc). Ready to plug and play and control it with the [ESP32 WiFi Marauder](https://github.com/0xchocolate/flipperzero-wifi-marauder/releases) or [Ghost ESP](https://github.com/Spooks4576/ghost_esp_app) GPIO apps.
- ⚡️ **MicroSD card slot** to save the output from the Marauder/Ghost_ESP apps (already configured in the firmware) and different configuration files (for evilportal, for example).
- ⚡️ **3dBi Omnidirectional antennas** (2.4GHz) mounted on the board's SMA connectors. Adds a great range improvement against the original devboard with PCB antenna.
- ⚡️ **GPS module and antenna:** The ATGM336H GPS module is connected to the ESP32-C3, which allows the Marauder firmware to collect GeoPositioning information on wardriving mode. This mode saves the location and WiFi SSID (among other data) in the VoyagerRF's MicroSD card with a certain file formatthat can be uploaded on websites such as [wigle.net](https://wigle.net/).
- ⚡️ **New NeoPixel status LED:** This RGB LED will indicate the status of the Marauder/Ghost_ESP firmware. You can also set manually the HEX code for the LED or flex it with the raimbow mode.
- ⚡️ **E01-2G4M27SX chip (nRF24)** Compatible with all nRF24 applications on the Flipper including [NRF24 Sniffer/MouseJacker](https://github.com/mothball187/flipperzero-nrf24), [NRF24 Batch](https://github.com/vad7/nRF24-Batch), [nRF24 Channel Scan](https://github.com/htotoo/NRF24ChannelScanner), among other apps. This [E01-2G4M27SX chip](https://www.cdebyte.com/products/E01-2G4M27SX) has a greater power than [normal nRF24](https://www.cdebyte.com/products/E01-ML01DP5) chips.
- ⚡️ **Manually soldered and tested**, I like to solder, so I prefer to build it myself to maximize quality.
- ⚡️ **3D Printed case:** For people looking for a clean look, I designed a case and a base to store and carry your VoyagerRF everywhere. 3D models ready for printing are provided below. (To be honest, I prefer to see the PCB and components).


Remember that the VoyagerRF Pro can be easily updated via the [Web Flasher](https://kashmir54.github.io/voyagerrf/v2.html) following the steps in the website. Also, you can change from Marauder to Ghost_ESP easily.

I have units in stock. If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com). Check out the [bundles and boards section](#bundles-and-boards) to see bundles and prices.

Here you have more photos, I liked the orange and green colors on them, but I also print the cases in black and white:

Front:
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_white_0.JPG" data-sub-html="VoyagerRF Pro white version">
      <img src="/images/flipper/pro/pro_white_0.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/pro/pro_black_0.JPG" data-sub-html="VoyagerRF Pro black version">
      <img src="/images/flipper/pro/pro_black_0.JPG" width="45%"/>
    </a>
  </div>
</div>
<br>
Back with translucent case:
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_white_back.JPG" data-sub-html="Back of the VoyagerRF Pro white version with translucent case">
      <img src="/images/flipper/pro/pro_white_back.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/pro/pro_black_back.JPG" data-sub-html="Back of the VoyagerRF Pro black version with translucent case">
      <img src="/images/flipper/pro/pro_black_back.JPG" width="45%"/>
    </a>
  </div>
</div>
<br>


---

<br>

# Bundles and Boards

If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com) on my email kashmir_54@hotmail.com and I will get back to you!

<br>

## VoyagerRF Pro / 70€

PCB available in black or white and 3D pieces in black, white, translucent, lime green and light orange. Antennas can be black or white, as you prefer.

Includes:

- 1 x VoyagerRF Pro board with latest Marauder Firmware preinstalled. If you want Ghost_ESP instead, let me know.
- 2 x 3dBi Omni Antenna (2.4GHz) Length: 11cm (4.33inch)
- 3D PLA+/PETG back + front frame + pin cases with rugged finish. Choose your color on the email (white, black, translucent, lime green, light orange!

There you have both colors to compare (in the photo the front frame has been removed):

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_both_cases.JPG" data-sub-html="Both colors">
      <img src="/images/flipper/pro/pro_both_cases.JPG" width="85%"/>
    </a>
  </div>
</div>

For example, a VoyagerRF Pro White with translucent cases will contain:

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_case.JPG" data-sub-html="White VoyagerRF Pro with translucent cases">
      <img src="/images/flipper/pro/pro_case.JPG" width="45%"/>
    </a>
  </div>
</div>


<br>

## Mini 3dBi 2.4GHz antennas / +10€

If you want your rig to be smaller, you can add these 3dBi mini antennas (2.5 cm / 1 inch). Performance is as great as almost as great as the larger ones.

Includes:

- 2 x 3dBi Omni Antenna (2.4GHz) Length: 2.5cm (1 inch)

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_mini_white.JPG" data-sub-html="VoyagerRF Pro White with mini antennas">
      <img src="/images/flipper/pro/pro_mini_white.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/pro/pro_mini_black.JPG" data-sub-html="VoyagerRF Pro Black with mini antennas">
      <img src="/images/flipper/pro/pro_mini_black.JPG" width="45%"/>
    </a>
  </div>
</div>

<br>


## Extra covers / +7€

If you can't decide the cover colors and you don't have a 3D printer, you can add an extra 3D covers colors. You can also get the STL and print them if you have 3D printer!

Includes:

- 1 x Front frame + back + pin covers PLA+/PETG case

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_lime_back.JPG" data-sub-html="VoyagerRF Pro White with lime green case">
      <img src="/images/flipper/pro/pro_lime_back.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/pro/pro_orange_back.JPG" data-sub-html="VoyagerRF Pro Black with light orange case">
      <img src="/images/flipper/pro/pro_orange_back.JPG" width="45%"/>
    </a>
  </div>
</div>

<br>

---

<br>



## 3D case files

You can download the STL for the cases and the base here:

| Part     | File       |
| ----------- | ----------- |
| Back  | [Pro_Back.stl](https://kashmir54.github.io/assets/files/cases/Pro_Back.stl)      |
| Base  | [Pro_Base.stl](https://kashmir54.github.io/assets/files/cases/Pro_Base.stl)      |

<br>

To mount it you will need:

- 4x M1.4 screws (6mm or 10mm long)
- 4x M1.4 Brass Insert Nuts

<br>

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/pro/pro_back.png" data-sub-html="Back cover for VoyagerRF Pro">
      <img src="/images/flipper/pro/pro_back.png" width="55%"/>
    </a>
  </div>
</div>


<br>

---

<br>


# Tutorials

You can use the [tutorials for the VoyagerRFv1/v2](https://kashmir54.github.io/flipper/wifi/#tutorials)! Still, I will post here how to update the board with the web flasher.

<div class="error">

<b>DISCLAIMER:</b> These tutorials and information are solely for educational purposes and not an intended use of the device. They provide a hands-on experience in the field of cybersecurity and penetration testing. Any illegal use is strictly prohibited. Test on your own environment and with your own gear.

</div>

<br>

# Troubleshooting

- Wardriving is not working / no GPS data is displayed

GPS is afected by weather, buildings, etc. Best option to pick up satellites is to keep the flipper horizontally so the GPS antena aims to the sky. Stay outdoors and wait for the module to pick satellites.

- I hit **scan** or other commands in ESP32 Wifi Marauder Companion and I get no output or I get a "Press BACK to send stopscan":
  
Make sure you have the board fully connected into the flipper. Go to the menu and enter the Marauder app again.

Make sure you have enough battery in the Flipper.

Hit the reset button (the one in the bottom of the board). 

<p align="center">
  <img src="/images/flipper/reset_button.png" width="40%"/>
</p>


- The micro SD card is connected to the VoyagerRF, but no pcaps are being saved:

Make sure to use **32GB or less** MicroSD card, **FAT32** file format and avoid using **SanDisk** brand (awkward, but the brand counts).

Connect the SD card before entering the ESP32 WiFi Marauder app.


- Please, if you have other issues, contact me on my email.


Thank you for reading and for your interest in the board. I hope you enjoyed this project as much as I did!


