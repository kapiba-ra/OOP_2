#pragma once

// Component(����)��Actor�ɒǉ������@�\

class Component
{
public:
	// Component�̎������Actor�̃|�C���^���󂯎��
	// updataOrder���������R���|�[�l���g����ɍX�V�����
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// �R���|�[�l���g�̍X�V���� �h���N���X��override����
	virtual void Update(float deltaTime) {};

	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// ���LActor
	class Actor* mOwner;
	// �X�V��
	int mUpdateOrder;
};