# Робот на ардуино

Робот на Arduino Leonardo с радиомодулем NRF24. Бот был протестирован на трассе и отлично ее прошел. Для лучшего сцепления с гладкой поверхностью на ходовые валки был нанесен термоклей вдоль оси движение в небольшом количестве.

![bot_1](https://github.com/gitzense/bot/assets/114235388/7a8126f7-8e7a-4fad-bdcd-471c5d8f2463)

В роботе использовалось:
- Motor Shield,
- Arduino Leonardo/Amperka Iskra Neo,
- NRF,
- NRF+,
- акумуляторы 18650 x2,
- DC motor x2,
- RGB LED лента (4 светодиода),
- servo 180 x2.



https://github.com/gitzense/bot/assets/114235388/fd15c5e6-877d-4c51-bf17-9566ea2b325a



Для контроллера использовалась плата Arduino Pro Micro. Джойствик отвечал за движение по оси X, потенциометр управлял сервоприводом для поворота, кнопка отвечала за движение сервопривода с режущей насадкой. 

![controller](https://github.com/gitzense/bot/assets/114235388/ffcce92f-f188-48de-ac5c-660ebebcb4e0)

В контроллере использовалось:
- Arduino Pro Micro,
- NRF,
- NRF+,
- повербанк,
- потенциометр,
- кнопка,
- джойстик.



https://github.com/gitzense/bot/assets/114235388/252602ba-0840-4733-84bb-086d04a6e93f




Использованные библиотеки:
- Servo by Michael Margolis (управление сервоприводами),
- Adafruit NeoPixel by Adafruit (настройка светодиодной ленты),
- RF24 by TMRh20 (управление радиомодулем).


1. Код для контроллера:

``` c++
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define VX_PIN A0
#define VY_PIN A1
#define BTN_PIN 2


RF24 radio(9, 10);
int data[3];

void setup() {
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
  data[0] = analogRead(VX_PIN);
  data[1] = analogRead(VY_PIN);
  data[2] = digitalRead(BTN_PIN);
 
  
  radio.write(&data, sizeof(data));
  delay(15);
}
```

2. Код для робота:

``` c++
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
  // put your setup code here, to run once:
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
```
