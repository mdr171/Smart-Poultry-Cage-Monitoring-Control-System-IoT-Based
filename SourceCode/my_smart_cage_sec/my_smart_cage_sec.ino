// firebase ==================================================================================================================================================================================

#if defined(ESP32)
    #include <WiFi.h>
#elif defined(ESP8266)
    #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info and the RTDB payload printing info and other helper functions.
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// insert your network credentials.
#define WIFI_SSID "Home Wi-Fi"
#define WIFI_PASSWORD "Wi-Fi@Home"

// insert Firebase project API key and RTDB URLefine the RTDB URL.
#define API_KEY "AIzaSyDJI1IcKp0aeLxnENxG-HAmq8ddwCGe6_U"
#define DATABASE_URL "https://my-smartcage-default-rtdb.firebaseio.com/"

// define the Firebase Data object.
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// pcf8574 ===================================================================================================================================================================================

#include "Arduino.h"
#include "PCF8574.h"

// Set the i2c HEX address
PCF8574 pcf8574(0x20);

// sheet =====================================================================================================================================================================================

#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbwR2MVhvM2lvJ1NeN_zJ4f7auwghHtbTyfd_jmyf9Cv9n34FPOXT472Ko3AY5jrXJXQ";

// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// clock =====================================================================================================================================================================================

struct CurrentTime {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
};

CurrentTime now;

bool task1_1Executed = false;
bool task1_2Executed = false;

// 00:00
// currentTime ===============================================================================================================================================================================

int onCurrentTimeHour = 0;
int onCurrentTimeMinute = 0;

int onCurrentTimeSecond = 0;
int offCurrentTimeSecond = 30;

// 19:00
// dailyReport ===============================================================================================================================================================================

int onDailyReportHour = 19;
int onDailyReportMinute = 0;

int onDailyReportSecond = 0;
int offDailyReportSecond = 30;

// 1 / 19:01
// monthlyReport =============================================================================================================================================================================

int onMonthlyReportDay = 1;
int onMonthlyReportHour = 19;
int onMonthlyReportMinute = 1;

int onMonthlyReportSecond = 0;
int offMonthlyReportSecond = 30;

// switch ====================================================================================================================================================================================

const int switch1Pin = P0;
const int switch2Pin = P1;
const int switch3Pin = P2;
const int switch4Pin = P3;

int count1;
int count2;
int count3;
int count4;

int M1_lastState1 = LOW;  
int M1_lastState2 = LOW;
int M1_lastState3 = LOW;  
int M1_lastState4 = LOW;

bool task2_1Executed = false;

// millis ====================================================================================================================================================================================

unsigned long startMillis;

unsigned long previousMillis_task0 = 0;
unsigned long interval_task0 = 200;

unsigned long previousMillis_task1_1 = 0;
unsigned long interval_task1_1 = 1000;

unsigned long previousMillis_task1_2 = 0;
unsigned long interval_task1_2 = 40000;

unsigned long previousMillis_task2_1 = 0;
unsigned long interval_task2_1 = 1000;

unsigned long previousMillis_task2_S1 = 0;
unsigned long interval_task2_S1 = 79200000;

unsigned long previousMillis_task2_S2 = 0;
unsigned long interval_task2_S2 = 79200000;

unsigned long previousMillis_task2_S3 = 0;
unsigned long interval_task2_S3 = 79200000;

unsigned long previousMillis_task2_S4 = 0;
unsigned long interval_task2_S4 = 79200000;

unsigned long previousMillis_task3 = 0;
unsigned long interval_task3 = 40000;

unsigned long previousMillis_task4 = 0;
unsigned long interval_task4 = 40000;

unsigned long previousMillis_task5 = 0;
unsigned long interval_task5 = 40000;

// ===========================================================================================================================================================================================
// void setup ================================================================================================================================================================================

void setup() {
  
    Serial.begin(115200);

    // clock =================================================================================================================================================================================

    now.year;
    now.month;
    now.day;
    now.hour;
    now.minute;
    now.second;

    startMillis = millis();

    // pcf8574 ===============================================================================================================================================================================
    
    if (pcf8574.begin()) {
        Serial.println("PCF8574 initialized successfully.");
    } else {
        Serial.println("Failed to initialize PCF8574. Please check wiring and address.");
    }

    pcf8574.pinMode(switch1Pin, INPUT_PULLUP);
    pcf8574.pinMode(switch2Pin, INPUT_PULLUP);
    pcf8574.pinMode(switch3Pin, INPUT_PULLUP);
    pcf8574.pinMode(switch4Pin, INPUT_PULLUP);
    
    // firebase ==============================================================================================================================================================================

    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");

    unsigned long startTime = millis();
    
    while (WiFi.status() != WL_CONNECTED) {
      
        Serial.print(".");
        delay(500);

        if (millis() - startTime >= 10000) {
          
            delay(2000);
            Serial.println("Connection");
            Serial.println("failed");
            return;
        }
    }

    Serial.println();
    Serial.print("Connected: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // assign the API key and RTDB URL to the config object.
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    // sign up
    if (Firebase.signUp(&config, &auth, "", "")){
      
        Serial.println("Sign up OK");
        
//        delay(1500);

        signupOK = true;
        
    } else {
      
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    // assign the callback function for the long running token generation task1_1.
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

// sheet =====================================================================================================================================================================================    

    // Use HTTPSRedirect class to create a new TLS connection
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  
    Serial.print("Connecting to ");
    Serial.println(host);

    // Try to connect for a maximum of 5 times
    bool flag = false;
    for (int i=0; i<5; i++){
        int retval = client->connect(host, httpsPort);
        if (retval == 1){
            flag = true;
                Serial.println("Connected");
                break;
        }
        else
            Serial.println("Connection failed. Retrying...");
    }
    if (!flag){
        Serial.print("Could not connect to server: ");
        Serial.println(host);
        return;
    }
    delete client;    // delete HTTPSRedirect object
    client = nullptr; // delete HTTPSRedirect object

}

// task5 =====================================================================================================================================================================================

void sendToFirebase(String cageNames[], String values[], int count) {
    for (int i = 0; i < count; i++) {
        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyReportA" + String(i + 1), values[i]);
        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyReportA" + String(i + 1), cageNames[i]);
    }
    
    // Send "-" for cages that do not meet the condition
    for (int i = count; i < 4; i++) {
        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyReportA" + String(i + 1), "-");
        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyReportA" + String(i + 1), "-");
    }
}

// ===========================================================================================================================================================================================
// void loop =================================================================================================================================================================================

void loop() {

    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - startMillis;

    // modeOnline ============================================================================================================================================================================

    if (WiFi.status() == WL_CONNECTED) {

        // task1_1 ===========================================================================================================================================================================

        if ((currentMillis - previousMillis_task1_1 >= interval_task1_1 || previousMillis_task1_1 == 0) && !task1_1Executed) {

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Day")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.day = Value.toInt();
                
                Serial.print("Current Day: ");
                Serial.println(now.day);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Year")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.year = Value.toInt();
                
                Serial.print("Current Year: ");
                Serial.println(now.year);
            }
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Hour")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.hour = Value.toInt();
                
                Serial.print("Current Hour: ");
                Serial.println(now.hour);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Minute")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.minute = Value.toInt();
                
                Serial.print("Current Minute: ");
                Serial.println(now.minute);
            }
            
            previousMillis_task1_1 = currentMillis;

            task1_1Executed = true;
            task1_2Executed = true;
        }

        if (elapsedMillis >= 1000) {
            // Update time
            now.second += elapsedMillis / 1000;
            startMillis = currentMillis - (elapsedMillis % 1000);

            // Handle overflow
            if (now.second >= 60) {
                now.minute += now.second / 60;
                now.second %= 60;
            }
            if (now.minute >= 60) {
                now.hour += now.minute / 60;
                now.minute %= 60;
            }
            if (now.hour >= 24) {
                now.day += now.hour / 24;
                now.hour %= 24;
            }

            Serial.print(now.day, DEC);
            Serial.print("/");
            Serial.print(now.year, DEC);
            Serial.print("-");
            Serial.print(now.hour, DEC);
            Serial.print(':');
            Serial.print(now.minute, DEC);
            Serial.print(':');
            Serial.print(now.second, DEC);
            Serial.println();
        }

        // task1_2 ===========================================================================================================================================================================

        if ((currentMillis - previousMillis_task1_2 >= interval_task1_2 || previousMillis_task1_2 == 0) && 
            (now.hour == onCurrentTimeHour && now.minute == onCurrentTimeMinute && now.second > onCurrentTimeSecond) &&
            (now.hour == onCurrentTimeHour && now.minute == onCurrentTimeMinute && now.second <= offCurrentTimeSecond)) {

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Day")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.day = Value.toInt();
                
                Serial.print("Current Day: ");
                Serial.println(now.day);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Year")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.year = Value.toInt();
                
                Serial.print("Current Year: ");
                Serial.println(now.year);
            }
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Hour")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.hour = Value.toInt();
                
                Serial.print("Current Hour: ");
                Serial.println(now.hour);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/Minute")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                now.minute = Value.toInt();
                
                Serial.print("Current Minute: ");
                Serial.println(now.minute);
            }
            
            previousMillis_task1_2 = currentMillis;
        }

        // task2_1 ===========================================================================================================================================================================

        if ((currentMillis - previousMillis_task2_1 >= interval_task2_1 || previousMillis_task2_1 == 0) && !task2_1Executed) {

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/CageDailyReportA1")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                count1 = Value.toInt();
                
                Serial.print("Current Count1: ");
                Serial.println(count1);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/CageDailyReportA2")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                count2 = Value.toInt();
                
                Serial.print("Current Count2: ");
                Serial.println(count2);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/CageDailyReportA3")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                count3 = Value.toInt();
                
                Serial.print("Current Count3: ");
                Serial.println(count3);
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/CageDailyReportA4")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                count4 = Value.toInt();
                
                Serial.print("Current Count4: ");
                Serial.println(count4);
            }

            previousMillis_task2_1 = currentMillis;

            task2_1Executed = true;
        }
        
        //task2_S1 ==========================================================================================================================================================================

        int currentState1 = pcf8574.digitalRead(switch1Pin);

        if ((currentMillis - previousMillis_task2_S1 > interval_task2_S1 || previousMillis_task2_S1 == 0) && (currentState1 == LOW && M1_lastState1 == HIGH)) {
      
            count1++;
            
            Serial.println("Switch 1 count: " + String(count1));
        
            previousMillis_task2_S1 = currentMillis;
        }

        //task2_S2 ==========================================================================================================================================================================

        int currentState2 = pcf8574.digitalRead(switch2Pin);

        if ((currentMillis - previousMillis_task2_S2 > interval_task2_S2 || previousMillis_task2_S2 == 0) && (currentState2 == LOW && M1_lastState2 == HIGH)) {
      
            count2++;
            
            Serial.println("Switch 2 count: " + String(count2));
        
            previousMillis_task2_S2 = currentMillis;
        }

        //task2_S3 ==========================================================================================================================================================================

        int currentState3 = pcf8574.digitalRead(switch3Pin);

        if ((currentMillis - previousMillis_task2_S3 > interval_task2_S3 || previousMillis_task2_S3 == 0) && (currentState3 == LOW && M1_lastState3 == HIGH)) {

            count3++;

            Serial.println("Switch 3 count: " + String(count3));
        
            previousMillis_task2_S3 = currentMillis;
        }

        //task2_S4 ==========================================================================================================================================================================

        int currentState4 = pcf8574.digitalRead(switch4Pin);

        if ((currentMillis - previousMillis_task2_S4 > interval_task2_S4 || previousMillis_task2_S4 == 0) && (currentState4 == LOW && M1_lastState4 == HIGH)) {

            count4++;

            Serial.println("Switch 4 count: " + String(count4));

            previousMillis_task2_S4 = currentMillis;
        }

        M1_lastState1 = currentState1;
        M1_lastState2 = currentState2;
        M1_lastState3 = currentState3;
        M1_lastState4 = currentState4;

        // task3 =============================================================================================================================================================================

        if ((currentMillis - previousMillis_task3 > interval_task3 || previousMillis_task3 == 0) && 
            (now.hour == onDailyReportHour && now.minute == onDailyReportMinute && now.second > onDailyReportSecond) &&
            (now.hour == onDailyReportHour && now.minute == onDailyReportMinute && now.second <= offDailyReportSecond)) {

            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyReportA1", count1) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyReportA2", count2) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyReportA3", count3) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyReportA4", count4)) {
            }

            Serial.println("Sent Count");

            previousMillis_task3 = currentMillis;
        }

        // task4 =============================================================================================================================================================================

        int targetMonthly;

        if ((currentMillis - previousMillis_task4 >= interval_task4 || previousMillis_task4 == 0) && 
            (now.day == onMonthlyReportDay && now.hour == onMonthlyReportHour && now.minute == onMonthlyReportMinute && now.second > onMonthlyReportSecond) &&
            (now.day == onMonthlyReportDay && now.hour == onMonthlyReportHour && now.minute == onMonthlyReportMinute && now.second <= offMonthlyReportSecond)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/TargetMonthlyBox")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                targetMonthly = Value.toInt();
            }

            Serial.print("Target Monthly : ");
            Serial.println(targetMonthly);
            previousMillis_task4 = currentMillis;
        }
        
        // task5 =============================================================================================================================================================================

    if ((currentMillis - previousMillis_task5 > interval_task5 || previousMillis_task5 == 0) && 
        (now.day == onMonthlyReportDay && now.hour == onMonthlyReportHour && now.minute == onMonthlyReportMinute && now.second > onMonthlyReportSecond) &&
        (now.day == onMonthlyReportDay && now.hour == onMonthlyReportHour && now.minute == onMonthlyReportMinute && now.second <= offMonthlyReportSecond)) {

        Serial.println("START");

        int counts[] = {count1, count2, count3, count4};
        String cages[] = {"A1", "A2", "A3", "A4"};
        int n = sizeof(counts) / sizeof(counts[0]);

        // Filter counts and cages based on targetMonthly
        int filteredCounts[n];
        String filteredCages[n];
        int index = 0;

        for (int i = 0; i < n; i++) {
            if (counts[i] < targetMonthly) {
                filteredCounts[index] = counts[i];
                filteredCages[index] = cages[i];
                index++;
            }
        }

        // Send data to Firebase based on filtered counts and cages
        if (index > 0) {
            // Convert filteredCounts to String
            String filteredCountsStr[index];
            for (int i = 0; i < index; i++) {
                filteredCountsStr[i] = String(filteredCounts[i]);
            }
            sendToFirebase(filteredCages, filteredCountsStr, index);
        } else {
            // If no cages are below target, send "-" for all entries
            String emptyCages[] = {"-", "-", "-", "-"};
            String emptyValues[] = {"-", "-", "-", "-"};
            sendToFirebase(emptyCages, emptyValues, 4);
        }

        Serial.println(count1);
        Serial.println(count2);
        Serial.println(count3);
        Serial.println(count4);

        int value0 = count1;
        int value1 = count2;
        int value2 = count3;
        int value3 = count4;

        // Reset counts
        count1 = 0;
        count2 = 0;
        count3 = 0;
        count4 = 0;

// sheet =====================================================================================================================================================================================

            static bool flag = false;
            
            if (!flag) {
                client = new HTTPSRedirect(httpsPort);
                client->setInsecure();
                flag = true;
                client->setPrintResponseBody(true);
                client->setContentTypeHeader("application/json");
            }
            
            if (client != nullptr){
                if (!client->connected()){
                    client->connect(host, httpsPort);
                }
            }
            
            else {
                Serial.println("Error creating client object!");
            }

            // Create json object string to send to Google Sheets
            payload = payload_base + "\"" + value0 + "," + value1 + "," + value2 + "," + value3 + "\"}";
  
            // Publish data to Google Sheets
            Serial.println("Publishing data...");
            Serial.println(payload);
            if(client->POST(url, host, payload)){ 
                // do stuff here if publish was successful
            }
            else{
                // do stuff here if publish was not successful
                Serial.println("Error while connecting");
            }
  
// ===========================================================================================================================================================================================

            previousMillis_task5 = currentMillis;
        }

        // task0 =============================================================================================================================================================================

        if (currentMillis - previousMillis_task0 >= interval_task0 || previousMillis_task0 == 0) {
          
            Firebase.reconnectWiFi(true);

            previousMillis_task0 = currentMillis;
        }
    }
}
