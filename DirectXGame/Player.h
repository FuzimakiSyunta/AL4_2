#include "Model.h"
#include "WorldTransform.h"
#include<Input.h>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 自キャラ
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
	}

private:
	WorldTransform worldTransform_;
	// ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;
};