#include <iostream>

int main(int argc, char** argv)
{
	double totalBalance = 0.0;
	double tmpBalance = 0.0;
	for(int i=0; i<12; ++i)
	{
		std::cin>>tmpBalance;
		totalBalance += tmpBalance;
	}
	totalBalance /= 12;
	totalBalance = ((int)(totalBalance * 100 + 0.5))/100.0;
	std::cout<<"$"<<totalBalance<<std::endl;
	return 0;
}