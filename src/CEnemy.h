#pragma once
#include "Character.h"
#include "UI.h"

// 基底エネミークラス
class Enemy : public Character
{

    // UI
    std::shared_ptr<UI> ui;

    // 行動状態
    ENEMY_ACTION action;

    // 座標
    int posX;
    int posY;


public:


    // 初期化
    Enemy(int id, const std::string& name, int hp, int atk, int def,
        int agr, int lv, int exp, const SkillData& skill)
        : Character(id, name, hp, atk, def, agr, lv, exp, skill)
    {

        ui = std::make_shared<UI>();
        action = ENEMY_ACTION::TURN_START;
        posX = -1;
        posY = -1;
    }

    // デストラクタ
    virtual ~Enemy() = default;

    // faceハンドル取得
    int getFaceHandle() const override
    {
        int index = this->ID - 100;
        return CLoad::Instance().getEfaceGrh(index);
    }


    // 座標を取得関数
    float getPosX() const 
    {
        return (float)posX;
    }
    float getPosY() const 
    {
        return (float)posY;
    }


    void Update() {};


    void Render(int arg_posX, int arg_posY, int arg_grh) override {

        // 本体描画
        this->posX = arg_posX;
        this->posY = arg_posY;
        ui->EnemyRender(posX, posY, arg_grh, 0.2f);
        //　HPの描画
        ui->HpRender(posX - 150, posY - 250, 300, 20, this->maxHp, this->HP, GetColor(0, 255, 0));
        // 名前・レベル
        SetFontSize(20);
        DrawFormatString(posX - 150, posY - 290, GetColor(255, 0, 0), "%s", this->Name.c_str());
        DrawFormatString(posX - 150, posY - 270,  GetColor(255, 0, 0), "Lv:%2d",this->Lv);

    };

	void CutinDraw() override {
		// カットイン描画
		cutin->Draw();
	}

	// カットイン開始
    void StartCutin() override{
		int handle = CLoad::Instance().getEfaceGrh(this->ID - 100);
		cutin->Start(WINDOW_W - 300, -300, handle, Skill.Name,30,2,60);
    }

    // 攻撃内容
    void takeAction(std::vector<std::shared_ptr<Character>>& targets) override {

        // 生存判定
        if (!Alive)
        {
            return;
        }

        // ターゲットが決まれば、ターゲットを攻撃
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

};
