#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define VX_PIN A0
#define VY_PIN A1
#define BTN_PIN 2


RF24 radio(9, 10);
int data[3];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(2);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(0x1234567890LL);
  radio.stopListening();
  
  pinMode(BTN_PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 
  data[0] = analogRead(VX_PIN);
  data[1] = analogRead(VY_PIN);
  data[2] = digitalRead(BTN_PIN);
 
  
  radio.write(&data, sizeof(data));
  delay(15);

  Serial.println(data[0]);
  // Serial.println(data[3]);
}

