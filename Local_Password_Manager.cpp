#include <iostream>
#include <string>
#include <Windows.h>

#include "./include/Command.h"

int main(int argc, char* argv[]) {

	SetConsoleCP(CP_UTF8);

	Command user;

	user.run();

	return 0;
}