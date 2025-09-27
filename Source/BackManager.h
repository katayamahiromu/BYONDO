#pragma once
#include"Back.h"
#include<vector>

class Back_Manager
{
public:
	Back_Manager(){}
	~Back_Manager(){}

	static Back_Manager& Instance()
	{
		static Back_Manager instance;
		return instance;
	}

	void Updata(float elapsedTime);

	void Render(ID3D11DeviceContext* dc);

	void Clear();

	void Register(Back* back);
private:
	std::vector<Back*>backs;
};