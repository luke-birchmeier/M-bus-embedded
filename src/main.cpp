
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Henrys Phone";
const char *password = "Password";

const char *url = "https://mbus.ltp.umich.edu/bustime/api/v3/getpredictions?rt=BB&stpid=N411,N410&key=PVEAKStQa5ys9D4xcFDzxRz4W&format=json&top=10";

void setup()
{
  Serial.begin(115200);
  delay(10000);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20)
  {
    delay(500);
    Serial.print(".");
    retries++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("✅ Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("❌ Failed to connect to WiFi");
    int16_t results = WiFi.scanNetworks(false, true, false, 1000, 0, nullptr, nullptr);

    for (int i = 0; i < results; i++)
    {
      String ssid;
      uint8_t encType;
      int32_t rssi;
      uint8_t *bssid;
      int32_t channel;
      WiFi.getNetworkInfo(i, ssid, encType, rssi, bssid, channel);

      Serial.printf("%s %u %d %u %d\n", ssid, encType, rssi, bssid, channel);
    }
    Serial.printf("Finished %d\n", results);
    return;
  }

  // Make GET request
  HTTPClient http;

  Serial.println("Sending GET request...");
  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0)
  {
    Serial.printf("Response code: %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);
    }
  }
  else
  {
    Serial.printf("❌ HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void loop()
{
  // nothing here
}
