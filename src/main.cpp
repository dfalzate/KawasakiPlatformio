#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include "BluetoothSerial.h"
#include "logos.h"

U8G2_SH1107_64X128_F_SW_I2C u8g2(U8G2_R1, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define LED_BUILTIN 2

String message = "";
boolean initialization = true, start = false;


BluetoothSerial SerialBT;

#define LOGO_BLUETOOTH_HEIGHT 32
#define LOGO_BLUETOOTH_WIDTH 23
#define LOGO_KAWASAKI_HEIGHT 20
#define LOGO_KAWASAKI_WIDTH 127

String Valor;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);    
  SerialBT.begin("Kawasaki");
  Serial.println("Kawasaki bluetooth init!");  
  u8g2.begin();  
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(1,10,"Bluetooth init!");
  u8g2.drawXBMP(1, 20, LOGO_KAWASAKI_WIDTH, LOGO_KAWASAKI_HEIGHT, kawasaki);
  u8g2.sendBuffer();
  delay(5000);
  u8g2.noDisplay();
}

void loop(void) {

  if (start==true){
    if (initialization == true){
      u8g2.begin();
      u8g2.clearBuffer();
      initialization = false;
    }    
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(1,10,"Hello Diego!");
    u8g2.drawXBMP(1, 20, LOGO_BLUETOOTH_WIDTH, LOGO_BLUETOOTH_HEIGHT, logoBluetooth);
    u8g2.sendBuffer();	
  }
  
  if (SerialBT.available()){
    message = SerialBT.readStringUntil('\\');    
    if (message == "ATSTART"){
      start = true;
    } else if (message == "ATON" and start==true){
      digitalWrite(LED_BUILTIN, HIGH);
      u8g2.setCursor(40, 20);
      u8g2.print("Encendio!");
      u8g2.setCursor(40, 50);          
      u8g2.setFont(u8g2_font_fub20_tr);  
      u8g2.print("N");
    } else if (message == "ATOFF" and start==true){
      digitalWrite(LED_BUILTIN, LOW);
      u8g2.setCursor(40, 20);          
      u8g2.print("Apagado!");
      u8g2.setCursor(40, 50);          
      u8g2.setFont(u8g2_font_fub20_tr);  
      u8g2.print("N");
      u8g2.sendBuffer();    
      delay(1000);
      u8g2.print("1");
      u8g2.sendBuffer();    
      delay(1000);
      u8g2.print("2");
      u8g2.sendBuffer();    
      delay(1000);
      u8g2.print("3");
      u8g2.sendBuffer();    
      delay(1000);
    }
    u8g2.sendBuffer();    
  }  
  delay(1000);  
}