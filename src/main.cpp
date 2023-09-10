/*****************************************************************************/
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                   sept 23 */
/*****************************************************************************/

#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


#define LED                            5


const char *ssid = "A1-A82861";
const char *password = "7PMGDV96J8";

AsyncWebServer server(80);

void initWiFi()
{
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        Serial.print("Connectiong: ");
        while (WiFi.status() != WL_CONNECTED)
        {Serial.print('.'); delay(1000);}
        Serial.println(WiFi.localIP());
}

void initSPIFFS()
{
     if(SPIFFS.begin(true)) Serial.println("SPIFFS mounted succesfully"); 
     else Serial.println("An error occured while mounting SPIFFS");
}

void setup() 
{
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW); 


    Serial.begin(115200);
    Serial.println("start! with  ****** S P I F F S ********");
    initWiFi();

    initSPIFFS();


    server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    { 
        request->send(SPIFFS, "/index.html", "text/html");
    }        
             );  

    server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    { 
        request->send(SPIFFS, "/style.css", "text/html");
    }        
             );  

    server.serveStatic("/", SPIFFS, "/");


    /*
    server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    { 
        request->send_P(200, "text/html", index_html);
        // 200 OK success status response
        // context type: html-text
        // Page above...
    }        
             );  
*/
    server.begin();    

}

void loop() 
{

}

