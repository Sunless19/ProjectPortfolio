#include <iostream>
#include <Windows.h>
#include <cstdlib>

void stopwatch()
{
	short seconds = 60;

	for (int i = 60; i >= 0; i--)
	{
		system("cls");
		std::cout << "Time remaining:" << seconds;
		--seconds;
		Sleep(1000); //1000ms = 1s
	}
	system("cls");
	std::cout << "Round finished!";
}

int main()
{
	stopwatch();
	return 0;
}