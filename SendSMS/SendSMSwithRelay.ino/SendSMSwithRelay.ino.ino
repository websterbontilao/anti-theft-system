#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2);

int inl = 7;
void setup() {
  pinMOde(inl, OUTPUT);
  digitalWrite(inl, HIGH);

  Serial.begin(9600);
  mySerial.begin(9600);
  
}

void loop(){
  digitalWrite(inl, HIGH);
  
  updateSerial();
  mySerial.println("AT+CMGF=1");
  
  updateSerial();
  mySerial.println("AT+CMGS=\"+639656442739\"");
  
  updateSerial();
  mySerial.print("on");
  
  updateSerial();
  mySerial.write(26);
  delay(60000);
  
  digitalWrite(inl, LOW);
  
  updateSerial();
  mySerial.println("AT+CMGF=1");
  
  updateSerial();
  mySerial.println("AT+CMGS=\"+639224012456\"");
  
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
