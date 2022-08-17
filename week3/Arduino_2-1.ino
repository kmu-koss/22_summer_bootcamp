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
        client.println("<title>IoT</title>");;
        client.println("");
        client.println("<!DOCTYPE HTML>");
        client.println("<html>");
        client.println("<head>");
        client.println("<meta charset='utf-8'>");
        client.println("<title>IoT</title>"); 
        client.println("<style>");
        client.println("</style>"); 
        client.println("</head>");
        client.println("<body>");
        client.println("<br /><br /><br/><br /><p class='font'>LED : <a href=\"/LED=ON\"><button class='font-sizeup'>ON</button></a></p>");
        client.println("<br /><br /><p class='font'>LED : <a href=\"/LED=OFF\"><button class='font-sizeup'>OFF</button></a></p>");
        client.println("</body>");
        client.println("</html>");
        client.println("");
        
        delay(1);
        break; // --> 이 break를 쓰지 않는다면, client에서 계속 HTML을 늘여써줘서 아래에 같은 웹이 뜨게 됩니다!! 중요!!
      }
    }
    client.stop(); // 클라이언트의 접속이 완료되면 접속 종료
    Serial.println("client disconnected");
  }
}
