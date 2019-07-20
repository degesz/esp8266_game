// Hardware setup:
//
//                         A0
//                          |
//                          |
//     3.3v  --------/\/\/\/\/\/\/---------gnd
//                   potentiometer
//
//
//  include libraries
#include <Ticker.h>
#include <WebSocketsServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// include the web page content 
#include "index.h" 
 
//SSID and Password of your WiFi router
const char* ssid = "your_ssid";                 //    CHANGE
const char* password = "your wifi password";    //     THIS
int value = 0;
 
ESP8266WebServer server(80); //Server on port 80

WebSocketsServer webSocket = WebSocketsServer(81); // Websockets server on port 81

Ticker timer; // setup timer object


//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}



//==============================================================
//                  SETUP
//==============================================================
void setup(void){

  timer.attach_ms(25, getData); //calls getData routine every 25ms
  Serial.begin(9600);  //setup serial connection
  webSocket.begin(); // initialize websockets
  webSocket.onEvent(webSocketEvent);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
 
  server.on("/", handleRoot);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  webSocket.loop();               //handle websockets
  server.handleClient();          //Handle client requests
}



void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){

  // I dont know what this does, but every websockets tutorial had this in the code
  
  }

  void getData(){ // this gets executed every 25ms

    //formatting data in JSON format
    String json = "{\"value\":";
    json += analogRead(A0);
    json += "}";
    
    webSocket.broadcastTXT(json.c_str(), json.length());  //sending data
    }
