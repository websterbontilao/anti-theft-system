#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); //A6 Tx & Rx is connected to Arduino #3 & #2
char msg;
char call;

//Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;

//Default baud of NEO-6M is 9600
int GPSBaud = 9600;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("GSM SIM800A BEGIN");
  Serial.println("Enter character for control option:");
  Serial.println("h : to disconnect a call");
  Serial.println("i : to receive a call");
  Serial.println("s : to send message");
  Serial.println("r : to receive message");
  Serial.println("c : to make a call");  
  Serial.println("e : to redial");
  Serial.println();
  delay(1000);

  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop()
{  
  
  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage("9282524809", "Hello");
      break;
    case 'c':
      MakeCall();
      break;
    case 'h':
      HangupCall();
      break;
    case 'e':
      RedialCall();
      break;
    case 'i':
      ReceiveCall();
      break;
    case 'r':
      ReceiveMessage();
      break;

  }
  
 if (mySerial.available()>0) {
 print(Serial.read());
 Serial.write(mySerial.available());
 }

  // Displays information when new sentence is available.
  while (gpsSerial.available() > 0)
    Serial.write(gpsSerial.read());

}

void SendMessage(String number, String message)
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.print("AT+CMGS=\"+63"+number+"\"\r");
  mySerial.println("AT+CMGS=\"+63"+number+"\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(message);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void ReceiveMessage()
{
  Serial.println("RecieveMessage()");
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to recieve a live SMS
  delay(1000);
  Serial.println(mySerial.available());
  if (mySerial.available()>0)
  {
    Serial.println("Recieved");
    msg=mySerial.read();
    Serial.print(msg);
  }
}

void MakeCall()
{
  mySerial.println("ATD+639282524809;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}


void HangupCall()
{
  mySerial.println("ATH");
  Serial.println("Hangup Call");
  delay(1000);
}

void ReceiveCall()
{
  mySerial.println("ATA");
  delay(1000);
  {
    call=mySerial.read();
    Serial.print(call);
  }
}

void print(char string) {
  Serial.println(string);
}

void RedialCall()
{
  mySerial.println("ATDL");
  Serial.println("Redialing");
  delay(1000);
}

