/////////////////////
void getWeatherData()
{

     BME280::TempUnit tempUnit(BME280::TempUnit_Fahrenheit);
     BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    
     bme.read(pres, temp, hum, tempUnit, presUnit);

     currentPressure = ((pres * 0.0002953) + .874);   //convert Pascals to inches mercury --add correction factor    //  + .877
     millibars = ((pres * .01) + 28.8);   //convert Pascals to millibars  --plus correction factor  // + 29.7

     HeatIndex = -42.379 + 2.04901523*temp + 10.14333127*hum - .22475541*temp*hum - .00683783*temp*temp - .05481717*hum*hum + .00122874*temp*temp*hum + .00085282*temp*hum*hum - .00000199*temp*temp*hum*hum;

     if ((temp <= 79.988) || (hum <= 40)) HeatIndex = temp;
    
     DewPoint = (243.04 * (log(hum/100) + ((17.625 * temp)/(243.04+temp))) / (17.625-log(hum/100) - ((17.625 * temp) / (243.04+temp))));

     atm = pres * 0.00000986923267;   //Convert Pascals to Atm (atmospheric pressure)

}

