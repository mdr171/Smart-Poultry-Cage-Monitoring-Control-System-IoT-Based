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
#define WIFI_SSID "ASUS_X00TD"
#define WIFI_PASSWORD "11111111"

// insert Firebase project API key and RTDB URLefine the RTDB URL.
#define API_KEY "AIzaSyDJI1IcKp0aeLxnENxG-HAmq8ddwCGe6_U"
#define DATABASE_URL "https://my-smartcage-default-rtdb.firebaseio.com/"

// define the Firebase Data object.
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

// sheet =====================================================================================================================================================================================

#include <Arduino.h>
//#include <ESP8266WiFi.h>
#include "HTTPSRedirect.h"

// rtc =======================================================================================================================================================================================

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const char* monthName[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char* dayName[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// dht11 =====================================================================================================================================================================================

#include <DHT.h>

#define DHTPIN 03
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// servo =====================================================================================================================================================================================

#include <Servo.h>

Servo myservo;

// lcd =======================================================================================================================================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

byte Temp[8] =
{
 B00100,
 B01010,
 B01010,
 B01110,
 B01110,
 B11111,
 B11111,
 B01110
};

byte Fan[8] =
{
 B00000,
 B01100,
 B00101,
 B11011,
 B10100,
 B00110,
 B00000,
 B00000
};

byte Light[8] =
{
 B00100,
 B10001,
 B01110,
 B11101,
 B11101,
 B01110,
 B01110,
 B00100
};

byte Signal[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00001,
 B00001
};

byte Signal2[8] =
{
 B00000,
 B00000,
 B00000,
 B00001,
 B00101,
 B10101,
 B10101,
 B10101
};

byte Signal3[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B10101,
 B10010,
 B10101
};

byte Vitamin[8] =
{
 B00000,
 B01110,
 B10001,
 B10001,
 B11111,
 B10001,
 B10001,
 B01110
};

byte Sad[8] =
{
 B00000,
 B00000,
 B01010,
 B00000,
 B00000,
 B01110,
 B10001,
 B00000
};

byte Happy[8] =
{
 B00000,
 B00000,
 B01010,
 B00000,
 B00000,
 B10001,
 B01110,
 B00000
};

// relay =====================================================================================================================================================================================

int relay1 = 0;
int relay2 = 2;

// ===========================================================================================================================================================================================
//        
// temporary
//
// fan =======================================================================================================================================================================================
//
int maxTemp = 27;

// 17:00 - 22:00
// light =====================================================================================================================================================================================
//
int onLightHour = 17;
int onLightMinute = 00;
int onLightSecond = 0;
//
int offLightHour = 22;
int offLightMinute = 0;
int offLightSecond = 0;
//
// targetMonthly ==============================================================================================================================================================================
//
int targetMonthly = 20;
//
// temporary
//
// ===========================================================================================================================================================================================

// 00:00
// rtc =======================================================================================================================================================================================

int onRTCHour = 0;
int onRTCMinute = 0;

int onRTCSecond = 0;
int offRTCSecond = 10;

// 00:01
// daily =====================================================================================================================================================================================

int onDailyHour = 0;
int onDailyMinute = 1;

int onDailySecond = 0;
int offDailySecond = 10;

// 1 / 00:02
// monthly ===================================================================================================================================================================================

int onMonthlyDay = 1;
int onMonthlyHour = 0;
int onMonthlyMinute = 2;

int onMonthlySecond = 0;
int offMonthlySecond = 10;

// 04:00
// vit =======================================================================================================================================================================================

int onVitHour = 4;
int onVitMinute = 0;

int onVitSecond = 0;
int offVitSecond = 10;

// servo =====================================================================================================================================================================================

int firstCorner = 30;
int secondCorner = 90;

// switch ====================================================================================================================================================================================

const int switchPin1 = 14;
const int switchPin2 = 12;
const int switchPin3 = 13;
const int switchPin4 = 13;

// Variabel untuk menyimpan jumlah tekanan tombol switch
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;

// Variabel untuk menyimpan status sebelumnya dari switch
int lastState1 = HIGH;  
int lastState2 = HIGH;
int lastState3 = HIGH;  
int lastState4 = HIGH;

// bool ======================================================================================================================================================================================

bool statusRelay1 = LOW;
bool statusRelay2 = LOW;
bool statusServo = false;

//const int ledPin = 12;

// millis ====================================================================================================================================================================================

unsigned long previousMillis_task0 = 0;
unsigned long interval_task0 = 200;

unsigned long previousMillis_task1_1 = 0;
unsigned long interval_task1_1 = 2000;

unsigned long previousMillis_task1_2 = 0;
unsigned long interval_task1_2 = 15000;

unsigned long previousMillis_task2_1 = 0;
unsigned long interval_task2_1 = 2000;

unsigned long previousMillis_task2_2 = 0;
unsigned long interval_task2_2 = 1000;

unsigned long previousMillis_task2_2a = 0;
unsigned long interval_task2_2a = 20000;

unsigned long previousMillis_task3 = 0;
unsigned long interval_task3 = 2000;

unsigned long previousMillis_task4Eks = 0;
unsigned long interval_task4Eks = 20000;

unsigned long previousMillis_task4Manual = 0;
unsigned long interval_task4Manual = 2000;

unsigned long previousMillis_task4Auto_1 = 0;
unsigned long interval_task4Auto_1 = 86400000;

unsigned long previousMillis_task4Auto_2 = 0;
unsigned long interval_task4Auto_2 = 2000;

//unsigned long previousMillis_task4Auto_3 = 0;
//unsigned long interval_task4Auto_3 = 3000;

unsigned long previousMillis_task5Eks = 0;
unsigned long interval_task5Eks = 20000;

unsigned long previousMillis_task5Manual = 0;
unsigned long interval_task5Manual = 2000;

unsigned long previousMillis_task5Auto_1 = 0;
unsigned long interval_task5Auto_1 = 86400000;

unsigned long previousMillis_task5Auto_2 = 0;
unsigned long interval_task5Auto_2 = 1000;

//unsigned long previousMillis_task5Auto_3 = 0;
//unsigned long interval_task5Auto_3 = 3000;

unsigned long previousMillis_task6Eks = 0;
unsigned long interval_task6Eks = 20000;

unsigned long previousMillis_task6Manual = 0;
unsigned long interval_task6Manual = 2000;

unsigned long previousMillis_task6Manual_a = 0;
unsigned long interval_task6Manual_a = 5000;

unsigned long previousMillis_task6Auto_1 = 0;
unsigned long interval_task6Auto_1 = 86400000;

unsigned long previousMillis_task6Auto_2 = 0;
unsigned long interval_task6Auto_2 = 1000;

unsigned long previousMillis_task6Auto_2a = 0;
unsigned long interval_task6Auto_2a = 15000;
unsigned long durationServo = 500;

//unsigned long previousMillis_task6Auto_3 = 0;
//unsigned long interval_task6Auto_3 = 3000;
//
//unsigned long previousMillis_task6Auto_3a = 0;
//unsigned long interval_task6Auto_3a = 20000;

// switch ====================================================================================================================================================================================

unsigned long previousMillis_task7_1 = 0;
unsigned long interval_task7_1 = 86400000;

// task7_2_1a
unsigned long previousMillis_task7_2_1a = 0;
unsigned long interval_task7_2_1a = 1000;

// task7_2_1b
unsigned long previousMillis_task7_2_1b = 0;
unsigned long interval_task7_2_1b = 1000;

// task7_2_2a
unsigned long previousMillis_task7_2_2a = 0;
unsigned long interval_task7_2_2a = 1000;

// task7_2_2b
unsigned long previousMillis_task7_2_2b = 0;
unsigned long interval_task7_2_2b = 1000;

// task7_2_3a
unsigned long previousMillis_task7_2_3a = 0;
unsigned long interval_task7_2_3a = 1000;

// task7_2_3b
unsigned long previousMillis_task7_2_3b = 0;
unsigned long interval_task7_2_3b = 1000;

// task7_2_4a
unsigned long previousMillis_task7_2_4a = 0;
unsigned long interval_task7_2_4a = 1000;

// task7_2_4b
unsigned long previousMillis_task7_2_4b = 0;
unsigned long interval_task7_2_4b = 1000;

// task7_3
unsigned long previousMillis_task7_3 = 0;
unsigned long interval_task7_3 = 1000;

// task7_3a
unsigned long previousMillis_task7_3a = 0;
unsigned long interval_task7_3a = 15000;

// task7_4
unsigned long previousMillis_task7_4 = 0;
unsigned long interval_task7_4 = 1000;

// task7_4a
unsigned long previousMillis_task7_4a = 0;
unsigned long interval_task7_4a = 15000;

// sheet =====================================================================================================================================================================================

//// Enter network credentials:
//const char* ssid     = "ASUS_X00TD";
//const char* password = "11111111";

// Enter Google Script Deployment ID:
const char *GScriptId = "AKfycbwlvlhePgC7qJADdXHQU_6qYtKHYVuGoKgNjMHOWBlwbjZynGs-lBHxb95kIoGu8uNa";

// Enter command (insert_row or append_row) and your Google Sheets sheet name (default is Sheet1):
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

// Google Sheets setup (do not edit)
const char* host = "script.google.com";
const int httpsPort = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;

// ===========================================================================================================================================================================================

// void setup ================================================================================================================================================================================

void setup() {
  
    Serial.begin(115200);

//    pinMode(ledPin, OUTPUT);
    
    analogReference(EXTERNAL);

    // relay =================================================================================================================================================================================
    
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    myservo.write(firstCorner);

    // switch ================================================================================================================================================================================

    pinMode(switchPin1, INPUT_PULLUP);
    pinMode(switchPin2, INPUT_PULLUP);
    pinMode(switchPin3, INPUT_PULLUP);
    pinMode(switchPin4, INPUT_PULLUP);

    // dht11 =================================================================================================================================================================================

    dht.begin();

    // rtc ===================================================================================================================================================================================

    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, let's set the time!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // lcd ===================================================================================================================================================================================

    lcd.init();
    lcd.backlight();
    
    lcd.createChar(1, Temp);
    lcd.createChar(2, Fan);
    lcd.createChar(3, Light);
    lcd.createChar(4, Signal);
    lcd.createChar(5, Signal2);
//    lcd.createChar(6, Signal3);
    lcd.createChar(6, Vitamin);
    lcd.createChar(7, Sad);
    lcd.createChar(8, Happy);

    // servo =================================================================================================================================================================================

    myservo.attach(16);
    
    // firebase ==============================================================================================================================================================================

    // connect to wifi.
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting");

    unsigned long startTime = millis();
    
    while (WiFi.status() != WL_CONNECTED) {
      
        Serial.print(".");
        lcd.print(".");
        delay(500);

        if (millis() - startTime >= 10000) {
          
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Connection");
            lcd.setCursor(0, 1);
            lcd.print("failed");
            delay(2000);
            lcd.clear();
            Serial.println("Connection");
            Serial.println("failed");
            return;
        }
    }

    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Serial.println();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected:");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());

    // assign the API key and RTDB URL to the config object.
    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;

    // sign up
    if (Firebase.signUp(&config, &auth, "", "")){
      
        Serial.println("Sign up OK");
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sign up OK");
        delay(1500);
        lcd.clear();

        signupOK = true;
        
    } else {
      
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    // assign the callback function for the long running token generation task.
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

// ===========================================================================================================================================================================================

}

// void loop =================================================================================================================================================================================

void loop() {

    unsigned long currentMillis = millis(); // grab current time

    // modeOnline ============================================================================================================================================================================

    if (WiFi.status() == WL_CONNECTED) {

        // task1_1 ===========================================================================================================================================================================

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if (currentMillis - previousMillis_task1_1 >= interval_task1_1 || previousMillis_task1_1 == 0) {

            Serial.print("Suhu: ");
            Serial.print(temperature);
            Serial.print("°C");
            Serial.print("\t");
            Serial.print("Kelembaban: ");
            Serial.print(humidity);
            Serial.println("%");

            previousMillis_task1_1 = currentMillis;
        }

        // task1_2 ===========================================================================================================================================================================

        if (currentMillis - previousMillis_task1_2 >= interval_task1_2 || previousMillis_task1_2 == 0) {

            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/Temp", temperature)) {

                Serial.println("Temp");
                
            } else {

                Serial.println("task1_2: Firebase Temp failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }
            
            previousMillis_task1_2 = currentMillis;
        }

        // task2_1 ===========================================================================================================================================================================

        DateTime now = rtc.now();

        now = now + TimeSpan(0, 0, 12, 0);

        int previousMonthIndex = now.month() - 2;
        if (previousMonthIndex < 0) {
            previousMonthIndex = 11;
        }

        if (currentMillis - previousMillis_task2_1 >= interval_task2_1 || previousMillis_task2_1 == 0) {

            Serial.print(dayName[now.dayOfTheWeek()]);
            Serial.print(", ");
            Serial.print(now.day(), DEC);
            Serial.print(" ");
            Serial.print(monthName[now.month() - 1]);
            Serial.print(" ");
            Serial.print(now.year(), DEC);
            Serial.print(" ");
            Serial.print(now.hour(), DEC);
            Serial.print(':');
            Serial.print(now.minute(), DEC);
            Serial.print(':');
            Serial.print(now.second(), DEC);
            Serial.println();
            Serial.print(monthName[previousMonthIndex]);
            Serial.println();
            
            previousMillis_task2_1 = currentMillis;
        }

        // task2_2 ===========================================================================================================================================================================

        if ((currentMillis - previousMillis_task2_2 >= interval_task2_2 || previousMillis_task2_2 == 0)) {

            if ((now.hour() == onRTCHour && now.minute() == onRTCMinute && now.second() > onRTCSecond) &&
                (now.hour() == onRTCHour && now.minute() == onRTCMinute && now.second() <= offRTCSecond)) {

                if (currentMillis - previousMillis_task2_2a >= interval_task2_2a) {
                    
                    if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/DayName", dayName[now.dayOfTheWeek()]) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/Day", now.day()) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/MonthName", monthName[now.month() - 1]) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/Year", now.year()) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/PreviousMonthName", monthName[previousMonthIndex])) {
                    }

                    Serial.print("RTC");
                    
                    previousMillis_task2_2a = currentMillis;
                }
                
            } else {

            }
            
            previousMillis_task2_2 = currentMillis;
        }

        // task3 =============================================================================================================================================================================

        if (currentMillis - previousMillis_task3 >= interval_task3 || previousMillis_task3 == 0) {

            // signal
            lcd.setCursor(18, 0);
            lcd.write(4);
            lcd.setCursor(19, 0);
            lcd.write(5);

            // temp
            lcd.setCursor(0, 0);
            lcd.write(1);
            lcd.setCursor(2, 0);
            lcd.print(temperature);
            lcd.print((char)223);
            lcd.print("C");
        
            previousMillis_task3 = currentMillis;
        }

        // task4Eks ==========================================================================================================================================================================

        static bool task4Eks = false;

        if (currentMillis - previousMillis_task4Eks >= interval_task4Eks || previousMillis_task4Eks == 0) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/A1")) {

                if (fbdo.dataType() == "string") {

                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {
                      
                        task4Eks = true;

                        Serial.print("Eks: ");
                        Serial.println(Value);
                        
                    } else if (Value == 0) {
                      
                        task4Eks = false;

                        Serial.print("Eks: ");
                        Serial.println(Value);
                        
                    }
                }
                
            } else {
              
                Serial.println("task4Eks: Firebase A1 failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }

            previousMillis_task4Eks = currentMillis;
        }

        // task4Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task4Manual >= interval_task4Manual || previousMillis_task4Manual == 0) && (task4Eks == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/C1")) {
              
                if (fbdo.dataType() == "string") {
                  
                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {

                        lcd.setCursor(17, 0);
                        lcd.write(2);

                        Serial.print("M: ");
                        Serial.println(Value);

                        digitalWrite(relay1, HIGH);
                        
                    } else if (Value == 0) {

                        lcd.setCursor(17, 0);
                        lcd.print(" ");

                        Serial.print("M: ");
                        Serial.println(Value);
                      
                        digitalWrite(relay1, LOW);
                    }
                }
                
            } else {
              
                Serial.println("task4Manual: Firebase C1 failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }

            previousMillis_task4Manual = currentMillis;
        }

// ===========================================================================================================================================================================================
        
        // temporary
        
        // task4Auto_1 =======================================================================================================================================================================

        if (currentMillis - previousMillis_task4Auto_1 >= interval_task4Auto_1 || previousMillis_task4Auto_1 == 0) {
          
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/MaxTemp", String(maxTemp))) {

                Serial.println("MaxTemp");
                  
            } else {
                  
                Serial.println("task4Auto_1: Firebase MaxTemp failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }
            
            previousMillis_task4Auto_1 = currentMillis;
        }

        // temporary
        
// ===========================================================================================================================================================================================
        
        // task4Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task4Auto_2 >= interval_task4Auto_2 || previousMillis_task4Auto_2 == 0) && (task4Eks == true)) {
          
            if (temperature > maxTemp) {

                int Value = 1;

                lcd.setCursor(17, 0);
                lcd.write(2);

                digitalWrite(relay1, HIGH);

                Serial.print("A: ");
                Serial.println(Value);
                
            } else {

                int Value = 0;

                lcd.setCursor(17, 0);
                lcd.print(" ");

                digitalWrite(relay1, LOW);

                Serial.print("A: ");
                Serial.println(Value);

            }

            previousMillis_task4Auto_2 = currentMillis;
        }

//        // task4Auto_3 =====================================================================================================================================================================
//
//        if ((currentMillis - previousMillis_task4Auto_3 >= interval_task4Auto_3 || previousMillis_task4Auto_3 == 0) && (task4Eks == true)) {
//          
//            if (temperature > maxTemp) {
//
//                int Value = 1;
//                              
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C1", String(Value))) {
//                  
//                } else {
//                  
//                    Serial.println("task4Auto_3: Firebase C1 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//                
//            } else {
//
//                int Value = 0;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C1", String(Value))) {
//                  
//                } else {
//                  
//                    Serial.println("task4Auto_3: Firebase C1 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//
//            }
//
//            previousMillis_task4Auto_3 = currentMillis;
//        }
        
        // task5Eks ==========================================================================================================================================================================

        static bool task5Eks = false;

        if (currentMillis - previousMillis_task5Eks >= interval_task5Eks || previousMillis_task5Eks == 0) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/A2")) {

                if (fbdo.dataType() == "string") {

                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {
                      
                        task5Eks = true;

                        Serial.print("Eks2: ");
                        Serial.println(Value);
                        
                    } else if (Value == 0) {
                      
                        task5Eks = false;

                        Serial.print("Eks2: ");
                        Serial.println(Value);
                        
                    }
                }
                
            } else {
              
                Serial.println("task5Eks: Firebase A2 failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }

            previousMillis_task5Eks = currentMillis;
        }

        // task5Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task5Manual >= interval_task5Manual || previousMillis_task5Manual == 0) && (task5Eks == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/C2")) {
              
                if (fbdo.dataType() == "string") {
                  
                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {

                        lcd.setCursor(16, 0);
                        lcd.write(3);

                        Serial.print("M2: ");
                        Serial.println(Value);

                        digitalWrite(relay2, HIGH);
                        
                    } else if (Value == 0) {

                        lcd.setCursor(16, 0);
                        lcd.print(" ");

                        Serial.print("M2: ");
                        Serial.println(Value);
                      
                        digitalWrite(relay2, LOW);
                    }
                }
                
            } else {
              
                Serial.println("task5Manual: Firebase C2 failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }

            previousMillis_task5Manual = currentMillis;
        }

// ===========================================================================================================================================================================================
        
        // temporary
        
        // task5Auto_1 =======================================================================================================================================================================

        if (currentMillis - previousMillis_task5Auto_1 >= interval_task5Auto_1 || previousMillis_task5Auto_1 == 0) {
          
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/OnLightHour", String(onLightHour)) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/OnLightMinute", String(onLightMinute)) &&
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/OffLightHour", String(offLightHour)) &&
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/OffLightMinute", String(offLightMinute))) {

                Serial.println("set Light");
                  
            } else {
                  
                Serial.println("task5Auto_1: Firebase setLight failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }
            
            previousMillis_task5Auto_1 = currentMillis;
        }

        // temporary
        
// ===========================================================================================================================================================================================

        // task5Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task5Auto_2 >= interval_task5Auto_2 || previousMillis_task5Auto_2 == 0) && (task5Eks == true)) {
          
            if ((now.hour() == onLightHour && now.minute() >= onLightMinute) || 
                (now.hour() > onLightHour && now.hour() < offLightHour) ||
                (now.hour() == offLightHour && now.minute() <= offLightMinute)) {

                int Value = 1;

                lcd.setCursor(16, 0);
                lcd.write(3);

                digitalWrite(relay2, HIGH);

                Serial.print("A2: ");
                Serial.println(Value);

            } else {

                int Value = 0;

                lcd.setCursor(16, 0);
                lcd.print(" ");

                digitalWrite(relay2, LOW);

                Serial.print("A2: ");
                Serial.println(Value);

            }

            previousMillis_task5Auto_2 = currentMillis;
        }

//        // task5Auto_3 =======================================================================================================================================================================
//
//        if ((currentMillis - previousMillis_task5Auto_3 >= interval_task5Auto_3 || previousMillis_task5Auto_3 == 0) && (task5Eks == true)) {
//          
//            if ((now.hour() == onLightHour && now.minute() >= onLightMinute) || 
//                (now.hour() > onLightHour && now.hour() < offLightHour) ||
//                (now.hour() == offLightHour && now.minute() <= offLightMinute)) {
//
//                int Value = 1;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C2", String(Value))) {
//
//                } else {
//
//                    Serial.println("task5Auto_3: Firebase C2 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//                
//            } else {
//
//                int Value = 0;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C2", String(Value))) {
//
//                } else {
//                  
//                    Serial.println("task5Auto_3: Firebase C2 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//
//            }
//
//            previousMillis_task5Auto_3 = currentMillis;
//        }

        // task6Eks ==========================================================================================================================================================================

        static bool task6Eks = false;

        if (currentMillis - previousMillis_task6Eks >= interval_task6Eks || previousMillis_task6Eks == 0) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/A3")) {

                if (fbdo.dataType() == "string") {

                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {
                      
                        task6Eks = true;

                        Serial.print("Eks3: ");
                        Serial.println(Value);
                        
                    } else if (Value == 0) {
                      
                        task6Eks = false;

                        Serial.print("Eks3: ");
                        Serial.println(Value);
                        
                    }
                }
                
            } else {
              
                Serial.println("task6Eks: Firebase A3 failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }

            previousMillis_task6Eks = currentMillis;
        }

        // task6Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task6Manual >= interval_task6Manual || previousMillis_task6Manual == 0) && (task6Eks == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/C3")) {
              
                if (fbdo.dataType() == "string") {
                  
                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {

                        lcd.setCursor(15, 0);
                        lcd.write(6);

                        Serial.print("M3: ");
                        Serial.println(Value);

                        if (currentMillis - previousMillis_task6Manual_a >= interval_task6Manual_a) {
                            previousMillis_task6Manual_a = currentMillis;
                            statusServo = true;
                        }
                        
                    } else if (Value == 0) {

                        lcd.setCursor(15, 0);
                        lcd.print(" ");

                        Serial.print("M3: ");
                        Serial.println(Value);
                      
                        myservo.write(firstCorner);
                    }
                }
            }
        
            previousMillis_task6Manual = currentMillis;
        }

        if (statusServo) {

            myservo.write(secondCorner);

            if (currentMillis - previousMillis_task6Manual_a >= durationServo) {
                statusServo = false;
                myservo.write(firstCorner);
            }
        }
        
// ===========================================================================================================================================================================================
        
        // temporary
        
        // task6Auto_1 =======================================================================================================================================================================

        if (currentMillis - previousMillis_task6Auto_1 >= interval_task6Auto_1 || previousMillis_task6Auto_1 == 0) {
          
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/OnVitHour", String(onVitHour)) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/OnVitMinute", String(onVitMinute))) {

                Serial.println("set Vit");
                  
            } else {
                  
                Serial.println("task6Auto_1: Firebase setVit failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }
            
            previousMillis_task6Auto_1 = currentMillis;
        }

        // temporary
        
// ===========================================================================================================================================================================================

        // task6Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task6Auto_2 >= interval_task6Auto_2 || previousMillis_task6Auto_2 == 0) && (task6Eks == true)) {
          
            if ((now.hour() == onVitHour && now.minute() == onVitMinute && now.second() > onVitSecond) &&
                (now.hour() == onVitHour && now.minute() == onVitMinute && now.second() <= offVitSecond)) {

                int Value = 1;
            
                lcd.setCursor(15, 0);
                lcd.write(6);

                Serial.print("A3: ");
                Serial.println(Value);

                if (currentMillis - previousMillis_task6Auto_2a >= interval_task6Auto_2a) {
                    previousMillis_task6Auto_2a = currentMillis;
                    statusServo = true;
                }
                
            } else {

                int Value = 0;

                lcd.setCursor(15, 0);
                lcd.print(" ");

                Serial.print("A3: ");
                Serial.println(Value);

                myservo.write(firstCorner);

            }
            
            previousMillis_task6Auto_2 = currentMillis;
        }

        if (statusServo) {

            myservo.write(secondCorner);

            if (currentMillis - previousMillis_task6Auto_2a >= durationServo) {
                statusServo = false;
                myservo.write(firstCorner);
            }
        }

//        // task6Auto_3 =====================================================================================================================================================================
//
//        if ((currentMillis - previousMillis_task6Auto_3 >= interval_task6Auto_3 || previousMillis_task6Auto_3 == 0) && (task6Eks == true)) {
//          
//            if ((now.hour() == onVitHour && now.minute() == onVitMinute && now.second() > onVitSecond) &&
//                (now.hour() == onVitHour && now.minute() == onVitMinute && now.second() <= offVitSecond)) {
//                
//                if (currentMillis - previousMillis_task6Auto_3a >= interval_task6Auto_3a) {
//                    previousMillis_task6Auto_3a = currentMillis;
//                    statusServo = true;
//                }
//                
//            } else {
//
//                int Value = 0;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C3", String(Value))) {
//
//                } else {
//
//                    Serial.println("task6Auto_3: Firebase C3 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//
//            }
//            
//            previousMillis_task6Auto_3 = currentMillis;
//        }
//
//        if (statusServo) {
//
//            int Value = 1;
//
//            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C3", String(Value))) {
//
//            } else {
//
//                Serial.println("task6Auto_3: Firebase C3 failed");
//                Serial.println("REASON: " + fbdo.errorReason());
//            }
//
//            if (currentMillis - previousMillis_task6Auto_3a >= durationServo) {
//                statusServo = false;
//                
//                int Value = 0;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C3", String(Value))) {
//
//                } else {
//
//                    Serial.println("task6Auto_3: Firebase C3 failed");
//                    Serial.println("REASON: " + fbdo.errorReason());
//                }
//            }
//        }

// ===========================================================================================================================================================================================
        
        // temporary
        
        // task7_1 ===========================================================================================================================================================================

        if (currentMillis - previousMillis_task7_1 >= interval_task7_1 || previousMillis_task7_1 == 0) {
          
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/TargetMonthly", String(targetMonthly))) {

                Serial.println("target Monthly");
                  
            } else {
                  
                Serial.println("task7_1: Firebase targetMonthly failed");
                Serial.println("REASON: " + fbdo.errorReason());
            }
            
            previousMillis_task7_1 = currentMillis;
        }

        // temporary
        
// ===========================================================================================================================================================================================

        // task7_2_1a ========================================================================================================================================================================

        int currentState1 = digitalRead(switchPin1);

        // Memeriksa jika terjadi perubahan dari HIGH ke LOW
        if ((currentMillis - previousMillis_task7_2_1a > interval_task7_2_1a || previousMillis_task7_2_1a == 0) && (currentState1 == LOW && lastState1 == HIGH)) {
      
            count1++;
            Serial.println("Switch 1 count: " + String(count1));

//            lcd.setCursor(0, 2);
//            lcd.print("A");
//            lcd.setCursor(0, 3);
//            lcd.print("1");
            
            lcd.setCursor(1, 2);
            lcd.write(8);
            lcd.setCursor(1, 3);
            lcd.print(count1);
        
            previousMillis_task7_2_1a = currentMillis;
        }

        // task7_2_1b ========================================================================================================================================================================
    
        // Memeriksa jika terjadi perubahan dari LOW ke HIGH
        if ((currentMillis - previousMillis_task7_2_1b > interval_task7_2_1b || previousMillis_task7_2_1b == 0) && (currentState1 == HIGH && lastState1 == LOW)) {
      
            previousMillis_task7_2_1b = currentMillis;
        }

        // task7_2_2a ========================================================================================================================================================================

        int currentState2 = digitalRead(switchPin2);

        // Memeriksa jika terjadi perubahan dari HIGH ke LOW
        if ((currentMillis - previousMillis_task7_2_2a > interval_task7_2_2a || previousMillis_task7_2_2a == 0) && (currentState2 == LOW && lastState2 == HIGH)) {
      
            count2++;
            Serial.println("Switch 2 count: " + String(count2));


//            lcd.setCursor(2, 2);
//            lcd.print("A");
//            lcd.setCursor(2, 3);
//            lcd.print("2");
            
            lcd.setCursor(3, 2);
            lcd.write(8);
            lcd.setCursor(3, 3);
            lcd.print(count2);
        
            previousMillis_task7_2_2a = currentMillis;
        }

        // task7_2_2b ========================================================================================================================================================================
    
        // Memeriksa jika terjadi perubahan dari LOW ke HIGH
        if ((currentMillis - previousMillis_task7_2_2b > interval_task7_2_2b || previousMillis_task7_2_2b == 0) && (currentState2 == HIGH && lastState2 == LOW)) {
      
            previousMillis_task7_2_2b = currentMillis;
        }

        // task7_2_3a ========================================================================================================================================================================

        int currentState3 = digitalRead(switchPin3);

        // Memeriksa jika terjadi perubahan dari HIGH ke LOW
        if ((currentMillis - previousMillis_task7_2_3a > interval_task7_2_3a || previousMillis_task7_2_3a == 0) && (currentState3 == LOW && lastState3 == HIGH)) {
      
            count3++;
            Serial.println("Switch 3 count: " + String(count3));

//            lcd.setCursor(4, 2);
//            lcd.print("A");
//            lcd.setCursor(4, 3);
//            lcd.print("3");
            
            lcd.setCursor(5, 2);
            lcd.write(8);
            lcd.setCursor(5, 3);
            lcd.print(count3);
        
            previousMillis_task7_2_3a = currentMillis;
        }

        // task7_2_3b ========================================================================================================================================================================
    
        // Memeriksa jika terjadi perubahan dari LOW ke HIGH
        if ((currentMillis - previousMillis_task7_2_3b > interval_task7_2_3b || previousMillis_task7_2_3b == 0) && (currentState3 == HIGH && lastState3 == LOW)) {
            
            previousMillis_task7_2_3b = currentMillis;
        }

        // task7_2_4a ========================================================================================================================================================================

        int currentState4 = digitalRead(switchPin4);

        // Memeriksa jika terjadi perubahan dari HIGH ke LOW
        if ((currentMillis - previousMillis_task7_2_4a > interval_task7_2_4a || previousMillis_task7_2_4a == 0) && (currentState4 == LOW && lastState4 == HIGH)) {
      
            count4++;
            Serial.println("Switch 4 count: " + String(count4));

//            lcd.setCursor(6, 2);
//            lcd.print("A");
//            lcd.setCursor(6, 3);
//            lcd.print("4");
            
            lcd.setCursor(7, 2);
            lcd.write(8);
            lcd.setCursor(7, 3);
            lcd.print(count4);
        
            previousMillis_task7_2_4a = currentMillis;
        }

        // task7_2_4b ========================================================================================================================================================================
    
        // Memeriksa jika terjadi perubahan dari LOW ke HIGH
        if ((currentMillis - previousMillis_task7_2_4b > interval_task7_2_4b || previousMillis_task7_2_4b == 0) && (currentState4 == HIGH && lastState4 == LOW)) {
            
            previousMillis_task7_2_4b = currentMillis;
        }

        lastState1 = currentState1;
        lastState2 = currentState2;
        lastState3 = currentState3;
        lastState4 = currentState4;
    
        // task7_3 ===========================================================================================================================================================================

        if (currentMillis - previousMillis_task7_3 > interval_task7_3 || previousMillis_task7_3 == 0) {
        
            if ((now.hour() == onDailyHour && now.minute() == onDailyMinute && now.second() > onDailySecond) &&
                (now.hour() == onDailyHour && now.minute() == onDailyMinute && now.second() <= offDailySecond)) {
            
                if (currentMillis - previousMillis_task7_3a >= interval_task7_3a) {
                    previousMillis_task7_3a = currentMillis;

                    if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyA1", count1) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyA2", count2) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyA3", count3) && 
                        Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageDailyA4", count4)) {
                    }
                    Serial.println("sent count");
                }
                
            } else {

            }
      
            previousMillis_task7_3 = currentMillis;
        }
    
        // task7_4 ===========================================================================================================================================================================

        if (currentMillis - previousMillis_task7_4 > interval_task7_4 || previousMillis_task7_4 == 0) {

            if ((now.day() == onMonthlyDay && now.hour() == onMonthlyHour && now.minute() == onMonthlyMinute && now.second() > onMonthlySecond) &&
                (now.day() == onMonthlyDay && now.hour() == onMonthlyHour && now.minute() == onMonthlyMinute && now.second() <= offMonthlySecond)) {

                if (currentMillis - previousMillis_task7_4a >= interval_task7_4a) {
                    previousMillis_task7_4a = currentMillis;
                    
                    int previousCount1 = count1;
                    int previousCount2 = count2;
                    int previousCount3 = count3;
                    int previousCount4 = count4;

                    int value0 = count1;
                    int value1 = count2;
                    int value2 = count3;
                    int value3 = count4;
        
                    count1 = 0;
                    count2 = 0;
                    count3 = 0;
                    count4 = 0;

                    Serial.println("START");

// sheet =====================================================================================================================================================================================

                    static bool flag = false;
                    if (!flag){
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

                    if (previousCount1 < targetMonthly && previousCount2 < targetMonthly && previousCount3 < targetMonthly && previousCount4 < targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", previousCount3) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA4", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "A3") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA4", "A4")) {
                        }

                        Serial.println("1, 2, 3, 4");
            
                    } else if (previousCount1 >= targetMonthly && previousCount2 < targetMonthly && previousCount3 < targetMonthly && previousCount4 < targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount3) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A3") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "A4")) {
                        }

                        Serial.println("2, 3, 4");
            
                    } else if (previousCount1 < targetMonthly && previousCount2 >= targetMonthly && previousCount3 < targetMonthly && previousCount4 < targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount3) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A3") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "A4")) {
                        }

                        Serial.println("1, 3, 4");
            
                    } else if (previousCount1 < targetMonthly && previousCount2 < targetMonthly && previousCount3 >= targetMonthly && previousCount4 < targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "A4")) {
                        }

                        Serial.println("1, 2, 4");
            
                    } else if (previousCount1 < targetMonthly && previousCount2 < targetMonthly && previousCount3 < targetMonthly && previousCount4 >= targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", previousCount3) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "A3")) {
                        }

                        Serial.println("1, 2, 3");

                    } else if (previousCount1 >= targetMonthly && previousCount2 >= targetMonthly && previousCount3 < targetMonthly && previousCount4 < targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount3) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A3") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A4")) {
                        }

                        Serial.println("3, 4");

                    } else if (previousCount1 < targetMonthly && previousCount2 >= targetMonthly && previousCount3 >= targetMonthly && previousCount4 < targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A4")) {
                        }

                        Serial.println("1, 4");

                    } else if (previousCount1 < targetMonthly && previousCount2 < targetMonthly && previousCount3 >= targetMonthly && previousCount4 >= targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount2) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A2")) {
                        }

                        Serial.println("1, 2");

                    } else if (previousCount1 >= targetMonthly && previousCount2 < targetMonthly && previousCount3 < targetMonthly && previousCount4 >= targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount3) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A3")) {
                        }

                        Serial.println("2, 3");

                    } else if (previousCount1 >= targetMonthly && previousCount2 < targetMonthly && previousCount3 >= targetMonthly && previousCount4 < targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount2) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount4) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A2") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A4")) {
                        }

                        Serial.println("2, 4");

                    } else if (previousCount1 < targetMonthly && previousCount2 >= targetMonthly && previousCount3 < targetMonthly && previousCount4 >= targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", previousCount3) && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "A3")) {
                        }

                        Serial.println("1, 3");
          
                    } else if (previousCount1 >= targetMonthly && previousCount2 >= targetMonthly && previousCount3 >= targetMonthly && previousCount4 < targetMonthly) {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount4) && 
                    
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A4")) {
                        }

                        Serial.println("4");
          
                    } else if (previousCount1 < targetMonthly && previousCount2 >= targetMonthly && previousCount3 >= targetMonthly && previousCount4 >= targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount1) && 
                    
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A1")) {
                        }

                        Serial.println("1");
          
                    } else if (previousCount1 >= targetMonthly && previousCount2 < targetMonthly && previousCount3 >= targetMonthly && previousCount4 >= targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount2) && 
                    
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A2")) {
                        }

                        Serial.println("2");
          
                    } else if (previousCount1 >= targetMonthly && previousCount2 >= targetMonthly && previousCount3 < targetMonthly && previousCount4 >= targetMonthly) {
          
                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", previousCount3) && 
                    
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "A3")) {
                        }

                        Serial.println("3");
          
                    } else {

                        if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA1", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA2", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA3", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageMonthlyA4", "") && 
                        
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA1", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA2", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA3", "") && 
                            Firebase.RTDB.setString(&fbdo, "My_SmartCage/CageNameMonthlyA4", "")) {
                        }

                        Serial.println("----");
                    }
                }
            
            } else {
                Serial.println("FINISH");
            }

            previousMillis_task7_4 = currentMillis;
        }

        // task0 ==============================================================================================================================================================================

        if (currentMillis - previousMillis_task0 >= interval_task0 || previousMillis_task0 == 0) {
          
            Firebase.reconnectWiFi(true);

            previousMillis_task0 = currentMillis;
        }
    }
}
