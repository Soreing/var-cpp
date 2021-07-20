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

/*int opPlusTest()
{
	var bol = true;
	var num = 25;
	var dec = 10.2;
	var txt = "Sample Text";
	var arr1 = array{1, 10.2, "Hello_World"};
	var arr2 = array{1, 2, true};
	var obj1 = Object();
	var obj2 = Object();
	var fnc = add;

	obj1["width"]  = 1;
	obj1["height"] = 2;
	obj1["length"] = 3.5;

	obj2["weight"] = 6.75;
	obj2["price"]  = 20.99;

	std::cout<< (bol + true) <<"\n";
	std::cout<< (bol + 10.5) <<"\n";
	std::cout<< (num + 5) <<"\n";
	std::cout<< (num + dec) <<"\n";
	std::cout<< (dec + 5) <<"\n";
	std::cout<< (txt + bol) <<"\n";
	std::cout<< (txt + num) <<"\n";
	std::cout<< (txt + dec) <<"\n";
	std::cout<< (txt + arr1) <<"\n";
	std::cout<< (txt + obj1) <<"\n";
	std::cout<< (txt + fnc) <<"\n";
	std::cout<< (arr1 + arr2) <<"\n";
	std::cout << (obj1+obj2) << "\n";

	system("PAUSE");
}*/

int main()
{
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
