#include <Arduino.h>
#include <U8g2lib.h>
#include "BluetoothSerial.h"

#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1107_64X128_F_SW_I2C u8g2(U8G2_R1, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define LED_BUILTIN 2

String message = "";

BluetoothSerial SerialBT;

#define LOGO_BLUETOOTH_HEIGHT 32
#define LOGO_BLUETOOTH_WIDTH 23

static const unsigned char PROGMEM logoBluetooth[] =
    {
  B10000000,  B11111111,  B00000000,
  B11100000,  B11111111,  B00000011,
  B11110000,  B11111111,  B00001111,
  B11111000,  B11100011,  B00001111,
  B11111100,  B11000011,  B00011111,
  B11111100,  B10000011,  B00111111,
  B11111110,  B00000011,  B00111111,
  B11111110,  B00000011,  B00111110,
  B11111110,  B00100011,  B00111100,
  B10001110,  B01100011,  B01111000,
  B00001110,  B01100011,  B01111000,
  B00011110,  B00100010,  B01111100,
  B00111111,  B00000000,  B01111110,
  B01111111,  B00000000,  B01111111,
  B11111111,  B10000000,  B01111111,
  B11111111,  B11000001,  B01111111,
  B11111111,  B11000001,  B01111111,
  B11111111,  B10000000,  B01111111,
  B01111111,  B00000000,  B01111111,
  B00111111,  B00000000,  B01111110,
  B00011110,  B00100010,  B01111100,
  B00001110,  B01100011,  B01111000,
  B10001110,  B01100011,  B01111000,
  B11111110,  B00100011,  B00111100,
  B11111110,  B00000011,  B00111110,
  B11111110,  B00000011,  B00111111,
  B11111100,  B10000011,  B00111111,
  B11111100,  B11000011,  B00011111,
  B11111000,  B11100011,  B00001111,
  B11110000,  B11111111,  B00001111,
  B11100000,  B11111111,  B00000011,
  B10000000,  B11111111,  B00000000,
};


String Valor;

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000);  
  u8g2.begin();
  SerialBT.begin("Kawasaki");
  Serial.println("Aranque");
  delay(1000);  
}

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_6x12_tr);	// choose a suitable font
  u8g2.drawStr(1,10,"Hello Diego!");	// write something to the internal memory  
  u8g2.drawXBMP(1, 20, 23, 32, logoBluetooth);			// transfer internal memory to the display
  u8g2.sendBuffer();	
  if (SerialBT.available()){
    String newMessage = SerialBT.readStringUntil('\\');
    u8g2.clearBuffer();	
    if (newMessage == "ATON"){
      digitalWrite(LED_BUILTIN, HIGH);
      u8g2.drawStr(1,10,"Encendio!");	// write something to the internal memory  
    } else if (newMessage == "ATOFF"){
      digitalWrite(LED_BUILTIN, LOW);
      u8g2.drawStr(1,10,"Apagado!");	// write something to the internal memory  
    }
    u8g2.sendBuffer();
    // char incomingChar = SerialBT.read();
    // if (incomingChar != '\n'){
    //   message += String(incomingChar);
    // }
    // else{
    //   message = "";
    // }    
    // Serial.write(incomingChar);  
    // if (message=="AB"){
    //   Serial.println("Es AB");
    //   digitalWrite(LED_BUILTIN,HIGH);
    // } else{
    //   Serial.println("No es AB");
    //   digitalWrite(LED_BUILTIN,LOW);
    // }
  }
  // if (SerialBT.available())
  // {
  //   u8g2.clearBuffer();	
  //   u8g2.drawStr(1,10,"Hello Diego bluetooth!");	// write something to the internal memory  
  //   Valor = SerialBT.readStringUntil('\n');
  //   Serial.println(Valor);
  //   u8g2.setCursor(1, 30);
  //   u8g2.print(Valor);    
  //   u8g2.sendBuffer();	
  // }
  delay(1000);  
}