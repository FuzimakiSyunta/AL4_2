#include "Player.h"
#include"Keisan.h"


void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {

	modelFighterBody_ = modelBody;
	modelFighterHead_ = modelHead;
	modelFighterL_arm = modelL_arm;
	modelFighterR_arm = modelR_arm;

	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm.Initialize();
	worldTransformR_arm.Initialize();
	// 初期化
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	//体の初期化
	worldTransformBody_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformBody_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformBody_.translation_ = {0.0f, 0.0f, 0.0f};
	//頭の初期化
	worldTransformHead_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHead_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 1.5f, 0.0f};
	//左腕の初期化
	worldTransformL_arm.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformL_arm.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformL_arm.translation_ = {-0.5f, 1.25f, 0.0f};
	// 右腕の初期化
	worldTransformR_arm.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformR_arm.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_arm.translation_ = {0.5f, 1.25f, 0.0f};
}

void Player::Update()
{
	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0,joyState))
	{
		//速さ
		const float speed = 0.3f;
		worldTransformBody_.parent_ = &worldTransform_;
		worldTransformHead_.parent_ = &worldTransform_;
		worldTransformL_arm.parent_ = &worldTransform_;
		worldTransformR_arm.parent_ = &worldTransform_;
		//移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};
		//移動量に速さを反映
		move = VectorMultiply(speed, Normalize(move));
		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	
	    // カメラの角度から回転行列を計算する
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		// カメラとリンク
		move = TransformNormal(move, MakeRotateYMatrix(viewProjection_->rotation_.y));
	
		if (move.z != 0 || move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}
	};

	UpdateFloatingGimmick();

	// 行列の更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm.UpdateMatrix();
	worldTransformR_arm.UpdateMatrix();
}

void Player::InitializeFloatingGimmick() { 
	floatingParamerer_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル<frame>
	const float Period = 120.0f;
	//1フレームでのパラメータ加算値
	const float Step = 2.0f * (float)M_PI / (float)Period;
	//パラメータを1ステップ分加算
	floatingParamerer_ += Step;
	//2πwを超えたら0に戻す
	floatingParamerer_ = std::fmod(floatingParamerer_,2.0f*(float)M_PI);
	//浮遊の振幅
	const float Amplitude = 1.2f;
	//浮遊を座標に反映
	worldTransform_.translation_.y = std::sin(floatingParamerer_) * Amplitude;
}
void Player::Draw(ViewProjection& viewProjection) 
{
	modelFighterBody_->Draw(worldTransformBody_, viewProjection); 
	modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	modelFighterL_arm->Draw(worldTransformL_arm, viewProjection);
	modelFighterR_arm->Draw(worldTransformR_arm, viewProjection);
}

