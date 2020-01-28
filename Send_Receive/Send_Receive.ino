#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(10, 11);

String receivedData;
String disableCode = "disable";
String enableCode = "enable";
String getLocationCode = "locati";

void setup() {
  // put your setup code here, to run once:

  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
}

void sendMessage(String number) {
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();

   mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\""+number+"\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.print(10.36245,6);
//  smsSerial.print(lat,6);
  mySerial.print(",");
  mySerial.print(123.9312,6);
  updateSerial();
  mySerial.write(26);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Receive message
  if (mySerial.available() > 0) {
    String messageReceived = mySerial.readString();
    // code number of characters (6)
    String senderNum = messageReceived.substring(9, 22);
    receivedData = messageReceived.substring(51, 57);
    
    if (receivedData.equals(disableCode)) {
      Serial.println("Disabled");
      // Do disabled code here
    }

    if (receivedData.equals(enableCode)) {
      Serial.println("Enabled");
      // Do enable code here
    }

    if (receivedData.equals(getLocationCode)) {
      Serial.println("Get Location");
      sendMessage(senderNum);
      // Do get location code here
    }
    
    delay(500);
  }
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
