#pragma once
#include "Character.h"
#include "UI.h"

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

	// アニメーション
	GRTYPE AnimType;

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

	// アニメーションタイプのゲット、セット
	void setAnimType(GRTYPE arg_type) { AnimType = arg_type; }
	GRTYPE getAnimType() const { return AnimType; }

	// UIへアニメーションの変更とUI上のタイプを取得
	void setAnimChange(bool arg_change) { ui->setAnimWait(arg_change); }
	bool getAnimChange() const { return ui->getAnimWait(); }

	// キャラクター座標を取得関数
	float getPosX() const { return (float)posX; }
	float getPosY() const { return (float)posY; }
	int getMaxExp() const { return maxExp; }


	std::shared_ptr<UI> getUi() const { return ui; }

	// アニメーション更新処理
	void AnimUpdata() {

		ui->PlayerAnim(AnimType);

		switch (Skill.effectType)
		{
		case EffectType::EF_NONE:
			break;
		case EffectType::EF_ATTACK:
			break;
		case EffectType::EF_SKILL1:
			if (hasSkillEffect()) {skillEffect->lengthPlayerAnim();}
			break;
		case EffectType::EF_SKILL2:
			if (hasSkillEffect()) { skillEffect->PlayAnim(); }
			break;
		case EffectType::EF_ALL_SKILL1:
			if (hasSkillEffect()) { skillEffect->PlayAnim(); }
			break;
		case EffectType::EF_ALL_SKILL2:
			if (hasSkillEffect()) { skillEffect->PlayAnim(); }
			break;
		case EffectType::EF_HEAL:
			if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
			break;
		case EffectType::EF_BUFF:
			if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
			break;
		case EffectType::EF_DEBUFF:
			if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
			break;
		default:
			break;
		}
		if (hasAttackEffect()) { attackEffect->sidePlayAnim(); }

	}

	// 経験値取得
	void addExp(int value) {
		this->Exp += value;
	}

	// レベルアップ関数
	void levelUp() {

		// レベルを上げる。
		this->Lv++;
		// 経験値を経験値上限分戻す。
		this->Exp -= this->maxExp;
		// 経験値上限を10%上昇させる
		this->maxExp = static_cast<int>(this->maxExp * 1.3f);
		// 最低ダメージの上昇
		this->minDamage += 1;

		// 各ステータスを3%上昇させる
		this->maxHp = static_cast<int>(this->maxHp * 1.2f);
		this->ATK = static_cast<int>(this->ATK * 1.2f);
		this->DEF = static_cast<int>(this->DEF * 1.2f);
		this->AGR = static_cast<int>(this->AGR * 1.2f);
	}

	// 行動選択関数
	void takeAction(std::vector<std::shared_ptr<Character>>& targets) override {
		
		// チェックの判定があれば、対象選択
		if (!Alive)	return;

		int damage = -1;

		// スマートポインタエネミーをnullptrで生成
		switch (actionChoice)
		{
		case ATTACK:

			// 生存判定
			if (!Alive) { return; }

			//ターゲットを攻撃
			for (auto& tar : targets) {
				damage = cal->DamageCal(this->ATK , getAtkbuff() ,tar->getDefense() , tar->getDefbuff());
				//ダメージの最低保証
				if (damage < minDamage) damage = minDamage;
				// ターゲットを攻撃
				tar->takeDamage(damage);
				tar->damageRenderStart(tar->getPosX() - 150, tar->getPosY() - 150, 40, damage);
				// アニメーションタイプ変更
				AnimType = ATTACK;
			}

			// 終了

			break;

		case SKILL:

			// 生存判定
			if (!Alive) { return; }

			// スキルクラスのスキル設定と数値を代入
			// ターゲットに関数呼び出し
			for (auto& tar : targets) {
				useSkill(tar, Skill.type, Skill.power, Skill.trun);
			}

			// 終了

			break;
		}

	}

	// スキル
	void useSkill(std::shared_ptr<Character>& arg_targets, SkillType arg_type, float arg_power, int arg_trun) {

		int damage = -1;
		int heal = -1;
		int turn = -1;
		float arg_buff = -1.0f;

		switch (arg_type)
		{
		case SkillType::ATTACK:

			// 対象者に攻撃力
			// ダメージ量を計算
			// combo数追加を実装予定
			damage = cal->DamageCal(this->ATK , arg_power , getAtkbuff(), arg_targets->getDefense());
			// ０未満にしない
			if (damage < minDamage) damage = minDamage;
			//　ダメージ関数
			arg_targets->takeDamage(damage);
			// ダメージ描画
			arg_targets->damageRenderStart(arg_targets->getPosX() - 150, arg_targets->getPosY() - 150,40, damage);
			// アニメーションタイプ
			AnimType = SKILL;

			break;

		case SkillType::HEAL:

			// 対象者のHPを上昇
			// HP回復量を計算
			heal = cal->Healcal(this->maxHp, arg_power);
			// ヒール関数
			arg_targets->takeHeal(heal);
			// 回復描画
			arg_targets->healRenderStart(arg_targets->getPosX() - 20, arg_targets->getPosY() - 5,20, heal);
			// アニメーションタイプ
			AnimType = MAGIC;

			break;

		case SkillType::ATK_BUFF:

			// 対象者の攻撃力上昇
			// バフ量を計算
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// ターゲットにxターンの攻撃増強バフの付与
			arg_targets->takeAtkBuff(arg_buff, turn);
			//stateChange = true;
			// アニメーションタイプ
			AnimType = MAGIC;

			break;

		case SkillType::AGR_BUFF:

			// 対象の速度上昇
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// バフ付与
			arg_targets->takeAgrBuff(arg_buff, turn);
			// アニメーションタイプ
			AnimType = MAGIC;

			break;

		case SkillType::DEF_BUFF:

			// 対象の速度上昇
			arg_buff = static_cast<float>(arg_power);
			turn = static_cast<int>(arg_trun);
			// バフ付与
			arg_targets->takeDefBuff(arg_buff, turn);
			// アニメーションタイプ
			AnimType = MAGIC;

			break;

		case SkillType::NONE:

			// 何もしない

			break;

		default:
			// 何もしない
			break;
		}

	}

	// 描画
	void Render(int arg_posX, int arg_posY, int arg_grh) override{

		// 本体描画
		posX = arg_posX;
		posY = arg_posY;
		ui->PlayerRender(posX, posY, arg_grh, AnimType);

		//　HPの描画
		ui->HpRender(posX + 5, posY - 10, 40, 10, this->maxHp, this->HP, GetColor(0, 255, 0));

		SetFontSize(15);
		//　名前とレベル表示
		//DrawFormatString(posX + 5, posY - 40, GetColor(255, 255, 255), "%s",this->Name.c_str());
		//DrawFormatString(posX + 5, posY - 25, GetColor(255, 255, 255), "Lv:%d", this->Lv);


	}

	void EffectDraw(int arg_x, int arg_y) {
		// エフェクト
		if (hasAttackEffect()) { attackEffect->effectDraw(targetPosX, targetPosY, targetSizeW, targetSizeH); }
		if (hasSkillEffect()) { skillEffect->effectDraw(arg_x, arg_y, targetSizeW, targetSizeH); }

	}

	// ステータスをリセット
	void statusReset() {
		this->Lv = 1;
	}
};