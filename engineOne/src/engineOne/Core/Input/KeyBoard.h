#pragma once
#include<bitset>
class Window;

class KeyBoard
{
public:
	KeyBoard() {};
public:
	inline static constexpr	size_t numKeys = sizeof(uint8_t);
private:
	bool m_KeyStates[numKeys]
};

