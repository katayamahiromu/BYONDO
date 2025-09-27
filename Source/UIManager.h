#pragma once
#include"UI.h"
#include<vector>

class UIManager
{
public:
	UIManager(){}
	~UIManager(){}

	static UIManager& Instance()
	{
		static UIManager instance;
		return instance;
	}

	void Updata(float elapsedTime);

	void Render(ID3D11DeviceContext* dc);

	void Clear();

	void Register(UI* back);
private:
	std::vector<UI*>UIs;
};