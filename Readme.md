# LetMeCreate library

This library is a collection of small wrappers for some interfaces of the Ci-40. It aims at making easier to develop on this platform. Also, it provides some wrappers for a few clicks.

Interface supported:
  - I²C
  - SPI
  - UART
  - LED's
  - Switch
  - GPIO (Mikrobus and Raspberry Pi interfaces)
  - PWM
  - ADC

MikroClick board supported:
  - Thermo3
  - Proximity
  - Motion
  - Relay2
  - Joystick
  - Air quality
  - Accel (SPI only)
  - 8x8R (Led Matrix)

Examples are installed in /usr/bin/letmecreate_examples.
Tests are installed in /usr/bin/letmecreate_tests.

## Integration in Openwrt

To add new packages, Openwrt relies on feeds: a collection of packages.

### Installation steps

Clone the library and openwrt somewhere on you computer:

```sh
$ mkdir ci-40
$ cd ci-40
$ git clone https://github.com/CreatorDev/openwrt.git
$ mkdir -p custom/letmecreate
$ cd custom/letmecreate
$ git clone https://github.com/francois-berder/LetMeCreate.git
```

Copy the Makefile to the right location:
```sh
$ cp LetMeCreate/miscellaneous/Makefile.devel Makefile
```

To register the feed in openwrt, go back in openwrt folder and open feeds.conf.default.
Add this line:
```
src-link custom /change/this/path/to/the/location/of/ci-40/custom/directory/
```

Update and install all feeds:
```sh
$ ./scripts/feeds update -a
$ ./scripts/feeds install -a
```
In make menuconfig, select Libraries, you should see an entry for letmecreate library:

![Libraries menu](/miscellaneous/libraries_menu.png)

Select the letmecreate library in make menuconfig, and compile Openwrt:

```sh
$ make -j1 V=s
```
In the image (a tarball in bin/pistachio), you should see the examples in /usr/bin/letmecreate_examples.

To compile only the library (only possible once you built Openwrt once):

```sh
$ make package/letmecreate/{clean,compile} -j1 V=s
```
