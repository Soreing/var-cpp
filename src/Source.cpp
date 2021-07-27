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
	/*var test = Object();
	test["firstName"]  = "John";
	test["lastName"] = "Smith";
	test["isAlive"] = true;
	test["age"] = 27;
	test["price"]  = 20.99;
	test["address"] = Object();
	test["address"]["streetAddress"] = "21 2nd Street";
	test["address"]["city"] = "New York";
	test["address"]["state"] = "NY";
	test["address"]["postalCode"] = "10021-3100";
	test["phoneNumbers"] = array{Object(),Object()};
	test["phoneNumbers"][0]["type"] = "home";
	test["phoneNumbers"][0]["number"] = "212 555-1234";
	test["phoneNumbers"][1]["type"] = "office";
	test["phoneNumbers"][1]["number"] = "646 555-4567";
	test["children"] = array{};
	test["spouse"] = var();

	char buffer[512];
	size_t len = writeJCON(test, buffer, 512);

	var res = readJCON(buffer, 512);
	std::cout<< res.toJSON() <<"\n";*/

	var bol = true;
	var num = 5;
	var dec = 10.2;
	var txt = "Hello";
	var arr = array{1,2,3};

	std::cout<< (++dec) <<"\n"; 
	std::cout<< dec <<"\n"; 
	std::cout<< (dec++) <<"\n"; 
	std::cout<< dec <<"\n"; 

	system("PAUSE");
}
