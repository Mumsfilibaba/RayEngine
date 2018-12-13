#include "Sandbox.h"

int main(int argc, char* argv[])
{
	Sandbox* pApplication = new Sandbox();
	int result = pApplication->Run();
	
	delete pApplication;
	return result;
}