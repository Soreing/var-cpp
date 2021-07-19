#include <string>
#include <vector>
#include <iostream>

#include "varcpp.h"

var add(var a, var b, var c)
{
	std::cout<< "A: " << a <<"\n";
	std::cout<< "B: " << b <<"\n";
	std::cout<< "C: " << c <<"\n";

	return 0xDEAD;
}

int main()
{
	var dat;
	var num = 25;
	var dec = 10.2;
	var txt = "Sample Text";
	var obj = Object();
	var arr = array{1, 10.2, "Hello_World"};
	var fnc = add;

	obj["width"]  = 1;
	obj["height"] = 2;
	obj["length"] = 3.5;

	std::cout << dat << "\n";
	std::cout << num << "\n";
	std::cout << dec << "\n";
	std::cout << txt << "\n";
	std::cout << arr << "\n";
	std::cout << obj << "\n";
	std::cout << fnc << "\n\n";

	std::cout << "Function Returned: "<< fnc(1, "Hello", 12.5) <<"\n";
	std::cout << "Function Returned: "<< fnc(1, 2) <<"\n";
	std::cout << "Function Returned: "<< fnc(1, 2, 3, 4) <<"\n\n";

	system("PAUSE");
}
