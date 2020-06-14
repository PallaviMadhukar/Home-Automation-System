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
//const int pwm=2;
const int in_1 = 7 ;
const int in_2 = 6;

const int pwm2= 5;
const int in2_1= 8; 


//Ultrasonic
int trig=3;//trig to digital pin 13
int echo=4;//echo to digital pin 12
int delayReq=1000;

void setup()
{
  pinMode(A1,INPUT);
pinMode(2,OUTPUT) ;    //we have to set PWM pin as output
pinMode(in_1,OUTPUT) ;  //Logic pins are also set as output
pinMode(in_2,OUTPUT) ;
pinMode(A0, OUTPUT);
//digitalWrite(A0, HIGH);

Serial.begin(9600);

  // Initiate a serial communication
SPI.begin();      // Initiate  SPI bus
mfrc522.PCD_Init();   // Initiate MFRC522
Serial.println("Approximate your card to the reader...");
Serial.println();
}

void loop() {
int sensorValue = analogRead(A1);
Serial.print("sensor value is");
Serial.println(sensorValue);
//1024 is dry, 400 is wet- check values and change 
//sensorValue=1045;
if(sensorValue<400){
  Serial.println("hi");
digitalWrite(in2_1,HIGH);
digitalWrite(A0,HIGH) ;
analogWrite(pwm2,255) ; }
else{
digitalWrite(in2_1,HIGH) ;
digitalWrite(A0,LOW) ;
analogWrite(pwm2,255) ;
delay(10000);}
// Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
Serial.print("UID tag :");
String content= "";
byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) 
{
  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  Serial.print(mfrc522.uid.uidByte[i], HEX);
  content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
Serial.println();
Serial.print("Message : ");
content.toUpperCase();
if (content.substring(1) == "F2 E6 C4 E9") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
//to turn anticlockwise
    digitalWrite(in_1,LOW) ;
    digitalWrite(in_2,HIGH) ;
    analogWrite(2,255) ;
    delay(8000);
    
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,HIGH) ;
    delay(1500);
    
    pinMode(trig,OUTPUT);
    digitalWrite (trig,LOW);

    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
      //recieves the reflected ping
    long timeUs,distanceCm;
    pinMode(echo,INPUT);
    timeUs=pulseIn(echo,HIGH);//time taken in microseconds
    distanceCm = usToCm(timeUs);//distance in centimeters
    Serial.print("The distance is ");
    Serial.println(distanceCm);
    delay(delayReq);

    //change here for gate to close 
    while(distanceCm<int(10))
    {
      
    pinMode(trig,OUTPUT);
    digitalWrite (trig,LOW);

    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);
      //recieves the reflected ping
    pinMode(echo,INPUT);
    timeUs=pulseIn(echo,HIGH);//time taken in microseconds
    distanceCm = usToCm(timeUs);//distance in centimeters
    Serial.print("The distance is ");
    Serial.println(distanceCm);
    delay(delayReq);
    }
    if(distanceCm>int(10)){
    // to turn clockwise
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,LOW) ;
    analogWrite(2,255) ;
    delay(8000);
    
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,HIGH) ;
    delay(1500);
    }
    /*For brake
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,HIGH) ;
    delay(200);
    
    //For Anti Clock-wise motion - IN_1 = LOW , IN_2 = HIGH
    digitalWrite(in_1,LOW) ;
    digitalWrite(in_2,HIGH) ;
    delay(200);
    //For brake
    digitalWrite(in_1,HIGH) ;
    digitalWrite(in_2,HIGH) ;
    delay(200);*/

  
}
else   {
    Serial.println(" Access denied");
    delay(3000);
  }
delay(30000);
}

long usToCm(long us)//function to calculate distance from time
{
   return us /29 /2;
}
