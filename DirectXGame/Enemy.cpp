#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	// 基底クラスの初期化
	BaseCharacter::Initialize(models);
	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformL_arm.Initialize();
	worldTransformR_arm.Initialize();
	// 場所の初期化
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 10.0f};
	// 体の初期化
	worldTransformBody_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformBody_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
	// 左腕の初期化
	worldTransformL_arm.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformL_arm.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformL_arm.translation_ = {-0.5f, 1.25f, 0.0f};
	// 右腕の初期化
	worldTransformR_arm.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformR_arm.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_arm.translation_ = {0.5f, 1.25f, 0.0f};
}

void Enemy::Update() {
	// 基底クラスの更新
	BaseCharacter::Update();

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm.parent_ = &worldTransform_;
	worldTransformR_arm.parent_ = &worldTransform_;
	// 速度
	Vector3 move = {0, 0, 1};
	
	move = TransformNormal(move,worldTransform_.matWorld_);

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.rotation_.y += 0.05f;

	// 行列の更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm.UpdateMatrix();
	worldTransformR_arm.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	// 3Dモデルを描画
	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformL_arm, viewProjection);
	models_[2]->Draw(worldTransformR_arm, viewProjection);
}

Vector3 Enemy::GetCenterPosition() const { 
	//ローカル座標のオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	// ワールド座標変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}
