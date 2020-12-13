#include <stdio.h>
#include <math.h>

typedef struct list {
	int number;
	list* next;
}list;

list* Get__PrimeFactors(int number)
{
	list* p_prime_numbers_list = NULL;
	while (number % 2 == 0)
	{
		Add__ToList(2);
		number /= 2;
	}
	int factor_to_check = 3;
	for  (;  factor_to_check < (int)sqrt(number) ; factor_to_check+=2)
	{
		while (number % factor_to_check == 0)
		{
			Add__ToList(factor_to_check);
			number /= factor_to_check;
		}
	}
	if (number > 2)
	{
		Add__ToList(number);
	}
}

int main(int argc, char* argv[])
{
		
}