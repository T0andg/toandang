#define BLYNK_TEMPLATE_ID "TMPL6rAjFuHan"
#define BLYNK_TEMPLATE_NAME "NRF24L01"
#define BLYNK_AUTH_TOKEN "jap4n7TsSMO_EUg7pkHfEEFLdRE_UfCf"

#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <string.h>
#include <EEPROM.h>
String apiKey = "H34VDQ24PDCJUQ57";
 
const char* ssid = "Name_Wifi";
const char* password = "Password_Wifi";
const uint64_t Address_NodeGate = 0xE8E8F0F0E1LL;

WiFiClient client;
int gatewayID = EEPROM.read(0);
const int postingInterval = 5 * 1000;       //Gui du lieu len sau moi 5s
WidgetLED LED_ON_APP(V2);
char auth[] = BLYNK_AUTH_TOKEN;
RF24 radio(4, 5); // use this for NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
char Recv_Data[32];
typedef struct {
int count;
float temperature;
float humidity;
}Data_Node_t;

Data_Node_t Data_DHT;
void setup()
{
  SPI.begin();
  Blynk.begin(auth, ssid, password);
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(true); 
  radio.setChannel(52);
  radio.setRetries(10,15);
  radio.setDataRate (RF24_250KBPS);
  radio.openReadingPipe(1, Address_NodeGate);
  radio.enableDynamicPayloads();
  radio.printDetails();
  Serial.print("Receiver Started, ID: ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print(gatewayID);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();

}


void loop()
{
  if (radio.available() ) 
  {
    Blynk.run();
    radio.read(Recv_Data, 32);
    memcpy(&Data_DHT, Recv_Data, sizeof(Data_DHT));
    Serial.println("Data Received:");
    Serial.print("Packet No. = ");
    Serial.println(Data_DHT.count);     
  

    Serial.print("Temperature = ");
    Serial.print(Data_DHT.temperature);   
    Serial.println("*C");
    float t = Data_DHT.temperature;

    Serial.print("Humidity = ");
    Serial.print(Data_DHT.humidity);     
    Serial.println("%");
    float h = Data_DHT.humidity;
    Serial.println();

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
  }
}

