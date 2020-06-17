#include "DHT.h"
#include<Wire.h>
#define DHTPIN 10
#define DHTTYPE DHT11
#define FAN 7
#define LDR A0
int BARLED[]={2,3,4,5,6};
#define lights1 13
#define lights2 12
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN,OUTPUT) ;  //Logic pins are also set as output
  for(int i=0;i<5;i++)
{ pinMode(BARLED[i],OUTPUT);
  digitalWrite(BARLED[i],LOW);
}pinMode(lights1,OUTPUT);pinMode(lights2,OUTPUT);
digitalWrite(lights2,LOW);
}

void loop() {
delay(100);
}

void receiveEvent(int howMany) {
int y=Wire.read();
int x = Wire.read();    // receive byte as an integer
if(y==1) kitchenlight(x);
else if(y==2) kitchenfan(x);
else if(y==3) hall(x);
Serial.println(y);
Serial.println(x);
}

void tempReading()
{
delay(2000);
float t = dht.readTemperature();
if (isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
t=28;
}
Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" *C ");  
if(t>25) fanControl(1);
else fanControl(0);
}

void fanControl(int val)
{
if (val==1) digitalWrite(FAN,HIGH) ;
else if(val==0)digitalWrite(FAN,LOW) ;
}

void lightReading()
{
int intensity = analogRead(LDR);
int count;
if(intensity<=50) count=5;
else if(intensity<=100) count=4;
else if(intensity<=250) count=3;
else if(intensity<=400) count=2;
else if(intensity<650) count=1;
else if(intensity>=650) count=0;
lightControl(count);
}

void lightControl(int count)
{
for(int i=0;i<count;i++) digitalWrite(BARLED[i],HIGH);
for(int i=count;i<5;i++) digitalWrite(BARLED[i],LOW);
}

void kitchenlight(int val)//0123456
{
if(val==6) for(int i=0;i<20;i++)
{
  lightReading(); delay(2000);
    }
else lightControl(val);
}

void kitchenfan(int val)//012345
{
if(val==2) tempReading();
else fanControl(val);
}

void hall(int val)
{
 switch(val)
{
  case(0): digitalWrite(lights1,LOW); break;
case(1): digitalWrite(lights1,HIGH); break;
case(2):{ 
      digitalWrite(lights1,HIGH);
      delay(2000);
      digitalWrite(lights1,LOW);
      delay(2000);
     break;
     }
}}
