#ifndef VARCPP_H
#define VARCPP_H

#include <string>
#include <vector>

#define vec std::vector		// std::vector shortcut macro
#define array vec<var>		// std::vector<var> shortcut macro
typedef std::string str;	// std::string shortcut macro
typedef const char* cstr;	// const char* c-string shortcut macro

// Internal var types for operations
enum varType { boolean_t, integer_t, real_t, text_t, array_t, object_t, function_t, undefined_t };

// Empty base class for var objects
class Object {};


// Dynamic typeless variable class
// Data is allocated on the heap for each instance, and only the pointer is stored
// The original type of the variable is converted to an internal varType for operations
class var
{
public:
	void*   data;	// Pointer to the allocated data. void* is universal
	varType type;	// Internal type of the variable for operations

public:
	var();
	var(void* data, varType type);

	// Copy Constructor and Assignment
	var(const var &val);
	var& operator=(const var &val);

	// Delegate the construction of any type to the assignment operator
	template <class T>
	var(T val);

	var& operator=(int    val);
	var& operator=(short  val);
	var& operator=(long   val);
	var& operator=(bool   val);
	var& operator=(double val);
	var& operator=(float  val);
	var& operator=(str    val);
	var& operator=(cstr   val);
	var& operator=(Object val);
	var& operator=(const array &val);

	// Convert the value of the var to the appropriate string based on type
	// Objects and arrays print all elements
	str toString() const;

	// Accesses an attribute in an object var
	// The attributes are addressed by a tage name
	var& operator[](str name);

	// Accesses an element in an array var
	// The attributes are addressed by an index
	var& operator[](size_t idx);

	// Clean memory allocation if the object contains any
	void clean();

	~var();
};

template <class T>
var::var(T val)
	: data(NULL), type(undefined_t)
{
	this->operator=(val);
}

// Load the value of the var to the ostream 
std::ostream& operator<<(std::ostream& o, const var& v);


// Named attributes for var objects
struct atr
{	str name;
	var val;
};

#endif