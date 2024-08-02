#pragma once

// Component(成分)はActorに追加される機能

class Component
{
public:
	// Componentの持ち主のActorのポインタを受け取る
	// updataOrderが小さいコンポーネントが先に更新される
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// コンポーネントの更新処理 派生クラスでoverrideする
	virtual void Update(float deltaTime) {};

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// 所有Actor
	class Actor* mOwner;
	// 更新順
	int mUpdateOrder;
};