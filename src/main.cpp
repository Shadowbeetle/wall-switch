#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <esp8266httpclient.h>
#include "secrets.h"

// void setup() {
//   Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  
//   pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  
//   delay(10);
//   Serial.println('\n');
  
//   WiFi.begin(ssid, password);             // Connect to the network
//   Serial.print("Connecting to ");

//   Serial.print(ssid); Serial.println(" ...");
// }

// bool isConnectionNotifiactionPrinted;
// bool isLedOn = true;
// unsigned long previousMillis = 0;
// int i = 0;
// const uint8_t OFF = HIGH;
// const uint8_t ON = LOW;

// void loop() { 
//   unsigned long currentMillis = millis();

//   if (WiFi.status() != WL_CONNECTED) {
//     isConnectionNotifiactionPrinted = false;
//     if (currentMillis - previousMillis >= 1000) {
//       previousMillis = currentMillis;
//       Serial.print(++i); Serial.print(' ');

//       if (isLedOn) {
//         isLedOn = false;
//         digitalWrite(LED_BUILTIN, OFF);
//       } else {
//         isLedOn = true;
//         digitalWrite(LED_BUILTIN, ON);
//       }
//     }
//   } else {
//     if (!isConnectionNotifiactionPrinted) {
//       isLedOn = false;
//       digitalWrite(LED_BUILTIN, OFF);
//       i = 0;
//       isConnectionNotifiactionPrinted = true;

//       Serial.println('\n');
//       Serial.println("Connection established!");  
//       Serial.print("IP address:\t");
//       Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
//     }
//   }
// }

// int ledpin = 5; // D1(gpio5)
int button = 4; //D2(gpio4)
int buttonState = HIGH;
bool isPushed = false;
int i = 0;
bool isConnectionNotifiactionPrinted;
unsigned long previousMillis = 0;

void checkInitialWifiConnection() {
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    isConnectionNotifiactionPrinted = false;
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      Serial.print(++i); Serial.print(' ');

      // if (isLedOn) {
      //   isLedOn = false;
      //   digitalWrite(LED_BUILTIN, OFF);
      // } else {
      //   isLedOn = true;
      //   digitalWrite(LED_BUILTIN, ON);
      // }
    }
  } else {
    if (!isConnectionNotifiactionPrinted) {
      // isLedOn = false;
      // digitalWrite(LED_BUILTIN, OFF);
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
  // pinMode(ledpin, OUTPUT);
  // pinMode(button, INPUT);
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
  // buttonState = digitalRead(button); // put your main code here, to run repeatedly:
  // if (buttonState == LOW) {
  //   // digitalWrite(ledpin, HIGH); 
  //   Serial.println("button pushed");
  //   delay(200);
  // }
  // if (buttonState == HIGH) {
  //   // digitalWrite(ledpin, LOW); 
  //   Serial.println("button relased");
  //   delay(200);
  // }
}