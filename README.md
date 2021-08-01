# var-cpp

# Description

var-cpp is a C++ library for creating JavaScript-like var variables that allow the assignment of data or functions with different types at runtime. The library is complete with relational and arithmetic operators, as well as functions for working with arrays.

# Installation
Include `varcpp.h` in your project for defining the var class. If you want to compile the library from source, include the cpp files for the implementation, all following the format `v_*.cpp`. Alternatively, you can compile the source code to a static library and include it that way.

# Usage
## Creating Variables
You can declare a var object and assign it data. Internally the object has the following types: boolean, integer, real number, text, array, object, binary object, functions, undefined.
```c++
var bol = true;
var num = 5;
var dec = 10.2;
var txt = "Sample Text";
```

After the definition of the variable, you can re-assign the data in the variable, even with a different type from what was assigned originally
```c++
var number = 10;
number = "Ten";
```

## Casting to Basic Types
Numerical type vars can be explicitly casted to some basic C++ types using C-style casting. Currently you can cast to `bool`, `int` and `double`.
```c++
var vdec = 10.2;
double cdec = (double)vdec;
```
Additionally, if a string value represents a value that can be converted to a number, it is possible to cast it to another type to do do simple text to int conversion
```c++
var txt = "10.2";
double dec = (double)txt; 
```

## Printing and Displaying
You can use the toString() function which returns some string representation of the variable, or you an also use the ostream operator, which is overloaded to call the toString() function.
```c++
var num = 5;
string str = num.toString();
cout << num << "  " << num.toString() << endl; 
```
```
Output:
5  5
```

## Working with Arrays
Array type vars use vectors to store elements and resize dynamically. Elements in a var array can have different types. To create an array, you can use the `array` macro.
```c++
var emp = array{} //Creates an empty array
var arr = array{true, 5, 10.2, "Sample Text"};
```

Arrays can even contain other arrays and more complex types
```c++
var complex = array{"Two Arrays", array{1,2,3}, array{true, 10.2}};
```

To access the elements of an array, you can use the square bracket operator `[]` with an index. The elements are returned by reference, so you can make changes to them. If you supply an index too large, the array is resized to be valid.
```c++
var arr = array{1,2,3};
arr[0] = 10.2;
arr[4] = "Sample Text";

cout <<  arr << endl;
```
```
Output:
[ 10.2, 2, 3, undfined, Sample Text ]
```

## Working with Objects
Object type vars use a vector of attributes, where each attribute has a string name and a var value. To create an object, you can use the `object` macro.
```c++
var emp = object{} //Creates an empty object
var obj = object{{"Width", 4}, {"Height", 5}, {"Price", 10.2}};
```
Similarly to arrays, objects can contain other complex attributes.
```c++
var person = object{
    {"name", "John Smith"},
    {"age", 25},
    {"contacts", array{
        object{
            {"type", "home"},
            {"number", "212 555-1234"}
        },
        object{
            {"type", "office"},
            {"number", "646 555-4567"}
        }
    }},
};
```

To access the attributes of an object, you can use the square bracket operator [] with a string. The elements are returned by reference, so you can make changes to them. If you use a name that does not match an attribute, a new attribute is created and returned. **Attribute names are case sensitive!**
```c++
var obj = object{{"Width", 4}, {"Height", 5}, {"Price", 10.2}};
obj["Width"] = "Four";
obj["Quantity"] = 10;

cout << obj << endl;
cout << obj["Something"] << endl;
```
```
Output:
{ Width:Four, Height:5, Price:10.2, Quantity:10 }
undefined
```

## Binary Objects and Byte Streams
Any C/C++ object or byte stream can be stored as a blob type. To create a Binary Object, you can use the blob structure, which expects a char pointer `(char*)` and number of bytes.
```c++
class Box{
public:
    int w, h, l;
    Box(int w, int h, int l)
        : w(w), h(h), l(l)
    {} 
};

void main()
{
    Box b(10, 25, 100);
    char bytes[6] = {0, 1, 2, 3, 4, 5};

    var bx = blob{(char*)&b, sizeof(Box)};
    var ar = blob{bytes, 6};

    cout << bx << endl;
    cout << ar << endl;
}
```
```
Output:
0x0a0000001900000064000000
0x000102030405
```
If you want to retrieve a pointer to the object, you can use the getter function `getData()`, then cast it to the appropriate pointer from `void*`

## Functions as Variables
You can assign functions to var objects and call them with arguments later. Functions **must** use only var return type and arguments, otherwise the behavior is unpredicrable.
```c++
var add(var left, var right){
    return left + right;
}

void main(){
    var foo = add;
    cout << foo(1, 3) << endl;
}
```
Due to limitations, you can only have up to 8 arguments for any function. If you supply more or less arguments than the function requires to be called, the function is not called and undefined is returned.
```c++
cout << foo(1) << endl;
cout << foo(1, 2, 3) << endl;
```
```
Output:
undefined
undefined
```

## Logical and Relational Operators
Logical operators are overloaded to explicitly cast vars to booleans. For numeric types, any value that is non zero is `true`. For other types, any value is `true`. undefined is always `false`.
```c++
var tru = true;
var fls = false;
cout<< (tru || fls) << " " << (tru && fls) << endl;
```
```
Output:
1 0
```
Relational operators compare variables similar to JavaScript
- Numerical comparison if both values are numbers or one is a number and the other converts to a number
- Alphabetical comparison if both values are string or at least one object does not convert to a number. Complex types are converted to string using toString()
- Functions can be compared for equality, but not less/greater than

```c++
var num = 5;
var tx1 = "123";
var tx2 = "5";

cout << (num < tx1);   //true
cout << (tx2 < tx1);   //false
cout << (num == tx2);  //true
```

To check if the values are strictly the same, you can compare the variables to internal types. Internal types include `boolean_t`, `integer_t`, `real_t`, `text_t`, `array_t`, `object_t`, `blob_t`, `function_t`, `undefined_t`.
```c++
var txt = "5";
if(txt == integer_t && txt == 5){
    cout << "Same" << endl;
} else {
    cout << "Different" << endl;
}
```
```
Output:
Different
```
You can also compare if two variables have the same type with the `getType()` getter function.
```c++
var tx1 = "Sample Text";
var tx2 = "5";
if(tx1.getType() == tx2.getType()){
    cout << "Same" << endl;
} else {
    cout << "Different" << endl;
}
```
```
Output:
Same
```

## Arithmetic Operators
All basic math operations are implemented for numerical types, which includes `boolean_t`, `integer_t` and `real_t`. When two different types are used, the result always matches the more complex type.
```c++
var num = 50;
var dec = 10.5;
cout << (num + dec) << endl;
cout << (num - dec) << endl;
cout << (num * dec) << endl;
cout << (num / dec) << endl;
cout << (num % dec) << endl;
```
```
Output:
60.5
39.5
525
4.761905
8.0
```

When arithmetic operators are used together with assignment, such as `+=`, `-=`, etc.. the original type of the variable is preserved.
```c++
var num = 50;
var dec = 10.5;
num /= dec;
cout << num << endl;
```
```
Output:
4
```

The `+` operator also joins together objects with strings if at least one operand is a string.
```c++
var txt = "Sample Text";
var num = 5;
cout << (txt + num) << endl;
```
```
Output:
Sample Text5
```

When used between two arrays or objects, the `+` operator creates a new array or object that contains all the elements or attributes from each operand. the `+=` operator merges the elements/attributes of the right into the left hand operand.
```c++
var ar1 = array{true, 5, 10.2};
var ar2 = array{"Sample Text", 99.75};
ar1 += ar2;

cout << ar1 << endl;
```
```
Output:
[ true, 5, 10.2, Sample Text, 99.75 ]
```

## Array Functions
Several functions from JavaScript and C++ vectors are implemented to work with vars of `array_t` type.

`erase`, `every`, `fill`, `filter`, `find`, `findIndex`, `forEach`, `includes`, `indexOf`, `insert`, `join`, `length`, `pop`, `push`, `reverse`, `shift`, `slice`, `some`, `splice`, `unshift`
