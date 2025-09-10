#pragma once
#include "CBase.h"
#include "Character.h"

// 基底エネミークラス
class Enemy : public Character
{

    // UI
    std::shared_ptr<UI> ui;

    // 座標
    int posX;
    int posY;


public:


    // 初期化
    Enemy(const CharacterData& data, const Skill& skilldata) : Character(data, skilldata) {
        ui = std::make_shared<UI>();

        posX = -1;
        posY = -1;
    }

    // デストラクタ
    virtual ~Enemy() = default;

    // 座標を取得関数
    float getPosX() const { return (float)posX; }
    float getPosY() const { return (float)posY; }



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

    // 行動内容
    void takeAction(std::vector<std::shared_ptr<Character>>& targets) override {

        // 生存判定
        if (!Alive) {return;}
        
        // ターゲットが決まれば、ターゲットを攻撃
        for (auto& tar : targets) {
            int damage = this->ATK - tar->getDefense();
            if (damage < 1) damage = 1;
            tar->takeDamage(damage);
            moveCheck = true;
            tar->damageRenderStart(tar->getPosX() - 20, tar->getPosY() - 5, 20, damage);
            
        }

    }


};
