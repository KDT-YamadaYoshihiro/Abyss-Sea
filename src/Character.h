#pragma once
#include "CEffect.h"
#include "CLoad.h"

#include "CBattle_SP.h"
#include "CharacterData.h"
#include "Calculation.h"
#include "Icon.h"
#include "Cutin.h"

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

	SkillData Skill;

	BuffEffect Buff;

	// エフェクト
	std::shared_ptr<Effect> attackEffect;
	std::shared_ptr<Effect> skillEffect;

	// アイコン
	std::vector<CIcon> icons;

	// 死亡時エフェクト(エネミーの時)
	std::shared_ptr<Effect> deadEffect;

	// 計算クラス
	std::shared_ptr<Calculation> cal;

	// カットインクラス
	std::shared_ptr<Cutin> cutin;
	// 

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
	Character(int id, const std::string& name, int hp, int atk, int def,
		int agr, int lv, int exp, const SkillData& skill)
		:ID(id),
		Name(name),
		HP(hp),
		maxHp(hp),
		ATK(atk),
		DEF(def),
		AGR(agr),
		Lv(lv),
		Exp(exp),
		Alive(true),
		Skill(skill)
	{
		attackEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(EFFECT_TYPE::EF_ATTACK));
		skillEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(Skill.effectType));
		deadEffect = std::make_shared<Effect>(CLoad::Instance().getEffectGrh(EFFECT_TYPE::EF_NONE));
		cal = std::make_shared<Calculation>();
		cutin = std::make_shared<Cutin>();
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
	int getSkillID() const { return Skill.ID; }
	std::string getSkillName() const { return Skill.Name; }
	SKILL_TYPE getSkillType() const { return Skill.type; }
	SKILL_TARGET_TYPE getTargetType() const { return Skill.targetType; }
	float getSkillPower() const { return Skill.power; }
	EFFECT_TYPE getEfType() const { return Skill.effectType; }
	std::string getDetails() const { return Skill.Details; }
	//SkillData getSkill() const { return Skill; }

	BuffEffect getAtkBuffEffect() const { return Buff; }
	float getAtkbuff() const { return Buff.atkMultiplier; }
	float getAgrbuff() const { return Buff.agrMultiplier; }
	float getDefbuff() const { return Buff.defMultiplier; }
	bool getDrawPower() const { return drawPower; }
	int getPower() const { return Power; }


	// アイコン画像のセット,ゲット関数
	//void setFaceHandle(int handle) { faceGrh = handle; }
	virtual int getFaceHandle() const = 0;

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

	// カットイン終了ゲット関数
	bool getCutinEnd() const { return cutin->GetEndFlag(); }

	// エフェクト
	void setAttackEffect(std::shared_ptr<Effect> ef) { attackEffect = ef; }
	void setSkillEffect(std::shared_ptr<Effect> ef) { skillEffect = ef; }

	// エフェクトのゲット関数
	std::shared_ptr<Effect> getAttackEffect() const { return  attackEffect; }
	std::shared_ptr<Effect> getSkillEffect() const { return skillEffect; }

	// エフェクトの再生中かどうか
	bool hasAttackEffect() const { return  attackEffect->getEfDraw(); }
	bool hasSkillEffect() const { return skillEffect->getEfDraw(); }

	// hpの上限値まで戻す
	void hpReset() { this->HP = this->maxHp;  this->Alive = true; }

	// エフェクトアニメーションリセット
	void effectrReset() {
		attackEffect->effectInit();
		skillEffect->effectInit();
	}

	// カットインスタート関数
	virtual void StartCutin() = 0;
	virtual void CutinDraw() = 0;
	// 描画関数
	virtual void Render(int arg_posX, int arg_posY, int arg_grh) = 0;

	// 基本的な行動（攻撃）を仮想関数として定義
	virtual void takeAction(std::vector<std::shared_ptr<Character>>& targets) = 0;

	// エフェクトの再生
	void PlayAttackEffect(int x, int y, int sizeW, int sizeH) {
		if (!attackEffect->getEfDraw()) {
			targetPosX = x;					// 座標設定
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			attackEffect->effectStart();	// エフェクトスタート関数
		}
	}
	void PlaySkillEffect(int x, int y, int sizeW, int sizeH) {
		if (!skillEffect->getEfDraw()) {
			targetPosX = x;					// 座標設定
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			skillEffect->effectStart();		// エフェクトスタート関数
		}
	}

	// エネミー側で使用
	std::shared_ptr<Effect> getDeadEffect() const { return deadEffect; }
	bool hasDeadEffect() const { return deadEffect->getEfDraw(); }
	// エネミー死亡時のアニメーション更新
	void DeadAnimUpdate(){
		if (hasDeadEffect()) { deadEffect->sidePlayAnim(); }
	}
	// エネミー死亡時のアニメーション描画
	void DeadAnimDraw() {
		if (hasDeadEffect()) { deadEffect->effectDraw(targetPosX, targetPosY, targetSizeW, targetSizeH); }
	}
	// 死亡時アニメーションの再生関数
	void PlayDeadEffect(int x, int y, int sizeW, int sizeH) {
		if (!deadEffect->getEfDraw()) {
			targetPosX = x;					// 座標設定
			targetPosY = y;
			targetSizeW = sizeW;
			targetSizeH = sizeH;
			deadEffect->effectStart();		// エフェクトスタート関数
		}
	}

	// カットイン更新
	void CutinUpdate() 
	{
		cutin->Update();
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
	// 回復量表示
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
	// スキルパワーの表示
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

		this->HP -= damage;
		if (this->HP <= 0) {
			this->HP = 0;
			Alive = false;
		}
	}

	// 回復させる
	void takeHeal(int heal) {
		// 生存判定
		if (!Alive) { return; }

		// 回復
		this->HP += heal;
		// HPの回復上限
		if (this->HP >= this->maxHp) this->HP = this->maxHp;
	}

	// バフをかける
	// 攻撃力バフ
	void takeAtkBuff(float buffPower, int turn) {
		Buff.atkMultiplier = buffPower;
		Buff.atkturn = turn;
		addIcon(BUFF_ATK, CLoad::Instance().getIconGrh(BUFF_ATK), turn);
	}

	// 速度バフ
	void takeAgrBuff(float buffPower, int turn) {
		Buff.agrMultiplier = buffPower;
		Buff.agrturn = turn;
		addIcon(BUFF_AGR, CLoad::Instance().getIconGrh(BUFF_AGR), turn);

	}

	// 防御力バフ
	void takeDefBuff(float buffPower, int turn) {
		Buff.defMultiplier = buffPower;
		Buff.defturn = turn;
		addIcon(BUFF_DEF, CLoad::Instance().getIconGrh(BUFF_DEF), turn);

	}

	// アイコンの追加
	void addIcon(BUFF_TYPE type, int handle, int turn) {
		bool exists = false;

		for (int i = 0; i < (int)icons.size(); i++) {
			if (icons[i].getType() == type) {
				exists = true;
				break;
			}
		}

		if (!exists) {
			CIcon newIcon(type, 100, 100, 30, handle, turn);
			icons.push_back(newIcon);
		}

		// 残りターンの多い順に並べ替え
		for (int i = 0; i < (int)icons.size() - 1; i++) {
			for (int j = i + 1; j < (int)icons.size(); j++) {
				if (icons[i].getTurn() < icons[j].getTurn()) {
					CIcon tmp = icons[i];
					icons[i] = icons[j];
					icons[j] = tmp;
				}
			}
		}
	}

	// バフの更新処理
	void updateBuff() {

		bool needRecheck = false; // 削除があったらtrueにする

		if (Buff.atkturn > 0) {
			Buff.atkturn--;
			if (Buff.atkturn == 0) {
				Buff.atkMultiplier = 1.0f;
				removeIcon(BUFF_ATK);
				needRecheck = true;
			}
		}

		if (Buff.agrturn > 0) {
			Buff.agrturn--;
			if (Buff.agrturn == 0) {
				Buff.agrMultiplier = 1.0f;
				removeIcon(BUFF_AGR);
				needRecheck = true;
			}
		}

		if (Buff.defturn > 0) {
			Buff.defturn--;
			if (Buff.defturn == 0) {
				Buff.defturn = 1.0f;
				removeIcon(BUFF_DEF);
				needRecheck = true;
			}
		}

		// 各アイコンのターンも減少
		for (auto& icon : icons) {
			icon.DecreaseTurn();
		}

		// 残りターン０のアイコン削除（手動で）
		if (!icons.empty()) {
			for (int i = (int)icons.size() - 1; i >= 0; i--) {
				if (icons[i].isExpired()) {
					icons.erase(icons.begin() + i);
					needRecheck = true;
				}
			}
		}
		 //再度ターン順に並べ替え（左が多い）
		//if (needRecheck && icons.size() > 1) {
		//	for (int i = 0; i < (int)icons.size() - 1; i++) {
		//		for (int j = i + 1; j < (int)icons.size(); j++) {
		//			if (icons[i].getTurn() < icons[j].getTurn()) {
		//				std::swap(icons[i], icons[j]);
		//			}
		//		}
		//	}
		//}

		if (needRecheck && icons.size() > 1) {
			std::sort(icons.begin(), icons.end(), [](const CIcon& a, const CIcon& b) {
				return a.getTurn() > b.getTurn(); // 大きい順にソート
				});
		}
	}

	// アイコン削除
	void removeIcon(BUFF_TYPE type) {
		for (int i = 0; i < (int)icons.size(); i++) {
			if (icons[i].getType() == type) {
				icons.erase(icons.begin() + i);
				break; 
			}
		}
	}

	//　バフ・デバフアイコン描画
	void DrawIcons(int baseX, int baseY) {
		int offset = 0;
		for (int i = 0; i < (int)icons.size(); i++) {
			icons[i].setPos(baseX + offset, baseY);
			icons[i].Draw();
			offset += 27;
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
		removeIcon(BUFF_ATK);
		removeIcon(BUFF_AGR);
		removeIcon(BUFF_DEF);
	}


};