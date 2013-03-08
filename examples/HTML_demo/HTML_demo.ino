
/*
    Test for the HTML library
            (08/03/13)
    
    An http address is created and parsed,
    then the values are retrieved either
    by index or by parameter
    
*/

#include <HTML.h>


void setup(){
  Serial.begin(9600);
  
  GET myget;
//  myget.SetPrinter(&Serial);
  
  char http[] = "http://www.test.com/?arduino=1.0.1&board=UNO NotHere";
  
  //use one of the three parsers below
  int qty = myget.ParseAddress(http, " "); //correct values
//  int qty = myget.ParseAddress(http); //returns -4 because of overflow
//  int qty = myget.ParseAddress(http, ". "); //will find only 1 because of the '.' character
  
  Serial.println(http);
  Serial.print(">> quantity: ");
  Serial.println(qty);
  
  Serial.print("* [0] = ");
  Serial.println(myget[0]);
  Serial.print("* [1] = ");
  Serial.println(myget[1]);
  Serial.print("* [3] = ");
  Serial.println(myget[3]);
  Serial.print("* [-1] = ");
  Serial.println(myget[-1]); //returns NULL, therefore can display something strange
  
  Serial.print("* [arduino] = ");
  Serial.println(myget["arduino"]);
  Serial.print("* [board] = ");
  Serial.println(myget["board"]);
  Serial.print("* [test] = ");
  Serial.println(myget["test"]); //returns NULL, therefore can display something strange
  Serial.print("* [ardu] = ");
  Serial.println(myget["ardu"]); //returns NULL, therefore can display something strange
  Serial.print("* [arduinooo] = ");
  Serial.println(myget["arduinooo"]); //returns NULL, therefore can display something strange
  
  Serial.println("\nPointer Addresses:");
  Serial.println((int)myget[-1], HEX);
  Serial.println((int)myget["board"], HEX); //can find something, depens of the delimiters used
  Serial.println((int)myget["test"], HEX);
  Serial.println((int)myget["ardu"], HEX);
  Serial.println((int)myget["arduinooo"], HEX);
  
  Serial.println("\n\tdone!");
}


void loop(){
}




