//1. Main Door access using Key entered in a Keypad
//2. Burglar Alarm
//3. Smoke Detector

#define door 13 //1 when NC, so other wire to be grounded
#define smokeDetector A0
#define buzzer 10

//Keypad
#include <Keypad.h>
const byte ROWS = 4; //Number of rows
const byte COLS = 4; //Number of columns
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}};//Order of keys
byte rowPins[ROWS] = {9, 8, 7, 6}; //Connections
byte colPins[COLS] = {5, 4, 3, 2}; 
Keypad customKeypad =
Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char password[]= "1234";

//GSM
int timesTosend=1;
int count=0;
char phone_no[] = "+91821705XXXX"; //Fill phone number

void gsm(char message[])
{ //Send SMS via GSM module
  while (count<timesTosend)
  {
    delay(1500);
    Serial.print("ATD");
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
    count++;
  }
  count=0;
  delay(1000);
  Serial.println("AT+CMGF=1");
}

void setup() 
{
  pinMode(door,INPUT);
  pinMode(smokeDetector, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(200);
}


void loop()
{
//Smoke Alarm
  int smoke = analogRead(smokeDetector); //Smoke levels taken as input
  if(smoke>=180)  
  {//Buzzer switched on
    char smokeAlarm []= "WARNING: SMOKE";
    gsm(smokeAlarm);
    digitalWrite(buzzer,HIGH);
    delay(10000);//Wait for ten seconds
    int smokeStill=analogRead(smokeDetector);
    if(smokeStill<180)
      digitalWrite(buzzer,LOW); 
  }
  
//Burglar Alarm
  int openClose= digitalRead(door);
  int burglar = 0;
  if(openClose)
  {//If door is opened
    for(int i=0;i<4;i++)
    { //Read four keys
      char customKey = customKeypad.getKey();
      int wait=0;
      while(!customKey)
      {  //Keep checking for ten seconds
        delay(100);
        customKey = customKeypad.getKey();//Waiting
        wait++;
        if(wait>100) 
          break;
      }
      if(!customKey) //Burglar alarm switched on 
        burglar =1;
        break;
    }
    Serial.println(customKey);
    if(customKey != password[i])
    {
      burglar =1 ;
      break;//buzzer
    }
  }
  if(burglar)
  {
    digitalWrite(buzzer,HIGH);
    char burglarAlarm[] = "WARNING : BURGLAR";
    gsm(burglarAlarm);
  }
}
