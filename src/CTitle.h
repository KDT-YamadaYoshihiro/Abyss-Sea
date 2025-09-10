#pragma once
#include "Manager.h"
#include "Character.h"
#include "CharacterFactory.h"


// タイトルスクリーンクラス
class CTitle : public ScreenBase {

#define TWO	2


	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	std::shared_ptr<UI> ui[TWO];

	float posX;
	float posY;
	float sway;

	int bgPosX[TWO];
	int bgPosY[TWO];

public:

	void Update() override;

	void Render() override;

	CTitle() {

		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		for (int i = 0; i < TWO; i++) {
			ui[i] = std::make_shared<UI>();
			
			bgPosX[0] = 0;
			bgPosY[0] = 0;

			bgPosX[1] = WINDOW_W;
			bgPosY[1] = 0;
		}

		posX = WINDOW_W / 2.0f;
		posY = 600.0f;
		sway = 1.0f;


	};

	virtual ~CTitle();

};