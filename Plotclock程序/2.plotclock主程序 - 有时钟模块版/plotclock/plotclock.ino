//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2019/07/13

//视频教程：小贱钟的安装程序 https://www.ixigua.com/i6713939566358692356/
//最新制作了web版编译器的使用教程视频，还有更多视频内容正在上传请关注我们的西瓜视频 https://www.ixigua.com/i6713192503262052871/

//https://create.arduino.cc/editor/wjd76/30f046de-337f-45ae-ba1e-f39a50179be3/preview 
//最新增加了一个可以写字母的程序，高手可以研究。

//如果需要运行调试摆臂的程序，打开此代码，如果需要运行主程序，请注释掉下一行代码↓↓↓↓↓↓↓
//#define CALIBRATION 



//左右悬臂舵机的 0或180度位置,，数字增加，左侧舵机逆时针调整，右侧舵机顺时针调整
//【此数值可能需要调节】
#define SERVOLEFTNULL 2000  //数值减小，顺时针旋转，加大则逆时针旋转
//【此数值可能需要调节】
#define SERVORIGHTNULL 1020  //数值减小，顺时针旋转，加大则逆时针旋转

//2.调节到位0、180，再调节下面参数
//左右悬臂舵机的90度位置,，数字增加，左侧舵机顺时针调整，右侧舵机逆时针调整
//【此数值可能需要调节】
#define SERVOFAKTORLEFT 670 //数值加大，顺时针旋转，减小则逆时针旋转
//【此数值可能需要调节】
#define SERVOFAKTORRIGHT 680  //数值减小，顺时针旋转，加大则逆时针旋转


//升举舵机的3个角度 
//【此数值可能需要调节】
#define LIFT0 1450 //落笔写字 on drawing surface
#define LIFT1 1350  //写字时抬臂动作 between numbers 
#define LIFT2 1200  //高抬笔架  going towards sweeper 
//【此数值可能需要调节】


//
//以上参数，请运行调试程序 plotclockadj ，调整好位置后，将数据复制过来
//

//笔擦的坐标位置，如不能对准笔擦可以微调单位毫米
int  rubberx=72,rubbery=48;   //【此数值可能需要调节】

//三只舵机的接口号
#define SERVOPINLIFT  2 //抬臂舵机
#define SERVOPINLEFT  3 //左臂舵机
#define SERVOPINRIGHT 4 //右臂舵机

// 速度  数字越小越慢，太快了容易抖 1000~2000
#define LIFTSPEED 1000 

// 悬臂的长度，根据图纸测量，无需改变
#define L1 35
#define L2 57.2
#define L3 14.2

// 左右舵机轴心的位置，根据图纸测量，无需改变
#define O1X 22
#define O1Y -25
#define O2X 47
#define O2Y -25

//需要的库函数
#include <Servo.h>
#include <DS3231.h>
#include <Wire.h>

//时钟模块，如果没有时钟模块，请用另一个程序
DS3231 Clock;  
bool h12;
bool PM;



int servoLift = LIFT2;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = rubberx;
volatile double lastY = rubbery;

int last_min = 0;

void setup() 
{ 
  Wire.begin();
  //第一次运行 请设置时间，设置成功后请删除此行代码,或者如下格式注释掉
 Clock.setMinute(5);//设置分钟 
  Clock.setHour(9);  //设置小时 
 // Clock.setMinute(48);  
//  Clock.setHour(18);
  //第一次运行 请设置时间，设置成功后请删除此行代码
  
  servo1.attach(SERVOPINLIFT);  //初始化抬臂舵机  lifting servo
  servo2.attach(SERVOPINLEFT);  //初始化左臂舵机  left servo
  servo3.attach(SERVOPINRIGHT); //初始化右臂舵机  right servo
  lift(2); //抬笔
  drawTo(rubberx, rubbery); //停留在笔擦位置
  delay(1000);

} 

void loop() 
{ 
int minute,hour; 
minute=Clock.getMinute();
hour=Clock.getHour(h12, PM);//读取时间
  
int i = 0;
  //下面代码任选一行，进行不同模式的书写
  //if (last_min != minute())  //每分钟书写一次时间
  //if  (1)                  //反复不间断的擦写模式
  
  if (1)  //如需修改请复制上面代码
  {

    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    lift(0);

    
    while ((i+1)*10 <= hour)
    {
      i++;
    }

    number(3, 3, 111, 1);
    number(5, 25, i, 0.9);
    number(19, 25, (hour-i*10), 0.9);
    number(28, 25, 11, 0.9);

    i=0;
    while ((i+1)*10 <= minute)
    {
      i++;
    }
    number(34, 25, i, 0.9);
    number(48, 25, (minute-i*10), 0.9);
    lift(2);
    drawTo(rubberx, rubbery);
    lift(1);
    last_min = minute;

    servo1.detach();
    servo2.detach();
    servo3.detach();
  }


} 

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
//这里是写字的函数，如果你觉得字写的丑，可以自行修改字体，结果可能是更丑
//还可以自行增加其他内容，比如字母甚至是汉字
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

  case 111:

    lift(0);
    drawTo(rubberx, rubbery);
    drawTo(58, 42);

    drawTo(58, 45);
    drawTo(2, 45);
    drawTo(2, 41);
    drawTo(58, 41);
    drawTo(60, 37);

    drawTo(2, 37);
    drawTo(2, 33);
    drawTo(60, 33);
    drawTo(60, 29);

    drawTo(2, 29);
    drawTo(2, 25);
    drawTo(60, 25);
    drawTo(60, 20);

    drawTo(2, 20);
    drawTo(60, rubbery);

    drawTo(rubberx + 1 , rubbery);
    lift(2);

    break;

  case 11:
    drawTo(bx + 5 * scale, by + 15 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 15 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    drawTo(bx + 5 * scale, by + 5 * scale);
    lift(0);
    bogenGZS(bx + 5 * scale, by + 5 * scale, 0.1 * scale, 1, -1, 1);
    lift(1);
    break;

  }
}


//抬臂函数，不同的摆臂高度
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

//里面似乎有勾股定理？
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

//cos？sin？
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

//用各种三角函数把位置坐标换算成舵机的角度，具体咋算的，请参考
//Plotclock by joo - Thingiverse
//http://www.thingiverse.com/thing:248009/
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
    delayMicroseconds(LIFTSPEED);
  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.621) + M_PI); //36,5掳
  Hy = Ty + L3 * sin((a1 - a2 + 0.621) + M_PI);

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

    servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));
    delayMicroseconds(LIFTSPEED);
}
