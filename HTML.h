#ifndef RC_HTML
#define RC_HTML

/*
	RoboCore HTML Library
		(v1.0 - 08/03/2013)

  Library to manipulate HTML
    (tested with Arduino 1.0.1)

  Copyright 2013 RoboCore (François) ( http://www.RoboCore.net )
  
  ------------------------------------------------------------------------------
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  ------------------------------------------------------------------------------

*/


#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h> //for Arduino 1.0 or later
#else
#include <WProgram.h> //for Arduino 22
#endif

#define RC_HTML_DEBUG

//-----------------------------------------------------------------------------------

#define GET_PARAMETER_QTY 10
#define GET_PARAMETER_SIZE 10
#define GET_VALUE_SIZE 10

class GET{
  private:
    char _parameters[GET_PARAMETER_QTY][GET_PARAMETER_SIZE];
    char _values[GET_PARAMETER_QTY][GET_VALUE_SIZE];
    char _buffer[GET_VALUE_SIZE + 1];
    HardwareSerial* _printer;

    void PrintParameters(void);
    void PrintValues(void);
  
  public:
    GET(void);
    ~GET(void);
    int ParseAddress(const char* http_address);
    int ParseAddress(const char* http_address, char* delimiter);
    void SetPrinter(HardwareSerial* printer);
    char* operator[](const int index);
    char* operator[](const char* parameter);

};


#endif




