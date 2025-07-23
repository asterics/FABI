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
as a computer mouse and/or keyboard) and a config manager software for configuration of
the desired functions. The FABI system can be attached to computers, tablets or smart phones via USB.
Additionally, a bluetooth-module is available for devices that do not have a USB port (some Android or iOS phones).

## Folder structure of this repository
Essential information can be found in the following folders
* _Case Designs:_ different 3d-printable / lasercut designs for enclosures
* _Documentation:_ all user and construction manuals
* _FabiGUI_C#:_ the (outdated) graphical user interface (for Windows)
* _hardware:_ the KiCad design files and models for schematics and PCB layout
* _Settings:_ demo configurations which can be loaded into the FABI device via the GUI

The source of the current web based config manager can be found in another repository: [source of web based config manager](https://github.com/asterics/Addon-Bluetooth-WebGUI/tree/main/webgui)

## Configuration Manager and User manual

The FABI system does not ony offer fixed mappings of buttons to functions - it allows storing multiple configuration settings and changing them "on-the-fly". For configuration the [web-based config manager](https://fabi.asterics.eu/) should be used.
You can find information how to configure the individual button functions and system options in the user manual:

* [English user manual for configuration manager](https://github.com/asterics/FABI/blob/master/Documentation/UserManual/Markdown/Fabi%20User%20Manual.md) 
* [German user manual for configuration manager](https://github.com/asterics/FABI/blob/master/Documentation/UserManual/Markdown/Fabi%20Anwendungsanleitung.md)



## Building the hardware

There are various options for building FABI-compatible switch interfaces: you can order parts and build our construction kit, or make a completely self-driven version.
The most affordable version consists of just a microcontroller and attached momentary switches (material cost: a few bucks ...). The construction kit offers a PCB, LC-Display, 
Neopixel color led for indication of active configuration and optional modules (pressure sensor for sip/puff use, Bluetooth module for wireless access to phones/tablets etc.)
Following documentation is available for these different approaches:
* **Construction kit / PCB Version**: for building the FABI PCB Version from the construction kit.\
See [Construction kit building guide](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/PCB-Version/FABIManual.pdf)
* **DIY Building Guide - "intermediate"**: guide for creating a FABI device with a bare microcontroller and the provided 3d-printed enclosures.\
See [English version of construction manual](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/3D-printedBox/en/FABI_ConstructionManual.pdf) 
or [German version of construction manual](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/3D-printedBox/de/FABI_Bauanleitung.pdf)
* **DIY Building Guide - "all-selfmade"**: guide for creating a FABI device with a bare microcontroller and without the provided 3d-printed enclosures.\
See [DIY Building Guide](https://github.com/asterics/FABI/blob/master/Documentation/ConstructionManual/SelfmadeBox/FABI_Construction_SelfmadeBox.pdf)

## Building and Installing the Firmware

The FABI system consists of a firmware part which runs on a Raspberry Pi Pico microcontroller.
Since Version v3.7, the firmware development for the FABI (and our other alternative input devices) happens in a dedicated repository, see [FabiWare](https://github.com/asterics/FabiWare).
All firmware versions until v2.8 can be found in this repository by browsing the release section or by using the desired tags.
For information about the older firmware for the Arduino Pro Micro microcontroller, and the optional [Bluetooth Module](https://github.com/asterics/esp32_mouse_keyboard) which was used in the legacy hardware version, see https://github.com/asterics/FABI/wiki
Please __note__ the COM-Port installation instructions for older Windows systems (Win7/8) [here](https://github.com/raspberrypi/pico-feedback/issues/118).


![FABI PCB version, equipped with Bluetooth Module](https://github.com/asterics/FABI/blob/master/img/pcb_open.JPG "FABI PCB version with equipped bluetooth module")


## Example setups and applications

![An opened FABI box, where you can see the Teensy microcontroller board and the 6 jackplugs](https://github.com/asterics/FABI/blob/master/img/7.jpg "FABI box with jackplugs")
![A wooden piece with 6 buttons, arranged between, left and right of your hands. Enabling faster working with shortcuts on each button](https://github.com/asterics/FABI/blob/master/img/2.jpg "RapidCoding prototype, powered by FABI")
![A FABI box, where 3 different external buttons are connected](https://github.com/asterics/FABI/blob/master/img/8.jpg "FABI box with external buttons")


# Links and Credits

Most of the work for FABI has been accomplished at the UAS Technikum Wien in course of the R&D-projects *ToRaDes* (MA23 project 18-04) and *WBT* (MA23 project 26-02), which have been supported by the [City of Vienna](https://www.wien.gv.at/kontakte/ma23/index.html),
see: [ToRaDes Project Information](https://embsys.technikum-wien.at/projects/torades/index.php), [Webpage WBT project](https://wbt.wien).

Have a look at the [AsTeRICS Foundation homepage](https://www.asterics-foundation.org) for applications and our other Open Source projects:

* [The FLipMouse controller](https://github.com/asterics/FLipMouse) - an open source alternative input device for controlling computers and mobile devices with minimal muscle movement.
* [The FLipPad controller](https://github.com/asterics/FLipMouse) - a flexible touchpad for controlling computers and mobile devices with minimal muscle movement.
* [Asterics Grid Open Source AAC](https://grid.asterics.eu) - an open source, cross plattform communicator / talker for Augmented and Alternative Communication (AAC).
* [The AsTeRICS framework](https://github.com/asterics/AsTeRICS) - provides high flexibility for building Assistive Technology solutions. 


# Support us
Please support the development of Open Source Assistive Technology projects by donating to the AsTeRICS Foundation:

<div>
<a title="Donate with PayPal" href="https://www.paypal.com/donate/?hosted_button_id=38AJJNS427MJ2" target="_blank" style="margin-right:3em">
<img src="https://github.com/asterics/AsTeRICS-Grid/raw/master/app/img/donate-paypal.png" width=300/></a>
<span>&nbsp;&nbsp;&nbsp;</span>
<a title="Donate at opencollective.com" href="https://opencollective.com/asterics-foundation" target="_blank">
<img src="https://github.com/asterics/AsTeRICS-Grid/raw/master/app/img/donate-open-collective.png" width=300/></a>
</div>


