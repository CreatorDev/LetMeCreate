![logo](https://static.creatordev.io/logo-md-s.svg)

# LetMeCreate library

## Build status

**Master**  
[![Build Status](https://travis-ci.org/francois-berder/LetMeCreate.svg?branch=master)](https://travis-ci.org/francois-berder/LetMeCreate)

**Dev**  
[![Build Status](https://travis-ci.org/francois-berder/LetMeCreate.svg?branch=dev)](https://travis-ci.org/francois-berder/LetMeCreate)

## Introduction

This library is a collection of small wrappers for some interfaces of the Ci40. It aims at making easier to develop on this platform. Also, it provides some wrappers for a few clicks. Notice that you cannot compile the library on Ci40 because cmake cannot run on it. There exists a Python binding of this library called [PyLetMeCreate](https://github.com/francois-berder/PyLetMeCreate).

Supported interfaces:  

|Interface|-|
|:------------| :-------------------|
|I²C|SPI|
|UART|LED's|
|Switch|GPIO (Mikrobus and Raspberry Pi interfaces)|
|PWM| ADC|

MikroClick board supported:  
 
|Interface|||
|:------------|:-------------------|:-------------------|
|7Seg|8x8R (Led Matrix)|Accel|
|ADC|Air quality|Alcohol|
|Alphanum|Bargraph|CO|
|Color|Color2|EVE|
|Fan|GYRO|IR distance|
|IR eclipse|Joystick|Light|
|LIN Hall|Motion|OLED|
|Opto|Proximity|Relay (partial support)|
|Relay2|Relay4 (partial support)|RTC|
|Thermo3|UNI Hall|Weather|

The Raspberry PI sense Hat is supported by the library, except the EEPROM because the pins are not connected on the I2C bus. The atmel chip is confusing the I2C driver of the Ci40 which makes it sometimes impossible to communicate with the hat. Inserting the hat after the board finished booting often solves the issue (assuming it does not cause a reset of the Ci40 because of a brown-out reset).

Examples are installed in /usr/bin/letmecreate_examples.
Tests are installed in /usr/bin/letmecreate_tests.

## Contributing to LetMeCreate

The master branch is the stable branch and the dev branch is merged to master just before doing a release. All pull requests must be done on dev branch.
Before creating a wrapper for a click board, make sure that it is not already supported in the dev branch. To avoid any overlap, contact me my email (fberder@outlook.fr).


About code style, I follow roughly the [linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle) (two notable exceptions: indentation is set to 4 spaces and no 80 characters limit per line). Keep the style of your wrapper consistent with the rest of the code. For instance, most functions return 0 if they succeed and a negative number if not.


Each wrapper must implement features provided by only one click board. Do not mix code related to different click boards in a wrapper. Add doxygen documentation to all of your functions and do not forget to add a doxygen header on top of your header file otherwise the documentation will not be generated by doxygen.


Keep examples very simple and avoid parsing arguments. Examples are there to show one or two features of a **single** click board. Do not use multiple click boards in an example. The idea behind writing examples is to show how easy the library makes it using click boards. Hence, the shorter the example is, the better it is.

## Integration in Openwrt

The library is already part of Imagination Technologies' OpenWrt.
To compile the library (only possible once you built Openwrt once):

```sh
$ make package/letmecreate/{clean,compile} -j1 V=s
```

### Installation steps

You can install LetMeCreate package on OpenWRT executing:

```sh
$ opkg install letmecreate
```

Each release has the ipk as an attachment. You can download the ipk, copy it to your Ci40 and install with opkg:

```sh
$ opkg install path-to-the-ipk
```

### Usage example
```c
/**
 * This example shows how to use the Thermo3 Click wrapper of the LetMeCreate
 * library.
 *
 * It reads the temperature from the sensor and exits.
 *
 * The Thermo3 Click must be inserted in Mikrobus 1 before running this program.
 */

#include <stdio.h>
#include <letmecreate/letmecreate.h>


int main(void)
{
    float temperature = 0.f;

    i2c_init();
    i2c_select_bus(MIKROBUS_1);

    thermo3_click_enable(0);
    thermo3_click_get_temperature(&temperature);
    printf("temperature: %.3f°C\n", temperature);
    thermo3_click_disable();

    i2c_release();

    return 0;
}
```
You can compile the C code example using **GCC**. Execute:
```sh
$ gcc thermo3.c -o thermo3 -lletmecreate_core -lletmecreate_click
$ ./thermo3
```
