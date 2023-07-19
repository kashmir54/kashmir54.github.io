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
- [Renders and OBJs](#renders-objs-and-3d-cases)
- [Tutorials](#tutorials)
  - [Flashing Marauder on XIAO-ESP32-C3](#flashing-marauder-on-xiao-esp32-c3)
  - [Flashing EvilPortal on XIAO-ESP32-C3](#flashing-evilportal-on-xiao-esp32-c3)
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

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.7)
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

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.7)
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

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.7)
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

- VoyagerRF board with Marauder Firmware preinstalled (v0.10.7)
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
- [Flashing XIAO-ESP32-C3](#flashing-marauder-on-xiao-esp32-c3)
- [Flashing EvilPortal on XIAO-ESP32-C3](#flashing-evilportal-on-xiao-esp32-c3)

## Getting started

New tutorials comming on next updates!


## Flashing Marauder on Xiao-ESP32-C3

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


## Flashing Marauder on Xiao-ESP32-C3

Many people have requested a guide for flashing ESP32C3 with new trending app, [EvilPortal](https://github.com/bigbrodude6119/flipper-zero-evil-portal). Following these steps you will achieve it.

<div class="warning">

<b>NOTE:</b> This steps were valid on version 0.0.2. Tutorial could get old as the version evolve.

</div>

**Step 1.** Install [Arduino IDE](https://www.arduino.cc/en/software).


**Step 2.** Download [latests evilportal release](https://github.com/bigbrodude6119/flipper-zero-evil-portal/releases/latest) specifically, we will need EvilPortal.ino. You might want to download evil_portal_sd_folder.zip and unleashed-.evil_portal.fap.zip since you need them for seting up your Flipper:

<p align="center">
  <img src="/images/flipper/evilportal/evilportal_src.png" width="80%"/>
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
  <img src="/images/flipper/evilportal/esp32.png" width="50%"/>
</p>

**Step 6** Modify AsyncWebSocket.cpp:

There is a bug in the ESPAsyncWebServer library for the ESP32C3, mainteiner has not fixed it, so we have to do it manually. 

Go to the folder where the file is, it should be something like this one:

<div class="warning">

<b>NOTE:</b> C:\Users\youruser\Documents\Arduino\libraries\ESPAsyncWebServer-master\src

</div>

Then open the file and edit the following line:

<p align="center">
  <img src="/images/flipper/evilportal/edit.png" width="50%"/>
</p>

<p align="center">
  <img src="/images/flipper/evilportal/line.png" width="50%"/>
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
  <img src="/images/flipper/evilportal/ok.png" width="70%"/>
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

- I hit **scan** or other commands in ESP32 Wifi Marauder Companion and I get no output or I get a "Press BACK to send stopscan":

  - If you are using the VoyagerRF **WITH** the Micro SD card connected:
    
    This happens usually on Flipper's RogueMaster firmware. Go to GPIO and set _5V on GPIO_ to **ON**.

  - If you are using the VoyagerRF **WITHOUT** the Micro SD card connected:
    
    There is a bug on the Marauder v0.10.4 firmware or companion app. If there is no SD card connected, when booting (entering the ESP32 WiFi Marauder app), the ESP32 will be bugged, you can hit the reset button while being in the app and you can use the module as normal. Everytime you get into the app with no Micro SD card connected to the VoyagerRF, you will have to hit the reset button. I'm trying to work this out.

<p align="center">
  <img src="/images/flipper/reset_button.png" width="60%"/>
</p>

- The SD card is connected and no pcaps are being saved:

Check the configs.h and make sure that only #define MARAUDER_FLIPPER is uncommented and that you have changed the #define SD_CS 4 as previously described.

Connect the SD card before entering the ESP32 WiFi Marauder app.


- Please, if you have other issues, please, contact me on my email.

