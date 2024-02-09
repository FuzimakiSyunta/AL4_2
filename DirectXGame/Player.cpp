﻿#include "Player.h"
#include"Keisan.h"
#include"ImGuiManager.h"


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
	worldTransformHammer_.Initialize();
	// 場所の初期化
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
	//ハンマーの初期化
	worldTransformHammer_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransformHammer_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformHammer_.translation_ = {0.0f, 1.0f, 0.0f};
	//カウンターの初期化
	Count_ = 0;
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
		case Behavior::kJump:
			BehaviorJumpInitialize();
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
	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;
	}

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformR_arm.parent_ = &worldTransformBody_;
	worldTransformL_arm.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	BaseCharacter::Update();

	// 行列の更新
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm.UpdateMatrix();
	worldTransformR_arm.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();

	// デバック
	float playerRot[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};
	float playerPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	//// 画面の座標を表示
	//ImGui::Begin("Player");
	//ImGui::SliderFloat3("playerRot", playerRot, -28.0f, 28.0f);
	//ImGui::SliderFloat3("playerPos", playerPos, -28.0f, 28.0f);
	//ImGui::Text("%d\n", behaviorRequest_);
	//ImGui::Text("%d\n", Count_);
	//ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	worldTransform_.rotation_.x = playerRot[0];
	worldTransform_.rotation_.y = playerRot[1];
	worldTransform_.rotation_.z = playerRot[2];
}

void Player::InitializeFloatingGimmick() { 
	floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	//浮遊移動のサイクル<frame>
	const float Period = 120.0f;
	//1フレームでのパラメータ加算値
	const float Step = 2.0f * (float)M_PI / (float)Period;
	//パラメータを1ステップ分加算
	floatingParameter_ += Step;
	//2πwを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_,2.0f*(float)M_PI);
	//浮遊の振幅
	const float Amplitude = 0.1f;
	//浮遊を座標に反映
	worldTransform_.translation_.y = std::sin(floatingParameter_) * Amplitude;
	const float amplitube = 0.1f;
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitube;
	worldTransformR_arm.rotation_.x = std::sin(floatingParameter_) * amplitube;
	worldTransformL_arm.rotation_.x = std::sin(floatingParameter_) * amplitube;
}
void Player::BehaviorRootUpdate() {
	//BehaviorRootInitialize();
	// 基底クラスの更新
	BaseCharacter::Update();
	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 速さ
		const float speed = 0.3f;

		// 移動量
		velocity_ = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * speed, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * speed};
		// 移動量に速さを反映
		velocity_ = VectorMultiply(speed, Normalize(velocity_));

		velocity_ = TransformNormal(velocity_, MakeRotateYMatrix(viewProjection_->rotation_.y));

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

		if (velocity_.z != 0 || velocity_.y != 0) {
			worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
		}

	}

	//if (Input::GetInstance()->GetJoystickState(0, joyState)) {
	//	if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_B) {
	//		behaviorRequest_ = Behavior::kAttack;
	//	}
	//	/*if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
	//		behaviorRequest_ = Behavior::kJump;
	//	}*/
	//}
		

	UpdateFloatingGimmick();
}

void Player::BehaviorAttackUpdate() {

	worldTransformL_arm.rotation_.x = worldTransformHammer_.rotation_.x + 3.08f;
	worldTransformR_arm.rotation_.x = worldTransformHammer_.rotation_.x + 3.08f;
	if (isHammerSet_ == false) {
		if (worldTransformHammer_.rotation_.x >= -0.4f) {
			worldTransformHammer_.rotation_.x -= 0.05f;
		}
		if (worldTransformHammer_.rotation_.x <= -0.4f) {
			worldTransformHammer_.rotation_.x = -0.4f;
			stanbyTime++;
			if (stanbyTime >= 12) {
				isHammerSet_ = true;
				stanbyTime = 0;
			}
		}
	} else if (isHammerSet_ == true) {
		if (worldTransformHammer_.rotation_.x <= 1.6f) {
			worldTransformHammer_.rotation_.x += 0.1f;
		}
		if (worldTransformHammer_.rotation_.x >= 1.6f) {
			worldTransformHammer_.rotation_.x = 1.6f;
			stanbyTime++;
			if (stanbyTime >= 12) {
				isHammerSet_ = false;
				behaviorRequest_ = Behavior::kRoot;
				stanbyTime = 0;
			}
		}
	}
}


void Player::BehaviorRootInitialize() {
	isHammerDraw_ = false;
	worldTransformHammer_.rotation_.x = 0.0f;
	
}

void Player::BehaviorAttackInitialize() { isHammerDraw_ = true; }

void Player::BehaviorJumpInitialize() { 
	worldTransformBody_.translation_.y = 0;
	worldTransformL_arm.rotation_.x = 0;
	worldTransformR_arm.rotation_.x = 0;
	//ジャンプ初速
	const float kJumpFirstSpeed = 1.0f;
	//ジャンプ初速を与える
	velocity_.y = kJumpFirstSpeed;

}
void Player::OnCollision() { 
	Count_++;
	
	behaviorRequest_ = Behavior::kJump; 
}

void Player::BehaviorJumpUpdate() { 
	// 移動
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// 重力加速度
	const float kGravityAcceleration = 0.08f;
	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};
	// 加速する
	velocity_ = Add(velocity_, accelerationVector);
	// 着地
	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0;
		// ジャンプ終了
		behaviorRequest_ = Behavior::kRoot;
	}
}
Vector3 Player::GetCenterPosition() const {
	//ローカル座標のオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	//ワールド座標変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

void Player::Draw(const ViewProjection& viewProjection) 
{
	//3Dモデルを描画
	models_[0]->Draw(worldTransformBody_, viewProjection); 
	models_[1]->Draw(worldTransformHead_, viewProjection); 
	models_[2]->Draw(worldTransformL_arm, viewProjection); 
	models_[3]->Draw(worldTransformR_arm, viewProjection); 
	if (isHammerDraw_ == true) {
		models_[4]->Draw(worldTransformHammer_, viewProjection);
	}

	/*modelFighterBody_->Draw(worldTransformBody_, viewProjection); 
	modelFighterHead_->Draw(worldTransformHead_, viewProjection);
	modelFighterL_arm->Draw(worldTransformL_arm, viewProjection);
	modelFighterR_arm->Draw(worldTransformR_arm, viewProjection);ff*/
}


