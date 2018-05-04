# ECE387_SimonSays

**Version 1.2.0**

This repositories includes detailed, step by step instructions on how to connect and interface a Pmod MTDS with the Arduino Uno. References to examples and to my own code are provided along the way.

---

## Getting Started

1. Download and follow the MTDS quick start guide
   * This download will also include examples refrenced later.
   * Click [HERE](https://reference.digilentinc.com/reference/software/mtds/start) to download the quick start guide for the MTDS.
   * Follow the instructions within the quick start quide to set up your SD card and to install the appropriate libraries onto the Arduino IDE.

2. Connecting the Pmod MTDS to your Arduino Uno
   * Follow the instructions listed [HERE](https://forum.digilentinc.com/topic/9302-how-to-connect-a-pmod-mtds-to-an-arduino-uno/).
   * **Note** regarding the reset pin (see excerpt below). By default the reset pin is set to the Arduino pin 5. If this pin is available for use in your design, then you do not need to worry about setting a new pin. However, if you are already using pin 5, you can set the reset to a new pin by using the following code: 

```
Reset on the Pmod MTDS relates to the CS pin number +5. You can find this in the MtdsHal.cpp on line 233  

bool MtdsHalResetDisplay(int pinSel) {
    int pinRst = pinSel + 5;

You can change this to be any digital pin number on the Arduino. I would suggest choosing a digital pin from 5 - 7.
```
   
3. Getting familiar with the MTDS
   * **WARNING** A few of the examples had compiler errors when I tried to run them. I skipped these examples and luckily I did not need them to understand how to use the MTDS.
   * See my [diagram](https://github.com/NicholsKyle/ECE387_SimonSays/blob/master/Media/Z_Z.png) for orientation and (0,0) position.
   * If you're planing on using the MTDS outside of this application, it would be in your best interest, at this point, to go through the 'MtdsDemo' and 'MydispDemo' examples. These examples are in the 'MTDS' zip file which contained the quick start guide. **NOTE** if you are unable to open the examples (ex. "MyDispDemo3.pde"), it is because your computer does not recognize the '.pde' file type. **SOLUTION** right click on the example you are trying to open. Go to 'open with' and select your prefered text editor app. Now copy and paste the code into an Arduino IDE sketch.
   * If you are here to follow this application then the examples that I found to be most helpful can be found [HERE](https://github.com/NicholsKyle/ECE387_SimonSays/tree/master/Example_Code/Most_Helpful). 

4. STOP! Before running my code, read the [GAMEPLAY/ RULES](https://github.com/NicholsKyle/ECE387_SimonSays/blob/master/Added_Value/Rules:Instructions_SimonSays.pdf) document

5. Running my code
   * Once you have the basics of the Pmod MTDS down you will be able to understand [my code](https://github.com/NicholsKyle/ECE387_SimonSays/blob/master/Added_Value/SimonSays_Game/SimonSays_Game.ino). Most of the comments within my code are purely logic baised. **Do not** expect to learn the functionality of the Pmod MTDS from my code alone.
