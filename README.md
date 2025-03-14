# PicoRAMCart
Emulate Raster/Bob!k/C.P.U. RAM Cartridge with A8PicoCart

## What?

This is an alternative firmware for the [A8PicoCart](https://github.com/robinhedwards/A8PicoCart)
that emulates a RAM cartridge with XEGS bankswitching.
Original hardware project is [here](http://raster.infos.cz/atari/hw/ramcart/ramcart.htm).

## How?

* You need to connect two switches to the two spare GPIO pins (28 and 29) for a functional cartridge.

![](PicoRAMCart.png)

* Download the new firmware from the [releases page](https://github.com/ivop/PicoRAMCart/releases) and flash it to your A8PicoCart.
* Insert the cartridge in your Atari with both switches in the OFF position. On the image above that is to the left.
* Switch on your computer and boot one of the two supplied ATR images.
* Run ```RFF21```.
* Press ```RETURN``` for a directory listing.
* Select one of the ROM files with the cursor keys and press ```RETURN``` again.
* Follow on-screen instructions:
  * Turn both switches ON.
  * Press ```RETURN```.
  * Wait for the file to be copied to the cartridge.
  * Turn OFF write enable switch.
  * Press ```RESET```.

The two ROMs on the original software disk ask you to turn OFF the cartridge after it is booted. This is normal behaviour. The program has been copied to normal RAM.
The second disk contains two programs that actually run from ROM and need the cartridge enable switch to stay in the ON position.

Here's a YouTube short demonstrating the procedure: https://www.youtube.com/shorts/3Dks8j9dF18

## Bootsel without opening the cartridge?

The first time you flash this new firmware, you'll need to open the cartridge and hold the bootsel button while connecting it to your PC.
After that it can be closed again. If you want to reflash the firmware (newer version, your own experiments), there is no need to reopen the cartridge anymore.
When connected to the PC instead of the Atari, it drops to a serial debug console.
If you open the serial device at 1200 baud, it will automatically reboot in bootsel mode.
For example, on Linux you can run ```stty -F /dev/ttyACM0 1200``` or ```minicom -D /dev/ttyACM0 -b 1200```.
On Windows, using _putty_ and setting the serial baudrate to 1200 should work.

## Building the firmware?

To build the firmware from source you need to have a functional [Pi Pico build environment](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).
You can set the PICO_SDK_PATH and PICO_EXTRAS_PATH environment variables to point to the right directories.
If they are not set, cmake will download and build them itself. The same will happen if _picotool_ is not in your $PATH.

```
git clone https://github.com/ivop/PicoRAMCart
cd PicoRAMCart
mkdir build
cd build
cmake ..
make
```

The firmware file is named PicoRAMCart.uf2.

## Who?

PicoRAMCart firmware is Copyright © 2025 by Ivo van Poorten and is licensed under the terms of the BSD 2-Clause license.  
A8PicoCart hardware is Copyright © 2023 by Robin Edwards.  
Original RAM Cartridge design by Radek Štěrba (Raster) and Robert Petružela (Bob!k), 2005.
