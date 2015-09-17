#include "app/app.h"

App* app;

int main(){
	app = new App();
	if(app->Init() == ApplicationFail::NONE){
		while (app->Update() == true){
			app->Tick();
			app->Draw();
		}
		app->Shutdown();
	}

	delete(app);

	return 0;
}