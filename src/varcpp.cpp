#include "varcpp.h"
#include <iostream>
#include <math.h>

var::var() 
	: data(NULL), type(undefined_t)
{
}

var::var(void* data, varType type) 
	: data(data), type(type) 
{
}

var::var(const var &val)
	: data(NULL), type(undefined_t)
{
	this->operator=(val);
}

var& var::operator=(const var &val)
{
	type = val.type;
	vec<var>* elements   = NULL;
	vec<atr>* attributes = NULL;

	switch (type)
	{	case boolean_t:
			data = (void*)new bool(*(bool*)(val.data));
			break;

		case integer_t:
			data = (void*)new int(*(int*)(val.data));
			break;

		case real_t:
			data = new double(*(double*)(val.data));
			break;

		case text_t:
			data = (void*)new str(*(str*)(val.data));
			break;

		case array_t:
			elements = (vec<var>*)val.data;
			data = (void*)new vec<var>(elements->size());

			for (int i = 0; i < (int)elements->size(); i++)
			{	(*(vec<var>*)data)[i] = (*elements)[i];
			}

			break;

		case object_t:
			attributes = (vec<atr>*)val.data;
			data = (void*)new vec<atr>(attributes->size());

			for (int i = 0; i < (int)attributes->size(); i++)
			{	(*(vec<atr>*)data)[i] = (*attributes)[i];
			}

			break;
			
		case function_t:
			Function* fnc = (Function*)val.data;
			data = (void*) new Function{fnc->params, fnc->fptr};
			break;
	}
	return *this;
}

var& var::operator=(int val)
{
	if (data != NULL)
	{	clean();
	}

	data = (void*)new int(val);	
	type = integer_t; 
	return *this; 
}

var& var::operator=(short val) 
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new int(val);	
	type = integer_t; 
	return *this; 
}

var& var::operator=(long val) 
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new int(val);	
	type = integer_t; 
	return *this; 
}

var& var::operator=(bool val) 
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new bool(val);	
	type = boolean_t; 
	return *this; 
}

var& var::operator=(double val)
{
	if (data != NULL)
	{	clean();
	}

	data = (void*)new double(val); 
	type = real_t; 
	return *this; 
}

var& var::operator=(float val) 
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new double(val); 
	type = real_t; 
	return *this; 
}

var& var::operator=(str val)
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new str(val);	
	type = text_t; 
	return *this; 
}

var& var::operator=(cstr val) 
{ 
	if (data != NULL)
	{	clean();
	}

	data = (void*)new str(str(val)); 
	type = text_t; 
	return *this; 
}

var& var::operator=(const array &val)
{
	if (data != NULL)
	{	clean();
	}

	data = (void*)new vec<var>(val.size());
	for (int i = 0; i < (int)val.size(); i++)
	{	(*(vec<var>*)data)[i] = val[i];
	}

	type = array_t;
	return *this;
}

var& var::operator=(Object val)
{
	if (data != NULL)
	{	clean();
	}

	data = (void*)new vec<atr>();
	type = object_t;
	return *this;
}

var& var::operator=(var(*val)())
{
	if (data != NULL)
	{	clean();
	}

	data = (void*)new Function{0, val};
	type = function_t;

	return *this;
}

var& var::operator[](str name)
{
	if (type != object_t)
	{	throw "Error";
	}

	str tag = name;
	vec<atr>* attr = (vec<atr>*)data;

	// Find the attribute in the object by name if it exists
	for (int i = 0; i < (int)attr->size(); i++)
	{	if (tag == attr->at(i).name)
		{	return attr->at(i).val;
		}
	}

	// Add a new attribute to the object if it's not found and return it
	attr->push_back(atr{tag, var()});
	return attr->at(attr->size() - 1).val;
}

var& var::operator[](size_t idx)
{
	if (type != array_t)
	{	throw "Error";
	}

	vec<var>* arr = (vec<var>*)data;

	// If the array is too small, resize it.
	if (idx >= arr->size())
	{	arr->resize(idx+1);
	}

	return (*arr)[idx];
}

var var::operator()()
{
	Function* fnc = (Function*) data;
	if (type != function_t && fnc->params != 0) 
	{	return var();
	}

	return ((var(*)())data)();
}

var var::operator()(var p1) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t && fnc->params != 1) 
	{	return var();
	}

	return ((var(*)(var)) (fnc->fptr) )(p1);
}

var var::operator()(var p1, var p2) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 2) 
	{	return var();
	}

	return ((var(*)(var, var)) (fnc->fptr) )
		(p1, p2);
}

var var::operator()(var p1, var p2, var p3) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 3) 
	{	return var();
	}

	return ((var(*)(var, var, var)) (fnc->fptr) )
		(p1, p2, p3);
}

var var::operator()(var p1, var p2, var p3, var p4) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 4) 
	{	return var();
	}

	return ((var(*)(var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 5) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 6) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 7) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6, p7);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7, var p8) 
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 8) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6, p7, p8);
}

var var::operator+(var right)
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = getNum(*this) + getNum(right);
		varType rtype = type > right.type ? type : right.type;

		switch(rtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}
	else if(type == text_t)
	{	result = *(str*)data + right.toString();
	}
	else if(right.type == text_t)
	{	result = toString() + *(str*)right.data;
	}
	else if(type == array_t && right.type == array_t)
	{	mergeArrays(*this, right, result);
	}
	else if(type == object_t && right.type == object_t)
	{	mergeObjects(*this, right, result);
	}

	return result;
}

var var::operator-(var right)
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = getNum(*this) - getNum(right);
		varType rtype = type > right.type ? type : right.type;

		switch(rtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

var var::operator*(var right)
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = getNum(*this) * getNum(right);
		varType rtype = type > right.type ? type : right.type;

		switch(rtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

var var::operator/(var right)
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		if(getNum(right)==0)
		{	return var();
		}

		double value = getNum(*this) / getNum(right);
		varType rtype = type > right.type ? type : right.type;

		switch(rtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

var var::operator%(var right)
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double lvalue = getNum(*this);
		double rvalue = getNum(right);
		varType rtype = type > right.type ? type : right.type;

		if(getNum(right)==0)
		{	return var();
		}

		switch(rtype)
		{	case boolean_t: result = lvalue == 1 ? false : true; break;
			case integer_t: result = (int)lvalue % (int)rvalue; break;
			case real_t:    result = fmod(lvalue, rvalue); break;
		}
	}

	return result;
}

bool var::operator&&(var right)
{
	return (bool)*this && (bool)right;
}

bool var::operator||(var right)
{
	return (bool)*this || (bool)right;
}

int var::compare(const var &right)
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
		case real_t:   lVal = getNum(*this); break;
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
		case real_t:   rVal = getNum(right); break;
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

bool var::operator==(const var &right)
{	
	return compare(right) == 1;
}

bool var::operator<(const var &right)
{
	return compare(right) == 0;
}

bool var::operator>(const var &right)
{	
	return compare(right) == 2;
}

bool var::operator!=(const var &right)
{	
	return compare(right) != 1;
}

bool var::operator<=(const var &right)
{
	int res = compare(right);
	return res == 0 || res == 1;
}

bool var::operator>=(const var &right)
{	
	int res = compare(right);
	return res == 2 || res == 1;
}

var::operator bool()
{
	switch(type)
	{	case boolean_t:   return *(bool*)data;
		case integer_t:   return *(int*)data != 0 ? true: false;
		case real_t:      return *(double*)data != 0 ? true: false;
		case text_t:      return data != NULL ? true : false;
		case array_t:     return data != NULL ? true : false;
		case object_t:    return data != NULL ? true : false;
		case function_t:  return data != NULL ? true : false;
		default:          return 0;
	}
}

var::operator int()
{
	if(type == text_t)
	{	size_t cpos = 0;
		str* txt = (str*)data;
		int num = stoi(*txt, &cpos);
		return cpos == txt->length() && cpos > 0 ? num : 0; 
	}

	switch(type)
	{	case boolean_t: return *(bool*)data ? 1 : 0;
		case integer_t: return *(int*)data;
		case real_t:    return (int)*(double*)data;
		default:        return 0; 
	}
}

var::operator double()
{
	if(type == text_t)
	{	size_t cpos = 0;
		str* txt = (str*)data;
		double num = stod(*txt, &cpos);
		return cpos == txt->length() && cpos > 0 ? num : 0; 
	}

	switch(type)
	{	case boolean_t: return *(bool*)data ? 1 : 0;
		case integer_t: return *(int*)data;
		case real_t:    return *(double*)data;
		default:        return 0; 
	}
}

// Convert the value of the var to the appropriate string based on type
// Objects and arrays print all elements
str var::toString() const
{
	if (type == boolean_t)
	{	return (*(bool*)data ? "true" : "false");
	}
	else if (type == integer_t)
	{	return std::to_string(*(int*)data);
	}
	else if (type == real_t)
	{	return std::to_string(*(double*)data);
	}
	else if (type == text_t)
	{	return *(str*)data;
	}
	else if (type == array_t)
	{
		// Get a reference to the vector
		vec<var>* elem = (vec<var>*)data;
		if (elem->size() == 0)
		{	return "{ }";
		}

		// Set the string to the first element and 
		// add every other element with a comma
		str res = elem->at(0).toString();
		for (int i = 1; i < (int)elem->size(); i++)
		{	res += ", " + elem->at(i).toString();
		}

		// Format the output to be between braces
		// and return the result
		res = "{ " + res + " }";
		return res;
	}
	else if (type == object_t)
	{
		// Get a reference to the vector
		vec<atr>* attr = (vec<atr>*)data;
		if (attr->size() == 0)
		{	return "{ }";
		}

		// Set the string to the first element and 
		// add every other element with a comma, including the name
		str res = attr->at(0).name + ":" + attr->at(0).val.toString();
		for (int i = 1; i < (int)attr->size(); i++)
		{	res += ", " + attr->at(i).name + ":" + attr->at(i).val.toString();
		}

		// Format the output to be between braces
		// and return the result
		res = "{ " + res + " }";
		return res;
	}
	else if (type == function_t)
	{	// Generic function statement
		std::string count = std::to_string(((Function*)data)->params);
		return "[function] (" + count + " Args)";
	}

	return "undefined";
}

// Load the value of the var to the ostream 
std::ostream& operator<<(std::ostream& o, const var& v)
{
	o << v.toString();
	return o;
}

// Clean memory when vars are deleted
void var::clean()
{
	if (data != NULL)
	{
		switch (type)
		{	case boolean_t:
				delete (bool*)data;
				break;
			case integer_t: 
				delete (int*)data; 
				break;
			case real_t: 
				delete (double*)data; 
				break;
			case text_t: 
				delete (str*)data; 
				break;
			case array_t: 
				delete (array*)data; 
				break;
			case object_t: 
				delete (vec<atr>*)data; 
				break;
			case function_t: 
				delete (Function*)data; 
				break;
		}

		data = NULL;
		type = undefined_t;
	}
}

var::~var()
{
	if (data != NULL)
	{	clean();
	}
}

double getNum(const var& num)
{
	switch(num.type)
	{
		case boolean_t: return *(bool*)num.data ? 1 : 0;
		case integer_t: return *(int*)num.data;
		case real_t:    return *(double*)num.data;
		default:        return 0;
	}
}

void mergeObjects(var &left, var &right, var &res)
{
	res = Object();
	vec<atr>* lAtr = (vec<atr>*)left.data;
	vec<atr>* rAtr = (vec<atr>*)right.data;
	vec<atr>* aAtr = (vec<atr>*)res.data;
	
	size_t size = lAtr->size() + rAtr->size();
	aAtr->resize(size);
	
	size_t idx = 0;
	for(; idx < lAtr->size(); idx++)
	{	(*aAtr)[idx] = (*lAtr)[idx];
	}

	for(size_t i=0; i < rAtr->size(); i++, idx++)
	{	(*aAtr)[idx] = (*rAtr)[i];
	}

	return;
}

void mergeArrays(var &left, var &right, var &res)
{
	res = array{};
	vec<var>* lArr = (vec<var>*)left.data;
	vec<var>* rArr = (vec<var>*)right.data;
	vec<var>* aArr = (vec<var>*)res.data;
	
	size_t size = lArr->size() + rArr->size();
	aArr->resize(size);
	
	size_t idx = 0;
	for(; idx < lArr->size(); idx++)
	{	(*aArr)[idx] = (*lArr)[idx];
	}

	for(size_t i=0; i < rArr->size(); i++, idx++)
	{	(*aArr)[idx] = (*rArr)[i];
	}

	return;
}