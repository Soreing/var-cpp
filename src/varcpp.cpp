#include "varcpp.h"

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
		return "[function]";
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