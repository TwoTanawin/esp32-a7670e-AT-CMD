#include <HardwareSerial.h>

#define MODEM_PWRKEY         4
#define MODEM_TX             26
#define MODEM_RX             27

HardwareSerial SerialAT(1);

void sendATCommand(const char* cmd);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  // Configure the modem power key pin
  pinMode(MODEM_PWRKEY, OUTPUT);
  digitalWrite(MODEM_PWRKEY, LOW);

  // Start communication with the SIMCom module
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  // Power on the module
  Serial.println("Powering on the module...");
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, LOW);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(5000);

  // Wait for the module to power up
  delay(10000);

  // Send AT command
  sendATCommand("AT");
}

void loop() {
  // Nothing here
}

void sendATCommand(const char* cmd) {
  Serial.print("Sending command: ");
  Serial.println(cmd);
  SerialAT.println(cmd);

  unsigned long timeout = millis() + 5000; // 5 seconds timeout
  while (millis() < timeout) {
    if (SerialAT.available()) {
      String response = SerialAT.readString();
      Serial.print("Response: ");
      Serial.println(response);
      if (response.indexOf("OK") != -1) {
        Serial.println("AT command executed successfully.");
      } else {
        Serial.println("No OK response received.");
      }
      return;
    }
  }
  Serial.println("No response or timeout.");
}
