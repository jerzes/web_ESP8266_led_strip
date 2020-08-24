#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 22

const char* ssid = "";    //  Your Wi-Fi Name
const char* password = "";   // Wi-Fi Password

int LED = 2;   // led connected to GPIO2 (D4)
Adafruit_NeoPixel pixels(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

ESP8266WebServer  server(80);
void setup()
{
  Serial.begin(9600);

  pixels.begin();
  uint8_t mac[6] {0x00, 0x1c, 0x10, 0xe6, 0x3a, 0xbf};
  wifi_set_macaddr(STATION_IF, mac);
  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("WiFi connected");
  server.begin();  // Starts the Server
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void handle_ledon() {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 204));
    pixels.show();
    server.send(200, "text/html", SendHTML(1));
  }
}

void handle_ledoff() {
  pixels.clear();
  pixels.show();
  server.send(200, "text/html", SendHTML(0));
}
void handle_OnConnect() {

  server.send(200, "text/html", SendHTML(0));
}
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
void loop()
{
  server.handleClient();
}

String SendHTML(uint8_t ledstat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

  if (ledstat)
  {
    ptr += "<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>LED Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
