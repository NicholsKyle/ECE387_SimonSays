# ECE387_SimonSays

**Version 1.0.0**

Code and examples used to write a my version of the iconic simon says game

---

## Componets Used

1. [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3)
2. [Pmod MTDS](https://store.digilentinc.com/pmod-mtds-multi-touch-display-system/)  
   **DISCLAIMER:** linked sites may not have the cheapest prices on the market
3. FAT formatted MicroSD card
4. MicroSD card reader ** if your computer doesn't have one built in **
---

## Getting Started

 1. Pmod MTDS library 
   * Click [HERE](https://reference.digilentinc.com/reference/software/mtds/start) to download the libary and quick start guide for the MTDS.
   * Follow the instructions within the quick start quide to set up your SD card and to install the appropriate libraries onto your Arduino IDE

 2. Connecting the Pmod MTDS to the Arduino  
   * Follow the instructions listed [HERE](https://forum.digilentinc.com/topic/9302-how-to-connect-a-pmod-mtds-to-an-arduino-uno/)  
   * Note regarding the reset pin (see excerpt below). By default the reset pin is set to the Arduino pin 5. If you are able to   use this pin with your setup then you don't need to do anything more. However, if pin 5 is already in use you can change it   using the following code.  
```
Reset on the Pmod MTDS relates to the CS pin number +5. You can find this in the MtdsHal.cpp on line 233  

bool MtdsHalResetDisplay(int pinSel) {
    int pinRst = pinSel + 5;

You can change this to be any digital pin number on the Arduino. I would suggest choosing a digital pin from 5 - 7.
```
