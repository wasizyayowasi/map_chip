#include "DxLib.h"
#include "game.h"
#include "SceneTest.h"


namespace
{
}

void SceneTest::init()
{
	m_map.load();
}

void SceneTest::end()
{
	m_map.unload();
}

SceneBase* SceneTest::update()
{
	m_map.update();

	return this;
}

void SceneTest::draw()
{
	m_map.draw();

	DrawString(0, 0, "test", GetColor(255, 255, 255));
}

