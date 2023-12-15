#include"Model.h"

class BaseCharacter {
public:
	// �f�X�g���N�^

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	//���f���f�[�^�z��
	std::vector<Model*> models_;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

private:

	

};
