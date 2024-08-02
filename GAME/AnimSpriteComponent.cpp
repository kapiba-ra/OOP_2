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
		// CurrFrame�̍X�V
		mCurrFrame += mAnimFPS * deltaTime;

		// CurrFrame�̊����߂� �e�N�X�`���̐��𒴂��Ȃ��悤��
		while (mCurrFrame >= mAnimTextures.size())
		{
			mCurrFrame -= mAnimTextures.size();
		}

		// �����_�̃e�N�X�`����ݒ� int�ɃL���X�g����Ə��������͖��������
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// �A�j���[�V�����̍ŏ��̃t���[����n��
		// SetTexture��SpriteComponent�Œ�`����Ă���
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}
