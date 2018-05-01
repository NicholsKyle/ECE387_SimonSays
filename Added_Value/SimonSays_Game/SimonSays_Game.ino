#include <mtds.h>
#include <string.h>
#include <stdint.h>
#include <MyDisp.h>

// =================== for buttons
RCT rctDisp;
uint32_t msDisp;

// =================== for touch
HDS   hdsDisp;
HDS   hdsFng1;
HBMP  hbmpFng1;
HDS   hdsFng2;
HBMP  hbmpFng2;

// =================== varrables for game
const int maxLevel = 150;
int master[maxLevel];
int user[maxLevel];
int level = 1;
int userCount = 0;
int piezoPin = 2;
unsigned long time_Now;  
bool loser = false;
char holdScore[150]; 
char holdLastGameScore[150]; 
int lastGameScore;

// =================== calls for buttons
void buttonB1();
void buttonB1Pressed();
void buttonB1Released();

void buttonB2();
void buttonB2Pressed();
void buttonB2Released();

void buttonB3();
void buttonB3Pressed();
void buttonB3Released();

void buttonB4();
void buttonB4Pressed();
void buttonB4Released();

// ================ calls for game
void resetGame();
bool getTwoTouch();
void drawWelcomeScreen();
void coverWelcomeScreen();
void drawButtons();
void drawScore();
void getMasterCode();
void displayMasterCode();
void clearMasterCode();
bool getUserCode();
void clearUserCode();
bool testMatch();
void gameLost();

// for testing olny
void printMaster();
void printUser();

//============================================================================= Setup
void setup() {
  
  bool fStat;
  Serial.begin(9600);
  // need random seed to make true random numbers later on
  randomSeed(analogRead(0));
  msDisp = millis() + 100000;

  // define the edges of the screen
  rctDisp.xcoLeft = 0;
  rctDisp.ycoTop = 0;
  rctDisp.xcoRight = 239;
  rctDisp.ycoBottom = 319;
  
  // it is possible that we are trying to initialize the interface to the
  // display board before it has finished its power on initialization. If so,
  // then the first call to mtds.begin() will fail. We run in a loop trying
  // to initialize until it succeeds. This usually doesn't take more than one
  // or two iterations of the loop before it succeeds.
  while(true) {
    fStat = mtds.begin(pinMtdsSelStd, frqMtdsSpiDefault);
    if (fStat) {
      Serial.println("mtds.begin() succeeded");
      break;
    }
    else {
      Serial.println("mtds.begin() failed");
      delay(1000);
    }
  }
}
//============================================================================= Loop
void loop() {
  // clear/ reset anything that maybe left over from the last game
  resetGame();
  
  // wait for two fingers to touch the screen to start the game
  while(getTwoTouch()){ 
    Serial.println("waiting for 2 touch");
    }
  
  // start game loop
  // loop through the game until the current level reaches the max level
  while(level <= maxLevel){

    // hold the current (level - 1) this is how many levels the user has completed
    lastGameScore = level - 1;
    itoa (lastGameScore, holdLastGameScore, 10);
             
    // update the buttons
    drawButtons();
    drawScore();

    // make sure the user code is cleared before every round
    clearUserCode();
    
    // delay to let the user get ready
    delay(800);
    
    // each level add a number between 1-4 to the master
    getMasterCode();

    // display the master code on the screen
    displayMasterCode();

    // set a new time_Now after the computer has finished displaying its code
    time_Now = millis();

    // keep checking for user input until i is equal 
    // to the level that we're currently on
    int i = 0;
    userCount = 0;
    while(i < level) {
      // if we get input add one to i and userCount
      while(getUserCode()) {
        // check to see if 1000ms has passed since the last touch message
        // if true, the user loses
        if(millis() >= (time_Now + 1000)){
          // break out of the game
          loser = true;
          level = maxLevel + 1;
          break;
        }
      Serial.println("waiting inside getUserCode");
      }
      i++;
      userCount++;
    }
    
    // if loser = false, keep playing the game
    // else break out of the game
    if(!loser) {   
        // make sure we keep updating the buttons
        drawButtons();
    
       // if user code doesnt match master code, call gameLost()
       if(testMatch()) {
         // gameLost sets the level = maxLevel + 1 to jump out of the game loop
         gameLost();
      }
      
    // round passed, move to next level
    level++;
    }
  } // end of game loop

  // if the game timed out display gameLost()
  if(loser){
    gameLost();
  }

}// end loop - game resets
//============================================================================= Methods
void resetGame() {
  level = 1;
  mtds.ClearDisplay(0); 
  clearMasterCode();
  clearUserCode();
}//=============================================
bool getTwoTouch() {
  // draw the wwlcome screen while were wait on a 2touch message
  drawWelcomeScreen();
  
  // If there is a message in the message queue. Read it into the local
  // MTCH structure so that we can process it
  MTCH mtch;
  mtds.GetMsg((MEVT *)&mtch);

  // return false if we get a finger 2 down message: this means that finger 2 
  // was previously not on the display and has just touched the display
  switch (mtch.msg) {   
    case msgFinger2Down:
    coverWelcomeScreen();
    return false;
  }
  return true;
}
//=============================================//=============================================//=============================================
//=============================================//=============================================//=============================================
//=============================================//=============================================//=============================================
//=============================================//=============================================//=============================================
void drawWelcomeScreen() {
  PNT pntTxt;
 
  char message[ ] = "Welcome to SIMON SAYS!";
  char message2[ ] = "Touch two fingers to the";
  char message3[ ] = "screen to start a new game.";
  char message4[ ] = "Last game score was = ";
  

  // display the last game score
  itoa (lastGameScore, holdLastGameScore, 10);
  
  // Get the DS for drawing on the display and initialize it.
  hdsDisp = mtds.GetDisplayDs();
  
  mtds.SetFgColor(hdsDisp, clrWhite);
  mtds.SetBgColor(hdsDisp, clrBlack);
  mtds.SetDrwRop(hdsDisp, drwCopyPen);
  mtds.SetFont(hdsDisp, hfntConsole);
  
  // print the messages
  mtds.TextOut(hdsDisp, 2, 2, strlen(message), message);
  mtds.TextOut(hdsDisp, 2, 22, strlen(message2), message2);
  mtds.TextOut(hdsDisp, 2, 32, strlen(message3), message3);
  mtds.TextOut(hdsDisp, 2, 52, strlen(message4), message4);
  mtds.TextOut(hdsDisp, 180, 52, strlen(holdLastGameScore), holdLastGameScore);

  // always release the mtds
  mtds.ReleaseDs(hdsDisp);
  
}
//=============================================
void coverWelcomeScreen() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 0;
  ycoT = 0;
  xcoR = 239;
  ycoB = 158;
  
  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlack, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================
void drawButtons() {
  buttonB1();
  buttonB2();
  buttonB3();
  buttonB4();
  return;
}
//=============================================
void drawScore() {
  int16_t  xco;
  int16_t yco;
  int16_t dyco;
  PNT pntTxt;
 
  itoa (level, holdScore, 10);
  
  // Get the DS for drawing on the display and initialize it.
  hdsDisp = mtds.GetDisplayDs();
  
  mtds.SetFgColor(hdsDisp, clrWhite);
  mtds.SetBgColor(hdsDisp, clrBlack);
  mtds.SetDrwRop(hdsDisp, drwCopyPen);
  mtds.SetFont(hdsDisp, hfntConsole);

  // We'll use a two pixel margin at the top and left of the screen.
  xco = 2;
  yco = 2;
  
  // print the string
  mtds.TextOut(hdsDisp, xco, yco, strlen(holdScore), holdScore);
  
  mtds.ReleaseDs(hdsDisp);
}
//=============================================
void getMasterCode() {
  // create a random number between 1 and 4
  // add the random number to the master array
  // at pos level-1
  master[level-1] = (random(10) % 4+1);
  return;
}
//=============================================
void displayMasterCode() {
  
// displays the code for the user to follow.
// the diffuculty represents the user input at 
// the beginning of the game, see getDifficulty

for(int i = 0; i < level; i++){
  switch (master[i]) {
    
    case 0:
    break;
    
    case 1:
    buttonB1Pressed();
    tone(piezoPin, 4500, 300);
    delay(150);
    buttonB1Released();
    drawButtons();
    break;

    case 2:
    buttonB2Pressed();
    tone(piezoPin, 4000, 300);
    delay(150);
    buttonB2Released();
    drawButtons();
    break;

    case 3:
    buttonB3Pressed();
    tone(piezoPin, 3000, 300);
    delay(150);
    buttonB3Released();
    drawButtons();
    break;

    case 4:
    buttonB4Pressed();
    tone(piezoPin, 2000, 300);
    delay(150);
    buttonB4Released();
    drawButtons();
    break;
  }
  delay(150);
}
return;
}
//=============================================
void clearMasterCode() {
  // sets the master array to all 0's
  for(int i = 0; i < sizeof(master); i++) {
    master[i] = 0;
  }
  return;
}
//=============================================
bool getUserCode() {
  // update the buttons while we wait for input
  drawButtons();
  
  // If there is a message in the message queue. Read it into the local
  // MTCH structure so that we can process it
    MTCH mtch;
    mtds.GetMsg((MEVT *)&mtch);
    
    // (xco,yco) gives the position of the touch event. For a finger down
    // message, it gives the pixel position of where the finger came down
    // =============================================
    // | See MtdsDemo1-MtdsTest3 for documentation |
    // | on how to draw objects on the MTDS        |
    // =============================================
    
    switch (mtch.msg) {   
      
      // finger 1 down message: this means that finger 1 was not previously
      // on the display and has just touched the display. Check to see which 
      // quadrant it touched
      case msgFinger1Down: 
      //================ B1
      if((mtch.xco > 0) && (mtch.xco < 118) && (mtch.yco > 0) && (mtch.yco < 158)) {
          time_Now = millis();
          tone(piezoPin, 4500, 300);
          buttonB1Pressed();
          delay(100);
          buttonB1Released();
          user[userCount] = 1;
          return false;
      }
      //================ B2
      if((mtch.xco > 0) && (mtch.xco < 118) && (mtch.yco > 158) && (mtch.yco < 319)) {
          time_Now = millis();
          tone(piezoPin, 4000, 300);
          buttonB2Pressed();
          delay(100);
          buttonB2Released();
          user[userCount] = 2;
          return false;
      }
      //================ B3
      if((mtch.xco > 118) && (mtch.xco < 239) && (mtch.yco > 158) && (mtch.yco < 319)) {
          time_Now = millis();       
          tone(piezoPin, 3000, 300);
          buttonB3Pressed();
          delay(100);
          buttonB3Released();
          user[userCount] = 3;
          return false;
      }
      //================ B4
      if((mtch.xco > 118) && (mtch.xco < 239) && (mtch.yco > 0) && (mtch.yco < 158)) {
          time_Now = millis();         
          tone(piezoPin, 2000, 300);
          buttonB4Pressed();
          delay(100);
          buttonB4Released();
          user[userCount] = 4;
          return false;
      }
      break;
    }
   return true;
}
//=============================================
void clearUserCode() {
  // clear the user code and set the userCount
  // to zero
  userCount = 0;
  if(sizeof(user) == 0) {
    user[0] = 0;
    return;
  }
  for(int j = 0; j < sizeof(user)/2; j++) {
    user[j] = 0;
  }
return;
}
//=============================================
bool testMatch() {
  // test to see if the master code is equal to the user code
  for(int i = 0; i <= level-1 ; i++) {
    if((master[i] != user[i])) {
      return true;
    }
  }
  return false;
}
//=============================================
void gameLost() {
  // user has lost the game
  // clear the display
  mtds.ClearDisplay(0);

  // set the level to 100
  level = maxLevel + 1;

  // declare a new object to draw
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();
  
  // set corridanates for an ellipse
  xcoL = 0;
  ycoT = 0;
  xcoR = 239;
  ycoB = 319;

  // draw the ellipse with color red
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrRed, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
  
  // play game lost tone
  tone(piezoPin, 1500, 300); 
  delay(200);
  tone(piezoPin, 1700, 200);
  delay(200);
  tone(piezoPin, 2000, 800);
  delay(200);
  tone(piezoPin, 2000, 200);
  delay(500);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The rest of the code is for drawing |
// buttons - each buttons code is      |
// identical except for the kind of    | 
// shape drawn and the color.          |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//=============================================================================
//      B1 -- RED
//=============================================================================
void buttonB1() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();
  
  // set corridanates for an ellipse
  xcoL = 0;
  ycoT = 10;
  xcoR = 118;
  ycoB = 158;

  // draw the ellipse
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrRed, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B1 PRESSED -- RED
//=============================================================================
void buttonB1Pressed() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();
  
  // set corridanates for a rectangle
  xcoL = 0;
  ycoT = 10;
  xcoR = 118;
  ycoB = 158;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrRed, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B1 RELEASED -- BLACK
//=============================================================================
void buttonB1Released() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();
  
  // set corridanates for a rectangle
  xcoL = 0;
  ycoT = 10;
  xcoR = 118;
  ycoB = 158;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlack, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  //mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B2 -- GREEN
//=============================================================================
void buttonB2() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for an ellipse
  xcoL = 0;
  ycoT = 158;
  xcoR = 118;
  ycoB = 309;

  // draw the ellipse
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrGreen, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B2 PRESSED -- GREEN
//=============================================================================
void buttonB2Pressed() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 0;
  ycoT = 158;
  xcoR = 118;
  ycoB = 309;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrGreen, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B2 RELEASED -- BLACK
//=============================================================================
void buttonB2Released() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 0;
  ycoT = 158;
  xcoR = 118;
  ycoB = 309;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlack, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B3 -- YELLOW
//=============================================================================
void buttonB3() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for an ellipse
  xcoL = 118;
  ycoT = 158;
  xcoR = 239;
  ycoB = 309;

  // draw the ellipse
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrYellow, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B3 PRESSED -- YELLOW
//=============================================================================
void buttonB3Pressed() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 118;
  ycoT = 158;
  xcoR = 239;
  ycoB = 309;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrYellow, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B3 RELEASED -- BLACK
//=============================================================================
void buttonB3Released() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 118;
  ycoT = 158;
  xcoR = 239;
  ycoB = 309;

  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlack, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B4 -- BLUE
//=============================================================================
void buttonB4() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for an ellipse
  xcoL = 118;
  ycoT = 10;
  xcoR = 239;
  ycoB = 158;
  
  // draw the ellipse
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlue, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Ellipse(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B4 PRESSED -- BLUE
//=============================================================================
void buttonB4Pressed() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 118;
  ycoT = 10;
  xcoR = 239;
  ycoB = 158;
  
  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlue, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//=============================================================================
//      B4 RELEASED -- BLACK
//=============================================================================
void buttonB4Released() {
  HDS   hdsDisp;
  HBR   hbrDiagCross;
  int   xcoL;
  int   ycoT;
  int   xcoR;
  int   ycoB;

  // getting the display DS to draw on
  hdsDisp = mtds.GetDisplayDs();

  // set corridanates for a rectangle
  xcoL = 118;
  ycoT = 10;
  xcoR = 239;
  ycoB = 158;
  
  // draw the rectangle
  hbrDiagCross = mtds.CreatePatternBrush(idpsDiagCross, clrBlack, clrBlack);
  mtds.SetFgColor(hdsDisp, clrBlack);
  mtds.SetPen(hdsDisp, penSolid);
  mtds.SetBrush(hdsDisp, hbrDiagCross);
  mtds.Rectangle(hdsDisp, xcoL, ycoT, xcoR, ycoB);

  // release or destroy any resources that we no longer need
  mtds.DestroyBrush(hbrDiagCross);
  mtds.ReleaseDs(hdsDisp);
}
//============================================= 
//         FOR TESTING
//============================================= 
void printMaster() {
  // for testing only 
  for(int k = 0; k < sizeof(master)/2; k++) {
    Serial.print(master[k]);
    Serial.print(", ");
  }
  return;
}
//=============================================
void printUser() {
  // for testing only 
  for(int k = 0; k < sizeof(user)/2; k++) {
    Serial.print(user[k]);
    Serial.print(", ");
  }
  return;
}
