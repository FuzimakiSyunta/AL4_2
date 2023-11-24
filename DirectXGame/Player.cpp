#include "Player.h"
#include"Keisan.h"


void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	model_ = modelFighterBody_;
	model_ = modelFighterHead_;
	model_ = modelFighterL_arm;
	model_ = modelFighterR_arm;
	worldTransformBody_.Initialize();
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, -1.0f, 0.0f};
}
void Player::InitializeFloatingGimmick() { 
	floatingParamerer_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル<frame>
	const float Period = 120.0f;
	//1フレームでのパラメータ加算値
	const float Step = 2.0f * M_PI / Period;
	//パラメータを1ステップ分加算
	floatingParamerer_ += Step;
	//2πwを超えたら0に戻す
	floatingParamerer_ = std::fmod(floatingParamerer_,2.0f*M_PI);
	//浮遊の振幅
	const float Amplitude = 10;
	//浮遊を座標に反映
	
}

void Player::Update()
{
	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0,joyState))
	{
		//速さ
		const float speed = 0.3f;
		//移動量
		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};
		//移動量に速さを反映
		move = VectorMultiply(speed, Normalize(move));
		//移動
		worldTransform_.translation_=Add(worldTransform_.translation_,move);
	    // カメラの角度から回転行列を計算する
		 Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		// カメラとリンク
		move = TransformNormal(move, MakeRotateYMatrix(viewProjection_->rotation_.y));
	
		if (move.z != 0 || move.y != 0) {
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		}

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	};

	// 行列の更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) 
{
	model_->Draw(worldTransform_, viewProjection); 
}

