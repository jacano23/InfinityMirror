/* LedStripColorTester: Example Arduino sketch that lets you
 * type in a color on a PC and see it on the LED strip.
 *
 * To use this, you will need to plug an Addressable RGB LED
 * strip from Pololu into pin 12.  After uploading the sketch,
 * select "Serial Monitor" from the "Tools" menu.  In the input
 * box, type a color and press enter.
 *
 * The format of the color should be "R,G,B!" where R, G, and B
 * are numbers between 0 and 255 representing the brightnesses
 * of the red, red, and blue components respectively.
 *
 * For example, to get red, you could type:
 *   40,100,0!
 *
 * You can leave off the exclamation pouint16_t if you change the
 * Serial Monitor's line ending setting to be "Newline" instead
 * of "No line ending".
 *
 * Please note that this sketch only transmits colors to the
 * LED strip after it receives them from the computer, so if
 * the LED strip loses power it will be off until you resend
 * the color.
 */
 
#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 97
rgb_color colors[LED_COUNT];
rgb_color color;

int sortDelayAmount = 25;

long redBar = 0;
long blueBar = 90000L;
long greenBar = 180000L;
rgb_color zeroColor;
int command;
const int numberOfDataValues = 10;
int dataValues [numberOfDataValues];
int currentValue;

char incomingByte;  // incoming data
int  LED = 13;      // LED pin
int dataNumber; 
int singleValueMode = 0;

int topWallCount;
int topWallTailCount;
int leftWallCount;
int bottomWallCount; 
int rightWallCount;

int topWallStart;
int topWallEnd;
int topWallTailStart;
int topWallTailEnd;
int leftWallStart;
int leftWallEnd;
int bottomWallStart;
int bottomWallEnd;
int rightWallStart;
int rightWallEnd;



long Value(rgb_color inputColor)
{
  if(singleValueMode)
    return inputColor.red + inputColor.blue + inputColor.green;
  long value = inputColor.green;
  if(inputColor.red > 0 & inputColor.blue > 0)
  {
    value = redBar + 300L*inputColor.blue;
  }
  else if(inputColor.blue > 0 & inputColor.green > 0)
  {
    value = blueBar + 300L*inputColor.green;
  }
  else if(inputColor.green > 0 & inputColor.red > 0)
  {
    value = greenBar + 300L*inputColor.red;
  }
  else if(inputColor.red == 0 & inputColor.blue == 0)
  {
    value = greenBar;
  }
  else if(inputColor.blue == 0 & inputColor.green == 0)
  {
    value = redBar;
  }
  else
  {
    value = blueBar;
  }
  
  return value;
}

long Value2(rgb_color inputColor)
{
  return inputColor.green*300 + inputColor.blue - inputColor.red*300;
}


void Sort()
{
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    for(uint16_t j = i; j >= 0; j--)
    {
      if(Value(colors[j]) < Value(colors[j-1]))
      {
        rgb_color temp = colors[j];
        colors[j] = colors[j-1];
        colors[j-1] = temp;
        ledStrip.write(colors, LED_COUNT);         
        delay(1);  
      }
    }
  }
}

void BubbleSort()
{
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    for(uint16_t j = 0; j < LED_COUNT - 1; j++)
    {
      if(Value(colors[j]) > Value(colors[j+1]))
      {
        int red = colors[j].red;
        int blue = colors[j].blue;
        int green = colors[j].green;
        colors[j].red = colors[j+1].red;
        colors[j].green = colors[j+1].green;
        colors[j].blue = colors[j+1].blue;
        colors[j+1].red = red;
        colors[j+1].green = green;
        colors[j+1].blue = blue;
        
        ledStrip.write(colors, LED_COUNT);        
        delay(sortDelayAmount);  
      }
    }
  }
}

void InsertionSort()
{
  for(int i = 1; i < LED_COUNT; i++)
  {
    int j = i;
    while(j > 0 & Value(colors[j-1]) > Value(colors[j]))
    {
      int red = colors[j].red;
      int blue = colors[j].blue;
      int green = colors[j].green;
      colors[j].red = colors[j-1].red;
      colors[j].green = colors[j-1].green;
      colors[j].blue = colors[j-1].blue;
      colors[j-1].red = red;
      colors[j-1].green = green;
      colors[j-1].blue = blue;
      
      ledStrip.write(colors, LED_COUNT); 
      delay(sortDelayAmount);
      j = j - 1;
    }
  }
}

void ShellSort()
{
  int gaps [] = { 20, 10, 8, 5, 2, 1 };
  int numberOfGaps = 6;
  for(int i = 0; i < numberOfGaps; i++)
  {
    for(int k = 0; k < gaps[i]; k++)
    {
      for(int l = 1; l < LED_COUNT/gaps[i]; l++)
      {
        int j = l*gaps[i] + k;
        int cont = 1;
        while(cont)
        {
          if(j < LED_COUNT & j - gaps[i] >= 0)
            cont = 1;
          else
            cont = 0;
          if(cont)
          {
            if(j > 0 & Value(colors[j-gaps[i]]) > Value(colors[j]))
            {
              int red = colors[j].red;
              int blue = colors[j].blue;
              int green = colors[j].green;
              colors[j].red = colors[j-gaps[i]].red;
              colors[j].green = colors[j-gaps[i]].green;
              colors[j].blue = colors[j-gaps[i]].blue;
              colors[j-gaps[i]].red = red;
              colors[j-gaps[i]].green = green;
              colors[j-gaps[i]].blue = blue;
              
              ledStrip.write(colors, LED_COUNT); 
              delay(sortDelayAmount);
              j = j - gaps[i];
            }       
            else
              cont = 0;    
          }
        }        
      }
    }
  }  
}

void QuickSort(int low, int high)
{
  if(low == high | low > high)
    return;     
  if(low == high + 1 | low == high - 1)
  {
      if(Value(colors[low]) > Value(colors[high]))
      {
        int red = colors[low].red;
        int blue = colors[low].blue;
        int green = colors[low].green;
        colors[low].red = colors[high].red;
        colors[low].green = colors[high].green;
        colors[low].blue = colors[high].blue;
        colors[high].red = red;
        colors[high].green = green;
        colors[high].blue = blue;
          ledStrip.write(colors, LED_COUNT);  
        delay(sortDelayAmount);
      }
      return;
  }
  int pivot = low;
  int left = low + 1;
  int right = high;
  
  while(left < right)
  {
    while(Value(colors[left]) < Value(colors[pivot]) & left < LED_COUNT - 1)
      left++;
    while(Value(colors[right]) >= Value(colors[pivot]) & right >= left)
      right--;
    if(left < right)
    {
      int red = colors[left].red;
      int blue = colors[left].blue;
      int green = colors[left].green;
      colors[left].red = colors[right].red;
      colors[left].green = colors[right].green;
      colors[left].blue = colors[right].blue;
      colors[right].red = red;
      colors[right].green = green;
      colors[right].blue = blue;
        ledStrip.write(colors, LED_COUNT);   
   //left++;     
        delay(sortDelayAmount);  
    }
  }
  
  int red = colors[pivot].red;
  int blue = colors[pivot].blue;
  int green = colors[pivot].green;
  colors[pivot].red = colors[right].red;
  colors[pivot].green = colors[right].green;
  colors[pivot].blue = colors[right].blue;
  colors[right].red = red;
  colors[right].green = green;
  colors[right].blue = blue;
        ledStrip.write(colors, LED_COUNT);        
        delay(sortDelayAmount);  
  
  QuickSort(low, right - 1);
  QuickSort(right + 1, high);
  
  
}

void ZeroLedArray()
{
  for(int i = 0; i < LED_COUNT; i++)      
      colors[i] = zeroColor;
}

void WriteAllLeds(rgb_color colorInput)
{
  for(int i = 0; i < LED_COUNT; i++)      
      colors[i] = colorInput;
      
  ledStrip.write(colors, LED_COUNT);  
}

void Flash(rgb_color colorInput, int flashAmount, int delayAmount)
{
  while(!Serial.available())
  {
    WriteAllLeds(colorInput);
    delay(flashAmount);
    WriteAllLeds(zeroColor);
    if(delayAmount > 0)
      delay(delayAmount);
      /*
    for(int i = 0; i < 100; i++)
    {
      if(Serial.available())
        i = 100;
      delay(10);
    }
    */
  }
}

void Chase(rgb_color colorInput, int amount, int delayAmount)
{
  int currentPosition = 0;
  while(!Serial.available())
  {
    ZeroLedArray();
    for(int i = 0; i < amount; i++)
    {
      colors[(currentPosition + i)%LED_COUNT] = colorInput;
    }
    ledStrip.write(colors, LED_COUNT);  
    currentPosition = (currentPosition + 1)%LED_COUNT;
    if(delayAmount > 0)
      delay(delayAmount);
  }
}

void Fade(rgb_color colorInput, int delayAmount)
{
  WriteAllLeds(zeroColor);
  int percent = 0;
  rgb_color currentColor;
  int change = 5;
  while(!Serial.available())
  {
    if(percent == 0)
      change = 2;
    else if(percent == 100)
      change = - 2;
      
    percent += change;
    
    currentColor.red = colorInput.red*percent/100;
    currentColor.green = colorInput.green*percent/100;
    currentColor.blue = colorInput.blue*percent/100;
    
    WriteAllLeds(currentColor);
    if(delayAmount > 0)
      delay(delayAmount);
  }
}

void Twinkle(rgb_color colorInput, int amount, int delayAmount)
{
  int currentPosition = 0;
  while(!Serial.available())
  {
    ZeroLedArray();
    for(int i = 0; i < amount; i++)
    {
      colors[random(0,LED_COUNT)] = colorInput;
    }
    ledStrip.write(colors, LED_COUNT);  
    if(delayAmount > 0)
      delay(delayAmount);
  }
}

void TopWall(rgb_color colorInput)
{
  ZeroLedArray();
  for(int i = topWallStart; i <= topWallEnd; i++)
      colors[i] = colorInput;
  for(int i = topWallTailStart; i <= topWallTailEnd; i++)
      colors[i] = colorInput;
  ledStrip.write(colors, LED_COUNT);  
}

void LeftWall(rgb_color colorInput)
{
  ZeroLedArray();
  for(int i = leftWallStart; i <= leftWallEnd; i++)
      colors[i] = colorInput;
  ledStrip.write(colors, LED_COUNT);  
}

void BottomWall(rgb_color colorInput)
{
  ZeroLedArray();
  for(int i = bottomWallStart; i <= bottomWallEnd; i++)
      colors[i] = colorInput;
  ledStrip.write(colors, LED_COUNT);  
}

void RightWall(rgb_color colorInput)
{
  ZeroLedArray();
  for(int i = rightWallStart; i <= rightWallEnd; i++)
  //for(int i = 10; i <= 30; i++)
      colors[i] = colorInput;
  ledStrip.write(colors, LED_COUNT);  
}

void CounterClockwise(rgb_color inputColor, int delayAmount)
{
  int currentPosition = 0;
  while(!Serial.available())
  {
    ZeroLedArray();
    if(currentPosition == 0)
      TopWall(inputColor);
    else if(currentPosition == 1)
      LeftWall(inputColor);
    else if(currentPosition == 2)
      BottomWall(inputColor);
    else if(currentPosition == 3)
      RightWall(inputColor);
    
    currentPosition = (currentPosition + 1)%4;
    
    if(delayAmount > 0)
      delay(delayAmount);
  }
}

void Clockwise(rgb_color inputColor, int delayAmount)
{
  int currentPosition = 0;
  while(!Serial.available())
  {
    ZeroLedArray();
    if(currentPosition == 0)
      TopWall(inputColor);
    else if(currentPosition == 1)
      RightWall(inputColor);
    else if(currentPosition == 2)
      BottomWall(inputColor);
    else if(currentPosition == 3)
      LeftWall(inputColor);
    
    currentPosition = (currentPosition + 1)%4;
    
    if(delayAmount > 0)
      delay(delayAmount);
  }
}

void TwoColorSort(rgb_color colorInput1, rgb_color colorInput2)
{
  
}

void RandomizeAll()
{
   int maximum = 250;
  
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    int choose = random(0,3);
    if(choose == 1)
    {
      int number = random(0,maximum);
      color.red = number;
      color.blue = maximum - 1 - number;  
      color.green = 0;
      
      colors[i] = color;
    }
    else if(choose == 2)
    {
      int number = random(0,maximum);
      color.blue = number;
      color.green = maximum - 1 - number;  
      color.red = 0;
      
      colors[i] = color;
    }
    else
    {
      int number = random(0,maximum);
      color.green = number;
      color.red = maximum - 1 - number;  
      color.blue = 0;
      
      colors[i] = color;
    }
  }
  
  
  ledStrip.write(colors, LED_COUNT);   
}

//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************
//*****************************************************************************************************

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  currentValue = 0;
  command = 0;
  dataNumber = 0;
  
  for(int i = 0; i < numberOfDataValues; i++)
    dataValues[i] = 0;
  
  
  color.red = 0;
  color.blue = 0;
  color.red = 0;
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    colors[i].red = 0;
    colors[i].green = 0;
    colors[i].blue = 0;
  }
  
  ledStrip.write(colors, LED_COUNT);  
  
  RandomizeAll();
  
  color.red = 0;
  color.blue = 0;
  color.red = 0;
  singleValueMode = 0;
  
  zeroColor.red = 0;
  zeroColor.green = 0;
  zeroColor.blue = 0;
  
  topWallCount = 10;
  topWallTailCount = 9;
  leftWallCount = 27;
  bottomWallCount = 20; 
  rightWallCount = 27;

  topWallStart = 0;
  topWallEnd = topWallStart + topWallCount;
  topWallTailStart = 88;
  topWallTailEnd = topWallTailStart + topWallTailCount; 
  leftWallStart = 60;
  leftWallEnd = leftWallStart + leftWallCount;
  bottomWallStart = 39;
  bottomWallEnd = bottomWallStart + bottomWallCount;
  rightWallStart = 11;
  rightWallEnd = rightWallStart + rightWallCount;

}

void loop()
{
}

//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
void ProcessCommand()
{
  //Set all to a single color
  //1.Red.Green.Blue!
  if(dataValues[0] == 1)
  {
    rgb_color singleColor;
    singleColor.red = dataValues[1];
    singleColor.green = dataValues[2];
    singleColor.blue = dataValues[3];
    dataValues[0] == 0;
    WriteAllLeds(singleColor);
    Serial.println(".......");
    Serial.println(1);
    Serial.println(singleColor.red);
    Serial.println(singleColor.green);
    Serial.println(singleColor.blue);
  }
  //Flashes on the goes off
  //3.Red.Green.Blue.On Delay.Off Delay!
  else if(dataValues[0] == 3)
  {
    rgb_color flashColor;
    flashColor.red = dataValues[1];
    flashColor.green = dataValues[2];
    flashColor.blue = dataValues[3];
    dataValues[0] == 0;
    int flashAmount = dataValues[4];
    int delayAmount = dataValues[5];
    Flash(flashColor, flashAmount, delayAmount);
  }
  //Chase around the mirror
  //4.Red.Green.Blue.Number of Leds on.Movement delay!
  else if(dataValues[0] == 4)
  {
    rgb_color chaseColor;
    chaseColor.red = dataValues[1];
    chaseColor.green = dataValues[2];
    chaseColor.blue = dataValues[3];
    int amount = dataValues[4];
    int delayAmount = dataValues[5];
    dataValues[0] == 0;
    Chase(chaseColor, amount, delayAmount);
  }
  //Fade - brightens then dims
  //5.Red.Green.Blue.Delay!
  else if(dataValues[0] == 5)
  {
    rgb_color fadeColor;
    fadeColor.red = dataValues[1];
    fadeColor.green = dataValues[2];
    fadeColor.blue = dataValues[3];
    dataValues[0] == 0;
    int delayAmount = dataValues[4];
    Fade(fadeColor, delayAmount);
  }
  //Twinkle - The is the "matrix" effect
  //6.Red.Green.Blue.number of Leds.transition delay!
  else if(dataValues[0] == 6)
  {
    rgb_color twinkleColor;
    twinkleColor.red = dataValues[1];
    twinkleColor.green = dataValues[2];
    twinkleColor.blue = dataValues[3];
    dataValues[0] == 0;
    int amount = dataValues[4];
    int delayAmount = dataValues[5];
    Twinkle(twinkleColor, amount, delayAmount);
  }
  //Ignore
  else if(dataValues[0] == 7)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    TopWall(inputColor);
  }
  //Ignore
  else if(dataValues[0] == 8)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    LeftWall(inputColor);
  }
  //Ignore
  else if(dataValues[0] == 9)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    BottomWall(inputColor);
  }
  //Ignore
  else if(dataValues[0] == 10)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    RightWall(inputColor);
  }
  //Counter Clockwise side turns
  //11.Red.Green.Blue.delay amount!
  else if(dataValues[0] == 11)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    dataValues[0] == 0;
    int delayAmount = dataValues[4];
    CounterClockwise(inputColor, delayAmount);
  }
  //Clockwise side turns
  //12.Red.Green.Blue.delay amount!
  else if(dataValues[0] == 12)
  {
    rgb_color inputColor;
    inputColor.red = dataValues[1];
    inputColor.green = dataValues[2];
    inputColor.blue = dataValues[3];
    dataValues[0] == 0;
    int delayAmount = dataValues[4];
    Clockwise(inputColor, delayAmount);
  }  
  //Bubble Sort - Single Color
  //101.Red.Green.Blue!
  else if(dataValues[0] == 101)
  {
    singleValueMode = 1;
    color.red = dataValues[1];
    color.green = dataValues[2];
    color.blue = dataValues[3];
    
    for(int i = 0; i < LED_COUNT; i++)
    {
      int numerator = random(1,101);
      
      colors[i].red = color.red*numerator/100;
      colors[i].green = color.green*numerator/100;
      colors[i].blue = color.blue*numerator/100;
    }
      
    ledStrip.write(colors, LED_COUNT);   
    BubbleSort();
    singleValueMode = 0;
  }  
  //Bubble Sort - Gradient
  //102!
  else if(dataValues[0] == 102)
  {
   RandomizeAll();
   BubbleSort();
  }
  //Insertion Sort - Single Color
  //103.Red.Green.Blue!
  else if(dataValues[0] == 103)
  {
    singleValueMode = 1;
    color.red = dataValues[1];
    color.green = dataValues[2];
    color.blue = dataValues[3];
    
    for(int i = 0; i < LED_COUNT; i++)
    {
      int numerator = random(1,101);
      
      colors[i].red = color.red*numerator/100;
      colors[i].green = color.green*numerator/100;
      colors[i].blue = color.blue*numerator/100;
    }
      
    ledStrip.write(colors, LED_COUNT);   
    InsertionSort();
    singleValueMode = 0;
  }  
  //Insertion Sort - Gradient
  //104!
  else if(dataValues[0] == 104)
  {
   RandomizeAll();
   InsertionSort();
  }
  //Shell Sort - Single Color
  //105.Red.Green.Blue!
  else if(dataValues[0] == 105)
  {
    singleValueMode = 1;
    color.red = dataValues[1];
    color.green = dataValues[2];
    color.blue = dataValues[3];
    
    for(int i = 0; i < LED_COUNT; i++)
    {
      int numerator = random(1,101);
      
      colors[i].red = color.red*numerator/100;
      colors[i].green = color.green*numerator/100;
      colors[i].blue = color.blue*numerator/100;
    }
      
    ledStrip.write(colors, LED_COUNT);   
    ShellSort();
    singleValueMode = 0;
  }  
  //Shell Sort - Gradient
  //106!
  else if(dataValues[0] == 106)
  {
   RandomizeAll();
   ShellSort();
  }
  //Quick Sort - Single Color
  //107.Red.Green.Blue!
  else if(dataValues[0] == 107)
  {
    singleValueMode = 1;
    color.red = dataValues[1];
    color.green = dataValues[2];
    color.blue = dataValues[3];
    
    for(int i = 0; i < LED_COUNT; i++)
    {
      int numerator = random(1,101);
      
      colors[i].red = color.red*numerator/100;
      colors[i].green = color.green*numerator/100;
      colors[i].blue = color.blue*numerator/100;
    }
      
    ledStrip.write(colors, LED_COUNT);   
    QuickSort(0,LED_COUNT-1);
    singleValueMode = 0;
  }  
  //Quick Sort - Gradient
  //108!
  else if(dataValues[0] == 108)
  {
   RandomizeAll();
   QuickSort(0,LED_COUNT-1);
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
