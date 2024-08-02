#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	// Actor�̏�ԊǗ��p�ɗ񋓌^��p�� E��Enum
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	/*
	Update()�̓Q�[������Ăяo�����X�V�֐� override�s��
	UpdateComponents()��Actor�����S�R���|�[�l���g���X�V override�s��
	UpdateActor()��Actor�Ǝ��̍X�V���� override�\
	*/
	void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime) {};

	// �Q�b�^�[�ƃZ�b�^�[
	const Vector2& GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	void SetScale(float scale) { mScale = scale; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// �R���|�[�l���g�̒ǉ��E�폜
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actor�̏��
	State mState;

	// ���W�ϊ� �g�����X�t�H�[��
	Vector2 mPosition;	// ���S�ʒu
	float mScale;		// �g�嗦
	float mRotation;	// ��]�̊p�x

	// Actor�����R���|�[�l���g�̔z�� �����ɑg�ݍ���
	std::vector<class Component*> mComponents;
	// Game�N���X�̃|�C���^��p�� �R���X�g���N�^�Ŏ擾�����
	class Game* mGame;
};