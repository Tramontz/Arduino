#include <Wire.h> 
#include <LiquidCrystal.h>
#include <Keypad.h>
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
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
  A3, A2, A1, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
  12, 13, A5, A4
}; //connect to the column pinouts of the keypad  //connect to the column pinouts of the keypad

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
boolean cancel;
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
  lcd.print("TEAM A");// you can add your team
  lcd.setCursor(0,1);
  lcd.print("TEAM B");// you can add your team 
  keypad.setHoldTime(15);
  keypad.setDebounceTime(15);// original 50
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
         case 'c': cancel=false;
         break;
      }
    break;
    case HOLD:
      switch (key){
        case 'd': defusing= true;
        break;
        case 'c': cancel=true;
        break;
      }
    break;
  }
}


////////////////////////////////////////////////////////////////////////////////////


  char* menu1[]={"Search&Destroy","Sabotage","Domination", "Configuration"      };      
  char* menu2[]={"Game Config","Sound Config", "Relay Test", "Auto Test"      };
  char* GAME_TIME="Game Time:";
  char* BOMB_TIME="Bomb Time:";
  char* ZERO_MINUTES="00 minutes";
  char* ARM_TIME="Arm Time:";
  char* ZERO_SECS="00 seconds";
  char* ENABLE_SOUND="Enable Sound?";
  char* YES_OR_NOT="A : Yes B : No";
  char* ENABLE_RELAYPIN="Enable Relay?";
  char* ENABLE_CODE="Enable Code Arm?";
  char* GAME_TIME_TOP="GAME TIME";
  char* ARMING_BOMB = "ARMING BOMB";
  char* ENTER_CODE = "Enter Code";
  char* CODE_ERROR = "Code Error!";
  char* BOMB_ARMED = "BOMB ARMED";
  char* DETONATION_IN = "DETONATION IN";
  char* DISARMING = "DISARMING BOMB" ;
  char* DISARM = "DISARMING";
  char* GAME_OVER = " GAME OVER! ";
  char* DEFENDERS_WIN = " DEFENDERS WIN ";
  char* SABOTAGE_FAIL= "SABOTAGE FAIL!";
                   
  ////////////////////////////////////////////////////////////////////////////////


  /*
 * this class provide some service method for catch and code/decode/change password
 */

//This fuction compare codeInput[8] and password[8] variables
boolean comparePassword(){

  for(int i=0;i<8;i++){
    if(codeInput[i]!=password[i])return false;
  }
  return true;
}

//Set the password variable
void setCode(){

  lcd.setCursor(0, 1);
  for(int i=0;i<8;i++){
    while(1){
      var= getNumber();
      if(var !='x'){
        codeInput[i] = var;

        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }
  }
}
void setCodeTime(){

  timeCalcVar=millis(); //get time since is activate

  for(int i=0;i<8;i++){                                     //print a * while pressin activating button
    while(1){               
      if(ACTIVATESECONDS*1000+timeCalcVar-millis()<=100){
        codeInput[i]='x';
        break;
      }

      lcd.setCursor(11,0);
      printTimeDom(ACTIVATESECONDS*1000+timeCalcVar-millis(),false);

      var= getNumber();
      if(var !='x'){
        codeInput[i] = var;

        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }
  }
}
void setPass(){ //set password domain
  lcd.setCursor(0, 1);

  for(int i=0;i<8;i++){ 
    while(1){
      var= getNumber();
      if(var !='x'){
        password[i] =  var;
        if (i != 0){
          lcd.setCursor(i-1,1);
          lcd.print("*");
          lcd.print(var);
        }
        else
        {
          lcd.print(var);
        }
        tone(tonepin,2400,30);
        break;
      }
    }  
  }
}

void setNewPass(){  //change old password

  while(1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter New Pass");
    setPass();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Retype Pass");

    setCode();

    if(comparePassword()){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password Set OK!");
      delay(2000); 
      break; 
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ERROR Dont Match!");
      if(soundEnable)tone(tonepin,errorTone,200);
      delay(2000); 

    }
  }
}
//Whait until a button is pressed if is a number return the number 'char' if not return x



char getNumber(){
  while(1){
    var = keypad.getKey();
    if (var){//
      switch (var) {
      case 'a': 
        return 'x';
        break;
      case 'b': 
        return 'x';
        break;

      case 'c': 
        return 'x';
        break;
      case 'd': 
        return 'x';
        break;
      case '*': 
        return 'x';
        break;
      case '#': 
        return 'x';
        break;
      default:
        return var;
        break;
      }
    }
    return 'x';
  }
}

byte getRealNumber(){

  while(1){                         //loop for decode input in type real
    var = keypad.waitForKey();

    if (var){
      switch (var) {
      case '1': 
        return 1;
        break;
      case '2': 
        return 2;
        break;

      case '3': 
        return 3;
        break;
      case '4': 
        return 4;
        break;
      case '5': 
        return 5;
        break;
      case '6': 
        return 6;
      case '7': 
        return 7;
        break;
      case '8': 
        return 8;
        break;
      case '9': 
        return 9;
        break;
        case '0': 
        return 0;
        break;
        
      default:
        return 11;
        break;
      }
    }
    return 11;
  }
}


//////////////////////////////////////////////////////////////////////////////////



void drawBar(byte porcent){
  //TODO: Optimize this code 
  int box=(8*porcent)/10;
  lcd.setCursor(0,1);
  while(box>=5){
    if(box>=5)
    {
      lcd.write(4);
      box-=5;
    }
  }
    switch(box){
    case 0:
      break;
    case 1:
      lcd.write((uint8_t)0);
      break;
    case 2:
      lcd.write(1);
      break;
    case 3:
      lcd.write(2);
      break;
    case 4:
      lcd.write(3);
      break;
    }
  

}
void cls(){
  lcd.clear();
  lcd.setCursor(0,0);
}

void printTime(unsigned long minute, unsigned long aTime){

  timeCalcVar=minute-aTime/60000;
  //Hours
  
  if(timeCalcVar/60==0 && refresh){
      lcd.clear();
      refresh=false;
      //delay(100);
      lcd.setCursor(3,1);
      Serial.println("!!!!");
  }
   
  if(timeCalcVar/60>=1){
    
    if(timeCalcVar/60<10)
  {
    lcd.setCursor(2,1);
    lcd.print("0");
    lcd.print(timeCalcVar/60);
  }
  else
  {
    lcd.print(timeCalcVar/60);
  }
  
  lcd.print(":");
  
  }
  //minutes
  if(timeCalcVar%60<10)
  {
    lcd.print("0");
    lcd.print(timeCalcVar%60);
  }
  else
  {
    lcd.print(timeCalcVar%60);
  }
  lcd.print(":");
  //seconds
  timeCalcVar=aTime/1000;
  if(59-(timeCalcVar%60)<10)
  {
    lcd.print("0");
    lcd.print(59-(timeCalcVar%60));
  }
  else
  {
    lcd.print(59-(timeCalcVar%60));
  }
  lcd.print(":");
  //this not mach with real time, is just a effect, there is a bug in milliseconds calculate
  lcd.print(999-(millis()%1000));
}

void printTimeDom(unsigned long aTime, boolean showMillis){
  //minutes
  if((aTime/60000)<10)
  {
    lcd.print("0");
    lcd.print(aTime/60000);
  }
  else
  {
    lcd.print(aTime/60000);
  }
  lcd.print(":");
  //seconds
  if(((aTime/1000)%60)<10)
  {
    lcd.print("0");
    lcd.print((aTime/1000)%60);
  }
  else
  {
    lcd.print((aTime/1000)%60);
  }
  if(showMillis){
    lcd.print(":");
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
      lcd.print(999-millis()%1000);

  }
}

void startGameCount(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Ready to Begin");
  lcd.setCursor(0,1);
  lcd.print("Push ANY Button");
  keypad.waitForKey();//if you press a button game start

  cls();
  lcd.setCursor(1,0);
  lcd.print("Starting Game");
  for(int i = 5; i > 0 ; i--){ // START COUNT GAME INIT
    lcd.setCursor(5,1);
    tone(tonepin,2000,100);
    lcd.print("IN ");
    lcd.print(i);
    delay(1000);
  }
  cls();
}

void checkArrows(byte i,byte maxx ){

  if(i==0){
    lcd.setCursor(15,1);
    lcd.write(6); 
  }
  if(i==maxx){  
    lcd.setCursor(15,0);
    lcd.write(5);
  }
  if(i>0 && i<maxx){
    lcd.setCursor(15,1);
    lcd.write(6);
    lcd.setCursor(15,0);
    lcd.write(5);  
  }
}

void activateRelay(){

  digitalWrite(RELAYPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(RELAY_TIME); 
  digitalWrite(RELAYPIN, LOW);

}




///////////////////////////////////////////////////////////////////////////////



void mainMenu(){
 
  digitalWrite(GREENLED, LOW); 
  digitalWrite(REDLED, LOW);

  lcd.print("Wellcome to Tramontz Softair mode 2.0, please select game mode (press any key for continue)");
  // check for incoming serial data:

  var = keypad.waitForKey();
  digitalWrite(GREENLED, LOW); 
  digitalWrite(REDLED, LOW); 

  // if whe start a new game from another we need to restart propertly this variables
  saStatus=false;
  sdStatus=false;
  doStatus=false;
  //Draw menu
  cls();//clear lcd and set cursor to 0,0
  int i=0;
 // HERE YOU CAN ADD MORE ITEMS ON THE MAIN MENU
  lcd.print(menu1[i]);
  lcd.setCursor(15,1);  //formattazione lcd
  checkArrows(i,2); //rileva le frecce direzionali
  
  while(1){ //attesa della scelta del game mod

    var = keypad.waitForKey();
    if(var == BT_UP && i>0){
      tone(tonepin,2400,30);
      i--;
      cls();
      lcd.print(menu1[i]);
      checkArrows(i,2);
      delay(50);
    }
    if(var == BT_DOWN && i<2){
      tone(tonepin,2400,30);
      i++;
      cls(); 
      lcd.print(menu1[i]);    
      checkArrows(i,2);
      delay(50);
    }

    if(var == BT_SEL){
      tone(tonepin,2400,30);
      cls();
      switch (i){

      case 0:
        sdStatus=true;
        configQuickGame();
        startGameCount();
        search();
        break;
      case 1: 
        saStatus=true;
        configQuickGame();
        startGameCount();
        //sabotage();   activate this when sabotage mode exist
        break;
      case 2:
        doStatus=true;
        configQuickGame();
        startGameCount();
        //domination();   activate this when domination mode exist
        break;
      case 3:
        config();
        break;

      }
    }
  }
}


void config(){
  //Draw menu
  lcd.clear();
  lcd.setCursor(0, 0);
  int i=0;
  
  delay(500);
  lcd.print(menu2[i]);
  checkArrows(i,3);

  while(1){
    var=keypad.waitForKey();
    if(var == BT_UP && i>0){
      tone(tonepin,2400,30);
      i--;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,3);
      delay(50);

    }
    if(var == BT_DOWN && i<3){
      tone(tonepin,2400,30);
      i++;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,3);
      delay(50);
    }
    if(var == BT_CANCEL){
      tone(tonepin,2400,30);
      mainMenu();
    }
    if(var == BT_SEL){
      tone(tonepin,2400,30);
      lcd.clear();
      switch (i){

      case 0:
        //gameConfigMenu();
        break;

      case 1:
        //soundConfigMenu();
        break;

      case 2:
        cls();
        lcd.print("RELAYPIN ON!");
        digitalWrite(RELAYPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(4000);   // wait for 4 second
        cls();
        lcd.print("RELAYPIN OFF!");
        digitalWrite(RELAYPIN, LOW);
        delay(2000);
        config();
        break;        

      }
    }
  }
}

void configQuickGame(){

  cls();
  //GAME TIME
  if(sdStatus || doStatus || saStatus){
    menu1:
    cls();
    lcd.print(GAME_TIME);
    delay(100);
    lcd.setCursor(0,1);
    lcd.print("00:00 hh:mm");
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    byte var2=0;
    for(int i=0;i<4;i++){ 
      while(1){
        if(i==2 && var2==0){
          lcd.print(":");
          var2=1;
        }

        byte varu= getRealNumber();
        if(varu !=11){

          time[i] =  varu;
          Serial.print(varu);


          lcd.print(varu);
          tone(tonepin,2400,30);

          break;
        }
      }  
    }
    lcd.noCursor();
    lcd.noBlink();
    lcd.setCursor(13,1);
    lcd.print("ok?");
    //zona donde pasamos los items a
    //redibujar
    while(1){
      var = keypad.waitForKey();
      if(var == 'd') // Accept
      {
        tone(tonepin,2400,30);
        GAMEMINUTES= ((time[0]*600)+(time[1]*60)+(time[2]*10)+(time[3]));
        break;
      }    
  if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,30);
        goto menu1;
      }           
    }
    tone(tonepin,2400,30);
    cls();
  }
  //BOMB TIME
  if(sdStatus || saStatus){
 
    menu2:
    cls();
    lcd.print(BOMB_TIME);
    delay(100);
    lcd.setCursor(0,1);
    lcd.print(ZERO_MINUTES);
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    for(int i=0;i<2;i++){ 
      while(1){
        byte varu= getRealNumber();
        if(varu !=11){
          time[i] =  varu;
          lcd.print(varu);
          tone(tonepin,2400,30);
          break;
        }
      }  
    }
    lcd.noCursor();
    lcd.noBlink();   
    lcd.setCursor(13,1);
    lcd.print("ok?");
    //zona donde pasamos los items a
    //redibujar
    while(1){
      var = keypad.waitForKey();
      if(var == 'd') //
      {
        tone(tonepin,2400,30);
        BOMBMINUTES= ((time[0]*10)+(time[1]));
        break;
      }    
  if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,30);
        goto menu2;
      }           
    }
    tone(tonepin,2400,30);
    cls();
  }
  cls();
  //ARMING TIME
  if(sdStatus || doStatus || saStatus){
        
    menu3:
    cls();
    lcd.print(ARM_TIME);
    delay(100);
    lcd.setCursor(0,1);
    lcd.print(ZERO_SECS);
    lcd.cursor();
    lcd.blink();
    lcd.setCursor(0,1);
    for(int i=0;i<2;i++){ 
      while(1){
        byte varu= getRealNumber();
        if(varu !=11){
          time[i] =  varu;
          lcd.print(varu);
          tone(tonepin,2400,30);
          break;
        }
      }  
    }
    lcd.noCursor();
    lcd.noBlink(); 
    lcd.setCursor(13,1);
    lcd.print("ok?");  
    
    //zona donde pasamos los items a
    //redibujar
    while(1){
      var = keypad.waitForKey();
      if(var == 'd') // Accept
      {
        tone(tonepin,2400,30);
        ACTIVATESECONDS= ((time[0]*10)+(time[1]));
        break;
      }    
  if(var == 'c') // Cancel or Back Button :')
      {
        tone(tonepin,2400,30);
        goto menu3;
      }           
    }
    tone(tonepin,2400,30);
    cls();
  }
  //sound??
  if(sdStatus || saStatus || doStatus){
    cls();
    lcd.print(ENABLE_SOUND);
    lcd.setCursor(0,1);
    lcd.print(YES_OR_NOT);

    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        soundEnable=true;
        tone(tonepin,2400,30);
        break;
      }  

      if(var == 'b' ){
        soundEnable=false;
        tone(tonepin,2400,30);
        break;
      }  
    }
  } 
  //Activate RELAY at Terrorist game ends??? Boom!

  if(sdStatus || saStatus){
    cls();
    lcd.print(ENABLE_RELAYPIN);
    lcd.setCursor(0,1);
    lcd.print(YES_OR_NOT);
    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        relayEnable=true;
        tone(tonepin,2400,30);
        break;
      }  
      if(var == 'b' ){
        relayEnable=false;
        tone(tonepin,2400,30);
        break;
      }  
    } 
  }
  //You Want a password enable-disable game?
  if(sdStatus || saStatus){
    cls();
    lcd.print(ENABLE_CODE);
    lcd.setCursor(0,1);
    lcd.print(YES_OR_NOT);

    while(1)
    {
      var = keypad.waitForKey();
      if(var == 'a' ){
        tone(tonepin,2400,30);
        setNewPass();
        passwordEnable = true;
        break;
      }  
      if(var == 'b' ){
        tone(tonepin,2400,30);
        passwordEnable = false;
        break;
      }  
    } 
    tone(tonepin,2400,30);
  }  
  //Continue the game :D
}


////////////////////////////////////////////////////////////////////////



void explodeEvent(){
  digitalWrite(REDLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("TERRORISTS WIN");
  lcd.setCursor(4,1);
  lcd.print("GAME OVER");
  for(int i = 200; i>0; i--)// this is the ultra hi definition explosion sound
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(relayEnable){
    activateRelay(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A: Yes B: No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);      
      if(sdStatus){
        startGameCount();
        search();
      }
      /*if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis()
        sabotage();
      }*/
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      mainMenu();

      break;
    }  
  } 
}
void failEvent(){
  digitalWrite(REDLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1,0);
  lcd.print("  TIME OUT");
  lcd.setCursor(4,1);
  lcd.print("GAME OVER");
  for(int i = 200; i>0; i--)// this is the ultra hi definition explosion sound
  {
    tone(tonepin,i);
    delay(20);
  }
  noTone(tonepin);
  if(relayEnable){
    activateRelay(); 
  }
  delay(5000);
  cls();

  //end code
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      if(sdStatus){
        startGameCount();
        search();
      }
     /* if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis()
        sabotage();
      }*/
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      mainMenu();

      break;
    }  
  } 
}
void disarmedEvent(){
  endGame = false;
  digitalWrite(REDLED, LOW); 
  digitalWrite(GREENLED, LOW);
  if(sdStatus || saStatus){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("BOMB DISARMED");
    lcd.setCursor(3,1);
    lcd.print("GOODS WIN");
    digitalWrite(GREENLED, HIGH);  
    delay(5000);
    digitalWrite(GREENLED, LOW); 
  }
  //end code
  lcd.clear();
  lcd.print("Play Again?");
  lcd.setCursor(0,1);
  lcd.print("A : Yes B : No");
  digitalWrite(REDLED, LOW);  
  digitalWrite(GREENLED, LOW); 
  while(1)
  {
    var = keypad.waitForKey();
    if(var == 'a' ){
      tone(tonepin,2400,30);
      if(sdStatus){
        startGameCount();
        search();
      }
      /*if(saStatus){
        saStatus=true;
        startGameCount();
        start=true; //to set iTime to actual millis()
        sabotage();
      }*/
    }  
    if(var == 'b' ){
      tone(tonepin,2400,30);
      mainMenu();
      break;
    }  
  } 
}


//////////////////////////////////////////////////////////////////////////////


void search() {
  refresh = true;
  cls();
  digitalWrite(REDLED, LOW);
  digitalWrite(GREENLED, LOW);
  //SETUP INITIAL TIME
  int minute = GAMEMINUTES - 1;
  unsigned long iTime = millis(); //  initialTime in millisec
  unsigned long aTime;
  //var='o';

  //Starting Game Code
  while (1) {    
                                 
    if (endGame) {    //default condition, If you fail disarm, variable endgame set to false
      failEvent();
    }

    //this part for blink the led (hope work)
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 50)digitalWrite(GREENLED, HIGH);
    if (timeCalcVar >= 90 && timeCalcVar <= 130)digitalWrite(GREENLED, LOW);

    lcd.setCursor(3, 0);
    lcd.print(GAME_TIME_TOP);
    aTime = millis() - iTime;
    lcd.setCursor(3, 1);

    //printing time on lcd screen

    printTime(minute, aTime);

    //###########################CHECKINGS##################

    //Check If Game End
    if (minute - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0)failEvent();
    //Serial.println(keypad.getKey());
    //if the game has password mode enabled, this code provide management
    if ('d' == keypad.getKey() && passwordEnable) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(ARMING_BOMB);
      delay(1000);//a little delay to think in the password
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(ENTER_CODE);

      setCodeTime(); //i need to set the comparation variable first it writes on codeInput[]

      if (comparePassword()) destroy(); //compare password
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print(CODE_ERROR);
      if (soundEnable)tone(tonepin, errorTone, 200);
      delay(500);
      cls();
    }
    //Check If Is Activating, detect keypad pressuring
    while (defusing && !passwordEnable)
    {
      digitalWrite(GREENLED, LOW);
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(ARMING_BOMB);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis(); //start disabling time
      while (defusing)
      {
        keypad.getKey();
                percent = (millis() - xTime) / (ACTIVATESECONDS * 10);
                
        drawBar(percent);
        //check if game time runs out during the disabling
        aTime = millis() - iTime;
        Serial.println(millis()-xTime);
        if ((minute - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) || minute - aTime / 60000 > 4000000000) {
          endGame = true;
        }
        timeCalcVar = (millis() - xTime) % 1000;

        if ( timeCalcVar >= 0 && timeCalcVar <= 40)
        {
          digitalWrite(REDLED, HIGH);
          if (soundEnable)tone(tonepin, alarmTone1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 520)
        {
          if (soundEnable)tone(tonepin, alarmTone2, 200);
          digitalWrite(REDLED, LOW);
        }



        if (percent >= 100)
        {
          digitalWrite(GREENLED, LOW);
          destroy();// jump to the next gamemode
        }
      }
      cls();
      digitalWrite(REDLED, LOW);

    }
  }
}

void destroy() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print(BOMB_ARMED);
  delay(1000);
  int minute = BOMBMINUTES - 1;
  unsigned long iTime = millis();
  unsigned long aTime;
  int largoTono = 50;

  //MAIN LOOP
  while (1) {

    //If you fail disarm.
    if (endGame) {
      explodeEvent();
    }

    //Led Blink

    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      digitalWrite(REDLED, HIGH);
      if (soundEnable)tone(tonepin, activeTone, largoTono);
    }
    if (timeCalcVar >= 180 && timeCalcVar <= 220) {
      digitalWrite(REDLED, LOW);
    }
    //Sound
    aTime = millis() - iTime;
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 245 && timeCalcVar <= 255 && minute - aTime / 60000 < 2 && soundEnable)tone(tonepin, activeTone, largoTono);
    if (timeCalcVar >= 495 && timeCalcVar <= 510 && minute - aTime / 60000 < 4 && soundEnable)tone(tonepin, activeTone, largoTono);
    if (timeCalcVar >= 745 && timeCalcVar <= 760 && minute - aTime / 60000 < 2 && soundEnable)tone(tonepin, activeTone, largoTono);
    if ( minute - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) < 10)largoTono = 300;

    lcd.setCursor(1, 0);
    lcd.print(DETONATION_IN);
    //Passed Time

    lcd.setCursor(3, 1);

    ////////HERE ARE THE TWO OPTIONS THAT ENDS THE GAME///////////

    ////TIME PASED AWAY AND THE BOMB EXPLODES
    if (minute - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) // Check if game ends
    {
      explodeEvent();
    }
    //print time

    printTime(minute, aTime);

    //// SECOND OPTION: YOU PRESS DISARMING BUTTON

    //IF IS A PASSWORD GAME

    if ('d' == keypad.getKey() && passwordEnable) {

      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(DISARMING);
      delay(1000);//a little delay to think in the password

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(ENTER_CODE);

      setCodeTime();// we need to set the compare variable first

      //then compare

      if (comparePassword()) {
        disarmedEvent();
      }
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print(CODE_ERROR);
      if (soundEnable)tone(tonepin, errorTone, 200);
      delay(500);
      cls();
    }

    if (defusing && !passwordEnable) // disarming bomb
    {
      lcd.clear();
      digitalWrite(REDLED, LOW);
      lcd.setCursor(3, 0);
      lcd.print(DISARM);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();
      while (defusing)
      {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime = millis() - iTime;
        if ((minute - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) || minute - aTime / 60000 > 4000000000) {
          endGame = true;
        }
        timeCalcVar = (millis() - xTime) % 1000;
        if (timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(GREENLED, HIGH);
          if (soundEnable)tone(tonepin, alarmTone1, 200);
        }
        if (timeCalcVar  >= 480 && timeCalcVar <= 500)
        {
          if (soundEnable)tone(tonepin, alarmTone2, 200);
          digitalWrite(GREENLED, LOW);
        }
        unsigned long seconds = (millis() - xTime);
        percent = seconds / (ACTIVATESECONDS * 10);
        drawBar(percent);

        //BOMB DISARMED GAME OVER
        if (percent >= 100)disarmedEvent();
      }
      digitalWrite(REDLED, LOW);
      digitalWrite(GREENLED, LOW);
      cls();
    }
  }
}


