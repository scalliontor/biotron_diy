#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// ----------------------------------------------------------------------------
// Network Configuration
// ----------------------------------------------------------------------------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// The IP Address of the computer running TouchDesigner or the Python receiver
IPAddress outIp(192, 168, 1, 100); 
const unsigned int outPort = 8000;    // Port to send OSC messages to
const unsigned int localPort = 8888;  // Local port to listen on UDP

WiFiUDP Udp;

// ----------------------------------------------------------------------------
// Hardware Pins
// ----------------------------------------------------------------------------
// Capacitive Touch Pin - using GPIO4 (T0 on ESP32)
const int TOUCH_PIN = 4;
// LDR Pin (Light sensor) - using GPIO34 (ADC1_CH6)
const int LDR_PIN = 34;

// ----------------------------------------------------------------------------
// DSP Filter: Exponential Moving Average (EMA) Parameters
// ----------------------------------------------------------------------------
// Alpha handles the smoothing factor (0.0 < alpha <= 1.0)
// Higher alpha = less smoothing, more responsive. Lower alpha = more smoothing.
const float TOUCH_ALPHA = 0.2f;
const float LIGHT_ALPHA = 0.1f;

float emaTouchValue = 0.0f;
float emaLightValue = 0.0f;

// Timers for non-blocking read and send loops
unsigned long lastProcessTime = 0;
const int processIntervalMs = 20; // 50 Hz update rate (Every 20ms)

void setup() {
  Serial.begin(115200);

  // Setup ADC pin
  pinMode(LDR_PIN, INPUT);

  // Increase touch sensitivity
  // touchSetCycles(0x1000, 0x1000) will set measurement cycles and sleep cycles higher
  touchSetCycles(0x1000, 0x1000);

  // Initial read to set baseline for EMA
  emaTouchValue = (float)touchRead(TOUCH_PIN);
  emaLightValue = (float)analogRead(LDR_PIN);

  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(localPort);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastProcessTime >= processIntervalMs) {
    lastProcessTime = currentMillis;

    // 1. Read Raw Sensors
    // Capacitive Touch: lower value = stronger touch/closer body
    uint16_t rawTouch = touchRead(TOUCH_PIN);
    // Light (LDR + 10k divider): 0 to 4095
    uint16_t rawLight = analogRead(LDR_PIN);

    // 2. Apply Edge DSP (EMA Filter)
    emaTouchValue = (TOUCH_ALPHA * (float)rawTouch) + ((1.0f - TOUCH_ALPHA) * emaTouchValue);
    emaLightValue = (LIGHT_ALPHA * (float)rawLight) + ((1.0f - LIGHT_ALPHA) * emaLightValue);

    // 3. Construct and Send OSC Packets
    // Send Touch values
    OSCMessage msgTouch("/biotron/touch");
    msgTouch.add((float)emaTouchValue);
    Udp.beginPacket(outIp, outPort);
    msgTouch.send(Udp);
    Udp.endPacket();
    msgTouch.empty();

    // Send Light values
    OSCMessage msgLight("/biotron/light");
    msgLight.add((float)emaLightValue);
    Udp.beginPacket(outIp, outPort);
    msgLight.send(Udp);
    Udp.endPacket();
    msgLight.empty();

    // Optional: Print to Serial Plotter for local debugging
    // Serial.print("RawTouch:"); Serial.print(rawTouch); Serial.print(",");
    // Serial.print("EmaTouch:"); Serial.print(emaTouchValue); Serial.print(",");
    // Serial.print("RawLight:"); Serial.print(rawLight); Serial.print(",");
    // Serial.print("EmaLight:"); Serial.println(emaLightValue);
  }
}
