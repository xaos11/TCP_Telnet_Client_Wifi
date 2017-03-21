#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

IPAddress server(192, 168, 1, 226); //could replace this with hostname, but it is good enough for my needs

WiFiClient client;

void setup() {
  // start the Ethernet connection:
  // Ethernet.begin(mac, ip); replaced this with wifi
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  WiFiMulti.addAP("SSID", "Password");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

 Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);
   Serial.println("connecting...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 8765)) { //port to connect to; telnet/TCP
    Serial.println("connected");
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
   pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    if (stringComplete) {
    Serial.println(inputString);
    if (inputString.indexOf("Command") >=0) {
          Serial.println("String does contain command!");
          digitalWrite(LED_BUILTIN ,!digitalRead(LED_BUILTIN));  
          // if input string contains "Command", toggle the LED on pin 13. I used this connected to an LIRC server to toggle the LED with an IR remote control. 
          // Node-red would probably be more suited for this, but this works well for standalone control of the MCU    
    }
    // clear the string:    
    inputString = "";
    stringComplete = false;
  }
  
  if (client.available()) { 
    char c = client.read();
    //Serial.print(c);  replaced this with above println(inputString) 

    inputString += c;
    if (c == '\n') {
      stringComplete = true;
      //if string is complete, this will trigger the loop at the top of the main loop
    }
    
  }
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while (true);
  }
}  
