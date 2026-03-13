#include <Wire.h>
#include <Adafruit_MPR121.h>

Adafruit_MPR121 mpr = Adafruit_MPR121();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Khoi dong MPR121...");

  if (!mpr.begin(0x5A)) {
    Serial.println("KHONG TIM THAY MPR121! Kiem tra day cap.");
    while (1);
  }

  Serial.println("MPR121 OK!");
  mpr.setThresholds(6, 3);
}

void loop() {
  uint16_t touched = mpr.touched();

  // Gui filtered, baseline, va touch
  Serial.print("{\"touch\":[");
  for (int i = 0; i < 12; i++) {
    Serial.print((touched & (1 << i)) ? 1 : 0);
    if (i < 11) Serial.print(",");
  }
  Serial.print("],\"val\":[");
  for (int i = 0; i < 12; i++) {
    Serial.print(mpr.filteredData(i));
    if (i < 11) Serial.print(",");
  }
  Serial.print("],\"base\":[");
  for (int i = 0; i < 12; i++) {
    Serial.print(mpr.baselineData(i));
    if (i < 11) Serial.print(",");
  }
  Serial.println("]}");

  delay(100);
}
