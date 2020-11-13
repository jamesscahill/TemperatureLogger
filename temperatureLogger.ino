#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include <WiFi.h>
#include <vector>
#include <RtcDS3231.h>
#include <Adafruit_AHTX0.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))



RtcDS3231<TwoWire> Rtc(Wire);



const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;

const char *ssid = "ESPUI";
const char *password = "espui";

const char *hostname = "espui";

// We need 35,040 reads
struct tempLogs
{

  std::vector<double> ahtTemp;
  std::vector<double> ahtHumidity;
  std::vector<double> lightMeter;
  std::vector<double> poolTemp;
  std::vector<double> grandstandTemp;
}tempLogContainer;

int statusLabelId;
int graphId;
int millisLabelId;
int testSwitchId;
char datestring[20];
int screen = 0; //controle de tela
int sda = 33;
int scl = 32; 
float tempRead;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
unsigned long del;
int period = 5000;



void getSensorReads(){




}


void saveData(){



}

void loadData(){

}

void currentTime(){
 RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();
    // RtcTemperature temp = Rtc.GetTemperature();
 //tempRead=temp.AsFloatDegC();
}



void printDateTime(const RtcDateTime& dt)
{
    

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}








void setup()
{
  ESPUI.setVerbosity(Verbosity::VerboseJSON);
  Serial.begin(115200);

  WiFi.setHostname(hostname);

  // try to connect to existing network
  WiFi.begin(ssid, password);
  Serial.print("\n\nTry to connect to existing network");

  {
    uint8_t timeout = 10;

    // Wait for connection, 5s timeout
    do
    {
      delay(500);
      Serial.print(".");
      timeout--;
    } while (timeout && WiFi.status() != WL_CONNECTED);

    // not connected -> create hotspot
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("\n\nCreating hotspot");

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid);

      timeout = 5;

      do
      {
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

  uint16_t tab1 = ESPUI.addControl(ControlType::Tab, "24 Hours", "24 Hours");
  uint16_t tab2 = ESPUI.addControl(ControlType::Tab, "Week", "Week");
  uint16_t tab3 = ESPUI.addControl(ControlType::Tab, "Month", "Month");
  uint16_t tab4 = ESPUI.addControl(ControlType::Tab, "Year", "Year");
  uint16_t tab5 = ESPUI.addControl(ControlType::Tab, "Settings", "Settings");

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

void loop()
{
  // put your main code here, to run repeatedly:
  dnsServer.processNextRequest();

  static long oldTime = 0;
  static bool testSwitchState = false;
}