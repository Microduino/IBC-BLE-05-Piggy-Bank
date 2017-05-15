#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"
#include "eeprom.h"
#include "piggybankjson.h"

Key keyCoin(PIN_LINEA, INPUT_PULLUP);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  read_eeprom();
  if (songnumber < 0 || songnumber > 11 || money < 0|| money > 255)
  {
    songnumber = 2;
    money = 0;
    moneyupload(money);
  }
#if DEBUG
  Serial.println("**************START************");
  Serial.println("--------------");
  Serial.print("RESET DATA:");
  Serial.println(restart);
  Serial.print("songIndex DATA:");
  Serial.println(songnumber);
  Serial.println("--------------");
#endif
}

void loop() {
  //Serial.print("LINE Val:");
  //Serial.println(analogRead(PIN_LINEA));

  piggybankjson_data();

  if (keyCoin.read(0, LINE_MAX) == SHORT_PRESS)//Check if a coin was dropped.
  {
    playIndex = 0;//Resume music.
    money++;
    if (money > 255)
      money = 255;
    moneyupload(money);
    write_eeprom();
  }

  if (!playSound(songnumber))//Play music.
  {
    noTone(PIN_BUZZER);
    ledBreath(COLOR_INDIGO, 10);
    //setAllLed(COLOR_NONE);
  }
  delay(15);
}
