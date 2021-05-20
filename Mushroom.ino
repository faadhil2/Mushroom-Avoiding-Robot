#include <Ultrasonic.h>

Ultrasonic ultrasonicFR(13, 12);
Ultrasonic ultrasonicFL(7, 2);
Ultrasonic ultrasonicR(9,8);
Ultrasonic ultrasonicL(4,3);

int distanceR, distanceL, distanceFR, distanceFL, previousTurn;
int AA = 6;   //pins 5,6,10, 11 are the only pins that can output 
int AB = 5;   //an 'analog' signal, meaning that you can adjust the
int BA = 11;  //strength of the the signal (google 'post-width modulation')
int BB = 10;

int spd = 190;  //adjust the speed of the motors
                //you can input values 0-255, but I found that
                //128 is the lowest you can go to make the motors move

int frontTriggerDist = 15;    //min distance the sensors trigger
boolean infraredL, infraredR;

void setup()
{
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
  pinMode(BA, OUTPUT);
  pinMode(BB, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
  previousTurn = 0;
}

void loop() 
{
//  findDistances();
//  checkEdge();
//  detectFront();
  followLine();
}


void forward()
{
  analogWrite(AA, spd);
  analogWrite(AB, 0);
  analogWrite(BA, spd);
  analogWrite(BB, 0); 
}

void reverse()  
{
  analogWrite(AA, 0);
  analogWrite(AB, spd);
  analogWrite(BA, 0);
  analogWrite(BB, spd);
}

void turnLeft()
{
  analogWrite(AA, 0);
  analogWrite(AB, spd);
  analogWrite(BA, spd);
  analogWrite(BB, 0);
}

void  turnRight()
{
  analogWrite(AA, spd);
  analogWrite(AB, 0);
  analogWrite(BA, 0);
  analogWrite(BB, spd); 
}

void  halt()
{
  analogWrite(AA, 0);
  analogWrite(AB, 0);
  analogWrite(BA, 0);
  analogWrite(BB, 0);  
}

boolean frontDetects()
{
  return (((distanceFL < frontTriggerDist) && (distanceFL != 0)) && ((distanceFL < frontTriggerDist) && (distanceFL != 0)));
}

boolean frontLDetects()
{
  return ((distanceFL < frontTriggerDist) && (distanceFL != 0));
}

boolean frontRDetects()
{
  return ((distanceFR < frontTriggerDist) && (distanceFR != 0));
}

void findLine()
{
   infraredL = (digitalRead(A0) == LOW);
   infraredR = (digitalRead(A1) == LOW);
}

void followLine()
{
  findLine();
  
  if (infraredL && !infraredR)
  {
    turnLeft();
    previousTurn = 1;
    wait(100);
    while (infraredL && !infraredR)
    {
      wait(50);
      findLine();
    }
  }
  else if (infraredR && !infraredL)
  {
    turnRight();
    previousTurn = 2;
    wait(100);
    while (infraredR && !infraredL)
    {
      wait(50);
      findLine();
    }
  }
  else if (!infraredR && !infraredL)
  {
    if (previousTurn == 1)
    {
      reverse();
      wait(200);
      turnRight();
      wait(50);
      do
      {
        wait(50);
        findLine();
//        if (infraredR && infraredR)
//        {
//          break;
//        }
      } while (!(infraredL && !infraredR));
    }
    else if (previousTurn == 2)
    {
      reverse();
      wait(200);
      turnLeft();
      wait(50);
      do
      {
        wait(50);
        findLine();
//        if (infraredR && infraredL)
//        {
//          break;
//        }
      } while (!(infraredR && !infraredL));
    }
  }
  else
  {
    forward();
  }
}

boolean checkEdge()
{
  if (distanceL > 10)
  {  
      reverse();
      wait(500);
      turnRight();
      wait(1100);
  }
  else if (distanceR > 10)
  {
      reverse();
      wait(500);
      turnLeft();
      wait(1100);
  }
  else
  {
      forward();
  }
}

void detectFront()
{
  if (frontLDetects())
  {
    reverse();
    wait(300);
    turnRight();
    wait(1100);
  }
  else if (frontRDetects())
  {
    reverse();
    wait(300);
    turnLeft();
    wait(1100);
  }
}

void findDistances()
{
 distanceL = ultrasonicL.distanceRead(CM);
 distanceR = ultrasonicR.distanceRead(CM);
 distanceFL = ultrasonicFL.distanceRead(CM);
 distanceFR = ultrasonicFR.distanceRead(CM);
}

boolean edgeDetects()
{
 return (distanceL > 10) || (distanceR > 10);
}

void wait(int del)
{
      for (int i = 0; i < (del / 50); i++)
      {
        delay(50);
//        findDistances();
//        if (edgeDetects() || frontLDetects() || frontRDetects())
//        {
//          break;
//        }
      }
}

