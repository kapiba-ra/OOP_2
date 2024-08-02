#include "Ufo.h"
#include "SpriteComponent.h"

Ufo::Ufo(Game* game)
	: Actor(game)

{
	// Actorクラスのメンバ配列mComponentsに追加される
	// Gameクラスのメンバ配列mSpritesにも追加される
	new SpriteComponent(this);
}
