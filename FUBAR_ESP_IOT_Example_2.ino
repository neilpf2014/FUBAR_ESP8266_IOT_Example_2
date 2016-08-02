// IOT class example 2
// This does a wifi scan and allows us to connect to an AP
// Based on ESP8266 wifi example


#include "ESP8266WiFi.h"
#include "Arduino.h"

const char APc[] = ""; // SSID, for testing will remove
const char Pwc[] = ""; // Passphase, for testing will remove
char* PW1;
char* AP1;
int apSlen;
String password;
String APname;
int APID;
int APflag;
int numNWKS;
int Connected;
int wlStatus;

// Read String input handling: thanks Rick
// "String" is now returned as a ptr to char assay
char* readStringInput(void)
{
	char Ch = 'a';
	char buffer[50];
	int i = 0;
	while ((Ch != '\n') && (i < 50))
	{
		while ((Serial.available() > 0))
		{
			Ch = Serial.read();
			Serial.print(Ch);
			if (Ch != '\n')
			{
				buffer[i] = Ch;
				i++;
			}
		}
	}
	char* somearray = new char[i+1];
	for (int j = 0; j < i; j++)
		somearray[j] = buffer[j];
	somearray[i] = '/0'; // null termination
	return somearray;
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

// wifi scanning done here
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

// ardunio setup function
void setup(void) 
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
	// only scan and attempt conection if disconnected
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
			if (PW1 != nullptr)
				delete PW1;
			PW1 = readStringInput();
			Serial.print("password string is "); // debug for PW char array
			Serial.println(PW1);
			// put connection code here
			// next 5 lines convert AP string to Chr array
			apSlen = APname.length() + 1;
			if (AP1 != nullptr)
				delete AP1;
			AP1 = new char[apSlen+1];  // need to make sure not creating a memory leak here
			APname.toCharArray(AP1, apSlen);

			Serial.print("ap test char array is :");
			Serial.println(AP1); //debug char array conversion
			//Serial.println(PWD);

			//connect to wifi here and let us know when connected
			wlStatus = WiFi.begin(AP1, PW1);
			while (WiFi.status() != WL_CONNECTED) 
			{
				delay(500);
				Serial.print(".");
			}
			Serial.print("wl status ");
			Serial.println(wlStatus);
		}
	}

	// show Ip's if connected to AP
	Serial.print("test is "); // debug
	Serial.println(wlStatus); // debug
	if (WiFi.status() == WL_CONNECTED)
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
	// Wait a bit before looping again
	// might add some more code -- like option to disconnect
	delay(5000);
}
