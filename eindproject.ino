#include <WiFi.h> // wifi for arduino nano 33 IoT
#include <PubSubClient.h>

char ssid[] = "NoWifi"; // your network SSID (name)
char pass[] = "MALLALI1975"; // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS; // the Wi-Fi radio's status
const char* mqttServer = "192.168.0.208"; // ip adres of the raspb
const int mqttPort = 1883; // mqtt port
const char* mqttUser = "wail"; // username
const char* mqttPassword = "Admin"; // password
const char* clientID = "home"; // MQTT client ID

WiFiClient espClient;
PubSubClient client(espClient);

#define switch 2
float ok = 0.59;

void setup() {
  Serial.begin(115200); // start the serial monitor
  WiFi.begin(ssid, pass); // start the wifi

  pinMode(switch, INPUT);
  Serial.print("searching...");
  
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
}

void loop() {
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
  
  ok = ok + 0.1;
  
  String value = String(ok);
  
  // send the values to the raspb
  client.publish("home/cpu/value", String(ok).c_str());
  client.publish("test/mosq", "game");
  client.publish("test/mosq", " ");
  delay(1000);//Wait for 1 seconds
  
  client.disconnect();  // disconnect from the MQTT broker

  delay(1000); // Wait for 1 seconds
}
