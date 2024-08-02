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
	// RunLoopで呼び出す
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void LoadData();
	void UnloadData();

	// ロードされるテクスチャのマップ
	std::unordered_map<std::string, SDL_Texture*> mTextures;

	// アクティブなActor群を含むmAactorsと
	// 待ち状態のActor群を含むmPendingActors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;	// SDLが作るWindowへのポインタ
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	//class Ship* mShip;
};

