//1. Gate Access only using Smart Card (RFID)
//2. Automatic closing of Gate using Ultrasonic sensor
//3. Gardening System

#include <SPI.h>
#include <MFRC522.h>
//RFID 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  
//Motor 
const int pwm=2;
const int in_1 = 7 ;
const int in_2 = 6;
const int pwm2= 5;
const int in2_1= 8; 
const int in2_2=A0;
const int gate=8000;//delay for gate distance
//Ultrasonic
int trig=3;//trig to digital pin 13
int echo=4;//echo to digital pin 12
int delayReq=1000;
//Soil moisture sensor
int moisture=A1;

void setup()
{
  pinMode(moisture,INPUT); //soil moisture sensor inputs values
  pinMode(pwm1,OUTPUT);    //we have to set PWM pin as output
  pinMode(pwm2,OUTPUT);
  pinMode(in_1,OUTPUT);  //Logic pins are also set as output
  pinMode(in_2,OUTPUT);
  pinMode(in2_1,OUTPUT);  
  pinMode(in2_2,OUTPUT);
  pinMode(trig,OUTPUT);//we send trigger
  pinMode(echo,INPUT);//we recieve echo of trigger
  Serial.begin(9600);
  // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}
void loop() {
  soilmoisture();
  rfid();
  delay(5000);}

void soilmoisture()
{
  int sensorValue = analogRead(moisture);
  Serial.print("Sensor value is");
  Serial.println(sensorValue);
  //1024 is dry, 400 is wet
  if(sensorValue<400)
  motoroff(in2_1,in2_2,pwm2);//wet so off pump
  else{
  motorcw();//dry so on pump
  delay(10000);
}}

void rfid(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) return;//no new card
  if ( ! mfrc522.PICC_ReadCardSerial())  return; //UID not read
  Serial.print("UID tag :");//Show UID on serial monitor
  String content= ""; byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  Serial.print(mfrc522.uid.uidByte[i], HEX);
  content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F2 E6 C4 E9") //UID of authorized card
  { Serial.println("Authorized access");
    motoracw(in_1,in_2,pwm1); delay(gate); //anticlockwise(open)
    motoroff(in_1,in_2,pwm1); delay(1500);
    int distanceCm=ultrasonic();
    while(distanceCm<int(10)) distanceCm=ultrasonic();//obstacle
    if(distanceCm>int(10)){
    motorcw(in_1,in_2,pwm1);delay(gate);//clockwise(close)
    motoroff(in_1,in_2,pwm1);delay(1500);}}
  else   {Serial.println(" Access denied");delay(3000);}}

void motoroff(int in1,int in2,int pwm){//switch off motor
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  analogWrite(pwm,0);
}

void motorcw(int in1,int in2,int pwm){//clockwise
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(pwm,255);
}

void motoracw(int in1,int in2, int pwm){//anticlockwise
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(pwm,255);
}

int ultrasonic(){
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);//sending ping
    //recieves the reflected ping
    timeUs=pulseIn(echo,HIGH);//time taken in microseconds
    distance = usToCm(timeUs);//distance in centimeters
    Serial.print("The distance is ");
    Serial.println(distance);
    delay(delayReq);
    return distance;
}

  long usToCm(long us)//function to calculate distance from time
{
   return us /29 /2;
}
