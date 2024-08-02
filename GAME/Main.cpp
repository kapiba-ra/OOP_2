#include "Game.h"

int main(int argc, char** argv)
{
	Game game;	// Gameのコンストラクタ起動
	bool success = game.Initialize();

	// 初期化に成功したらRunLoopを起動
	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
}