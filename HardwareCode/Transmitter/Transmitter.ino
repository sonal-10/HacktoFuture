#include <dht11.h>
#include<SoftwareSerial.h>
//The Above Libraries requried for the SerialCommunication And DHT11 Sensor

//Define the Digital Pins That Transmit the data To ESP8266 wifi Module
#define RX 2
#define TX 3
#define dht_apin 11
#define IRpin A0
//Creating an Obejct for DHT11 sensor and define the pin 12
dht11 dht_Object;
String AP =   "HACKATHON-1";            //AccessPoint Name
String PASS = "Hack@sjec1";           //AccessPoint Password
String API =  "N5K6ORNGMGLB3Y5A";       //Write API KEY
String HOST = "api.thingspeak.com";     //Host Address
String PORT = "80";                      //Port Number

//Declaration for AT Commands
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(RX,TX);

void setup() {
  // Setting the BaudRate and AT commands for ESP8266
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  pinMode(IRpin,INPUT);
}

void loop() {
  // Upadating the data to the ThingSpeak Cloud   
  String getData ="GET /update?api_key="+ API + "&field1="+getTemperature() +"&field2="+getHumidity()+"&field3="+getIR()  ;

  //Sending Data
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);
 delay(1500);
 countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}
String getIR(){
  int IRvalue = 0;
  IRvalue = analogRead(IRpin);
  Serial.println("IR value=");
  Serial.println(IRvalue);
  delay(50);
  return String(IRvalue);
}
String getTemperature(){
  dht_Object.read(dht_apin);
  int temp = dht_Object.temperature;
  Serial.println("Celsius =");
  Serial.print(temp);
  int temperature= temp + 273;
  Serial.println("Kelvin =");
  Serial.print(temperature);
  delay(50);
  return String(temperature); 
}

String getHumidity(){

   dht_Object.read(dht_apin);
   Serial.println(" Humidity in %= ");
   int humidity = dht_Object.humidity;
   Serial.println(humidity);
   delay(50);
   return String(humidity);   
}

// AT commands
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OKtest");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
