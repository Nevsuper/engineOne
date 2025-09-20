#pragma once
#include "../common.h"

namespace Gui
{
	class Imgui
	{

	private:

		struct DrawData
		{
			IVec2 pos;
			IVec2 size;
			Color color;
		};
		struct State
		{
			const int displayWidth, displayHeight;
			IVec2 mousePos;
			bool isMousePressed;
			int hotItem;
			int activeItem;
		};
	};
}

