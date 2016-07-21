// IOT class example 2
// This does a wifi scan and allows us to connect to an AP
// Based on ESP8266 wifi example


#include "ESP8266WiFi.h"
#include "Arduino.h"

String password;
String APname;
int APID;
int APflag;
int numNWKS;
int Connected;
int wlStatus;

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

// reads and parses the ID # for the AP, set to trap for int bigger then 255
int readAPInt(void)
{
	char Ch;
	int num;
	int numr;
	int dec;
	
	dec = 0;
	num = -500;
	Ch = 'a';

	while ((Ch != '\n') && (num < 255))
	{
		while ((Serial.available() > 0))
		{
			Ch = Serial.read();
			Serial.print(Ch);
			if ((int(Ch) > 47) && (int(Ch) < 58))
			{
				(dec == 0) ? (num = 0) : (num);
				numr = num;
				num = ((int(Ch) - 48) + (numr * int(pow(10, dec))));
				dec++;
			}
		}
	}
	// no AP will ever be over 255
	(abs(num) > 255) ? (num = 256) : (num);
	return num;
}

int WifiScan(void)
{
	Serial.println("scan start");

	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
	{
		Serial.println("no networks found");
	}
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
	return n;
}

void setup() 
{
	Serial.begin(115200);
	password = "";
	APflag = 0;
	Connected = 0;
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	Serial.println("Setup done");
}


void loop() 
{
	// only scan if disconnected
	if (Connected < 1)
	{
		numNWKS = WifiScan();
		Serial.println("");
		password = "";
		Serial.println("Enter AP Id #");
		APID = readAPInt();
		Serial.print("ID  is ");
		Serial.println(APID);
		if (APID <= numNWKS)
		{
			APname = WiFi.SSID(APID - 1);
			Serial.print("you have selected AP: ");
			Serial.println(APname);
			APflag = 1;
		}
		else
		{
			Serial.println("you have not selected a valid AP");
			APflag = 0;
		}
		Serial.println("");
		if (APflag > 0)
		{
			Serial.println("Enter pass phrase for Wifi");
			password = readStringInput();
			Serial.print("password string is ");
			Serial.println(password);
			// put connection code here
			char* APN; // need char array
			APname.toCharArray(APN,APname.length());
			char* PWD; // need char array
			password.toCharArray(PWD, password.length());
			wlStatus = WiFi.begin(APN, PWD);
			delete APN;
			delete PWD;
		}
	}
	// show Ip's if connected to AP
	if (wlStatus == WL_CONNECTED)
	{
		Serial.print("Connected to ... ");
		Serial.println(APname);
		Serial.println("");
		Serial.print("IP is: ");
		Serial.println(WiFi.localIP());
		Serial.print("gateway is: ");
		Serial.println(WiFi.gatewayIP());
		Serial.print("subnet mask is: ");
		Serial.println(WiFi.subnetMask());
		Serial.println("");
		Connected = 1;
	}
	else
	{
		Connected = 0;
	}
	// Wait a bit before scanning again
	delay(5000);
}
