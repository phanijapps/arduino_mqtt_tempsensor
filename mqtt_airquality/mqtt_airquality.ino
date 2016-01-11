#include <DHT.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>


#define DHTPIN 2     //Data Pin

#define DHTTYPE DHT22   

#define MQTT_SERVER "test.mosquitto.org" //MQTT Server
#define MQTT_TOPIC "demo/arduino/airquality" //TOPIC to Publish

DHT dht(DHTPIN, DHTTYPE);


//Ethernet Shield Setup - Start.
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 200, 177); // Should be changed according to the subnet

IPAddress server (85,119,83,194);

//Setting up mqtt client
EthernetClient ethClient;
PubSubClient client(ethClient);

void setup() {
   // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Setting up server");
  Ethernet.begin(mac, ip);
  delay(2000);
  
  dht.begin();
  Serial.println("Setting up server");
  client.setServer(server,1883);
  MQTT_connect();
  Serial.println("Temp & Humidity Sensor Initailzied..");
 
}

void loop() {

  float h = dht.readHumidity();           
  float f = dht.readTemperature(true);
  if (isnan(h) ||isnan(f)) {
     Serial.println("Failed to read from DHT sensor!");
     return;
  } 

  String loc = "";
  //Build json object
 

  String jsonPayload = "{\"loc\":\"";
    jsonPayload += loc;
    jsonPayload +="\",\"temp\":\"";
    jsonPayload += f;
    jsonPayload +="\",\"humidity\":\"";
    jsonPayload += h;
    jsonPayload +="\"}";
  
  const char *payload = jsonPayload.c_str(); // convert string to const char*
  Serial.println(jsonPayload);
  //airquality.publish(jsonPayload);
  if(!client.connected()){
    Serial.println("Not Connected");
  }
  boolean result = client.publish(MQTT_TOPIC, payload);
  Serial.println(result);

  delay(2000);
}


// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  if(!client.connected()){
     Serial.println("Not connected");
     boolean connectionResult = client.connect("pjd-mqtt",MQTT_TOPIC,1,0,"Test Connection");
     Serial.println(connectionResult);
     Serial.println("MQTT Connected! ");    
  }
  
}


void callback(char* topic, byte* payload, unsigned int length) {

}

