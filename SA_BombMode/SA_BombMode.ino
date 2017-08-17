#include <LiquidCrystal.h>
#include <Keypad.h>
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);



void setup() {

  lcd.print("Wellcome to Tramontz Softair mode 2.0");
  // put your setup code here, to run once:

   // open the serial port:
  Serial.begin(9600);

char BT_RIGHT = '4';
char BT_UP = 'a';
char BT_DOWN = 'b';
char BT_LEFT = '6';
char BT_SEL = 'd';   // Ok key  
char BT_CANCEL = 'c';
char BT_DEFUSER = 'x';   // not implemented

}

void loop() {
}
