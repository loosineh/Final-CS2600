
#include <PubSubClient.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h> 
#include <WiFi.h>
#include <PubSubClient.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// WiFi
const char* ssid = "tmobilemenva";
const char* password = "Chelsea2009$";

// MQTT Broker
const char* mqtt_server = "192.168.12.163";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : [");
  Serial.print(topic);
   Serial.println();
  Serial.print(" publish data is:");
  lcd.clear();
  {
  for(int i=0;i<length;i++)
  {
    Serial.print((char)payload[i]);  
    lcd.setCursor(0, 0);
    lcd.print("OSOYOO LCD"); // Start Print text to Line 1
    lcd.setCursor(i, 1);
    lcd.write((char)payload[i]);
    }
  }

   
   
 
  Serial.println();
} //end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
 
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("OsoyooCommand");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight 
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.setCallback(callback);
  client.loop();

}
