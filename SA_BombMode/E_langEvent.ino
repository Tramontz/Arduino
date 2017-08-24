
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
