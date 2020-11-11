#include <DNSServer.h>
#include <ESPUI.h>
#include <WiFi.h>
#include <vector>





const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;



const char *ssid = "ESPUI";
const char *password = "espui";

const char *hostname = "espui";


// We need 35,040 reads
struct tempLogs{
    
    std::vector<double> dhtTemp;
    std::vector<double> poolTemp;
}



int statusLabelId;
int graphId;
int millisLabelId;
int testSwitchId;




void setup() {
   ESPUI.setVerbosity(Verbosity::VerboseJSON);
  Serial.begin(115200);

#if defined(ESP32)
  WiFi.setHostname(hostname);
#else
  WiFi.hostname(hostname);
#endif

  // try to connect to existing network
  WiFi.begin(ssid, password);
  Serial.print("\n\nTry to connect to existing network");

  {
    uint8_t timeout = 10;

    // Wait for connection, 5s timeout
    do {
      delay(500);
      Serial.print(".");
      timeout--;
    } while (timeout && WiFi.status() != WL_CONNECTED);

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("\n\nCreating hotspot");

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid);

      timeout = 5;

      do {
        delay(500);
        Serial.print(".");
        timeout--;
      } while (timeout);
    }
  }

  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("\n\nWiFi parameters:");
  Serial.print("Mode: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? "Station" : "Client");
  Serial.print("IP address: ");
  Serial.println(WiFi.getMode() == WIFI_AP ? WiFi.softAPIP() : WiFi.localIP());

 
  /*
   * .begin loads and serves all files from PROGMEM directly.
   * If you want to serve the files from SPIFFS use ESPUI.beginSPIFFS
   * (.prepareFileSystem has to be run in an empty sketch before)
   */
 
  // Enable this option if you want sliders to be continuous (update during move) and not discrete (update on stop)
  // ESPUI.sliderContinuous = true;

  /*
   * Optionally you can use HTTP BasicAuth. Keep in mind that this is NOT a
   * SECURE way of limiting access.
   * Anyone who is able to sniff traffic will be able to intercept your password
   * since it is transmitted in cleartext. Just add a string as username and
   * password, for example begin("ESPUI Control", "username", "password")
   */


  ESPUI.begin("ESPUI Control");

}

void loop() {
  // put your main code here, to run repeatedly:
 dnsServer.processNextRequest();

  static long oldTime = 0;
  static bool testSwitchState = false;

}