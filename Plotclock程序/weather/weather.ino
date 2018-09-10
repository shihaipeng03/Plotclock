// Plotclock
// cc - by Johannes Heberlein 2014
// v 1.02
// thingiverse.com/joo   wiki.fablab-nuernberg.de
// units: mm; microseconds; radians
// origin: bottom left of drawing surface
// time library see http://playground.arduino.cc/Code/time 
// RTC  library see http://playground.arduino.cc/Code/time 
//               or http://www.pjrc.com/teensy/td_libs_DS1307RTC.html  
// Change log:
// 1.01  Release by joo at https://github.com/9a/plotclock
// 1.02  Additional features implemented by Dave:
//       - added ability to calibrate servofaktor seperately for left and right servos
//       - added code to support DS1307, DS1337 and DS3231 real time clock chips
//       - see http://www.pjrc.com/teensy/td_libs_DS1307RTC.html for how to hook up the real time clock 

// delete or mark the next line as comment if you don't need these
//#define CALIBRATION      // enable calibration mode
//#define REALTIMECLOCK    // enable real time clock
#define WEATHER          // enable weather mode
static const String weatherlocation = "Palo%20Alto"; //Location for weather query

// When in calibration mode, adjust the following factor until the servos move exactly 90 degrees
#define SERVOFAKTORLEFT 590
#define SERVOFAKTORRIGHT 610

// Zero-position of left and right servo
// When in calibration mode, adjust the NULL-values so that the servo arms are at all times parallel
// either to the X or Y axis
#define SERVOLEFTNULL 1910
#define SERVORIGHTNULL 940

#define SERVOPINLIFT  2
#define SERVOPINLEFT  3
#define SERVOPINRIGHT 4

#define TIMEPIN 5
#define WEATHERPIN 6

// lift positions of lifting servo
#define LIFT0 1570 // on drawing surface
#define LIFT1 1400  // between numbers
#define LIFT2 1200  // going towards sweeper

// speed of liftimg arm, higher is slower
#define LIFTSPEED 1200

// length of arms
#define L1 35
#define L2 55.1
#define L3 13.2

// origin points of left and right servo 
#define O1X 22
#define O1Y -25
#define O2X 47
#define O2Y -25

#include <Time.h> // see http://playground.arduino.cc/Code/time 
#include <Servo.h>
#include <HttpClient.h>

#ifdef REALTIMECLOCK
// for instructions on how to hook up a real time clock,
// see here -> http://www.pjrc.com/teensy/td_libs_DS1307RTC.html
// DS1307RTC works with the DS1307, DS1337 and DS3231 real time clock chips.
// Please run the SetTime example to initialize the time on new RTC chips and begin running.

  #include <Wire.h>
  #include <DS1307RTC.h> // see http://playground.arduino.cc/Code/time    
#endif

int servoLift = 1500;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = 75;
volatile double lastY = 47.5;

int last_min = 0;

void setup() 
{
Serial.begin(9600);
Bridge.begin(); 
#ifdef REALTIMECLOCK

  // Set current time only the first to values, hh,mm are needed  
  tmElements_t tm;
  if (RTC.read(tm)) 
  {
    setTime(tm.Hour,tm.Minute,tm.Second,tm.Day,tm.Month,tm.Year);
    Serial.println("DS1307 time is set OK.");
  } 
  else 
  {
    if (RTC.chipPresent())
    {
      Serial.println("DS1307 is stopped.  Please run the SetTime example to initialize the time and begin running.");
    } 
    else 
    {
      Serial.println("DS1307 read error!  Please check the circuitry.");
    } 
    // Set current time only the first to values, hh,mm are needed
    setTime(19,38,0,0,0,0);
  }
#else  
  // Set current time only the first to values, hh,mm are needed
  setTime(16,18,0,0,0,0);
#endif

  drawTo(75.2, 47);
  lift(0);
  servo1.attach(SERVOPINLIFT);  //  lifting servo
  servo2.attach(SERVOPINLEFT);  //  left servo
  servo3.attach(SERVOPINRIGHT);  //  right servo

  pinMode(TIMEPIN, INPUT);
  pinMode(WEATHERPIN, INPUT);
  delay(1000);

} 

void loop() 
{ 
//***********************************************************
//Test without hardware

  String result = getWeather();
         int temp = getTemp(result);
         drawTo(74.2, 47.5);
         number(3, 3, 111, 1); //Sweep 
         temperature(temp);
         lift(2);
         
         int iconCode = getIcon(result);        
         int w = mapIconCode(iconCode);
         drawTo(74.2, 47.5);
         number(3, 3, 111, 1); //Sweep
         weather(w); 
         lift(2);
         
 //***********************************************************

#ifdef CALIBRATION

  // Servohorns will have 90° between movements, parallel to x and y axis
  drawTo(-3, 29.2);
  delay(500);
  drawTo(74.1, 28);
  delay(500);

#else 

  
  int i = 0;
 // 
 if ( last_min != minute()| digitalRead(TIMEPIN)!=HIGH | digitalRead(WEATHERPIN)!=HIGH ) {

    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    if(digitalRead(TIMEPIN)!=HIGH){
      lift(0);
    
      hour();
      while ((i+1)*10 <= hour())
      {
        i++;
      }
  
    
      number(3, 3, 111, 1); //Sweep  
      number(5, 25, i, 0.9);
      number(19, 25, (hour()-i*10), 0.9);
      number(28, 25, 11, 0.9);
    
      i=0;
      while ((i+1)*10 <= minute())
      {
        i++;
      }
      number(34, 25, i, 0.9);
      number(48, 25, (minute()-i*10), 0.9);
      lift(2);
      
    } else if(digitalRead(WEATHERPIN)!=HIGH){
      /*
        //Temperature test
        //int temp = random(451);
        drawTo(74.2, 47.5);
        number(3, 3, 111, 1); //Sweep 
        temperature(temp);
        lift(2);
        
        //Weather symbol test
        int w= random(3);
        drawTo(74.2, 47.5);
        number(3, 3, 111, 1); //Sweep
        weather(w); 
         lift(2);
*/
         String result = getWeather();
         int temp = getTemp(result);
         drawTo(74.2, 47.5);
         number(3, 3, 111, 1); //Sweep 
         temperature(temp);
         lift(2);
         
         int iconCode = getIcon(result);        
         int w = mapIconCode(iconCode);
         drawTo(74.2, 47.5);
         number(3, 3, 111, 1); //Sweep
         weather(w); 
         lift(2);
    }
    
    
   
    drawTo(74.2, 47.5);
    lift(1);
    last_min = minute();
  
    servo1.detach();
    servo2.detach();
    servo3.detach();
    }

#endif

} 

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {

  switch (num) {

  case 0:
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(0);
    bogenGZS(bx + 7 * scale, by + 10 * scale, 10 * scale, -0.8, 6.7, 0.5);
    lift(1);
    break;
  case 1:

    drawTo(bx + 3 * scale, by + 15 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(1);
    break;
  case 2:
    drawTo(bx + 2 * scale, by + 12 * scale);
    lift(0);
    bogenUZS(bx + 8 * scale, by + 14 * scale, 6 * scale, 3, -0.8, 1);
    drawTo(bx + 1 * scale, by + 0 * scale);
    drawTo(bx + 12 * scale, by + 0 * scale);
    lift(1);
    break;
  case 3:
    drawTo(bx + 2 * scale, by + 17 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 3, -2, 1);
    bogenUZS(bx + 5 * scale, by + 5 * scale, 5 * scale, 1.57, -3, 1);
    lift(1);
    break;
  case 4:
    drawTo(bx + 10 * scale, by + 0 * scale);
    lift(0);
    drawTo(bx + 10 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 6 * scale);
    drawTo(bx + 12 * scale, by + 6 * scale);
    lift(1);
    break;
  case 5:
    drawTo(bx + 2 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 6 * scale, 6 * scale, -2.5, 2, 1);
    drawTo(bx + 5 * scale, by + 20 * scale);
    drawTo(bx + 12 * scale, by + 20 * scale);
    lift(1);
    break;
  case 6:
    drawTo(bx + 2 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 6 * scale, 6 * scale, 2, -4.4, 1);
    drawTo(bx + 11 * scale, by + 20 * scale);
    lift(1);
    break;
  case 7:
    drawTo(bx + 2 * scale, by + 20 * scale);
    lift(0);
    drawTo(bx + 12 * scale, by + 20 * scale);
    drawTo(bx + 2 * scale, by + 0);
    lift(1);
    break;
  case 8:
    drawTo(bx + 5 * scale, by + 10 * scale);
    lift(0);
    bogenUZS(bx + 5 * scale, by + 15 * scale, 5 * scale, 4.7, -1.6, 1);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 5 * scale, -4.7, 2, 1);
    lift(1);
    break;

  case 9:
    drawTo(bx + 9 * scale, by + 11 * scale);
    lift(0);
    bogenUZS(bx + 7 * scale, by + 15 * scale, 5 * scale, 4, -0.5, 1);
    drawTo(bx + 5 * scale, by + 0);
    lift(1);
    break;

  case 111: //Sweep

    lift(0);
    drawTo(70, 46);
    drawTo(65, 43);

    drawTo(65, 49);
    drawTo(5, 49);
    drawTo(5, 45);
    drawTo(65, 45);
    drawTo(65, 40);

    drawTo(5, 40);
    drawTo(5, 35);
    drawTo(65, 35);
    drawTo(65, 30);

    drawTo(5, 30);
    drawTo(5, 25);
    drawTo(65, 25);
    drawTo(65, 20);

    drawTo(5, 20);
    drawTo(60, 44);

    drawTo(75.2, 47);
    lift(2);

    break;

  case 11: //Two dots
    drawTo(bx + 5 * scale, by + 15 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;

     case 12: //One dot
    drawTo(bx + 5 * scale, by + 0 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 0 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
   
    break;

  }
}

void weather(int w){
  Serial.println(w);
  int bx = 5;
  int by = 25;
  switch(w){
    case 0:
    //Sun
    bx +=30;
    drawTo(bx + 5, by + 6);
    lift(0);
    bogenGZS(bx, by + 10, 8, -2, 6.7, 1); //Kreis
    //Strahlen waagerecht
    lift(1);
    drawTo(bx-16, by+10);
    lift(0);
    drawTo(bx-8, by+10);
    lift(1);
    drawTo(bx+8,by+10);
    lift(0);
    drawTo(bx+16, by+10);

     //Strahlen senkrecht
    lift(1);
    drawTo(bx, by+26);
    lift(0);
    drawTo(bx, by+18);
    lift(1);
    drawTo(bx,by+2);
    lift(0);
    drawTo(bx, by-6);

    //Strahlen diagonal
    lift(1);
    drawTo(bx-11.34, by+21.34);
    lift(0);
    drawTo(bx-5.67, by+15.67);
    lift(1);
    drawTo(bx+5.67,by+4.33);
    lift(0);
    drawTo(bx+11.34, by-1.34);

    //Strahlen diagonal
    lift(1);
    drawTo(bx+11.34, by+21.34);
    lift(0);
    drawTo(bx+5.67, by+15.67);
    lift(1);
    drawTo(bx-5.67,by+4.33);
    lift(0);
    drawTo(bx-11.34, by-1.34);
    break;

    case 1:
    //Cloudy
    drawTo(bx + 9,by + 2);
    lift(0);
    drawTo(bx + 50,by + 2);
    bogenGZS(bx + 51, by + 5, 5, -2, 1, 1); //Todo: Radius, Start, Ende testen
    bogenGZS(bx + 43, by + 12, 7, -2, 1, 1);
    bogenGZS(bx + 29, by + 17, 7, -2, 1, 1);
    bogenGZS(bx + 13, by + 15, 6, -2, 1, 1);
    bogenGZS(bx + 6, by + 6, 5, -2, 1, 1);
    lift(1);
    break;

    case 2:
    //Rain
    bx +=23;
    drawTo(bx +0, by + 20);
    lift(0);
    bogenGZS(bx + 1, by + 17, 0.1, 1, -1, 1);
    lift(1);
    drawTo(bx + 0, by + 10);
    lift(0);
    bogenGZS(bx + 1, by + 7, 0.1, 1, -1, 1);
    lift(1);
    
    drawTo(bx + 5, by + 15);
    lift(0);
    bogenGZS(bx + 6, by + 12, 0.1, 1, -1, 1);
    lift(1);
    drawTo(bx + 5, by + 5);
    lift(0);
    bogenGZS(bx + 6, by + 2, 0.1, 1, -1, 1);
    lift(1);

     drawTo(bx + 10, by + 20);
    lift(0);
    bogenGZS(bx + 11, by + 17, 0.1, 1, -1, 1);
    lift(1);
    drawTo(bx + 10, by + 10);
    lift(0);
    bogenGZS(bx + 11, by + 7, 0.1, 1, -1, 1);
    lift(1);

     drawTo(bx + 15, by + 15);
    lift(0);
    bogenGZS(bx + 16, by + 12, 0.1, 1, -1, 1);
    lift(1);
    drawTo(bx + 15, by + 5);
    lift(0);
    bogenGZS(bx + 16, by + 2, 0.1, 1, -1, 1);
    lift(1);
    break;
  }
}
//int temp is the temperature*10 in celsius, e.g. 14.6 C is 146
void temperature(int temp){
  //Temperature Digit 1
  int i=0;
  while ((i+1)*100 <= temp)
      {
        i++;
      } 
  number(5, 25, i, 0.9);
  //Temperature Digit 2
  temp=temp-i*100;
  i=0;
  
  while ((i+1)*10 <= temp)
  {
    i++;
  } 
  number(19, 25, i, 0.9);
  //One dot
  number(28, 25, 12, 1); 
  //Temperature Digit 3
  temp=temp-i*10;
  number(34, 25, i, 0.9);

  //Degree Circle
  int bx =48;
  int by = 25;
  drawTo(bx,by+17);
  lift(0);
  bogenUZS(bx,by+20,3,-2,6,1);
  lift(1);
/*
  //C for Celsius
  drawTo(bx+10,by+15);
  lift(0);
  bogenGZS(bx+5,by+5,7.5,-4,-0.5,0.5);
  lift(1);
*/
 //F for Fahrenheit
  int bx =50;
  int by = 25;
  drawTo(bx, by+15);
  lift(0);
  drawTo(bx,by);
  lift(1);
  drawTo(bx, by+15);
  lift(0);
  drawTo(bx+5,by+15);
  lift(1);
  drawTo(bx,by+10);
  lift(0);
  drawTo(bx+5,by+10);
  lift(1);
}

void lift(char lift) {
  switch (lift) {
    // room to optimize  !

  case 0: //850

      if (servoLift >= LIFT0) {
      while (servoLift >= LIFT0) 
      {
        servoLift--;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT0) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }

    }

    break;

  case 1: //150

    if (servoLift >= LIFT1) {
      while (servoLift >= LIFT1) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);

      }
    } 
    else {
      while (servoLift <= LIFT1) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }

    }

    break;

  case 2:

    if (servoLift >= LIFT2) {
      while (servoLift >= LIFT2) {
        servoLift--;
        servo1.writeMicroseconds(servoLift);
        delayMicroseconds(LIFTSPEED);
      }
    } 
    else {
      while (servoLift <= LIFT2) {
        servoLift++;
        servo1.writeMicroseconds(servoLift);        
        delayMicroseconds(LIFTSPEED);
      }
    }
    break;
  }
}


void bogenUZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = -0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) > ende);

}

//bx,by = Mittelpunkt, Start, Ende = Kreisauschnitt , 20 inkr. für 1 ; sqee= Rund(1) Oval(0.5)
void bogenGZS(float bx, float by, float radius, int start, int ende, float sqee) {
  float inkr = 0.05;
  float count = 0;

  do {
    drawTo(sqee * radius * cos(start + count) + bx,
    radius * sin(start + count) + by);
    count += inkr;
  } 
  while ((start + count) <= ende);
}


void drawTo(double pX, double pY) {
  double dx, dy, c;
  int i;

  // dx dy of new point
  dx = pX - lastX;
  dy = pY - lastY;
  //path lenght in mm, times 4 equals 4 steps per mm
  c = floor(4 * sqrt(dx * dx + dy * dy));

  if (c < 1) c = 1;

  for (i = 0; i <= c; i++) {
    // draw line point by point
    set_XY(lastX + (i * dx / c), lastY + (i * dy / c));

  }

  lastX = pX;
  lastY = pY;
}

double return_angle(double a, double b, double c) {
  // cosine rule for angle between c and a
  return acos((a * a + c * c - b * b) / (2 * a * c));
}

void set_XY(double Tx, double Ty) 
{
  delay(1);
  double dx, dy, c, a1, a2, Hx, Hy;

  // calculate triangle between pen, servoLeft and arm joint
  // cartesian dx/dy
  dx = Tx - O1X;
  dy = Ty - O1Y;

  // polar lemgth (c) and angle (a1)
  c = sqrt(dx * dx + dy * dy); // 
  a1 = atan2(dy, dx); //
  a2 = return_angle(L1, L2, c);

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTORLEFT) + SERVOLEFTNULL));

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5°
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}

String getWeather(){
  HttpClient client;
  String result;
  //units=imperial means °Fahrenheit, units=metric means °Celsius
  String url = "http://api.openweathermap.org/data/2.5/weather?q={"+weatherlocation+"}&mode=json&units=imperial";
  
  Serial.println("Getting data");
  client.get(url);
    while(client.available()>0){
      char c = client.read();
      Serial.print(c);
      if(c!=-1){
      result += c;
      } else {
        Serial.println(result);
        client.close();
        client.flush();
        return result;
      }
    }
}

//extract the temperature value from the JSON result from openweathermap.org
//and return it as integer value
int getTemp(String weathermap){
  
  int tempIndexFrom = weathermap.indexOf("temp\":") + 6;
  int tempIndexTo = weathermap.indexOf(',',tempIndexFrom + 1);
  String temp =  weathermap.substring(tempIndexFrom, tempIndexTo);
  Serial.println("Temp= " + temp);

  //Convert to int without comma, round up or down
  String tempString;
  for(int i=0;i<temp.length();i++){
    if(isDigit(temp.charAt(i))){
      tempString += temp.charAt(i);
    }
  }
  Serial.println((tempString.toInt()+5)/10);
  return (tempString.toInt()+5)/10; 
  //returns the temperature * 10 with 1 rounded digit after comma
}

//extract the icon code from the JSON result from openweathermap.org
//and return it as integer value
int getIcon(String weathermap){
  int iconIndexFrom = weathermap.indexOf("icon\":") + 7 ;
  int iconIndexTo = weathermap.indexOf('}',iconIndexFrom + 1) - 1;
  String icon =  weathermap.substring(iconIndexFrom, iconIndexTo);
  Serial.println("Icon= "+icon);
  return icon.toInt();
}

int mapIconCode(int code){
  if(code < 3){ //openweather 01,02 are sunny
    return 0; //0:draws a sun
  } else if (code < 9 && code > 2){ //openweather 03,04 are cloudy
    return 1; //1:draws a cloud
  } else if (code < 50 && code > 04){ //openweather 90,10,11,12 are rain(and snow)
    return 2; //2:draws rain
  } else if (code == 50){ //openweather 50 is fog
    //TODO draw foggy waves
    return 1;
  }
}




