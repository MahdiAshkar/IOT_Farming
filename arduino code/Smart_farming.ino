#include <DHT.h> 
#include <SoftwareSerial.h>
SoftwareSerial Sim800(2,3);// 3 tx , 2 rx
#define DHTPIN A1
#define SoilMoisturePIN A0
int maximumMoistureLevel; 
int currentMoistureLevel; 
float temperature = 0;
float humidity = 0;
DHT dht;

void setup() {
  Sim800.begin(9600);
  Serial.begin(9600);  
  dht.setup(DHTPIN, 1);
  pinMode (SoilMoisturePIN, INPUT); //A0   is the pin used for the Soil Moisture Sensor
  maximumMoistureLevel = analogRead(SoilMoisturePIN);
}

void loop() {
  delay(60000);
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println("°C");
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
  delay(100);

  //------------------------
  currentMoistureLevel = analogRead(SoilMoisturePIN);
  Serial.print("SoilMoisture= ");
  Serial.println(currentMoistureLevel);
  delay(100); 
  //------------------------
  if (Sim800.available())
      Serial.write(Sim800.read());
 
  Sim800.println("AT");
  delay(1000);
  ShowResponse();
  Sim800.println("AT+CPIN?");
  delay(1000);
  
  Sim800.println("AT+CREG?");
  delay(1000);
 
  Sim800.println("AT+CGATT?");
  delay(1000);
 
  Sim800.println("AT+CIPSHUT");
  delay(1000);
 
  Sim800.println("AT+CIPSTATUS");
  delay(1000);
 
  Sim800.println("AT+CIPMUX=0");
  delay(1000);
  ShowResponse();
 
  Sim800.println("AT+CSTT=\"mtnirancell\"");//start task and setting the APN,
  delay(1000);
 
  ShowResponse();
  Sim800.println("AT+CIICR");//bring up wireless connection
  delay(1000);

  ShowResponse();
 
  Sim800.println("AT+CIFSR");//get local IP adress
  delay(1000);

  ShowResponse();
 
  Sim800.println("AT+CIPSPRT=0");
  delay(1000);

  ShowResponse();
  
  Sim800.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(2000);

  ShowResponse();

  Sim800.println("AT+CIPSEND");//begin send data to remote server
  ShowResponse();
  delay(1000);
  
  String str="GET https://api.thingspeak.com/update?api_key=86PTGDGU2YH28NJE&field1=" \
  + String(temperature) +"&field2="+String(humidity)+ "&field3="+ String(currentMoistureLevel);

  Serial.println(str);
  Sim800.println(str);//begin send data to remote server
  
  
  delay(1000);
   
  ShowResponse();
 
  Sim800.println((char)26);//sending
  delay(2000);//waitting for reply, important! the time is base on the condition of internet 
  Sim800.println();
 
  ShowResponse();
 
  Sim800.println("AT+CIPSHUT");//close the connection
  delay(100);
  
  ShowResponse();

}

void ShowResponse(){
  while(Sim800.available()!=0)
  Serial.write(Sim800.read());
  delay(2000); 
}


