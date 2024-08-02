#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

// �w�i�`��p�̃R���|�[�l���g

class BGSpriteComponent :
    public SpriteComponent
{
public:
    // �`�揇���̏����l�͏���������(�w�i)
    BGSpriteComponent(class Actor* owner, int drawOrder = 10);
    // �X�V�ƕ`��͂��ꂼ��Component,SpriteComponent����override
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;
    // �w�i�̃e�N�X�`���̐ݒ�
    void SetBGTextures(const std::vector<SDL_Texture*>& textures);
    // ��ʃT�C�Y�ƃX�N���[�����x��Setter,Getter
    void SetScreenSize(const Vector2& size) { mScreenSize = size; }
    void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
    float GetScrollSpeed() const { return mScrollSpeed; }

private:
    // �X�̔w�i�摜�ƃI�t�Z�b�g���J�v�Z��������\����
    struct BGTexture
    {
        SDL_Texture* mTexture;
        Vector2 mOffset;
    };
    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};

