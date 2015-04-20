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
#define LED_COUNT 120
rgb_color colors[LED_COUNT];
rgb_color color;

long redBar = 0;
long blueBar = 90000L;
long greenBar = 180000L;

long Value(rgb_color inputColor)
{
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
        ledStrip.write(colors, 120);         
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
        
        ledStrip.write(colors, 120);        
        delay(1);  
      }
    }
  }
}

void QuickSort(int low, int high)
{
  if(low == high | low > high)
    return; 
    
  int pivot = low;
  int left = low + 1;
  int right = high;
  
  while(left < right)
  {
    while(Value(colors[left]) < Value(colors[pivot]))
      left++;
    while(Value(colors[right]) > Value(colors[pivot]))
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
        ledStrip.write(colors, 120);        
        delay(100);  
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
        ledStrip.write(colors, 120);        
        delay(10);  
  
  QuickSort(low, right - 1);
  QuickSort(right + 1, high);
  
  
}

void setup()
{
  // Start up the serial port, for communication with the PC.
  Serial.begin(115200);
  //Serial.print(123456); 
  
  
  color.red = 0;
  color.blue = 0;
  color.red = 0;
  for(uint16_t i = 0; i < 120; i++)
  {
    colors[i].red = 0;
    colors[i].green = 0;
    colors[i].blue = 0;
  }
  
  ledStrip.write(colors, 120);  
  
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
  
  
  ledStrip.write(colors, 120);   
  /*
  colors[LED_COUNT-3].red = maximum;
  colors[LED_COUNT-3].green = 0;
  colors[LED_COUNT-3].blue = 0;
  colors[LED_COUNT-2].red = 0;
  colors[LED_COUNT-2].green = maximum;
  colors[LED_COUNT-2].blue = 0;
  colors[LED_COUNT-1].red = 0;
  colors[LED_COUNT-1].green = 0;
  colors[LED_COUNT-1].blue = maximum;
  */
  ledStrip.write(colors, 120);   
  
  //BubbleSort();
  QuickSort(0, LED_COUNT - 1);
  
  color.red = 0;
  color.blue = 0;
  color.red = 0;
  /*
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    if(colors[i].red == 0 & colors[i].blue == 0)
    {
      colors[i].green = 0;
      ledStrip.write(colors, 120);   
      delay(1);
    }
    else if(colors[i].blue == 0 & colors[i].green == 0)
    {
      colors[i].red = 0;
      ledStrip.write(colors, 120);  
      delay(1);
    }
    else if(colors[i].green == 0 & colors[i].red == 0)
    {
      colors[i].blue = 0;
      ledStrip.write(colors, 120);  
      delay(1);
    }
  }
  */
  for(int i = 0; i < LED_COUNT - 1; i++)
  {
    long x = Value(colors[i]);
    Serial.println(x);
    Serial.println(", ");
  }
    Serial.println(Value(colors[LED_COUNT - 1]));
  
}

void loop()
{
  
  
  
  //Serial.print("H");

}

