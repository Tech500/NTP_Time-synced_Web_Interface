////////////////////////////////
void beep(unsigned char delayms)
{

     // wait for a delayms ms
     digitalWrite(sonalert, HIGH);
     delay(3000);
     digitalWrite(sonalert, LOW);

}
