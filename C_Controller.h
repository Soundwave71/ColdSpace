#pragma once
#include "C_Base.h"

class C_Controller : public C_Base{
public:
	C_Controller() : C_Base(Component::Controller){}
	void ReadIn(std::stringstream& l_stream){
		l_stream >> m_controlType;
	}
	unsigned int GetControlType(){return m_controlType;};
private:
	unsigned int m_controlType;
};