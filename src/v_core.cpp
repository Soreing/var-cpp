#include "varcpp.h"
#include <iostream>
#include <math.h>

#define ASSERT_ARRAY    if(type!=array_t) {throw var::TypeMismatchException();}
#define ASSERT_OBJECT   if(type!=object_t) {throw var::TypeMismatchException();}

#define CELAN_PREVIOUS_ALLOCATION if(data != NULL) {clean();}


void* var::getData() const
{	return data;
}

varType var::getType() const
{	return type;
}

// Default Constructor (Undefined Type)
var::var() 
	: data(NULL), type(undefined_t)
{
}

// Parameterized Constructor
var::var(void* data, varType type) 
	: data(data), type(type) 
{
}

// Copy Constructor (Delegates to Assignment Operator)
var::var(const var &val)
	: data(NULL), type(undefined_t)
{
	this->operator=(val);
}

// Assignment Operator copying from the same type
// Allocates a new pointer and copies over the data
var& var::operator=(const var &val)
{
	type = val.type;
	array* elements   = NULL;
	object* attributes = NULL;

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
			elements = (array*)val.data;
			data = (void*)new array(elements->size());

			for (size_t i = 0; i < elements->size(); i++)
			{	(*(array*)data)[i] = (*elements)[i];
			}

			break;

		case object_t:
			attributes = (object*)val.data;
			data = (void*)new object(attributes->size());

			for (size_t i = 0; i < attributes->size(); i++)
			{	(*(object*)data)[i] = (*attributes)[i];
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
	CELAN_PREVIOUS_ALLOCATION
	type = integer_t;
	data = (void*)new int(val);	
	return *this; 
}

var& var::operator=(short val) 
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = integer_t;
	data = (void*)new int(val);	
	return *this; 
}

var& var::operator=(long val) 
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = integer_t; 
	data = (void*)new int(val);	
	return *this; 
}

var& var::operator=(bool val) 
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = boolean_t; 
	data = (void*)new bool(val);	
	return *this; 
}

var& var::operator=(double val)
{
	CELAN_PREVIOUS_ALLOCATION
	type = real_t;
	data = (void*)new double(val); 
	return *this; 
}

var& var::operator=(float val) 
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = real_t;
	data = (void*)new double(val); 
	return *this; 
}

var& var::operator=(str val)
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = text_t; 
	data = (void*)new str(val);	
	return *this; 
}

var& var::operator=(cstr val) 
{ 
	CELAN_PREVIOUS_ALLOCATION
	type = text_t; 
	data = (void*)new str(str(val)); 
	return *this; 
}

var& var::operator=(const array &val)
{
	CELAN_PREVIOUS_ALLOCATION
	type = array_t;
	data = (void*)new array(val.size());

	for (size_t i = 0; i < val.size(); i++)
	{	(*(array*)data)[i] = val[i];
	}

	return *this;
}

var& var::operator=(const object &val)
{
	CELAN_PREVIOUS_ALLOCATION
	type = object_t;
	data = (void*)new object(val.size());

	for (size_t i = 0; i < val.size(); i++)
	{	(*(object*)data)[i] = val[i];
	}

	return *this;
}

var& var::operator=(var(*val)())
{
	CELAN_PREVIOUS_ALLOCATION
	type = function_t;
	data = (void*)new Function{0, val};
	return *this;
}

// Finds an attribute in an object by name and returns it
// If no attribute is found, a new one is created and returned
const var& var::operator[](str name) const
{
	ASSERT_OBJECT
	object* attr = (object*)data;

	for (size_t i = 0; i < attr->size(); i++)
	{	if (name == attr->at(i).name)
		{	return attr->at(i).val;
		}
	}

	attr->push_back(atr{name, var()});
	return attr->at(attr->size() - 1).val;
}

var& var::operator[](str name)
{
	ASSERT_OBJECT
	object* attr = (object*)data;

	for (size_t i = 0; i < attr->size(); i++)
	{	if (name == attr->at(i).name)
		{	return attr->at(i).val;
		}
	}

	attr->push_back(atr{name, var()});
	return attr->at(attr->size() - 1).val;
}

// Finds an element in an array by index and returns it
// If the array is too small, resize it.
const var& var::operator[](size_t idx) const
{
	ASSERT_ARRAY
	array* arr = (array*)data;

	if (idx >= arr->size())
	{	arr->resize(idx+1);
	}

	return (*arr)[idx];
}

var& var::operator[](size_t idx)
{
	ASSERT_ARRAY
	array* arr = (array*)data;

	if (idx >= arr->size())
	{	arr->resize(idx+1);
	}

	return (*arr)[idx];
}

// Calls a function
var var::operator()() const
{
	Function* fnc = (Function*) data;
	if (type != function_t && fnc->params != 0) 
	{	return var();
	}

	return ((var(*)())data)();
}

var var::operator()(var p1) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t && fnc->params != 1) 
	{	return var();
	}

	return ((var(*)(var)) (fnc->fptr) )(p1);
}

var var::operator()(var p1, var p2) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 2) 
	{	return var();
	}

	return ((var(*)(var, var)) (fnc->fptr) )
		(p1, p2);
}

var var::operator()(var p1, var p2, var p3) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 3) 
	{	return var();
	}

	return ((var(*)(var, var, var)) (fnc->fptr) )
		(p1, p2, p3);
}

var var::operator()(var p1, var p2, var p3, var p4) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 4) 
	{	return var();
	}

	return ((var(*)(var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 5) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 6) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 7) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6, p7);
}

var var::operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7, var p8) const
{ 
	Function* fnc = (Function*) data;
	if (type != function_t || fnc->params != 8) 
	{	return var();
	}

	return ((var(*)(var, var, var, var, var, var, var, var)) (fnc->fptr) )
		(p1, p2, p3, p4, p5, p6, p7, p8);
}

// Converts the value to a primitive bool
// Non zero numeric types and all other types but undefined are true
var::operator bool() const
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

// Converts the var to a primitive int type
// Strings are converted to numbers if the format is correct in decimal
// Numeric types (bool, int, real) are taken by their value
// Other types resolve to 0 
var::operator int() const
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

// Converts the var to a primitive double type
// Strings are converted to numbers if the format is correct in decimal
// Numeric types (bool, int, real) are taken by their value
// Other types resolve to 0 
var::operator double() const
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
		array* elem = (array*)data;
		if (elem->size() == 0)
		{	return "[ ]";
		}

		// Set the string to the first element and 
		// add every other element with a comma
		str res = elem->at(0).toString();
		for (int i = 1; i < (int)elem->size(); i++)
		{	res += ", " + elem->at(i).toString();
		}

		// Format the output to be between braces
		// and return the result
		res = "[ " + res + " ]";
		return res;
	}
	else if (type == object_t)
	{
		object* attr = (object*)data;
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
				delete (object*)data; 
				break;
			case function_t: 
				delete (Function*)data; 
				break;
		}

		data = NULL;
		type = undefined_t;
	}
}

// Destructor
var::~var()
{
	CELAN_PREVIOUS_ALLOCATION
}
