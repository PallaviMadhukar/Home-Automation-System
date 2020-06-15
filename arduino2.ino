#define door 13 //1 when NC, so other wire to be grounded
#define smokeDetector A0
#define buzzer 10
//keypad
#include <Keypad.h>
const byte ROWS = 4; //number of rows
const byte COLS = 4; //number of columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};//order of keys
byte rowPins[ROWS] = {9, 8, 7, 6}; //connections
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad customKeypad =
Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char password[]= "1234";
//gsm
int timesTosend=1;
int count=0;
char phone_no[] = "+918217059088";

void gsm(char message[]){
  while (count<timesTosend) {
  delay(1500);
  Serial.print("ATD");;
  Serial.print(phone_no);
  Serial.println(";");
  delay(10000);
  Serial.println("ATH");
  delay(1000);
  Serial.print("AT+CMGS=\"");
  Serial.print(phone_no);
  Serial.println("\"");
  delay(1000);
  Serial.println(message);
  Serial.write(0x1A);
  delay(5000);
  count++;}
count=0;
delay(1000);
Serial.println("AT+CMGF=1");
}

void setup() {
pinMode(door,INPUT);
pinMode(smokeDetector, INPUT);
pinMode(buzzer, OUTPUT);
Serial.begin(9600);
delay(1000);
Serial.println("AT+CMGF=1");
delay(200);
}


void loop(){
//smoke
int smoke = analogRead(smokeDetector);
  if(smoke>=180)  {
    char smokeAlarm []= "WARNING: SMOKE";
    gsm(smokeAlarm);
    digitalWrite(buzzer,HIGH);
    delay(10000);//wait for ten seconds
    int smokeStill=analogRead(smokeDetector);
    if(smokeStill<180)
      digitalWrite(buzzer,LOW); 
  }
//burglar
  int openClose= digitalRead(door);
  int burglar = 0;
  if(openClose){//open
    for(int i=0;i<4;i++){ //four keys
      char customKey = customKeypad.getKey();
      int wait=0;
      while(!customKey){  //keep checking for ten seconds
        delay(100);
        customKey = customKeypad.getKey();//waiting
        wait++; if(wait>100) break;
      }if(!customKey)
        burglar =1;
        break;
    }Serial.println(customKey);
    if(customKey != password[i])  {
        burglar =1 ;break;//buzzer
}}if(burglar){
    digitalWrite(buzzer,HIGH);
    char burglarAlarm[] = "WARNING : BURGLAR";
    gsm(burglarAlarm);
}}}
