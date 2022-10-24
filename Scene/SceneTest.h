#pragma once
#include "SceneBase.h"
#include "Map.h"

class SceneTest : public SceneBase
{
public:
	SceneTest() 
	{
	}
	virtual ~SceneTest() {}


	virtual void init() override;
	virtual void end() override;

	virtual SceneBase* update() override;
	virtual void draw() override;

private:
	
	Map m_map;

};