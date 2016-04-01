

#include <SPI.h>
#include <WiFi.h>

char ssid[] = "AirDroidAP"; //  your network SSID (name) 
char pass[] = "12345678";    // your network password (use for WPA, or use as key for WEP)

int keyIndex = 0;            // your network key Index number (needed only for WEP)

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

int status = WL_IDLE_STATUS;

WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);
  pinMode(pin5,OUTPUT);
  pinMode(pin5,OUTPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid,pass);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
  analogWrite(pin6,brightness);
  analogWrite(pin5,brightness);
 }


void loop() {
  // wait for a new client:
  WiFiClient client = server.available();


  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      client.flush();    
      Serial.println("We have a new client");
      //client.println("Hello, client!"); 
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      
      //blink bulb pin1
      if(thisChar=='1'){
        server.write("pin2 blinked");
        digitalWrite(pin1,HIGH);
        delay(1000);
        digitalWrite(pin1,LOW);
        delay(1000);
      }
      
      //blink bulb pin 3
      else if(thisChar=='2'){
        server.write("pin3 blinked");
        digitalWrite(pin2,HIGH);
        delay(1000);
        digitalWrite(pin2,LOW);
        delay(1000);
      }
      
      //on off bulb -pin 4
      else if(thisChar=='3'){
        if(digitalRead(pin3)==HIGH){
          server.write("pin4 off");
          digitalWrite(pin3,LOW);
          delay(1000);
        }
        else{
          server.write("pin4 on");
          digitalWrite(pin3,HIGH);
          delay(1000);
        }
      }
      
      //on off bulb condition - pin 5
      else if(thisChar=='4'){
        if(digitalRead(pin4)==HIGH){
          server.write("pin5 off");
          digitalWrite(pin4,LOW);
          delay(1000);
        }
        else{
          server.write("pin5 on");
          digitalWrite(pin4,HIGH);
          delay(1000);
        }
      }
      //fade the bulb 
      else if(thisChar=='5'){
        char fadeChar = client.read();
        if(fadeChar == 'i'){
          brightness = brightness + fadeamount;
          server.write(brightness);
          //Serial.write(brightness);
          analogWrite(pin5,brightness);
          delay(1000);
          //analogWrite(pin5,100);
        }
        else if(fadeChar =='d'){
          brightness = brightness - fadeamount;
          server.write(brightness);
          analogWrite(pin5,brightness);
          delay(1000);
        }
        else{
          server.write("invalid command");
        }
          
      }
      
      else if(thisChar=='6'){
        inputString += (char)client.read();
        inputString += (char)client.read();
        inputString += (char)client.read();
        Serial.print("i"+inputString.toInt());
        server.write("changed");
        analogWrite(pin6,inputString.toInt());
        delay(1000);
        inputString="";
        
      }
      
      
      //if no comand
      else{
        server.write("no pin to oprate");
      }
      
      
      
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }
  delay(250);
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


