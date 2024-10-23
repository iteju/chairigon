#define BLYNK_TEMPLATE_ID "TMPL36gqTa1yi"
#define BLYNK_TEMPLATE_NAME "Wheel Chair"
#define BLYNK_AUTH_TOKEN "KoYCLKTOCDo0_yetsH9uhIfKMAyanxYj"
#define BLYNK_PRINT Serial

#define IN1 14
#define IN2 27
#define IN3 26
#define IN4 25

#define LEFT_SENSOR 32
#define RIGHT_SENSOR 33

// Blynk Virtual Pin for Left and Right control
#define VPIN_LEFT V0
#define VPIN_RIGHT V1

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Wi-Fi credentials
const char* ssid = "tsr";
const char* pass = "1234567890";

bool manualControl = false;  // If true, robot is under manual control
bool moveLeft = false;
bool moveRight = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  Serial.begin(115200);
  stopMotors();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");

  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);
}

void loop() {
  Blynk.run();  // Run Blynk
  if (!manualControl) {  // Line following logic only when not in manual control mode
    int leftState = digitalRead(LEFT_SENSOR);
    int rightState = digitalRead(RIGHT_SENSOR);
    Serial.print("Left Sensor: ");
    Serial.println(leftState);

    if (leftState == LOW && rightState == LOW) {  // Both sensors on the line
      moveForward();
    } else if (leftState == HIGH && rightState == LOW) {  // Left sensor off the line, right sensor on the line
      turnRight();
    } else if (leftState == LOW && rightState == HIGH) {  // Left sensor on the line, right sensor off the line
      turnLeft();
    } else {  // Both sensors off the line
      stopMotors();
    }
  }
}

// Blynk Button 1 (Left Control)
BLYNK_WRITE(VPIN_LEFT) {
  int pinValue = param.asInt();  // Get button value (0 or 1)
  if (pinValue) {  // Button pressed
    manualControl = true;
    moveLeft = true;
    turnLeft();
    delay(1000);  // Move left for 1 second
    manualControl = false;
  }
}

// Blynk Button 2 (Right Control)
BLYNK_WRITE(VPIN_RIGHT) {
  int pinValue = param.asInt();  // Get button value (0 or 1)
  if (pinValue) {  // Button pressed
    manualControl = true;
    moveRight = true;
    turnRight();
    delay(1000);  // Move right for 1 second
    manualControl = false;
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
