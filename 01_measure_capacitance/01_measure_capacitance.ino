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
float baseline = 0;

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

    // Calib baseline - đừng chạm
    float sum = 0;
    for (int i = 0; i < 50; i++) {
        sum += touchRead(TOUCH_PIN);
        delay(10);
    }
    baseline = sum / 50.0;
    smooth = baseline;
    Serial.printf("Baseline: %.2f\n", baseline);
}

void loop() {
    int raw = touchRead(TOUCH_PIN);
    smooth = ALPHA * raw + (1.0 - ALPHA) * smooth;

    // Tay chạm → smooth giảm → delta dương
    // Tay chạm → smooth giảm → delta dương
    float delta = baseline - smooth;
    
    // Khôi phục lại công thức nguyên thủy siêu nhạy: nhân cứng thay vì %
    float value = constrain(delta * 3.0, 0.0, 100.0);

    Serial.printf("[🌿 TOUCH] %.2f\n", value);

    // Gửi OSC qua mạng
    OSCMessage msg("/biotron/touch");
    msg.add((float)value);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();

    delay(20);
}