#pragma once
#include"keyCodes.h"
#include<engineOne/utils/utils.h>

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

	void Flush() noexcept { m_PreviousKeyStates = m_CurrentKeyStates; }
private:
	void OnKeyPressed(KeyCode code) noexcept;
	void OnKeyReleased(KeyCode code) noexcept;
public:
	inline static constexpr size_t s_NumKeys = 256u;
private:
	std::array<bool, s_NumKeys> m_CurrentKeyStates;
	std::array<bool, s_NumKeys> m_PreviousKeyStates;
};



