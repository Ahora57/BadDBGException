 
#include <iostream>
#include <Windows.h>

int main()
{
	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE))
		{
			std::cout << "Just hello!\n";
		}
		Sleep(500);
	}
    
}
 