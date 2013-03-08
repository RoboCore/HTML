
/*
	RoboCore HTML Library
		(v1.0 - 08/03/2013)

  Library to manipulate HTML
    (tested with Arduino 1.0.1)

  Released under the Beerware licence

*/


#include "HTML.h"
#include <string.h>



//-----------------------------------------------------------------------------------

// Constructor
GET::GET(void){
  _printer = NULL;
}

//-----------------------------------------------------------------------------------

// Destructor
GET::~GET(void){
  /* do nothing */
}

//-----------------------------------------------------------------------------------

// Parse the HTTP address
//  (returns the number of parameters parsed, -1 if given address is invalid,
//    -2 if too much parameters, -3 if overflow of parameter string and
//    -4 if overflow of value string)
int GET::ParseAddress(const char* http_address){
  return ParseAddress(http_address, NULL);
}


// Parse the HTTP address
//  (returns the number of parameters parsed, -1 if given address is invalid,
//    -2 if too much parameters, -3 if overflow of parameter string and
//    -4 if overflow of value string)
int GET::ParseAddress(const char* http_address, char* delimiters){
  //check if valid string
  int http_length = strlen(http_address);
  if(http_length <= 0)
    return -1;
  
  //reset arrays
  for(int i=0 ; i < GET_PARAMETER_QTY ; i++){
    for(int j=0 ; j < GET_PARAMETER_SIZE ; j++)
      _parameters[i][j] = NULL;
    for(int j=0 ; j < GET_VALUE_SIZE ; j++)
      _values[i][j] = NULL;
  }
  
  //check if valid delimiters
  int delimiters_length = strlen(delimiters);
  boolean use_delimiters = false;
  if(delimiters_length > 0)
    use_delimiters = true;
  
  boolean found_parameters = false; //TRUE when start of parameters
  boolean is_value = false; //TRUE when data is a value, FALSE when is a parameter
  boolean skip = false; //TRUE when has to skip a control character
  boolean eos = false; //TRUE if End Of String (when using delimiters)
  byte str_size = 0; //counter to store in the parameters/values array
  int index = 0; //number of parameters found
  for(int i=0 ; i < http_length; i++){
    //exit for if found carriage return
    if((http_address[i] == '\n') || (http_address[i] == '\r'))
      break;
    
    // --- check for control characters ---
    
    //check if '?' character
    if(http_address[i] == '?'){
      found_parameters = true; //set
      skip = true; //set
    }
    
    //check if '=' character
    if(http_address[i] == '='){
      str_size = 0; //reset
      is_value = true; //set
      skip = true; //set
    }
    
    //check if '&' character
    if(http_address[i] == '&'){
      str_size = 0; //reset
      index++; //new parameter found
      is_value = false; //reset
      skip = true; //set
    }
    
    
    //check if valid number of parameters
    if(index >= GET_PARAMETER_QTY){
      index = -3; // (-2)
      break;
    }
    
    // --- end of check ---
    
    //store data
    if(found_parameters && !skip){
      //break on delimiters
      if(use_delimiters){ //check whether to search or not
        for(int j=0 ; j < delimiters_length ; j++){
          if(http_address[i] == delimiters[j]){
            eos = true; //reached end of string
            break;
          }
        }
      }
      
      if(!eos){ //continue if not EOS
        if(is_value){ //add value
          if(str_size < GET_VALUE_SIZE){
            _values[index][str_size] = http_address[i]; //store
            str_size++; //increase
          } else {
            index = -5; // (-4)
            break;
          }
        } else { //add parameter
          if(str_size < GET_PARAMETER_SIZE){
            _parameters[index][str_size] = http_address[i]; //store
            str_size++; //increase
          } else {
            index = -4; // (-3)
            break;
          }
        }
      }
    }
    
    //update skip
    if(skip)
      skip = false;
    
    
    //exit on End Of String
    if(eos)
      break;
  }
  
  // only for debug
  PrintParameters();
  PrintValues();
  // --------------
  
  return (index + 1);
}

//-----------------------------------------------------------------------------------

// Print the parameters
void GET::PrintParameters(void){
  if(_printer == NULL)
    return;
  
#ifdef RC_HTML_DEBUG
  _printer->println("# Parameters");
  for(int i=0 ; i < GET_PARAMETER_QTY ; i++){
    _printer->print('\t');
    _printer->print(i);
    _printer->print(':');
    for(int j=0 ; j < GET_PARAMETER_SIZE ; j++)
      _printer->print(_parameters[i][j]);
    _printer->println();
  }
#endif
}

//-----------------------------------------------------------------------------------

// Print the values
void GET::PrintValues(void){
  if(_printer == NULL)
    return;
  
#ifdef RC_HTML_DEBUG
  _printer->println("# Values");
  for(int i=0 ; i < GET_PARAMETER_QTY ; i++){
    _printer->print('\t');
    _printer->print(i);
    _printer->print(':');
    for(int j=0 ; j < GET_VALUE_SIZE ; j++)
      _printer->print(_values[i][j]);
    _printer->println();
  }
#endif
}

//-----------------------------------------------------------------------------------

// Set the printer of the class
void GET::SetPrinter(HardwareSerial* printer){
#ifdef RC_HTML_DEBUG
  _printer = printer;
#endif
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

// Operator [] to search by index
//  (returns NULL if index out of range)
char* GET::operator[](const int index){
  //check if valid index
  if((index < 0) || (index >= GET_PARAMETER_QTY))
    return NULL;
  
  _buffer[GET_VALUE_SIZE] = '\0'; //null terminated string
  for(int i=0 ; i < GET_VALUE_SIZE ; i++){
    _buffer[i] = _values[index][i];
    if(_buffer[i] == NULL)
      break;
  }
  return _buffer;
}

//-----------------------------------------------------------------------------------

// Operator [] to search by parameter
//  (returns NULL if invalid parameter or not found)
char* GET::operator[](const char* parameter){
  //check if valid size
  int length = strlen(parameter);
  if(length > GET_PARAMETER_SIZE)
    return NULL;
  
  boolean found = false;
  int index;
  for(int i=0 ; i < GET_PARAMETER_QTY ; i++){
    for(int j=0 ; j < length ; j++){
      //if found different character goto next index
      if(parameter[j] != _parameters[i][j]){
        break;
      } else if(j == (length - 1)){ //last character
        //check for exact match
        if(length == GET_PARAMETER_SIZE){ //fills the array
          found = true;
          index = i;
        } else if(_parameters[i][j+1] == NULL){ //next character is null
          found = true;
          index = i;
        }
      }
    }
    
    //check if found
    if(found)
      break;
  }
  
  //return
  if(!found)
    return NULL;
  else {
    _buffer[GET_VALUE_SIZE] = '\0'; //null terminated string
    for(int i=0 ; i < GET_VALUE_SIZE ; i++){
      _buffer[i] = _values[index][i];
    if(_buffer[i] == NULL)
      break;
    }
    return _buffer;
  }
}

//-----------------------------------------------------------------------------------















