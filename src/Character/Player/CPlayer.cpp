#include "CPlayer.h"

void Player::AnimUpdata()
{
	ui->PlayerAnim(AnimType);

	switch (Skill.effectType)
	{
	case EFFECT_TYPE::EF_NONE:
		break;
	case EFFECT_TYPE::EF_ATTACK:
		break;
	case EFFECT_TYPE::EF_SKILL1:
		if (hasSkillEffect()) { skillEffect->lengthPlayerAnim(); }
		break;
	case EFFECT_TYPE::EF_SKILL2:
		if (hasSkillEffect()) { skillEffect->PlayAnim(); }
		break;
	case EFFECT_TYPE::EF_ALL_SKILL1:
		if (hasSkillEffect()) { skillEffect->PlayAnim(); }
		break;
	case EFFECT_TYPE::EF_ALL_SKILL2:
		if (hasSkillEffect()) { skillEffect->PlayAnim(); }
		break;
	case EFFECT_TYPE::EF_HEAL:
		if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
		break;
	case EFFECT_TYPE::EF_BUFF:
		if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
		break;
	case EFFECT_TYPE::EF_DEBUFF:
		if (hasSkillEffect()) { skillEffect->sidePlayAnim(); }
		break;
	default:
		break;
	}
	if (hasAttackEffect()) { attackEffect->sidePlayAnim(); }

}

void Player::StartCutin()
{
	int handle = CLoad::Instance().getPfaceGrh(this->getId());
	int seHandle = CLoad::Instance().getSeHandle(SE_CUTIN1);
	cutin->Start(100, -300, handle, seHandle, Skill.Name, 30, 2, 60);
}

void Player::AddExp(int value)
{
	this->Exp += value;
}

void Player::LevelUp()
{
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

void Player::TakeAction(std::vector<std::shared_ptr<Character>>& targets, int trun)
{
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
			damage = cal->DamageCal(this->ATK, getAtkbuff(), tar->getDefense(), tar->getDefbuff());
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
			UseSkill(tar, Skill.type, Skill.power, Skill.turn);
		}

		// 終了

		break;
	}
}

void Player::UseSkill(std::shared_ptr<Character>& arg_targets, SKILL_TYPE arg_type, float arg_power, int arg_trun)
{
	int damage = -1;
	int heal = -1;
	int turn = -1;
	float arg_buff = -1.0f;
	int bace_atk = 0;

	switch (arg_type)
	{
	case SKILL_TYPE::ATTACK:					// 対象者に攻撃力

		// ダメージ量を計算
		// スキル補正
		bace_atk = this->ATK * arg_power;
		// ダメージ計算
		damage = cal->DamageCal(bace_atk, Buff.atkMultiplier, getAtkbuff(), arg_targets->getDefense());
		// ０未満にしない
		if (damage < minDamage) damage = minDamage;
		//　ダメージ関数
		arg_targets->takeDamage(damage);
		// ダメージ描画
		arg_targets->damageRenderStart(arg_targets->getPosX() - 150, arg_targets->getPosY() - 150, 40, damage);
		// アニメーションタイプ
		AnimType = SKILL;

		break;

	case SKILL_TYPE::HEAL:					// 対象者に回復	

		// HP回復量を計算
		heal = cal->Healcal(this->maxHp, arg_power);
		// ヒール関数
		arg_targets->takeHeal(heal);
		// 回復描画
		arg_targets->healRenderStart(arg_targets->getPosX() - 20, arg_targets->getPosY() - 5, 20, heal);
		// アニメーションタイプ
		AnimType = MAGIC;

		break;

	case SKILL_TYPE::ATK_BUFF:				// 対象者の攻撃力上昇

		// 
		arg_buff = static_cast<float>(arg_power);
		turn = static_cast<int>(arg_trun);
		// ターゲットにxターンの攻撃増強バフの付与
		arg_targets->takeAtkBuff(arg_buff, turn);
		//stateChange = true;
		// アニメーションタイプ
		AnimType = MAGIC;

		break;

	case SKILL_TYPE::AGR_BUFF:				// 対象者の速度上昇

		// 
		arg_buff = static_cast<float>(arg_power);
		turn = static_cast<int>(arg_trun);
		// バフ付与
		arg_targets->takeAgrBuff(arg_buff, turn);
		// アニメーションタイプ
		AnimType = MAGIC;

		break;

	case SKILL_TYPE::DEF_BUFF:				// 対象の速度上昇

		//
		arg_buff = static_cast<float>(arg_power);
		turn = static_cast<int>(arg_trun);
		// バフ付与
		arg_targets->takeDefBuff(arg_buff, turn);
		// アニメーションタイプ
		AnimType = MAGIC;

		break;

	case SKILL_TYPE::NONE:

		// 何もしない

		break;

	default:
		// 何もしない
		break;
	}

}

void Player::Render(int arg_posX, int arg_posY, int arg_grh)
{
	// 本体描画
	posX = arg_posX;
	posY = arg_posY;
	ui->PlayerRender(posX, posY, arg_grh, AnimType);

	//　HPの描画
	ui->HpRender(posX + 5, posY - 10, 40, 10, this->maxHp, this->HP, GetColor(0, 255, 0));
}

void Player::CutinDraw()
{
	// カットイン描画
	cutin->Draw();
}

void Player::EffectDraw(int arg_x, int arg_y)
{
	// エフェクト
	if (hasAttackEffect()) { attackEffect->effectDraw(targetPosX, targetPosY, targetSizeW, targetSizeH); }
	if (hasSkillEffect()) { skillEffect->effectDraw(arg_x, arg_y, targetSizeW, targetSizeH); }
}

void Player::StatusReset()
{
	this->Lv = 1;
}
