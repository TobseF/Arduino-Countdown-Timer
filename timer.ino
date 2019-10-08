#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <aREST.h>
​
// Arduino countdown timer for the "SparkFun 7-Segment Serial Display".
// The countdown can be controlled by the REST interface:
// `start`   (POST) time [ms], Starts the countdown timer with the gicen sart time.
// `stop`    (POST) time [ms], Stops the countdown timer and displays the finish time.
// `resume`  (GET)  Restarts the contdown.
// `disable` (GET)  Deactivates the timer, so all lights are off.

// These are the Arduino pins required to create a software seiral instance.
const int softwareTx = 16;
// Unsed
const int softwareRx = 7;
​
SoftwareSerial s7s(softwareRx, softwareTx);
// Create aREST instance
aREST rest = aREST();
​
// WiFi parameters
const char* ssid = "WLAN-Name";
const char* password = "WLAN-PWD";
​
int start(String msecs);
int stop(String dummy);
int resume(String dummy);
int disable(String dummy);
​
// The port to listen for incoming TCP connections
#define LISTEN_PORT           5001
​
#define INITIALIZING          0
#define CONNECTED             1
#define RUNNING               2
#define PAUSED                3
#define DISABLED              4
#define FAILING               5
#define EXPLODED              6
​
#define SCROLLSPEED           500
​
WiFiServer server(LISTEN_PORT);
​
unsigned int inputMillis = 0;
unsigned int remainingMillis = 0;
unsigned long timeStarted = 0;
​
char tempString[10];
​
short state = INITIALIZING;
​
void setup(){
  s7s.begin(9600);
  
  // Clears display, resets cursor
  clearDisplay(); 
​
  // Set hightes brightness
  setBrightness(255);  
​
  // Start Serial
  Serial.begin(115200);
  
  // Exposed REST functions
  rest.function("start", start);
  rest.function("stop", stop);
  rest.function("resume", resume);
  rest.function("disable", disable);
​
  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id("bombTimer");
  rest.set_name("esp8266");
​
  // Init WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    scrollPrint("COnnECtInG", SCROLLSPEED, 1);
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  state = CONNECTED;
​
  // Start the server
  server.begin();
  Serial.println("Server started");
​
  // Print the IP address
  Serial.println(WiFi.localIP());
​
  delay(1000);
  clearDisplay();
}
​
void scrollPrint(const char *myWord, short scrollspeed, short times) {
​
  // Modify the input string to add two spaces before and three spaces after it
  char *modified;
  modified = (char *) malloc(strlen(myWord) + 6);
  strcpy(modified, "  ");
  strcat(modified, myWord);
  strcat(modified, "   ");
​
  // Skip the 2 spaces at the beginning
  short cursor = 2;
​
  for (short c = 0; c < times; c++) {
    short end = 3;
    short endTrigger = 0;
​
    while (end > 0) {
      char temp[5];
      for (short i = 0; i < 4; i++) {
        if (modified[cursor + i] == 0) {
          cursor = 2 - i;
          endTrigger = 1;
        }
        temp[i] = modified[cursor + i];
      }
​
      end -= endTrigger;
​
      temp[4] = 0;
​
      s7s.print(temp);
​
      cursor++;
      delay(scrollspeed);
    }
  }
}
​
void loop(){
​
  if (state == CONNECTED) {
    scrollPrint("rEAdY", SCROLLSPEED, 1);
  } else if (state == RUNNING) {
​
    remainingMillis = inputMillis - (millis() - timeStarted);
    unsigned int remainingSeconds = remainingMillis / 1000;
​
    if (remainingSeconds >= 60) {
      unsigned int printedMinutes = 0;
      unsigned int printedSeconds = 0;
      printedMinutes = remainingSeconds / 60;
      printedSeconds = remainingSeconds - (printedMinutes * 60);
​
      blinkColons(remainingSeconds);
​
      sprintf(tempString, "%02d%02d", printedMinutes, printedSeconds);
    } else {
      unsigned int printedMillis = remainingMillis / 10;
      blinkColons(remainingMillis / 250);
      sprintf(tempString, "%04d", printedMillis);
    }
​
    s7s.print(tempString);
​
​
​
    if (remainingSeconds == 0) {
      explode();
    }
​
  } else if (state == PAUSED) {
    s7s.print(tempString);
  } else if (state == DISABLED) {
    clearDisplay();
  } else if (state == FAILING) {
    scrollPrint("Error", SCROLLSPEED, 1);
  } else if (state == EXPLODED) {
    scrollPrint("Reached Zero", SCROLLSPEED, 1);
  }
​
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
​
}
​
void explode() {
  clearDisplay();
  state = EXPLODED;
}
​
int resume(String dummy) {
  if (state == PAUSED && inputMillis != 0) {
    state = RUNNING;
    inputMillis = remainingMillis;
    timeStarted = millis();
    return 1;
  }
  return 0;
}
​
int stop(String dummy) {
  if (inputMillis != 0) {
    state = PAUSED;
    return 1;
  }
}
​
int disable(String dummy) {
  state = DISABLED;
  return 1;
}
​
int start(String msecs) {
  unsigned int cmd = atoi(msecs.c_str());
  cmd = cmd / 1000;
  if (cmd < 1 || cmd > 3600)
  {
    clearDisplay();
    setDecimals(0b00000000);
    state = FAILING;
    return 0;
  }
  else
  {
    clearDisplay();
    inputMillis = cmd * 1000;
    timeStarted = millis();
    state = RUNNING;
    return 1;
  }
}
​
void blinkColons(int secs){
  if (secs % 2)
    setDecimals(0b00010000);
  else
    setDecimals(0b00000000);
}
​
// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplay(){
  s7s.write(0x76);  // Clear display command
}
​
// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightness(byte value){
  s7s.write(0x7A);  // Set brightness command byte
  s7s.write(value);  // brightness data byte
}
​
// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals){
  s7s.write(0x77);
  s7s.write(decimals);
}