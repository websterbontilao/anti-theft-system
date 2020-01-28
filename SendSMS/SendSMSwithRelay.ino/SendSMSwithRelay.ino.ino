#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 8);

int inl = 17;
void setup() {
  pinMode(inl, OUTPUT);
  digitalWrite(inl, HIGH);

  Serial.begin(9600);
  mySerial.begin(9600);
  
}

void loop(){
  digitalWrite(inl, HIGH);
  
  updateSerial();
  mySerial.println("AT+CMGF=1");
  
  updateSerial();
  mySerial.println("AT+CMGS=\"+639959514051\"");
  
  updateSerial();
  mySerial.print("on");
  
  updateSerial();
  mySerial.write(26);
  delay(10000);
  
  digitalWrite(inl, LOW);
  
  updateSerial();
  mySerial.println("AT+CMGF=1");
  
  updateSerial();
  mySerial.println("AT+CMGS=\"+639067317096\"");
  
  updateSerial();
  mySerial.print("off");
  
  updateSerial();
  mySerial.write(26);
  delay(30000);
}

void updateSerial(){
  delay(500);

  while(Serial.available()) {
    mySerial.write(Serial.read());
  }

  while(mySerial.available()){
    Serial.write(mySerial.read());
  }
}
