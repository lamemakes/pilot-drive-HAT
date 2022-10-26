# The hardware of PILOT Drive

This repo contains all of the hardware for PILOT Drive - mainly STL, KiCad schematic + PCB files, and the code used for the ATTiny85 functionality.

**NOTE:** The 3D files & Build of Materials (BOM) are for my specific build. Your milage may vary as you modify this to fit your own needs. The HAT should remain the same over most builds.


## The PILOT Drive HAT

(**TLDR**) The features of the HAT are:
- Optocouplers to isolate & read the ACC +12v & Headlight 12v+ lines
- Via I2C the Raspberry Pi communicates with the ATTiny85 to set the shutdown time
- When there is no ACC +12v detected from the Pi or the ATTTiny, the Pi begins a safe shutdown and the ATTiny begins a countdown (Longer than it would take the Pi to shutdown) to kill the power to the BAT +12v. 
- The headlight detection optocoupler is connected to the RPi's GPIO pins
- An I2C connected RTC is included to allow the Pi to keep time without network connectivity.
- (Mostly) Conforms to the standards of the [Raspberry Pi HAT specifications](https://github.com/raspberrypi/hats), as it includes an ID EEPROM and follows most design specs, but more needs to be implemented

The design has been breadboard tested, but lacks actual PCB contruction. This will come very soon, and will ideally be demo'd at the Rochester Maker Faire.

_The Schematic of the HAT_
![image](https://user-images.githubusercontent.com/83597346/198093957-4e250405-35cb-4f44-a056-c505167d59f6.png)
 
_HAT PCB rendering (with *MOST* of the needed STEP files)_
![image](https://user-images.githubusercontent.com/83597346/198093405-7d286601-3e7c-4de6-9d54-55a776eca015.png)
![image](https://user-images.githubusercontent.com/83597346/198093722-e21cc907-e123-458c-897d-2e77b1140fd2.png)
![image](https://user-images.githubusercontent.com/83597346/198093887-36414013-3995-44c0-a988-c09d25f34e27.png)


 
 ## Build of Materials
 
 My build consists of:
 - [Raspberry Pi 4 B 4GB RAM](https://www.adafruit.com/product/4296)
 - [7" HDMI Touchscreen](https://www.amazon.com/Kuman-Capacitive-Display-Raspberry-SC7B)
 - [Powered USB Hub](https://www.amazon.com/gp/product/B083XTKV8V) - Might be integrated into the HAT later, not sure
 - [12v to 5v 10A buck converter](https://www.amazon.com/gp/product/B01M03288J) - Used for Pi, USB Hub & Touch Screen
 - [Male HDMI to mini HDMI flex cable](https://www.amazon.com/gp/product/B01367WEI4)
 - [USB Sound Card](https://www.amazon.com/gp/product/B00OJ5AV8I)
 - [HDMI to Pi Camera](https://www.amazon.com/gp/product/B06XDNBM63) - Allows for extension of the Pi Camera cable for backup camera
 - [Fisheye Pi Camera](https://www.amazon.com/gp/product/B076MPL9P1) - For backup camera

 
## STL Files

The 3D files were designed for mouting in the Honda Element head unit space, with all of the components of my BOM to make everything one discrete package.


## ATTINY85 Code

Please note the arduino code is functional but temporary, have my sights on being fully "vanilla" C soon.


## NOTES:
- PILOT Drive hardware is a **MAJOR** WIP! More will come soon.
- If building the HAT schematic, sections listed as "ID EEPROM" & "RPi 5v ZVD" are recommended by Raspberry Pi [Hat Standards](https://github.com/raspberrypi/hats/blob/master/designguide.md) but aren't required to make the circuit function.
- The schematic's +5v screw terminal in refers to the 12v to 5v buck converter (see BOM). 

_My "finished" build_
![PILOT_Drive](https://user-images.githubusercontent.com/83597346/194785200-4c0141d9-606c-4f25-9de7-094e111a38c3.jpg)

