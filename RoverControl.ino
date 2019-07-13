#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define wifiID "pritomn7"
#define wifiPass "temppassword1"
#define IP "192.168.0.100"
#define TRIG_PIN A4 
#define ECHO_PIN A5 
#define MAX_DISTANCE 200

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 
SoftwareSerial esp(12,13); //RX = 12, TX = 13;
Servo myservo;

//define output pins
const int motor1PWM = 11;
const int motor1Pin1 = 7;
const int motor1Pin2 = 8;
const int motor2PWM = 5;
const int motor2Pin1 = 2;
const int motor2Pin2 = 4;
const int servoPWM = 3;
const int motorDelay = 200;
const int servoDelay = 1000;
const int minDist = 15;
//define motor speed
int motorSpeed = 60; //motor speed (PWM)
int adj = 10.5;

//define variables
boolean goesForward=false;
int distance = 100;
int totalDist = 0;
String dir = "f";

void setup() {
  // put your setup code here, to run once:
  
  //set pin modes
  pinMode(motor1PWM, OUTPUT);
  pinMode(motor2PWM, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(servoPWM, OUTPUT);
  
  myservo.attach(servoPWM);
  Serial.begin(9600);
  esp.begin(9600);

  //connect wifi router  
  while(!connectWiFi()); 

  myservo.write(92);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  //Serial.println("initial Distance: "+distance);
}

void loop() {
  // put your main code here, to run repeatedly:
 totalDist += distance;
 int distanceF = 0;
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 //Serial.println(distance);
 if(distance<=30){
  moveStop();
  delay(100);
  moveBackward();
  //delay(300);
  moveStop();
  delay(200);
  Serial.print(totalDist);
  Serial.println(","+dir);
  sendData(); //sends data to the base station
  totalDist = 0; //reinitiating total distance
  dir = "f"; // reinitiating dir
  
  distanceF = lookForward();
  Serial.print("forward: ");
  Serial.println(distanceF);
  delay(200);
  
  distanceR = lookRight();
  Serial.print("Right: ");
  Serial.println(distanceR);
  delay(200);
  
  distanceL = lookLeft();
  Serial.print("left: ");
  Serial.println(distanceL);
  delay(200);
  
  if(distanceR > distanceF && distanceR >= distanceL){
    turnRight();
    moveStop();
    dir = "r";
    Serial.println("turned Right.");
  }
  else if(distanceL > distanceF && distanceL >= distanceR){
    turnLeft();
    moveStop();
    dir = "l";
    Serial.println("turned Left.");
  }
  else {
    dir = "f";
    Serial.println("forward");
  }
 }
 else moveForward();
 
 distance = readPing();
 
}


/*NECESSARY FUNCTIONS*/

//sets up connection with the internet via the wifimodule
boolean connectWiFi(){
  Serial.println("Connecting wi-fi...");
  String cmd ="AT+CWMODE=1";
  esp.println(cmd);
  delay(2000);
  esp.flush(); //clear buffer
  cmd="AT+CWJAP=\"";
  cmd+=wifiID;
  cmd+="\",\"";
  cmd+=wifiPass;
  cmd+="\"";
  esp.println(cmd);
  delay(2000);
  if(esp.find("OK")){
    Serial.println("Connection succeeded!");
    return true;
  }else{
    Serial.println("Connection failed!");
    return false;
  }
  Serial.println();
}

void sendData(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",6789";
  esp.println(cmd);
  delay(2000);
  
  String data = String(totalDist);
  data += ",";
  data += dir;
  data += "\r\n";
  delay(1000);
  int strsize = data.length();
  esp.println("AT+CIPSEND=" + String(strsize));
  delay(2000);
  esp.print(data);
 // delay(2000);
  //Serial.println(cmd);
  if(esp.find("OK")){
   Serial.println("Transmission completed with success: "+data);
  }else{
    Serial.println("Transmission failed!");
  }  
}

/*void sendDebug(String cmd){
  Serial.print("SEND: ");
  Serial.println(cmd);
  esp.println(cmd);
}*/

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

int lookForward()
{
    myservo.write(92); 
    delay(500);
    int distance = readPing();
    delay(100); 
    return distance;
}

int lookRight()
{
    myservo.write(10); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(92); 
    return distance;
}

int lookLeft()
{
    myservo.write(180); 
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(92); 
    return distance;
    delay(100);
}

void moveForward(){
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(motor1PWM, motorSpeed);
  //delay(1000);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor2PWM, motorSpeed+adj);
  delay(motorDelay);  
}

void turnRight(){
 // Serial.println("inside turn right");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(motor1PWM, motorSpeed);
 // delay(1000);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  analogWrite(motor2PWM, motorSpeed+adj);
  delay(motorDelay);
  
}

void turnLeft(){
  //Serial.println("inside turn left");
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  analogWrite(motor1PWM, motorSpeed);
 // delay(1000);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor2PWM, motorSpeed+adj);
  delay(motorDelay);
}

void moveStop(){
// digitalWrite(motor1PWM, 0);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
 // delay(1000);
 // digitalWrite(motor2PWM, motorSpeed+adj);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  //delay(delay);
}

void moveBackward(){
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  analogWrite(motor1PWM, motorSpeed);
  //delay(1000);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  analogWrite(motor2PWM, motorSpeed+adj);
  delay(motorDelay);  
}


