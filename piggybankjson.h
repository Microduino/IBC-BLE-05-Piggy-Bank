#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String songS = "";
String restart = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2da1\"}";

//{"count":1}
aJsonObject *createMessage(uint8_t x)
{
  aJsonObject *msg = aJson.createObject();
  aJsonObject *analog = aJson.createItem(x);
  aJson.addItemToObject(msg, "count", analog);
  return msg;
}

void moneyupload(uint8_t x)
{
  aJsonObject *msg = createMessage(x);
  aJson.print(msg, &serial_stream);
  Serial.println();
  aJson.deleteItem(msg);
}

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}

//{"reset":"true"}
//{"songIndex":"3"}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("count", money);
      Ctrldata("songIndex", songnumber);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *resetval = aJson.getObjectItem(msg, "reset");
  if (resetval) {
    restart = resetval->valuestring;
    if (restart == "true")
    {
      money = 0;
      moneyupload(money);
      tone(PIN_BUZZER, 800);
      setAllLed(COLOR_GREEN);
      delay(1000);
      noTone(PIN_BUZZER);
      setAllLed(COLOR_NONE);
      write_eeprom();
    }
  }
  else {
#if DEBUG
    Serial.println("no restart data");
#endif
  }
  aJsonObject *songval = aJson.getObjectItem(msg, "songIndex");
  if (songval) {
    songS = songval->valuestring;
    songnumber = songS.toInt();
    playIndex = 0;
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no songIndex data");
#endif
  }
#if DEBUG
  Serial.println("--------------");
  Serial.print("RESET DATA:");
  Serial.println(restart);
  Serial.print("songIndex DATA:");
  Serial.println(songnumber);
  Serial.println("--------------");
#endif
}

void piggybankjson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}
