#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid     = "WIFI NAME"; 
const char* password = "PASSWORD"; 

// Initialize Telegram BOT 
const char BotToken[] = ""; 
String id = "";
WiFiClientSecure net_ssl; 
long checkTelegramDueTime;
int checkTelegramDelay = 1000;

UniversalTelegramBot bot (BotToken, net_ssl); 

SoftwareSerial mySerial(2,3);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];  
char messageFromPC[numChars] = {0};
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void handleNewMessages(int numNewMessages){
  for(int i = 0; i<numNewMessages; i++){
    String id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if(from_name == "") from_name = "Guest";

    if(text == "/status"){
      bot.sendMessage(id, "Su planta:");
    }

    if(text == "/help"){
      bot.sendMessage(id, "Todos los comandos que puedes usar son:\n /status:Te muestra el estado de tu planta");
    }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.print("Funciono");

  mySerial.begin(115200);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  
  net_ssl.setInsecure();

  bot.sendMessage(id,"Conectado");
}

void loop() {
  long now = millis();

  if(now >= checkTelegramDueTime){
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages){
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    checkTelegramDueTime = now + checkTelegramDelay;
  }
  if(Serial.available() > 0){
      char c = Serial.read();
      int n;
      String dato;
      while(Serial.available() > 0){
        c = Serial.read();
        n = c;
        if(n>13){dato+=c;}
        delay(10);
      }
      bot.sendMessage(id, dato);
  }

  delay(2000);
}

