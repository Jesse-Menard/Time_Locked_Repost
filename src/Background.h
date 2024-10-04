#pragma once
#ifndef __BACKGROUND__
#define __BACKGROUND__

#include "DisplayObject.h"
class Background : public DisplayObject
{
public:
	Background(const std::string& file_path = "../Assets/backgrounds/default.jpg", std::string name = "default", bool centered = false);
	~Background();

	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
private:
	//std::string file_path;
	std::string m_name;
};

#endif // Defined __BACKGROUND__