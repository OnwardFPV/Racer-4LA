#include "BluetoothSerial.h"
#include <FastLED.h>
#include "SPIFFS.h"
#define NUM_LEDS 544

CRGB leds[NUM_LEDS];
   
BluetoothSerial SerialBT;


    
void setup(){
    //In order to get debugging information returned on the serial monitor, uncomment the following command.
    
    //Serial.begin(115200);                                         //Debugging Serial Port

    FastLED.addLeds<WS2812B, 22, GRB>(leds, NUM_LEDS);            //Setup The LEDs
    FastLED.show();                                               //Clear any Start up LEDs
    
    SerialBT.begin("R4LA LED");                                   //Start the Bluetooth Connection
  
    SPIFFS.begin(true);                                           //Mount the File Structure
    File file = SPIFFS.open("/LED_States.txt");                   //Open the Settings File

    String brightness;                                            //Setup the Brightness Variable
    brightness = file.readStringUntil(',');                       //Read the Brightenss Variable
    
    for (int i = 0; i <=543; i++){                                //Update the LEDs to the codes Onfile
      String rawledcolor;                                         
      rawledcolor = file.readStringUntil(',');                    
      String ledcolorString = rawledcolor;                        
      long ledcolor = strtol(ledcolorString.c_str(), NULL, 0);    
      leds[i] = ledcolor;                                         //Assign the color code to the LED
    }
  
    file.close();                                                 //End the file read
    
    FastLED.setBrightness(brightness.toInt());                    //Brightness variable is converted from string to int
    FastLED.show();                                               //Update the LEDs
}
    
void loop()
{

//The following packet is intended to erase the previous file stores in SPIFFS and collect the startup variables.
//The first section of the packet is a string to erase the old file and start the new file.
//The second section of the packet is a string to set the brightness of the LEDs.

 if(SerialBT.available()) {
    if (SerialBT.readStringUntil(',')== "NEW") {
      File file = SPIFFS.open("/LED_States.txt",FILE_WRITE);
      String BT_Packet = SerialBT.readString();
      file.print(BT_Packet);
      file.close();
      
      //If you need to see the data being recieved over the bluetooth port uncomment the following commands.
      //If you are not getting information or the code is causing an error, make sure the debugging serial port commands
      //are uncommented.
      
      //Serial.println("New File Created");
      //Serial.print("Packet Recieved");
      //Serial.println(BT_Packet);    
      }

      //The following packets are structured sections of the total LED color codes. Due to the amount of data being sent over bluetooth
      //the total color codes have to be sent over in sections. Each section is delayed by 1.2 seconds to let the bluetooth buffer clear.
      
      else {
        File file = SPIFFS.open("/LED_States.txt",FILE_APPEND);
        String BT_Packet = SerialBT.readString();
        file.print(BT_Packet);
        file.close();

        //If you need to see the data being recieved over the bluetooth port uncomment the following commands.
        //If you are not getting information or the code is causing an error, make sure the debugging serial port commands
        //are uncommented.
        
        //Serial.println("File Appended");      
        //Serial.print("Recieved Packet:");
        //Serial.println(BT_Packet);
    }
  }
}
  
