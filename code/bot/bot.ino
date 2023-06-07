#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Adafruit_NeoPixel.h>

#include <Servo.h>
Servo myservo;
Servo myservo2;

#define SPEED_1 5 
#define DIR_1 4

#define SPEED_2 6
#define DIR_2 7

#define MATRIX_PIN    0
#define LED_COUNT 4

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(LED_COUNT, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

RF24 radio(9, 10);
int data[3];

int pos = 15;

void setup() {

  Serial.begin(9600);
  myservo.attach(3);
  myservo2.attach(12);
  matrix.begin();
  radio.begin();
  radio.setChannel(2);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, 0x1234567890LL);
  radio.startListening();

  for (int i = 4; i < 8; i++) {     
    pinMode(i, OUTPUT);
  }

}

void loop() { 

  if (radio.available()) {
    radio.read(&data, sizeof(data));

    for (int i = 0; i < matrix.numPixels(); i++) {
      matrix.setPixelColor(i, 255, 255, 255);
      matrix.show();  
    }

    int servoPosition = map(data[0], 0, 1023, 0, 180);
    myservo.write(servoPosition);

    if (data[2] == 1) {
      myservo2.write(45);
      pos = 15;
    }  
    else if (data[2] == 0) {
      myservo2.write(pos);
      pos += 1;
      if (pos == 75) {
        pos = 15;
      }
    }
    if (data[1] < 200) {
      digitalWrite(DIR_1, HIGH);
      digitalWrite(DIR_2, HIGH);
      analogWrite(SPEED_1, 220);
      // analogWrite(SPEED_2, 220);
     }
    else if (data[1] > 900) {
      digitalWrite(DIR_1, LOW);
      digitalWrite(DIR_2, LOW);
      analogWrite(SPEED_1, 255);
      analogWrite(SPEED_2, 255);
    }
    else {
      digitalWrite(DIR_1, HIGH);
      digitalWrite(DIR_2, LOW);
      analogWrite(SPEED_1, 0);
      analogWrite(SPEED_2, 0);      
    }
  }
}
