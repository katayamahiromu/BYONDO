#include"Character.h"
#include"Mathf.h"
#include "Graphics/Graphics.h"

//更新行列
void Character::UpdateTransform() {
	//スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列を作成

	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	//位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	//計算したワールドを取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

//移動処理
void Character::Move(float vx, float vz, float speed) {
	//横方向ベクトルを設定
	moveVecX = vx;
	moveVecZ = vz;
	//最大速度設定
	maxMoveSpeed = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;
	//上入力がない場合は水平に戻す
	if (vx == 0.0f)
	{
		if (angle.z == 0.0f)return;

		if (angle.z > 0.0f)
		{
			angle.z += DirectX::XMConvertToRadians(80) * speed;
			if (angle.z < 0.0f) {
				angle.z = 0.0f;
			}
		}

		if (angle.z < 0.0f)
		{
			angle.z -= DirectX::XMConvertToRadians(80) * speed;
			if (angle.z > 0.0f) {
				angle.z = 0.0f;
			}
		}

		return;
	}
	float lenth = sqrtf(vx * vx + vz * vz);

	//進行ベクトルを単位ベクトル化
	vx /= lenth;
	vz /= lenth;

	//自身の回転値から前方向を求める
	float frontX = sinf(angle.z);
	float frontZ = cosf(angle.z);

	//回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (vx * frontX) + (vz * frontZ); //内積

	//内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	//小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot; //補正値
	if (rot > speed) rot = speed;

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (vz * frontX) - (vx * frontZ);

	//2Dの外積値が正の場合か負の場合によって左右判定を行える
	//左右判定を行う事によって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.z += rot;
	}
	else
	{
		angle.z -= rot;
	}

	//回転の制御
	if (angle.z > DirectX::XMConvertToRadians(40))
	{
		angle.z = DirectX::XMConvertToRadians(40);
	}

	if (angle.z < DirectX::XMConvertToRadians(-40))
	{
		angle.z = DirectX::XMConvertToRadians(-40);
	}
	
}

//ジャンプ処理
void Character::Junp(float speed) {
	//上方向の力を設定
	velocity.y = speed;
}

//衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//速力を力に加える
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime) {
	//経過フレーム
	float elapsedFrame = 60.0f * elapsedTime;

	//垂直速力更新処理
	//UpdateVerticalVelocity(elapsedFrame);
	//水平速力更新処理
	UpdateHorizontalVelocity(elapsedFrame);
	//垂直移動更新処理
	//UpdateVerticalMove(elapsedTime);
	//水平移動更新処理
	UpdateHorizontalMove(elapsedTime);
}

bool Character::ApplyDamage(float damage,float invincibleTime)
{
	//ダメージが０の場合は健康状態を変更する必要がない
	if (damage == 0.0f) return false;
	//死亡している場合は健康状態を変更しない
	if (health <= 0) return false;
	//ダメージ処理
	if (invincibleTime > 0.0f) {
		health -= damage;
		//死亡通知
		if (health <= 0.0f) {
			OnDead();
		}
		//ダメージ通知
		else {
			OnDamaged();
			invicibleTime = invincibleTime;
		}
	}
	//健康状態が変更した場合はtrueを返す
	return true;
}

void Character::UpdateInvinciblTImer(float elapsedTime)
{
	if (invicibleTime > 0.0f) {
		invicibleTime -= elapsedTime;
	}
}

// 死亡判定
void Character::CheckDeath()
{
	// スクリーンの描画範囲から大きく離れていたら殺す
	//if (screenPosition.x <= -100.0f || screenPosition.x >= Graphics::Instance().GetScreenWidth() + 100.0f
	//	|| screenPosition.y <= -100.0f || screenPosition.y >= Graphics::Instance().GetScreenHeight() + 100.0f)
	//{
	//	OnDead();
	//}

	if (position.x <= -100.0f || position.x >= 70.0f
		|| position.z <= -100.0f || position.z >= 100.0f)
	{
		OnDead();
	}
}

// 描画判定
void Character::CheckDraw()
{
	// スクリーンの描画範囲から出ていれば消す
	//if (screenPosition.x <= 140 + radius * 2 || screenPosition.x >= 860 - radius * 2
	//	|| screenPosition.y <= -100.0f || screenPosition.y >= Graphics::Instance().GetScreenHeight() + 100.0f)
	//{
	//	isDraw = false;
	//}
	//else isDraw = true;

	//if (position.x <= -52.0f + radius || position.x >= 21.0f - radius
	//	|| position.z <= -50.0f || position.z >= 50.0f)
	//{
	//	isDraw = false;
	//}
	//else isDraw = true;

	isDraw = true;
}

// 行動範囲制限
bool Character::MoveLimit(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	DirectX::XMFLOAT2 screenPosition;
	screenPosition = ToScreenCoordinate(dc, view, projection);

	if (screenPosition.y < 0.0f)
	{
		move_limit = MOVE_LIMIT::UP;
		return false;
	}
	
	if (screenPosition.y > 720.0f)
	{
		move_limit = MOVE_LIMIT::DAWN;
		return false;
	}
	if (screenPosition.x > 800.0f)
	{
		move_limit = MOVE_LIMIT::RIGHT;
		return false;
	}

	if (screenPosition.x < 100.0f)
	{
		move_limit = MOVE_LIMIT::LEFT;
		return false;
	}

	move_limit = MOVE_LIMIT::NONE;
	return true;
}


DirectX::XMFLOAT2 Character::ToScreenCoordinate(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// ワールド座標からスクリーン座標へ変換
	DirectX::XMFLOAT3 wP = position;
	wP.y += height;
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&wP);
	float ViewportX = viewport.TopLeftX;
	float ViewportY = viewport.TopLeftY;
	float ViewportWidth = viewport.Width;
	float ViewportHeight = viewport.Height;
	float ViewportMinZ = 0.0f;
	float ViewportMaxZ = 1.0f;

	DirectX::XMVECTOR ScreenPosition;
	ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,
		ViewportX,
		ViewportY,
		ViewportWidth,
		ViewportHeight,
		ViewportMinZ,
		ViewportMaxZ,
		Projection,
		View,
		World);

	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	return screenPosition;
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ平面の速力を減速する
	float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
	if (length > 0.0f) 
	{
		//摩擦力
		float friction = this->friction * elapsedFrame;
		//空中にいるときは摩擦力を減らす
		//if (!IsGround()) friction -= this->airControl;

		//摩擦による横方向の減速処理
		if(length > friction)
		{
			float vx = velocity.x / length;
			float vz = velocity.z / length;
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		//横方向の速力が摩擦力以下になったので速力を無効化
		else
		{
			velocity.x = 0;
			velocity.z = 0;
		}
	}

	//XZ平面の速力を加速する
	if (length <= maxMoveSpeed)
	{
		//移動ベクトルがゼロベクトルでないなら加速する
		float moveVecLength = sqrtf((moveVecX * moveVecX) + (moveVecZ * moveVecZ));
		if(moveVecLength > 0.0f)
		{
			//加速力
			float acceleration = this->acceleration * elapsedFrame;
			////空中にいるときは加速力を減らす
			//if (!IsGround()) acceleration -= this->airControl;
			////移動ベクトルによる加速処理
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			//最大速度制限
			float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
			if (length > maxMoveSpeed)
			{
				velocity.x *= maxMoveSpeed / length;
				velocity.z *= maxMoveSpeed / length;
			}
		}

		////下りガタガタしないようにする
		//if (IsGround() && slopeRate > 0.0f)
		//{
		//	velocity.y -= length * slopeRate;
		//}
	}
	//移動ベクトルをリセット
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

//水平移動更新処理
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//移動処理
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;
}