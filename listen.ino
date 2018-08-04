/////////////
void listen()   // Listen for client connection
{

  if(WiFi.status() != WL_CONNECTED)
  {
      wifiStart();

      delay(1000 * 10);   //wait 10 seconds before writing 

     //Open a "WIFI.TXT" for appended writing.   Client access ip address logged.
     File logFile = SPIFFS.open("/WIFI.TXT", "a");

     if (!logFile)
     {
          Serial.println("File failed to open");
     }
     else
     {
          logFile.print("Connected WiFi:  ");
          logFile.println(dtStamp);
     }

  
  }

  if ((MINUTE % 15 == 0) && (SECOND == 0))   //Write Data to "LOG.TXT" every 15 MINUTES.
  {

      getWeatherData();

      lastUpdate = dtStamp;   //store dtstamp for use on dynamic web page
      updateDifference();  //Get Barometric Pressure difference
      logtoSD();   //Output to SPIFFS  --Log to SPIFFS on 15 minute interval.
      delay(10);  //Be sure there is enough SPIFFS write time
      webInterface();
      speak();
      
      pastPressure = currentPressure;

  }

  // Check if a client has connected
  client = server.available();
    
  if (client)
  {

    // Process this request until it completes or times out.
    // Note that this is explicitly limited to handling one request at a time!

    // Clear the incoming data buffer and point to the beginning of it.
    bufindex = 0;
    memset(&buffer, 0, sizeof(buffer));

    // Clear action and path strings.
    memset(&action, 0, sizeof(action));
    memset(&path,   0, sizeof(path));

    // Set a timeout for reading all the incoming data.
    unsigned long endtime = millis() + TIMEOUT_MS;

    // Read all the incoming data until it can be parsed or the timeout expires.
    bool parsed = false;

    while (!parsed && (millis() < endtime) && (bufindex < BUFFER_SIZE))
    {

      if (client.available())
      {

         buffer[bufindex++] = client.read();
         
      }

      parsed = parseRequest(buffer, bufindex, action, path);

    }

    if(parsed)
    {

        error = 0;

      // Check the action to see if it was a GET request.
      if (strcmp(action, "GET") == 0)
      {

         digitalWrite(online,HIGH);   //turn on online LED indicator

         String ip1String = "10.0.0.146";   //Host ip address
         String ip2String = client.remoteIP().toString();   //client remote IP address

         Serial.print("\n");
         Serial.println("Client connected:  " + dtStamp);
         Serial.print("Client IP:  ");
         Serial.println(ip2String);
         Serial.println(F("Processing request"));
         Serial.print(F("Action: "));
         Serial.println(action);
         Serial.print(F("Path: "));
         Serial.println(path);

         //accessLog();

        // Check the action to see if it was a GET request.
        if (strncmp(path, "/favicon.ico", 12) == 0)  // Respond with the path that was accessed.
        {
          char *filename = "/FAVICON.ICO";
          strcpy(MyBuffer, filename);

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: image/x-icon");
          client.println();

          error = 0;            

          readFile();

        }
        // Check the action to see if it was a GET request.
        if ((strcmp(path, "/Weather") == 0) || (strcmp(path, "/") == 0))   // Respond with the path that was accessed.
        {

          getWeatherData();

          if(!isnan(dew))
          {
            delay(200);
  
            // First send the success response code.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: html");
            client.println("Connnection: close");
            client.println("Server: Robotdyn WiFi D1 R2");
            // Send an empty line to signal start of body.
            client.println("");
            // Now send the response data.
            // output dynamic webpage
            client.println("<!DOCTYPE HTML>");
            client.println("<html lang='en'>");
            client.println("<head><title>Weather Observations</title></head>");
            client.println("<body>");
            // add a meta refresh tag, so the browser pulls again every 15 seconds:
            //client.println("<meta http-equiv=\"refresh\" content=\"15\">");
            client.println("<h2>Treyburn Lakes<br>");
            client.println("Indianapolis, IN 46239</h2><br>");
                      
            if(lastUpdate != NULL)
            {
               client.println("Last Update:  ");
               client.println(lastUpdate);
               client.println("   <br>");
            }
  
            client.println("Humidity:  ");
            client.print(hum, 1);
            client.print(" %<br>");
            client.println("Dew point:  ");
            client.print(DewPoint, 1);
            client.print(" F. <br>");
            client.println("Temperature:  ");
            client.print(temp, 1);
            client.print(" F.<br>");
            client.println("Heat Index:  ");
            client.print(HeatIndex, 1);
            client.print(" F. <br>");
            client.println("Barometric Pressure:  ");
            client.print(currentPressure, 3);   //Inches of Mercury
            client.print(" inHg.<br>");
  
            if (pastPressure == currentPressure) 
            {
               client.println(0.000);
               client.print(" inHg --Difference since last update <br>");
            }
            else
            {
               client.println(difference, 3);
               client.print(" inHg --Difference since last update <br>");
            }
  
            client.println("Barometric Pressure:  ");
            client.println(millibars, 1);   //Convert hPA to millbars
            client.println(" mb.<br>");
            client.println("Atmosphere:  ");
            client.print(atm, 2);
            client.print(" atm <br>");
            client.println("Elevation:  824 Feet<br>");
            client.println("<h2>Weather Observations</h2>");
            client.println("<h3>" + dtStamp + "    </h3><br>");
  
            links();
  
            end();

          }
          else
          {
            
            // First send the success response code.
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: html");
            client.println("Connnection: close");
            client.println("Server: Robotdyn WiFi D1 R2");
            // Send an empty line to signal start of body.
            client.println("");
            // Now send the response data.
            // output dynamic webpage
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><title>Weather Observations</title>");
            client.println("<body>");
            // add a meta refresh tag, so the browser pulls again every 15 seconds:
            //client.println("<meta http-equiv=\"refresh\" content=\"15\">");
            client.println("<h2>Treyburn Lakes<br>");
            client.println("Indianapolis, IN 46239</h2></head><br>");

            if(lastUpdate != NULL)
            {
               //client.println("Last Update:  ");
               client.println(lastUpdate);
               client.println("   <br>");
            }
            
            client.println("<h2>Invalid Data --Sensor Failure</h2\n");
            client.println("</body>");
            client.println("</html>");

            links();
            
            end();
            
          }
          
          
        }
        // Check the action to see if it was a GET request.
        else if (strcmp(path, "/SdBrowse") == 0)   // Respond with the path that was accessed.
        {

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");
          client.println("<head><title>SDBrowse</title><head />");
          // print all the files, use a helper to keep it clean
          client.println("<h2>Collected Observations</h2>");

          //////////////// Code to listFiles from martinayotte of the "ESP8266 Community Forum" ///////////////
          String str = String("<html><head></head>");

          if (!SPIFFS.begin())
          {
             Serial.println("SPIFFS failed to mount !");
          }
          Dir dir = SPIFFS.openDir("/");
          while (dir.next())
          {
             str += "<a href=\"";
             str += dir.fileName();
             str += "\">";
             str += dir.fileName();
             str += "</a>";
             str += "    ";
             str += dir.fileSize();
             str += "<br>\r\n";
          }
          str += "</body></html>\r\n";

          client.print(str); 

          ////////////////// End code by martinayotte //////////////////////////////////////////////////////
          client.println("<br><br>");
          client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/Weather    >Current Observations</a><br>");
          client.println("</body>");
          client.println("</html>");

          end();

        } 
        // Check the action to see if it was a GET request.
        else if (strcmp(path, "/Graphs") == 0)   // Respond with the path that was accessed. 
        {

          //delayTime =1000;
      
      // First send the success response code.
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: html");
          client.println("Connnection: close");
          client.println("Server: Robotdyn D1 R2");
          // Send an empty line to signal start of body.
          client.println("");
          // Now send the response data.
          // output dynamic webpage
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");
          client.println("<head><title>Graphed Weather Observations</title></head>");
          // add a meta refresh tag, so the browser pulls again every 15 seconds:
          //client.println("<meta http-equiv=\"refresh\" content=\"15\">");
          client.println("<br>");
          client.println("<h2>Graphed Weather Observations</h2><br>");
          client.println("<p>");
          client.println("<frameset rows='30%,70%' cols='33%,34%'>");
          client.println("<iframe width='450' height='260' 'border: 1px solid #cccccc;' src='https://thingspeak.com/channels/290421/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&timescale=15&title=Temperature&type=line&xaxis=Date&yaxis=Degrees'></iframe>");
          client.println("<iframe width='450' height='260' 'border: 1px solid #cccccc;' src='https://thingspeak.com/channels/290421/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&timescale=15&title=Humidity&type=line&xaxis=Date&yaxis=Humidity'></iframe>");
          client.println("<br>");
          client.println("<br>");
          delay(200);
          //client.println("</frameset>");
          //client.println("<frameset rows='30%,70%' cols='33%,34%'>");
          client.println("<iframe width='450' height='260' 'border: 1px solid #cccccc;' src='https://thingspeak.com/channels/290421/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&timescale=15&title=Barometric+Pressure&type=line&xaxis=Date&yaxis=Pressure'></iframe>");
          client.println("<iframe width='450' height='260' 'border: 1px solid #cccccc;' src='https://thingspeak.com/channels/290421/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&timescale=15&title=Dew+Point&type=line'></iframe>");
          client.println("</frameset>");
          client.println("<br><br>");
          client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/Weather    >Current Observations</a><br>");
          client.println("<br>");
          client.println("</p>");
          client.println("</body>");
          client.println("</html>");

          end(); 

        }
        else if((strncmp(path, "/LOG", 4) == 0) ||  (strcmp(path, "/WIFI.TXT") == 0) || (strcmp(path, "/DIFFER.TXT") == 0)|| (strcmp(path, "/SERVER.TXT") == 0) || (strcmp(path, "/README.TXT") == 0))     // Respond with the path that was accessed.
        {

          char *filename;
          char name;
          strcpy( MyBuffer, path );
          filename = &MyBuffer[1];

          if ((strncmp(path, "/FAVICON.ICO", 12) == 0) || (strncmp(path, "/SYSTEM~1", 9) == 0) || (strncmp(path, "/ACCESS", 7) == 0))
          {

             client.println("HTTP/1.1 404 Not Found");
             client.println("Content-Type: text/html");
             client.println();
             client.println("<h2>404</h2>");
             delay(250);
             client.println("<h2>File Not Found!</h2>");
             client.println("<br><br>");
             client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/SdBrowse    >Return to File Browser</a><br>");

             Serial.println("Http 404 issued");
       
             error = 1; 

            end();
          }
          else 
          {

             client.println("HTTP/1.1 200 OK");
             client.println("Content-Type: text/plain");
             client.println("Content-Disposition: attachment");
             client.println("Content-Length:");
             client.println("Connnection: close");
             client.println();

             readFile(); 
       
             end();
          }

        }
        // Check the action to see if it was a GET request.
        else if(strncmp(path, "/zzzzzzzzzzz", 7) == 0)   //replace "/zzzzzzz" with your choice.  Makes "ACCESS.TXT" a restricted file.  Use this for private access to remote IP file.
        {
          //Restricted file:  "ACCESS.TXT."  Attempted access from "Server Files:" results in
          //404 File not Found!

          char *filename = "/ACCESS.TXT";
          strcpy(MyBuffer, filename);

          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/plain");
          client.println("Content-Disposition: attachment");
          //client.println("Content-Length:");
          client.println();

          readFile();

          end();

        }
        else  
        {

          delay(1000);

          // everything else is a 404
          client.println("HTTP 404 Not Found");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<h2>404</h2>");
          delay(250);
          client.println("<h2>File Not Found!</h2>");
          client.println("<br><br>");
          client.println("<a href=http://" + publicIP + ":" + LISTEN_PORT + "/SdBrowse    >Return to File Browser</a><br>");
          
          Serial.println("Http 404 issued");
          
          error = 1;

          end();
          
        }
        
      }
      else 
      {
        // Unsupported action, respond with an HTTP 405 method not allowed error.
        Serial.print(action);
        client.println("HTTP Method Not Allowed");
        client.println("");
        Serial.println("");
        Serial.println("Http 405 issued");
        Serial.println("");

        digitalWrite(online, HIGH);   //turn-on online LED indicator

        error = 2;

        end();

      }
       
      accessLog();
  
    }   
    
  }
    

}

//********************************************************************
//////////////////////////////////////////////////////////////////////
// Return true if the buffer contains an HTTP request.  Also returns the request
// path and action strings if the request was parsed.  This does not attempt to
// parse any HTTP headers because there really isn't enough memory to process
// them all.
// HTTP request looks like:
//  [method] [path] [version] \r\n
//  Header_key_1: Header_value_1 \r\n
//  ...
//  Header_key_n: Header_value_n \r\n
//  \r\n
bool parseRequest(uint8_t* buf, int bufSize, char* action, char* path)
{
     // Check if the request ends with \r\n to signal end of first line.
     if (bufSize < 2)
          return false;

     if (buf[bufSize - 2] == '\r' && buf[bufSize - 1] == '\n')
     {
          parseFirstLine((char*)buf, action, path);
          return true;
     }
     return false;
}


///////////////////////////////////////////////////////////////////
// Parse the action and path from the first line of an HTTP request.
void parseFirstLine(char* line, char* action, char* path)
{
     // Parse first word up to whitespace as action.
     char* lineaction = strtok(line, " ");

     if (lineaction != NULL)

          strncpy(action, lineaction, MAX_ACTION);
     // Parse second word up to whitespace as path.
     char* linepath = strtok(NULL, " ");

     if (linepath != NULL)

          strncpy(path, linepath, MAX_PATH);
}


