// Line part
#include <TridentTD_LineNotify.h>

#define SSID "PPP_2.4GHz"                                         //ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD "0868034053"                                     //ใส่ รหัส Wifi
// #define SSID "AAA-xs"                                             //ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
// #define PASSWORD "AAAAA123"                                       //ใส่ รหัส Wifi
#define LINE_TOKEN "PvUfdSB9kEuNSzXdPaAOAAC7uohX136iw1ppT8onDPD"  //ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

// LoRa part
#include <SPI.h>
#include <LoRa.h>

/*
D21 --> LoRaNSS
D18 --> LoRaSCK
D19 --> LoRaMISO
D23 --> LoRaMOSI
*/
#define SS_PIN 21   // Define your NSS (Chip Select) pin 
#define RST_PIN 34  // Define your RST (Reset) pin
#define DI0_PIN 35  // Define your DIO0 (Interrupt) pin (optional)

//Web
#include <WebServer.h>
WebServer server(80);




///////////////////////////////////////////////////////////////////
// Source and destination addresses
byte hostDestination = 0xAA;  // address of this device
byte hostDestinationSensor = 0xBB;   // destination to 
byte slave01 = 0x01;   // destination to 

String receivedData = "";  // Initialize an empty string
String Incoming = ""; 
byte recipient;        //--> recipient address
byte sender;          //--> sender address
byte receivedDataLength;
byte webMSGb;
String smokeSensorValue = "N/A";
String humidity = "N/A";
String temperature = "N/A";

///////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
  /* 
  // my code 
  LINE.notify("Testing...");

  // ตัวอย่างส่งข้อความ
  LINE.notify("ยินดีต้อนรับ");
  LINE.notify("Cybertice.com");
  // ตัวอย่างส่งข้อมูล ตัวเลข
  LINE.notify(2342);          // จำนวนเต็ม
  LINE.notify(212.43434, 5);  // จำนวนจริง แสดง 5 หลัก

  // เลือก Line Sticker ได้จาก https://devdocs.line.me/files/sticker_list.pdf
  LINE.notifySticker(3, 240);         // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
  LINE.notifySticker("Hello", 1, 2);  // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ

  // ตัวอย่างส่ง รูปภาพ ด้วย url
  LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
  LINE.notifyPicture("จตุธาตุ", "https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");

*/

  //LoRa SetUp
  while (!Serial)
    ;
  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);  // Configure LoRa pins

  if (!LoRa.begin(433E6)) {  // Initialize LoRa at the desired frequency (e.g., 433 MHz)
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1)
      ;
  }

  Serial.println("LoRa receiver ready.");
  delay(2000);
  
  
  //Web
  beginServer();
}

///////////////////////////////////////////////////////////////////
void loop() {
  receiveMSG(LoRa.parsePacket());
  // testSend();

  server.handleClient();

  // Update "smoke-value" using JavaScript

}

///////////////////////////////////////////////////////////////////




