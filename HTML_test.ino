
/*
    Test for the HTML library
            (08/03/13)
    
    An http address is created and parsed,
    then the values are retrieved either
    by index or by parameter
    
*/

#include "HTML.h"


void setup(){
  Serial.begin(9600);
  
  GET myget;
//  myget.SetPrinter(&Serial);
  
  char http[] = "http://www.test.com/?arduino=1.0.1&board=UNO";
  Serial.println(http);
  
  int qty = myget.ParseAddress(http);
  Serial.print(">> quantity: ");
  Serial.println(qty);
  
  Serial.print("* [0] = ");
  Serial.println(myget[0]);
  Serial.print("* [1] = ");
  Serial.println(myget[1]);
  Serial.print("* [3] = ");
  Serial.println(myget[3]);
  
  Serial.print("* [arduino] = ");
  Serial.println(myget["arduino"]);
  Serial.print("* [board] = ");
  Serial.println(myget["board"]);
  Serial.print("* [test] = ");
  Serial.println(myget["test"]);
  
  Serial.println("\n\tdone!");
}


void loop(){
}




