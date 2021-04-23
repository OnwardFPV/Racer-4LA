#include "BluetoothSerial.h"
#include <FastLED.h>
#include "SPIFFS.h"
#define NUM_LEDS 544

CRGB leds[NUM_LEDS];
   
BluetoothSerial SerialBT;


    
void setup(){
    Serial.begin(115200);

    FastLED.addLeds<WS2812B, 22, GRB>(leds, NUM_LEDS);            //Setup The LEDs
    FastLED.show();                                               //Clear any Start up LEDs
    
    SerialBT.begin("R4LA LED");                                   //Start the Bluetooth Connection
  
    SPIFFS.begin(true);                                           //Mount the File Structure
    File file = SPIFFS.open("/LED_States.txt");                   //Open the Settings File

    String brightness;
    brightness = file.readStringUntil(',');
    
    for (int i = 0; i <=543; i++){                                //Update the LEDs to the codes Onfile
      String rawledcolor;
      rawledcolor = file.readStringUntil(',');
      String ledcolorString = rawledcolor;
      long ledcolor = strtol(ledcolorString.c_str(), NULL, 0);
      Serial.print('LED');
      Serial.print(i);
      Serial.print(' ');
      Serial.print(ledcolor);
      leds[i] = ledcolor;
    }
  
    file.close();
    FastLED.setBrightness(brightness.toInt());
    FastLED.show();
}
    
void loop()
{

  
 if(SerialBT.available()) {
    if (SerialBT.readStringUntil(',')== "NEW") {
      Serial.println("New File Created");
      File file = SPIFFS.open("/LED_States.txt",FILE_WRITE);
      String BT_Packet = SerialBT.readString();
      file.print(BT_Packet);
      file.close();
      Serial.print("Recieved Packet:");
      Serial.println(BT_Packet);
  
      
      }
      else {
        Serial.println("File Appended");
        File file = SPIFFS.open("/LED_States.txt",FILE_APPEND);
        String BT_Packet = SerialBT.readString();
        file.print(BT_Packet);
        file.close();
        Serial.print("Recieved Packet:");
        Serial.println(BT_Packet);
    }
  }
}
  
