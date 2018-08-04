/////////////////
void accessLog()
{
     getDateTime();

     String ip1String = "10.0.0.146";   //Host ip address
     String ip2String = client.remoteIP().toString();   //client remote IP address

     //Open a "access.txt" for appended writing.   Client access ip address logged.
     File logFile = SPIFFS.open("/ACCESS.TXT", "a");

     if (!logFile)
     {
          Serial.println("File failed to open");
     }

     if ((ip1String == ip2String) || (ip2String == "0.0.0.0"))
     {

          //Serial.println("IP Address match");
          logFile.close();

     }
     else
     {
          //Serial.println("IP address that do not match ->log client ip address");

          logFile.print("Accessed:  ");
          logFile.print(dtStamp + " -- ");
          logFile.print("Client IP:  ");
          logFile.print(client.remoteIP());
          logFile.print(" -- ");
          logFile.print("Action:  ");
          logFile.print(action);
          logFile.print(" -- ");
          logFile.print("Path:  ");
          logFile.print(path);

          //Serial.println("Error = " + (String)error);


          if ((error == 1) || (error == 2))
          {

               if(error == 1)
               {

                    Serial.println("Error 404");
                    logFile.print("  Error 404");
                    error = 0;

               }

               if(error == 2)
               {

                    Serial.println("Error 405");
                    logFile.print("  Error 405");

               }

          }

          error = 0;

          logFile.println("");
          logFile.close();

     }

}
