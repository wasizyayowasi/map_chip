#include "Map.h"
#include "DxLib.h"
#include "game.h"
#include <cassert>
#include "Pad.h"
#include <iostream>
#include <fstream>

namespace {
	//�}�b�v�`�b�v1�̃T�C�Y
	constexpr int kChipSize = 32;

	//�`�b�v�̐�
	constexpr int kBgNumX = Game::kScreenWidth / kChipSize;
	constexpr int kBgNumY = Game::kScreenHeight / kChipSize;

	//���o�̓t�@�C����
	const char* const kFileName = "bin/map.bin";

}

Map::Map() :
	m_handle(-1),
	m_graphWidth(0),
	m_graphHeight(0),
	m_cursorNo(0),
	m_mapData(kBgNumX * kBgNumY, 0),
	m_scrollX(0),
	m_scrollY(0)
{
}

Map::~Map() {

}

void Map::load() {
	m_handle = LoadGraph("data/mapchip.png");
	GetGraphSize(m_handle, &m_graphWidth, &m_graphHeight);
}

void Map::unload() {
	DeleteGraph(m_handle);
}

void Map::update() {

	int indexX = m_cursorNo % kBgNumX;
	int indexY = m_cursorNo / kBgNumX;

	if (Pad::isTrigger(PAD_INPUT_1)) {
		//�w�肵���}�b�v�`�b�v�̕ύX
		if (m_mapData[m_cursorNo] < (chipNum() - 1) ){
			m_mapData[m_cursorNo]++;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_2)) {
		//�w�肵���}�b�v�`�b�v�̕ύX
		if (m_mapData[m_cursorNo] > 0) {
			m_mapData[m_cursorNo]--;
		}
	}
	if (Pad::isTrigger(PAD_INPUT_3)) {
		//�t�@�C���̏o��
		//outPutData();
		readData();
	}

	if (Pad::isPress(PAD_INPUT_UP)) {
		m_scrollY += 8;
		if (m_scrollY > Game::kScreenHeight) {
			m_scrollY -= Game::kScreenHeight;
		}
	}
	if (Pad::isPress(PAD_INPUT_DOWN)) {
		m_scrollY -= 8;
		if (m_scrollY < -Game::kScreenHeight) {
			m_scrollY += Game::kScreenHeight;
		}
	}
	if (Pad::isPress(PAD_INPUT_LEFT)) {
		m_scrollX += 8;
		if (m_scrollX > Game::kScreenWidth) {
			m_scrollX -= Game::kScreenWidth;
		}
	}
	if (Pad::isPress(PAD_INPUT_RIGHT)) {
		m_scrollX -= 8;
		if (m_scrollX < -Game::kScreenWidth) {
			m_scrollX += Game::kScreenWidth;
		}
	}
}

void Map::draw() {

	//m_scrollX > 0 �E�ɂ���Ă���
	//m_scrollX < 0 ���ɂ���Ă���
	//m_scrollY > 0 ���ɂ���Ă���
	//m_scrollY < 0 ��ɂ���Ă���
	int offsetX = m_scrollX;
	if (offsetX > 0) offsetX -= Game::kScreenWidth;
	int offsetY = m_scrollY;
	if (offsetY > 0) offsetY -= Game::kScreenHeight;
	
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			drawMap(offsetX + x * Game::kScreenWidth, offsetY + y * Game::kScreenHeight);
		}
	}

	drawCursor();
}

void Map::drawMap(int offsetX, int offsetY) {
	for (int x = 0; x < kBgNumX; x++) {
		for (int y = 0; y < kBgNumY; y++) {
			const int chipNo = m_mapData[y * kBgNumX + x];
			assert(chipNo >= 0);
			assert(chipNo < chipNum());
			int graphX = (chipNo % chipNumX()) * kChipSize;
			int graphY = (chipNo / chipNumX()) * kChipSize;

			DrawRectGraph(x * kChipSize + offsetX, y * kChipSize + offsetY,
				graphX, graphY, kChipSize, kChipSize,
				m_handle, true, false);
		}
	}
}

void Map::drawCursor() {
	int indexX = m_cursorNo % kBgNumX;
	int indexY = m_cursorNo / kBgNumX;

	int graphX = indexX * kChipSize;
	int graphY = indexY * kChipSize;

	DrawBox(graphX, graphY, graphX + kChipSize, graphY + kChipSize, GetColor(255, 0, 0), false);
}


int Map::chipNumX() {
	return (m_graphWidth / kChipSize);
}

int Map::chipNumY() {
	return (m_graphHeight / kChipSize);
}

int Map::chipNum() {
	return (chipNumX() * chipNumY());
}

void Map::outPutData() {
	std::ofstream ofs(kFileName, std::ios::binary);

	//�t�@�C���̓ǂݍ��݂Ɏ��s
	if (!ofs) {
		return;
	}
	ofs.write(reinterpret_cast<char*>(m_mapData.data()), sizeof(int) * kBgNumX * kBgNumY);

	ofs.close();
}

void Map::readData() {
	std::ifstream ifs(kFileName, std::ios::binary);

	//�t�@�C���̓ǂݍ��݂Ɏ��s
	if (!ifs) {
		return;
	}
	ifs.read(reinterpret_cast<char*>(m_mapData.data()), sizeof(int) * kBgNumX * kBgNumY);

	ifs.close();
}