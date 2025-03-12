#include <iostream>
#include <cmath>

#include <profilepp/profilepp.hpp>

void function1()
{
	PROFILE_FN();

	for(int idx = 0; idx < 10'000; ++idx)
		std::cout << std::sqrt(idx) << std::endl;
}

void benchmarking()
{
	PROFILE_FN();

	function1();

	{
		PROFILE("Loop #1");
		double y;
		for(int idx = 0; idx < 10'000; ++idx)
		{
			y += sqrt(idx);
			std::cout << y << std::endl;
		}
			
	}
}

int main(int argc, char *argv[])
{
	ProfileSession::getInstance()->begin();
	
	benchmarking();

	ProfileSession::getInstance()->end();
	
	return EXIT_SUCCESS;
}
