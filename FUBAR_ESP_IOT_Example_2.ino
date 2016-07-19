// IOT class example 2
// This does a wifi scan and allows us to connect to an AP
// Based on ESP8266 wifi example


#include "ESP8266WiFi.h"
#include "Arduino.h"

String password;

// Read String input handling: thanks Rick
// removed to function to make code cleaner
String readStringInput(void)
{
	char Ch = 'a';
	String inputStr;
	while (Ch != '\n')
	{
		while ((Serial.available() > 0))
		{
			Ch = Serial.read();
			Serial.print(Ch);
			inputStr += String(Ch);
		}
	}
	// not sure why this doesn't work ???
	if (inputStr == "\n")
	{
		inputStr = "nothing";
	}
	return inputStr;
}

void setup() {
	Serial.begin(115200);
	password = "";
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	Serial.println("Setup done");
}

void loop() {
	Serial.println("scan start");


	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
		Serial.println("no networks found");
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			delay(10);
		}
	}
	Serial.println("");
	password = "";
	Serial.println("Enter some string");

	// string input handling function defined above
	password = readStringInput();
	Serial.println("");
	Serial.print("password string is ");
	Serial.println(password);
	// Wait a bit before scanning again
	delay(2000);
}
