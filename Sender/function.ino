
///////////////////////////////////////////////////////////////////////////
void testLoRaInit() {
  if (!LoRa.begin(433E6)) { // Initialize LoRa at the desired frequency (e.g., 433 MHz)
    Serial.println("LoRa initialization failed. Check your connections.");
  } else {
    Serial.println("LoRa receiver ready.");

  }

}

///////////////////////////////////////////////////////////////////////////
void analogRead() {
  int saread = analogRead(A7);
  int sdread = digitalRead(2);

  if (sdread == 1) {
    Serial.println("Gas Detected");
    String savalue = String(smokeValue(saread, last_saread));
    String smokeAlert = "1";

    String message = "Smokeก";  //AAAx001 is serial number of sensor // 1 if Detected fire
    LoRa.beginPacket();                 // Start packet transmission
    LoRa.print(message);                // Send your data
    LoRa.endPacket();                   // Finish packet transmission
    Serial.println(message);
    delay(2000);

    last_saread = saread;
  } else {
    Serial.println("not smoke");
    last_saread = saread;
  }

  Serial.print("Analog:");
  Serial.print(saread);
  Serial.print(',');
  Serial.print("upper_limit:");
  Serial.print("650");  // limit graph plot value
  Serial.print(',');
  Serial.print("lower_limit:");
  Serial.print("200");
  Serial.println();
  // delay(1000);

}

///////////////////////////////////////////////////////////////////////////
// Read sensor and detect if it has smoke?, print value, and send alert ///
void digitalRead() {
  int sdread = digitalRead(2);
  int saread = analogRead(A7);

  // detect when saread > 545
  if (sdread == 1) { 
    Serial.println("Gas Detected");
    String svalue = String(smokeValue(saread, last_saread));

    String message = "Alert!!";
    sendMSG(message, hostDestination);
    Serial.println("Sent");
    delay(2000);

    last_saread = saread;
    Serial.println("break if");
  } 
  else {
    Serial.println("no smoke");

    last_saread = saread;

  }

  Serial.print("Analog:");
  Serial.print(saread);
  Serial.print(',');
  Serial.print("Digital:");
  Serial.print(sdread*100);
  Serial.print(',');
  Serial.print("upper_limit:");
  Serial.print("650");  // limit graph plot value
  Serial.print(',');
  Serial.print("lower_limit:");
  Serial.print("200");
  Serial.println();
  // delay(1000);
  
}

///////////////////////////////////////////////////////////////////////////
int smokeValue(int sread, int last_sread) {
  int sdiff = sread - last_sread;
  return sdiff;
}

///////////////////////////////////////////////////////////////////////////
void onReceive(int packetSize) {
  // Serial.println("Receiving...");

  if (packetSize == 0) return;  // No data received
  
  if (packetSize) {
    receivedData = "";  // Initialize an empty string
    //---------------------------------------- read packet header bytes:
      recipient = LoRa.read();        //--> recipient address
      Serial.print("Send to: ");
      Serial.print(recipient);
      sender = LoRa.read();          //--> sender address
      Serial.print(" ,Send from: ");
      Serial.print(sender);
      receivedDataLength = LoRa.read();  //--> incoming msg length
      Serial.print(" ,Send length: ");
      Serial.println(receivedDataLength);

    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    processMSG();
    
  }
  // LoRa.receive();
}

///////////////////////////////////////////////////////////////////////////
void processMSG() {

  //check if this MSG for me?
  if (recipient != localAddress) {
    Serial.println("This message is not for me.");
    delay(2000);
    return; //--> skip rest of function
  }

  //check if this MSG length correct?
  if (receivedDataLength != receivedData.length()) {
    Serial.println("error: message length does not match length");
    delay(1000);
    return; //--> skip rest of function
  }

  Serial.print("Received: ");
  Serial.println(receivedData);

  String saread = String(analogRead(A7));
  sendMSG(saread, hostDestinationSensor);

  receivedData = "";
  delay(2000);
}

///////////////////////////////////////////////////////////////////////////
void sendMSG(String testMSG, byte Destination) {
    LoRa.beginPacket();            // Start packet transmission

    LoRa.write(Destination);        //--> add destination address
    Serial.print("Send to: ");
    Serial.print(Destination);

    LoRa.write(localAddress);       //--> add sender address
    Serial.print(" ,Send from: ");
    Serial.print(localAddress);

    LoRa.write(testMSG.length());  //--> add payload length
    Serial.print(" ,Send length: ");
    Serial.println(testMSG.length());
    
    // String message = "Alert!!";
    // sendMSG(message, hostDestination);

    Serial.println(testMSG);
    LoRa.print(testMSG);            // Send your data
    LoRa.endPacket();              // Finish packet transmission
    Serial.print("Sent message: ");
    Serial.println(testMSG);
    Serial.println("Printed");

  // delay(2000); // Send data every 10 seconds (adjust as needed)
}

///////////////////////////////////////////////////////////////////////////
// void sendMessage(String Outgoing, byte Destination) {
//   LoRa.beginPacket();             //--> start packet
//   LoRa.write(Destination);        //--> add destination address
//   LoRa.write(localAddress);       //--> add sender address
//   LoRa.write(Outgoing.length());  //--> add payload length
//   LoRa.print(Outgoing);           //--> add payload
//   LoRa.endPacket();               //--> finish packet and send it
//   Serial.println(Outgoing);

//   delay(2000);
// }
