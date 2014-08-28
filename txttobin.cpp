#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <exception>
using namespace std;

bool gDebug = false;

string * parseProcessString( string ascii_str, string * pbin_out);
void my_text_to_hex(string input_ascii, string *pbin_out);
int convertHexDigit(char single_char);

class myexception: public exception
{
  virtual const char* what() const throw()
  {
    return "Invalid ASCII Hex Character - Letters must be from A to F only!";
  }
} myex;

int main (void)
{
  int i;
  FILE *fp;
  string ascii_in;
  string bin_out = "";
  string * pbin_out;
  // char bin_out[1000];
  // char * pbin_out;

  
  // Note:  Docs say that cin will stop inputing at first blank char, so need to be sure no blank chars.
  cin >> ascii_in;
  if (gDebug) {
      cerr  << "After input, Input length: ";
      cerr << ascii_in.length() << endl;
  }

  parseProcessString(ascii_in, &bin_out);
  // In order to get binary data out. March 20, 2012.
  // cout << bin_out;
  fp = freopen(NULL, "wb", stdout);  // Reopen stdout as binary output.
  // ** Note that this is only sending about 4 characters. FIXED by using the length() method !!
  fwrite(bin_out.c_str(), 1, bin_out.length(), fp);
  // fwrite(bin_out, 1, sizeof(bin_out), fp);
  // fwrite(bin_out.c_str(), sizeof(bin_out.c_str()), 1, stdout);
  fclose(fp);
  
  return 0;
}

string * parseProcessString( string ascii_str, string * pbin_out)
// string * parseProcessString( string ascii_str, char * pbin_out)
{
    string::iterator it;
    bool second_char = false;
    string twochars;
    
    for ( it = ascii_str.begin() ; it < ascii_str.end() ; it++)
    {
	if (second_char){
	    second_char = false;
	    twochars += *it;
	    my_text_to_hex(twochars, pbin_out);
	    twochars.erase();
	}
	else
	{
	    twochars = *it;
	    second_char = true;
	    
	}
	
    }
    if ( second_char ) cerr << "Error: Odd number of characters!" << endl;
    
    return pbin_out;
    
}

void my_text_to_hex(string input_ascii, string *pbin_out)
// void my_text_to_hex(string input_ascii, char *pbin_out)
{
    char single_char = 'B';
    const char * input;
    int input_int, accum;
    char buffer[100];

    string err_msg = "\n Error:  Invalid ASCII Hex Character: ";

    if ( input_ascii.length() > 2 ) {
	cerr << "\nError:  More than two ascii digits!\n" << endl;
	return;
    }
    
    // if (gDebug) printf("In my_text_to_hex, input string: %s \n",input_ascii.c_str());
    input_ascii.copy(&single_char,1,1);
    input_int = convertHexDigit(single_char);
    accum = input_int;
    
    input_ascii.copy(&single_char,1,0);
    input_int = convertHexDigit(single_char);
    accum += input_int*16;  // This results in the decimal version of the correct hex result.

    if (gDebug) {
	sprintf(buffer, "\n In my_text_to_hex, accum: %d\n",accum);
	cerr << buffer << endl;
    }
    
    // memset(ls_hex, '\0', 3);
    // sprintf(ls_hex, "%.2X", input_ascii.c_str());
    // input = input_ascii.c_str();
    // sprintf(ls_hex, "%1.1X", input_int);

    (*pbin_out) += accum;
    // (*pbin_out).append((char) accum);
    return;
}

int convertHexDigit(char single_char) 
{
    char ls_hex[3];
    int input_int, ils_hex, output_int;
    string err_msg = "\n Error:  Invalid ASCII Hex Character: ";
    char buffer[100];

    sprintf(ls_hex, "%X",(int)single_char);
    ils_hex = atoi(ls_hex);
    if (gDebug) {
	// printf("In convertHexDigit, input integer: %d \n",ils_hex);
	sprintf(buffer, "In convertHexDigit, input integer: %d \n",ils_hex);
	cerr << buffer << endl;
    }

    // This solves the wrong problem.

    input_int = ils_hex;

    if ( input_int < 30 ) {
	cerr << err_msg << input_int << endl;
	throw myex;
	return 0;  // bad value
    } else {
	if (input_int > 39 ) {
	    if (input_int < 41 ) {
		cerr << err_msg << input_int << endl;
		throw myex;
		return 0;  // bad value
	    } else {
		if (input_int > 46 ) {
		    if (input_int < 61 ) { 
			cerr << err_msg << input_int << endl;
			throw myex;
			return 0;  // bad value
		    } else if ( input_int < 67 ) {  
			input_int = input_int - 20;  // Convert lower case to upper case hex.
			output_int = input_int - 31;
		    } else {
			cerr << err_msg << input_int << endl;
			throw myex;
			return 0; // bad value
		    }
		    
		} else {  // Hex value is between 41 and 46 inclusive.
		    output_int = input_int - 31;
		    
		}
		
	    }
	} else {  // Hex value is between 31 and 39 inclusive.
	    output_int = input_int - 30;
	    
	}
	
    }

    // output_int = input_int;  // Shortcut to bypass commented out code.
    if (gDebug) { 
	sprintf(buffer, "In convertHexDigit, output string after validation: %d \n",output_int); 
	cerr << buffer << endl;
    }

    return output_int;

}
