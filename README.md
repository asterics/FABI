# Welcome to FABI

The FABI (Flexible Assistive Button Interface) allows control of a computer’s mouse cursor or
keyboard by just using momentary switches. This can be helpful for people who want to create 
user interactions with the press of a button - and it allows people who can't use standard computer input devices
to play games, surf the internet, communicate and much more.

![A FABI box with attached 3d-printed switch](https://github.com/asterics/FABI/blob/master/img/FABI4.png "FABI box with attached switch")


## How?

The FABI Interface can be actuated via dedicated buttons, momentary switches or self-made
electrical contacts. Additionally, a sip-puff sensor can be used to create user input.
FABI consists of a hardware module (a low-cost microcontroller which behaves
as a computer mouse and/or keyboard) and a graphical software application for configuration of
the desired functions. The FABI system can be attached to computers, tablets or smart phones via USB.
Additionally, a bluetooth-module is available for devices that do not have a USB port (some Android or iOS phones).


## Building the hardware

There are various options for build FABI-compatible switch interfaces: you can order parts and build our construction kit, or make a completely self-driven version.
Following documentation is available for these different approaches:
* **Construction manual**: manual for building FABI from the construction kit.\
See [Construction kit building guide](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/PCB-Version/FABIManual.pdf)
* **DIY Building Guide**: guide for creating FABI boxes or buttons apart from the standard 3D printed parts. 
See [DIY Building Guide](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/SelfmadeBox/FABI_Construction_SelfmadeBox.pdf)
See [English version of construction manual](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/3D-printedBox/en/FABI_ConstructionManual.pdf) 
or [German version of construction manual](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/3D-printedBox/de/FABI_Bauanleitung.pdf)

## Installing the Software

The FABI system consists of a firmware part which runs on the Arduino Pro Micro microcontroller, and a GUI with runs on a PC.
The fimware and can be built and uploaded (installed) to the controller using a recent version of the Arduino IDE.
Besides the Arduino framework, the following libraries are used und must be installed before the firmware can be compiled:

* [Light_WS2812 Neopixel library by cpldcpu](https://github.com/cpldcpu/light_ws2812) 
* [SSD1306 Oled/LCD library by Lekus2k](https://github.com/lexus2k/ssd1306)

Please follow the installation instructions in their github repositories.

The graphical configuration software (GUI) will come in two flavours: Currently, a [Windows-based GUI](https://github.com/asterics/FABI/tree/master/FabiGUI_C%23) is available (written in C#) which is also desribed in the User manual (see below).
We are currently working on a **cross-platform GUI which runs in the browser** and adds improved User Experience - so stay tuned !!

## Configuration and User manual

The FABI system does not ony offer a single mapping of button to function - it allow to store multiple configuration settings and change between them "on-the-fly".
You can find information how to configure the individual button functions and system options in the user manual:
* [English version of user manual](https://github.com/asterics/FABI/blob/master/Documentation/UserManual/FABI_GUI/en/FABI_UserManual.pdf) 
* [German version of user manual](https://github.com/asterics/FABI/blob/master/Documentation/UserManual/FABI_GUI/de/FABI_AnwendungsAnleitung.pdf)


## Example applications

The folder _case designs_ contains different acrylic glass designs for example applications. In the folder _Settings_ you can find different demo configurations which can be loaded via the GUI.
Some of the examples are also located in the [wiki](https://github.com/asterics/FABI/wiki).

![An opened FABI box, where you can see the Teensy microcontroller board and the 6 jackplugs](https://github.com/asterics/FABI/blob/master/img/7.jpg "FABI box with jackplugs")
![A wooden piece with 6 buttons, arranged between, left and right of your hands. Enabling faster working with shortcuts on each button](https://github.com/asterics/FABI/blob/master/img/2.jpg "RapidCoding prototype, powered by FABI")
![A FABI box, where 3 different external buttons are connected](https://github.com/asterics/FABI/blob/master/img/8.jpg "FABI box with external buttons")

## About / Credits

FABI is an open source Assistive Technology module developed by the AsTeRICS Foundation in cooperation with the UAS Technikum Wien.
(see https://www.asterics-foundation.org).

All software and hardware documents are open source and we took care to use the most
affordable components available on the market to establish these functionalities – making FABI the
most reasonably priced flexible assistive button interface we know !!
