#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "KMU_SW";
const char* password = "kookminsw";

IPAddress ip (192, 168, 33, 29); // 연결할, 고정 IP 주소 --> wifi 설정때문인지 고정이 되지 않음 --> 그래도 시리얼 모니터에 나오는것 그대로 들어가서 사용하면 될
IPAddress gateway (192, 168, 0, 1); // 게이트웨이 주소
IPAddress subnet (255, 255, 252, 0); // 서브마스크 주소
IPAddress dns (168, 126, 63, 1); // DNS 주소
int ledPin = D3; // LED 핀 번호 설정 
WiFiServer server(80); //80 port로 웹서버를 열겠다~ ++++) server는 WiFiServer라는 클래스의 인스턴스가 됩니다.

const String url = "http://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?serviceKey=Hn8hkZzM%2B7YvGOv2I7um%2FU94g1HNg4Dp%2FB7PGzgCY2hPZKpX7DJ4urpTtr%2Bz%2FMq8XZydM%2BAbNiRWqNixDKrnKQ%3D%3D&returnType=xml&numOfRows=100&pageNo=1&stationName=%EC%84%B1%EB%B6%81%EA%B5%AC&dataTerm=DAILY&ver=1.3";
String line = "";
//미세먼지 저장 변수
float PM25;
float PM10;
String PM25_1;
String PM10_1;
String announce_time;  


//----------------------------------------------------------------------------------------------------------

void get_PM() {
  if ((WiFi.status() == WL_CONNECTED)) 
   {Serial.println("Starting connection to server(PM)...");
    HTTPClient http;
    http.begin(url);       //Specify the URL
    int httpCode = http.GET();  //Make the request
    if (httpCode > 0) {         //Check for the returning code
      line = http.getString();
   }
  else {Serial.println("Error on HTTP request");}
    parsing();
    http.end(); //Free the resources
  }
}

//----------------------------------------------------------------------------------------------------------

void parsing() {
  int data_start= line.indexOf(F("<dataTime>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int data_end= line.indexOf(F("</dataTime>"));  
  announce_time = line.substring(data_start + 10, data_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("announce_time: ")); Serial.println(announce_time);
  
  
  int PM25_start= line.indexOf(F("<pm25Value>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int PM25_end= line.indexOf(F("</pm25Value>"));  
  PM25_1 = line.substring(PM25_start + 11, PM25_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("PM2.5: ")); Serial.println(PM25_1);
  PM25 = PM25_1.toInt();   // 자료형 변경 String -> int
  
  int PM10_start= line.indexOf(F("<pm10Value>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int PM10_end= line.indexOf(F("</pm10Value>"));  
  PM10_1 = line.substring(PM10_start + 11, PM10_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("PM10: ")); Serial.println(PM10_1);
  PM10 = PM10_1.toInt();   // 자료형 변경 String -> int
  line = "";
}

//---------------------------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200); // 115200의 속도로 시리얼 통신을 시작.
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println(); // Serial에 ()에 해당하는 내용 전달
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.config (ip, gateway, subnet);
  WiFi.begin(ssid, password);
 
  
  while (WiFi.status() != WL_CONNECTED) { // wifi가 연결 될때까지 .출력
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin(); //웹 서버 시작
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP()); // ESP8266의 Ipv4 주소 출력 --> ESP8266은 wifi를 사용한다 --> 따라서 IP를 부여받는다!!
  Serial.println("/");
  get_PM();
}

//---------------------------------------------------------------------------------------------------------------------------

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available(); // client가 접속하면 true // client가 접속하지 않고 있다면 false를 반환

  if(client){ // 만약에 클라이언트가 접속했다면...
    Serial.println("new client entered");
    
    while(client.connected()){  // client가 계속 접속하고 있으면...(연결 유지 여부 확인)
      
      if(client.available()){ //buffer에 도착한 메시지가 있는지 체크해서 반환해준다
        // 해당 파트를 통해서 url parameter가 변화함에 따라 하드웨어를 제어할 수 있게 만들어줄 수 있습니다.
        String request = client.readStringUntil('\r'); //
        Serial.println(request);
        client.flush();
        int value = LOW;
        if (request.indexOf("/LED=ON") != -1) { // 만약에 요청(아마 url parameter)이 /LED=ON이다 --> LED를 켜준다.
          digitalWrite(ledPin, HIGH);
          value = HIGH;
        }
        if (request.indexOf("/LED=OFF") != -1){ // 만약에 요청(아마 url parameter)이 /LED=OFF이다 --> LED를 꺼준다.
          digitalWrite(ledPin, LOW);
          value = LOW;
        }
        
        // <HTML 파트입니다!!!>

        
        delay(1);
        break; // --> 이 break를 쓰지 않는다면, client에서 계속 HTML을 늘여써줘서 아래에 같은 웹이 뜨게 됩니다!! 중요!!
      }
    }
    client.stop(); // 클라이언트의 접속이 완료되면 접속 종료
    Serial.println("client disconnected");
  }
}
