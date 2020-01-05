#include <ESP8266WiFi.h>
#include <FirebaseArduino.h> 

#define FIREBASE_HOST "samsung-lecture.firebaseio.com"    // 데이터베이스 주소
#define FIREBASE_AUTH ""                                  // 데이터베이스 접속 키
#define WIFI_SSID "Test.dslab.24"                         // 와이파이 SSID
#define WIFI_PASSWORD "dslab123"                          // 와이파이 Password



void setup() {
  Serial.begin(115200);
  pinMode(D3 , OUTPUT);
  pinMode(D5 , OUTPUT);
  
  //와이파이 연결
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
  Serial.print(".");
  delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  //파이어베이스 연결
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  //아날로그값 입력 및 온도값 계산
  float temp = analogRead(A0);
  temp = round((temp*330/1024 - 55)*10)/10;
  
  Serial.print("temp : ");
  Serial.println(temp);
  
  Firebase.setFloat("Temperature", temp);
  
  if(Firebase.getString("LED/AUTO") == "true"){
    // Auto모드일 경우에만 실행
    Serial.println("Auto Mode");
    
    if(temp > 30){
      Firebase.setString("State", "HIGH");
      Firebase.setInt("Green", 0);
      Firebase.setInt("Red", 1);
    }else{
      Firebase.setString("State", "LOW");
      Firebase.setInt("Green", 1);
      Firebase.setInt("Red", 0);
    }
    
    int g_led = Firebase.getInt("Green");
    int r_led = Firebase.getInt("Red");
    
    String str = "Temperature is " + String(Firebase.getFloat("Temperature")) + ".\n" + Firebase.getString("State") + " Temperature.";
    Serial.println(str);
    
    digitalWrite(D3, r_led);
    digitalWrite(D5, g_led);
    
  }else if(Firebase.getString("LED/AUTO") == "false"){
    // Manual모드일 경우에만 실행
    Serial.println("Manual Mode");
    Serial.print("LED : ");
    
    int brightness = round(Firebase.getString("LED/Brightness").toFloat());
    
    if(Firebase.getString("LED/ON") == "true"){
      //LED/ON이 true이면 Brightness값으로 LED ON

      analogWrite(D3, brightness);
      analogWrite(D5, brightness);
      Serial.println("ON");
      
    } else if(Firebase.getString("LED/ON") == "false"){
      digitalWrite(D3, 0);
      digitalWrite(D5, 0);
      Serial.println("OFF");
    }
    Serial.print("Brightness : ");
    Serial.println(brightness);
    
  }

}
