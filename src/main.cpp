#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <esp8266httpclient.h>
#include "secrets.h"

int redLed = 5; //D1(gopi5)
bool isRedLedOn = false;

int greenLed = 14; //D5(gpio14)
bool isGreenLedOn = false;

int button = 4; //D2(gpio4)
int buttonState = HIGH;
bool isPushed = false;
int i = 0;
bool isConnectionNotifiactionPrinted;
unsigned long previousMillis = 0;

void turnOnLed(int ledPin, bool* isLedOn) {
  digitalWrite(ledPin, HIGH);
  *isLedOn = true;
}

void turnOffLed(int ledPin, bool* isLedOn) {
  digitalWrite(ledPin, LOW);
  *isLedOn = false;
}

void toggleLed(int ledPin, bool* isLedOn) {
  if (isLedOn) {
    return turnOffLed(ledPin, isLedOn);
  } else {
    return turnOnLed(ledPin, isLedOn);
  }
}

void checkInitialWifiConnection() {
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    isConnectionNotifiactionPrinted = false;
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      Serial.print(++i); Serial.print(' ');

      toggleLed(redLed, &isRedLedOn);
    }
  } else {
    if (!isConnectionNotifiactionPrinted) {
      turnOffLed(redLed, &isRedLedOn);

      isConnectionNotifiactionPrinted = true;

      Serial.println('\n');
      Serial.println("Connection established!");  
      Serial.print("IP address:\t");
      Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
      i = 0;

      for (int i = 0; i<3; i++) {
        if (i > 0) { // so that we don't delay in the beginning and end unnecessarily
          delay(200);
        }
        turnOnLed(greenLed, &isGreenLedOn);
        delay(200);
        turnOffLed(greenLed, &isGreenLedOn);
      }
    }
  }
}

IRAM_ATTR void handleButtonPushInterrupt() {
  isPushed = true;
}

HTTPClient http;
WiFiClient wifiClient;

void onButtonPush() {
  Serial.println(WiFi.status());
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  delay(200); // debounce to prevent double push
  isPushed = false;
  turnOnLed(greenLed, &isGreenLedOn);
  Serial.print("It's pushed: "); Serial.println(++i);
  
  http.begin(wifiClient, url);  //Specify request destination http://host:8000/toggle
  int httpCode = http.POST(""); 
  String payload = http.getString();

  if (httpCode != 200) {
    turnOnLed(redLed, &isGreenLedOn);
  } else {
    turnOffLed(redLed, &isGreenLedOn);
  }

  Serial.print("Got response: "); Serial.print(httpCode); Serial.print(" "); Serial.println(payload);

  http.end();
  turnOffLed(greenLed, &isGreenLedOn);
}

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button), handleButtonPushInterrupt, FALLING);

  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");

  Serial.print(ssid); Serial.println(" ...");
}

void loop() {
  checkInitialWifiConnection();

  if (isPushed && isConnectionNotifiactionPrinted) {
    onButtonPush();
  }
}