# ECE387_SimonSays

**Version 1.2.0**

This repositories includes detailed, step by step instructions on how to connect and interface a Pmod MTDS with the Arduino Uno. References to examples and to my own code are provided along the way.

---

## Pre-requisites

1. [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3)
2. [Arduino IDE](https://www.arduino.cc/en/Main/Software)
3. [Pmod MTDS](https://store.digilentinc.com/pmod-mtds-multi-touch-display-system/)
4. FAT formatted MicroSD card
5. MicroSD card reader (if your computer doesn't have one built in)
6. Piezo speaker
7. 100 ohm resistor  
**DISCLAIMER:** linked sites may not have the cheapest prices on the market

---

## Getting Started

1. Download and follow the MTDS quick start guide
   * This download will also include examples refrenced later.
   * Click [HERE](https://reference.digilentinc.com/reference/software/mtds/start) to download the quick start guide for the MTDS.
   * Follow the instructions within the quick start quide to set up your SD card and to install the appropriate libraries onto the Arduino IDE.

2. Connecting the Pmod MTDS to your Arduino Uno
   * Follow the instructions listed [HERE](https://forum.digilentinc.com/topic/9302-how-to-connect-a-pmod-mtds-to-an-arduino-uno/).
   * **Note** regarding the reset pin (see excerpt below). By default the reset pin is set to the Arduino pin 5. If you are able to use this pin with your setup then you don't need to do anything more. However, if pin 5 is already in use you can change it using the following code: 
```
Reset on the Pmod MTDS relates to the CS pin number +5. You can find this in the MtdsHal.cpp on line 233  

bool MtdsHalResetDisplay(int pinSel) {
    int pinRst = pinSel + 5;

You can change this to be any digital pin number on the Arduino. I would suggest choosing a digital pin from 5 - 7.
```
3. Piezo speaker
   * If you need a quick refresher on how to connect/ use a Piezo speaker, [HERE](https://programmingelectronics.com/an-easy-way-to-make-noise-with-arduino-using-tone/) is a link to a simple example.
   * I have my speaker attached to pin 2 of the Arduino.
   
4. Getting familiar with the MTDS
   * **WARNING** A few of the examples had compiler errors when I tried to run them. I skipped these examples and luckily I did not need them to understand how to use the MTDS.
   * If you're planing on using the MTDS outside of this application, it would be in your best interest, at this point, to go through the 'MtdsDemo' and 'MydispDemo' examples. These examples are in the 'MTDS' zip file which contained the quick start guide. **NOTE** if you are unable to open the examples (ex. "MyDispDemo3.pde"), it is because your computer does not recognize the '.pde' file type. **SOLUTION** right click on the example you are trying to open. Go to 'open with' and select your prefered text editor app. Now copy and paste the code into an Arduino IDE sketch.
   * If you are here to follow this application then the examples that I found to be most helpful can be found [HERE](https://github.com/NicholsKyle/ECE387_SimonSays/tree/master/Example_Code). 

5. Running my code
   * Once you have the basics of the Pmod MTDS down you will be able to understand [my code](https://github.com/NicholsKyle/ECE387_SimonSays/tree/master/Added_Value/SimonSays_Game). Most of the comments within my code are purely logic baised. **Do not** expect to learn the functionality of the Pmod MTDS from my code alone.
   * Before running my code read the [RULES](https://github.com/NicholsKyle/ECE387_SimonSays/blob/master/Added_Value/Rules:Instructions_SimonSays.pdf).

---

## Known Issues/ Notes

1. The MTDS:
   * Touching ones nail to the screen will not register as a touch. 
2. The system as a whole:
   * Touch messages received outside of the 'getUserCode' method will be saved and read later. Meaning that if you touch the screen when you arent supppose to, the system saves that message and reads it later - even if you have not touched the screen since the error touch.
   * That being said: the user might use this flaw to cheat. The user is able to touch the screen while the computer is displaying its code. If the user is fast enough, it is possible to copy the compluter as it displays its code rather than waiting for the computer to finish.




