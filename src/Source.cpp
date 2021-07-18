#include <string>
#include <vector>
#include <iostream>

#include "varcpp.h"

int main()
{

	var data;
	var num = 25;
	var dec = 10.2;
	var text = "Sample Text";
	var obj = Object();
	var arr = array{1, 10.2, "Hello_World"};

	obj["width"]  = 1;
	obj["height"] = 2;
	obj["length"] = 3.5;

	std::cout << data << "\n";
	std::cout << num << "\n";
	std::cout << dec << "\n";
	std::cout << text << "\n";
	std::cout << arr << "\n";
	std::cout << obj << "\n\n";

	system("PAUSE");
}

