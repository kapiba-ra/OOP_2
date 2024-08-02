#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	// Actorの状態管理用に列挙型を用意 EはEnum
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	/*
	Update()はゲームから呼び出される更新関数 override不可
	UpdateComponents()はActorが持つ全コンポーネントを更新 override不可
	UpdateActor()はActor独自の更新処理 override可能
	*/
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime) {};

	// ゲッターとセッター
	const Vector2& GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// コンポーネントの追加・削除
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actorの状態
	State mState;

	// 座標変換 トランスフォーム
	Vector2 mPosition;	// 中心位置
	float mScale;		// 拡大率
	float mRotation;	// 回転の角度

	// Actorが持つコンポーネントの配列 ここに組み込む
	std::vector<class Component*> mComponents;
	// Gameクラスのポインタを用意 コンストラクタで取得される
	class Game* mGame;
};