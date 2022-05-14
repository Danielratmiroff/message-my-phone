#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const int LED = 2;
const int BUTTON = 4;
const char *ssid = "FRITZ!Box 7560 QT";
const char *pass = "62091186156206291321";
String url = "http://api.callmebot.com/text.php?source=web&user=@danielratmiroff&text=Dont%20worry%20Daniel,%20I%20Gotcha!";
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
      if (http.begin(client, url))
      {

        digitalWrite(LED, LOW);
        int httpCode = http.GET();
        for (int i = 1; i < 7; i++)
        {
          delay(4500);
          Serial.print(i);
          Serial.println(" /GET Request made");
          http.GET();
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