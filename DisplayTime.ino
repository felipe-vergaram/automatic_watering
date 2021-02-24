void DisplayTime() {

  if (hour(t)<10){
    lcd.setCursor(6,0);
    lcd.print(0);
    lcd.setCursor(7,0);
    lcd.print(hour(t));
  }
  else {
    lcd.setCursor(6,0);
    lcd.print(hour(t));
  }

  lcd.setCursor(8,0);
  lcd.print(":");
  
  if (minute(t)<10){
    lcd.setCursor(9,0);
    lcd.print(0);
    lcd.setCursor(10,0);
    lcd.print(minute(t));
  }
  else {
    lcd.setCursor(9,0);
    lcd.print(minute(t));
  }
}
