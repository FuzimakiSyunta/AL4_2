#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Input.h"

class FollowCamera
{
public:
	///初期化
	void Initialize();
	//更新
	void Update();
	
	void SetTarget(const WorldTransform* target) { target_ = target; };

	const ViewProjection& GetViewProjection() { return viewProjection_; };

private:
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// キーボード
	Input* input_ = nullptr;

private:
	// 追従対象
	const WorldTransform* target_ = nullptr;

};
