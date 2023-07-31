#include <LiquidCrystal.h>

//MOTOR_left
#define ena 11
#define inp1 12
#define inp2 13

//MOTOR_right
#define enb 10
#define inp3 8
#define inp4 9

#define abc 2 
#define en  3
#define d4  4
#define d5  5
#define d6  6
#define d7  7


// #define motor_difference_factor 6/5


LiquidCrystal lcd(abc, en ,d4,d5,d6,d7);

//Sensors latest readings
int Realtime_RightSensorReading;
int Realtime_CenterSensorReading;
int Realtime_LeftSensorReading;

//Sensors Previous Values
int Prev_RightSensorReading;
int Prev_LeftSensorReading;
int Prev_CenterSensorReading;

float SpeedDeltaValue = 0.0078125;
float COUNTER1=0;
float COUNTER =0;
float MaxSpeedLimit=210;
float VariableSpeed=MaxSpeedLimit;

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
  lcd.print("DL&WOI"); // Draw a Line & Walk On It
  lcd.setCursor(0,1);

  // Speed
  analogWrite(A5,128);
}

void loop()
{
  GetReadingsFromSensors();

  //Serial.begin(9600);
  //print_readings();

  // If readings change update the LCD
  if (!CheckReadingsSame()){
    switch(GetCaseNum()) {
      case 0 : {break;}// RETURNING BACK TO TRACK ****[[IMPORTANT]]****
      case 1 : {lcd.setCursor(0,1);lcd.print("RIGHT fast"); /*Serial.println("right");*/break;}
      case 2 : {lcd.setCursor(0,1);lcd.print("FORWARD");    /*Serial.println("FORWARD");*/break;}
      case 3 : {lcd.setCursor(0,1);lcd.print("RIGHT");      /*Serial.println("right");*/break;}
      case 4 : {lcd.setCursor(0,1);lcd.print("LEFT fast");  /*Serial.println("left");*/break;}
      case 5 : {break;}
      case 6 : {lcd.setCursor(0,1);lcd.print("LEFT");       /*Serial.println("left");*/break;}
      case 7 : {lcd.setCursor(0,1);lcd.print("FORWARD");    /*Serial.println("FORWARD");*/break;}
    }
  }
  switch(GetCaseNum()) {
    case 0 : {ReturnToTrack();break;}              // RETURNING BACK TO TRACK ****[[IMPORTANT]]****
    case 1 : {SaveReadings();TurnRightFast();break;}    // TURN FATSTER
    case 2 : {SaveReadings();MoveForward();break;}
    case 3 : {SaveReadings();TurnRightGradual();break;}
    case 4 : {SaveReadings();TurnLeftFast();break;}     // TURN FATSTER
    case 5 : {break;}                           // Impossible
    case 6 : {SaveReadings();TurnLeftGradual();break;}
    case 7 : {SaveReadings();MoveForward();break;}
  }

}

void MoveForward(){
  if (COUNTER>=0){
    COUNTER-=(SpeedDeltaValue);
    COUNTER1+=(SpeedDeltaValue);
  }

  VariableSpeed+=COUNTER1;

  if (VariableSpeed>MaxSpeedLimit) VariableSpeed=MaxSpeedLimit;

  if (VariableSpeed<0) VariableSpeed=0;

  ApplyTurnAction(VariableSpeed, VariableSpeed);
}

void TurnRightGradual(){
  if (VariableSpeed<=MaxSpeedLimit) COUNTER+=SpeedDeltaValue;

  VariableSpeed-= COUNTER1;

  if (VariableSpeed<0) VariableSpeed=0;

  ApplyTurnAction(MaxSpeedLimit,VariableSpeed);
}

void TurnLeftGradual(){
  if (VariableSpeed<=MaxSpeedLimit) COUNTER+=SpeedDeltaValue;

  VariableSpeed-= COUNTER1;

  if (VariableSpeed<0) VariableSpeed=0;

  ApplyTurnAction(VariableSpeed,MaxSpeedLimit);
}

void TurnRightFast(){
  ApplyTurnAction(MaxSpeedLimit,0);
  /*Serial.println("right fast");*/
}
void TurnLeftFast(){
  ApplyTurnAction(0, MaxSpeedLimit);
  /*Serial.println("left fast");*/
}

void ReturnToTrack() {
  //Serial.println("returning...");delay(100);
  //Serial.print("__");
  if      (Prev_RightSensorReading==1  && Prev_LeftSensorReading==0)
    TurnRightFast();
  else if (Prev_RightSensorReading==0  && Prev_LeftSensorReading==1) 
    TurnLeftFast();
  else if ((Prev_RightSensorReading==0  && Prev_LeftSensorReading==0) && Prev_CenterSensorReading==1) 
    MoveForward(); 
}


void ApplyTurnAction(float TurnLeftHigh, float TurnRightHigh) {
  digitalWrite(inp1,1);
  digitalWrite(inp2,0);
  analogWrite(ena,(int)TurnLeftHigh);

  digitalWrite(inp3,1);
  digitalWrite(inp4,0);
  analogWrite(enb,(int)TurnRightHigh);

  //              lcd.setCursor(0,1);
  //              lcd.print(TurnLeftHigh);
  //              lcd.setCursor(8,1);
  //              lcd.print(TurnRightHigh);
  //              delay(100);
}



bool CheckReadingsSame(){
  return (Prev_RightSensorReading  == Realtime_RightSensorReading )&&
         (Prev_LeftSensorReading   == Realtime_LeftSensorReading  )&&
         (Prev_CenterSensorReading == Realtime_CenterSensorReading);
}

void GetReadingsFromSensors(){
  Realtime_RightSensorReading  = digitalRead(A2);
  Realtime_CenterSensorReading = digitalRead(A1);
  Realtime_LeftSensorReading   = digitalRead(A0);
  /*print_readings();*/
}

void SaveReadings(){
  Prev_RightSensorReading  = Realtime_RightSensorReading;
  Prev_CenterSensorReading = Realtime_CenterSensorReading;
  Prev_LeftSensorReading   = Realtime_LeftSensorReading;
}
int GetCaseNum(){
  return (int)Realtime_LeftSensorReading  *4 +
         (int)Realtime_CenterSensorReading*2 +
         (int)Realtime_RightSensorReading;
}
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
