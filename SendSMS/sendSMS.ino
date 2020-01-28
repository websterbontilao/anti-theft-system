#include <SoftwareSerial.h>
#include <TinyGPS.h>

//Create software serial object to communicate with A6
// A6 PWR -> A6 VCC_IN
// A6 GND -> Arduino GND
// A6 U_Tx -> Arduino D07
// A6 U_Rx -> Arduino D06

// 9282524809
// 9235576953
//String MsgDisabled = "Successfully disabled.";
//String MsgEnabled = "Successfully enabled.";

// Arduino Pins
int GPS_RXPin = 20;
int GPS_TXPin = 21;
int SMS_RXPin = 17;
int SMS_TXPin = 16;
int relayDigitalPin = 17;
int GPSBaud = 9600;

SoftwareSerial smsSerial(SMS_TXPin, SMS_RXPin); //gsm

// Create a TinyGPS++ object
float lat,lon;
boolean enableReceive;
TinyGPS gps;

void setup()
{
//  Serial.begin(115200); // connect gps sensor get location
  smsSerial.begin(9600);
  Serial.begin(57600); // connect serial gps initial baud
  Serial.println("The GPS Received Signal:");

  enableReceive = false;
//  receiveMessage();
  sendMessage();
}
 
void loop()
{
  if (!enableReceive) {
    displayGPSInfo();
  } else {
    sendMessage();
  }
}

void sendMessage() {
  Serial.println("SENDING MESSAGE");
  // Configuring TEXT mode
  smsSerial.println("AT+CMGF=1");
  updateSerial();
  
  smsSerial.println("AT+CMGS=\"+639067317096\"");
  updateSerial();
  
  smsSerial.print(10.36245,6);
//  smsSerial.print(lat,6);
  smsSerial.print(",");
  smsSerial.print(123.9312,6);
//  smsSerial.print(lon,6);

  updateSerial();
  smsSerial.write(26); //ASCII code undo

  enableReceive = true;
}

void receiveMessage(){
  Serial.println("RECEIVING MESSAGE");
  // Configuring TEXT mode
  smsSerial.println("AT+CMGF=1");
  updateSerial();
  
  // Decides how newly arrived SMS messages should be handled
  smsSerial.println("AT+CNMI=1,2,0,0,0"); 
  updateSerial();
}

void toggleRelay(boolean state){
  if (state) {
    digitalWrite(relayDigitalPin, HIGH);
  }
  else {
    digitalWrite(relayDigitalPin, LOW);
  }
}

void updateSerial()
{
  delay(500);
  
  while (Serial.available()) 
  {
    //Forward what Serial received to Software Serial Port
    smsSerial.write(Serial.read());
  }
  while(smsSerial.available()) 
  {
    //Forward what Software Serial received to Serial Port
    Serial.write(smsSerial.read());
  }
}

void displayGPSInfo()
{ 
  while(Serial1.available()){ // check for gps data
    if(gps.encode(Serial1.read()))// encode gps data
    { 
      gps.f_get_position(&lat,&lon); // get latitude and longitude
  
      Serial.print("Position: ");
      
      //Latitude
      Serial.print("Latitude: ");
      Serial.print(lat,6);
      
      Serial.print(",");
      
      //Longitude
      Serial.print("Longitude: ");
      Serial.println(lon,6); 
      
      sendMessage();
      set_delay(3);
   }
  }
}

void set_delay(int seconds)
{
  delay(seconds * 1000);
}
