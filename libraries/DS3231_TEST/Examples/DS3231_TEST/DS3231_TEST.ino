/*
DS3231_test.pde
Eric Ayars
4/11

Test/demo of read routines for a DS3231 RTC.

Turn on the serial monitor after loading this to check if things are
working as they should.

*/

#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
byte ADay, AHour, AMinute, ASecond, ABits;
bool ADy, A12h, Apm;

byte year, month, date, DoW, hour, minute, second;

void setup() {
	// Start the I2C interface
	Wire.begin();
        Clock.setSecond(50);//Set the second 
        Clock.setMinute(59);//Set the minute 
        Clock.setHour(11);  //Set the hour 
        Clock.setDoW(5);    //Set the day of the week
        Clock.setDate(31);  //Set the date of the month
        Clock.setMonth(5);  //Set the month of the year
        Clock.setYear(13);  //Set the year (Last two digits of the year)
	// Start the serial interface
	Serial.begin(9600);
}
void ReadDS3231()
{
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}
void loop() {ReadDS3231();delay(1000);
	// send what's going on to the serial monitor.
	// Start with the year
/*	Serial.print("2");
	if (Century) {			// Won't need this for 89 years.
		Serial.print("1");
	} else {
		Serial.print("0");
	}
	Serial.print(Clock.getYear(), DEC);
	Serial.print('-');
	// then the month
	Serial.print(Clock.getMonth(Century), DEC);
	Serial.print('-');
	// then the date
	Serial.print(Clock.getDate(), DEC);
	Serial.print(' ');*/
	// and the day of the week
	/*Serial.print(Clock.getDoW(), DEC);
	Serial.print(' ');*/
	// Finally the hour, minute, and second
	/*Serial.print(Clock.getHour(h12, PM), DEC);
	Serial.print(':');
	Serial.print(Clock.getMinute(), DEC);
	Serial.print(':');
	Serial.print(Clock.getSecond(), DEC);
	// Add AM/PM indicator
	if (h12) {
		if (PM) {
			Serial.print(" PM ");
		} else {
			Serial.print(" AM ");
		}
	} else {
		Serial.print(" 24h ");
	}
	// Display the temperature
	Serial.print("T=");
	Serial.print(Clock.getTemperature(), 2);
	// Tell whether the time is (likely to be) valid
	if (Clock.oscillatorCheck()) {
		Serial.print(" O+");
	} else {
		Serial.print(" O-");
	}*/
	// Indicate whether an alarm went off
	/*if (Clock.checkIfAlarm(1)) {
		Serial.print(" A1!");
	}
	if (Clock.checkIfAlarm(2)) {
		Serial.print(" A2!");
	}*/
	// New line on display
	//Serial.print('\n');
       // delay(1000);
	// Display Alarm 1 information
/*	Serial.print("Alarm 1: ");
	Clock.getA1Time(ADay, AHour, AMinute, ASecond, ABits, ADy, A12h, Apm);
	Serial.print(ADay, DEC);
	if (ADy) {
		Serial.print(" DoW");
	} else {
		Serial.print(" Date");
	}
	Serial.print(' ');
	Serial.print(AHour, DEC);
	Serial.print(' ');
	Serial.print(AMinute, DEC);
	Serial.print(' ');
	Serial.print(ASecond, DEC);
	Serial.print(' ');
	if (A12h) {
		if (Apm) {
			Serial.print('pm ');
		} else {
			Serial.print('am ');
		}
	}
	if (Clock.checkAlarmEnabled(1)) {
		Serial.print("enabled");
	}
	Serial.print('\n');
	// Display Alarm 2 information
	Serial.print("Alarm 2: ");
	Clock.getA2Time(ADay, AHour, AMinute, ABits, ADy, A12h, Apm);
	Serial.print(ADay, DEC);
	if (ADy) {
		Serial.print(" DoW");
	} else {
		Serial.print(" Date");
	}
	Serial.print(' ');
	Serial.print(AHour, DEC);
	Serial.print(' ');
	Serial.print(AMinute, DEC);
	Serial.print(' ');
	if (A12h) {
		if (Apm) {
			Serial.print('pm');
		} else {
			Serial.print('am');
		}
	}
	if (Clock.checkAlarmEnabled(2)) {
		Serial.print("enabled");
	}*/
	/* display alarm bits
	Serial.print('\n');
	Serial.print('Alarm bits: ');
	Serial.print(ABits, DEC);
	*/
/*
	Serial.print('\n');
	Serial.print('\n');
	delay(1000);

	// Display the time once more as a test of the getTime() function
	Clock.getTime(year, month, date, DoW, hour, minute, second);
	
        Serial.print(year, DEC);
        Serial.print("/");
	Serial.print(month, DEC);
        Serial.print("/");
	Serial.print(date, DEC);
        Serial.print("day of the week :");
	Serial.println(DoW, DEC);
	Serial.print(hour, DEC);
        Serial.print(":");
	Serial.print(minute, DEC);
        Serial.print(":");
	Serial.println(second, DEC);*/
}

