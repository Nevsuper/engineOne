#pragma once
#include<Windows.h>
#include<winuser.h>

inline bool isKeyPressed(int vKey)
{
	return (GetAsyncKeyState(vKey) & (short)0x8000) != 0;
}