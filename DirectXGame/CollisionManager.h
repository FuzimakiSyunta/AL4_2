#pragma once
#include"Collider.h"
#include<list>
class CollisionManager {
public:
	//リセット
	void Reset();
	
	///当たり判定を行いたいオブジェクトを登録
	void AddCollider(Collider* collider);

	void ChackAllCollisions();

private:
	///コライダー二つの当たり判定
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	//コライダー
	std::list<Collider*> colliders_;

};
