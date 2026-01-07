#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Server URL
const char* serverUrl = "Enter server url";

// NFC Module (MFRC522) Setup
#define SS_PIN 5    
#define RST_PIN 22  
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Custom I2C Pins for LCD
#define SDA_PIN 2
#define SCL_PIN 15

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Non-blocking delay settings
unsigned long lastScanTime = 0;
const unsigned long scanDelay = 500;

unsigned long lastMessageTime = 0;
bool showMessage = false;
const unsigned long messageDelay = 2000;

void setup() {
    Serial.begin(115200);
    SPI.begin();
    mfrc522.PCD_Init();

    Wire.begin(SDA_PIN, SCL_PIN);
    lcd.init();
    lcd.backlight();

    connectWiFi();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan a Card");
}

void loop() {
    unsigned long currentMillis = millis();

    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    if (currentMillis - lastScanTime >= scanDelay) {
        lastScanTime = currentMillis;
        scanCard();
    }

    if (showMessage && currentMillis - lastMessageTime >= messageDelay) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan a Card");
        showMessage = false;
    }
}

void connectWiFi() {
    Serial.print("Connecting to WiFi");
    lcd.setCursor(0, 0);
    lcd.print("Connecting WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
            delay(500);
            Serial.print(".");
        }
        
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("\nWiFi Failed! Retrying in 5 seconds...");
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("WiFi Failed!");
            delay(5000); // Wait for 5 seconds before retrying
        }
    }
    
    lcd.clear();
    Serial.println("\nConnected to WiFi!");
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    delay(2000);
    lcd.clear();
}

void scanCard() {
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    String cardUID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        cardUID += String(mfrc522.uid.uidByte[i], HEX);
    }
    cardUID.toUpperCase();

    if (cardUID.isEmpty()) {
        Serial.println("Invalid UID!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Invalid UID!");
        lastMessageTime = millis();
        showMessage = true;
        return;
    }

    Serial.println("NFC UID: " + cardUID);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    lcd.print(cardUID);
    sendDataToServer(cardUID);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

void sendDataToServer(String cardUID) {
    if (WiFi.status() != WL_CONNECTED) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi Error!");
        Serial.println("WiFi not connected!");
        lastMessageTime = millis();
        showMessage = true;
        return;
    }

    HTTPClient http;
    if (!http.begin(serverUrl)) {
        Serial.println("Failed to connect to server!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Server Error!");
        lastMessageTime = millis();
        showMessage = true;
        return;
    }

    http.addHeader("Content-Type", "application/json");
    DynamicJsonDocument doc(256);
    doc["nfc_uid"] = cardUID;
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sending...");
    int httpResponseCode = http.POST(jsonPayload);
    http.end();

    lcd.clear();
    lcd.setCursor(0, 0);
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
        lcd.print("Success!");
        Serial.println("Data sent successfully!");
    } else {
        lcd.print("Failed: " + String(httpResponseCode));
        Serial.println("HTTP Error Code: " + String(httpResponseCode));
    }
    lastMessageTime = millis();
    showMessage = true;
}
