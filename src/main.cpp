#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Servo.h>
#include <addons/RTDBHelper.h>

/* Define the WiFi credentials */
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

/* Define the RTDB URL */
#define DATABASE_URL "https://alpha-beta-food-locker-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* Define the Firebase Data object */
FirebaseData fbdo;

/* Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

int servoPin = 13;
Servo doorServo;

void setup()
{
    Serial.begin(115200);

    doorServo.attach(servoPin);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the database URL(required) */
    config.database_url = DATABASE_URL;

    config.signer.test_mode = true;

    Firebase.reconnectWiFi(true);
    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
}


void loop()
{
    int isLockerOpen = 0;
    if (millis() - dataMillis > 5000)
    {
        int pos = 0;
        dataMillis = millis();
        Serial.printf("Get int ref... %s\n", Firebase.getInt(fbdo, "/arduino/is_locker_open", &isLockerOpen) ? String(isLockerOpen).c_str() : fbdo.errorReason().c_str());
        if (isLockerOpen == 0) {
          Serial.println("Locker Closed!");
          pos = 0;
          doorServo.write(pos);
        } else if (isLockerOpen == 1) {
          Serial.println("Locker Open!");
          pos = 180;
          doorServo.write(pos);
        }    
    }
}
