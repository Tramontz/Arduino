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

