---
title: "VoyagerRF for Flipper Zero"
categories: [flipper]
date: 2023-04-06
comments: true
description: VoyagerRF module by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

<p class="vera">VoyagerRF</p>

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_1.jpg" class="main" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" class="main" width="40%"/>
</div>

<br>

---

<br>

## Page index

- [Intro](#intro)
- [Models](#models)
- [Bundles and Boards](#bundles-and-boards)
- [Community posts](#community-posts)
- [Renders and OBJs](#renders-objs-and-3d-cases)
- [Tutorials](#tutorials)
  - [Setup Evilportal](#rogueap--evilportal)
  - [Flashing Marauder on VoyagerRF using web interface](#flashing-marauder-on-voyagerrf-using-web-interface)
  - [Flashing Marauder on VoyagerRF using micro SD card](#flashing-marauder-on-voyagerrf-using-micro-sd-card)
  - [Flashing Marauder on XIAO-ESP32-C3](#flashing-marauder-on-xiao-esp32-c3)
  - [Flashing EvilPortal on XIAO-ESP32-C3](#flashing-evilportal-on-xiao-esp32-c3)
- [Troubleshooting](#troubleshooting)


<br>

---

<br>


## Intro

After working on the NRF24 board and checking the range improvement of the external CC1101 with the antenna, I wondered if I can also enhance the WiFi dev module in terms of range, size, and functionality. After hours of design, programming, and research, I produced the VoyagerRF, a multipurpose board with several interesting features such as:

- ESP32-C3: A low-consumption WiFi and Bluetooth chip with the latest Marauder firmware. This chip will allow you to perform all WiFi and Bluetooth tools deployed on marauder (including evilportal, sour apple, etc). Ready to plug and play and control it with the ESP32 WiFi Marauder GPIO app.
- 3dBi Omnidirectional WiFi antenna (2.4GHz) mounted into the board and connected to the ESP32-C3 for great range improvement against the original devboard.
- MicroSD card slot to save the output from the Marauder app (already configured in the firmware) and different configuration files (for evilportal, for example).
- NRF24/CC1101 socket with decoupling capacitors for noise reduction. Compatible with [External CC1101 configuration](https://github.com/quen0n/flipperzero-ext-cc1101) and [NRF24 Sniffer/MouseJacker](https://github.com/UberGuidoZ/Flipper/blob/main/GPIO/ReadMe.md#nrf24l01--mousejack-pinout---8-pin) (see chip compatibility since not all chips have the same pinout).
- Compact design 

I wanna give a big shoutout to [justcallmekoko](https://github.com/justcallmekoko) for the amazing community he is building and for the developement and maintenance of the [marauder project](https://github.com/justcallmekoko/ESP32Marauder/). Make sure you check out his [YouTube channel](https://www.youtube.com/justcallmekoko).

I have some units in stock. If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com). Check out the [last section](#bundles-and-boards) to see bundles and prices.

Here you have more photos (also in black matte!):

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_5.jpg" width="40%"/>
  <img src="/images/flipper/voyager_white_2.jpg" width="40%"/>
</div>

<br>

---

<br>

## Models

Currently, I am building two models. The first one (normal) is focused on capabilities and range, and it includes an SMA connector to attach a 2.4GHz antenna. This increases the range significantly against the original dev board and any other PCB antennas. 
The second one (mini) is focused on portability and features a flat antenna in the back connected to the ESP32C3. This increases substantially the range in comparison to the original dev board, which makes it an interesting option. Here you have some pictures for the mini board:

<div display="grid" align="center">
  <img src="/images/flipper/voyagermini_black.jpg" width="40%"/>
  <img src="/images/flipper/voyagermini_black_back.jpg" width="40%"/>
</div>

<br>

---

<br>

# Bundles and Boards

If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com) on my email kashmir_54@hotmail.com and I will get back to you!

## VoyagerRF Complete Pack / 70€

Available in black or white. Antennas can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.13.5)
- SMA Connector for ESP32C3 soldered into the PCB
- 3dBi Omni Antenna (2.4GHz)
- 3dBi Helical Antenna (433Mhz) 
- CC1101 module
- NRF24 module

<div display="grid" align="center">
  <img src="/images/flipper/models/white_3.jpg" width="40%"/>
  <img src="/images/flipper/models/black_3.jpg" width="40%"/>
</div>


<br>

## VoyagerRF / 45€

Available in black or white. The antenna can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.13.5)
- SMA Connector for ESP32C3 soldered into the PCB
- 3dBi Omni Antenna (2.4GHz)

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_2.jpg" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" width="40%"/>
</div>


<br>

---

<br>


# Community posts

Thanks for sharing the VoyagerRF over the internet! This little project got some awesome backers posting on reddit about it, go check them out!

- [Original post](https://www.reddit.com/r/flipperzero/comments/12hfs8t/voyagerrf_a_multipurpose_gpio_board_based_on/)

- [Post by u/Cyber-bros](https://www.reddit.com/r/flipperzero/comments/1375tcj/which_color_would_you_get_white_or_black/)

- [Post by u/Deauther94](https://www.reddit.com/r/flipperzero/comments/12r7ub1/got_a_upgrade/)

- [Post by @Ano1X8](https://twitter.com/Ano1X8/status/1697994130465464815)

<div display="grid" align="center">
  <img src="/images/flipper/reddit/cyber.png" width="30%"/>
  <img src="/images/flipper/reddit/deauth.png" width="30%"/>
  <img src="/images/flipper/reddit/anon.png" width="30%"/>
</div>


<br>

---

<br>

## Renders, objs and 3D cases.

Many people asked me for the 3D model of the board to start creating some cases, I have uploaded the file and you can download the obj [here](https://kashmir54.github.io/assets/files/OBJ_PCB_VoyagerRF_v0.1.zip). 

Here are some of the cases created by the community:

- [VoyagerRF cover by CristianMoreira](https://www.thingiverse.com/thing:6262485)

Currently I don't have a 3D case designed by myself, if you create a 3D case, bear in mind the cable for the back antenna and the antenna itself! Also I will be grateful if you could share the 3D model with me or upload it to any of the free platforms (thingiverse or similars), so others can enjoy it. I will place the link and author in this blog.

- [VoyagerRF ver 0.1](https://kashmir54.github.io/assets/files/OBJ_PCB_VoyagerRF_v0.1.zip)

<p align="center">
  <img src="/images/flipper/voyager_render_v0.png" width="80%"/>
</p>


<br>

---

<br>


# Tutorials

In this section you can find resource to explore further this little board.

- [Setup Evilportal](#rogueap--evilportal)
- [Flashing Marauder on VoyagerRF using micro SD card](#flashing-marauder-on-voyagerrf-using-micro-sd-card)
- [Flashing XIAO-ESP32-C3](#flashing-marauder-on-xiao-esp32-c3)
- [Flashing EvilPortal on XIAO-ESP32-C3](#flashing-evilportal-on-xiao-esp32-c3)


## Getting started

What can you do with the VoyagerRF and the rest of the modules in the kit?

Possibilities are limitless! But here I will go over some ideas that you might find interesting and useful in some red teaming tasks.

<div class="error">

<b>DISCLAIMER:</b> These tutorials and information are solely for educational purposes and not an intended use of the device. They provide a hands-on experience in the field of cybersecurity and penetration testing. Any illegal use is strictly prohibited. Test on your own environment and with your own gear.

</div>

<br>

## RogueAP / Evilportal

<div>
<iframe width="560" height="315" src="https://www.youtube.com/embed/q-dpkW7P3o0" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</div>

A rogue access point is a wireless access point that has been installed on a secure network without explicit authorization from a local network administrator, whether added by a well-meaning employee or by a malicious attacker. [Wikipedia, Rogue Access Point](https://en.wikipedia.org/wiki/Rogue_access_point).

In this case, we can use the VoyagerRF and Marauder firmware to create a RogueAP to log user credentials. Requirements:

- [Unleashed Firmware](https://github.com/DarkFlippers/unleashed-firmware) (v060+) or any other firmware with [Marauder Companion app](https://github.com/0xchocolate/flipperzero-wifi-marauder) v0.6.0+ (\[ESP32\] WiFi Marauder app).
- [Marauder firmware (v0.11.0+)](https://github.com/justcallmekoko/ESP32Marauder/wiki/update-firmware) on you WiFi devboard
- WiFi devboard with micro SD card slot (or a VoyagerRF 😉)
- 32GB or less MicroSD card + USB adapter
- A computer or a device to load files into the VoyagerRF board micro SD card

Once you have all the requirements set you are good to go.

Steps:

**Step 1.** Set the configuration files.

You need two files to be placed on the MicroSD card: **ap.config.txt** and **index.html**. The first one holds the AP name (the WiFi name) and the second one stores the html code that will be displayed in the captive portal (the webpage that pops up when you connect to the WiFi).

For the ap.config.txt I will use this content:

```
Google Free Wifi
```

And for the index.html I have use a login form with Google's look and feel. You can choose any html you want, in this [Github repo](https://github.com/bigbrodude6119/flipper-zero-evil-portal/tree/main/portals) you will find many html files created by [roshanravan](https://github.com/roshanravan), download it and rename it to **index.html** .

```html
<!DOCTYPE html>
<html>
<head>
    ...
    <meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1">
</head>
<body>
    <div class="login-container">
        <form action="/get" id="email-form-step">
            ...
            <h1>Sign in</h1>
            <h2>Use your Google Account</h2>
            <input name="email" type="text" class="g-input" placeholder="Email" required>
            <input name="password" type="password" class="g-input" placeholder="Password" required>
            <button class="gbtn-primary" type="submit">Next</button>
        </form>
    </div>
</body>
</html>

```

**Step 2.** Load the files into the MicroSD card.

Just that, place the two files in the root folder or the MicroSD card

**Step 3.** Eject the MicroSD card and plug it into the VoyagerRF board.

**Step 4.** Launch WiFi Marauder app on the flipper with the MicroSD card already plugged in and launch the portal.

**Step 5.** Test it out on your mobile phone, it will display the captive portal once connected. Here are some examples:

<div display="grid" align="center">
  <img src="/images/flipper/evilportal/amazon.jpg" width="30%"/>
  <img src="/images/flipper/evilportal/google.jpg" width="30%"/>
  <img src="/images/flipper/evilportal/microsoft.jpg" width="30%"/>
</div>

_Templates by [roshanravan](https://github.com/roshanravan)._

As easy as that! If you have any concerns, check out the YouTube video showing these steps!

<br>

## NRF24 Sniffer: Wireless BadUSB

TBA

<br>

## WiFi WPA Handshake: catch and crack

TBA

<br>

## Flashing Marauder on VoyagerRF using web interface

I was curious about the Web Serial API and I also wanted to provide an easy way to update the VoyagerRF. That's how I ended up creating the new [VoyagerRF Flasher web application](https://kashmir54.github.io/voyagerrf/). Without further ado, let's see how it works:

<div class="note">

<b>NOTE: Perform the update with the board disconnected from the Flipper. Optionally, remove the MicroSD card from the VoyagerRF.</b>

</div>

**Step 1.** Open the website and connect the board holding the ESP32C3 BOOT button (the one on the top with the "B" letter):

Boot button (press, hold, and connect while holding the button, then, release):

<p align="center">
  <img src="/images/flipper/webflash/1_0_boot.png" width="50%"/>
</p>

Web interface:

<p align="center">
  <img src="/images/flipper/webflash/1_0_flasher.png" width="80%"/>
</p>

**Step 2.** Select the version you want to flash and select the "USB JTAG/serial debug unit". The COM number can be different, so don't worry about that.

<p align="center">
  <img src="/images/flipper/webflash/1_1_connect.png" width="60%"/>
</p>

**Step 3.** Now, select "Install VoyagerRF Marauder vX.X.X"

<p align="center">
  <img src="/images/flipper/webflash/1_2_install.png" width="50%"/>
</p>

**Step 4.** Hit next, doesn't matter if you erase the device, MicroSD card data will remain.

<p align="center">
  <img src="/images/flipper/webflash/1_1_install.png" width="40%"/>
</p>

**Step 5.** Hit "Install" to perform the update

<p align="center">
  <img src="/images/flipper/webflash/1_3_install.png" width="40%"/>
</p>

**Step 6.** Wait for the process to finish. If successful, the following message will be displayed:

<p align="center">
  <img src="/images/flipper/webflash/1_4_done.png" width="40%"/>
</p>

Now you can check the version in the reboot option in the ESP32 WiFi Marauder app in your Flipper!


## Flashing Marauder on VoyagerRF using micro SD card

Since current update from source might be hard to compile and manage with the new dependencies, I have prepared the bin file to update from the SD on marauder firmware. If you got a VoyagerRF, you have Marauder firmware preinstalled, so you can perform this update.

Here you have the video:

<div>
<iframe width="560" height="315" src="https://www.youtube.com/embed/JZKtMV9_qtk" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>
</div>

And these are the steps to update:

**Step 1.** Download the desired version of marauder from the following options:

| Version     | File       |
| ----------- | ----------- |
| [v0.10.7](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.10.7)   | [voyagerrf_marauder_v0_10_7.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_10_7.bin)      |
| [v0.11.0-rc1](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.11.0-RC1)   | [voyagerrf_marauder_v0_11_0.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_11_0.bin)      |
| [v0.11.0-rc2](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.11.0-RC2)   | [voyagerrf_marauder_v0_11_0_rc2.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_11_0_rc2.bin)      |
| [v0.11.0-rc3](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.11.0-RC3)   | [voyagerrf_marauder_v0_11_0_rc3.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_11_0_rc3.bin)      |
| [v0.12.0](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.12.0)  | [voyagerrf_marauder_v0_12_0.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_12_0.bin)      |
| [v0.13.1](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.13.1)  | [voyagerrf_marauder_v0_13_1.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_13_1.bin)      |
| [v0.13.2](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.13.2)  | [voyagerrf_marauder_v0_13_2.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_13_2.bin)      |
| [v0.13.4](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.13.4)  | [voyagerrf_marauder_v0_13_4.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_13_4.bin)      |
| [v0.13.5](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.13.5)  | [voyagerrf_marauder_v0_13_5.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_13_5.bin)      |
| [v0.13.6](https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v0.13.6)  | [voyagerrf_marauder_v0_13_6.bin](https://kashmir54.github.io/assets/files/voyagerrf_marauder_v0_13_6.bin)      |



**Step 2.** Rename the file to **update.bin** and move it to your micro SD card (the one that will go to the VoyagerRF).

<p align="center">
  <img src="/images/flipper/update/file_sd.png" width="70%"/>
</p>

**Step 3.** Insert the micro SD card in the VoyagerRF and enter into the companion app (ESP32 WiFi Marauder app) and go to _Update_ option and select the sd option, hit enter, then if you have renamed the file correctly, the update will start:

<p align="center">
  <img src="/images/flipper/update/1_0_update.png" width="55%"/>
</p>

Updating...

<p align="center">
  <img src="/images/flipper/update/1_1_update.png" width="55%"/>
</p>

Reboot the Marauder and you can see the new version:

<p align="center">
  <img src="/images/flipper/update/1_2_update.png" width="55%"/>
</p>

And the evilportal command (if you got the v0.11.0+) ready to use:

<p align="center">
  <img src="/images/flipper/update/1_3_update.png" width="55%"/>
</p>

That's it, now we have to wait until companion app gets updated.


## Flashing Marauder on Xiao-ESP32-C3

Are you looking to explore further with the ESP32-C3? Then, let me share with you the flashing process and the different steps needed to shape the Arduino IDE environment.

<div class="warning">

<b>WARNING:</b> This tutorial is valid for Marauder v0.10.7 or before, version v0.11.0 requires tricky configurations that won't be explained in this page. If you already have marauder firmware, use the [update from micro SD card](#flashing-marauder-on-voyagerrf-using-micro-sd-card) option, it will be painless. If you installed evilportal or other firmware and you don't have the possibility to update from the micro sd card, I suggest you install this v0.10.0 version and then update from the sd card.

If you are going to flash the VoyagerRF's ESP32 or connect its USBC to your computer or to a power source, make sure to unplug the VoyagerRF from your flipper in order to avoid malfunctions or data corruption on your Flipper's micro SD card.

</div>


**Step 1.** Install [Arduino IDE](https://www.arduino.cc/en/software).

**Step 2.** Download [latests ESP32 Marauder source code](https://github.com/justcallmekoko/ESP32Marauder/releases/latest) and extract the ZIP content in a folder:

<p align="center">
  <img src="/images/flipper/marauder_src.png" width="80%"/>
</p>

**Step 3.** Go to the folder you have extracted, over esp32_marauder and open esp32_marauder.ino with ArduinoIDE:

<div class="note">

<b>ESP32Marauder-0.10.x > esp32_marauder > esp32_marauder.ino</b>

</div>

<p align="center">
  <img src="/images/flipper/open_ino.png" width="80%"/>
</p>

**Step 4.** Set up the libraries for Marauder:

We will install the firmware from the source code, therefore, we will need the libraries used in the Marauder. To do it, download the libraries as a zip as displayed in the following image:

<p align="center">
  <img src="/images/flipper/download.png" width="90%"/>
</p>

You gotta do it over all the following requirements:

- [lv_arduino](https://github.com/lvgl/lv_arduino)
- [LinkedList](https://github.com/ivanseidel/LinkedList)
- [TFT_eSPI](https://github.com/justcallmekoko/TFT_eSPI)
- [JPEGDecoder](https://github.com/Bodmer/JPEGDecoder)
- [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
- [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson/archive/refs/tags/v6.18.2.zip)
- [SwitchLib](https://github.com/justcallmekoko/SwitchLib/archive/refs/tags/v1.1.1.zip)

Then to add the libraries go to the following menu and select the downloaded ZIPs one by one.

<p align="center">
  <img src="/images/flipper/add_lib.png" width="70%"/>
</p>

**Step 5.** Install _esp32 by Espressif Systems_ from the Boards Manager:

Currently, I found the ESP32 stable at version 2.0.9. Go to boards, search for esp32 and install version 2.0.9:

<p align="center">
  <img src="/images/flipper/esp32.png" width="50%"/>
</p>


**Step 6.** Modifying platform.txt for ESP32-C3 support:

As stated in the [ArduinoIDE setup instructions](https://github.com/justcallmekoko/ESP32Marauder/wiki/arduino-ide-setup#if-you-are-following-these-instructions-you-do-not-need-to-do-this), you have to modify the platform.txt. To do that, find the esp32 platform.txt on your system. It should be somewhere near this path:

<div class="note">

<b>C:\Users\your_user\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6\platform.txt</b>

</div>

Now, add **-w** to build.extra_flags.esp32c3 option:

```bash
build.extra_flags.esp32c3=-w -DARDUINO_USB_MODE=1 -DARDUINO_USB_CDC_ON_BOOT={build.cdc_on_boot} ...
```

And add **-zmuldefs** to compiler.c.elf.libs.esp32c3 option:

```bash
compiler.c.elf.libs.esp32c3=-zmuldefs -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm ...
```

Save and restart Arduino IDE (if open).

**Step 7.** Change Micro SD Card pin

In the configs.h, change the MARAUDER_FLIPPER SD_CS definition to pin 4, since the VoyagerRF board is designed to work with CS pin in that GPIO:

```c
// SD DEFINITIONS
#ifdef MARAUDER_V4
  #define SD_CS 12
#endif

#ifdef MARAUDER_V6
  #define SD_CS 12
#endif

#ifdef MARAUDER_KIT
  #define SD_CS 12
#endif

#ifdef MARAUDER_MINI
  #define SD_CS 4
#endif

#ifdef MARAUDER_FLIPPER
  #define SD_CS 4 // Change from 10 to 4 here
#endif

#ifdef ESP32_LDDB
  #define SD_CS 4
#endif
```

**Step 8.** Set the board (XIAO-ESP32-C3)

Connect your VoyagerRF board (remember to disconnect it from the Flipper). Go to the boards menu and select the proper model (XIAO-ESP32-C3):

<p align="center">
  <img src="/images/flipper/xiao_esp32_option.png" width="80%"/>
</p>

Go over port and select the board that poped up when connected (don't mind the fingerprint, it will be a little bit random):

<p align="center">
  <img src="/images/flipper/port.png" width="70%"/>
</p>

The rest of the options are as follow:

| Setting     | Value       |
| ----------- | ----------- |
| USB CDC On Boot (important)   | Disabled      |
| CPU Frequency         | 160MHz (WiFi) |
| Core Debug Level      | None          |
| Erase All Flash...    | Disabled      |
| Flash Frequency       | 80 MHz        |
| Flash Mode            | QIO           |
| Flash Size            | 4MB (32Mb)    |
| Partition Scheme      | Minimal SPIFFS|
| Upload Speed          | 921600        |


**Step 9.** Hit Compile and Upload and all should run correctly. You should have this output if all went smoothly:

<p align="center">
  <img src="/images/flipper/output.png" width="80%"/>
</p>


## Flashing EvilPortal on Xiao-ESP32-C3

Many people have requested a guide for flashing ESP32C3 with new trending app, [EvilPortal](https://github.com/bigbrodude6119/flipper-zero-evil-portal). Following these steps you will achieve it.

<div class="warning">

<b>WARNING:</b> Marauder firmware (v0.11.0+) and Flipper's Marauder Companion app (v0.6.0+) already includes the evilportal within, so there is no need to have this Evilportal firmware. If you flash the VoyagerRF board with this standalone version you won't be able to use marauder until you flash it again. This steps were valid on version 0.0.2. Tutorial could get old as the version evolve.

</div>

**Step 1.** Install [Arduino IDE](https://www.arduino.cc/en/software).


**Step 2.** Download [latests evilportal release](https://github.com/bigbrodude6119/flipper-zero-evil-portal/releases/latest) specifically, we will need EvilPortal.ino. You might want to download evil_portal_sd_folder.zip and unleashed-.evil_portal.fap.zip since you need them for seting up your Flipper:

<p align="center">
  <img src="/images/flipper/evilportal/evilportal_src.png" width="90%"/>
</p>


**Step 3.** Open EvilPortal.ino with ArduinoIDE:

<p align="center">
  <img src="/images/flipper/evilportal/open_ino_2.png" width="80%"/>
</p>


**Step 4.** Set up the libraries for EvilPortal:

We will install the firmware from the source code, therefore, we will need the libraries used in the EvilPortal. To do it, download the libraries as a zip as displayed in the following image:

<p align="center">
  <img src="/images/flipper/evilportal/lib.png" width="90%"/>
</p>

You gotta do it over all the following libraries:

- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)

Then to add the libraries go to the following menu and select the downloaded ZIPs one by one.

<p align="center">
  <img src="/images/flipper/evilportal/install_lib.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/flipper/evilportal/add_lib.png" width="70%"/>
</p>

**Step 5.** Install _esp32 by Espressif Systems_ from the Boards Manager:

Currently, I found the ESP32 stable at version 2.0.9. Go to boards, search for esp32 and install version 2.0.9:

<p align="center">
  <img src="/images/flipper/esp32.png" width="50%"/>
</p>

**Step 6** Modify AsyncWebSocket.cpp:

There is a bug in the ESPAsyncWebServer library for the ESP32C3, mainteiner has not fixed it, so we have to do it manually. 

Go to the folder where the file is, it should be something like this one:

<div class="note">

<b>NOTE:</b> C:\Users\youruser\Documents\Arduino\libraries\ESPAsyncWebServer-master\src

</div>

Then open the file and edit the following line:

<p align="center">
  <img src="/images/flipper/evilportal/edit.png" width="70%"/>
</p>

<p align="center">
  <img src="/images/flipper/evilportal/line.png" width="70%"/>
</p>

Replace it with the following typed return statement as shown in the following piece of code: _return IPAddress((uint32_t)0);_

```c
IPAddress AsyncWebSocketClient::remoteIP() {
    if(!_client) {
        return IPAddress((uint32_t)0);  //Before: return IPAddress(0U);
    }
    return _client->remoteIP();
}
```

Code and library wise you are all set, now move into board selection.


**Step 7** Set the board (XIAO-ESP32-C3)

Connect your VoyagerRF board (remember to disconnect it from the Flipper). Go to the boards menu and select the proper model (XIAO-ESP32-C3):

<p align="center">
  <img src="/images/flipper/evilportal/board.png" width="80%"/>
</p>

Go over port and select the board that poped up when connected (don't mind the fingerprint, it will be a little bit random):

<p align="center">
  <img src="/images/flipper/evilportal/port.png" width="70%"/>
</p>

The rest of the options are as follow:

| Setting     | Value       |
| ----------- | ----------- |
| USB CDC On Boot (important)   | Disabled      |
| CPU Frequency         | 160MHz (WiFi) |
| Core Debug Level      | None          |
| Erase All Flash...    | Disabled      |
| Flash Frequency       | 80 MHz        |
| Flash Mode            | QIO           |
| Flash Size            | 4MB (32Mb)    |
| Partition Scheme      | Minimal SPIFFS|
| Upload Speed          | 921600        |


If everything is successful, you should see the following messages:

<p align="center">
  <img src="/images/flipper/evilportal/ok.png" width="80%"/>
</p>

Now we can set up the rest of the environment.


**Step 8** Load APP (.fap) and set up data to your Flipper:

Connect your flipper, use the qFlipper app or other file explorer, then, get the unleashed-evil_portal.fap.zip we downloaded from the repo, unzip it, and drop it to the apps/gpio folder as shown in the image:

<p align="center">
  <img src="/images/flipper/evilportal/app.png" width="70%"/>
</p>

Do the same with the folder evil_portal_sd_folder.zip, unzip it, and drop the complete folder 

<p align="center">
  <img src="/images/flipper/evilportal/evil_folder.png" width="70%"/>
</p>

Within that folder you should have two files, ap.config.txt, where you have to place the AP name and the index.html, which is the page that is going to load when the Captive Portal is displayed in the phone.

And that's it, you are now able to plug the VoyagerRF and show up with the captive portal when clients connect. You can also save the logs to the flipper sd card with no issue:

<p align="center">
  <img src="/images/flipper/evilportal/set_up.jpg" width="80%"/>
</p>

We connect to the Wifi Network and the captive portal shows up:

<p align="center">
  <img src="/images/flipper/evilportal/captive_portal.jpg" width="80%"/>
</p>

Inseting the credentials and saving them to the log file:

<p align="center">
  <img src="/images/flipper/evilportal/logs.png" width="50%"/>
</p>


# Troubleshooting

- If the compilation output errors with duplicates variables and headers:

Check that the changes on platform.txt are correct and that you have restarted Arduino IDE.

- I'm connecting the module to the Flipper, but commands are not displaying anything:

Go to Tools > USB CDC On Boot and set "Disable" so the connection is set on the TX/RX ports instead of the USBC connector in the XIAO-ESP32-C3

- I'm trying to update, but it is not working.

Using a Samsung MicroSD card will cause Marauder not to boot, as stated on the [official wiki](https://github.com/justcallmekoko/ESP32Marauder/wiki/sd-update).

- I hit **scan** or other commands in ESP32 Wifi Marauder Companion and I get no output or I get a "Press BACK to send stopscan":

  - If you are using the VoyagerRF **WITH** the Micro SD card connected:
    
    This happens usually on Flipper's RogueMaster firmware. Go to GPIO and set _5V on GPIO_ to **ON**.

  - If you are using the VoyagerRF **WITHOUT** the Micro SD card connected:
    
    There is a bug on the Marauder v0.10.4 firmware or companion app. If there is no SD card connected, when booting (entering the ESP32 WiFi Marauder app), the ESP32 will be bugged, you can hit the reset button while being in the app and you can use the module as normal. Everytime you get into the app with no Micro SD card connected to the VoyagerRF, you will have to hit the reset button. I'm trying to work this out.

<p align="center">
  <img src="/images/flipper/reset_button.png" width="60%"/>
</p>

- The micro SD card is connected to the VoyagerRF, but no pcaps are being saved:

Make sure to use **32GB or less** MicroSD card, **FAT32** file format and NOT to use **SanDisk** brand (awkward, but the brand counts).

Check the configs.h and make sure that only #define MARAUDER_FLIPPER is uncommented and that you have changed the #define SD_CS 4 as previously described.

Connect the SD card before entering the ESP32 WiFi Marauder app.

- Please, if you have other issues, please, contact me on my email.

