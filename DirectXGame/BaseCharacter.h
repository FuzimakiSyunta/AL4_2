#pragma once
#include"Model.h"
#include"Collider.h"

class BaseCharacter : public Collider{
public:
	// デストラクタ

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	virtual Vector3 GetCenterPosition() const override;

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;
};
