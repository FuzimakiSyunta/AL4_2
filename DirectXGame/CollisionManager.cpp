#include "CollisionManager.h"
#include "Keisan.h"

void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

void CollisionManager::AddCollider(Collider* collider) { colliders_.push_back(collider); }

void CollisionManager::ChackAllCollisions() {
	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;
		//
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// コライダーAの中心座標を取得
	Vector3 posA = colliderA->GetCenterPosition();
	// コライダーBの中心座標を取得
	Vector3 posB = colliderB->GetCenterPosition();

	// 座標の差分ベクトル
	Subtract(posA, posB);
	// 座標AとBの距離を求める
	float distance = Length(Subtract(posA, posB));
	// 玉と玉の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}
