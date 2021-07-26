#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include "varcpp.h"

var add(var a, var b, var c)
{
	std::cout<< "A: " << a <<"\n";
	std::cout<< "B: " << b <<"\n";
	std::cout<< "C: " << c <<"\n";

	return 0xDEAD;
}

bool isHello(var element)
{	return element == "Hello" ? true : false;
}

struct Bound
{	int lower;
	int upper;
};

int main()
{
	var obj = Object();
	obj["width"]  = 1;
	obj["height"] = 2;
	obj["length"] = 3.5;
	obj["weight"] = 6.75;
	obj["price"]  = 20.99;


	var a = array{
		true, 
		1000, 
		10.2, 
		"Hello_World", 
		array{
			true, 
			1, 
			10.2, 
			"Hello_World"
		}, 
		obj, 
		add, 
		var()
	};

	char buffer[512];
	size_t len = writeJCON(a, buffer, 512);
	
	var b = readJCON(buffer, 512);
	std::cout<< b <<"\n";

	system("PAUSE");

	var dat;
	var num = 25;
	var dec = 10.2;
	var txt = "Sample Text";
	var obj1 = Object();
	var obj2 = Object();
	var arr = array{1, 10.2, "Hello_World"};
	var fnc = add;

	obj1["width"]  = 1;
	obj1["height"] = 2;
	obj1["length"] = 3.5;

	obj2["weight"] = 6.75;
	obj2["price"]  = 20.99;

	std::cout << dat << "\n";
	std::cout << num << "\n";
	std::cout << dec << "\n";
	std::cout << txt << "\n";
	std::cout << arr << "\n";
	std::cout << obj1 << "\n";
	std::cout << obj2 << "\n";
	std::cout << (obj1+obj2) << "\n";
	std::cout << fnc << "\n\n";

	std::cout << "Function Returned: "<< fnc(1, "Hello", 12.5) <<"\n";
	std::cout << "Function Returned: "<< fnc(1, 2) <<"\n";
	std::cout << "Function Returned: "<< fnc(1, 2, 3, 4) <<"\n\n";

	system("PAUSE");
}
