#pragma once
#include"keyCodes.h"
#include<engineOne/utils/utils.h>
#include<array>
class Window;
class Input
{
	friend class Window;
public:
	Input();


	bool IsKeyDown(KeyCode code) const noexcept { return m_CurrentKeyStates[enumValue(code)]; }
	bool IsKeyPressed(KeyCode code) const noexcept {return m_CurrentKeyStates[enumValue(code)] && !m_PreviousKeyStates[enumValue(code)];
	}

	bool IsKeyUp(KeyCode code) const noexcept { return !m_CurrentKeyStates[enumValue(code)]; }
	bool IsKeyReleased(KeyCode code) const noexcept {return !m_CurrentKeyStates[enumValue(code)] && m_PreviousKeyStates[enumValue(code)];
	}

	void Flush() noexcept;

	int GetMouseMoveDeltaX() const noexcept	{	return mousePosDeltaX;	}
	int GetMouseMoveDeltaY() const noexcept	{	return mousePosDeltaY;	}
	
private:
	void OnKeyPressed(KeyCode code) noexcept;
	void OnKeyReleased(KeyCode code) noexcept;
	void OnMouseMoved(int dx, int dy) noexcept;
public:
	inline static constexpr size_t s_NumKeys = 256u;
private:
	std::array<bool, s_NumKeys> m_CurrentKeyStates;
	std::array<bool, s_NumKeys> m_PreviousKeyStates;
	int mousePosDeltaX, mousePosDeltaY;
};



