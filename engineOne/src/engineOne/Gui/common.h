#pragma once

namespace Gui
{
	struct IVec2
	{
		union
		{
			struct{	int x, y; };
			struct{	int u, v; };
			int compnents[2];
		};
	};

	struct Color
	{
		union
		{
			struct { float r, g, b, a; };
			struct { float x, y, z, w; };
			float compnents[4];
		};
	};
	
}