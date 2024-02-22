#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// Replace with the name of your WiFi network
const char* ssid = "AndroidAP";

// Replace with the password of your WiFi network
const char* password = "kareemash";

// Replace with the IP address of the ESP32
const IPAddress ip(192, 168, 43, 57);

// Replace with the port number you're using
const int port = 8080;

WiFiUDP udp;
int motorEnablePin1 = 14; // replace with the pin number connected to the enable pin of motor 1
int motorDir1Pin1 = 27;   // replace with the pin number connected to direction pin 1 of motor 1
int motorDir2Pin1 = 26;   // replace with the pin number connected to direction pin 2 of motor 1
int motorEnablePin2 = 5; // replace with the pin number connected to the enable pin of motor 2
int motorDir1Pin2 = 16;   // replace with the pin number connected to direction pin 1 of motor 2
int motorDir2Pin2 = 17;   // replace with the pin number connected to direction pin 2 of motor 2
int pwmChannel1 = 0;      // PWM channel for motor 1
int pwmChannel2 = 1;      // PWM channel for motor 2
int frequency = 30000;     // PWM frequency
int pwmResolution = 8;    // PWM resolution in bits

void setup() {
  Serial.begin(115200);
  pinMode(motorEnablePin1, OUTPUT);
  pinMode(motorDir1Pin1, OUTPUT);
  pinMode(motorDir2Pin1, OUTPUT);
  pinMode(motorEnablePin2, OUTPUT);
  pinMode(motorDir1Pin2, OUTPUT);
  pinMode(motorDir2Pin2, OUTPUT);
  ledcSetup(pwmChannel1, frequency, pwmResolution);
  ledcSetup(pwmChannel2, frequency, pwmResolution);
  ledcAttachPin(motorEnablePin1, pwmChannel1);
  ledcAttachPin(motorEnablePin2, pwmChannel2);
  
  // Connect to the WiFi network
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Set up the UDP socket
  udp.begin(port);
  
  Serial.println("Listening for UDP packets...");
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    
    char packetBuffer[255];
    udp.read(packetBuffer, packetSize);
    packetBuffer[packetSize] = 0;
    
    if (strcmp(packetBuffer, "1") == 0) {
      digitalWrite(motorDir1Pin1, HIGH);
      digitalWrite(motorDir2Pin1, LOW);
      digitalWrite(motorDir1Pin2, HIGH);
      digitalWrite(motorDir2Pin2, LOW);
      ledcWrite(pwmChannel1, 150);
      ledcWrite(pwmChannel2, 150);
      Serial.print("forward: ");
      udp.beginPacket(ip, port);
      // uint8_t msg[] = "Forward"; (msg,sizeOf(msg))
      udp.print("Forward");
      udp.endPacket();
    } else if (strcmp(packetBuffer, "-1") == 0) {
      digitalWrite(motorDir1Pin1, LOW);
      digitalWrite(motorDir2Pin1, HIGH);
      digitalWrite(motorDir1Pin2, LOW);
      digitalWrite(motorDir2Pin2, HIGH);
      ledcWrite(pwmChannel1, 150);
      ledcWrite(pwmChannel2, 150);
      Serial.print("backward: ");
      udp.beginPacket(ip, port);
      // uint8_t msg[] = "Backward";
      udp.print("Backward");
      udp.endPacket();
    } else if(strcmp(packetBuffer, "0") == 0){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, 0);
      Serial.print("stop: ");
      udp.beginPacket(ip, port);
      // uint8_t msg[] = "Stop";
      udp.print("test");
      udp.endPacket();
   }
    Serial.println(packetBuffer);
  }
  

}
