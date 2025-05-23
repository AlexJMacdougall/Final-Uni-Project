#pragma once

#include <raylib.h>

//Script class that provides a template to be overwritten by other scripts
class Script
{
public:
	Script::Script(){};
	virtual void update(float dt) = 0;
protected:
};

//Component that can be added to entities to allow scripts to be attached
class ScriptComponent
{
public:
	ScriptComponent() = default;

	void update(float dt) { if (m_Script) { m_Script->update(dt); } }; //Runs attached script's update function

	template<typename T,typename ... Args>
	void attachScript(Args&& ...args)
	{
		T* scriptPtr = new T(std::forward<Args>(args)...); //Create a pointer to a new script and pass in the variadic arguments
		m_Script = scriptPtr; //Set script to T
	}
private:
	Script* m_Script{ nullptr }; //Pointer to script object, set as nullptr until assigned
};
