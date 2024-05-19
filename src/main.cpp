#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int LED = 2;    // ESP8266's led
const int BUTTON = 4; // button gpio
const char *ssid = ""; // wifi name
const char *pass = ""; // wifi password
const char *username = ""; 
const char *message = "";

String url = "http://api.callmebot.com/start.php?user=" + String(username) + "&text=" + String(message);

int button_state = 0;

HTTPClient http;
WiFiClient client;

void printWifiStatus()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal Strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println("WiFi shield not present");
  }

  Serial.println();
  Serial.print("\r\n");
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, pass);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10000);
    Serial.print(++i);
    Serial.print(" ");
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
  }

  Serial.println();
  Serial.println("Connected to WiFi");
  printWifiStatus();
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    button_state = digitalRead(BUTTON);
    if (button_state == 1)
    {
      digitalWrite(LED, HIGH);
      if (http.begin(client, url))
      {
        int httpCode = http.GET();
        Serial.println(" /GET Request made");
        
        unsigned long startTime = millis();
        while (millis() - startTime < 3500) {
          digitalWrite(LED, LOW);
          delay(100);
          digitalWrite(LED, HIGH);
          delay(100);
        }
        
        Serial.println("Response code: " + String(httpCode));
        http.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
      delay(200);
    }
    else
    {
      digitalWrite(LED, HIGH);
      delay(200);
    }
  }
}