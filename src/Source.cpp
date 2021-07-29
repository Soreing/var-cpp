#include "varcpp.h"
#include <iostream>

int main()
{
	var arr = array{};

	try
	{	arr.pop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	system("PAUSE");
}
