///////////////
void readFile()
{

     digitalWrite(online, HIGH);   //turn-on online LED indicator

     String filename = (const char *)&MyBuffer;

     Serial.print("File:  ");
     Serial.println(filename);

     File webFile = SPIFFS.open(filename, "r");

     if (!webFile)
     {
          Serial.println("File failed to open");
          Serial.println("\n");
     }
     else
     {
          char buf[1024];
          int siz = webFile.size();
          //.setContentLength(str.length() + siz);
          //webserver.send(200, "text/plain", str);
          while (siz > 0)
          {
               size_t len = std::min((int)(sizeof(buf) - 1), siz);
               webFile.read((uint8_t *)buf, len);
               client.write((const char*)buf, len);
               siz -= len;
          }
          webFile.close();
     }

     error = 0;

     delayTime = 1000;

     MyBuffer[0] = '\0';

     digitalWrite(online, LOW);   //turn-off online LED indicator

     listen(); 
   
}

