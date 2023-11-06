///////////////////////////////////////////////////////////////////////////
void testSend() {

  // Serial.println(packetSize);
  String address = "0x01";
  Serial.print("Sent message: ");
  Serial.println(address);
  LoRa.beginPacket();
  LoRa.print(address);
  LoRa.endPacket();
  LoRa.receive();

  delay(5000);
}

///////////////////////////////////////////////////////////////////////////
void receiveMSG(int packetSize) {
  // Serial.println("Receiving...");
  if (packetSize == 0) return;  // No data received

  if (packetSize) {
    receivedData = "";        // Initialize an empty string
                              //---------------------------------------- read packet header bytes:
    recipient = LoRa.read();  //--> recipient address
    Serial.print("Send to: ");
    Serial.print(recipient);
    sender = LoRa.read();  //--> sender address
    Serial.print(" ,Send from: ");
    Serial.print(sender);
    receivedDataLength = LoRa.read();  //--> incoming msg length
    Serial.print(" ,Send length: ");
    Serial.println(receivedDataLength);

    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    processMSG();

    // delay(3000);
  }
}

///////////////////////////////////////////////////////////////////////////
void processMSG() {

  //check if this MSG for me?
  if (recipient != hostDestinationSensor) {
    Serial.println("This message is not for sensor.");

    if (recipient != hostDestination) {
      Serial.println("This message is not for me.");

      smokeSensorValue = "N/A";
      server.send(200, "text/html", getPage(smokeSensorValue));
      return;  //--> skip rest of function
    }

    //MSG for alert!!!
    //check if this MSG length correct?
    if (receivedDataLength != receivedData.length()) {
      Serial.println("error: message length does not match length");

      server.send(200, "text/html", getPage(smokeSensorValue));
      return;  //--> skip rest of function

      Serial.print("Received: ");
      Serial.println(receivedData);

      smokeSensorValue = "N/A";
      server.send(200, "text/html", getPage(smokeSensorValue));
    }
    smokeSensorValue = "N/A";
    server.send(200, "text/html", getPage(smokeSensorValue));
  }

  //MSG for sensor
  //check if this MSG length correct?
  if (receivedDataLength != receivedData.length()) {
    Serial.println("error: message length does not match length");
    server.send(200, "text/html", getPage(smokeSensorValue));
    return;  //--> skip rest of function
  }
  LINE.notify(receivedData);
  Serial.print("Received: ");
  Serial.println(receivedData);

  smokeSensorValue = receivedData;
  
  server.send(200, "text/html", getPage(smokeSensorValue));
}

///////////////////////////////////////////////////////////////////////////
void beginServer() {
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getPage(smokeSensorValue));
  });

  server.on("/send", HTTP_POST, []() {
    String webMSGs = server.arg("message");

    if (webMSGs.length() > 0) {
      LoRa.beginPacket();

      // Convert the input string to a byte
      if (webMSGs.startsWith("0x")) {
        webMSGb = strtoul(webMSGs.c_str(), NULL, 16); // Assuming the input is in hexadecimal format (e.g., "0x01")
      } else {
        webMSGb = webMSGs.toInt(); // Convert the input to an integer (decimal)
      }
      LoRa.write(webMSGb);        //--> add destination address
      Serial.print("Send to: ");
      Serial.print(webMSGb);

      LoRa.write(hostDestination);       //--> add sender address
      Serial.print(" ,Send from: ");
      Serial.print(hostDestination);


      LoRa.write(webMSGs.length());  //--> add payload length
      Serial.print(" ,Send length: ");
      Serial.println(webMSGs.length());
      
      LoRa.print(webMSGs);
      LoRa.endPacket();
      LoRa.receive();
      Serial.print("Sent message: ");
      Serial.println(webMSGs);

      //รอให้รับค่า smoke ก่อนค่อยรันโค้ดต่อไป //ยังใช้ไม่ได้
      int i = 0;
      while (1) {
        Serial.println("while...");
      
        int packetSize = LoRa.parsePacket();
        if (packetSize != 0) {  
          receiveMSG(packetSize); // Process the received response
          Serial.println("Returning a response.");
          break;
        }

        i++; // Increment the counter

        if (i > 5000) {
          smokeSensorValue = "Server was interupt!! Please enter your IP again...";
          Serial.println("Reached the maximum time. Breaking the loop.");
          break; // Break the loop when i > 10000
        }
        
        // smokeSensorValue = "Wait for reaspond";
        // server.send(200, "text/html", getPage(smokeSensorValue));
      }


      Serial.print("smokeSensorValue: ");
      Serial.println(smokeSensorValue);
      // Send a response to the client
      
      server.send(200, "text/html", getPage(smokeSensorValue));
      }

    smokeSensorValue = "N/A";
    server.send(200, "text/html", getPage(smokeSensorValue));
    });

  server.begin();
  Serial.println("HTTP server started");
}



String getPage(String value) {
  String page = "<html><head><title>Forest Sensor</title><style>body{font-family:Arial,sans-serif;background-color:#f7f7f7;text-align:center;}h1{color:#333;margin-top:50px;}form{background-color:#fff;max-width:400px;margin:0 auto;padding:20px;border-radius:5px;box-shadow:0 0 10px rgba(0,0,0,0.2);}input[type='text']{width:100%;padding:10px;margin-bottom:10px;border:1px solid #ccc;border-radius:3px;}input[type='submit']{background-color:#007bff;color:#fff;border:none;padding:10px 20px;border-radius:3px;cursor:pointer;}input[type='submit']:hover{background-color:#0056b3;}#humidity-container,#temperature-container{background-color:#fff;border-radius:5px;padding:10px;margin:20px auto;max-width:300px;box-shadow:0px 0px 10px rgba(0,0,0,0.2);}#humidity-value,#temperature-value{font-weight:bold;color:#007BFF;}</style></head><body><h1>FOREST Sensor</h1><form action='/send' method='POST'><input type='text' name='message' placeholder='Enter your message'><input type='submit' value='Send'></form><div id='humidity-container'>Humidity: <span id='humidity-value'>"+humidity+"</span></div><div id='temperature-container'>Temperature: <span id='temperature-value'>"+temperature+"</span></div></body></html>";

    return page;
}