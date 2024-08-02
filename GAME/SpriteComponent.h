#pragma once
#include "Component.h"
#include "SDL.h"

// 描画用のコンポーネント

class SpriteComponent :
    public Component
{
public:
    // drawOrderが低い程、遠くに置かれる
    SpriteComponent(class Actor* owner, int drawOrder = 100);
    ~SpriteComponent();
    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);
    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

protected:
    // 描画するテクスチャ
    SDL_Texture* mTexture;

    int mDrawOrder;
    int mTexHeight;
    int mTexWidth;
};

