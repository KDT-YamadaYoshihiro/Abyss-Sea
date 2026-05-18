#pragma once
#include "Scene/Manager/Manager.h"
#include "Character/Character.h"
#include "Character/Factory/CharacterFactory.h"

// タイトルスクリーンクラス
class CTitle : public ScreenBase {

	// UIの数
#define UI_MAX		2

	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	std::shared_ptr<UI> ui[UI_MAX];

	std::shared_ptr<FadeAlpha> fade;

	float posX = -1.0f;
	float posY = -1.0f;
	float sway = -1.0f;

	int bgPosX[UI_MAX];
	int bgPosY[UI_MAX];

public:

	void Update() override;

	void Render() override;

	CTitle():
		bgm(nullptr),
		se(nullptr),
		fade(nullptr),
		posX(WINDOW_W / 2.0f),
		posY(600.0f),
		sway(1.0f)
	{

		bgm = ScreenManager::Instance().getbgm();
		se = ScreenManager::Instance().getSe();
		fade = ScreenManager::Instance().getFade();

		for (int i = 0; i < UI_MAX; i++) {
			ui[i] = std::make_shared<UI>();
			
			bgPosX[0] = 0;
			bgPosY[0] = 0;

			bgPosX[1] = WINDOW_W;
			bgPosY[1] = 0;
		}


	};

	virtual ~CTitle();

};