#include "Game.h"

int main(int argc, char** argv)
{
	Game game;	// Game�̃R���X�g���N�^�N��
	bool success = game.Initialize();

	// �������ɐ���������RunLoop���N��
	if (success)
	{
		game.RunLoop();
	}

	game.Shutdown();
	return 0;
}