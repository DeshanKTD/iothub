#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>


/* ------------------ */
/* SKETCH CREDENTIALS */
/* ------------------ */

char* outTopic     = "arduinoOut/motor"; // * MQTT channel where physical updates are published
char* inTopic      = "arduinoIn/motor"; // * MQTT channel where lelylan updates are received
char* clientId     = "arduino";             // * set a random string (max 23 chars, will be the MQTT client id)


/* ------------ */
/* SKETCH LOGIC */
/* ------------ */

/* Server settings */
byte server[] = { 192, 168, 43, 252 }; // MQTT server address
IPAddress ip(172, 16, 0, 100);

char ssid[] = "AirDroidAP";     //  your network SSID (name)
char pass[] = "12345678";  // your network password
int keyIndex = 0;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

/* Sample payload published to lelylan */
/* The id is the status property id of the basic light /*
/* http://lelylan.github.io/types-dashboard-ng/#/types/518be107ef539711af000001/ */ 
char* payloadOn  = "{\"properties\":[{\"id\":\"518be5a700045e1521000001\",\"value\":\"on\"}]}";
char* payloadOff = "{\"properties\":[{\"id\":\"518be5a700045e1521000001\",\"value\":\"off\"}]}";

/* Ethernet configuration */
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x4B, 0xC2  };
WiFiClient wifiClient;

/* MQTT communication */
void callback(char* topic, byte* payload, unsigned int length); // subscription callback
PubSubClient client(server, 1883, callback, wifiClient);         // mqtt client

/* Pins configuration */

int const pwm = 5;
int const dir1 = 2;
int const dir2 = 3;

//fade
//int const pin6 = 6;

//fade parameters
int speed_motor = 0;
int fadeamount = 10;
String inputString = "";

/* Button and led logics */
int state = HIGH;     // current state of the output pin
int reading;          // current reading from the input pin
int previous = LOW;   // previous reading from the input pin
long time = 0;        // the last time the output pin was toggled
long debounce = 200;  // the debounce time, increase if the output flickers

/* arduino setup */
void setup() {
  Serial.begin(9600);
  delay(500);

   // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  printWifiStatus();

  //pinMode initialization
  
  pinMode(pwm,OUTPUT);
  pinMode(dir1,OUTPUT);
  pinMode(dir2,OUTPUT);
  

  //digitalWrite(pin3,HIGH);

  //Ethernet.begin(mac,ip);
  Serial.println("Connected with IP: ");
  //Serial.println(Ethernet.localIP());

  lelylanConnection();      // MQTT server connection
  analogWrite(pwm,speed_motor);

  //lelylanConnection();
       digitalWrite(dir1,LOW);
       digitalWrite(dir2,HIGH);
       analogWrite(pwm,100);
  
}

/* arduino loop */
void loop() {
  
    lelylanConnection();
    //Serial.println(client.connected());
//      if(digitalRead(dir1) == 1 && digitalRead(dir2) == 0){
//        digitalWrite(dir1,LOW);
//        digitalWrite(dir2,HIGH);
//        Serial.println("motor 1 back");
//        //lelylanPublish("motor 1 back");
//      }
//
//       else if(digitalRead(dir1) == 0 && digitalRead(dir2) == 1){
//        digitalWrite(dir1,HIGH);
//        digitalWrite(dir2,LOW);
//        Serial.println("motor 1 forward");
//        //lelylanPublish("motor 1 forward");
//      }
//      delay(5000);
  
}

/* MQTT server connection */
void lelylanConnection() {

  // add reconnection logics
  if (!client.connected()) {
    // connection to MQTT server
    if (client.connect(clientId)) {
      Serial.println("[PHYSICAL] Successfully connected with MQTT");
      lelylanSubscribe(); // topic subscription
    }
    //Serial.println(client.connected());
  }
  client.loop();
  
}

/* MQTT publish */
void lelylanPublish(char* value) {
    client.publish(outTopic, value); // light on
}

/* MQTT subscribe */
void lelylanSubscribe() {
  client.subscribe(inTopic);
}

/* Receive Lelylan message and confirm the physical change */
void callback(char* topic, byte* payload, unsigned int length) {
  // copu the payload content into a char*
  char* json;
  json = (char*) malloc(length + 1);
  memcpy(json, payload, length);
  json[length] = '\0';
  //Serial.println(String(json).substring(0,5));
  //Serial.println(String("blink") == String(json).substring(0,5));

  // update the physical status and confirm the executed update
 
  //speed control
 if(String("speed") == String(json).substring(0,5)){
    if(String(json).charAt(6) == '2'){
      inputString = String(json).substring(8,11);
      analogWrite(pwm,inputString.toInt());
      Serial.println("bulb speed 1");
       char reply[] = "{\"userId\":8,\"deviceId\":2,\"deviceState\":{\"stateId\":1,\"stateName\":\"on\"}}";
        lelylanPublish(reply);
      delay(500);
      inputString="";
    }
  }

  //direction control
if(String("direc") == String(json).substring(0,5)){
    if(String(json).charAt(6) == '2'){
      if(digitalRead(dir1) == HIGH && digitalRead(dir2) == LOW){
        digitalWrite(dir1,LOW);
        digitalWrite(dir2,HIGH);
        Serial.println("motor 1 back");
        char reply[] = "{\"userId\":8,\"deviceId\":2,\"deviceState\":{\"stateId\":1,\"stateName\":\"on\"}}";
        lelylanPublish(reply);
      }

       if(digitalRead(dir1) == LOW && digitalRead(dir2) == HIGH){
        digitalWrite(dir1,HIGH);
        digitalWrite(dir2,LOW);
        Serial.println("motor 1 forward");
        char reply[] = "{\"userId\":8,\"deviceId\":2,\"deviceState\":{\"stateId\":1,\"stateName\":\"on\"}}";
        lelylanPublish(reply);
      }
      delay(500);
    }  
  }

  //digitalWrite(pin3, state);
  free(json);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
