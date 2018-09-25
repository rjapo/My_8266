#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char index_html[] PROGMEM={"<!Doctype html>\n<html>\n<style>\n.btn-group button {\n    background-color: #4CAF50; /* Green background */\n    border: 1px solid white; /* white border */\n    color: white; /* White text */\n    padding: 10px 10px; /* Some padding */\n    cursor: pointer; /* Pointer/hand icon */\n    width: 75%; /* Set a width if needed */\n    display: block; /* Make the buttons appear below each other */\n}\n\n.btn-group button:not(:last-child) {\n    border-bottom: none; /* Prevent double borders */\n}\n\n/* Add a background color on hover */\n.btn-group button:hover {\n    background-color: #3e8e41;\n}\n</style>\n<body>\n\n<h1>Select a Button</h1>\n\n<div class=\"btn-group\">\n  <button>Brando</button>\n  <button>Aaron</button>\n  <button>Cody</button>\n</div>\n\n</body>\n</html>\n"};
  
const char* ssid = "ApoNet";
const char* password = "8182893599";

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send_P(200, "text/html", index_html);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("[2J");
  Serial.println("Hello Rick, you are connected.");
  Serial.print("Connected to ");
 
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
