#include "Arduino.h"
// library for ws2812 LED & others
#include "FastLED.h"

#define NUM_LEDS 1
#define DATA_PIN 2

/* Simple serial comm example for ESP8266
* Will just read input from serial & read out 1st
* Char
*/
char Colchar;
String inputStr;
String ColorStr;
String intStr;
int hue = 0;
int sat = 255;
int br = 127;
int i;

uint8_t LedState;
// This is the array of leds.
CRGB leds[NUM_LEDS];

// Read String input handling: thanks Rick
// "String" is now returned as a ptr to char assay
char* readCharAInput(void)
{
	char Ch = 'a';
	char buffer[50];
	int i = 0;
	while ((Ch != '\n') && (i < 50))
	{
		while ((Serial.available() > 0))
		{
			Ch = Serial.read();
			if ((Ch != '\n') && (Ch != '\r')) //need to trap both <CR> & <LF> !
			{
				buffer[i] = Ch;
				i++;
			}
			Serial.print(Ch);
		}
	}
	char* somearray = new char[i + 1];
	for (int j = 0; j < i; j++)
		somearray[j] = buffer[j];
	somearray[i] = '\0'; // null termination
	return somearray;
}

// Std Ardunio "setup"
void setup()
{
	// set up leds
	FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);

	Serial.begin(115200);
	Serial.println("");
	Serial.println("serial open");

	leds[0] = CRGB::Black;
	FastLED.show();
}

// Main loop
void loop()
{
	Serial.println("Enter hue with 'h'");
	Serial.println("followed by number (1-255) to set it");
	inputStr = readCharAInput();
	Colchar = inputStr.charAt(0);
	ColorStr = inputStr.substring(1);
	if (ColorStr.length() > 3)
		ColorStr = ColorStr.substring(0, 2);
	i = ColorStr.toInt();
	// some error traping
	if ((i > 255) || (i < 0))
		i = 0;

	// let's parse out what's getting done
	switch (Colchar)
	{
	case 'h':
	{
		if (i > 0) hue = i;
		leds[0] = CHSV(hue, sat, br);
	}
	break;
	case 's':
	{
		if (i > 0) sat = i;
		leds[0] = CHSV(hue, sat, br);
	}
	break;
	case 'v':
	{
		if (i > 0) br = i;
		leds[0] = CHSV(hue, sat, br);
	}
	break;
	case 'b':
	{
		leds[0] = CRGB::Black;
	}
	break;
	case 'w':
	{
		leds[0] = CRGB::WhiteSmoke;
	}
	break;
	default:
		break;
	}

	FastLED.show();
	Serial.println("led Set .....");
}
