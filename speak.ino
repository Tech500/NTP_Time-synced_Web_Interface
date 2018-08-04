////////////
void speak()
{

     char t_buffered1[14];
     dtostrf(temp, 7, 1, t_buffered1);

     char t_buffered2[14];
     dtostrf(hum, 7, 1, t_buffered2);

     char t_buffered3[14];
     dtostrf(currentPressure, 7, 3, t_buffered3);

     char t_buffered4[14];
     dtostrf(DewPoint, 7, 1, t_buffered4);

     // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
     // pieces of information in a channel.  Here, we write to field 1.
     // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
     // pieces of information in a channel.  Here, we write to field 1.
     ThingSpeak.setField(1, t_buffered1);  //Temperature
     ThingSpeak.setField(2, t_buffered2);  //Humidity
     ThingSpeak.setField(3, t_buffered3);  //Barometric Pressure
     ThingSpeak.setField(4, t_buffered4);  //Dew Point F.

     // Write the fields that you've set all at once.
     ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
     
   getDateTime();
     
   Serial.println("");
     Serial.println("Sent data to Thingspeak.com  " + dtStamp + "\n"); 

     listen();
   
}

