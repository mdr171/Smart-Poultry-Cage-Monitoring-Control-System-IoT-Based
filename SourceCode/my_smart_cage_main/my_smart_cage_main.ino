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

// rtc =======================================================================================================================================================================================

#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

const char* monthName[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const char* dayName[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// 00:00
// rtc
int onRTCHour = 0;
int onRTCMinute = 0;

int onRTCSecond = 0;
int offRTCSecond = 30;

// vit
int onVitSecond = 0;
int offVitSecond = 30;

// dht11 =====================================================================================================================================================================================

#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// servo =====================================================================================================================================================================================

#include <Servo.h>

Servo myservo;

int firstCorner = 30;
int secondCorner = 90;

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

// millis ====================================================================================================================================================================================

unsigned long previousMillis_task0 = 0;
unsigned long interval_task0 = 200;

unsigned long previousMillis_task1_1 = 0;
unsigned long interval_task1_1 = 2000;

unsigned long previousMillis_task1_2 = 0;
unsigned long interval_task1_2 = 5000;

unsigned long previousMillis_task2_1 = 0;
unsigned long interval_task2_1 = 2000;

unsigned long previousMillis_task2_2 = 0;
unsigned long interval_task2_2 = 40000;

unsigned long previousMillis_task2_3 = 0;
unsigned long interval_task2_3 = 40000;

unsigned long previousMillis_task3 = 0;
unsigned long interval_task3 = 1000;

unsigned long previousMillis_task4Eks = 0;
unsigned long interval_task4Eks = 2000;

unsigned long previousMillis_task4Manual = 0;
unsigned long interval_task4Manual = 2000;

unsigned long previousMillis_task4Auto_1 = 0;
unsigned long interval_task4Auto_1 = 2000;

unsigned long previousMillis_task4Auto_2 = 0;
unsigned long interval_task4Auto_2 = 2000;

unsigned long previousMillis_task4Auto_3 = 0;
unsigned long interval_task4Auto_3 = 3000;

unsigned long previousMillis_task5Eks = 0;
unsigned long interval_task5Eks = 2000;

unsigned long previousMillis_task5Manual = 0;
unsigned long interval_task5Manual = 2000;

unsigned long previousMillis_task5Auto_1 = 0;
unsigned long interval_task5Auto_1 = 2000;

unsigned long previousMillis_task5Auto_2 = 0;
unsigned long interval_task5Auto_2 = 2000;

unsigned long previousMillis_task5Auto_3 = 0;
unsigned long interval_task5Auto_3 = 3000;

unsigned long previousMillis_task6Eks = 0;
unsigned long interval_task6Eks = 2000;

unsigned long previousMillis_task6Manual = 0;
unsigned long interval_task6Manual = 2000;

unsigned long previousMillis_task6Manual_a = 0;
unsigned long interval_task6Manual_a = 40000;

unsigned long previousMillis_servoStatus = 0;
unsigned long durationServo = 2000;

unsigned long previousMillis_task6Auto_1 = 0;
unsigned long interval_task6Auto_1 = 2000;

unsigned long previousMillis_task6Auto_2 = 0;
unsigned long interval_task6Auto_2 = 2000;

unsigned long previousMillis_task6Auto_2a = 0;
unsigned long interval_task6Auto_2a = 40000;

unsigned long previousMillis_task6Auto_3 = 0;
unsigned long interval_task6Auto_3 = 3000;

unsigned long previousMillis_task6Auto_3a = 0;
unsigned long interval_task6Auto_3a = 40000;

bool statusServo = false;

bool statusVit = false;

// ===========================================================================================================================================================================================
// void setup ================================================================================================================================================================================

void setup() {
  
    Serial.begin(115200);
    
    analogReference(EXTERNAL);

    // relay =================================================================================================================================================================================
    
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);

    myservo.write(firstCorner);

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
    Serial.print("Connected: ");
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
//        delay(1500);
        lcd.clear();

        signupOK = true;
        
    } else {
      
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }

    // assign the callback function for the long running token generation task.
    config.token_status_callback = tokenStatusCallback;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

}

// ===========================================================================================================================================================================================
// void loop =================================================================================================================================================================================

void loop() {

    unsigned long currentMillis = millis(); // grab current time

    // modeOnline ============================================================================================================================================================================

    if (WiFi.status() == WL_CONNECTED) {

        // task1_1 ===========================================================================================================================================================================

        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();

        if ((currentMillis - previousMillis_task1_1 >= interval_task1_1 || previousMillis_task1_1 == 0) && (statusVit == false)) {

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

        if ((currentMillis - previousMillis_task1_2 >= interval_task1_2 || previousMillis_task1_2 == 0) && (statusVit == false)) {

            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/Temp", temperature)) {

                Serial.println("Temp");
            }
            
            previousMillis_task1_2 = currentMillis;
        }

        // task2_1 ===========================================================================================================================================================================

        DateTime now = rtc.now();

        now = now + TimeSpan(0, 0, 5, 59);

        int previousMonthIndex = now.month() - 2;
        if (previousMonthIndex < 0) {
            previousMonthIndex = 11;
        }

        if ((currentMillis - previousMillis_task2_1 >= interval_task2_1 || previousMillis_task2_1 == 0) && (statusVit == false)) {

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

        if ((currentMillis - previousMillis_task2_2 >= interval_task2_2 || previousMillis_task2_2 == 0) && 
            (now.hour() == onRTCHour && now.minute() == onRTCMinute && now.second() > onRTCSecond) &&
            (now.hour() == onRTCHour && now.minute() == onRTCMinute && now.second() <= offRTCSecond) && (statusVit == false)) {
                    
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/DayName", dayName[now.dayOfTheWeek()]) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/Day", now.day()) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/MonthName", monthName[now.month() - 1]) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/Year", now.year()) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/PreviousMonthName", monthName[previousMonthIndex])) {
            }

            Serial.println("RTC Day");
            
            previousMillis_task2_2 = currentMillis;
        }

        // task2_3 ===========================================================================================================================================================================

        if ((currentMillis - previousMillis_task2_3 >= interval_task2_3 || previousMillis_task2_3 == 0) && 
            (now.second() > onRTCSecond) && 
            (now.second() <= offRTCSecond) && (statusVit == false)) {
                    
            if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/Hour", now.hour()) && 
                Firebase.RTDB.setString(&fbdo, "My_SmartCage/Minute", now.minute())) {
            }

            Serial.println("RTC Minute");
            
            previousMillis_task2_3 = currentMillis;
        }

        // task3 =============================================================================================================================================================================

        if ((currentMillis - previousMillis_task3 >= interval_task3 || previousMillis_task3 == 0) && (statusVit == false)) {

            lcd.clear();
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

            // time
            lcd.setCursor(0, 2);
            lcd.print(now.hour(), DEC);
            lcd.print(':');
            lcd.print(now.minute(), DEC);
            lcd.setCursor(0, 3);
            lcd.print(now.day(), DEC);
            lcd.print(" ");
            lcd.print(monthName[now.month() - 1]);
            lcd.print(" ");
            lcd.print(now.year(), DEC);
        
            previousMillis_task3 = currentMillis;
        }

        // task4Eks ==========================================================================================================================================================================

        static bool task4Eks = false;

        if ((currentMillis - previousMillis_task4Eks >= interval_task4Eks || previousMillis_task4Eks == 0) && (statusVit == false)) {
          
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
            }

            previousMillis_task4Eks = currentMillis;
        }

        // task4Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task4Manual >= interval_task4Manual || previousMillis_task4Manual == 0) && (task4Eks == false) && (statusVit == false)) {
          
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
                
            }

            previousMillis_task4Manual = currentMillis;
        }

        // task4Auto_1 =======================================================================================================================================================================

        int maxTemp;

        if ((currentMillis - previousMillis_task4Auto_1 >= interval_task4Auto_1 || previousMillis_task4Auto_1 == 0) && (statusVit == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/MaxTempBox")) {

                String Value = fbdo.stringData();
            
                Value.replace("\"", "");
                Value.replace("\\", "");

                maxTemp = Value.toInt();

                Serial.println("Max Temp");
            }
            
            previousMillis_task4Auto_1 = currentMillis;
        }
      
        // task4Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task4Auto_2 >= interval_task4Auto_2 || previousMillis_task4Auto_2 == 0) && (task4Eks == true) && (statusVit == false)) {
                
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

        // task4Auto_3 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task4Auto_3 >= interval_task4Auto_3 || previousMillis_task4Auto_3 == 0) && (task4Eks == true) && (statusVit == false)) {
          
            if (temperature > maxTemp) {

                int Value = 1;
                              
                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C1", String(Value))) {
                  
                }
                
            } else {

                int Value = 0;

                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C1", String(Value))) {
                  
                }

            }

            previousMillis_task4Auto_3 = currentMillis;
        }
        
        // task5Eks ==========================================================================================================================================================================

        static bool task5Eks = false;

        if ((currentMillis - previousMillis_task5Eks >= interval_task5Eks || previousMillis_task5Eks == 0) && (statusVit == false)) {
          
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
                
            }

            previousMillis_task5Eks = currentMillis;
        }

        // task5Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task5Manual >= interval_task5Manual || previousMillis_task5Manual == 0) && (task5Eks == false) && (statusVit == false)) {
          
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
                
            }

            previousMillis_task5Manual = currentMillis;
        }

        // task5Auto_1 =======================================================================================================================================================================

        int onLightHour;
        int onLightMinute;
        int offLightHour;
        int offLightMinute;

        if ((currentMillis - previousMillis_task5Auto_1 >= interval_task5Auto_1 || previousMillis_task5Auto_1 == 0) && (statusVit == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OnLightHourBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                onLightHour = Value.toInt();

                Serial.println("On Light Hour");
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OnLightMinuteBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                onLightMinute = Value.toInt();

                Serial.println("On Light Minute");
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OffLightHourBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                offLightHour = Value.toInt();

                Serial.println("Off Light Hour");
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OffLightMinuteBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                offLightMinute = Value.toInt();

                Serial.println("Off Light Minute");
            }
            
            previousMillis_task5Auto_1 = currentMillis;
        }

        // task5Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task5Auto_2 >= interval_task5Auto_2 || previousMillis_task5Auto_2 == 0) && (task5Eks == true) && (statusVit == false)) {   
          
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

        // task5Auto_3 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task5Auto_3 >= interval_task5Auto_3 || previousMillis_task5Auto_3 == 0) && (task5Eks == true) && (statusVit == false)) {
          
            if ((now.hour() == onLightHour && now.minute() >= onLightMinute) || 
                (now.hour() > onLightHour && now.hour() < offLightHour) ||
                (now.hour() == offLightHour && now.minute() <= offLightMinute)) {

                int Value = 1;

                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C2", String(Value))) {

                }
                
            } else {

                int Value = 0;

                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C2", String(Value))) {

                }

            }

            previousMillis_task5Auto_3 = currentMillis;
        }

        // task6Eks ==========================================================================================================================================================================

        static bool task6Eks = false;

        if ((currentMillis - previousMillis_task6Eks >= interval_task6Eks || previousMillis_task6Eks == 0)  && (statusVit == false)) {
          
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
            }

            previousMillis_task6Eks = currentMillis;
        }

        // task6Manual =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task6Manual >= interval_task6Manual || previousMillis_task6Manual == 0) && (task6Eks == false) && (statusVit == false)) {
          
            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/C3")) {
              
                if (fbdo.dataType() == "string") {
                  
                    int Value = fbdo.stringData().toInt();
                    if (Value == 1) {

                        lcd.setCursor(15, 0);
                        lcd.write(6);

                        Serial.print("M3: ");
                        Serial.println(Value);

                        if (currentMillis - previousMillis_task6Manual_a >= interval_task6Manual_a || previousMillis_task6Manual_a == 0) {
                            previousMillis_task6Manual_a = currentMillis;

                            Serial.println("statusServo: ");
                            Serial.println(statusServo);

                            statusServo = true;
                            statusVit = true;

                            previousMillis_servoStatus = currentMillis;  // Mulai waktu untuk servo
                        }
                        
                    } else if (Value == 0) {

                        lcd.setCursor(15, 0);
                        lcd.print(" ");

                        Serial.print("M3: ");
                        Serial.println(Value);
                    }
                }
            }
        
            previousMillis_task6Manual = currentMillis;
        }

        // task6Auto_1 =======================================================================================================================================================================

        int onVitHour;
        int onVitMinute;

        if ((currentMillis - previousMillis_task6Auto_1 >= interval_task6Auto_1 || previousMillis_task6Auto_1 == 0) && (task6Eks == true) && (statusVit == false)) {

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OnVitHourBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                onVitHour = Value.toInt();

                Serial.println("On Vit Hour");
            }

            if (Firebase.RTDB.getString(&fbdo, "My_SmartCage/OnVitMinuteBox")) {
                String Value = fbdo.stringData();
                Value.replace("\"", "");
                Value.replace("\\", "");
                onVitMinute = Value.toInt();

                Serial.println("On Vit Minute");
            }
            
            previousMillis_task6Auto_1 = currentMillis;
        }

        // task6Auto_2 =======================================================================================================================================================================

        if ((currentMillis - previousMillis_task6Auto_2 >= interval_task6Auto_2 || previousMillis_task6Auto_2 == 0) && (task6Eks == true) && (statusVit == false)) {
          
            if ((now.hour() == onVitHour && now.minute() == onVitMinute && now.second() > onVitSecond) &&
                (now.hour() == onVitHour && now.minute() == onVitMinute && now.second() <= offVitSecond)) {

                int Value = 1;
            
                lcd.setCursor(15, 0);
                lcd.write(6);

                Serial.print("A3: ");
                Serial.println(Value);

                if (currentMillis - previousMillis_task6Auto_2a >= interval_task6Auto_2a || previousMillis_task6Auto_2a == 0) {
                    previousMillis_task6Auto_2a = currentMillis;

                    Serial.println("statusServo 2: ");
                    Serial.println(statusServo);

                    statusServo = true;
                    statusVit = true;

                    previousMillis_servoStatus = currentMillis;  // Mulai waktu untuk servo
                }
                        
            } else {

                int Value = 0;

                lcd.setCursor(15, 0);
                lcd.print(" ");

                Serial.print("A3: ");
                Serial.println(Value);
            }
            
            previousMillis_task6Auto_2 = currentMillis;
        }

        if ((statusServo) && (statusVit)) {

            delay(300);
          
            if (currentMillis - previousMillis_servoStatus >= durationServo) {
                myservo.write(firstCorner);

                statusServo = false;
                statusVit = false;

                Serial.println("statusServo FINISH: ");
                Serial.println(statusServo);
            } else {
                myservo.write(secondCorner);
                
                Serial.println("statusServo START: ");
                Serial.println(statusServo);
            }
        }

//        // task6Auto_3 =======================================================================================================================================================================
//
//        if ((currentMillis - previousMillis_task6Auto_3 >= interval_task6Auto_3 || previousMillis_task6Auto_3 == 0) && (task6Eks == true) && (statusVit == false)) {
//          
//            if ((now.hour() == onVitHour && now.minute() == onVitMinute && now.second() > onVitSecond) &&
//                (now.hour() == onVitHour && now.minute() == onVitMinute && now.second() <= offVitSecond)) {
//                
//                if (currentMillis - previousMillis_task6Auto_3a >= interval_task6Auto_3a  || previousMillis_task6Auto_3a == 0) {
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
//            }
//
//            if (currentMillis - previousMillis_task6Auto_3a >= durationServo) {
//                statusServo = false;
//                
//                int Value = 0;
//
//                if (Firebase.RTDB.setString(&fbdo, "My_SmartCage/C3", String(Value))) {
//
//                }
//            }
//        }

        // task0 =============================================================================================================================================================================

        if (currentMillis - previousMillis_task0 >= interval_task0 || previousMillis_task0 == 0) {
          
            Firebase.reconnectWiFi(true);

            previousMillis_task0 = currentMillis;
        }
        
    }
    
}
