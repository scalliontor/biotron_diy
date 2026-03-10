#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

#define TOUCH_PIN 4
#define ALPHA 0.1

// --- CẤU HÌNH WIFI ---
const char* ssid = "fustic.studio";
const char* password = "phuthinh";

// --- CẤU HÌNH OSC (TouchDesigner IP) ---
IPAddress outIp(192, 168, 0, 172); // <- Thây đổi thành IP máy tính chạy TouchDesigner
const unsigned int outPort = 8000; // <- Port cấu hình trong OSC In CHOP
WiFiUDP Udp;

float smooth = 0;

void setup() {
    Serial.begin(115200);

    // Kết nối WiFi
    Serial.printf("\nConnecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    touchSetCycles(0x800, 0x800);

    // Khởi tạo giá trị ban đầu cho EMA
    smooth = (float)touchRead(TOUCH_PIN);
}

void loop() {
    int raw = touchRead(TOUCH_PIN);
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;

    // Map thành 0-100 (chỉnh 2 số này theo thực tế)
    // Dùng kiểu float để chia tỷ lệ mịn hơn (Arduino map() mặc định dùng số nguyên int)
    float value = (smooth - 15.0) * (0.0 - 100.0) / (40.0 - 15.0) + 100.0;
    
    // Constrain giá trị trong khoảng 0-100
    if (value < 0) value = 0;
    if (value > 100) value = 100;

    Serial.printf("[🌿 RAW] %d | [🌿 SMOOTH] %.2f | [🌿 TOUCH] %.2f\n", raw, smooth, value);

    // Gửi OSC qua mạng
    OSCMessage msg("/biotron/touch");
    msg.add((float)value);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();

    delay(20);
}