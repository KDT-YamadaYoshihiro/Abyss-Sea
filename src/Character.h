#pragma once

#include "Mouse.h"
#include "UI.h"
#include "CEffect.h"
#include "CLoad.h"

#include "CBattle_SP.h"
#include "CharacterData.h"

// キャラクター基底クラス
class Character {

protected:

	// ステータス情報
	int ID = -1;
	std::string Name = "";
	int HP = -1;
	int maxHp = -1;
	int ATK = -1;
	int DEF = -1;
	int AGR = -1;
	int Lv = 1;
	int Exp = -1;
	bool Alive = false;

	int skillID = -1;
	std::string skillName = "";
	SkillType type;
	SkillTargetType targetType;
	int skilltrun = -1;
	float skillPower = -1.0f;
	EffectType ef;
	std::string Details = "";

	BuffEffect Buff;

	// エフェクト
	std::shared_ptr<Effect> attackEffect;
	std::shared_ptr<Effect> skillEffect;

	// アイコン画像ハンドル変数
	int faceGrh = -1;
	// 行動種類(現状プレイヤーのみ使用)
	int actionChoice = -1;

	// クリック選択で使用する座標変数
	int posX = -1;
	int posY = -1;

	// エフェクトアニメーション用
	// ターゲット座標
	int targetPosX = -1;
	int targetPosY = -1;
	// ターゲットの画像ハーフサイズ
	int targetSizeW = -1;
	int targetSizeH = -1;

	// ダメージ表示用
	bool drawPower = false;
	int Power = -1;
	int framePower = -1;

	// カラー
	int r = -1;
	int g = -1;
	int b = -1;

	// フォントサイズ
	int fontSize = -1;

	// エネミー用
	bool moveCheck = false;

public:

	// 初期化
	Character(const CharacterData& data, const Skill& skilldata)
		:ID(data.ID),
		Name(data.Name),
		HP(data.HP),
		maxHp(data.HP),
		ATK(data.ATK),
		DEF(data.DEF),
		AGR(data.AGR),
		Lv(data.Lv),
		Exp(data.Exp),
		Alive(data.Alive),
		// スキル
		skillID(skilldata.ID),
		skillName(skilldata.Name),
		type(skilldata.type),
		targetType(skilldata.targetType),
		skilltrun(skilldata.trun),
		skillPower(skilldata.power),
		ef(skilldata.effectType),
		Details(skilldata.Details)
	{
		attackEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(EffectType::EF_ATTACK));
		skillEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(ef));
		actionChoice = -1;
		posX = -1;
		posY = -1;
		targetPosX = -1;
		targetPosY = -1;
		targetSizeW = -1;
		targetSizeH = -1;
		drawPower = false;
		Power = 0;
		framePower = 0;
		fontSize = 0;
		moveCheck = false;
	}

	virtual ~Character() {};

	// 各種ゲッター
	int getId() const { return ID; }
	std::string getName() const { return Name; }
	int getHp() const { return HP; }
	int getMaxHp() const { return maxHp; }
	int getAttack() const { return ATK; }
	int getDefense() const { return DEF; }
	int getSpeed() const { return AGR; }
	int getLv() const { return Lv; }
	int getExp() const { return Exp; }
	void setAlive(bool arg_alive) { Alive = arg_alive; }		// 生存判定の変更
	bool getAlive() const { return Alive; }
	int getSkillID() const { return skillID; }
	std::string getSkillName() const { return skillName; }
	SkillType getSkillType() const { return type; }
	SkillTargetType getTargetType() const { return targetType; }
	float getSkillPower() const { return skillPower; }
	EffectType getEfType() const { return ef; }
	std::string getDetails() const { return Details; }

	BuffEffect getAtkBuffEffect() const { return Buff; }
	float getAtkbuff() const { return Buff.atkMultiplier; }
	float getAgrbuff() const { return Buff.agrMultiplier; }
	float getDefbuff() const { return Buff.defMultiplier; }
	bool getDrawPower() const { return drawPower; }
	int getPower() const { return Power; }


	// アイコン画像のセット,ゲット関数
	void setFaceHandle(int handle) { faceGrh = handle; }
	int getFaceHandle() const { return faceGrh; }

	// 行動種類のセット,ゲット関数
	void setActionChoice(int arg_choice) { actionChoice = arg_choice; }
	int getActionChoice() const { return actionChoice; }

	// 座標のセット,ゲット関数
	// x
	void setPosX(int arg_posX) { posX = arg_posX; }
	int getPosX() const { return posX; }
	// y
	void setPosY(int arg_posY) { posY = arg_posY; }
	int getPosY()const { return posY; }

	// エネミー用
	bool getMoveCheck() const { return moveCheck; }
	void setMoveCheck(bool arg_moveCheck) { moveCheck = arg_moveCheck; }

	// エフェクト
	void setAttackEffect(std::shared_ptr<Effect> ef) { attackEffect = ef; }
	void setSkillEffect(std::shared_ptr<Effect> ef) { skillEffect = ef; }

	// エフェクトのポインター取得
	std::shared_ptr<Effect> getAttackEffect() const { return  attackEffect; }
	std::shared_ptr<Effect> getSkillEffect() const { return skillEffect; }

	// エフェクトのゲットセット関数
	bool hasAttackEffect() const { return  attackEffect->getEfDraw(); }
	bool hasSkillEffect() const { return skillEffect->getEfDraw(); }

	// hpの上限値まで戻す
	void hpReset() { this->HP = this->maxHp;  this->Alive = true; }

	// エフェクトアニメーションリセット
	void effectrReset() {
		attackEffect->effectInit();
		skillEffect->effectInit();
	}

	// 描画関数
	virtual void Render(int arg_posX, int arg_posY, int arg_grh) = 0;

	// 基本的な行動（攻撃）を仮想関数として定義
	virtual void takeAction(std::vector<std::shared_ptr<Character>>& targets) = 0;

	// エフェクトの再生
	// アタック
	void PlayAttackEffect(int x, int y, int sizeW, int sizeH) {
		if (!attackEffect->getEfDraw()) {
			targetPosX = x;					// 座標設定
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			attackEffect->effectStart();	// エフェクトスタート関数
		}
	}
	// スキル
	void PlaySkillEffect(int x, int y, int sizeW, int sizeH) {
		if (!skillEffect->getEfDraw()) {
			targetPosX = x;					// 座標設定
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			skillEffect->effectStart();		// エフェクトスタート関数
		}
	}

	// ダメージ表示
	void damageRenderStart(int arg_x, int arg_y, int arg_fontSize, int damage) {

		Power = damage;
		framePower = 60;
		drawPower = true;

		// カラー指定
		r = 255; g = 255; b = 0;
		// フォントサイズ
		fontSize = arg_fontSize;

		targetPosX = arg_x;
		targetPosY = arg_y;

	}
	// 回復表示
	void healRenderStart(int arg_x, int arg_y, int arg_fontSize, int heal) {

		Power = heal;
		framePower = 60;
		drawPower = true;

		// カラー指定
		r = 143; g = 195; b = 31;
		// フォントサイズ
		fontSize = arg_fontSize;
		targetPosX = arg_x;
		targetPosY = arg_y;
	}

	// スキルパワー表示
	void powerRender() {
		if (drawPower) { framePower--; }

		if (framePower <= 0) { drawPower = false; }

		if (framePower > 0) {
			SetFontSize(fontSize);
			DrawFormatString(targetPosX, targetPosY, GetColor(r, g, b), "%d", Power);
		}
	}


	// ダメージを与える
	void takeDamage(int damage) {
		// ダメージを与える。
		this->HP -= damage;
		// 死亡確認
		if (this->HP <= 0) {
			this->HP = 0;			// 0以下にならないようにする
			Alive = false;			// 生存判定をfalseにする。
		}
	}

	// 回復させる
	void takeHeal(int heal) {
		// 生存判定
		if (!Alive) { return; }

		// 回復
		this->HP += heal;
		// HPの回復上限
		if (this->HP >= this->maxHp) { 
			this->HP = this->maxHp;
		}

	}

	// バフをかける
	// 攻撃力バフ
	void takeAtkBuff(float buffPower, int turn) {
		Buff.atkMultiplier = buffPower;
		Buff.atkturn = turn;
	}

	// 速度バフ
	void takeAgrBuff(float buffPower, int turn) {
		Buff.agrMultiplier = buffPower;
		Buff.agrturn = turn;

	}

	// 防御力バフ
	void takeDefBuff(float buffPower, int turn) {
		Buff.defMultiplier = buffPower;
		Buff.defturn = turn;

	}


	// バフの更新処理
	void updateBuff() {
		if (Buff.atkturn > 0) {
			Buff.atkturn--;
			if (Buff.atkturn == 0) {
				Buff.atkMultiplier = 1.0f;
			}
		}

		if (Buff.agrturn > 0) {
			Buff.agrturn--;
			if (Buff.agrturn == 0) {
				Buff.agrMultiplier = 1.0f;
			}
		}

		if (Buff.defturn > 0) {
			Buff.defturn--;
			if (Buff.defturn == 0) {
				Buff.defturn = 1.0f;
			}
		}

	}

	// バフのリセット
	void BuffReset() {
		Buff.atkturn = 0;
		Buff.atkMultiplier = 1.0f;
		Buff.agrturn = 0;
		Buff.agrMultiplier = 1.0f;
		Buff.defturn = 0;
		Buff.defturn = 1.0f;
	}


};