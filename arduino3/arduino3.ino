#include "DHT.h"
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial EEBlue(10, 11); // RX | TX
String command="";//Stores response of the HC-06 Bluetooth device
#define DHTPIN 12     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
#define FAN 9
#define LDR A0
int BARLED[]={2,3,4,5,6};
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  EEBlue.begin(9600);  //Default Baud for comm 
  Serial.println("The bluetooth gates are open.");
  Serial.println("Connect to HC-05 from any other bluetooth device with 1234 as pairing key!.");
  dht.begin();
  pinMode(FAN,OUTPUT);
  digitalWrite(FAN,LOW);
  for(int i=0;i<5;i++){
    pinMode(BARLED[i],OUTPUT);
    digitalWrite(BARLED[i],LOW);
}Wire.begin(); // join i2c bus (address optional for master)
}

void loop() {
if(EEBlue.available())
{    while(EEBlue.available()) { // While there is more to be read, keep reading.
  command += (char)EEBlue.read();
}}command.trim();
control(command);
delay(5000);
command="";
}

void tempReading()
{
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
if(intensity<=75) count=5;
else if(intensity<=200) count=4;
else if(intensity<=300) count=3;
else if(intensity<=400) count=2;
else if(intensity<650) count=1;
else if(intensity>=650) count=0;
lightControl(count);
}
void lightControl(int count)
{
if(count>5) count=5;
if(count<0) count=0;
for(int i=0;i<count;i++) digitalWrite(BARLED[i],HIGH);
for(int i=count;i<5;i++) digitalWrite(BARLED[i],LOW);
}

void control(String command)
{//all 012 //fans 012 //lights 012
//bedroom 012 //bedroom light 0123456 //bedroom fan 012
//kitchen 012 //kitchen light 0123456 //kitchen fan 012 
//hall 012
Serial.println(command);
if(command==("all off")) all(0);
else if (command==("all on")) all(1);
else if (command==("all auto")) all(2);
else if(command==("fans off")) fans(0);
else if (command==("fans on")) fans(1);
else if (command==("fans auto")) fans(2);
else if(command==("lights off")) lights(0);
else if (command==("lights on")) lights(1);
else if (command==("lights auto")) lights(2);
else if (command==("bedroom off")) bedroom(0);
else if (command==("bedroom on")) bedroom(1);
else if (command==("bedroom auto")) bedroom(2);
else if (command==("kitchen off")) kitchen(0);
else if (command==("kitchen on")) kitchen(1);
else if (command==("kitchen auto")) kitchen(2);
else if (command==("hall off")) hall(0);
else if (command==("hall on")) hall(1);
else if (command==("hall auto")) hall(2);
else if (command==("bedroom light off")) bedroomlight(0);
else if (command==("bedroom light 1")) bedroomlight(1);
else if (command==("bedroom light 2")) bedroomlight(2);
else if (command==("bedroom light 3")) bedroomlight(3);
else if (command==("bedroom light 4")) bedroomlight(4);
else if (command==("bedroom light 5")) bedroomlight(5);
else if (command==("bedroom light auto")) bedroomlight(6);
else if (command==("bedroom fan off")) bedroomfan(0);
else if (command==("bedroom fan on")) bedroomfan(1);
else if (command==("bedroom fan auto")) bedroomfan(2);
else if (command==("kitchen light off")) kitchenlight(0);
else if (command==("kitchen light 1")) kitchenlight(1);
else if (command==("kitchen light 2")) kitchenlight(2);
else if (command==("kitchen light 3")) kitchenlight(3);
else if (command==("kitchen light 4")) kitchenlight(4);
else if (command==("kitchen light 5")) kitchenlight(5);
else if (command==("kitchen light auto")) kitchenlight(6);
else if (command==("kitchen fan off")) kitchenfan(0);
else if (command==("kitchen fan on")) kitchenfan(1);
else if (command==("kitchen fan auto")) kitchenfan(2);
}

void bedroomlight(int val){ //012345
if(val==6)
    for(int i=0;i<20;i++)
    { if(EEBlue.available()) break;
      lightReading(); delay(2000);
    }    else lightControl(val);}
void bedroomfan(int val)//012345
{if (val==2) tempReading();
else fanControl(val);}
void bedroom(int val)
{switch(val)
{case 0: {   bedroomlight(0);
bedroomfan(0);
break;}
case 1:{    bedroomlight(5);
bedroomfan(1);
break;}
case 2:{bedroomlight(6);
bedroomfan(2);
break;}
}}

void kitchenlight(int val){
  byte y =1;byte x = val;
  wire(y,x);}
void kitchenfan(int val){
  byte y =2;byte x = val;
  wire(y,x);}
void kitchen(int val)
{switch(val)
{case 0:{ kitchenlight(val);
kitchenfan(val);
break;}
case 1:{
kitchenlight(5);
kitchenfan(1);
break;}
case 2:{
kitchenlight(6);
kitchenfan(2);
break;
}}}

void hall(int val)
{
byte y =3;
byte x = val;
wire(y,x);
}
void wire(int y,int x){
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(y);        // sends five bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  x++;
  delay(500);
}

void fans(int fanval){
  kitchenfan(fanval);
  bedroomfan(fanval);
}
void lights(int lval){
switch(lval)
{case(0):    kitchenlight(0);
bedroomlight(0);
hall(0);
break;
case(1):   kitchenlight(5);
bedroomlight(5);
hall(1);
break;
case(2):bedroomlight(6);
kitchenlight(6);
hall(2);
break;}
}
void all(int val){
  lights(val);
  fans(val);
}
