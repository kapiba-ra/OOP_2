#include "Ufo.h"
#include "SpriteComponent.h"

Ufo::Ufo(Game* game)
	: Actor(game)

{
	// Actor�N���X�̃����o�z��mComponents�ɒǉ������
	// Game�N���X�̃����o�z��mSprites�ɂ��ǉ������
	new SpriteComponent(this);
}
