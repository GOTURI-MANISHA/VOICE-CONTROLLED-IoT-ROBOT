#include<BluetoothSerial.h>
BluetoothSerial bt;
String data=" ";
String status=" ";

#define m1 33
#define m2 32
#define m3 25
#define m4 26

#define ANALOG_IN_PIN 35
float adc_voltage = 0.0;
float in_voltage = 0.0;
float R1 = 30000.0;
float R2 = 7500.0; 
float ref_voltage = 3.3;
int adc_value = 0;

#define ir 27
int ir_sense;

#define buzzer 12

#define led 14

int i=0,j=0,k=0,l=0,m=0;

void setup() 
{
  Serial.begin(9600);
  bt.begin("ESP VOICE ROBOT");

  pinMode(ANALOG_IN_PIN,INPUT);
  pinMode(ir,INPUT);
  
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,LOW);

  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);

  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);

  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
}
void loop()
{
  ir_sense=digitalRead(ir);
  ir_sense=1-ir_sense;

  if(ir_sense==1)
  {
    data=" ";
    bt.println("OBJECT DETECTED");
    digitalWrite(buzzer,HIGH);
    stop();
    backward();
    delay(1000);
    stop();
  }
  else
  {
    digitalWrite(buzzer,LOW);

    bt_loop();
  }
  
  volt_loop();
  
  digitalWrite(2,HIGH);delay(50);digitalWrite(2,LOW);delay(50);
}
void bt_loop()
{
  if(bt.available()>0)
  {
    data=" ";
    data=bt.readString();
    Serial.print("data : ");Serial.println(data);
  }

  if(data=="FORWARD" || data=="Forward" || data=="forward")
  {
    forward();
  }
  else if(data=="BACKWARD" || data=="Backward" || data=="backward")
  {
    backward();
  }
  else if(data=="LEFT" || data=="Left" || data=="left")
  {
    left();
    delay(100);
    stop();
    delay(100);
  }
  else if(data=="RIGHT" || data=="Right" || data=="right")
  {
    right();
    delay(100);
    stop();
    delay(100);
  }
  else if(data=="STOP" || data=="Stop" || data=="stop")
  {
    stop();
  }
  else if(data=="LIGHT" || data=="Light" || data=="light")
  {
    data=" ";
    i=i+1;
    if(i==1)
    {
      digitalWrite(led,HIGH);
    }
    else
    {
      i=0;
      digitalWrite(led,LOW);
    }
  }
  else if(data=="SOUND" || data=="Sound" || data=="sound")
  {
    data=" ";
    j=j+1;
    if(j==1)
    {
      digitalWrite(buzzer,HIGH);
      delay(500);
    }
    else
    {
      j=0;
      digitalWrite(buzzer,LOW);
    }
  }
  else if(data=="VOLTAGE" || data=="Voltage" || data=="voltage")
  {
    data=" ";
    String val=String("VOLTAGE : ")+String(in_voltage)+String("V");
    bt.println(val);
  }
}
void forward()
{
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void backward()
{
  digitalWrite(m1,LOW);
  digitalWrite(m2,HIGH);
  digitalWrite(m3,LOW);
  digitalWrite(m4,HIGH);
}
void right()
{
  digitalWrite(m1,HIGH);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,HIGH);
}
void left()
{
  digitalWrite(m1,LOW);
  digitalWrite(m2,HIGH);
  digitalWrite(m3,HIGH);
  digitalWrite(m4,LOW);
}
void stop()
{
  digitalWrite(m1,LOW);
  digitalWrite(m2,LOW);
  digitalWrite(m3,LOW);
  digitalWrite(m4,LOW);
}

void volt_loop()
{
  adc_value = analogRead(ANALOG_IN_PIN);
  adc_voltage  = (adc_value * ref_voltage) / 4096.0;
  in_voltage = adc_voltage*(R1+R2)/R2;
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);
  delay(50);

  if(in_voltage >= 6 || in_voltage <= 3)
  {
    digitalWrite(buzzer,HIGH);
    bt.println("VOLTAGE FLUCTUATION DETECTED");
  }
  else
  {
    digitalWrite(buzzer,LOW);
  }
}
