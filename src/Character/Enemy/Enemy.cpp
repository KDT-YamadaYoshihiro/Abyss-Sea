#include "Enemy.h"

Enemy::Enemy(int id, const std::string& name, int hp, int atk, int def, int agr, int lv, int exp, const SkillData& skill)
    : Character(id, name, hp, atk, def, agr, lv, exp, skill),
    m_atMoveSpeed(5),
    m_waitMoveSpeed(0.1f),
	m_deathAlpha(255),
	moveCheck(false)
{
    ui = std::make_shared<UI>();
    action = ENEMY_ACTION::TURN_START;
    posX = WINDOW_W / 2 + 300;
    posY = WINDOW_H / 2 + 150;
    m_hpPosX = posX - 150;
    m_hpPosY = posY - 250;
}

void Enemy::Update()
{
    WaitMove();
    AttackMove();
}

void Enemy::Render(int arg_grh)
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deathAlpha);
    ui->EnemyRender(posX, posY, arg_grh, 0.2f);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// HPバーの描画
    ui->HpRender(m_hpPosX,m_hpPosY, 300, 20, this->maxHp, this->HP, GetColor(0, 255, 0));
	// 名前とレベルの描画
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

void Enemy::TakeAction(std::vector<std::shared_ptr<Character>>& targets)
{
	// 行動不能なら何もしない
    if (!Alive)
    {
        return;
    }

	// ダメージ計算とダメージ表示
    for (auto& tar : targets) {
        int damage = cal->DamageCal(this->ATK, tar->getDefense());
        if (damage < 1)
        {
            damage = 1;
        }
        tar->takeDamage(damage);
        moveCheck = true;
        tar->damageRenderStart(tar->getPosX() - 20, tar->getPosY() - 5, 20, damage);

    }
}

void Enemy::WaitMove()
{
	// 待機中の上下移動
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
	// 攻撃中の左右移動
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

bool Enemy::UpdateDeathFade()
{
    if (!Alive && m_deathAlpha > 0) {
        m_deathAlpha -= 1;
    }

    if (m_deathAlpha < 0) {
        m_deathAlpha = 0;
    }

    if (m_deathAlpha == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
