#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


/* ------------------ */
/* SKETCH CREDENTIALS */
/* ------------------ */

char* outTopic     = "arduinoOut/led"; // * MQTT channel where physical updates are published
char* inTopic      = "arduinoIn/led"; // * MQTT channel where lelylan updates are received
char* clientId     = "arduino";             // * set a random string (max 23 chars, will be the MQTT client id)


/* ------------ */
/* SKETCH LOGIC */
/* ------------ */

/* Server settings */
byte server[] = { 172, 16, 0, 2 }; // MQTT server address
IPAddress ip(172, 16, 0, 100);

/* Sample payload published to lelylan */
/* The id is the status property id of the basic light /*
/* http://lelylan.github.io/types-dashboard-ng/#/types/518be107ef539711af000001/ */ 
char* payloadOn  = "{\"properties\":[{\"id\":\"518be5a700045e1521000001\",\"value\":\"on\"}]}";
char* payloadOff = "{\"properties\":[{\"id\":\"518be5a700045e1521000001\",\"value\":\"off\"}]}";

/* Ethernet configuration */
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x4B, 0xC2  };
EthernetClient ethClient;

/* MQTT communication */
void callback(char* topic, byte* payload, unsigned int length); // subscription callback
PubSubClient client(server, 1883, callback, ethClient);         // mqtt client

/* Pins configuration */
int const pin1 = 2;          //blink
int const pin2 = 3;

int const pin3 = 4;          //on off
int const pin4 = 8;

int const pin5 = 5;          //fade
int const pin6 = 6;

//fade parameters
int brightness = 0;
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

  //pinMode initialization
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
  pinMode(pin5,OUTPUT);
  pinMode(pin6,OUTPUT);

  //digitalWrite(pin3,HIGH);

  Ethernet.begin(mac,ip);
  Serial.print("Connected with IP: ");
  Serial.println(Ethernet.localIP());

  lelylanConnection();      // MQTT server connection
  analogWrite(pin6,brightness);
  analogWrite(pin5,brightness);
}

/* arduino loop */
void loop() {
  lelylanConnection();

//  char* value;
//  reading = digitalRead(pin3);  // read the button state
//
////  // if the input just went from LOW and HIGH and we've waited long enough to ignore
////  // any noise on the circuit, toggle the output pin and remember the time
//  if (reading == HIGH && previous==LOW && millis() - time > debounce) {
//    if (state == LOW) {
//      Serial.println("Led turned on");
//      lelylanPublish("on");
//      state = HIGH;
//      digitalWrite(pin3,state);
//    } else {
//      Serial.println("Led turned off");
//      lelylanPublish("off");
//      state = LOW;
//      digitalWrite(pin3,state);
//    }
//
//    time = millis();
//  }

//  // effectively update the light status
//  digitalWrite(outPin, state);
//  previous = reading;

  
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

  //for blink
  if (String("blink") == String(json).substring(0,5)) {
    if(String(json).charAt(6) == '1'){
        digitalWrite(pin1,HIGH);
        delay(1000);
        digitalWrite(pin1,LOW);
        delay(1000);
        Serial.println("led 1 blinked");
        lelylanPublish("led 1 blinked");
    }

    else if(String(json).charAt(6) == '2'){
        digitalWrite(pin2,HIGH);
        delay(1000);
        digitalWrite(pin2,LOW);
        delay(1000);
        Serial.println("led 2 blinked");
        lelylanPublish("led 2 blinked");        
      }
  }


//switch bulb //pin 4
  else if (String("switch") == String(json).substring(0,6)) {
    if(String(json).charAt(7) == '1'){
      if(digitalRead(pin3)==HIGH){
          digitalWrite(pin3,LOW);
          delay(1000);
          Serial.println("led 1 off");
          lelylanPublish("led 1 off");  
        }
        else{
          digitalWrite(pin3,HIGH);
          delay(1000);
          Serial.println("led 1 on");
          lelylanPublish("led 1 on");
        }
    }
    //pin5
    else if(String(json).charAt(7) == '2'){
      if(digitalRead(pin4)==HIGH){
          digitalWrite(pin4,LOW);
          delay(1000);
          Serial.println("led 2 off");
          lelylanPublish("led 2 off");  
        }
        else{
          digitalWrite(pin4,HIGH);
          delay(1000);
          Serial.println("led 2 on");
          lelylanPublish("led 2 on");
        }
      }
  }
  else if(String("fade") == String(json).substring(0,4)){
    if(String(json).charAt(5) == '1'){
      inputString = String(json).substring(7,10);
      analogWrite(pin5,inputString.toInt());
      Serial.println("bulb fade 1");
      lelylanPublish("bulb fade 1");
      delay(500);
      inputString="";
    }
  
     if(String(json).charAt(5) == '2'){
      inputString = String(json).substring(7,10);
      analogWrite(pin6,inputString.toInt());
      Serial.println("bulb fade 2");
      lelylanPublish("bulb fade 2");
      delay(500);
      inputString="";
    }  
  }

  //digitalWrite(pin3, state);
  free(json);
}
