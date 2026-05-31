#pragma once
#include "Character/Character.h"
#include "Constants/define.h"

// 基底エネミークラス
class Enemy : public Character
{

    // UI
    std::shared_ptr<UI> ui;

    //// 死亡時エフェクト
    //std::shared_ptr<Effect> deadEffect;

    // 行動状態
    ENEMY_ACTION action;

    // 攻撃スピード
    float m_waitMoveSpeed;
    int m_atMoveSpeed;

    // HP用座標
    int m_hpPosX;
    int m_hpPosY;
	// 死亡時のアルファ値
    int m_deathAlpha = 255;
    // エネミー用
    bool moveCheck = false;

public:


    // 初期化
    Enemy(int id, const std::string& name, int hp, int atk, int def,
        int agr, int lv, int exp, const SkillData& skill);

    // デストラクタ
    virtual ~Enemy() = default;

    // faceハンドル取得
    int getFaceHandle() const override
    {
        int index = this->ID - 100;
        return CLoad::Instance().getEfaceGrh(index);
    }

	// 行動状態のゲット、セット
    bool getMoveCheck() const { return moveCheck; }
    void setMoveCheck(bool arg_moveCheck) { moveCheck = arg_moveCheck; }
    // 更新
    void Update();
    // 描画
    void Render(int posX, int posY, int path)override {};
    void Render(int arg_grh);
    // カットイン描画
    void CutinDraw() override;
	// カットイン開始
    void StartCutin() override;
    // 攻撃内容
    void TakeAction(std::vector<std::shared_ptr<Character>>& targets) override;
    // 待機Move
    void WaitMove();
    // 攻撃Move
    void AttackMove();
	// 死亡演出
    void UpdateDeathFade();
	// 死亡演出のアルファ値のゲット
    int GetDeathAlpha() const { return m_deathAlpha; }
};
