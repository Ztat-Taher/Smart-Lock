#include "esp_camera.h"
#include <WiFi.h>
#include <ESP32Servo.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char *ssid = "your wifi ssid";
const char *password = "your wifi password";

// MQTT broker details
const char *mqtt_server = "your broker's IP"; // Replace with your broker's IP
const int mqtt_port = 1883;

// Pins
#define TRIG_PIN 12
#define ECHO_PIN 13
#define BUZZER_PIN 14
#define SERVO_PIN 15
#define LED_WIFI 2
#define LED_MQTT 4

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

Servo servo; // Servo instance
WiFiClient espClient;
PubSubClient client(espClient);

void startCameraServer();
void setupLedFlash(int pin);

void connectToWiFi() {
  WiFi.begin(ssid, password);
  pinMode(LED_WIFI, OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(LED_WIFI, !digitalRead(LED_WIFI)); // Blink Wi-Fi LED
  }
  digitalWrite(LED_WIFI, HIGH); // Turn LED on when connected
}

void connectToMQTT() {
  client.setServer(mqtt_server, mqtt_port);
  pinMode(LED_MQTT, OUTPUT);
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      digitalWrite(LED_MQTT, HIGH); // Turn LED on when connected
    } else {
      delay(500);
      digitalWrite(LED_MQTT, !digitalRead(LED_MQTT)); // Blink MQTT LED
    }
  }
}

void publishDistance() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  if (distance < 20) { // Detect motion
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer on
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer off
    client.publish("door/motion", "Person detected at the door");
  }
}

void controlServo() {
  if (client.connected()) {
    client.loop();
    if (client.subscribe("servo/command")) {
      client.setCallback([](char *topic, byte *payload, unsigned int length) {
        String message;
        for (int i = 0; i < length; i++) {
          message += (char)payload[i];
        }
        if (message == "open") {
          servo.write(90); // Open door
        } else if (message == "close") {
          servo.write(0); // Close door
        }
      });
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 24000000;      
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;

  if (!psramFound()) {
    config.frame_size = FRAMESIZE_CIF; 
    config.fb_location = CAMERA_FB_IN_DRAM;
    Serial.println("PSRAM not found, using DRAM.");
  }

  // Initialize the camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed!");
    return;
  }

  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  servo.attach(SERVO_PIN);

  // Connect to Wi-Fi and MQTT
  connectToWiFi();
  connectToMQTT();

  // Start camera server
  startCameraServer();

  Serial.println("Setup complete. Camera ready!");
}

void loop() {
  publishDistance();
  controlServo();
  delay(100); // Adjust as needed
}
