#include <Wire.h> 
#include <LiquidCrystal.h>
#include <Keypad.h>
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','a'}
  ,
  {'4','5','6','b'}
  ,
  {'7','8','9','c'}
  ,
  {'*','0','#','d'}
};

byte rowPins[ROWS] = {
  12, 13, A5, A4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  A3, A2, A1, A0
}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char codeInput[8];
byte time[4];
boolean refresh=true;//1 refresh one time...
char password[8];
int key=-1;
char lastKey;
char var;
boolean passwordEnable=false;

//Buttons for lcd shield
char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok key  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // not implemented

//leds

const int REDLED = 11;
const int GREENLED = 10;
//const int BLUELED = 12;
//RELAYPIN
boolean relayEnable = false;
const int RELAYPIN = 9;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int RELAY_TIME = 5000;

//TIME INTS
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;

boolean endGame = false;

boolean sdStatus = false; //search and destroy game enable used in config
boolean saStatus = false; //same but SAbotaghe
boolean doStatus = false; //for DEmolition
boolean start = true;
boolean defusing;
boolean cancelando;
// SOUND TONES
boolean soundEnable = true;
int tonepin = 8; // Pin 13 for the sound
int alarmTone1 = 700;
int alarmTone2 = 2600;
int activeTone = 1330;
int errorTone = 100;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime;//initial time for zone
byte team=0; // 0 = neutral, 1 = green team, 2 = red team


/*
 * Arduino bombe mode for softair game, developed by Tramontz
 * developed with help by gnacio Lillo's project
 * 
 * follow this  on my GitHub
 */

void setup(){
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.setCursor(3,0);
  tone(tonepin,2400,30);
  lcd.print("FRAIS METAL JACKET");// you can add your team name or someting cool
  lcd.setCursor(0,1);
  lcd.print("ANOTHER LOOSER TEAM");// you can add your team name or someting cool
  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);

  //PinModes
  pinMode(GREENLED, OUTPUT);     
  pinMode(REDLED, OUTPUT); 
  pinMode(RELAYPIN, OUTPUT);  
  // Manually implements the progress bar
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0,bar1);
  lcd.createChar(1,bar2);
  lcd.createChar(2,bar3);
  lcd.createChar(3,bar4);
  lcd.createChar(4,bar5);
  lcd.createChar(5,up);
  lcd.createChar(6,down);
}

void loop(){
  mainMenu();
}
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case RELEASED:
      switch (key){
         case 'd': defusing=false;
         break;
         case 'c': cancelando=false;
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defusing= true;
        break;
        case 'c': cancelando=true;
        break;
      }
    break;
  }
}
