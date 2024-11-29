#include <Arduino.h>
#include <DHT.h>
#include <ESP32Servo.h>
#include "secrets/wifi.h"
#include "wifi_connect.h"
#include <WiFiClientSecure.h>
#include "ca_cert.h"
#include "secrets/mqtt.h"
#include <PubSubClient.h>
#include <Ticker.h>

float temperature = 0;
float humidity = 0;

#define DHTPIN 17      // Chân kết nối DHT11
#define DHTTYPE DHT11  // Loại cảm biến
#define LED_PIN 25     // Chân PWM điều khiển độ sáng đèn
#define SERVO_PIN 19   // Chân điều khiển Servo

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

namespace {
    const char *ssid = WiFiSecrets::ssid;
    const char *password = WiFiSecrets::pass;
    const char *Temp_topic = "Temp";
    const char *Hum_topic = "Hum";
    const char *LED_brightness_topic = "den";  // Chủ đề MQTT để điều khiển độ sáng đèn
    const char *Cua_topic = "Cua";            // Chủ đề MQTT để điều khiển cửa
    unsigned int publish_count = 0;
    uint16_t keepAlive = 15;    // seconds (default is 15)
    uint16_t socketTimeout = 5; // seconds (default is 15)
}

WiFiClientSecure tlsClient;
PubSubClient mqttClient(tlsClient);

Ticker mqttPulishTicker;

void mqttTempsensorPublish()
{
    // Đọc dữ liệu từ cảm biến DHT11
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    // Kiểm tra dữ liệu cảm biến
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
    }
    else {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C\tHumidity: ");
        Serial.print(humidity);
        Serial.println(" %");
    }
    
    // Đăng tải thông tin nhiệt độ và độ ẩm lên MQTT
    mqttClient.publish(Temp_topic, String(temperature).c_str(), false);
    mqttClient.publish(Hum_topic, String(humidity).c_str(), false);
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    if (strcmp(topic, LED_brightness_topic) == 0)
    {
        char brightnessStr[length + 1];
        memcpy(brightnessStr, payload, length);
        brightnessStr[length] = '\0';
        int brightness = atoi(brightnessStr);  // Chuyển đổi từ chuỗi sang giá trị số

        // Điều khiển độ sáng đèn LED
        Serial.print("LED brightness: ");
        Serial.println(brightness);
        
        // Áp dụng giá trị PWM cho LED (LED_PIN)
        analogWrite(LED_PIN, brightness * 255 / 10);  // Độ sáng nhận được là từ 0 đến 10, chuyển thành giá trị PWM 0-255
    }
    if (strcmp(topic, Cua_topic) == 0)
    {
        char commandStr[length + 1];
        memcpy(commandStr, payload, length);
        commandStr[length] = '\0';
        int command = atoi(commandStr);  // Chuyển đổi từ chuỗi sang giá trị số

        // Điều khiển servo để mở/đóng cửa
        if (command == 1)
        {
            // Quay servo để mở cửa (90 độ)
            Serial.println("Mở cửa...");
            myServo.write(90);
            delay(20);
        }
        else if (command == 0)
        {
            // Quay servo để đóng cửa (0 độ)
            Serial.println("Đóng cửa...");
            myServo.write(0);
            delay(20);  // Quay servo đến 0 độ
        }
    }
}

void mqttReconnect()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT connection...");
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        if (mqttClient.connect(client_id.c_str(), MQTT::username, MQTT::pass))
        {
            Serial.print(client_id);
            Serial.println(" connected");
            mqttClient.subscribe(LED_brightness_topic);  // Đăng ký nhận tin nhắn từ chủ đề điều khiển độ sáng LED
            mqttClient.subscribe(Cua_topic);            // Đăng ký nhận tin nhắn từ chủ đề điều khiển cửa
        }
        else
        {
            Serial.print("MQTT connect failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" try again in 1 seconds");
            delay(1000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    delay(10);
    setup_wifi(ssid, password);
    tlsClient.setCACert(ca_cert);

    mqttClient.setCallback(mqttCallback);
    mqttClient.setServer(MQTT::broker, MQTT::port);
    mqttPulishTicker.attach(1, mqttTempsensorPublish);  // Đặt thời gian cập nhật nhiệt độ và độ ẩm

    dht.begin();
    myServo.attach(SERVO_PIN);  // Chân kết nối Servo
    pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    if (!mqttClient.connected())
    {
        mqttReconnect();
    }
    mqttClient.loop();  // Lắng nghe các tin nhắn MQTT
}
