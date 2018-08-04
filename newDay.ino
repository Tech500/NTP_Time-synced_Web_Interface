/////////////
void newDay()   //Collect Data for twenty-four hours; then start a new day
{

     //Do file maintence on 1st day of week at appointed time from RTC.  Assign new name to "log.txt."  Create new "log.txt."
     if ((DOW) == 6)
     {
          fileStore();
     }

     //id = 1;   //Reset id for start of new day
     //Write log Header

     // Open file for appended writing
     File log = SPIFFS.open("/LOG.TXT", "a");

     if (!log) 
     {
          Serial.println("file open failed");
     }
     else
     {

        delay(1000);
        log.println("......"); //Just a leading blank line, in case there was previous data
        log.println("Date, Time, Humidity, Dew Point, Temperature, Heat Index, in. Hg., Difference, millibars, Atm, Elevation");
        log.close();
            
        Serial.println("......");
        Serial.println("Date, Time, Humidity, Dew Point, Temperature, Heat Index, in. Hg., Difference, millibars, Atm, Elevation");
        Serial.println("");
                     
   }
     Serial.println("\n");

}
