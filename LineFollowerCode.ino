

//MOTOR_left
#define ena 11
#define inp1 12
#define inp2 13

//MOTOR_right
#define enb 10
#define inp3 8
#define inp4 9

#include <LiquidCrystal.h>
#define abc 2 
#define en 3
#define d4 4
#define d5 5
#define d6 6
#define d7 7
#define motor_difference_factor 6/5
LiquidCrystal lcd(abc, en ,d4,d5,d6,d7);



//SENSORS
int RS;
int CS;
int LS;

//SAVES
int rs;
int ls;
int cs;

float add_sub = 0.0078125;
float COUNTER1=0;
float COUNTER =0;
float spee=210;
float speedio=spee;

void setup() {

  //MOTORS
  pinMode(ena,  OUTPUT);
  pinMode(inp1, OUTPUT);
  pinMode(inp2, OUTPUT);
  pinMode(enb,  OUTPUT);
  pinMode(inp3, OUTPUT);
  pinMode(inp4, OUTPUT);
  pinMode(A5   ,OUTPUT);

  //SENSORS
  pinMode(A0,INPUT);  //LEFT SENSOR
  pinMode(A1,INPUT);  //CENTER SENSOR
  pinMode(A2,INPUT);  //RIGHT SENSOR

  
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("DL&WOI");
  lcd.setCursor(0,1);
  analogWrite(A5,128);
}

void loop()
{
  check_readings();
  
  //Serial.begin(9600);
  //print_readings();

  if (!reading_same())
  {switch(case_number())
  {
   case 0 : {break;}// RETURNING BACK TO TRACK ****[[IMPORTANT]]****
   case 1 : {lcd.setCursor(0,1);lcd.print("RIGHT fast");/*Serial.println("right");*/break;}   //TURN FATSTER  لو هنتحكم في السرعة ،يعني لو مش هنتحكم هنمسحه
   case 2 : {lcd.setCursor(0,1);lcd.print("FORWARD   ");/*Serial.println("FORWARD");*/break;}   //will we need this????
   case 3 : {lcd.setCursor(0,1);lcd.print("RIGHT     ");/*Serial.println("right");*/break;}
   case 4 : {lcd.setCursor(0,1);lcd.print("LEFT fast ");/*Serial.println("left");*/break;}    //TURN FATSTER  لو هنتحكم في السرعة ،يعني لو مش هنتحكم هنمسحه
   case 5 : {break;}
   case 6 : {lcd.setCursor(0,1);lcd.print("LEFT      ");/*Serial.println("left");*/break;}
   case 7 : {lcd.setCursor(0,1);lcd.print("FORWARD   ");/*Serial.println("FORWARD");*/break;}
  }
  }
  switch(case_number())
  {
  case 0 : {ReturnBack();break;}      // RETURNING BACK TO TRACK ****[[IMPORTANT]]****
  case 1 : {save();TurnRightFast();break;}   //TURN FATSTER  لو هنتحكم في السرعة ،يعني لو مش هنتحكم هنمسحه
  case 2 : {save();MOTORSFORWARD();break;}   //will we need this????
  case 3 : {save();TurnRight();break;}
  case 4 : {save();TurnLeftFast();break;}    //TURN FATSTER  لو هنتحكم في السرعة ،يعني لو مش هنتحكم هنمسحه
  case 5 : {break;}
  case 6 : {save();TurnLeft();break;}
  case 7 : {save();MOTORSFORWARD();break;}
  }

}

void MOTORSFORWARD(){variable_speed_forward();}

void TurnRight(){variable_speed_turn_right();}
void TurnLeft(){variable_speed_turn_left();}
         
void TurnRightFast(){turnaction(spee,0);/*Serial.println("right fast");*/}
void TurnLeftFast() {turnaction(0, spee);/*Serial.println("left fast");*/}
      
void ReturnBack()
    {
           //Serial.println("returning...");delay(100);
           //Serial.print("__");
           if (rs==1  && ls==0) TurnRightFast();
      else if (rs==0  && ls==1) TurnLeftFast();
      else if ((rs==0  && ls==0) && cs==1) MOTORSFORWARD(); 
      }

int ANALOG_SPEED        (float speedo,float COUNTERo){return speedo-COUNTERo;}
int ANALOG_SPEED_FORWARD(float speedo,float COUNTERo){return speedo+COUNTERo;}

void variable_speed_forward()
{
          if (COUNTER>=0) {COUNTER-=(add_sub);COUNTER1+=(add_sub);}
          speedio= ANALOG_SPEED_FORWARD(speedio,COUNTER1);
          if (speedio>=spee)speedio=spee;
          if (speedio<=0) speedio=0;
          turnaction(speedio, speedio);
}

void variable_speed_turn_left()
{
              if (speedio<=spee) COUNTER+=add_sub;
              speedio= ANALOG_SPEED(speedio,COUNTER);
              if (speedio<0) {speedio=0;}
              turnaction(speedio,spee);
}
void variable_speed_turn_right()
{
              if (speedio<=spee) COUNTER+=add_sub;
              speedio= ANALOG_SPEED(speedio,COUNTER);
              if (speedio<0) {speedio=0;}
              turnaction(spee,speedio);
}

void turnaction(float TurnLeftHigh, float TurnRightHigh)
              {
              digitalWrite(inp1,1);
              digitalWrite(inp2,0);
              analogWrite(ena,TurnLeftHigh*motor_difference_factor);
              
              digitalWrite(inp3,1);
              digitalWrite(inp4,0);
              analogWrite(enb,TurnRightHigh);
              
//              lcd.setCursor(0,1);
//              lcd.print(TurnLeftHigh);
//              lcd.setCursor(8,1);
//              lcd.print(TurnRightHigh);
//              delay(100);
              }
bool reading_same(){return (((rs==RS)&& (ls==LS))&&(cs==CS));}

void check_readings(){RS = digitalRead(A2);CS = digitalRead(A1);LS = digitalRead(A0);/*print_readings();*/}

void save(){rs = RS;ls = LS;cs = CS;}
int case_number(){return ((int)LS*4 + (int)CS*2 + (int)RS);}
//void print_readings()
//{
//  Serial.print(LS);
//  Serial.print("__");
//  Serial.print(CS);
//  Serial.print("__");
//  Serial.print(RS);
//  Serial.print("__  ");
//  delay(100);
//}
