//version to test TMP device
//TMP: thanks to http://www.instructables.com/id/How-to-use-the-TMP36-temp-sensor-Arduino-Tutorial/
//LCD: thanks to http://arduinoairpollution.altervista.org/tutorial-arduino-mkr1000-display-lcd-hd44780/
//LCD: thanks to https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll & https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll

//Libraries & files
#include <LiquidCrystal.h>

//TMP Setup
const int temperaturePin = A4; // Pin A4 and a great number
const int targetPin = 17; // Pin A2
float voltage, degreesC, degreesF, targetTempC, targetTempF;

//LCD Setup
LiquidCrystal lcd(0, 1, 2, 3, 4, 5); // initialize the library with the numbers of the MKR Pin: ( From D0 to D5 )

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2); //16 spaces across 2 rows
}


void loop()
{
  getTemp();
  getTarget();
  lcdTicker();
  delay(1000); // repeat once per second (change as you wish!)
}

void getTemp() {
  voltage = analogRead(temperaturePin) * 0.004882814; //voltage mapping from documentation
  degreesC = (voltage - 0.5) * 100.0; // temperature mapping from documentation
  //more realistic voltage mapping for now - backed out from breath temp (est 98) vs room temp (est 70)
//  degreesC = (voltage - 1.1) * 416;
  degreesF = degreesC * (9.0 / 5.0) + 32.0; //TEAM FAHRENHEIT
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);
}

void getTarget() {
  //make a target range of between 100F and 60F - TEAM FAHRENHEIT
  targetTempC = map(analogRead(targetPin), 0, 1023, 37, 15);
  targetTempF = targetTempC * (9.0 / 5.0) + 32.0;
}

void lcdTicker() {
  lcd.setCursor(0, 0); // top left
  lcd.print("Temp Now: ");
  lcd.print(degreesF);
  lcd.print(" F");
  lcd.setCursor(0, 1); // bottom left
  lcd.print("Tgt Temp: ");
  lcd.print(targetTempF);
  lcd.print(" F");
  //scrolling TBD based on full functionality
}

