#include "varcpp.h"

// Simple logical AND operator that explicitly converts vars to booleans
bool var::operator&&(const var &right) const
{
	return (bool)*this && (bool)right;
}

// Simple logical OR operator that explicitly converts vars to booleans
bool var::operator||(const var &right) const
{
	return (bool)*this || (bool)right;
}

// Compares two vars of the same or different types
// 1) If the values are or convert to numbers, they are compared as numbers
// 2) If the values are or convert to strings, they are compared as strings
// 3) Functions can be compared to be equal, otherwise neither less or greater
// 4) Undefined types result in only inequality
// Return values: 0(less), 1(equal), 2(greater), -1(none)
int var::compare(const var &right) const
{
	size_t lStop = 0, rStop = 0;
	double lVal = 0, rVal = 0;
	str lTxt, rTxt;

	if(type == text_t)
	{	lTxt = *(str*)data;
		if( lTxt.length() > 0 && isdigit(lTxt[0]) )
		{	lVal = stod(lTxt, &lStop);
		}
	}

	switch(type)
	{	case boolean_t:
		case integer_t:
		case real_t:   lVal = (double)*this; break;
		case array_t:  lTxt = toString(); break;
		case object_t: lTxt = toString(); break;
	}

	if(right.type == text_t)
	{	rTxt = *(str*)right.data;
		if( rTxt.length() > 0 && isdigit(rTxt[0]) )
		{	rVal = stod(rTxt, &rStop);
		}
	}

	switch(right.type)
	{	case boolean_t:
		case integer_t:
		case real_t:   rVal = (double)right; break;
		case array_t:  rTxt = right.toString(); break;
		case object_t: rTxt = right.toString(); break;
	}

	varType minType = type < right.type ? type : right.type;
	varType maxType = type > right.type ? type : right.type;
	
	if(type == function_t && right.type == function_t)
	{	return ((Function*)data)->fptr == ((Function*)right.data)->fptr ? 1 : -1;
	}
	if(maxType >= function_t)
	{	return -1;
	}
	else if(minType >= text_t)
	{	return lTxt < rTxt ? 0 : lTxt > rTxt ? 2 : 1;
	}
	else if(type <= real_t && right.type <= real_t)
	{	return lVal < rVal ? 0 : lVal > rVal ? 2 : 1;
	}
	else if(type <= real_t)
	{	if(rStop == rTxt.length())
		{	return lVal < rVal ? 0 : lVal > rVal ? 2 : 1;
		}
	}
	else if(right.type <= real_t)
	{	if(lStop == lTxt.length())
		{	return lVal < rVal ? 0 : lVal > rVal ? 2 : 1;
		}
	}

	return -1; 
}

// Returns whether the two operands are equal
// Values equal if the compare function returns 1
bool var::operator==(const var &right) const
{	
	return compare(right) == 1;
}

// Returns whether the left hand operand is less than the right
// The value is less than if the compare function returns 0
bool var::operator<(const var &right) const
{
	return compare(right) == 0;
}

// Returns whether the left hand operand is greater than the right
// The value is less than if the compare function returns 2
bool var::operator>(const var &right) const
{	
	return compare(right) == 2;
}

// Returns whether the two operands are not equal
// The values are not equal if the compare function return anything but 1
bool var::operator!=(const var &right) const
{	
	return compare(right) != 1;
}

// Returns whether the left hand operand is less than or equal the right
// The value is less than if the compare function returns 0 or 1
bool var::operator<=(const var &right) const
{
	int res = compare(right);
	return res == 0 || res == 1;
}

// Returns whether the left hand operand is greater than or equal the right
// The value is less than if the compare function returns 2 or 1
bool var::operator>=(const var &right) const
{	
	int res = compare(right);
	return res == 2 || res == 1;
}

// Compares the internal type of the var to a given type, returns true if it matches
bool var::operator==(const varType t) const
{
	return type == t;
}

// Compares the internal type of the var to a given type, returns true if it matches
bool operator==(const varType t, const var& val)
{
	return val.getType() == t;
}