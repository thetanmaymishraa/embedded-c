// Copyright 2023-2024 KAIA.AI

#ifndef ESP32
    #error This example runs on ESP32
#endif

#include <WiFi.h>
#include <WebServer.h>
#include <LDS_YDLIDAR_X2_X2L.h>

// WiFi Credentials
const char* ssid = "My-Network";
const char* password = "aniket123";

// WebServer instance
WebServer server(80);

// LiDAR Pin Configuration
const uint8_t LIDAR_EN_PIN = 18;
const uint8_t LIDAR_PWM_PIN = 5;
const uint8_t LIDAR_TX_PIN = 17;
const uint8_t LIDAR_RX_PIN = 19;

#define LIDAR_PWM_FREQ     10000
#define LIDAR_PWM_BITS     11
#define LIDAR_PWM_CHANNEL  2

HardwareSerial LidarSerial(1);
LDS_YDLIDAR_X2_X2L lidar;

// Global Variable to Store LiDAR Data
String lidar_data = "Waiting for data...";

// HTML Page with Auto-Refresh
const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 LiDAR Data</title>
    <script>
        function fetchData() {
            fetch('/data')
            .then(response => response.text())
            .then(data => {
                document.getElementById('lidarData').innerText = data;
            });
        }
        setInterval(fetchData, 500);
    </script>
</head>
<body>
    <h2>ESP32 LiDAR Real-Time Data</h2>
    <p id="lidarData">Waiting for data...</p>
</body>
</html>
)rawliteral";

void handleRoot() {
    server.send_P(200, "text/html", html_page);
}

void handleLidarData() {
    server.send(200, "text/plain", lidar_data);
}

void setup() {
    Serial.begin(115200);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Start Web Server
    server.on("/", handleRoot);
    server.on("/data", handleLidarData);
    server.begin();
    Serial.println("Web Server Started!");

    // LiDAR Initialization
    Serial.print("LiDAR model: ");
    Serial.println(lidar.getModelName());

    Serial.print("LiDAR RX buffer size: ");
    Serial.print(LidarSerial.setRxBufferSize(1024));

    uint32_t baud_rate = lidar.getSerialBaudRate();
    Serial.print(", baud rate: ");
    Serial.println(baud_rate);

    LidarSerial.begin(baud_rate, SERIAL_8N1, LIDAR_RX_PIN, LIDAR_TX_PIN);

    lidar.setScanPointCallback(lidar_scan_point_callback);
    lidar.setPacketCallback(lidar_packet_callback);
    lidar.setSerialWriteCallback(lidar_serial_write_callback);
    lidar.setSerialReadCallback(lidar_serial_read_callback);
    lidar.setMotorPinCallback(lidar_motor_pin_callback);
    lidar.init();

    LDS::result_t result = lidar.start();
    Serial.print("LiDAR start() result: ");
    Serial.println(lidar.resultCodeToString(result));

    if (result < 0) {
        Serial.println("Is the LiDAR connected to ESP32?");
    }
}

void loop() {
    server.handleClient();  // Web Server Handling
    lidar.loop();           // LiDAR Processing
}

// LiDAR Serial Read Callback
int lidar_serial_read_callback() {
    return LidarSerial.read();
}

// LiDAR Serial Write Callback
size_t lidar_serial_write_callback(const uint8_t *buffer, size_t length) {
    return LidarSerial.write(buffer, length);
}

// LiDAR Scan Point Callback - Updates Web Server Data
void lidar_scan_point_callback(float angle_deg, float distance_mm, float quality, bool scan_completed) {
    static int i = 0;
    if (i % 20 == 0) {
        Serial.print("Point ");
        Serial.print(i);
        Serial.print(": Distance = ");
        Serial.print(distance_mm);
        Serial.print(" mm, Angle = ");
        Serial.println(angle_deg);

        lidar_data = "Point: " + String(i) + " | Distance: " + String(distance_mm) + " mm | Angle: " + String(angle_deg);
    }
    i++;

    if (scan_completed) {
        i = 0;
        Serial.println("Scan completed");
    }
}

// LiDAR Error Callback
void lidar_error_callback(LDS::result_t code, String aux_info) {
    Serial.print("LiDAR error ");
    Serial.print(lidar.resultCodeToString(code));
    Serial.print(": ");
    Serial.println(aux_info);
}

// LiDAR Motor Control Callback
void lidar_motor_pin_callback(float value, LDS::lds_pin_t lidar_pin) {
    int pin = (lidar_pin == LDS::LDS_MOTOR_EN_PIN) ? LIDAR_EN_PIN : LIDAR_PWM_PIN;

    if (value <= (float)LDS::DIR_INPUT) {
        if (value == (float)LDS::DIR_OUTPUT_PWM) {
            #if ESP_IDF_VERSION_MAJOR < 5
                ledcSetup(LIDAR_PWM_CHANNEL, LIDAR_PWM_FREQ, LIDAR_PWM_BITS);
                ledcAttachPin(pin, LIDAR_PWM_CHANNEL);
            #else
                ledcAttachChannel(pin, LIDAR_PWM_FREQ, LIDAR_PWM_BITS, LIDAR_PWM_CHANNEL);
            #endif
        } else {
            pinMode(pin, (value == (float)LDS::DIR_INPUT) ? INPUT : OUTPUT);
        }
        return;
    }

    if (value < (float)LDS::VALUE_PWM) {
        digitalWrite(pin, (value == (float)LDS::VALUE_HIGH) ? HIGH : LOW);
    } else {
        int pwm_value = ((1 << LIDAR_PWM_BITS) - 1) * value;
        #if ESP_IDF_VERSION_MAJOR < 5
            ledcWrite(LIDAR_PWM_CHANNEL, pwm_value);
        #else
            ledcWriteChannel(LIDAR_PWM_CHANNEL, pwm_value);
        #endif
    }
}

// LiDAR Packet Callback
void lidar_packet_callback(uint8_t *packet, uint16_t length, bool scan_completed) {
    return;
}
