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

// LCD
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Read temperature from DHT, display on screen 
void printTemp()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    lcd.print("DHT failed :(");
    return;
  }

  // Format output strings
  String temp = "T: " + String(t) + "* C";
  String hum = "H: " + String(h) + "%";

  // Output strings on LCD
  lcd.setCursor(0, 0);
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print(hum);

  // Print measurements on the serial ouptut
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

// True if alarm is activated
bool sentryMode = false;

// Degrees per distance measurements
int step = 5;

// Leftmost degree for turret
const int left_stop = 0;

// Rightmost degree for turret
const int right_stop = 180;

// Check if a command to stop/start sentry mode has been received on serial
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

// Check if an object has approached the sensor, and raise alarm if so
void checkAlarm()
{
  float distance = distanceSensor.measureDistanceCm();

  // Make sure we send alarm only once
  bool alarmSent = false;

  while (distance > 0 && distance <= 20)
  {

    // Send alarm
    if (!alarmSent)
    {
      // Send alarm to serial via JSON
      StaticJsonDocument<200> doc;
      doc["messageType"] = "alarm";
      serializeJson(doc, Serial);
      Serial.println();
      alarmSent = true;
    }

    // Blink red LED to scare the attacker
    digitalWrite(9, HIGH);
    delay(100);
    digitalWrite(9, LOW);
    delay(100);

	// Renew measurement
    distance = distanceSensor.measureDistanceCm();
  }
}

void loop () {
  // Turn to a computed degree
  if (sentryMode)
    myservo.write(i);
  
  // If we've reached left or right stop
  if ((step > 0 && i >= right_stop) || (step < 0 && i <= left_stop))
  {
    // Switch direction
    step *= -1;

    delay(200);
    printTemp(lcd, dht);
    checkCommand();
  }

  // Increment degree
  i += step;

  if (sentryMode)
    checkAlarm();

  delay(50);
}
