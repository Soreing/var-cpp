#include "varcpp.h"

// Create a new object with a list of attributes combined from 2 objects
void addObjects(const var &left, const var &right, var &res)
{
	res = Object();
	vec<atr>* lAtr = (vec<atr>*)left.getData();
	vec<atr>* rAtr = (vec<atr>*)right.getData();
	vec<atr>* aAtr = (vec<atr>*)res.getData();
	
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

// Create a new array with elements combined from 2 arrays
void addArrays(const var &left, const var &right, var &res)
{
	res = array{};
	array* lArr = (array*)left.getData();
	array* rArr = (array*)right.getData();
	array* aArr = (array*)res.getData();
	
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

// Copies and Adds the attributes from one object to another
void mergeObjects(var &left, const var &right)
{
	vec<atr>* lAtr = (vec<atr>*)left.getData();
	vec<atr>* rAtr = (vec<atr>*)right.getData();
	
	size_t idx = lAtr->size();
	size_t size = lAtr->size() + rAtr->size();
	lAtr->resize(size);
	
	for(size_t i=0; i < rAtr->size(); i++, idx++)
	{	(*lAtr)[idx] = (*rAtr)[i];
	}

	return;
}

// Copies and Adds the elements from one array to another
void mergeArrays(var &left, const var &right)
{
	vec<var>* lArr = (vec<var>*)left.getData();
	vec<var>* rArr = (vec<var>*)right.getData();
	
	size_t idx = lArr->size();
	size_t size = lArr->size() + rArr->size();
	lArr->resize(size);

	for(size_t i=0; i < rArr->size(); i++, idx++)
	{	(*lArr)[idx] = (*rArr)[i];
	}

	return;
}

// Adds the value of the same or different type vars and returns the result
// Numerical types (boolean_t, integer_t, real_t) are summed up and the result
// is up casted to the more complex type.
// When either operand is a text_t, the operands are concatenated as strings.
// Array_t and Object_t types combine elements and attributes if both operands
// are of the same type.
// Any other combination results in undefined_t
var var::operator+(const var &right) const
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this + (double)right;
		varType maxtype = type > right.type ? type : right.type;

		switch(maxtype)
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
	{	addArrays(*this, right, result);
	}
	else if(type == object_t && right.type == object_t)
	{	addObjects(*this, right, result);
	}

	return result;
}

// Subtracts the value of the same or different type vars and returns the result
// Numerical types (boolean_t, integer_t, real_t) are subtracted and the result
// is up casted to the more complex type.
// Operands that are not numerical result in an undefined_t value
var var::operator-(const var &right) const
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this - (double)right;
		varType maxtype = type > right.type ? type : right.type;

		switch(maxtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

// Multiplies the value of the same or different type vars and returns the result
// Numerical types (boolean_t, integer_t, real_t) are multiplied and the result
// is up casted to the more complex type.
// Operands that are not numerical result in an undefined_t value
var var::operator*(const var &right) const
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this * (double)right;
		varType maxtype = type > right.type ? type : right.type;

		switch(maxtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

// Divides the value of the same or different type vars and returns the result
// Numerical types (boolean_t, integer_t, real_t) are divided and the result
// is up casted to the more complex type.
// Operands that are not numerical result in an undefined_t value
// Division by zero also results in an undefined_t value
var var::operator/(const var &right) const
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		if((double)right == 0)
		{	return var();
		}

		double value = (double)*this / (double)right;
		varType maxtype = type > right.type ? type : right.type;

		switch(maxtype)
		{	case boolean_t: result = value == 0 ? false : true; break;
			case integer_t: result = (int)value; break;
			case real_t:    result = (double)value; break;
		}
	}

	return result;
}

// Finds the remainder between the same or different type vars and returns the result
// Numerical types (boolean_t, integer_t, real_t) are all casted up to double type,
// then based on the most complex type of both operands, the appropriate type is returned.
// Operands that are not numerical result in an undefined_t value
// Division by zero also results in an undefined_t value
var var::operator%(const var &right) const
{
	var result;

	if(type <= real_t && right.type <= real_t)
	{
		double lvalue = (double)*this;
		double rvalue = (double)right;
		varType maxtype = type > right.type ? type : right.type;

		if(rvalue == 0)
		{	return var();
		}

		switch(maxtype)
		{	case boolean_t: result = lvalue == 1 ? false : true; break;
			case integer_t: result = (int)lvalue % (int)rvalue; break;
			case real_t:    result = fmod(lvalue, rvalue); break;
		}
	}

	return result;
}

// Adds the value of the right hand operand to the var
// Numerical types (boolean_t, integer_t, real_t) are added by value
// When the left hand operand is a text_t, the right hand is concatenated.
// Array_t and Object_t types add elements to merge into the left hand operand
// Any other combination results in no change
// The left hand operand always retains its original internal type
var& var::operator+=(const var &right)
{
	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this + (double)right;
		switch(type)
		{	case boolean_t: *(bool*)data = value == 0 ? false : true; break;
			case integer_t: *(int*)data = (int)value; break;
			case real_t:    *(double*)data = (double)value; break;
		}
	}
	else if(type == text_t)
	{	*(str*)data += right.toString();
	}
	else if(type == array_t && right.type == array_t)
	{	mergeArrays(*this, right);
	}
	else if(type == object_t && right.type == object_t)
	{	mergeObjects(*this, right);
	}

	return *this;
}

// Subtracts the value of the right hand operand from the left
// Numerical types (boolean_t, integer_t, real_t) are subtracted by value
// Any other types results in no change
// The left hand operand always retains its original internal type
var& var::operator-=(const var &right)
{
	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this - (double)right;
		switch(type)
		{	case boolean_t: *(bool*)data = value == 0 ? false : true; break;
			case integer_t: *(int*)data = (int)value; break;
			case real_t:    *(double*)data = (double)value; break;
		}
	}

	return *this;
}

// Multiplies the value of the left hand operand by the right
// Numerical types (boolean_t, integer_t, real_t) are multiplied by value
// Any other types results in no change
// The left hand operand always retains its original internal type
var& var::operator*=(const var &right)
{
	if(type <= real_t && right.type <= real_t)
	{
		double value = (double)*this * (double)right;
		switch(type)
		{	case boolean_t: *(bool*)data = value == 0 ? false : true; break;
			case integer_t: *(int*)data = (int)value; break;
			case real_t:    *(double*)data = (double)value; break;
		}
	}

	return *this;
}

// Divides the value of the left hand operand by the right
// Numerical types (boolean_t, integer_t, real_t) are divided by value
// Any other types results in no change
// The left hand operand changes to undefined_t type on division by zero
var& var::operator/=(const var &right)
{
	if(type <= real_t && right.type <= real_t)
	{
		if((double)right == 0)
		{	*this = var();
			return *this;
		}

		double value = (double)*this / (double)right;
		switch(type)
		{	case boolean_t: *(bool*)data = value == 0 ? false : true; break;
			case integer_t: *(int*)data = (int)value; break;
			case real_t:    *(double*)data = (double)value; break;
		}
	}

	return *this;
}

// Finds the Remainder of the left hand operand divided by the right
// Numerical types (boolean_t, integer_t, real_t) are divided by value
// Any other types results in no change
// The left hand operand changes to undefined_t type on division by zero
var& var::operator%=(const var &right)
{
	if(type <= real_t && right.type <= real_t)
	{
		double lvalue = (double)*this;
		double rvalue = (double)right;

		if(rvalue == 0)
		{	return *this = var();
		}

		switch(type)
		{	case boolean_t: *(bool*)data = lvalue == 1 ? false : true; break;
			case integer_t: *(int*)data = (int)lvalue % (int)rvalue; break;
			case real_t:    *(double*)data = fmod(lvalue, rvalue); break;
		}
	}

	return *this;
}

// Increments a numerical type var by 1 and returns the old value
// Any other types results in no change
var var::operator++(int)
{
	var copy = *this;
	switch(copy.type)
	{	case boolean_t: *(bool*)data = true; break;
		case integer_t: *(int*)data += 1; break;
		case real_t:    *(double*)data += 1; break;
	}

	return copy;
}

// Decrements a numerical type var by 1 and returns the old value
// Any other types results in no change
var var::operator--(int)
{
	var copy = *this;
	switch(type)
	{	case boolean_t: *(bool*)data = false; break;
		case integer_t: *(int*)data -= 1; break;
		case real_t:    *(double*)data -= 1; break;
	}

	return copy;
}

// Increments a numerical type var by 1 and returns the new value
// Any other types results in no change
var& var::operator++()
{
	switch(type)
	{	case boolean_t: *(bool*)data = true; break;
		case integer_t: *(int*)data += 1; break;
		case real_t:    *(double*)data += 1; break;
	}

	return *this;
}

// Decrements a numerical type var by 1 and returns the new value
// Any other types results in no change
var& var::operator--()
{
	switch(type)
	{	case boolean_t: *(bool*)data = false; break;
		case integer_t: *(int*)data -= 1; break;
		case real_t:    *(double*)data -= 1; break;
	}

	return *this;
}