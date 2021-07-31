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
private:
	void*   data;	// Pointer to the allocated data. void* is universal
	varType type;	// Internal type of the variable for operations

public:
	// Constructors
	var();
	var(void* data, varType type);
	var(const var &val);
	template <class T> var(T val);

	// Assignment Operator
	var& operator=(const var &val);
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

	template<class... Args>
	var& operator=(var(*val)(Args...));
	var& operator=(var(*val)());

	// () Operator for calling functions, up to 8 parameters
	var operator()() const;
	var operator()(var p1) const;
	var operator()(var p1, var p2) const;
	var operator()(var p1, var p2, var p3) const;
	var operator()(var p1, var p2, var p3, var p4) const;
	var operator()(var p1, var p2, var p3, var p4, var p5) const;
	var operator()(var p1, var p2, var p3, var p4, var p5, var p6) const;
	var operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7) const;
	var operator()(var p1, var p2, var p3, var p4, var p5, var p6, var p7, var p8) const;

	// [] Operator for accessing attributes in Objects and elements in Arrays
	var& operator[](str name);
	const var& operator[](str name) const;
	var& operator[](size_t idx);
	const var& operator[](size_t idx) const;

	// C-Style Type Cast Operators
	explicit operator bool() const;
	explicit operator int() const;
	explicit operator double() const;

	// Arithmetic Operators
	var operator+(const var &right) const;
	var operator-(const var &right) const;
	var operator*(const var &right) const;
	var operator/(const var &right) const;
	var operator%(const var &right) const;

	// Compound Arithmetic Operators with Assignment
	var& operator+=(const var &right);
	var& operator-=(const var &right);
	var& operator*=(const var &right);
	var& operator/=(const var &right);
	var& operator%=(const var &right);

	// Increment and Decrement Operators (Pre and Post)
	var& operator++();
	var& operator--();
	var operator++(int);
	var operator--(int);

	// Comparison Function for Relational Operators
	int compare(const var &right) const;

	// Logical AND and OR Operators 
	bool operator&&(const var &right) const;
	bool operator||(const var &right) const;

	// Relational Operators
	bool operator==(const var &right) const;
	bool operator<(const var &right)  const;
	bool operator>(const var &right)  const;
	bool operator!=(const var &right) const;
	bool operator<=(const var &right) const;
	bool operator>=(const var &right) const;

	bool operator==(const varType t) const;

	// Gets the length of an array
	size_t length() const;

	// Checks if the array includes an element
	bool includes(const var &key) const;

	// Returns the index of an element
	size_t indexOf(const var &key) const;
	
	// Reverses the contents of an array
	void reverse();


	// Push a new element to the end of the array
	int push(const var &e);
	int push_back(const var &e);
	// Remove and return an element from the end of the array
	var pop();
	var pop_back();

	// Push a new element to the beginning of the array
	int unshift(const var &e);
	int push_front(const var &e);
	// Remove and return an element from the beginning of the array
	var shift();
	var pop_front();

	// Inserts a new element at a position in the array
	int insert(int pos, const var &e);
	
	//Erases one or a range of elements from the array
	int erase(int pos);
	int erase(int start, int end);

	// Erases and/or Inserts elements from/to the array 
	void splice(int beg, int count );
	void splice(int beg, const var &elements);
	void splice(int beg, int count, const var &elements);

	// Returns a sub section of the array
	var slice(int beg, int end = 0) const;

	// Joins the elements in the array with a separator string
	str join(str separator = ",") const;

	// Fills the array with values between indexes
	void fill(int start, int end, const var &val);
	void fill(int end, const var &val);

	// Tests if every element of an array resolves to true for a function
	bool every(bool(*fnc)(var e)) const;
	bool every(bool(*fnc)(var e, void* params), void* params) const;

	// Tests if at least one element of an array resolves to true for a function
	bool some(bool(*fnc)(var e)) const;
	bool some(bool(*fnc)(var e, void* params), void* params) const;

	// Returns a new array with only elements that resolved to true for a function
	var filter(bool(*fnc)(var e)) const;
	var filter(bool(*fnc)(var e, void* params), void* params) const;

	// Returns the first element in an array that resolved to true for a function
	var& find(bool(*fnc)(var e)) const;
	var& find(bool(*fnc)(var e, void* params), void* params) const;

	// Returns the index of the first element in an array that resolved to true for a function
	size_t findIndex(bool(*fnc)(var e)) const;
	size_t findIndex(bool(*fnc)(var e, void* params), void* params) const;

	// Calls a function with each element in an array
	void forEach(void(*fnc)(var &e));
	void forEach(void(*fnc)(const var &e)) const;
	void forEach(void(*fnc)(var &e, void* params), void* params);
	void forEach(void(*fnc)(const var &e, void* params), void* params) const;

	// Convert the value of the var to the appropriate string based on type
	// Objects and arrays print all elements
	str toString() const;
	str toJSON(str indent="") const;
	int toBinary(char* buff, int size) const;

	// Exception classes
	class ArrayIncorrectRangeException{};
	class ArrayIncorrectIndexException{};
	class ArrayElementNotFoundException{};
	class TypeMismatchException{};
	class BufferSizeException{};

	// Getters & Setters
	void* getData() const;
	varType getType() const;

	// Clean memory allocation if the object contains any
	void clean();
	~var();
};

// Compares the internal type of the var to a given type, returns true if it matches
bool operator==(const varType t, const var& val);

// Load the value of the var to the ostream 
std::ostream& operator<<(std::ostream& o, const var& v);

// Write var object to binary data
size_t writeBinary(const var& val, char* buff, const int size);

// Read var object from binary data
var readBinary(char* buff, const size_t size, size_t* bytes = NULL);

// Function structure
struct Function
{	int params;
	void* fptr;
};

// Named attributes for var objects
struct atr
{	str name;
	var val;
};

template <class T>
var::var(T val)
	: data(NULL), type(undefined_t)
{
	this->operator=(val);
}

template<class... Args>
var& var::operator=(var(*val)(Args...))
{
	if (data != NULL)
	{	clean();
	}

	type = function_t;
	data = (void*)new Function{sizeof...(Args), val};
	return *this;
}

#endif