#include "AppManager.h"

CAppManager* app;

int main(int argc, char **argv)
{
	app = new CAppManager();
	app->Init();	
	app->Run();

	delete app;
	return 0;
}