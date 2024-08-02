#pragma once
#include "SpriteComponent.h"
#include <vector>

// アニメーションの描画用のコンポーネント

class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    // 一定のフレームごとにアニメーションを更新 componentからoverride
    void Update(float deltaTime) override;
    // アニメーションに使うテクスチャの設定
    void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
    // アニメーションのFPSの設定
    float GetAnimFPS() const { return mAnimFPS; }
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

private:
    // アニメーションでのすべてのテクスチャ
    std::vector<SDL_Texture*> mAnimTextures;
    // 現在表示しているフレーム
    float mCurrFrame;
    // アニメーションのFPS 動的に変更可能
    float mAnimFPS;
};
