#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// I2C adresi genellikle 0x27'dir.
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("ROBOT HAZIR");

  lcd.setCursor(0, 1);
  lcd.print("HIZ:75");
  lcd.setCursor(10, 1);
  lcd.print("BAT:%91");

  lcd.setCursor(0, 2);
  lcd.print("X:120");
  lcd.setCursor(6, 2);
  lcd.print("Y:85");
  lcd.setCursor(12, 2);
  lcd.print("A:45");

  lcd.setCursor(0, 3);
  lcd.print("M1:1");
  lcd.setCursor(5, 3);
  lcd.print("M2:1");
  lcd.setCursor(10, 3);
  lcd.print("M3:0");
  lcd.setCursor(15, 3);
  lcd.print("M4:1");
}

void loop() {
}