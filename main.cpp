#include "console.h"

int main()
{
	Console console;
	if (console.init())
	{
		console.run();
	}
	return 0;
}
