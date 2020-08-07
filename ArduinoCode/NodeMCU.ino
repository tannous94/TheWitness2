#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>

#define ssid "Tannous"
#define pswd "******"

const char* host = "https://europe-west3-the-witness-eec95.cloudfunctions.net";
String str = "/getpins/";
String usr = "/acc/";
String mod_acc = "/update_acc/";

String acc_info[4] = {"", "", "", ""};
int m_index = 0;
// to deploy functions in firebase, type in cmd: firebase deploy --only functions

void(* resetFunc) (void) = 0; //declare reset function @ address 0

int flag = 1;
int sda = 17;
int csl = 16;
String payload;
String acc_payload;
String mod_payload;
String payload_check;

void check_for_new() {
  WiFiClient client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  Serial.println("Requesting URL: ");
  Serial.println("Checking if NON");

  String addr = host + str; //+ getData;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode != 200) {
      check_for_new();
  }
  payload_check = http.getString();
 // Serial.println(payload);
 delay(800);
 if (payload_check == "NON") {
      flag = 1;
        Wire.beginTransmission(8); /* begin with device address 8 */
       Wire.write('#');  /* sends hello string */
       Wire.endTransmission();
 } else {
   if (flag) {
    get_pins();
    delay(500);
    get_acc();
    delay(250);
    flag = 0;
  }
 }
  http.end();
  Serial.println("closing connection");
}

void get_pins() {
  WiFiClient client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  Serial.println("Requesting URL: ");
  Serial.println(str);
  //String getData = "?color1=(" + String(r) + "," + String(g) + "," + String(b) + ")";
  //getData += "&color2=(" + String(r) + "," + String(g) + "," + String(b) + ")";

  String addr = host + str; //+ getData;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode != 200) {
      get_pins();
  }
  payload = http.getString();
 // Serial.println(payload);
 if (payload != "NON") {
    for (int i = 0; i < payload.length(); i++) {
       Wire.beginTransmission(8); /* begin with device address 8 */
       Wire.write(payload[i]);  /* sends hello string */
       Wire.endTransmission();    /* stop transmitting */
    }
 }
  http.end();
  Serial.println("closing connection");
}

void get_acc() {
  WiFiClient client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  Serial.println("Requesting URL: ");
  Serial.println(usr);

  String addr = host + usr; //+ getData;
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode != 200) {
      get_acc();
  }
  acc_payload = http.getString();
 // Serial.println(payload);
 if (acc_payload != "NON") {
    for (int i = 0; i < acc_payload.length(); i++) {
       Wire.beginTransmission(8); /* begin with device address 8 */
       Wire.write(acc_payload[i]);  /* sends hello string */
       Wire.endTransmission();    /* stop transmitting */
    }
 }
  http.end();
  Serial.println("closing connection");
}

void update_acc_info() {
  WiFiClient client;
  const int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed!");
    return;
  }

  Serial.println("Requesting URL: ");
  Serial.println(mod_acc);

  String addr = host + mod_acc + "?lvl=" + acc_info[0] + "&pts=" + acc_info[1] + "&rnk=" + acc_info[2] + "&tim=" + acc_info[3];
  HTTPClient http;
  http.begin(addr);
  //http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // in POST
  auto httpCode = http.GET();
  Serial.println(httpCode);
  if (httpCode != 200) {
      update_acc_info();
  }
  mod_payload = http.getString();
 // Serial.println(payload);
 if (mod_payload != "NON") {
//    for (int i = 0; i < mod_payload.length(); i++) {
//       Wire.beginTransmission(8); /* begin with device address 8 */
//       Wire.write(mod_payload[i]);  /* sends hello string */
//       Wire.endTransmission();    /* stop transmitting */
//    }
    Serial.println("Data has been modified successfully");
 }
  http.end();
  Serial.println("closing connection");
}

void setup() {
 Serial.begin(9600); /* begin serial for debug */
 Wire.begin(sda, csl); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */

  Serial.println("Connecting to:");
  Serial.println(ssid);
  WiFi.begin(ssid, pswd);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Host: ");
  Serial.println(host);
  delay(3000);
 
}

void loop() {
  
  
//  if (flag) {
//    get_pins();
//    delay(500);
//    get_acc();
//    delay(250);
//    flag = 0;
//  }
  
check_for_new();
delay(200);
 Wire.requestFrom(8, 20); /* request & read data of size 13 from slave */
 while(Wire.available()){
    char c = Wire.read();
    if (c == ')'){
        m_index = 0;
        update_acc_info();
        for (int i = 0; i < 4; i++) {
            acc_info[i] = "";
        }
        delay(800);
        flag = 1;
       // resetFunc();
    }
    if (c >= '0' && c <= '9') {
        acc_info[m_index] += c;
    }
    if (c == '|') {
        m_index++;
    }
  Serial.print(c);
 }
 Serial.println();

 delay(1000);
}
