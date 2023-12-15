#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Keisan.h"

class Enemy : public BaseCharacter {
public:
	// デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;


private:
	//WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm;
	WorldTransform worldTransformR_arm;
	Model* model_ = nullptr;
	// 3Dモデル
	Model* modelEnemy_;
	
};
