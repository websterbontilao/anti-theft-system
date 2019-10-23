#include <SoftwareSerial.h>

//Create software serial object to communicate with A6
// A6 PWR -> A6 VCC_IN
// A6 GND -> Arduino GND
// A6 Tx -> Arduino D03
// A6 Rx -> Arduino D02
SoftwareSerial mySerial(3, 2); 

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and A6
  mySerial.begin(9600);
  
  Serial.println("Initializing..."); 
  delay(1000);
  
  //Once the handshake test is successful, it will back to OK
  mySerial.println("AT"); 
  updateSerial();
  
  sendMessage("9959514051", "Hello");
}

void loop()
{
}

void sendMessage(String num, String msg) {
  // Configuring TEXT mode
  mySerial.println("AT+CMGF=1");
  updateSerial();
  
  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
  mySerial.println("AT+CMGS=\"+63"+num+"\"");
  updateSerial();
  
  //text content
  mySerial.print(msg); 
  updateSerial();
  mySerial.write(26);
  
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    //Forward what Serial received to Software Serial Port
    mySerial.write(Serial.read());
  }
  while(mySerial.available()) 
  {
    //Forward what Software Serial received to Serial Port
    Serial.write(mySerial.read());
  }
}