---
title: "Kashmir54 - VoyagerRF for Flipper Zero"
categories: [flipper]
date: 2023-04-06
comments: true
description: VoyagerRF module by Kashmir54
image: /images/kashmir.png
favicon: /images/favicon.ico
---

# VoyagerRF

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_1.jpg" width="40%"/>
  <img src="/images/flipper/voyager_white_2.jpg" width="40%"/>
</div>

<br>

---

<br>

## Page index

- [Intro](#intro)
- [Models](#models)
- [Bundles and Boards](#bundles-and-boards)
- [Community posts](#community-posts)
- [Renders and OBJs](#renders-and-objs)
- [Tutorials](#tutorials)
- [Troubleshooting](#troubleshooting)


<br>

---

<br>


## Intro

After working on the NRF24 board and checking the range improvement of the external CC1101 with the antenna, I wondered if I can also enhance the WiFi dev module in terms of range, size, and functionality. After hours of design, programming, and research, I produced the VoyagerRF, a multipurpose board with several interesting features such as:

- ESP32-C3: A low-consumption WiFi chip with the latest Marauder firmware. Ready to plug and play and control it with the ESP32 WiFi Marauder GPIO app.
- 3dBi Omnidirectional WiFi antenna (2.4GHz) mounted into the board and connected to the ESP32-C3 for great range improvement against the original devboard.
- MicroSD card slot to save the output from the Marauder app (already configured in the firmware).
- NRF24/CC1101 socket with decoupling capacitors for noise reduction. Compatible with [External CC1101 configuration](https://github.com/quen0n/flipperzero-ext-cc1101) and [NRF24 Sniffer/MouseJacker](https://github.com/UberGuidoZ/Flipper/blob/main/GPIO/ReadMe.md#nrf24l01--mousejack-pinout---8-pin) (see chip compatibility since not all chips have the same pinout).
- Compact design 

I have some units in stock. If you are interested in getting the board and other accessories, you can [contact me](mailto:kashmir_54@hotmail.com). Check out the [last section](#bundles-and-boards) to see bundles and prices.

Here you have more photos (also in black matte!):

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_5.jpg" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" width="40%"/>
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

## VoyagerRF Complete Pack / 70€

Available in black or white. Antennas can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.4)
- SMA Connector for ESP32C3 solvered into the PCB
- 3dBi Omni Antenna (2.4GHz)
- 3dBi Helical Antenna (433Mhz) 
- CC1101 module
- NRF24 module

<div display="grid" align="center">
  <img src="/images/flipper/models/white_3.jpg" width="40%"/>
  <img src="/images/flipper/models/black_3.jpg" width="40%"/>
</div>

<br>

## VoyagerRF mini Complete Pack / 65€

Available in black or white. Antennas can be black or white, as you prefer. 

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.4)
- Flat WiFi antenna
- 3dBi Omni Antenna (2.4GHz)
- 3dBi Helical Antenna (433Mhz) 
- CC1101 module
- NRF24 module

<div display="grid" align="center">
  <img src="/images/flipper/models/mini_white_pack_0.jpg" width="40%"/>
  <img src="/images/flipper/models/mini_black_pack_0.jpg" width="40%"/>
</div>

<br>

## VoyagerRF / 45€

Available in black or white. The antenna can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.4)
- SMA Connector for ESP32C3 solvered into the PCB
- 3dBi Omni Antenna (2.4GHz)

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_2.jpg" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" width="40%"/>
</div>

<br>

## VoyagerRF mini / 40€

Available in black or white. The antenna can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.4)
- Flat WiFi antenna

<div display="grid" align="center">
  <img src="/images/flipper/voyagermini_white_0.jpg" width="40%"/>
  <img src="/images/flipper/voyagermini_black.jpg" width="40%"/>
</div>

<br>

---

<br>


# Community posts

Thanks for sharing the VoyagerRF over the internet! This little project got some awesome backers posting on reddit about it, go check them out!

- [Original post](https://www.reddit.com/r/flipperzero/comments/12hfs8t/voyagerrf_a_multipurpose_gpio_board_based_on/)

- [Post by u/Cyber-bros](https://www.reddit.com/r/flipperzero/comments/1375tcj/which_color_would_you_get_white_or_black/)

- [Post by u/Deauther94](https://www.reddit.com/r/flipperzero/comments/12r7ub1/got_a_upgrade/)


<div display="grid" align="center">
  <img src="/images/flipper/reddit/cyber.png" width="40%"/>
  <img src="/images/flipper/reddit/deauth.png" width="40%"/>
</div>


<br>

---

<br>

## Renders, objs and 3D cases.

Many people asked me for the 3D model of the board to start creating some cases, I have uploaded the file and you can download the obj [here](https://kashmir54.github.io/assets/files/OBJ_PCB_VoyagerRF_v0.1.zip). 

Currently I don't have a 3D case design, if you create a 3D case, bear in mind the cable for the back antenna and the antenna itself! Also I will be grateful if you could share the 3D model with me or upload it to any of the free platforms (thingiverse or similars), so others can enjoy it. I will place the link and author in this blog.

- [VoyagerRF ver 0.1](https://kashmir54.github.io/assets/files/OBJ_PCB_VoyagerRF_v0.1.zip)

<p align="center">
  <img src="/images/flipper/voyager_render_v0.png" width="80%"/>
</p>


<br>

---

<br>


# Tutorials

In this section you can find resource to explore further this little board.

- [Getting Started](#getting-started)
- [Flashing XIAO-ESP32-C3](#flashing-xiao-esp32-c3)


## Getting started

New tutorials comming on next updates!


## Flashing Xiao-ESP32-C3

Are you looking to explore further with the ESP32-C3? Then, let me share with you the flashing process and the different steps needed to shape the Arduino IDE environment.

<div class="warning">

<b>WARNING:</b> If you are going to flash the VoyagerRF's ESP32 or connect its USBC to your computer or to a power source, make sure to unplug the VoyagerRF from your flipper in order to avoid malfunctions or data corruption on your Flipper's micro SD card.

</div>


**Step 1.** Install [Arduino IDE](https://www.arduino.cc/en/software).

**Step 2.** Download [latests ESP32 Marauder source code](https://github.com/justcallmekoko/ESP32Marauder/releases/latest) and extract the ZIP content in a folder:

<p align="center">
  <img src="/images/flipper/marauder_src.png" width="80%"/>
</p>

**Step 3.** Go to the folder you have extracted, over esp32_marauder and open esp32_marauder.ino with ArduinoIDE:

<div class="warning">

<b>ESP32Marauder-0.10.4 > esp32_marauder > esp32_marauder.ino</b>

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

<div class="warning">

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



# Troubleshooting


- If the compilation output errors with duplicates variables and headers:

Check that the changes on platform.txt are correct and that you have restarted Arduino IDE.

- I'm connecting the module to the Flipper, but commands are not displaying anything:

Go to Tools > USB CDC On Boot and set "Disable" so the connection is set on the TX/RX ports instead of the USBC connector in the XIAO-ESP32-C3

- I hit scan or other commands in ESP32 Wifi Marauder Companion and I get no output:

This happens usually on Flipper's RogueMaster firmware. Go to GPIO and set _5V on GPIO_ to **ON**. 

- The SD card is not detected and no pcaps are being saved:

Check the configs.h and make sure that only #define MARAUDER_FLIPPER is uncommented and that you have changed the #define SD_CS 4 as previously described.


