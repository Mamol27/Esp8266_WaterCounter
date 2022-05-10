#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "DataFlow.h"
#include "MemValue.h"
#include "index.h"

// d1, d2, d3, d4, d5, d6, d7 - works on fall with mode INPUT_PULLUP
// I dont recomend to use other pins
// INPUT_PULLUP works fine with ground connection
// d4 == LED_BUILTIN
// d3 == FLASH
uint8_t DigitalPin[9] = {16, 5, 4, 0, 2, 14, 12, 13, 15};

struct Counter {
  unsigned long bounceDelay = 20;
  unsigned long highTime = 0;
  byte pin = 0;
  bool prevPosition = false;
  bool position = false;

  Counter(byte pinNumber): pin(pinNumber) {}
};

Counter clodWaterCounter(DigitalPin[1]);
Counter hotWaterCounter(DigitalPin[2]);

uint8_t RESET_BUTTON = 0;
unsigned long resetPressTime = 0;
bool resetPressed = false;
unsigned long holdFlashToReset = 5000; // hold flash button 5 seconds to reset controller

ESP8266WebServer server(80); // Server on port 80

unsigned int trueIS = 6861621; //random number, just punch keyboard
unsigned int offsetStarting = sizeof(unsigned int);

byte maxStringLen = 32;

DataFlow dataFlow{};
UintValue clientConfigured(&dataFlow);
UintValue coldWater(&dataFlow);
UintValue hotWater(&dataFlow);

StringValue pointName(&dataFlow, maxStringLen);
StringValue ssid(&dataFlow, maxStringLen);
StringValue password(&dataFlow, maxStringLen);
StringValue IP_client(&dataFlow, maxStringLen);

void (*resetFunc)(void) = 0; // restart controller

void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(clodWaterCounter.pin, INPUT_PULLUP);
  pinMode(hotWaterCounter.pin, INPUT_PULLUP);
  pinMode(RESET_BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(BUILTIN_LED, LOW);

  coldWater.load(offsetStarting);
  hotWater.load(coldWater.getEndOfRecord());
  clientConfigured.load(hotWater.getEndOfRecord());
  pointName.setOffset(clientConfigured.getEndOfRecord());

  if (clientConfigured.getValue() != trueIS) {
    configureClient();
  }

  pointName.load(clientConfigured.getEndOfRecord());
  
  ssid.load(pointName.getEndOfRecord());
  password.load(ssid.getEndOfRecord());
  IP_client.load(password.getEndOfRecord());

  setupClient();
  digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
  server.handleClient(); // Handling of incoming client requests
  
  resetSettings();

  processCounterOnFall(&clodWaterCounter, &coldWater);
  processCounterOnFall(&hotWaterCounter, &hotWater);

  delay(100); //energy economy
}

void resetSettings() {
  if (digitalRead(RESET_BUTTON) == 0) { // button works on fall
    if (!resetPressed) {
      resetPressed = true;
      resetPressTime = millis();
    }
    if (millis() - resetPressTime >= holdFlashToReset){
      clientConfigured.setValue(0); // after reset u will need to configure all again
      resetFunc();
    }
    Serial.println(millis() - resetPressTime >= holdFlashToReset);
  } else {
    resetPressed = false;
  }
}

void processCounterOnFall(Counter *counter, UintValue *value){

  if (millis() - counter->highTime <= counter->bounceDelay) {
    return;
  }
  counter->highTime = millis();

  counter->prevPosition = counter->position;
  counter->position = !digitalRead(counter->pin);

  if (counter->position && 
      !counter->prevPosition) 
  {
    value->setValue(value->getValue() + 1);
  }

}

void configureClient()
{

  const char *serverSsid = "ESPWebServer";
  const char *serverPassword = "test1234";

  WiFi.mode(WIFI_AP); // Only Access point

  IPAddress apIP = IPAddress(192, 168, 4, 1);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // subnet FF FF FF 00

  WiFi.softAP(serverSsid, serverPassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRootConfig);
  server.on("/config_page", handleSetup);
  server.begin();

  bool bblink = LOW;
  while (clientConfigured.getValue() != trueIS) {
    server.handleClient();
    delay(500);
    bblink = !bblink;
    digitalWrite(BUILTIN_LED, bblink);
  }
  resetFunc();
}

void handleRootConfig() 
{
  String s = PAGE_configuration;    // Read HTML contents
  server.send(200, "text/html", s); // Send web page
}

void handleSetup() 
{
  Serial.println("SAVING WIFI DATA");

  pointName.setValue(server.arg("pointName"));

  ssid.setOffsetAndValue(pointName.getEndOfRecord(), server.arg("ssid"));
  password.setOffsetAndValue(ssid.getEndOfRecord(), server.arg("password"));
  IP_client.setOffsetAndValue(password.getEndOfRecord(), server.arg("ip"));

  clientConfigured.setValue(trueIS);

  Serial.println("DATA SAVED");

  String s = "<h2><strong>Restarting</strong></h2>";
  server.send(200, "text/html", s); // Send web page
}

void setupClient() 
{
  Serial.printf("Connecting to %s\n", ssid.getValue());
  IPAddress apIP = parseIpFromString(IP_client.getValue());
  WiFi.config(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.begin(ssid.getValue(), password.getValue());

  // if you press reset button while controller fast blinking
  // it will reset all settings and you have to configure all again
  clientConfigured.setValue(0);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED)); // blink
  }
  clientConfigured.setValue(trueIS);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Print the local IP

  server.on("/", handleIndex); // Handle Index page
  server.on("/get/coldWater", handleColdWater);
  server.on("/get/hotWater", handleHotWater);
  server.on("/setCountersValue", handleRootSettings);
  server.on("/setCountersValue/ok", handleSetCountersValue);

  server.begin();
  Serial.println("Server listening");

  digitalWrite(BUILTIN_LED, LOW);
}

IPAddress parseIpFromString(String data)
{
  int8_t IpOctas[4];
  int8_t found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex; i++){
    if(data.charAt(i)=='.' || i==maxIndex){
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;

        IpOctas[found] = data.substring(strIndex[0], strIndex[1]).toInt();
        found++;
    }
  }

  return IPAddress(IpOctas[0], IpOctas[1], IpOctas[2], IpOctas[3]);
}

void handleIndex() {
  String s = PAGE_main;             // Read HTML contents
  server.send(200, "text/html", s); // Send web page
}

// Send value only to client ajax request
void handleColdWater() 
{
  char send[6];
  sprintf(send, "%06d", coldWater.getValue());
  server.send(200, "text/plane", send);
}

void handleHotWater() 
{
  char send[6];
  sprintf(send, "%06d", hotWater.getValue());
  server.send(200, "text/plane", send);
}

void handleRootSettings() 
{
  server.send(200, "text/html", PAGE_setCountersValue);
}

void handleSetCountersValue() 
{
  coldWater.setValue(server.arg("coldWaterSetting").toInt());
  hotWater.setValue(server.arg("hotWaterSetting").toInt());

  server.send(200, "text/html", PAGE_SettingsOk); 
}
