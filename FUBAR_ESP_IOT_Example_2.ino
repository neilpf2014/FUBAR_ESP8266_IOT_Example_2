// FUBAR IOT class example 2
// This does a wifi scan and allows us to connect to an AP
// Based on ESP8266 wifi example


#include "ESP8266WiFi.h"
#include "Arduino.h"


char* Passphase;
char* AccessPoint;
char* seloption;
int apSlen;
String APname;
int APID;
int APflag;
int numNWKS;
int Connected;
int wlStatus;
int cnttime;
uint8_t LedState;


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
	char* somearray = new char[i+1];
	for (int j = 0; j < i; j++)
		somearray[j] = buffer[j];
	somearray[i] = '\0'; // null termination
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

// function to show Ip's if connected to AP
uint8_t ShowConnectStatus(void)
{
	uint8_t conn;
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
		conn = 1;
	}
	else
	{
		conn = 0;
	}
	return conn;
}

// toggle onboard LED no retVal
void TogLED(int state)
{
	uint8_t rState;
	if (state > 0)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		rState = 0;
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW);
		rState = 1;
	}
}

// toggle onboard LED, returns led state
uint8_t TogLED(uint8_t state)
{
	uint8_t rState;
	if (state > 0)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		rState = 0;
	}
	else
	{
		digitalWrite(LED_BUILTIN, LOW);
		rState = 1;
	}
	return rState;
}

// ardunio setup function
void setup(void) 
{
	Serial.begin(115200);
	APflag = 0;
	Connected = 0;
	cnttime = 0;
	LedState = 0; // led off
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.println("Setup done");
}


void loop() 
{
	// only scan and attempt conection if disconnected
	if (Connected < 1)
	{
		numNWKS = WifiScan();
		Serial.println("");
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
			if (Passphase != nullptr)
				delete Passphase;
			Passphase = readCharAInput();
			Serial.print("password string is "); // debug for PW char array
			Serial.println("");
			Serial.println(Passphase);
			// put connection code here
			// next 5 lines convert AP string to Chr array
			apSlen = APname.length() + 1;
			if (AccessPoint != nullptr)
				delete AccessPoint;
			AccessPoint = new char[apSlen];  // need to make sure not creating a memory leak here
			APname.toCharArray(AccessPoint, apSlen);
			
			//for (int i = 0; i < strlen(PW1); ++i) {
			//	Serial.printf("%02x ", PW1[i]);
			//}
			Serial.println("");
			//connect to wifi here and let us know when connected
			// time out in 10 sec
			wlStatus = WiFi.begin(AccessPoint, Passphase);
			while ((WiFi.status() != WL_CONNECTED) && (cnttime < 20))
			{
				LedState = TogLED(LedState);
				delay(500);
				Serial.print(".");
				cnttime++;
			}
			if (cnttime > 19)
				Serial.println("wifi connection timed out !");
			TogLED(1);
			Connected = ShowConnectStatus();
			cnttime = 0;
		}
	}
	
	// show Ip's if connected to AP
	if (Connected > 0)
	{
		TogLED(0);
		Serial.println("Disconnect or show status again ?");
		Serial.println("Enter D or S");
		if (seloption != nullptr)
			delete seloption;
		seloption = readCharAInput();
		if ((seloption[0] == 'D') || (seloption[0] == 'd'))
		{
			WiFi.disconnect();
			TogLED(1);
			Connected = 0;
		}
		if ((seloption[0] == 'S') || (seloption[0] == 's'))
		{
			Connected = ShowConnectStatus();
		}
		else
			if (Connected > 0)
				Serial.println("this message will repeat");

		// Wait a bit before looping again
		delay(2000);
	}
}
