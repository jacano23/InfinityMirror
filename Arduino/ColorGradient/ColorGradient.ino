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
 * of the red, green, and blue components respectively.
 *
 * For example, to get green, you could type:
 *   40,100,0!
 *
 * You can leave off the exclamation point if you change the
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

void setup()
{
  // Start up the serial port, for communication with the PC.
  Serial.begin(115200);
  Serial.println("Ready to receive colors."); 

  
  int redStart = 255;
  int blueStart = 0;
  int greenStart = 0;
  for(uint16_t i = 0; i < LED_COUNT/3; i++)
  {
    int stepMove = 255*i*3/LED_COUNT;
    color.red =  redStart - stepMove;
    color.blue = blueStart + stepMove;
    color.green = 0;
    
    colors[i] = color;
  }
  
  redStart = 0;
  blueStart = 255;
  greenStart = 0;
  for(uint16_t i = 0; i < LED_COUNT/3; i++)
  {
    int stepMove = 255*i*3/LED_COUNT;
    color.blue =  blueStart - stepMove;
    color.green = greenStart + stepMove;
    color.red = 0;
    
    colors[i + LED_COUNT/3] = color;
  }
  
  redStart = 0;
  blueStart = 0;
  greenStart = 255;
  for(uint16_t i = 0; i < LED_COUNT/3; i++)
  {
    int stepMove = 255*i*3/LED_COUNT;
    color.green =  greenStart - stepMove;
    color.red = redStart + stepMove;
    color.blue = 0;
    
    colors[i + 2*LED_COUNT/3] = color;
  }

  ledStrip.write(colors, LED_COUNT);  
}



void Sort()
{
  for(int i = 0; i < 120; i++)
  {
    for(int j = i; j >= 0; j--)
    {
      if(colors[j].green < colors[j-1].green)
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

void loop()
{

}
