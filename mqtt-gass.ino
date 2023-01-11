#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Kontrakan Elite";
const char* password = "oteoteanget";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int sensorPin= A0;
float liquid_level;
int liquid_graph;
const int LedHijau = D1;
const int LedBiru = D2;
const int LedMerah = D3;
const int LedKuning = D0;
const int Buzzer = D7;
const int rainDrop = D6;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//===========================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) { 
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");    
      // ... and resubscribe
      client.subscribe("ESP32");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
   Serial.begin(9600);
   setup_wifi();
   client.setServer(mqtt_server, 1883);  
   delay(3000);
   pinMode(LedMerah, OUTPUT);
   pinMode(LedHijau, OUTPUT) ;
   pinMode(LedBiru, OUTPUT) ;
   pinMode(LedKuning, OUTPUT) ;
   pinMode (Buzzer, OUTPUT);
   pinMode(sensorPin, INPUT);
   pinMode(rainDrop, INPUT);
   // initialize the LED pin as an output:
   
   }
 
 
void loop() {
   float analogValue = analogRead(sensorPin);
   float digitalValue = digitalRead(rainDrop);

   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (isnan(analogValue)){
    Serial.println("Water Sensor tidak terbaca... !");
    return;
  }
  else if (isnan(digitalValue)){
    Serial.println("Rain Sensor tidak terbaca... !");
    return;
  }
  else { 
  
  sprintf(msg, "%.2f", analogValue);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("Volume", msg);  
  sprintf(msg, "%.2f", digitalValue);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("Kondisi", msg);
}
   // if the analog value is high enough, turn on the LED:
  if (analogValue >= 10 && analogValue < 200){
    digitalWrite(LedHijau, HIGH );
    digitalWrite(LedBiru, LOW );
    digitalWrite(LedMerah, LOW );
    digitalWrite (Buzzer, LOW);
    Serial.print("Ketinggian Air: ");
    Serial.println(analogValue);
    Serial.print("Kondisi Cuaca: ");
    Serial.println(digitalValue);
    client.publish("banjir","Aman cuyy!!");
    }else if (analogValue >= 200 && analogValue < 400){
      digitalWrite (LedBiru, HIGH);
      digitalWrite(LedHijau, LOW );
      digitalWrite(LedMerah, LOW );
      digitalWrite(Buzzer, LOW );
      Serial.print("Ketinggian Air: ");
      Serial.println(analogValue);
      Serial.print("Kondisi Cuaca: ");
      Serial.println(digitalValue);
      Serial.println("AWAS BANJIR CUYY !!!");
      client.publish("banjir","Awas Banjir cuyy!!");
      }else if (analogValue >= 400 && analogValue > 450 ){
        digitalWrite (LedMerah, HIGH );
        digitalWrite(LedBiru, LOW );
        digitalWrite(LedHijau, LOW );
        digitalWrite (Buzzer, HIGH);
        Serial.println("BANJIR CUY BANJIR !!!");
        client.publish("banjir","Banjir cuyy!!");
        }else if (analogValue < 10){
            digitalWrite (LedMerah, LOW );
            digitalWrite (LedHijau, LOW );
            digitalWrite (LedKuning, LOW);
            digitalWrite (LedBiru, LOW );
            digitalWrite (Buzzer, HIGH);
            }
if (digitalValue == 0){
          digitalWrite (LedMerah, HIGH);
          digitalWrite (LedKuning, HIGH );
          digitalWrite(LedBiru, LOW );
          digitalWrite(LedHijau, LOW );
          digitalWrite (Buzzer, LOW);
          Serial.println("AWAS UJAN KARENA BANJIR !!!");
          client.publish("hujan","HUJAN CUYYY!!");

} else if(digitalValue ==1){
        digitalWrite (LedMerah, LOW);
          digitalWrite (LedKuning, LOW );
          digitalWrite(LedBiru, LOW );
          digitalWrite(LedHijau, LOW );
          digitalWrite (Buzzer, HIGH);
          Serial.println("TIDAK UJAN !!!");
          client.publish("hujan","TIDAK HUJAN CUYY!!");
}
 }
  
