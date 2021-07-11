#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <esp8266httpclient.h>
#include "secrets.h"

int redLed = 5; //D1(gopi5)
bool isRedLedOn = false;

int button = 4; //D2(gpio4)
int buttonState = HIGH;
bool isPushed = false;
int i = 0;
bool isConnectionNotifiactionPrinted;
unsigned long previousMillis = 0;

bool turnOnLed(int ledPin) {
  digitalWrite(ledPin, HIGH);
  return true;
}

bool turnOffLed(int ledPin) {
  digitalWrite(ledPin, LOW);
  return false;
}

bool toggleLed(int ledPin, bool isLedOn) {
  if (isLedOn) {
    return turnOffLed(ledPin);
  } else {
    return turnOnLed(ledPin);
  }
}

void checkInitialWifiConnection() {
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    isConnectionNotifiactionPrinted = false;
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      Serial.print(++i); Serial.print(' ');

      isRedLedOn = toggleLed(redLed, isRedLedOn);
    }
  } else {
    if (!isConnectionNotifiactionPrinted) {
      isRedLedOn = turnOffLed(redLed);

      isConnectionNotifiactionPrinted = true;

      Serial.println('\n');
      Serial.println("Connection established!");  
      Serial.print("IP address:\t");
      Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
      i = 0;
    }
  }
}

IRAM_ATTR void handleButtonPushInterrupt() {
  isPushed = true;
}

HTTPClient http;
WiFiClient wifiClient;

void onButtonPush() {
  delay(200); // debounce to prevent double push
  isPushed = false;
  Serial.print("It's pushed: "); Serial.println(++i);
  
  http.begin(wifiClient, "http://192.168.1.117:8000/toggle");  //Specify request destination
  int httpCode = http.POST(""); 
  String payload = http.getString();

  Serial.print("Got response: "); Serial.print(httpCode); Serial.println(payload);

  http.end();
}

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  pinMode(redLed, OUTPUT);
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