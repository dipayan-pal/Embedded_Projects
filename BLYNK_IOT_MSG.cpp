/*****************************************
* Capture string from BLYNK IOT platform
* and display it on the serial prompt
* This data would be finally displayed in
* the LCD as a custom msg recieved from the
* cloud using a custom driver------------
* version 1.0  -- Dipayan Pal*************
******************************************/


//These ids come from the BLYNK template
//Must be defined at the very begining
/**************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL8ixLbjAG"
#define BLYNK_DEVICE_NAME "ESP Random Blynk"
#define BLYNK_AUTH_TOKEN "i7N3fEVjGMvZRt8ks8Z8eTf-duYF308I"
/**************************************************************/

#include<Arduino.h>
#include<BlynkSimpleEsp32.h> 	//BLYNK header for esp32 target

char ssid[]="*****";		//Local network credentials
char pass[]="*****";
char auth[]=BLYNK_AUTH_TOKEN;

WidgetTerminal terminal(V3);

BLYNK_WRITE(V3)
{
  String msg=param.asStr(); 	//Temp variable to store the string to be used in the display
				//driver ckt.
  Serial.print("Msg from BLYNK terminal => ");
  Serial.println(msg);
  terminal.flush();
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial connected");
  Blynk.begin(auth,ssid,pass);	//Start the BLYNK server
}

void loop()
{
  Blynk.run();			//Run BLYNK
}

