#include <stdlib.h>
#include "GameParameter.h"

void GameParameter::Update(float elapsedTime)
{
	if (cameraShakeFrame > 0)
	{
		cameraShakeFrame--;
#if 0
		cameraShakeOffset.x = (rand() % 3 - 1) * cameraShakeRange * cameraShakeFrame;
		cameraShakeOffset.y = (rand() % 3 - 1) * cameraShakeRange * cameraShakeFrame;
		cameraShakeOffset.z = (rand() % 3 - 1) * cameraShakeRange * cameraShakeFrame;
#else
		cameraShakeOffset.x = cameraShakeRange * cameraShakeFrame;
		if (cameraShakeFrame % 2) cameraShakeOffset.x = -cameraShakeOffset.x;
#endif
	}

}
