int command;
const int numberOfDataValues = 4;
int dataValues [numberOfDataValues];
int currentValue;

char incomingByte;  // incoming data
int  LED = 13;      // LED pin
int dataNumber; 

#include <PololuLedStrip.h>
PololuLedStrip<12> ledStrip;
#define LED_COUNT 120
rgb_color colors[LED_COUNT];
rgb_color color;


void setup() 
{
  Serial.begin(9600);
  
  currentValue = 0;
  command = 0;
  dataNumber = 0;
  
  for(int i = 0; i < numberOfDataValues; i++)
    dataValues[i] = 0;
}

void loop() {}

void ProcessCommand()
{
  if(dataValues[0] == 1)
  {
    
  }
  else if(dataValues[0] == 2)
  {
    color.red = dataValues[1];
    color.green = dataValues[2];
    color.blue = dataValues[3];
    
    for(int i = 0; i < LED_COUNT; i++)
      colors[i] = color;
      
    ledStrip.write(colors, LED_COUNT);   
  }
}


void serialEvent() {
   while (Serial.available()) {
     // get the new byte:
     int inInt = Serial.read() - 48; 

     
     if(inInt == -15)
     {
       dataValues[dataNumber] = currentValue;
       currentValue = 0;
       dataNumber = 0;
       ProcessCommand();
       for(int i = 0; i < numberOfDataValues; i++)
         dataValues[i] = 0;
     }
     else if(inInt == -2)
     {
       dataValues[dataNumber] = currentValue;
       currentValue = 0;
       dataNumber++;
     }
     else
     {
       currentValue = currentValue*10 + inInt;        
     }
   }
}
