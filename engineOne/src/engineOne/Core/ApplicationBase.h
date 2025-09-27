#pragma once
#include"Window.h"
#include <memory>
#include "Input/Input.h"


class Engine;

class ApplicationBase
{
	friend class Engine;
public:
	
	ApplicationBase(const std::string& appName) noexcept : m_ApplicationName(appName),m_pInput(nullptr),m_Aspect(1.3333f) {};

	ApplicationBase(const ApplicationBase&) = delete;
	ApplicationBase& operator=(const ApplicationBase&) = delete;

	virtual ~ApplicationBase() noexcept = default;


	virtual bool Init() noexcept = 0;

	virtual void Update(float deltaTime) noexcept = 0;
	virtual void OnRender() noexcept = 0;

	const std::string& getName() const noexcept {return m_ApplicationName;}
protected:
	//in the order of initialization and reverse order of destruction
	std::string m_ApplicationName;
	Input* m_pInput;
	float m_Aspect;
	Engine* m_pEngine;
};

