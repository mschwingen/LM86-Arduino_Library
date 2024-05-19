LM86 Arduino Library
====================

The LM86 is an 11-bit digital temperature sensor with
a 2-wire System Management Bus (SMBus) serial
interface. The LM86 accurately measures its own
temperature as well as the temperature of an external
device, such as processor thermal diode or diode
connected transistor such as the 2N3904

This library allows the user to read the local and remote temperature from a
LM86 chip. Periodic conversion and oneshot mode are supported.

Examples are included to get you started.

Repository Contents
-------------------

* **/examples** - Example sketch for the library (.ino). Run these from the Arduino IDE.
* **/src** - Source files for the library (.cpp, .h).
* **keywords.txt** - Keywords from this library that will be highlighted in the Arduino IDE.
* **library.properties** - General library properties for the Arduino package manager.

Documentation
--------------
The library enables the following functions:

### General functions

    bool begin(TwoWire &wirePort = Wire, ConvRate rate = ConvRate::CR_1Hz);

Initialize library and set conversion rate.
Please call begin() sometime after initializing the I2C interface with Wire.begin().

### Get Temperature

    bool oneshot(void);

Trigger one conversion and wait until it is done.

    float getLocalTemperature(void);
    float getRemoteTemperature(void);

Read temperature values. Returns -1000.0 in case of error.

License Information
-------------------

This product is _**open source**_!

Please review the LICENSE.md file for license information.

Distributed as-is; no warranty is given.
