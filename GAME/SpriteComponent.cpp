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
	if (mTexture)	//nullptr�łȂ��Ƃ��̂ݎ��s
	{
		SDL_Rect r;
		// ���ƍ��������L�A�N�^�[�̃X�P�[���ɉ����Ċg�k����
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// ��`�̒��S�����L�A�N�^�[�̈ʒu�ɍ��킹��
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// �`��
		SDL_RenderCopyEx(renderer	// �`���̃����_���[
			, mTexture	// �`�悵�����e�N�X�`��
			, nullptr	// �`�悵�����e�N�X�`���͈̔�(�S��)
			, &r		// �o�͐�̋�`
			, -Math::ToDegrees(mOwner->GetRotation())	// ���W�A������x����
			, nullptr	// ��]���S
			, SDL_FLIP_NONE);		// ���]�̗L��
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	// Draw�֐����g����悤�� (mTexture��nullptr�ł͂Ȃ��Ȃ�,true��Ԃ�)
	mTexture = texture;
	// �e�N�X�`���̍����ƕ������߂�
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
