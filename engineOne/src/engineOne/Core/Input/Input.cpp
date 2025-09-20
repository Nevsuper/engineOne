#include "pch.h"
#include "Input.h"

Input::Input()
{
	std::fill(m_CurrentKeyStates.begin(), m_CurrentKeyStates.end(), false);
	std::fill(m_PreviousKeyStates.begin(), m_PreviousKeyStates.end(), false);
}

void Input::OnKeyPressed(KeyCode code) noexcept
{
	m_CurrentKeyStates[enumValue(code)] = true;
	std::cout << "OnkeyPressed "<< std::endl;
}

void Input::OnKeyReleased(KeyCode code) noexcept
{
	m_CurrentKeyStates[enumValue(code)] = false;
	std::cout << "OnKeyReleased " << std::endl;
}
