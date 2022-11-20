// Loading the ESP8266WiFi library and the PubSubClient library
#include <ESP8266WiFi.h> //Esp8266 is able to connect to the internet
#include <PubSubClient.h> //For the MQTT protocol
#include "DHT.h" //For the DHT temperature sensor

#define DHTTYPE DHT11   // DHT 11
#define TEMP "/esp/temp" //Topic of the Temperature readings
// Change the credentials below, so your ESP8266 connects to your network router
const char* wifi_name = "Telenor5855obs";
const char* wifi_password = "365IFEMmI";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "10.0.0.13";
// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor & relays
const int DHTPin = 14;
const int relay1 = 5;
const int relay2 = 4;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers
long now = millis(); //https://www.arduino.cc/reference/en/language/functions/time/millis/
long lastMeasure = 0;
const long readInterval = 10000; //read sensor data for every 10 seconds

// This functions connects your ESP8266 to your router
void connecttoWifi() {
  delay(50);
  // Connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_name);
  WiFi.begin(wifi_name, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected.. ESP8266 IP address: ");
  Serial.println(WiFi.localIP());
}
//Function that controls the relays
void callback(String topic, byte* message, unsigned int length){
  Serial.print("Check message is arrived: ");
  Serial.println(topic);

  Serial.print("Message: ");
  Serial.println();
  
  String msgRelay = "";
  for(int i = 0; i < length; i++){
    Serial.print((char)message[i]);
    msgRelay += (char)message[i];
  }
  Serial.println("------------------------");
  if(topic=="esp8266/relay1"){
    //Serial.print("Changing Relay1 state to ");
    if(msgRelay == "1"){
      digitalWrite(relay1, HIGH);
      Serial.print("ON");
    } else if (msgRelay == "0"){
      digitalWrite(relay1, LOW);
      Serial.print("OFF");
    }
  }
  if(topic=="esp8266/relay2"){
    //Serial.print("Changing Relay2 state to ");
    if(msgRelay == "1"){
      digitalWrite(relay2, HIGH);
      Serial.print("ON");
    } else if (msgRelay == "0"){
      digitalWrite(relay2, LOW);
      Serial.print("OFF");
    }
  }
  Serial.println();
}
void reconnect() {
  // This will loop until reconnected.
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266")) {
      Serial.println("connected");
      client.subscribe("esp8266/relay1");
      client.subscribe("esp8266/relay2");  
    } else {
      Serial.print("failed.. Reconneting =");
      Serial.print(client.state());
      Serial.println(" Trying to reconnect in 5 seconds");
      // Wait 5 seconds before reconnecting
      delay(5000);
    }

  }
}

void readTemp(){
  now = millis();
  // Publishes new temperature 
  if (now - lastMeasure >= readInterval) {
    lastMeasure = now;
    // Read humidity
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();

    // Error if the sensor fail reads.
    if (isnan(h) || isnan(t)) {
      Serial.println("DHT sensor faild to read!");
      return;
    }

    // Computes temperature values in Celsius
    float tempCalculate = dht.computeHeatIndex(t, h, false);
    static char tempTemp[7];
    dtostrf(tempCalculate, 6, 2, tempTemp); //turns floats into strings

    // Publishes Temperature as that is the thing we need.
    client.publish(TEMP , tempTemp);
    
    Serial.println("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
  }
}


void setup() {
  dht.begin();
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW); 

  connecttoWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

//The loop lets us stay connected, and reads the temperature.
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266");
  
  readTemp();
}