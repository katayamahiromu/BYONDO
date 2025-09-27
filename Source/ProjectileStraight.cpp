#include"ProjectileStraight.h"

//コンストラクタ
ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	//model = new Model("Data/Model/Sword/Sword.mdl");
	//表示サイズを調整
	//scale.x = scale.y = scale.z = 0.5f;
	//scale.x = scale.y = scale.z = 3.0f;
	int i = 0;
}

//デストラクタ
ProjectileStraight::~ProjectileStraight() {
	delete model;
}

//更新処理
void ProjectileStraight::Update(float elapsedTime)
{
	//寿命処理
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0) {
		//自分を削除
		Destroy();
	}
	if (position.z > destoroy_pos)
	{
		Destroy();
	}
	//水平移動処理
	UpdateHorizontalMove(elapsedTime);

	//オブジェクト行列を更新
	UpdateTransform();
	//モデル行列更新
	model->UpdateTransform(transform);
}

//描画処理
void ProjectileStraight::Render(ID3D11DeviceContext*dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}

void ProjectileStraight::UpdateHorizontalMove(float elapsedTime)
{
	float speed = this->speed * elapsedTime;
	float mx = direction.x * speed;
	float mz = direction.z * speed;

	//DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
	//DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };
	//HitResult hit;
	//if (StageManager::Instance().RayCast(start,end,hit)) {

	//	//反射ベクトル
	//	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

	//	//壁までのベクトル
	//	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
	//	DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
	//	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
	//	//壁の法線
	//	DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

	//	//入射ベクトルを法線に射影
	//	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

	//	//補正位置の計算
	//	float distance = 0;
	//	DirectX::XMStoreFloat(&distance, Dot);

	//	// 反射ベクトルの計算
	//	DirectX::XMVECTOR Reflected = DirectX::XMVectorSubtract(Vec, DirectX::XMVectorScale(Normal, 2.0f * distance));

	//	//法線の大きさを距離に合わせる
	//	DirectX::XMVECTOR R = DirectX::XMVectorAdd(Vec, DirectX::XMVectorScale(Dot,2));
	//	DirectX::XMFLOAT3 r; 
	//	DirectX::XMStoreFloat3(&r, R);

	//	Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(R,speed));
	//	DirectX::XMStoreFloat3(&position, Position);
	//	this->direction = DirectX::XMFLOAT3(DirectX::XMVectorGetX(Reflected), 0.0f, DirectX::XMVectorGetZ(Reflected));
	//}
	//else
	//{
		//移動
	//}
	position.x += mx;
	position.z += mz;
}

// プレイヤー青(ミライ)
PBlueProjectileStraight::PBlueProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/tama_ao/tama_ao.mdl");
	scale.x = scale.y = scale.z = 0.5f;
}

// プレイヤー赤(カコ)
PRedProjectileStraight::PRedProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/tama_aka/tama_aka.mdl");
	scale.x = scale.y = scale.z = 0.5f;
}

// 敵
EProjectileStraight::EProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/teki_tama/teki_tama.mdl");
	//model = new Model("Data/Model/tama_ao/tama_ao.mdl");
	scale.x = scale.y = scale.z = 0.7f;	
}
