//#include <RH_NRF24.h>
#include "DHT.h"
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
 
RF24 radio(D4, D8); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 
 
//#define DHTPIN D1
#define DHT11_PIN 4

DHT dht11(DHT11_PIN, DHT11);
const uint64_t  Address_NodeGate = 0xE8E8F0F0E1LL;
int count = 0;


const char* ssid = "Sunny";
const char* password = "00000000";
WiFiClient client;
int deviceID = EEPROM.read(0);
int humidity, temperature;

typedef struct {
int count;
float temperature;
float humidity;
}Data_Node_t;

Data_Node_t Data_DHT;
 
void setup()
{
  SPI.begin();
  radio.begin();
  dht11.begin();
  Serial.begin(9600);
  radio.setChannel(52);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(10,15);
  radio.enableDynamicPayloads();
  radio.setAutoAck(true);
  radio.openWritingPipe(Address_NodeGate);
  radio.printDetails();
  radio.stopListening();
  Serial.print("transmitter Started, ID: ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
}
 
void loop() 
{
  Data_DHT.count = count;
  Data_DHT.temperature = dht11.readTemperature();
  Data_DHT.humidity = dht11.readHumidity();

  Serial.print("Packet No. = ");
  Serial.println(Data_DHT.count);
  
  Serial.print("Temperature = ");
  Serial.print(Data_DHT.temperature);
  Serial.println("*C");
 
 
  Serial.print("Humidity = ");
  Serial.print(Data_DHT.humidity);
  Serial.println("%");

  Serial.println();
  
  radio.write(&Data_DHT, 32);
  Serial.println("Data Packet Sent");
  Serial.println("");
  
  count++;
  delay(5000);
}