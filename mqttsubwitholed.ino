#include <WiFi.h> // wifi for arduino nano 33 IoT
#include <PubSubClient.h>
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

char ssid[] = "NoWifi"; // your network SSID (name)
char pass[] = "MALLALI1975"; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS; // the Wi-Fi radio's status
const char* mqttServer = "192.168.0.208"; // ip adres of the raspb
const int mqttPort = 1883; // mqtt port
const char* mqttUser = "wail"; // username
const char* mqttPassword = "Admin"; // password
const char* clientID = "home"; // MQTT client ID

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int i = 1;

WiFiClient espClient;
PubSubClient client(espClient);
String blue = "blue OFF";
String red = "red OFF";
String green = "green OFF";

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char *cstring = (char *) payload;
  cstring[length] = '\0';

  Serial.print(cstring);
  
   if (!strcmp(cstring, "blue ON")) 
    {
            digitalWrite(25, HIGH);
            /*display.setCursor(32, 18);
            display.print("blue ON");*/
            blue = "blue ON";
    } 
    if (!strcmp(cstring, "blue OFF")) 
    {
            digitalWrite(25, LOW);
            /*display.setCursor(32, 18);
            display.print("blue OFF");*/
            blue = "blue OFF";
    }
    
    if (!strncmp(cstring, "red ON", length))
    {
            /*display.setCursor(32, 28);
            display.print("red ON");*/
            red = "red ON";
    }
    if (!strncmp(cstring, "red OFF", length))
    {
            /*display.setCursor(32, 28);
            display.print("red OFF");*/
            red = "red OFF";
    }
    
    if (!strncmp(cstring, "green ON", length))
    {
            /*display.setCursor(32, 38);
            display.print("red ON");*/
            green = "green ON";
    }
    if (!strncmp(cstring, "green OFF", length))
    {
            /*display.setCursor(32, 38);
            display.print("green OFF");*/
            green = "green OFF";
    }

  Serial.println();
  
}

int flag = 0;
int flag2 = 0;
int flag3 = 0;
int flagger= 0;

int ledon = 1;
int ledoff = 0;
const byte interruptPin = 27;
volatile byte state = LOW;

void setup() 
{
  
  Serial.begin(115200); // start the serial monitor
  WiFi.begin(ssid, pass); // start the wifi
  Serial.print("searching...");

  pinMode(25, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("connected IP adres = ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.println("---------------------------------------");
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  attachInterrupt(digitalPinToInterrupt(interruptPin), button, RISING);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { 
    Serial.println(F("SSD1306 allocation failed"));
  }
  
}

void loop() 
{
  client.loop();
  
  // connect to the server
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("connected");
    } 
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("bruh/lmao");

  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(32, 18);
  display.print(blue);
  display.setCursor(32, 28);
  display.print(red);
  display.setCursor(32, 38);
  display.print(green); 
  display.display();
  

  if(flag == 1)
  {
    if(flag2 == 1)
    {
      client.publish("home/cpu/hens", "1");
    }
    else
    {
      client.publish("home/cpu/hens", "0");
    }
    delay(100);
    flag = 0;
    flag2 = !flag2;
  }
}

void button() 
{
  flag = 1;
}
