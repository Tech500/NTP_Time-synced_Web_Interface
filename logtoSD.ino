//////////////
void logtoSD()   //Output to SPIFFS Card every fifthteen minutes
{

     // Open a "log.txt" for appended writing
     File log = SPIFFS.open("/LOG.TXT", "a");

     if (!log)
     {
          Serial.println("file open failed");
     }

     //log.print(id);
     //log.print(" , ");
     log.print(lastUpdate);
     log.print(" , ");
     log.print("Humidity:  ");
     log.print(hum);
     log.print(" % , ");
     log.print("Dew Point:  ");
     log.print(DewPoint, 1);
     log.print(" F. , ");
     log.print(temp);
     log.print("  F. , ");
     // Reading temperature or humidity takes about 250 milliseconds!
     // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
     log.print("Heat Index:  ");
     log.print(HeatIndex, 1); 
     log.print(" F. ");
     log.print(" , ");
     log.print(currentPressure, 3);   //Inches of Mecury
     log.print(" in. Hg. ");
     log.print(" , ");

     if (pastPressure == currentPressure)
     {
          log.print("0.000");
          log.print(" Difference ");
          log.print(" ,");
     }
     else
     {
          log.print(difference,3);
          log.print(" Difference ");
          log.print(", ");
     }

     log.print(millibars, 2);  //Convert hPA to millibars
     log.print(" millibars ");
     log.print(" , ");
     log.print(atm, 3);
     log.print(" atm ");
     log.print(" , ");
     log.print("Elevation:  824 Feet");
     log.println();
     //Increment Record ID number
     //id++;
   Serial.print("\n");
   Serial.println("Data written to 'LOG.TXT'  " + dtStamp);
     Serial.print("\n");

     log.close();

     

     if(abs(difference) >= .020)   //After testing and observations of Data; raised from .010 to .020 inches of Mecury
     {
          // Open a "Differ.txt" for appended writing --records Barometric Pressure change difference and time stamps
          File diff = SPIFFS.open("/DIFFER.TXT", "a+");

          if (!diff)
          {
               Serial.println("file open failed");
          }

          Serial.println("\n");
          Serial.println("Difference greater than .020 inches of Mecury ,  ");
          Serial.print(difference, 3);
          Serial.println("\n");
          Serial.print("  ,");
          Serial.print(dtStamp);
          Serial.println("");

          diff.println("");
          diff.print("Difference greater than .020 inches of Mecury,  ");
          diff.print(difference, 3);
          diff.print("  ,");
          diff.print(dtStamp);
          diff.close();

          beep(50);  //Duration of Sonalert tone

     }

}



