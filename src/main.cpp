#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include "secrets.h"

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  
  // pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");

  // digitalWrite(LED_BUILTIN, HIGH);

  Serial.print(ssid); Serial.println(" ...");
}

bool isConnectionNotifiactionPrinted;
unsigned long previousMillis = 0;
int i = 0;

void loop() { 
  unsigned long currentMillis = millis();

  if (WiFi.status() != WL_CONNECTED) {
    isConnectionNotifiactionPrinted = false;
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      Serial.print(++i); Serial.print(' ');
    }
  } else {
    if (!isConnectionNotifiactionPrinted) {
      i = 0;
      isConnectionNotifiactionPrinted = true;

      Serial.println('\n');
      Serial.println("Connection established!");  
      Serial.print("IP address:\t");
      Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
    }
  }
}
