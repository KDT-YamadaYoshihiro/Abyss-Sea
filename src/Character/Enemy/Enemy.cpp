#include "Enemy.h"

Enemy::Enemy(int id, const std::string& name, int hp, int atk, int def, int agr, int lv, int exp, const SkillData& skill)
    : Character(id, name, hp, atk, def, agr, lv, exp, skill),
    m_atMoveSpeed(5),
    m_waitMoveSpeed(0.1f)
{

    ui = std::make_shared<UI>();
    action = ENEMY_ACTION::TURN_START;
    posX = WINDOW_W / 2 + 300;
    posY = WINDOW_H / 2 + 150;
    m_hpPosX = posX - 150;
    m_hpPosY = posY - 250;
    moveCheck = false;
}

void Enemy::Update()
{
    WaitMove();
    AttackMove();
}

void Enemy::Render(int arg_grh)
{
    ui->EnemyRender(posX, posY, arg_grh, 0.2f);
    //　HPの描画
    ui->HpRender(m_hpPosX,m_hpPosY, 300, 20, this->maxHp, this->HP, GetColor(0, 255, 0));
    // 名前・レベル
    SetFontSize(20);
    DrawFormatString(m_hpPosX, m_hpPosY - 40, GetColor(255, 0, 0), "%s", this->Name.c_str());
    DrawFormatString(m_hpPosX, m_hpPosY - 20, GetColor(255, 0, 0), "Lv:%2d", this->Lv);
}

void Enemy::CutinDraw()
{
    // カットイン描画
    cutin->Draw();
}

void Enemy::StartCutin()
{
    int handle = CLoad::Instance().getEfaceGrh(this->ID - 100);
    int seHandle = CLoad::Instance().getSeHandle(SE_CUTIN2);
    cutin->Start(WINDOW_W - 300, -300, handle, seHandle, Skill.Name, 30, 2, 60);
}

void Enemy::TakeAction(std::vector<std::shared_ptr<Character>>& targets, int trun)
{
    // 生存判定
    if (!Alive)
    {
        return;
    }

	if (trun % 3 == 0)
	{
        SkillAttack(targets);
	}
	else
	{
        NormalAttack(targets);
	}
}

void Enemy::NormalAttack(std::vector<std::shared_ptr<Character>>& targets)
{
    for (auto& tar : targets) {
        // ダメージ量を計算
        int damage = cal->DamageCal(this->ATK, tar->getDefense());
        // 0未満にしない
        if (damage < 1)
        {
            damage = 1;
        }
        //　ダメージ関数
        tar->takeDamage(damage);
        // アニメーション
        moveCheck = true;
        // ダメージ描画
        tar->damageRenderStart(tar->getPosX() - 20, tar->getPosY() - 5, 20, damage);
    }
}

void Enemy::SkillAttack(std::vector<std::shared_ptr<Character>>& targets)
{
    int damage = -1;
    int bace_atk = 0;

    for (auto& tar : targets) {

        // ダメージ量を計算
        // スキル補正
        bace_atk = this->ATK * this->Skill.power;
        // ダメージ計算
        damage = cal->DamageCal(bace_atk, Buff.atkMultiplier, getAtkbuff(), tar->getDefense());
        // ０未満にしない
        if (damage < 1) damage = 1;
        //　ダメージ関数
        tar->takeDamage(damage);
		// アニメーション
        moveCheck = true;
        // ダメージ描画
        tar->damageRenderStart(tar->getPosX() - 20, tar->getPosY() - 5, 20, damage);
    }

}

void Enemy::WaitMove()
{
    if (!moveCheck) {
        posY -= m_waitMoveSpeed;
    }
    if (m_waitMoveSpeed >= 0.0f && posY <= WINDOW_H / 2 + 140) {
        m_waitMoveSpeed *= -1.0f;
    }
    if (m_waitMoveSpeed <= 0.0f && posY >= WINDOW_H / 2 + 150) {
        m_waitMoveSpeed *= -1.0f;
    }

}

void Enemy::AttackMove()
{
    if (moveCheck) {
        posX -= m_atMoveSpeed;
    }
    if (m_atMoveSpeed >= 0 && posX <= WINDOW_W / 2 + 280) {
        m_atMoveSpeed *= -1;
    }
    if (m_atMoveSpeed <= 0 && posX >= WINDOW_W / 2 + 300) {
        m_atMoveSpeed *= -1;
        moveCheck = false;
    }
}
