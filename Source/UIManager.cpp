#include"UIManager.h"

void UIManager::Updata(float elapsedTime)
{
	for (UI* uis : UIs) {
		uis->Update(elapsedTime);
	}
}

void UIManager::Render(ID3D11DeviceContext* dc)
{
	for (UI* uis : UIs) {
		uis->Render(dc);
	}
}

void UIManager::Clear()
{
	for (UI* uis : UIs) {
		delete uis;
	}
	UIs.clear();
}

void UIManager::Register(UI*ui)
{
	UIs.emplace_back(ui);
}