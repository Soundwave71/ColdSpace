#include "Game.h"

int main(){
	// Program entry point.
	Game game;
	while(!game.GetWindow()->IsDone()){

		game.Update();

		game.Render();

		game.LateUpdate();
	}
	return 0;
}