#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

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
	var a = array{};
	a.fill(10, 0);
	
	srand(0);
	for(int i=0; i<10; i++)
	{	a[i] = rand() % 100;
	}

	std::cout << a << "\n";
	a.insert(5, "Hello");
	std::cout << a << "\n";
	a.insert(15, "Bye");
	std::cout << a << "\n";
	a.erase(5);
	std::cout << a << "\n";
	a.erase(5, 10);
	std::cout << a << "\n";
	a.erase(1, 100);
	std::cout << a << "\n";
	//a.forEach([](var &e)->void { e = e + 1;});

	//std::cout << a.length() << "\n";

	/*Bound bnd {30, 60};
	bool res = a.some([](var e, void* params)->bool
	{	
		Bound lims = *(Bound*)params;
		return ((e >= lims.lower) && (e <= lims.upper)) ? true : false;

	}, (void*)&bnd); */

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
