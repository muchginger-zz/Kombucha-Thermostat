/* 
 *****************************************************
 * Arduino Kombucha Thermostat
 * Created by: Aris Bloms III
 * 
 * A fully automated brewing process powered 
 * by an Arduino. This code uses a DS18B20 
 * Temperature sensor, a 16x2 LCD Display for
 * the user, and a LED/Buzzer for warning the user.
 * Instructions found at {INSERT LINK HERE}
 * 
 * The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * 
 * LED + pin to digital pin 8
 * Buzzer + pin to digital pin 9
 * DS18B20 Temperature Sensor data to digital pin 13
 * Relay data pin to digital pin 7
 * 
 **********************************************************
 * LiquidCrystal Library originally added 18 Apr 2008
 * by David A. Mellis
 * library modified 5 Jul 2009
 * by Limor Fried (http://www.ladyada.net)
 * example added 9 Jul 2009
 * by Tom Igoe
 * modified 22 Nov 2010
 * by Tom Igoe
 * modified 7 Nov 2016
 * by Arturo Guadalupi
 **********************************************************
 * OneWire Library originally added in 2007 by
 * Jim Studt.
 * Library v2.0 merged Robin James && Paul Stoffregen's
 * additions.
 * Library v2.1 added support from Guillermo Lovato, 
 * Jason Dangel, Glenn Trewitt, and Paul Stroffregen.
 **********************************************************
 *
 * Sources: https://playground.arduino.cc/Learning/OneWire
 *          https://www.arduino.cc/en/Tutorial/HelloWorld
 *          
 **********************************************************
 ************************************
 ** This work is licensed under a  **
 ** Creative Commons Attribution   **
 ** 4.0 International License.     **
 ************************************
 **********************************************************
 */
#include <OneWire.h>
#include <LiquidCrystal.h>
#include <DallasTemperature.h>

#define OneWireBus 13
// UNCOMMENT THIS IF YOU WANT THE LED AND/OR BUZZER
//const int ledPin = 8;
//const int buzzerPin = 9;
const int heaterPin = 7;

OneWire oneWire(OneWireBus); // DS18B20 on digital pin 13
DallasTemperature sensors(&oneWire);
 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // Start LCD (16x2 is the size of the LCD)
  lcd.begin(16,2);

  // Initialize the outputs...
  //pinMode(ledPin, OUTPUT);
  //pinMode(buzzerPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  // Turn off heater. (My relay is hooked up to the NC, 
  //  so it's LOW to turn on, and HIGH to turn off.)
  digitalWrite(heaterPin, HIGH);
/*
  //TESTING
  Serial.begin(9600);
*/
}

void loop() {
  lcd.clear();
  // Get Temperatures...
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  // Convert to Freedom Units...
  float tempF = (((tempC*9)/5)+32);
// If it doesnt work correctly, uncomment the Serial lines 
// and see what's going on.
/*  
  Serial.print(tempF);
  Serial.print('\n');
  Serial.print(tempC);
  Serial.print('\n');
*/
  // if the temperature is lower than 68 degrees F...
  if (tempF < 68) {
    // Print "WARMING..." and turn on heater...
    lcd.setCursor(0,0);
    lcd.print(tempF);
    lcd.print(" degrees F");
    lcd.setCursor(1,1);
    lcd.print("WARMING...");

    // Turn heater on
    digitalWrite(heaterPin, LOW);
/*
    // Turn on LED and Buzzer!
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin,HIGH);
    delay(1000);
    digitalWrite(buzzerPin,LOW);
    digitalWrite(ledPin, LOW);
    delay(1000);
*/
  }
  else if (tempF > 80) {
    // Print "Heater is off." and turn off heater.
    lcd.setCursor(0,0);
    lcd.print(tempF);
    lcd.print(" degrees F");
    lcd.setCursor(1,1);
    lcd.print("Heater is off.");

    // Turn heater off
    digitalWrite(heaterPin, HIGH);
  }
  else {
    // Print all is good.
    lcd.setCursor(0,0);
    lcd.print(tempF);
    lcd.print(" degrees F");
    lcd.setCursor(1,1);
    lcd.print("All is good.");
    
    // Don't mess with the heater, it'll 
    //  turn off in the second else if.
  }
  delay(1000);
}
