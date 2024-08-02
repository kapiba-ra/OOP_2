#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)	//nullptrでないときのみ実行
	{
		SDL_Rect r;
		// 幅と高さを所有アクターのスケールに応じて拡縮する
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// 矩形の中心を所有アクターの位置に合わせる
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// 描画
		SDL_RenderCopyEx(renderer	// 描画先のレンダラー
			, mTexture	// 描画したいテクスチャ
			, nullptr	// 描画したいテクスチャの範囲(全部)
			, &r		// 出力先の矩形
			, -Math::ToDegrees(mOwner->GetRotation())	// ラジアンから度数へ
			, nullptr	// 回転中心
			, SDL_FLIP_NONE);		// 反転の有無
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	// Draw関数が使えるように (mTextureがnullptrではなくなり,trueを返す)
	mTexture = texture;
	// テクスチャの高さと幅を求める
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
