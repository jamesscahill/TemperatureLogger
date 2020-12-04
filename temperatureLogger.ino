#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <DNSServer.h>
#include <ESPUI.h>
#include <WiFi.h>
#include <vector>
#include <RtcDS3231.h>
#include <Adafruit_AHTX0.h>
#include "Adafruit_VEML7700.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

int hourUpdate;
bool hourUpdated=false;
int minuteUpdate;
bool minuteUpdated=false;
int secondUpdate;
bool secondUpdated=false;


int dayUpdate;
bool dayUpdated=false;
int monthUpdate;
bool monthUpdated=false;
int yearUpdate;
bool yearUpdated = false;

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
   std::vector<double> lux;
}tempLogContainer;

uint16_t button1;
uint16_t button2;
uint16_t timeStatus;
uint16_t yearStatus;


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




void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}








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

void setTime(){


}


void hourCall( Control* sender, int type ) {
  Serial.println( sender->value );
  int hour = sender->value.toInt();
  if(hour >=0 && hour <=23){
  hourUpdate = hour;
  hourUpdated = true;
  }

}

void minuteCall( Control* sender, int type ) {
  Serial.println( sender->value );
  int minute = sender->value.toInt();
  if(minute >=0 && minute <=59){
  minuteUpdate = minute;
  minuteUpdated = true;

  }
}

void secondCall( Control* sender, int type ) {
  Serial.println( sender->value );
  int second = sender->value.toInt();
  if(second >=0 && second <=59){
  minuteUpdate = second;
  minuteUpdated = true;

  }
}

void dayCall( Control* sender, int type ) {
  Serial.println( sender->value );
   int day = sender->value.toInt();
  if(day >=1 && second <=31){
  day = second;
  minuteUpdated = true;

  }
}

void monthCall( Control* sender, int type ) {
  Serial.println( sender->value );
  int month = sender->value.toInt();
  if(month >=1 && second <=12){
  monthUpdate = month;
  monthUpdated = true;

  }
}

void yearCall( Control* sender, int type ) {
  Serial.println( sender->value );
  yearUpdate = sender->value.toInt();
  yearUpdated = true;
}

void timeButton( Control* sender, int type ) {
  switch ( type ) {
    case B_DOWN:
      Serial.println( "Status: Start" );
      ESPUI.updateControlValue( timeStatus, "Updated" );
    
      ESPUI.getControl( button1 )->color = ControlColor::Carrot;
      ESPUI.updateControl( button1 );
      break;

    case B_UP:
      Serial.println( "Status: Stop" );
      //ESPUI.updateControlValue( status, "Stop" );
   
      ESPUI.getControl( button1 )->color = ControlColor::Peterriver;
      ESPUI.updateControl( button1 );
      break;
  }
}

void yearButton( Control* sender, int type ) {
  switch ( type ) {
    case B_DOWN:
      Serial.println( "Status: Start" );
      ESPUI.updateControlValue( yearStatus, "Updated" );
    
      ESPUI.getControl( button2 )->color = ControlColor::Carrot;
      ESPUI.updateControl( button2 );
      break;

    case B_UP:
      Serial.println( "Status: Stop" );
     
   
      ESPUI.getControl( button2 )->color = ControlColor::Peterriver;
      ESPUI.updateControl( button2 );
      break;
  }
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

timeStatus = ESPUI.addControl(ControlType::Label, "Update Time","-", ControlColor::Alizarin, tab5);
 ESPUI.addControl( ControlType::Number, "Hour (0-23):", "0", ControlColor::Alizarin, tab5, &hourCall );
 ESPUI.addControl( ControlType::Number, "Minutes (0-59):", "0", ControlColor::Alizarin, tab5, &minuteCall );
 ESPUI.addControl( ControlType::Number, "Seconds (0-59):", "0", ControlColor::Alizarin, tab5, &secondCall );
 button1 =ESPUI.addControl( ControlType::Button, "Change Time", "Confirm", ControlColor::Wetasphalt, tab5, &timeButton );
 
 yearStatus = ESPUI.addControl(ControlType::Label, "Update Year","-" ,ControlColor::Alizarin, tab5);
 ESPUI.addControl( ControlType::Number, "Day", "0", ControlColor::Alizarin, tab5, &dayCall );
 ESPUI.addControl( ControlType::Number, "Month", "0", ControlColor::Alizarin, tab5, &monthCall );
 ESPUI.addControl( ControlType::Number, "Year", "0", ControlColor::Alizarin, tab5, &yearCall );
 button2 = ESPUI.addControl( ControlType::Button, "Change Date", "Confirm", ControlColor::Wetasphalt, tab5, &yearButton );







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