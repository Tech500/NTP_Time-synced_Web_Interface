////////////////
void fileStore()   //If 6th day of week, rename "log.txt" to ("log" + month + day + ".txt") and create new, empty "log.txt"
{

     // Open file for appended writing
     File log = SPIFFS.open("/LOG.TXT", "a");

     if (!log)
     {
          Serial.println("file open failed");
     }

     // rename the file "LOG.TXT"
     String logname;
     logname = "/LOG";
     logname += MONTH; ////logname += Clock.getMonth(Century);
     logname += DATE;   ///logname += Clock.getDate();
     logname += ".TXT";
     SPIFFS.rename("/LOG.TXT", logname.c_str());
     log.close();

     //For troubleshooting
     //Serial.println(logname.c_str());

}
