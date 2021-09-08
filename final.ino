#include <ArduinoJson.h>

// TEMPERATURE
#include "DHT.h"
#define DHTPIN 8 // DHT Pin
#define DHTTYPE DHT11 // We're using DHT 11
DHT dht(DHTPIN, DHTTYPE);


// ULTRASOUND DISTANCE SENSOR
#include <HCSR04.h>
// Initialize sensor that uses digital pins 13 and 12.
const byte triggerPin = 6;
const byte echoPin = 7;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// SERVO
#include <Servo.h>
Servo myservo;
int i = 0;

// Liquid Crystal
// include the library code:
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void printTemp(LiquidCrystal &lcd, DHT &dht)
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    lcd.print("DHT failed :(");
    return;
  }
  String temp = "T: " + String(t) + "* C";
  String hum = "H: " + String(h) + "%";
  lcd.setCursor(0, 0);
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print(hum);

  StaticJsonDocument<200> doc;
  doc["messageType"] = "message";
  doc["temp"] = t;
  doc["humidity"] = h;
  serializeJson(doc, Serial);
  Serial.println();

}


void setup () {
  i = 0;
  Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
  myservo.attach(13);
  pinMode(9, OUTPUT);
  dht.begin();
  lcd.begin(16, 2);
}


bool sentryMode = false;
int step = 5;
const int left_stop = 0;
const int right_stop = 180;


void checkCommand()
{
  if (Serial.available() > 0) {
    // read the incoming string:
    String incomingString = Serial.readString();

    if (incomingString == "sentryOn\n")
      sentryMode = true;
    if (incomingString == "sentryOff\n")
      sentryMode = false;
  }
}

void checkAlarm()
{
  float distance = distanceSensor.measureDistanceCm();
  bool alarmSent = false;

  while (distance > 0 && distance <= 20)
  {

    // Send alarm
    if (!alarmSent)
    {
      StaticJsonDocument<200> doc;
      doc["messageType"] = "alarm";
      serializeJson(doc, Serial);
      Serial.println();
      alarmSent = true;
    }


    digitalWrite(9, HIGH);
    delay(100);
    digitalWrite(9, LOW);
    delay(100);
    distance = distanceSensor.measureDistanceCm();
  }
}

void loop () {
  if (sentryMode)
    myservo.write(i);
  // Change direction
  if ((step > 0 && i >= right_stop) || (step < 0 && i <= left_stop))
  {
    step *= -1;
    delay(200);
    printTemp(lcd, dht);
    checkCommand();
  }

  i += step;

  if (sentryMode)
    checkAlarm();
  delay(50);

  //    delay(500);
}
