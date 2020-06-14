//1. Gate Access using RFID
//2. Automatic closing of Gate using ultrasonic sensor
//3. Gardening System using soil moisture sensor and pump

#include <SPI.h>
#include <MFRC522.h>

//RFID initialization
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  

//Gate motor initialization
const int pwm=2;
const int in_1 = 7 ;
const int in_2 = 6;
const int pwm2= 5;
const int in2_1= 8; 
const int in2_2= A0;
const int gate=8000; //Delay according to gate distance- time taken to open the gate

//Ultrasonic sensor initialization
int trig=3;//trig to digital pin 13
int echo=4;//echo to digital pin 12
int delayReq=1000;

//Soil moisture sensor initialization
int moisture=A1;

void setup()
{
  pinMode(moisture,INPUT); //Soil moisture sensor inputs moisture values
  pinMode(pwm1,OUTPUT);    //PWM pin as output
  pinMode(pwm2,OUTPUT);
  pinMode(in_1,OUTPUT);  //Logic/Control pins are also output
  pinMode(in_2,OUTPUT);
  pinMode(in2_1,OUTPUT);  
  pinMode(in2_2,OUTPUT);
  pinMode(trig,OUTPUT); //Trigger of ultrasonic sensor is output
  pinMode(echo,INPUT);//We recieve echo of trigger
  
  Serial.begin(9600);
  // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}

void loop()
{
  soilmoisture(); //Call gardening system function
  rfid(); //Call gate system function
  delay(5000);
}

void soilmoisture() //Reads moisture value and accordingly sets pump
{
  int sensorValue = analogRead(moisture); //Read moisture value
  Serial.print("Sensor value is"); //Print moisture value
  Serial.println(sensorValue);
  //1024 is dry, 400 is wet
  if(sensorValue<400)
  motoroff(in2_1,in2_2,pwm2); //Garden is sufficiently wet so pump can be switched off
  else
  {
  motorcw();//Garden is dry so pump can be switched on
  delay(10000);
  }
}

void rfid()
{
  if ( ! mfrc522.PICC_IsNewCardPresent()) return; //No new card detection
  if ( ! mfrc522.PICC_ReadCardSerial())  return; //UID not read
  
  Serial.print("UID tag :");//Show UID on serial monitor
  String content= ""; byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
  Serial.print(mfrc522.uid.uidByte[i], HEX);
  content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
  content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F2 E6 C4 E9") //UID of authorized card
  {
    Serial.println("Authorized access");
    motoracw(in_1,in_2,pwm1); //Anticlockwise(Open the gate)
    delay(gate); //Time taken to open the gate
    motoroff(in_1,in_2,pwm1); //Switch off motor(Gate is still open)
    delay(1500);
    int distanceCm=ultrasonic(); //Check if any obstacle
    while(distanceCm<int(10)) distanceCm=ultrasonic(); //Keep checking if any obstacle
    //Gets out of while loop when distanceCm>int(10) i.e. no obstacle 
    motorcw(in_1,in_2,pwm1);
    delay(gate); //Clockwise(Close the gate)
    motoroff(in_1,in_2,pwm1);
    delay(1500);
    }
  }
  else //UID not that of authorized card
  {
    Serial.println(" Access denied");delay(3000);
  }
}

void motoroff(int in1,int in2,int pwm) //Switch off gate motor
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
  analogWrite(pwm,0);
}

void motorcw(int in1,int in2,int pwm) //Clockwise
{
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  analogWrite(pwm,255);
}

void motoracw(int in1,int in2, int pwm) //Anticlockwise
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  analogWrite(pwm,255);
}

int ultrasonic() //Find distance to nearest object
{
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW); //Sending ping
    //Recieving the reflected ping
    timeUs=pulseIn(echo,HIGH);//Time taken in microseconds
    distance = usToCm(timeUs);//Distance in centimeters
    Serial.print("The distance is ");
    Serial.println(distance);
    delay(delayReq);
    return distance;
}

long usToCm(long us) //Calculate distance from time
{
   return us /29 /2;
}
