#include "FollowCamera.h"
#include"Keisan.h"
#include"ImGuiManager.h"

void FollowCamera::Initialize() {
	// ワールドトランスフォーム
	worldTransform_.Initialize();
	// ビュープロジェクション
	viewProjection_.Initialize();

	input_ = Input::GetInstance();
}

void FollowCamera::Update()
{
	if (target_) {
		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		offset = TransformNormal(offset, MakeRotateYMatrix(viewProjection_.rotation_.y)); // 途中
		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = Add(target_->translation_, offset);
	}
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float rotate = 0.03f;
		viewProjection_.rotation_.y+= joyState.Gamepad.sThumbRX / SHRT_MAX* rotate;
	}

	// 行列の更新
	viewProjection_.UpdateMatrix();
}
