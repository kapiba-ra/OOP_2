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
	// �r�f�I�E�I�[�f�B�I�T�u�V�X�e���̏�����
	// �������������̓[�����Ԃ���� �r�b�g�a�[���Ȃ珉��������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Window�̍쐬
	mWindow = SDL_CreateWindow(
		"GameProgramming in C++ (chapter 2)"	// �^�C�g����
		, 100		// Window�̍������x���W
		, 100		// Window�̍������y���W
		, 1024		// Window�̕�(Width)
		, 768		// Window�̍���(Height)
		, 0);		// �������������FullScreen�ɂł����肷��
	// nullptr�Ȃ珉�������s
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Renderer�̍쐬
	mRenderer = SDL_CreateRenderer(
		mWindow		// �쐬����Renderer�̕`��ΏۂƂȂ�Window
		, -1		// �O���t�B�b�N�X�h���C�o�[�̐ݒ�
		// CPU�ł͂Ȃ�GPU�ŕ`�揈���ƌv�Z | ��������
		, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// nullptr�Ȃ珉�������s
	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// SDL_image�̏����� ��������ƃ[���ȊO(�r�b�g�}�X�N)��Ԃ�
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	LoadData();

	// �������I�����̌o�ߎ��Ԃ��L�^����
	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::LoadData()
{
	// Ship���쐬
	//mShip = new Ship(this);	//Game�N���X�̃����o�z��mActors�ɒǉ������
	//mShip->SetPosition(Vector2(100.0f, 384.0f));
	//mShip->SetScale(1.5f);

	// Ufo�̍쐬
	Actor* temp;
	temp = new Ufo(this);


	// �w�i�p�̃A�N�^�[�����
	temp = new Actor(this);
	temp->SetPosition(Vector2(512.0f, 384.0f));
	// �����w�i
	// new���邾����Actor�N���X�̃����o�z��mComponents�ɒǉ������
	BGSpriteComponent* bg = new BGSpriteComponent(temp);
	std::vector<SDL_Texture*> bgtexs = {
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);
	// �߂��w�i
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
	// �L���[�ɃC�x���g�������true��Ԃ����[�v����
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// �L�[�̏�Ԃ̔z����擾 ������Ă���L�[�̐��͂���Ȃ��̂�NULL
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// �D�̓��͏���
	//mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	// �t���[�����[�g�̈ێ�(60fps)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
	// deltaTime���v�Z
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// �S�Ă�Actor���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �ۗ�����Actor��mActors�ֈړ�
	for (auto pending : mPendingActors)
	{
		mPendingActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ����Actor���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;

	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���񂾃A�N�^�[������ mActors����폜�����
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);

	// �X�v���C�g�R���|�[�l���g�̔z���P���Ƀ��[�v����(���Ԃ͐ݒ��)
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
	// �A�N�^�[���폜
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	// �e�N�X�`�����폜
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}

void Game::AddActor(Actor* actor)
{
	// Actor�̍X�V���Ȃ�ۗ��ɒǉ�
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	// ��X�V���Ȃ炻�̂܂�mActors�ɒǉ�
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// ��������z��̍Ō�Ɠ���ւ��čŌ�������Ƃ����A���S���Y��
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
	// �\�[�g�ς݂̔z��ő}���_��������
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
	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}������
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// ���Ԃ͎����Ă���̂ŃX���b�v�ł͂Ȃ������č폜
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// �e�N�X�`�������ɘA�z�z��ɓ����Ă��邩�`�F�b�N
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// �t�@�C�����烍�[�h
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			// ���[�h���s���̏���
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// �T�[�t�F�X����e�N�X�`�����쐬
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			// �e�N�X�`���ւ̕ϊ����s���̏���
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}
