#include "Model.h"
#include "WorldTransform.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include<Input.h>
#include "BaseCharacter.h"

class Player:public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models)override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform() {
		return worldTransform_;
	};

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();
	//浮遊ギミック更新
	void UpdateFloatingGimmick();

private:
	WorldTransform worldTransform_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm;
	WorldTransform worldTransformR_arm;
	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
	//3Dモデル
	Model* modelFighterBody_;
	Model* modelFighterHead_;
	Model* modelFighterL_arm;
	Model* modelFighterR_arm;
	//浮遊ギミック媒介変数変数
	float floatingParamerer_ = 0.0f;
	//周期

};