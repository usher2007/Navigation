#include <iostream>

const double pi = 3.115926;
int main(int argc, char** argv)
{
	int count = 0;
	std::cin>>count;
	double x=0.0, y=0.0, r2=0.0;
	int *years = new int[count];
	for(int i=0; i<count; ++i)
	{
		std::cin>>x>>y;
		r2 = x*x + y*y;
		years[i] = (int)(pi*r2*0.5 / 50) + 1;
	}

	for(int i=0; i<count; ++i)
	{
		std::cout<<"Property "<<(i+1)<<": This property will begin eroding in year "<<years[i]<<".\n";
	}
	std::cout<<"END OF OUTPUT.";
	return 0;
}