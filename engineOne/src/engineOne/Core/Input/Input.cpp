#include "pch.h"
#include "Input.h"

Input::Input()
{
	std::fill(m_CurrentKeyStates.begin(), m_CurrentKeyStates.end(), false);
	std::fill(m_PreviousKeyStates.begin(), m_PreviousKeyStates.end(), false);
}

 void Input::Flush() noexcept 
{
	m_PreviousKeyStates = m_CurrentKeyStates; 
	mousePosDeltaX = 0;
	mousePosDeltaY = 0;
}

void Input::OnKeyPressed(KeyCode code) noexcept
{
	m_CurrentKeyStates[enumValue(code)] = true;
}

void Input::OnKeyReleased(KeyCode code) noexcept
{
	m_CurrentKeyStates[enumValue(code)] = false;
}

void Input::OnMouseMoved(int dx, int dy) noexcept
{
	mousePosDeltaX += dx;
	mousePosDeltaY += dy;
}
