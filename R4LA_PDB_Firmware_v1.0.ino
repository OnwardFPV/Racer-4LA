//Code was created by Michael Beachum for use with OnwardFPV's R4LA PDB

//To change the overall brightness of the LEDs change the line of code "FastLED.setBrightness(33);" the "33" is can be a value between 0-255
//To change the name of the PDB when connecting to the controler change the line of code "SerialBT.begin("R4LA PDB");"

//Any changes to this code is allowed but not supported by OnwardFPV, tweak and modify the code at your own risk!

#include "BluetoothSerial.h"
#include <FastLED.h>
#include "SPIFFS.h"

#define NUM_LEDS 544

CRGB leds[NUM_LEDS];
   
BluetoothSerial SerialBT;


    
void setup(){

    FastLED.addLeds<WS2812B, 22, GRB>(leds, NUM_LEDS);            //Setup The LEDs
    FastLED.setBrightness(33);                                    //Set the LED Brightness
    FastLED.show();                                               //Clear any Start up LEDs
    
    SerialBT.begin("R4LA PDB");                                  //Start the Bluetooth Connection
  
    SPIFFS.begin(true);                                           //Mount the File Structure
    File file = SPIFFS.open("/LED_States.txt");                   //Open the Settings File
    String startcolorString = file.readString();                  //Convert the ASCII to String
    long startcolor = strtol(startcolorString.c_str(), NULL, 0);  //Convert the String to Long
    file.close();                                                 //Close the File
    fill_solid( leds, NUM_LEDS, startcolor);                      //Set the LED Color
    FastLED.show();                                               //Change the LEDs
}
    
void loop()
{
  String inputFromOtherSide;
 
  if (SerialBT.available()) {
    
      inputFromOtherSide = SerialBT.readString();                 //Read the incomming data
      String colorString = inputFromOtherSide;                    //Convert the incoming data to String
      long color = strtol(colorString.c_str(), NULL, 0);          //Convert the String to Long
      fill_solid( leds, NUM_LEDS, color);                         //Set the LED Color
      FastLED.show();                                             //Change the LEDs

      File file = SPIFFS.open("/LED_States.txt", FILE_WRITE);     //Open the Settings File
      file.print(color);                                          //Save the Color to File
      file.close();                                               //Close the File
      delay(1000);

      }
  }

  
