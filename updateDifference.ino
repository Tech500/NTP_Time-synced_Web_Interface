////////////////////////
float updateDifference()  //Pressure difference for fifthteen minute interval
{

     //Function to find difference in Barometric Pressure
     //First loop pass pastPressure and currentPressure are equal resulting in an incorrect difference result.
     //Future loop passes difference results are correct

     difference = currentPressure - pastPressure;  //This will be pressure from this pass thru loop, pressure1 will be new pressure reading next loop pass
     if (difference == currentPressure)
     {
          difference = 0;
     }

     return (difference); //Barometric pressure change in inches of Mecury

}
