//淘宝『有名称的店铺』更新时间 20190413


//↓↓↓↓↓↓↓  调试成功后，将以下代码复制到主程序   ↓↓↓↓↓↓↓
// When in calibration mode, adjust the following factor until the servos move exactly 90 degrees
#define SERVOFAKTOR 620


//1.先调节0、180度的位置。调节到位后，再调节90度位置
//左右悬臂舵机的 0或180度位置,，数字增加，左侧舵机逆时针调整，右侧舵机顺时针调整
//【此数值可能需要调节】
#define SERVOLEFTNULL 2050  //数值减小，顺时针旋转，加大则逆时针旋转  // 2050
//【此数值可能需要调节】
#define SERVORIGHTNULL 1080  //数值减小，顺时针旋转，加大则逆时针旋转 //1050

//2.调节到位0、180，再调节下面参数
//左右悬臂舵机的90度位置,，数字增加，左侧舵机顺时针调整，右侧舵机逆时针调整
//【此数值可能需要调节】
#define SERVOFAKTORLEFT 650 //数值加大，顺时针旋转，减小则逆时针旋转//630
//【此数值可能需要调节】
#define SERVOFAKTORRIGHT 605  //数值减小，顺时针旋转，加大则逆时针旋转//605


//升举舵机的3个角度 
//【此数值可能需要调节】
#define LIFT0 1500 //落笔写字 on drawing surface
#define LIFT1 1600  //写字时抬臂动作 between numbers
#define LIFT2 1730  //高抬笔架  going towards sweeper 
//【此数值可能需要调节】
//
//↑↑↑↑↑↑↑↑↑↑↑↑调试成功后，将以上代码复制到主程序 ↑↑↑↑↑↑↑↑↑↑↑↑
//以上参数，请运行调试程序 plotclock ，调整好位置后，将数据复制过来
//
//笔擦的坐标位置，如不能对准笔擦可以微调单位毫米
int  rubberx=72,rubbery=48;   //【此数值可能需要调节】

//三只舵机的接口号
#define SERVOPINLIFT  2 //抬臂舵机
#define SERVOPINLEFT  3 //左臂舵机
#define SERVOPINRIGHT 4 //右臂舵机

//抬笔速度  数字越小越快，太快了容易抖 500~5000
#define LIFTSPEED 1500 

//摆臂的长度，根据图纸测量，无需改变
#define L1 35
#define L2 57.2
#define L3 14.2

// 左右舵机轴心的位置，根据图纸测量，无需改变
#define O1X 22
#define O1Y -25
#define O2X 47
#define O2Y -25

//#include "ziku.h"
#include <Servo.h>

int servoLift = 1500;

Servo servo1;  // 
Servo servo2;  // 
Servo servo3;  // 

volatile double lastX = 72;
volatile double lastY = 48;

int last_min = 0;

void setup() 
{ 

  // 设置一个模拟时间，（小时，分钟，后面全填0）
  //如果此句编译错误，请将文件包内的libraries库放到Arduino文件夹内，具体操作办法见说明文档
  servo1.attach(SERVOPINLIFT);  //初始化抬臂舵机  lifting servo
  servo2.attach(SERVOPINLEFT);  //初始化左臂舵机  left servo
  servo3.attach(SERVOPINRIGHT); //初始化右臂舵机  right servo
  lift(2); //高抬笔
  drawTo(rubberx, rubbery); //停留在笔擦位置
  delay(1000);
} 

void loop() 
{ 
  lift(1); //抬笔
  number(28, 25, 999, 1); //写国字
  delay(5000);  
  while(1);
} 

// Writing numeral with bx by being the bottom left originpoint. Scale 1 equals a 20 mm high font.
// The structure follows this principle: move to first startpoint of the numeral, lift down, draw numeral, lift up
void number(float bx, float by, int num, float scale) {
  
  //阿拉伯数字
  char Num0[36] = "12,6,t,gzs,7,10,10,-0.8, 6.7, 0.5,u";
  char Num1[20] = "3,15,t,10,20,10,0,u";
  char Num2[36] = "2,12,t,uzs,8,14,6,3,-0.8,1,1,0,12,0";
  char Num3[]="2,17,t,uzs,5,15,5,3, -2, 1,uzs,5,5,5,1.57, -3, 1,u";
  char Num4[]="10,0,t,10,20,2,6,12,6,u";
  char Num5[]="2,5,t,gzs,5,6,6,-2.5,2,1,5,20,12,20,u";
  char Num6[]="2,10,t,uzs,7,6,6,2,-4.4,1,11,20,u";
  char Num7[]="2,20,t,12,20,2,0,u";
  char Num8[]="5,10,t,uzs,5,15,5,4.7, -1.6, 1,gzs,5,5,5, -4.7, 2, 1,u";
  char Num9[]="9,11,t,uzs,7,15,5,4, -0.5, 1,5,0,u";
  
  //液晶格式的数字
  char LC_Num0[]="2,18,t,2,2,10,2,10,18,2,18,u";  
  char LC_Num1[]="10,18,t,10,2,u";
  char LC_Num2[]="2,18,t,10,18,10,9,2,9,2,2,10,2,u";
  char LC_Num3[]="2,18,t,10,18,10,2,2,2,u,2,9,t,10,9,u";
  char LC_Num4[]="2,18,t,2,9,10,9,u,10,18,t,10,2,u";
  char LC_Num5[]="10,18,t,2,18,2,9,10,9,10,2,2,2,u";
  char LC_Num6[]="10,18,t,2,18,2,2,10,2,10,9,2,9,u";
  char LC_Num7[]="2,18,t,10,18,10,2,u";
  char LC_Num8[]="2,18,t,2,2,10,2,10,18,2,18,u,2,9,t,10,9,u";
  char LC_Num9[]="10,9,t,2,9,2,18,10,18,10,2,2,2,u";
  
  //英文字母大写
  char C_A[]="2,4,t,7,18,12,4,u,4,10,t,10,10,u";
  char C_B[]="0,2,t,0,18,uzs,2,18,5,2,-2,1.3,uzs,2,8,5,2,-2,1.3,u";
  char C_C[]="12,16,t,gzs,7,10,10,1,6,0.6,u";
  char C_D[]="0,2,t,0,18,uzs,2,18,10,2,-2,0.5,u";
  char C_E[]="12,18,t,2,18,2,2,12,2,u,2,10,t,10,10,u";
  char C_F[]="12,18,t,2,18,2,2,u,2,10,t,10,10,u";
  char C_G[]="";
  char C_H[]="2,18,t,2,2,u,12,18,t,12,2,u,2,9,t,12,9,u";
  char C_I[]="5,18,t,10,18,u,8,18,t,8,2,u,5,2,t,10,2,u";
  char C_J[]="";
  char C_K[]="2,18,t,2,2,u,11,18,t,2,9,u,3,10,t,14,3,u";
  char C_L[]="2,18,t,2,2,12,2,u";
  char C_M[]="0,2,t,4,18,7,2,8,18,13,2,u";
  char C_N[]="2,2,t,2,18,12,2,12,18,u";
  char C_O[]="12,6,t,gzs,7,10,10,-0.8, 6.7, 0.5,u";
  char C_P[]="0,2,t,0,18,uzs,2,18,5,2,-2,1.3,u";
  char C_Q[]="12,6,t,gzs,7,10,10,-0.8, 6.7, 0.5,u,10,5,t,14,1,u";
  char C_R[]="0,2,t,0,18,uzs,2,18,5,2,-2,1.3,12,2,u";
  char C_S[]="";
  char C_T[]="2,18,t,12,18,u,7,18,t,7,2,u";
  char C_U[]="";
  char C_V[]="2,18,t,6,2,12,18,u";
  char C_W[]="0,18,t,4,2,7,18,8,2,14,18,u";
  char C_X[]="2,18,t,12,2,u,12,18,t,2,2,u";
  char C_Y[]="2,18,t,7,10,7,2,u,7,10,t,12,18,u";
  char C_Z[]="2,18,t,12,18,2,2,12,2,u";
    
  //汉字
  char guo[]="1,3,t,12,3,12,18,1,18,1,3,u,3,14,t,10,14,u,7,14,t,7,7,u,5,10,t,10,10,u,3,7,t,11,7,u";//国
    
  switch (num) {
  case 0:
    drawStr(Num0,bx,by,1);
    break;
  case 1:
    drawStr(Num1,bx,by,1);
    break;
  case 2:
     drawStr(Num2,bx,by,1);
     break;
  case 3:
    drawStr(Num3,bx,by,1);
    break;
  case 4:
    drawStr(Num4,bx,by,1);
    break;
  case 5:
    drawStr(Num5,bx,by,1);
    break;
  case 6:
    drawStr(Num6,bx,by,1);
    break;
  case 7:
    drawStr(Num7,bx,by,1);
    break;
  case 8:
    drawStr(Num8,bx,by,1);
    break;
  case 9:
    drawStr(Num9,bx,by,1);
    break;
  case 111:
  {
    //定义笔擦的大小（X,Y两个方向，如果是圆形X=Y）
    int EraserX=5;
    int EraserY=5;
    //定义画板的大小
    int DrawboardWidth=70;
    int DrawboardHigh=55;
    //定义笔擦的位置
    int EraserPostionX=75;
    int EraserPostionY=47;
    
    int a=EraserX*2;
    int b=EraserY*2;
    
    lift(0);
    //移出，笔擦
    drawTo(EraserPostionX-a,EraserPostionY);
    //移到画板右上角
     drawTo(DrawboardWidth-a,DrawboardHigh-EraserY);
     
    for (int i=DrawboardHigh-EraserY; i >=EraserY+20 ; i=i-EraserY*2)  //10 是距离底边的距离
    {
         drawTo(EraserX,i); 
         drawTo(EraserX,i-EraserY); 
         drawTo(DrawboardWidth-a,i-EraserY);
         drawTo(DrawboardWidth-a,i-EraserY*2);
    }
    //移动到刚才移出笔擦的位置 
    drawTo(EraserPostionX-a,EraserPostionY);
    drawTo(EraserPostionX,EraserPostionY);
    lift(2);
    delay(1000);
    break;
  }
  case 11: //时间的中间分隔符：
  {
    char fenggefu[]="5,15,t,gzs,5,15,0.1,1,-1,1,u,5,5,t,gzs,5,5,0.1,1,-1,1,u";
    drawStr(fenggefu,bx,by,1);
    break;
  }
  case 999:
  {
    drawStr(C_H,bx,by,1);
    delay(2000);
    /*drawStr(C_E,bx,by,1);
    delay(1000);
    drawStr(C_F,bx,by,1);
    delay(1000);
    drawStr(C_H,bx,by,1);
    delay(1000);
    drawStr(C_I,bx,by,1);
    delay(1000);
    drawStr(C_K,bx,by,1);
    delay(1000);
    drawStr(C_L,bx,by,1);
    delay(1000);
    drawStr(C_M,bx,by,1);
    delay(1000);
    drawStr(C_N,bx,by,1);
    delay(1000);
    drawStr(C_T,bx,by,1);
    delay(1000);
    drawStr(C_V,bx,by,1);
    delay(1000);
    drawStr(C_W,bx,by,1);
    delay(1000);
    drawStr(C_X,bx,by,1);
    delay(1000);
    drawStr(C_Y,bx,by,1);
    delay(1000);
    drawStr(C_Z,bx,by,1);
      */
    break;
  }
  }
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

  servo2.writeMicroseconds(floor(((a2 + a1 - M_PI) * SERVOFAKTOR) + SERVOLEFTNULL));

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

  servo3.writeMicroseconds(floor(((a1 - a2) * SERVOFAKTOR) + SERVORIGHTNULL));

}



/*
字库编辑方式：
以字的左下角为原点标记出各个点的坐标。
DRAW TO命令 需要紧跟两个参数 TX,TY
GZS和UZS需要紧跟6个参数 tx ty radius start ende sqee
碰到画板 T 提笔 U 高于板擦位置 H
国字库：char guo[]="1,3,t,12,3,12,18,1,18,1,3,u,3,14,t,10,14,u,7,14,t,7,7,u,5,10,t,10,10,u,3,7,t,11,7,u";
*/

void drawStr(char *str,float bx, float by, float scale) {   //从字库中书写文字  bx,by是指文字的左下角原点
    float tx;
    float ty;
    float radius;
    int start;
    int ende;
    float sqee;
    
    char *p;
    p = strtok(str, ","); //从传过来的字符串中读取指定的代码或数据 
    
   while (p) {  
     String s(p);  //把char* 转换成STRING
     if (s == "t")  //放笔碰到画板
      {
        //调用lift(0)
        lift(0);
        // Serial.println("thoch broad");
      }
       
    else if (s == "u") //抬笔离开画板
      {
            //调用lift(1)
            lift(1);
           // Serial.println("up board");  
       }
       
     else if (s=="h")  //提高到高于板擦
       {
         //调用 lift(2)
         lift(2);
         //Serial.println("HIGE OVER ERASE");    
       }
       
     else if (s=="gzs" || s=="uzs" )    //GZS命令 一共要取出6个参数
       {
         p = strtok(NULL, ",");    //读取下一个数据
         tx = atof(p);  //取X坐标
         p = strtok(NULL, ",");    //读取下一个数据
         ty = atof(p);   //取Y坐标
         p = strtok(NULL, ",");    //读取下一个数据
         radius =atof(p);
         p = strtok(NULL, ",");    //读取下一个数据
         start =atof(p);
         p = strtok(NULL, ",");    //读取下一个数据
         ende = atof(p);
         p = strtok(NULL, ",");    //读取下一个数据
         sqee = atof(p);
         
         if (s=="gzs"){   //调用GZS
           //Serial.println("gzs"); 
           bogenGZS(bx + tx * scale, by + ty * scale, radius * scale, start, ende, sqee);
         }
         else {    //调用 UZS
           //Serial.println("uzs");  
           bogenUZS(bx + tx * scale, by + ty * scale, radius * scale, start, ende, sqee);
         }
         
       }   
     else     //DRAW TO 命令
       {
         tx = atoi(p);  //取X坐标
         p = strtok(NULL, ",");    //读取下一个数据
         ty = atoi(p);   //取Y坐标
         
         //Serial.println("draw to");
         //调用DRAW TO函数
         drawTo(bx + tx * scale, by + ty * scale);
       }
       
     p = strtok(NULL, ",");  //读取下一个数据
     //Serial.println(p);
    }
}
