#include "Sandbox.h"

int main(int argc, char* argv[])
{
	Sandbox* pApplication = new Sandbox();
	pApplication->Run();
	delete pApplication;

	return 0;
}