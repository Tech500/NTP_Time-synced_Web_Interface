//////////
void end()
{
  
  accessLog();
  
  // Wait a short period to make sure the response had time to send before
  // the connection is closed .
  
  delay(1000);
  
  //Client flush buffers
  client.flush();
  // Close the connection when done.
  client.stop();
  
  digitalWrite(online, LOW);   //turn-off online LED indicator

  getDateTime();
  
  Serial.println("Client closed:  " + dtStamp);

  delay(100);   //Delay for changing too quickly to new browser tab.



  listen();                
}

