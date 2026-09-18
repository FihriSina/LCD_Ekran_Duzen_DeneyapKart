#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD adresi genellikle 0x27'dir.
LiquidCrystal_I2C lcd(0x27, 20, 4);

// LCD için özel "ü" karakteri
byte uKarakteri[8] = {
  B01010,
  B00000,
  B10001,
  B10001,
  B10001,
  B10011,
  B01101,
  B00000
};

void setup() {
  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.createChar(0, uKarakteri);

  // Birinci satır
  lcd.setCursor(2, 0);
  lcd.print("Selamun Aleyk");
  lcd.write(byte(0));
  lcd.print("m");

  // İkinci satır
  lcd.setCursor(6, 1);
  lcd.print("Dostlar");
}

void loop() {
  // Yazı sürekli ekranda kalır.
}