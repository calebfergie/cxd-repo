/*
  Thermostat by calebfergie

  based on the example sketch WifiSSLClient (Wifi101), and insight from:
  https://github.com/cvalenzuela/understanding-networks/blob/master/HTTPS.md (HTTPS aka SSL for MRK1000)
  https://forum.arduino.cc/index.php?topic=461471.0  (HTTP request)
  https://github.com/tigoe/SensorExamples/blob/master/TMP36/TMP36.ino (Temp sensor)
  http://www.instructables.com/id/How-to-use-the-TMP36-temp-sensor-Arduino-Tutorial/ (Temp sensor)
  http://arduinoairpollution.altervista.org/tutorial-arduino-mkr1000-display-lcd-hd44780/ (LCD)
  https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll & https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll (LCD)
  https://github.com/tigoe/Interval (Interval)
*/

#include <SPI.h>
#include <WiFi101.h>
#include <LiquidCrystal.h>
#include "Interval.h"

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
String MAC = SECRET_MAC;
String sessionK = SECRET_SESSIONK;
int keyIndex = 0;            // your network key Index number (needed only for WEP)

const int temperaturePin = A4; // Pin A4 (aka 19, a great number)
const int dialPin = A6;        // temperature dial potentiometer
int sensorValue = 0;        // temperature sensor variable
int dialValue = 0;

float voltage, temperature, degreesF;  // temperature sensor variables
int targetTempC, targetTempF; // more temperature variables

String PostData, dataString; // strings for post requests to add data

int status = WL_IDLE_STATUS;
char server[] = "www.connected-devices-itp.herokuapp.com";    // name address for Google (using DNS)

//WIFI Instance
WiFiSSLClient client;

//LCD Instance
LiquidCrystal lcd(0, 1, 2, 3, 4, 5); // initialize the library with the numbers of the MKR Pin: ( From D0 to D5 )

//Timer Instance
Interval newReading;
Interval lcdUpdate;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  lcd.begin(16, 2); //16 spaces across 2 rows   // Initialize the LCD board
  newReading.setInterval(sendTemp, 1770000);   // Initialize the data send interval timing (every 28.5 minutes)
  lcdUpdate.setInterval(lcdTicker, 250);       // Initialize the LCD interval timing (every quarter second)
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  // check and send the information once on startup
  checkTemp();
  delay(1);
  getTarget();
  lcdTicker();
  makeJSON();
  sendTemp();
}

void loop() {
  getTarget();
  delay(1);
  checkTemp();
  lcdUpdate.check();
  makeJSON();
  newReading.check();
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void checkTemp() {
  // read the value from the sensor:
  sensorValue = analogRead(A4);
  // convert the reading to millivolts:
  voltage = sensorValue *  (3.3 / 1024.0);
  // convert the millivolts to temperature celsius:
  temperature = ((voltage - 0.5) / 0.01) - 2; // adjusted to reflect reality with a -5
  degreesF = temperature * (9.0 / 5.0) + 32.0; //TEAM FAHRENHEIT
//  Serial.print("Current temperature: ");
//  Serial.println(temperature);
//  Serial.println();
}

void getTarget() {
  dialValue = analogRead(dialPin);
  targetTempF = map(dialValue, 0, 1023, 50, 100);
  targetTempC = (targetTempF * 5 / 9) + 32;
}

void lcdTicker() {
  lcd.setCursor(0, 0); // top left
  lcd.print("Temp Now: ");
  lcd.print(int(degreesF));
  lcd.print(" F");
  lcd.setCursor(0, 1); // bottom left
  lcd.print("Tgt Temp: ");
  lcd.print(targetTempF);
  lcd.print(" F   ");
}

void makeJSON() {
  // make the data sub-JSON
  dataString = "{\"temp\":\"";
  dataString += temperature;
  dataString += "\", \"targetTempC\":\"";
  dataString += targetTempC;
  dataString += "\", \"fahrenheit\":\"";
  dataString += degreesF;
  dataString += "\", \"targetTempF\":\"";
  dataString += targetTempF;
  dataString += "\"}";

  // form a JSON-formatted string:
  PostData = "{\"macAddress\":\"";
  PostData += MAC;
  PostData += "\", \"sessionKey\":\"";
  PostData += sessionK;
  PostData += "\", \"data\": ";
  PostData += dataString;
  PostData += "}";
}

void sendTemp() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 443)) {
    Serial.println("connected to server - trying to send:");
    Serial.println(PostData);
    // Make a HTTP request:

    //HEADER
    client.println("POST /add HTTP/1.1");
    client.println("Host: connected-devices-itp.herokuapp.com");
    //client.println("User-Agent: Arduino/1.1");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(PostData.length());// number of bytes in the payload
    client.println();// important need an empty line here

    //BODY
    client.println(PostData); // the payload
    client.println();
  }
}
