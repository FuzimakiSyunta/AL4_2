#include "Player.h"
#include"Keisan.h"


void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	/*modelFighterBody_ = modelBody;
	modelFighterHead_ = modelHead;
	modelFighterL_arm = modelL_arm;
	modelFighterR_arm = modelR_arm;*/
	isHammerDraw_ = false;
	isHammerSet_ = false;
	stanbyTime = 0;

	// 初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm.Initialize();
	worldTransformR_arm.Initialize();
	// 場所の初期化
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 90.0f, 0.0f};
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
	//ハンマーの初期化
	worldTransformHammer_.Initialize();
	worldTransformHammer_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHammer_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHammer_.translation_ = {0.0f, 1.0f, 0.0f};
}

void Player::Update()
{
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();

			break;
		}
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransform_;
	worldTransformL_arm.parent_ = &worldTransform_;
	worldTransformR_arm.parent_ = &worldTransform_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

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
void Player::BehaviorRootUpdate() {
	BehaviorRootInitialize();
	XINPUT_STATE joyState;

	Vector3 move = {0.0f, 0.0f, 0.0f};

	const float kCharacterSpeed = 0.5f;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x = (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.z = (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
		move = Multiply(kCharacterSpeed, Normalize(move));
		

		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);

		move = TransformNormal(move, rotateYMatrix);

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	if (move.z != 0 || move.y != 0) {
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

	UpdateFloatingGimick();
}

void Player::Draw(const ViewProjection& viewProjection) 
{
	//3Dモデルを描画
	models_[0]->Draw(worldTransformBody_, viewProjection); 
	models_[1]->Draw(worldTransformHead_, viewProjection); 
	models_[2]->Draw(worldTransformL_arm, viewProjection); 
	models_[3]->Draw(worldTransformR_arm, viewProjection); 


	/*modelFighterBody_->Draw(worldTransformBody_, viewProjection); 
	modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	modelFighterL_arm->Draw(worldTransformL_arm, viewProjection);
	modelFighterR_arm->Draw(worldTransformR_arm, viewProjection);ff*/
}

