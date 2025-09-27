#include"BackManager.h"

void Back_Manager::Updata(float elapsedTime)
{
	for (Back* back : backs) {
		back->Update(elapsedTime);
	}
}

void Back_Manager::Render(
	ID3D11DeviceContext* dc
)
{
	for (Back* back : backs) {
		back->Render(dc);
	}
}

void Back_Manager::Clear()
{
	for (Back* back : backs) {
		delete back;
	}
	backs.clear();
}

void Back_Manager::Register(Back* back)
{
	backs.emplace_back(back);
}