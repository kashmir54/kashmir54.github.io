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

After working on the NRF24 board and checking the range improvement of the external CC1101 with the antenna, I wonderedif I can also enhance the WiFi dev module in terms of range, size and functionality. After hours of design, programming and research, I produced the VoyagerRF, a multipurpose board with several interesting features such as:

- ESP32-C3: A low consumption WiFi chip with latest Marauder firmware. Ready to plug-and-play and control it with the ESP32 WiFi Marauder GPIO app.
- 3dBi Omnidirectional WiFi antena (2.4GHz) mounted into the board and connected to the ESP32-C3 for great range improvement against the original devboard.
- MicroSD card slot to save the output from the Marauder app (already configured in the firmware).
- NRF24/CC1101 socket with decoupling capacitors for noise reduction. Compatible with [External CC1101 configuration](https://github.com/quen0n/flipperzero-ext-cc1101) and [NRF24 Sniffer/MouseJacker](https://github.com/UberGuidoZ/Flipper/blob/main/GPIO/ReadMe.md#nrf24l01--mousejack-pinout---8-pin) (see chip compatibility since not all chips have the same pinout).
- Compact adesign 

I have some units in stock. If you are interested in getting the board and other accesories, you can [contact me](mailto:kashmir_54@hotmail.com). Check out the [last section](#bundles-and-boards) to see bundles and prices.

Here you have more photos (there is also a black version!):

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_5.jpg" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" width="40%"/>
</div>

<br>

## Models

Currently, I am building two models. The first one is focused on capabilities and range, and it includes an SMA connector to attach a 2.4GHz antenna. This increases the range significantly against the original dev board. The second one is focused on portability, and features a flat antenna in the back connected to the ESP32C3. This increases substantially the range in comparison to the original dev board, which makes it an interesting option.

<div display="grid" align="center">
  <img src="/images/flipper/voyagermini_black.jpg" width="40%"/>
  <img src="/images/flipper/voyagermini_black_back.jpg" width="40%"/>
</div>

<br>

## Flashing Xiao-ESP32-C3

Are you looking to explore further with the ESP32-C3? Then, let me share with you the flashing process and the different steps needed to shape the Arduino IDE environment:

- Install [Arduino IDE](https://www.arduino.cc/en/software).

- Set up the libraries for Marauder:

We will install the firmware from the source code, therefore, we will need the libraries used in the marauder. To do it, download the libraries as zip as displayed in the following image:

<p align="center">
  <img src="/images/flipper/download.png" width="70%"/>
</p>

You gotta do it over all the following requirements:

[lv_arduino](https://github.com/lvgl/lv_arduino)
[LinkedList](https://github.com/ivanseidel/LinkedList)
[TFT_eSPI](https://github.com/justcallmekoko/TFT_eSPI)
[JPEGDecoder](https://github.com/Bodmer/JPEGDecoder)
[NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
[Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
[ArduinoJson](https://github.com/bblanchon/ArduinoJson/archive/refs/tags/v6.18.2.zip)
[SwitchLib](https://github.com/justcallmekoko/SwitchLib/archive/refs/tags/v1.1.1.zip)

Then to add the libraries go to the following menu and select the downloaded ZIPs one by one.

<p align="center">
  <img src="/images/flipper/add_lib.png" width="70%"/>
</p>

- Install esp32 board

Currently, I found the esp32 stable atversion 2.0.6, I will explore newer versions and test them out, but for now: Go to boards, search for esp32 and install version 2.0.6:

<p align="center">
  <img src="/images/flipper/esp32.png" width="50%"/>
</p>


- Modifiying platform.txt for ESP32-C3 support:

As stated in the [ArduinoIDE setup instructions](https://github.com/justcallmekoko/ESP32Marauder/wiki/arduino-ide-setup#if-you-are-following-these-instructions-you-do-not-need-to-do-this), you have to modify the platform.txt. To do that, find the esp32 platform.txt on your system. It should be some place near this path:

```
C:\Users\your_user\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.6\platform.txt
```

Now, add **-w** to build.extra_flags.esp32c3 option:

```bash
build.extra_flags.esp32c3=-w -DARDUINO_USB_MODE=1 -DARDUINO_USB_CDC_ON_BOOT={build.cdc_on_boot} ...
```

And add **-zmuldefs** to compiler.c.elf.libs.esp32c3 option:

```bash
compiler.c.elf.libs.esp32c3=-zmuldefs -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm ...
```

Save and restart Arduino IDE (if open).

- Change Micro SD Card pin

In the configs.h, change the MARAUDER_FLIPPER SD_CS definition to pin 4, since the VoyagerRF board is designed to work with CS pin in that GPIO:

```c
//...
//// SD DEFINITIONS
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
    #define SD_CS 4 //Change from 10 to 4 here
  #endif

  #ifdef ESP32_LDDB
    #define SD_CS 4
  #endif
//...
```

- Set the board (XIAO-ESP32-C3)

Go to the boards menu and select the proper model:

<p align="center">
  <img src="/images/flipper/xiao_esp32_option.png" width="70%"/>
</p>

Now hit compile and upload and all should run correctly. 


## Throubleshooting 

I have picked up some errors and solutions:

- If the compilation output errors with duplicates variables and headers:

Check that the changes on platform.txt are correct and that you have restarted Arduino IDE.

- I'm connecting the module to the Flipper, but commands are not displaying anything:

Go to Tools > USB CDC On Boot and set "Disable" so the conenction is set on the TX/RX ports instead of the USBC connector in the XIAO-ESP32-C3

- The SD card is not detected and no pcaps are being saved:

Check the configs.h and make sure that only #define MARAUDER_FLIPPER is uncommented and that you have changed the #define SD_CS 4 as previously described.

<br>

---

<br>

# Bundles and Boards

## VoyagerRF Complete Pack / 55€

Available in black or white. Antennas can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preintalled (v0.10.2)
- CC1101
- NRF24
- 3dBi Omni Antenna (2.4GHz)
- 3dBi Helical Antenna (433Mhz) 

<div display="grid" align="center">
  <img src="/images/flipper/models/white_3.jpg" width="40%"/>
  <img src="/images/flipper/models/black_3.jpg" width="40%"/>
</div>

<br>

## VoyagerRF mini Complete Pack / 50€

Available in black or white. Antennas can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preintalled (v0.10.2)
- CC1101
- NRF24
- 3dBi Omni Antenna (2.4GHz)
- 3dBi Helical Antenna (433Mhz)
- Flat WiFi antenna

<div display="grid" align="center">
  <img src="/images/flipper/voyagermini_white_0.jpg" width="40%"/>
  <img src="/images/flipper/voyagermini_black.jpg" width="40%"/>
</div>

<br>

## VoyagerRF / 40€

Available in black or white. Antenna can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preintalled (v0.10.2)
- 3dBi Omni Antenna (2.4GHz)

<div display="grid" align="center">
  <img src="/images/flipper/voyager_white_2.jpg" width="40%"/>
  <img src="/images/flipper/voyager_black_0.jpg" width="40%"/>
</div>

<br>

## VoyagerRF mini / 35€

Available in black or white. Antenna can be black or white, as you prefer.

Includes:

- VoyagerRF board with Marauder Firmware preintalled (v0.10.2)
- Flat WiFi antenna

<div display="grid" align="center">
  <img src="/images/flipper/voyagermini_white_0.jpg" width="40%"/>
  <img src="/images/flipper/voyagermini_black.jpg" width="40%"/>
</div>




<br>

---

<br>


# Tutorials

I'm planning to record several videos for tutorials using the ESP32. Stay tuned!