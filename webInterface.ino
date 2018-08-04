///////////////////
void webInterface() 
{
 
 
  char fahren[9];// Buffer big enough for 9-character float
  dtostrf(temp, 6, 1, fahren); // Leave room for too large numbers!

  char heat[9]; // Buffer big enough for 9-character float
  dtostrf(HeatIndex, 6, 1, heat); // Leave room for too large numbers!

  char humd[9]; // Buffer big enough for 9-character float
  dtostrf(hum, 6, 1, humd); // Leave room for too large numbers!

  char dewpt[9]; // Buffer big enough for 9-character float
  dtostrf(DewPoint, 6, 1, dewpt); // Leave room for too large numbers!

  char cpressure[9]; // Buffer big enough for 9-character float
  dtostrf(currentPressure, 6, 3, cpressure); // Leave room for too large numbers!

  char bars[9]; // Buffer big enough for 9-character float
  dtostrf(millibars, 6, 2, bars); // Leave room for too large numbers!

   
  String data = "fahren="     
            +  (String)fahren   
     
  + "&heat="                +  heat 
      
  + "&humd="                 +  humd

  + "&dewpt="               +  dewpt
    
  + "&cpressure="           +  cpressure
    
  + "&bars="                +  bars
    
  + "&last="                +  lastUpdate;
  
  
  if(WiFi.status()== WL_CONNECTED)
  {   //Check WiFi connection status

    HTTPClient http;    //Declare object of class HTTPClient

    http.begin("YourHostedDomainWebsiteURL/dataCollector.php");      //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header

    int httpCode = http.POST(data);   //Send the request
    String payload = http.getString();                  //Get the response payload

    

    if(httpCode == 200)
    {
      Serial.print("");
      Serial.print("HttpCode:  ");
      Serial.print(httpCode);   //Print HTTP return code
      Serial.print("  Data echoed back from Hosted website  " );
      Serial.println("");
      Serial.println(payload);    //Print payload response
     
      http.end();  //Close HTTPClient http
    }
    else  
    {
      Serial.print("");
      Serial.print("HttpCode:  ");
      Serial.print(httpCode);   //Print HTTP return code
      Serial.print("  Domain website data update failed.  ");
      Serial.println("");
     
      http.end();   //Close HTTPClient http
    }

    end();

  }
  else
  {

    Serial.println("Error in WiFi connection"); 
    
  }
  

}
