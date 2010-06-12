#include "stringCat.h"

#include <sstream>
using namespace std;

string stringCat( string mess, int num ){
	stringstream res;
	
	res << mess << num ;
	
	return res.str();
}

string stringCat( string mess, string autre ){
	string res = mess ;
	return res.append( autre );
}
