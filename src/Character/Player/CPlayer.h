#pragma once
#include "Character/Character.h"

// プレイヤークラス
class Player : public Character {

	// アクションの種類
	enum ActionType {
		P_ATTACK,
		P_SKILL,
	};

#define LV_MAX	30
#define MIN_DAMAGE	10

	// UI
	std::shared_ptr<UI> ui;

	// プレイヤーの座標
	int posX;
	int posY;

	// プレイヤーの経験値上限
	int maxExp = -1;
	// 最低ダメージ
	int minDamage = -1;

public:


	// コンストラクタ
	Player(int id, const std::string& name, int hp, int atk, int def,
		int agr, int lv, int exp, const SkillData& skill)
		: Character(id, name, hp, atk, def, agr, lv, exp, skill),
		posX(0),
		posY(0),
		maxExp(100)
	{

		minDamage = MIN_DAMAGE;

		ui =std::make_shared<UI>();
		AnimType = WAIT;
	}

	// デストラクタ
	virtual ~Player() = default;

	// フェイスハンドルのゲット関数
	int getFaceHandle() const override
	{
		return CLoad::Instance().getPfaceGrh(this->getId());
	}

	// UIへアニメーションの変更とUI上のタイプを取得
	void setAnimChange(bool arg_change) { ui->setAnimWait(arg_change); }
	bool getAnimChange() const { return ui->getAnimWait(); }

	// キャラクター座標を取得関数
	float getPosX() const { return (float)posX; }
	float getPosY() const { return (float)posY; }
	int getMaxExp() const { return maxExp; }

	std::shared_ptr<UI> getUi() const { return ui; }

	// アニメーション更新処理
	void AnimUpdata();
	// カットインスタート関数
	void StartCutin() override;
	// 経験値取得
	void AddExp(int value);
	// レベルアップ関数
	void LevelUp();
	// 行動選択関数
	void TakeAction(std::vector<std::shared_ptr<Character>>& targets, int trun = 0) override;
	// スキル
	void UseSkill(std::shared_ptr<Character>& arg_targets, SKILL_TYPE arg_type, float arg_power, int arg_trun);
	// 描画
	void Render(int arg_posX, int arg_posY, int arg_grh) override;
	// カットイン描画
	void CutinDraw() override;
	// エフェクト描画
	void EffectDraw(int arg_x, int arg_y);
	// ステータスをリセット
	void StatusReset();
};