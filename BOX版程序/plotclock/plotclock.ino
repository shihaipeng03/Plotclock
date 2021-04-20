//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2021/03/05
//Plotclock 小贱钟
//本程序对应商品 https://item.taobao.com/item.htm?spm=a1z10.3-c.w4002-1266050966.63.5c807027U2Y1rh&id=641165909231

//web版程序连接：https://create.arduino.cc/editor/wjd76/abcc4e7f-508f-45e2-8284-db8d60c20181/preview
//Github版链接：https://github.com/shihaipeng03/Plotclock


// 调试完毕后可以删除以下两行代码，或者注释掉 调试模式，改为正常运行模式
#define CALIBRATION      // 调试模式（调试摆臂的位置）删除或注释掉此行进入 正常运行模式（正常书写时间）



//↓↓↓↓↓↓↓  以下参数需要配合舵机摆臂运行位置进行修正，否则写字可能是歪的，甚至无法正常写字   ↓↓↓↓↓↓↓


//1.先调节0、180度的位置。调节到位后，再调节90度位置
//左右悬臂舵机的 0或180度位置,，数字增加，左侧舵机逆时针调整，右侧舵机顺时针调整
//【此数值可能需要调节】
#define SERVOLEFTNULL 2000  //数值减小，顺时针旋转，加大则逆时针旋转 默认2000
//【此数值可能需要调节】
#define SERVORIGHTNULL 1000  //数值减小，顺时针旋转，加大则逆时针旋转  默认1000

//2.调节到位0、180，再调节下面参数
//左右悬臂舵机的90度位置,，数字增加，左侧舵机顺时针调整，右侧舵机逆时针调整
//【此数值可能需要调节】
#define SERVOFAKTORLEFT 650 //数值加大，顺时针旋转，减小则逆时针旋转 默认650
//【此数值可能需要调节】
#define SERVOFAKTORRIGHT 650  //数值减小，顺时针旋转，加大则逆时针旋转 默认650


//升举舵机的3个角度 
//【此数值可能需要调节】
#define LIFT0 1350 //落笔写字 
#define LIFT1 1250  //写字时抬臂动作 一般比lift0 小100左右
#define LIFT2 1100  //高抬笔架  
//【此数值可能需要调节】



//笔擦的坐标位置，摆臂调节好后，如不能对准笔擦可以微调此参数，单位毫米
int  rubberx=82,rubbery=48;   //【此数值可能需要调节】

//三只舵机的接口号 舵机棕色线是GND=G
#define SERVOPINLIFT  2 //抬笔舵机
#define SERVOPINLEFT  3 //左臂舵机
#define SERVOPINRIGHT 4 //右臂舵机

//速度  数字越大越稳“贱” 太小了容易抖动 单位微妙 1000微妙=1毫秒
#define LIFTSPEED 2000 

//摆臂的长度，根据图纸测量，无需改变
#define L1 35
#define L2 57.1
#define L3 14

//左右舵机轴心的位置，根据图纸测量，无需改变
#define O1X 28.7 //box版 28.7，普通版22
#define O1Y -25
#define O2X 53.9 //box版53.9， 普通版47
#define O2Y -25


#include <Servo.h>

int servoLift = LIFT2;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = rubberx;
volatile double lastY = rubbery;

int last_min = 0;
long reference_hour,reference_minutes,reference_sec; //起始的基准时间

void setup() 
{ 
  //设置起始时间，（小时，分钟，秒）
  settime(12,34,0);  //每次通电后从12:34 开始计时

  
  servo1.attach(SERVOPINLIFT);  //初始化抬笔舵机  lifting servo
  servo2.attach(SERVOPINLEFT);  //初始化左臂舵机  left servo
  servo3.attach(SERVOPINRIGHT); //初始化右臂舵机  right servo
  
  lift(2);
  delay(300);
  drawTo(rubberx,rubbery); //将笔对准笔擦位置，准备擦画板
  
 } 

void loop() 
{ 

int i = 0;
#ifdef CALIBRATION  //条件编译，当定义了CALIBRATION 则编译下面99~117行 调节摆臂的代码，否则编译更下面正常运行的代码。
 
  //舵机摆在90°的区域内往复运动，调节舵机参数让摆臂平行于x轴和y轴
   
  drawTo(6.1, 30.7); //box版
  delay(1000);
   
  drawTo(83.3, 26.7); //让笔尖运行到此位置，正常情况两摆臂将垂直或平行于xy轴，根据运行情况，调节舵机参数让摆臂接近垂直或水平状态，详细操作见说明文档
  delay(1000);     //*/

 /* //如需调节抬臂高度，可以去掉注释，编译运行下面代码。
  //每个角度停留1秒钟，观察角度位置，调整LIFT0~2 三个参数
  lift(2);//高抬笔架
  delay(1000);
  lift(1);//写字时抬臂动作
  delay(1000);
  lift(0);//落笔写字 让摆臂龙门架刚好垂直，此位置最佳
  delay(1000);
  //*/

#else 

  //下面代码任选一行，进行不同模式的书写
  //if (last_min != minute())  //每分钟书写一次时间
  //if  (1)                  //反复不间断的擦写模式
  
  if (1) //如需修改请复制上面代码
  {
      /*//在串口显示当前时间，调试用可删除
      Serial.print(get_hour());
      Serial.print(':');
      Serial.print(get_minute());
      Serial.print(':');
      Serial.println(get_seconds());
      //*/

      
    if (!servo1.attached()) servo1.attach(SERVOPINLIFT);
    if (!servo2.attached()) servo2.attach(SERVOPINLEFT);
    if (!servo3.attached()) servo3.attach(SERVOPINRIGHT);

    lift(0);

    get_hour();
    while ((i+1)*10 <= get_hour())
    {
      i++;
    }

    number(3, 3, 111, 1);
    number(8, 25, i, 0.9);
    number(22, 25, (get_hour()-i*10), 0.9);
    number(33, 25, 11, 0.9);

    i=0;
    while ((i+1)*10 <= get_minute())
    {
      i++;
    }
    number(40, 25, i, 0.9);
    number(54, 25, (get_minute()-i*10), 0.9);
    lift(2);
    drawTo(rubberx, rubbery);
    lift(1);
    last_min = get_minute();

    servo1.detach();
    servo2.detach();
    servo3.detach();
  }

#endif
} 

//设置起始时间（小时，分钟，秒）
void settime(int ih,int im,int is) {
  reference_hour=ih % 24; //求余数 防止溢出
  reference_minutes=im % 60;
  reference_sec = is % 60;
}

//获得修正后的小时数
 long get_hour(){
   long sec = get_seconds();
   long ho = sec / 3600;
  ho = ho % 24;  //24或12小时制
  return(ho);
}

//获得修正后的分钟数
 long get_minute(){
    long sec = get_seconds();
    long mi = sec / 60 ;
    mi = mi % 60;
 return( mi) ;
}

//获得Arduino的启动时长 + 修正时间
 long  get_seconds(){
   long se = millis()/1000; 
  se = se +reference_sec + reference_minutes*60 + reference_hour*3600; 
 return(se);
}

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
//这里是写字的函数，如果你觉得字写的丑，可以自行修改字体
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
    //擦除画板，如有边缘未能擦到可以修改下面的位置参数
    lift(0);
    delay(50);
    drawTo(rubberx, rubbery);
    drawTo(70, rubbery);
    drawTo(62, 45);
    drawTo(7, 45);
    drawTo(7, 41);
    drawTo(62, 41);
    drawTo(62, 37);
    drawTo(7, 37);
    drawTo(7, 33);
    drawTo(62, 33);
    drawTo(62, 29);
    drawTo(7, 29);
    drawTo(7, 25);
    drawTo(62, 25);
    drawTo(62, 20);
    drawTo(7, 20);
    drawTo(7, rubbery);
    drawTo(58, rubbery);
    drawTo(rubberx + 3 , rubbery);
    lift(2);

    break;

  case 11:
  //打冒号
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


//抬笔函数，调整笔的高度 0，落笔写字； 1，书写时抬笔； 2，太高笔用于插板擦
void lift(char lift) {
  switch (lift) {
    // room to optimize  !

  case 0: 

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

  case 1: 

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


//运笔至坐标位置
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

  // calculate joinr arm point for triangle of the right servo arm
  a2 = return_angle(L2, L1, c);
  Hx = Tx + L3 * cos((a1 - a2 + 0.45937) + M_PI); //36,5掳  更正 原版是 35.5°，不是 36,5掳，新版修改后是26.32°
  Hy = Ty + L3 * sin((a1 - a2 + 0.45937) + M_PI); //感谢武汉网友 自由华莱士 的鼎力帮助，特此鸣谢

  // calculate triangle between pen joint, servoRight and arm joint
  dx = Hx - O2X;
  dy = Hy - O2Y;

  c = sqrt(dx * dx + dy * dy);
  a1 = atan2(dy, dx);
  a2 = return_angle(L1, (L2 - L3), c);

   servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTORRIGHT) + SERVORIGHTNULL));

}
