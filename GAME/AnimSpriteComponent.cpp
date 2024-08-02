#include "AnimSpriteComponent.h"
#include "Math.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// CurrFrameの更新
		mCurrFrame += mAnimFPS * deltaTime;

		// CurrFrameの巻き戻し テクスチャの数を超えないように
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		// 現時点のテクスチャを設定 intにキャストすると小数部分は無視される
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// アニメーションの最初のフレームを渡す
		// SetTextureはSpriteComponentで定義されている
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}
