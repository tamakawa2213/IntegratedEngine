#include "GameTime.h"
#include "../Graphics/imgui.h"

namespace
{
	float RelativeTime = 1.0f;
	float NumberOfUpdate = 0;	//現在フレームの残り更新回数
	bool Stop = false;
}

namespace GameTime
{
	void Update()
	{
#if _DEBUG
		ImGui::Begin("GameTime");
		ImGui::SliderFloat("RelativeTime", &RelativeTime, 1.0f, 10.0f);

		if (Stop)
		{
			if (ImGui::Button("Start"))
			{
				Stop = false;
			}
		}
		else
		{
			if (ImGui::Button("Stop"))
			{
				Stop = true;
			}
		}
		ImGui::End();
#endif

		if(!Stop)
		NumberOfUpdate += RelativeTime;
	}

	float GetRelativeTime()
	{
		return RelativeTime;
	}

	int RemainingTimes()
	{
		return (int)NumberOfUpdate;
	}

	void UpDated()
	{
		NumberOfUpdate -= 1.0f;
	}
	bool IsStop()
	{
		return Stop;
	}
	void TimeStop()
	{
		Stop = true;
	}
	void TimeStart()
	{
		Stop = false;
	}
}