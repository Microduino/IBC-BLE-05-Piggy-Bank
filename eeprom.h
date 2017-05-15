#include <EEPROM.h>

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

int songnumber = 2;
int money = 0;

struct config_type
{
  int EEPROMsong;
  int EEPROMmoney;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(5, config_readback);
  songnumber = config_readback.EEPROMsong;
  money = config_readback.EEPROMmoney;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMsong = songnumber;
  config.EEPROMmoney = money;
  EEPROM_write(5, config);         // 变量config存储到EEPROM，地址0写入
}
