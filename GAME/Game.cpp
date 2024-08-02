#include "Game.h"
#include "SDL_image.h"
#include "Actor.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include <algorithm>
#include "Ufo.h"

Game::Game()
	: mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
{
}

bool Game::Initialize()
{
	// ビデオ・オーディオサブシステムの初期化
	// 初期化成功時はゼロが返される ビット和ゼロなら初期化成功
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Windowの作成
	mWindow = SDL_CreateWindow(
		"GameProgramming in C++ (chapter 2)"	// タイトル名
		, 100		// Windowの左上隅のx座標
		, 100		// Windowの左上隅のy座標
		, 1024		// Windowの幅(Width)
		, 768		// Windowの高さ(Height)
		, 0);		// ここをいじるとFullScreenにできたりする
	// nullptrなら初期化失敗
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Rendererの作成
	mRenderer = SDL_CreateRenderer(
		mWindow		// 作成するRendererの描画対象となるWindow
		, -1		// グラフィックスドライバーの設定
		// CPUではなくGPUで描画処理と計算 | 垂直同期
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// nullptrなら初期化失敗
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// SDL_imageの初期化 成功するとゼロ以外(ビットマスク)を返す
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	// 初期化終了時の経過時間を記録する
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::LoadData()
{
	// Shipを作成
	//mShip = new Ship(this);	//Gameクラスのメンバ配列mActorsに追加される
	//mShip->SetPosition(Vector2(100.0f, 384.0f));
	//mShip->SetScale(1.5f);

	// Ufoの作成
	Actor* temp;
	temp = new Ufo(this);


	// 背景用のアクターを作る
	temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// 遠い背景
	// newするだけでActorクラスのメンバ配列mComponentsに追加される
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// 近い背景
	bg = new BGSpriteComponent(temp, 50);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	// キューにイベントがあればtrueを返しループする
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// キーの状態の配列を取得 押されているキーの数はいらないのでNULL
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// 船の入力処理
	//mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// フレームレートの維持(60fps)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	// deltaTimeを計算
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// 全てのActorを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 保留中のActorをmActorsへ移動
	for (auto pending : mPendingActors)
	{
		mPendingActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだActorを一時配列に追加
	std::vector<Actor*> deadActors;

	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを消す mActorsから削除される
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// スプライトコンポーネントの配列を単純にループ処理(順番は設定済)
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::UnloadData()
{
	// アクターを削除
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	// テクスチャを削除
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::AddActor(Actor* actor)
{
	// Actorの更新中なら保留に追加
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	// 非更新中ならそのままmActorsに追加
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// 見つけたら配列の最後と入れ替えて最後を消すというアルゴリズム
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// ソート済みの配列で挿入点を見つける
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	// イテレータの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// 順番は失われているのでスワップではなく見つけて削除
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// テクスチャが既に連想配列に入っているかチェック
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// ファイルからロード
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			// ロード失敗時の処理
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// サーフェスからテクスチャを作成
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			// テクスチャへの変換失敗時の処理
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
