void wifiStart()
{  
  
  WiFi.mode(WIFI_STA);

  //wifi_set_sleep_type(NONE_SLEEP_T);

  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  

  //setting the addresses   
  IPAddress ip(10,0,0,54);
  IPAddress gateway(10,0,0,1);
  IPAddress subnet(255,255,255,0); 
  IPAddress dns(75,75,75,75);

  WiFi.begin(ssid, password);

  WiFi.config(ip, gateway, subnet, dns);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(100);
    Serial.print("."); 
  }

  WiFi.waitForConnectResult();

  Serial.printf("Connection result: %d\n", WiFi.waitForConnectResult());

}
