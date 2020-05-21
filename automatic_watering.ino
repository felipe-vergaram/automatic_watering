#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include "Characters.h"

//Constantes
const int SetupPin = 2; //Switch arriba
const int ManualPin = 3;//Switch abajo
const int SelectPin = 4;//Pulsador 1
const int PlusPin = 5;  //Pulsador 2
const int MinusPin = 6; //Pulsador 3

//Estados de botones
int SetupButton = 0;
int ManualButton = 0;
int SelectButton = 0;
int PlusButton = 0;
int MinusButton = 0;


//Estado de sistemas
int KeyByte = 0;
int SetupStatus = 0;

//Configurar LCD
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);

//Definir tiempo
time_t t;
unsigned long t1, t2;
/*
byte black[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

byte arrow[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100, 
};
*/
void setup() {
  lcd.begin(16,2);
  lcd.home();
  lcd.backlight();
  pinMode(SetupPin,INPUT);
  pinMode(ManualPin,INPUT);
  pinMode(SelectPin,INPUT);
  pinMode(PlusPin,INPUT);
  pinMode(MinusPin,INPUT);

  lcd.createChar(0, black);
  lcd.createChar(1, arrow);

}

void loop() {

  //Check Arduino State
  SetupButton = digitalRead(SetupPin);
  ManualButton = digitalRead(ManualPin);
  t = now();

  //Mostrar la hora
  DisplayTime();
  //delay(400);
  //lcd.clear();
  lcd.setCursor(12,1);
  lcd.write("idle");
  //delay(400);
  //lcd.home();
  //lcd.write(byte(1));
/*
  if(SetupButton==HIGH){
    lcd.home();
    lcd.clear();
    lcd.print("SETUP");
    delay(400);
    lcd.clear();
  }
  */

  
  if(SetupButton==HIGH){
    lcd.clear();
  }
  while(SetupButton == HIGH){
    DisplayTime();
    lcd.home();
    lcd.print("S");
    //Check status
    SetupButton = digitalRead(SetupPin);
    SelectButton = digitalRead(SelectPin);
    PlusButton = digitalRead(PlusPin);
    MinusButton = digitalRead(MinusPin);

    if(SelectButton == LOW){
      SetupStatus = 1-SetupStatus;
      while(SelectButton == LOW){
        SelectButton = digitalRead(SelectPin);
      }
      lcd.clear();
    }
    //Cambiar hora/min
    if(SetupStatus == 1){
      lcd.setCursor(6,1);
      lcd.write(byte(1));
      lcd.setCursor(7,1);
      lcd.write(byte(1));
      
      if(PlusButton == LOW){
        adjustTime(3600);     //hora +1
        delay(300);
      }
      if(MinusButton == LOW){
        adjustTime(-3600);    //hora -1
        delay(300);
      }
    }
    else{
      lcd.setCursor(9,1);
      lcd.write(byte(1));
      lcd.setCursor(10,1);
      lcd.write(byte(1));

      if(PlusButton == LOW){
        adjustTime(60);       //min +1
        delay(300);
      }
      if(MinusButton == LOW){
        adjustTime(-60);      //min -1
        delay(300);
      }
    }
    t = now();
    if(SetupButton==LOW){
      lcd.clear();
      lcd.home();
      lcd.print("Fuera Setup");
      delay(1000);
      lcd.clear();
    }
   
    
  }
  
  //Funcionamiento Automático
  
}
