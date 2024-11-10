---
title: "VoyagerRF Micro for Flipper Zero"
categories: [flipper]
comments: true
description: VoyagerRF Micro module by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

<p class="vera">VoyagerRF Micro</p>

A small form factor ESP32 WiFi Devboard for Flipper Zero with GPS.

<br>
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/both.JPG" data-sub-html="VoyagerRF Micro">
      <img src="/images/flipper/micro/both.JPG" class="main" width="70%"/>
    </a>

  </div>
</div>

<br>

---

<br>

## Page index

- [Intro](#intro)
- [Galery](#galery)
- [Bundles and Boards](#bundles-and-boards)
- [3D Case files](#3d-case-files)
- [Tutorials](#tutorials)
- [Troubleshooting](#troubleshooting)


<br>

## Related tools

<div class="grid-one">
  <a class="box" href="https://kashmir54.github.io/voyagerrf/index.html">
    <div class="main-box" >
      <div style="padding-left: 10%; padding-right: 10%;">
        <p>Update or switch your firmware on the VoyagerRF Micro board easily! (Marauder / Ghost_ESP)</p>
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

From previous [VoyagerRF](https://kashmir54.github.io/flipper/wifi/) and [VoyagerRFv2](https://kashmir54.github.io/flipper/voyagerrf_v2/) I realized that I wanted something smaller since many times I won't need the nRF24 nor the CC1101 attachment. So, that's why I crafted the VoyagerRF Micro, a compact horizontally mounted ESP32-C3 based WiFi/BLE Devboard that supports [Marauder firmware](https://github.com/justcallmekoko/ESP32Marauder/) developed by [justcallmekoko](https://github.com/justcallmekoko) and [Ghost_ESP](https://github.com/Spooks4576/Ghost_ESP) by [Spooks4576](https://github.com/Spooks4576). It supports several feaures such as EvilPortal, Wardriving, Bluetooth tools, WiFi tools and the NeoPixel RGB LED.

Let's check the new features:

- ⚡️ **Ready for Ghost_ESP Firmware:** Now the VoyagerRFv2 board supports Ghost_ESP, you can use the [Web Flasher](https://kashmir54.github.io/voyagerrf/) to easily change/update your firmware.
- ⚡️ **New form factor:** The new PLA antenna/complete enclosure and the horizontal mount makes this devboard really compact, so it can fit into a pocket while connected to the flipper, making it easy to carry, specially on wardriving.
- ❌ **Removed CC1101/nRF24 Socket:** For the sake of being compact, I decided to remove the CC1101 and nRF24 since I use those in certain tasks, but normally I don't need them.

And the features inherited from previous versions:

- ✅ **GPS module and antenna:** The ATGM336H GPS module is connected to the ESP32C3, which allows the Marauder firmware to collect GeoPositioning information on wardriving mode. This mode saves the location and WiFi SSID (among other data) in the VoyagerRF's MicroSD card with a certain file formatthat can be uploaded on websites such as [wigle.net](https://wigle.net/).
- ✅ **NeoPixel status LED:** This RGB LED will indicate the status of the Marauder firmware. Blue for sniffing and red for attack mode. You can also set manually the HEX code for the LED or flex it with the raimbow mode.
- ✅ **3D Printed case:** For people looking for a clean look, I designed a case and a base to store and carry your VoyagerRF everywhere. 3D models ready for printing are provided below. (To be honest, I prefer to see the PCB and components).
- ✅ **ESP32-C3:** A low-consumption **WiFi and Bluetooth** chip with the latest [Marauder firmware](https://github.com/justcallmekoko/ESP32Marauder/). This chip will allow you to use all WiFi and Bluetooth tools deployed on marauder (including deauths, evilportal, sour apple, BLE spam, etc). Ready to plug and play and control it with the [ESP32 WiFi Marauder](https://github.com/0xchocolate/flipperzero-wifi-marauder/releases) GPIO app from the flipper.
- ✅ **MicroSD card slot:** You can opt to save everything on Flipper's SD Card, but I prefer to have a spare SD card to save the output from the Marauder app (already configured in the firmware) and to store different configuration files (for evilportal, for example).
- ✅ **3dBi Omnidirectional Mini WiFi antenna** (2.4GHz) mounted into the board's SMA connector. Adds a great range improvement against the original devboard (PCB antenna). I might look small, but the range is outstanding.

Remember that the VoyagerRF Micro can be also updated via the [Web Flasher](https://kashmir54.github.io/voyagerrf/v2.html) following the steps in the website. It's so easy, just connect the board via USB to your PC with Google Chrome browser, click 3 buttons and you are ready to go!

If you are reading this, it is because I have units 🟢 **in stock**. If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com). Check out the [bundles and boards](#bundles-and-boards) to see the prices.

<br>

---

<br>

## Galery

Here you have more photos with the small and included case and the extra full cover:

Front:
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/both.JPG" data-sub-html="VoyagerRF Micro">
      <img src="/images/flipper/micro/both.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/both_b.JPG" data-sub-html="VoyagerRF Micro back">
      <img src="/images/flipper/micro/both_b.JPG" width="45%"/>
    </a>
  </div>
</div>
<br>

Back:
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/both_back.JPG" data-sub-html="VoyagerRF Micro">
      <img src="/images/flipper/micro/both_back.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/both_back_b.JPG" data-sub-html="VoyagerRF Micro back">
      <img src="/images/flipper/micro/both_back_b.JPG" width="45%"/>
    </a>
  </div>
</div>
<br>

Some more details here and there 🌊:
<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/black_1.JPG" data-sub-html="VoyagerRF Micro Black front no case">
      <img src="/images/flipper/micro/black_1.JPG" width="30%"/>
    </a>
    <a href="/images/flipper/micro/black_2.JPG" data-sub-html="VoyagerRF Micro Black back no case">
      <img src="/images/flipper/micro/black_2.JPG" width="30%"/>
    </a>
    <a href="/images/flipper/micro/black_3.JPG" data-sub-html="VoyagerRF Micro Black no case">
      <img src="/images/flipper/micro/black_3.JPG" width="30%"/>
    </a>
    <a href="/images/flipper/micro/white_1.JPG" data-sub-html="VoyagerRF Micro White front no case">
      <img src="/images/flipper/micro/white_1.JPG" width="30%"/>
    </a>
    <a href="/images/flipper/micro/white_2.JPG" data-sub-html="VoyagerRF Micro White back no case">
      <img src="/images/flipper/micro/white_2.JPG" width="30%"/>
    </a>
    <a href="/images/flipper/micro/white_3.JPG" data-sub-html="VoyagerRF Micro White">
      <img src="/images/flipper/micro/white_3.JPG" width="30%"/>
    </a>
  </div>
</div>
<br>

<br>

---

<br>

# Bundles and Boards

If you are interested in getting the board and the case, you can [contact me](mailto:kashmir_54@hotmail.com) on my email kashmir_54@hotmail.com and I will get back to you! If you read this, that's because they are 🟢 **in stock!** 

<br>

## VoyagerRF Micro with full cover / 50€

Available in black or white. Antennas can be black or white separatelly, as you prefer.

Includes:

- VoyagerRF Micro board with latest Marauder Firmware preinstalled
- **Full PLA case** + 2xM1.4 Screws
- 3dBi Mini WiFi Omni Antenna (2.4GHz)
- GPS Antenna (under the case)

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/black_b1.JPG" data-sub-html="VoyagerRF Micro black with full cover">
      <img src="/images/flipper/micro/black_b1.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/white_b1.JPG" data-sub-html="VoyagerRF Micro black back with full cover">
      <img src="/images/flipper/micro/white_b1.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/black_b2.JPG" data-sub-html="VoyagerRF Micro white with full cover">
      <img src="/images/flipper/micro/black_b2.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/white_b2.JPG" data-sub-html="VoyagerRF Micro white back with full cover">
      <img src="/images/flipper/micro/white_b2.JPG" width="45%"/>
    </a>
  </div>
</div>


<br>

## VoyagerRF Micro / 45€

Available in black or white. The antenna can be black or white separatelly, as you prefer.

Includes:

- VoyagerRF Micro board with latest Marauder Firmware preinstalled
- **GPS antenna PLA case** + 2xM1.4 Screws
- 3dBi Mini WiFi Omni Antenna (2.4GHz)
- GPS Antenna (under the case)

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/black_1.JPG" data-sub-html="VoyagerRF Micro">
      <img src="/images/flipper/micro/black_1.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/white_1.JPG" data-sub-html="VoyagerRF Micro back">
      <img src="/images/flipper/micro/white_1.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/black_2.JPG" data-sub-html="VoyagerRF Micro">
      <img src="/images/flipper/micro/black_2.JPG" width="45%"/>
    </a>
    <a href="/images/flipper/micro/white_2.JPG" data-sub-html="VoyagerRF Micro back">
      <img src="/images/flipper/micro/white_2.JPG" width="45%"/>
    </a>
  </div>
</div>
<br>


---

<br>



## 3D case files

You can download the STL for the small cases and the full case here soon!

| Part     | File       | Thingiverse |
| ----------- | ----------- | ----------- |
| GPS antenna cover | [v1_micro_mini_cover.stl](https://kashmir54.github.io/assets/files/cases/v1_micro_mini_cover.stl)      | [TBA]()      |
| Full case  | [v1_micro_full_cover.stl](https://kashmir54.github.io/assets/files/cases/v1_micro_full_cover.stl)      | [TBA]()      |

<br>

To mount it you will need:

- 4x M1.4 (6mm long)
- 4x M1.4 nut inserts

<br>

<div display="grid" align="center">
  <div class="lightgallery">
    <a href="/images/flipper/micro/mini_cover.png" data-sub-html="Mini cover for VoyagerRF Micro">
      <img src="/images/flipper/micro/mini_cover.png" width="40%"/>
    </a>
    <a href="/images/flipper/micro/cover.png" data-sub-html="Full cover for VoyagerRF Micro">
      <img src="/images/flipper/micro/cover.png" width="45%"/>
    </a>
  </div>
</div>


<br>

---

<br>


# Tutorials

You can use the [tutorials for the VoyagerRFv1](https://kashmir54.github.io/flipper/wifi/#tutorials) and [VoyagerRFv2](https://kashmir54.github.io/flipper/voyagerrf_v2/#tutorials)! I will remake a tutorial section with a compilation for all boards, so it is not as messy as now 😬.

<div class="error">

<b>DISCLAIMER:</b> All tutorials and information are solely for educational purposes and not an intended use of the device. They provide a hands-on experience in the field of cybersecurity and penetration testing. Any illegal use is strictly prohibited. Test on your own environment and with your own gear.

</div>

<br>


# Troubleshooting

- Wardriving is not working / no GPS data is displayed

GPS is afected by weather, buildings, etc. Best option to get a fix with a set of satellites is to keep the Flipper upwards so the GPS antena aims to the sky. Stay outdoors and wait for the module to get the fix (red light blinks).

- I hit **scan** or other commands in ESP32 Wifi Marauder Companion and I get no output or I get a "Press BACK to send stopscan":
  
Make sure you have the board fully connected into the flipper. Go to the menu and enter the Marauder app again.

Make sure you have enough battery in the Flipper.

Hit the reset button (the one in the bottom of the board). 

<p align="center">
  <img src="/images/flipper/micro/reset.png" width="60%"/>
</p>

Last option, go to the [Web Flasher](https://kashmir54.github.io/voyagerrf/v2.html) and reinstall the firmware, just it case it got corrupted.


- The Micro SD card is connected to the VoyagerRF, but no pcaps are being saved:

Make sure to use **32GB or less** MicroSD card, **FAT32** file format and avoid using **SanDisk** brand (awkward, but the brand counts).

Connect the SD card before entering the ESP32 WiFi Marauder app.


- Please, if you have other issues, contact me on my email!


Thank you for reading and for your interest in the board. I hope you enjoy this project as much as I do!


