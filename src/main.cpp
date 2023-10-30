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
#include <Arduino_JSON.h>


#define LED                            5
#define CORNER                         2
#define NUM_OUTPUTS                    2

int outputGPIOs[NUM_OUTPUTS] = {LED, CORNER};


const char *ssid = "A1-A82861";
const char *password = "7PMGDV96J8";

const char* PARAM_INPUT_OUTPUT = "output";
const char * PARAM_INPUT_STATE = "state";

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

String getOutputStates()
{
    JSONVar myArray;

    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        myArray["gpios"][i]["output"] = String(outputGPIOs[i]);
        myArray["gpios"][i]["state"] = String(digitalRead(outputGPIOs[i]));
    }
    String jsonString = JSON.stringify(myArray);
    Serial.print(jsonString);
    Serial.println("ok!");
    return jsonString;
}


void setup() 
{
    pinMode(LED, OUTPUT);    digitalWrite(LED, LOW); 
    pinMode(CORNER, OUTPUT); digitalWrite(CORNER, LOW); 


    Serial.begin(115200);
    Serial.println("start! with  ****** S P I F F S ********");

    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        pinMode(outputGPIOs[i], OUTPUT);
    } 

    initWiFi();

    initSPIFFS();


    server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    { 
        request->send(SPIFFS, "/index.html", "text/html", false);
    }        
             );  

    server.on("/", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    { 
        request->send(SPIFFS, "/style.css", "text/html");
    }        
             );  

    server.on("/logo", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    {
        request->send(SPIFFS, "/logo.png", "image/png");
    }
             );

    server.serveStatic("/", SPIFFS, "/");

    server.on("/states", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    {
        String json = getOutputStates();
        request->send(200, "application/json", json);
        json = String();
    }
             );

    server.on("/update", HTTP_GET, 
    [](AsyncWebServerRequest *request)
    {
        String output;
        String state;
        if (request->hasParam(PARAM_INPUT_OUTPUT) && request->hasParam(PARAM_INPUT_STATE))
        {
            output = request->getParam(PARAM_INPUT_OUTPUT)->value();
            state = request->getParam(PARAM_INPUT_STATE)->value();

            digitalWrite(output.toInt(), state.toInt());
        }
        else
        {
            output ="No message sent";
            state = "No message sent";
        }
        Serial.print("GPIO: ");
        Serial.print(output);
        Serial.print(" -Set to: ");
        Serial.println(state);

        request->send(200, "text/plain", "OK");
    }
             );


   
    server.begin();    

}

void loop() 
{

}

