#include <SoftwareSerial.h>
#include <TinyGPS.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(10, 11);
TinyGPS gps;

String receivedData;
String disableCode = "disabl";
String enableCode = "enable";
String getLocationCode = "locati";

float lat, lon;

bool sysEnabled;

void setup() {
  // put your setup code here, to run once:

  // Turn ON GSM Module (START)
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(7000);
  // Turn ON GSM Module (END)
  
  // State Modes
  // TRUE - Enabled
  // FALSE - Disabled
  sysEnabled = true;

  // Set Relay VCC in Pin 16
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);

  // Set Pin for Relay Module
  pinMode(17, OUTPUT);
  digitalWrite(17, LOW);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  Serial.begin(9600);
  Serial1.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT+IPR?"); //Handshaking with SIM900
  updateSerial();

  mySerial.println("AT+CSQ"); // SIGNAL QUALITY
  updateSerial();

  mySerial.println("AT+CCID");
  updateSerial();

  mySerial.println("AT+CCREG=?");
  updateSerial();
}

void sendMessage(String number) {
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();

   mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\""+number+"\"");
  updateSerial();
  mySerial.print(lat,7);
  mySerial.print(",");
  mySerial.print(lon,7);
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
    
    // Check if recieved code is Disabled code
    // And check if system state is Enabled
    if (receivedData.equals(disableCode) && sysEnabled == true) {
      digitalWrite(17, HIGH);
      getCurrentLocation();
      sendMessage(senderNum);
      toggleSystemState();
    }

    if (receivedData.equals(enableCode) && sysEnabled == false) {
      digitalWrite(17, LOW); 
      toggleSystemState();
    }

    if (receivedData.equals(getLocationCode)) {
      Serial.println("Get Location");
      getCurrentLocation();
      sendMessage(senderNum);
    }
    
    delay(500);
  }
}

void getCurrentLocation() {
  bool _hasResult = false;
  // Set serial baud rate

  while(_hasResult == false){
    while(Serial1.available() && _hasResult == false) {
      if(gps.encode(Serial1.read()))
      {
        gps.f_get_position(&lat,&lon);
        _hasResult = true;
      }
    }
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

void toggleSystemState() {
  if (sysEnabled) {
    sysEnabled = false;
  }
  else {
    sysEnabled = true;
  }
}
