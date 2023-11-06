///////////////////////////////////////////////////////////////////////////
#include <SPI.h>
#include <LoRa.h>

#define SS_PIN 6   // NSS (Chip Select) connected to D10
#define RST_PIN 9  // RST (Reset) connected to D9
#define DI0_PIN 10  // DIO0 (Interrupt) connected to D2

// Source and destination addresses
byte hostDestination = 0xAA;   // destination to send to
byte hostDestinationSensor = 0xBB;   // destination to send to
byte localAddress = 0x01;  // address of this device


int last_saread = 1000;
String receivedData = "";  // Initialize an empty string
String Incoming = ""; 
byte recipient;        //--> recipient address
byte sender;          //--> sender address
byte receivedDataLength;


///////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  LoRa.setPins(SS_PIN, RST_PIN, DI0_PIN);  // Configure LoRa pins

  if (!LoRa.begin(433E6)) {  // Initialize LoRa at the desired frequency (e.g., 433 MHz)
    Serial.println("LoRa initialization failed. Check your connections.");
    while (1)
      ;
  }

  // // Set Receive Call-back function
  // LoRa.onReceive(onReceive);
  // // Place LoRa in Receive Mode
  LoRa.receive();

  Serial.println("LoRa receiver ready.");
  delay(2000);
}

///////////////////////////////////////////////////////////////////////////
void loop() {
  // testLoRaInit();

  sendMSG("Hello!", hostDestination);
  delay(1000);

  // analogRead();
  //-----------------Used code-----------------//
  // onReceive(LoRa.parsePacket());
  // digitalRead();
  //-------------------------------------------//
    // String message = "Alert!!";
    // sendMSG(message, hostDestination);
    // delay(3000);


  // receivedData = "";
  // Serial.println("end loop!!!");
}

///////////////////////////////////////////////////////////////////////////


