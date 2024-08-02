#pragma once
#include "SDL.h"
#include <unordered_map>
#include <vector>
#include <string>

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

private:
	// RunLoop�ŌĂяo��
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	// ���[�h�����e�N�X�`���̃}�b�v
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// �A�N�e�B�u��Actor�Q���܂�mAactors��
	// �҂���Ԃ�Actor�Q���܂�mPendingActors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;	// SDL�����Window�ւ̃|�C���^
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	//class Ship* mShip;
};

