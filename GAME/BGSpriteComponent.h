#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

// 背景描画用のコンポーネント

class BGSpriteComponent :
    public SpriteComponent
{
public:
    // 描画順序の初期値は小さくする(背景)
    BGSpriteComponent(class Actor* owner, int drawOrder = 10);
    // 更新と描画はそれぞれComponent,SpriteComponentからoverride
    void Update(float deltaTime) override;
    void Draw(SDL_Renderer* renderer) override;
    // 背景のテクスチャの設定
    void SetBGTextures(const std::vector<SDL_Texture*>& textures);
    // 画面サイズとスクロール速度のSetter,Getter
    void SetScreenSize(const Vector2& size) { mScreenSize = size; }
    void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
    float GetScrollSpeed() const { return mScrollSpeed; }

private:
    // 個々の背景画像とオフセットをカプセル化する構造体
    struct BGTexture
    {
        SDL_Texture* mTexture;
        Vector2 mOffset;
    };
    std::vector<BGTexture> mBGTextures;
    Vector2 mScreenSize;
    float mScrollSpeed;
};

