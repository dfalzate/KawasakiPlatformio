#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// #include "BluetoothSerial.h"
#include "logos.h"

U8G2_SH1107_64X128_F_SW_I2C u8g2(U8G2_R1, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE);

#define LED_BUILTIN 2
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

String message = "", bluetoothReceived, character;
;
boolean initialization = true, start = false, endCharacter;

// BluetoothSerial SerialBT;

#define LOGO_BLUETOOTH_HEIGHT 32
#define LOGO_BLUETOOTH_WIDTH 23
#define LOGO_KAWASAKI_HEIGHT 20
#define LOGO_KAWASAKI_WIDTH 127

String Valor;

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      Serial.println("*********");
      Serial.print("New value: ");
      bluetoothReceived = "";
      endCharacter = false;
      for (int i = 0; i < value.length(); i++)
      {
        character = value[i];
        if (character == "/")
          endCharacter = true;
        if (endCharacter == false)
        {
          bluetoothReceived += value[i];
        }
      }
      Serial.println(bluetoothReceived);
      Serial.println("*********");
    }
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.clearBuffer();
    if (bluetoothReceived == "ATOn")
    {
      digitalWrite(LED_BUILTIN, HIGH);
      u8g2.drawStr(1, 10, "ON");
    }
    else if (bluetoothReceived == "ATOff")
    {
      digitalWrite(LED_BUILTIN, LOW);
      u8g2.drawStr(1, 10, "OFF");
    }
    else
    {
      Serial.println("No equal");
      Serial.println(bluetoothReceived + "/");
      u8g2.drawStr(1, 10, "Different");
    }
    u8g2.sendBuffer();
  }
};

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("KawaApp");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("Kawasaki bluetooth init!");
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.drawStr(1, 10, "Bluetooth init!");
  u8g2.drawXBMP(1, 20, LOGO_KAWASAKI_WIDTH, LOGO_KAWASAKI_HEIGHT, kawasaki);
  u8g2.sendBuffer();
  delay(5000);
  u8g2.noDisplay();
}

void loop(void)
{

  if (start == true)
  {
    if (initialization == true)
    {
      u8g2.begin();
      u8g2.clearBuffer();
      initialization = false;
    }
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(1, 10, "Hello Diego!");
    u8g2.drawXBMP(1, 20, LOGO_BLUETOOTH_WIDTH, LOGO_BLUETOOTH_HEIGHT, logoBluetooth);
    u8g2.sendBuffer();
  }

  // if (SerialBT.available()){
  //   message = SerialBT.readStringUntil('\\');
  //   if (message == "ATSTART"){
  //     start = true;
  //   } else if (message == "ATON" and start==true){
  //     digitalWrite(LED_BUILTIN, HIGH);
  //     u8g2.setCursor(40, 20);
  //     u8g2.print("Encendio!");
  //     u8g2.setCursor(40, 50);
  //     u8g2.setFont(u8g2_font_fub20_tr);
  //     u8g2.print("N");
  //   } else if (message == "ATOFF" and start==true){
  //     digitalWrite(LED_BUILTIN, LOW);
  //     u8g2.setCursor(40, 20);
  //     u8g2.print("Apagado!");
  //     u8g2.setCursor(40, 50);
  //     u8g2.setFont(u8g2_font_fub20_tr);
  //     u8g2.print("N");
  //     u8g2.sendBuffer();
  //     delay(1000);
  //     u8g2.print("1");
  //     u8g2.sendBuffer();
  //     delay(1000);
  //     u8g2.print("2");
  //     u8g2.sendBuffer();
  //     delay(1000);
  //     u8g2.print("3");
  //     u8g2.sendBuffer();
  //     delay(1000);
  //   }
  //   u8g2.sendBuffer();
  // }
  delay(1000);
}