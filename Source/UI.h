#pragma once
#include"Graphics/Sprite.h"

class UI
{
public:
	virtual ~UI() {}
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(ID3D11DeviceContext* dc) = 0;
};

class  HP_Gauge:public UI
{
public:
	HP_Gauge();
	~HP_Gauge();

	void Initialize();

	static HP_Gauge& Instance()
	{
		static HP_Gauge instance;
		return instance;
	}

	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc);
private:
	Sprite* HP;
	float tex_odd = 0;
};

class Bomb_Gauge :public UI
{
public:
	Bomb_Gauge();
	~Bomb_Gauge();

	static Bomb_Gauge& Instance()
	{
		static Bomb_Gauge instance;
		return instance;
	}
	
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc);
	void Use_Update(float elapsedTime);
	bool Get_Flag() { return bomb_flag; }
	void Use_Flag() { use = true; }
private:
	Sprite* bomb;
	float speed = 10;
	float angle = 0;
	bool bomb_flag = false;
	bool use = false;
};

class Handl :public UI
{
public:
	static Handl& Instance()
	{
		static Handl instance;
		return instance;
	}
	Handl();
	~Handl();
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc);
private:
	Sprite* handle;
	float angle = 0;
};

