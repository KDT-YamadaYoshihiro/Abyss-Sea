#pragma once
#include "UI.h"
#include "CStage.h"
#include "CharacterFactory.h"
#include "Character.h"
#include "Calculation.h"

// バトルスクリーンクラス
class CBattle : public ScreenBase {

#define FONT_BIGSIZE	 50
#define FONT_SIZE		 30
#define FONT_MINSIZE	 15

#define MAX_VOL			255

	// キャラクターセット、ゲット関数用
	struct Position {int x; int y;};

	// プレイヤーの行動モード(何もしない,行動選択, ターゲットリスト作成,　ターゲット選択) 
	enum class TargetInput{P_NONE, ACTIONCHOICE, LISTCREATE,TARGETCHOICE,END};
	// プレイヤーの行動モード変数
	TargetInput targetInput = TargetInput::P_NONE;

	// エネミーの行動モード
	enum class ActionMode{E_NONE,TARGETRANDOM,END};
	ActionMode actionMode = ActionMode::E_NONE;

	// バトルモード	 バトル、MENU、終了
	enum class State{BATTLE,MENU,DEAD_EFFECT,END};
	State state = State::BATTLE;

	// MENUモード		デフォ、再挑戦、リタイア、設定
	enum class MenuState{NONE,RETRY,RETIRE,SETTING};
	MenuState menuState = MenuState::NONE;

	// UIクラスの生成
	std::shared_ptr<UI> ui;

	// sp
	std::shared_ptr<BattleSP> sp;
	
	// エネミー
	std::vector<std::shared_ptr<Character>> enemies;
	// 行動順リスト
	std::vector<std::shared_ptr<Character>> turnOrder;

	// ターゲットリスト
	std::vector<std::shared_ptr<Character>> TargetList;
	// 対象者
	std::shared_ptr<Character> selectTarget;
	// 決定したターゲットリスト
	std::vector<std::shared_ptr<Character>> v;

	// 音
	std::shared_ptr<Sound>bgm;
	std::shared_ptr<Sound> se;

	// フェード
	std::shared_ptr<Fade> fade;
	// 再挑戦用のフェード
	//std::shared_ptr<Fade> retryFade;

	std::shared_ptr<Effect> effect;

	std::shared_ptr<Calculation> cal;

	// 現在ターンのインデックス
	int currentTurnIndex = 0;
	// ターン数
	int turn = -1;
	// 生成するステージ
	int stage = -1;
	// 生成するエネミーのID
	int e_id;

	// プレイヤーGRH
	int PlayerGrh = - 1;
	// エネミーGRH
	int EnemyGrh = -1;

	// 座標変数
	int pPosX[PLAYER_MAX];
	int pPosY[PLAYER_MAX];

	int ePosX = -1;
	int ePosY = -1;

	// ボタン座標
	int atButtonX = -1;
	int ButtonY = -1;
	int skButtonX = -1;
	int ButtonSizeX = -1;
	int ButtonSizeY = -1;

	int speed = -1;


	// 三角形の座標
	int trPosX[BUTTAN_MAX];
	int trPosY[BUTTAN_MAX];

	// 音量設定時に使用　クリックフレーム
	int clickFrame = -1;

	// アイコン下名前表示文字数制限
	int nameMax = -1;

	// ブレンドモードの変数
	int blendNum = -1;
	int blendSpeed = -1;

	// 終了フレームカウント
	int endFrame;

	// 戻るボタンの座標、サイズ変数
	int boxX = -1;
	int boxY = -1;
	int sizeW = -1;
	int sizeH = -1;

	// 他ボタンの座標
	int buttonX = -1;
	int buttonY = -1;

	// スキル説明の表示有無
	bool skdescDraw = false;

public:

	// 初期化
	CBattle() :
		// ボタン座標
		atButtonX(450),
		ButtonY(WINDOW_H - 60),
		skButtonX(atButtonX + 210),
		ButtonSizeX(200),
		ButtonSizeY(50),
		clickFrame(0),
		nameMax(6),
		blendNum(50),
		blendSpeed(1),
		// UIボタン座標
		boxX(WINDOW_W / 2 + 220),
		boxY(WINDOW_H - 100),
		sizeW(200),
		sizeH(80),
		buttonX(WINDOW_W / 2 - (FONT_BIGSIZE * 1.5)),
		buttonY(250),
		skdescDraw(false)
	{
		
		// 座標変数
		for (int i = 0; i < PLAYER_MAX; i++) { pPosX[i] = 300 + (i % 2) * 50; pPosY[i] = WINDOW_H / 2 + 60; }
		for (int i = 0; i < ENEMY_MAX; i++) { ePosX = WINDOW_W / 2 + 300; ePosY = WINDOW_H / 2 + 150; }
		

		// 初期化関数
		BattleInit();

		trPosX[BGM_UP] = WINDOW_W / 2 - 280;
		trPosY[BGM_UP] = WINDOW_H / 2 - 120;
		trPosX[BGM_DOWN] = WINDOW_W / 2 - 280;
		trPosY[BGM_DOWN] = WINDOW_H / 2 + 70;
		trPosX[SE_UP] = WINDOW_W / 2 + 260;
		trPosY[SE_UP] = WINDOW_H / 2 - 120;
		trPosX[SE_DOWN] = WINDOW_W / 2 + 260;
		trPosY[SE_DOWN] = WINDOW_H / 2 + 70;
	};

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:


	// バトル初期化（再挑戦時使用)
	void BattleInit() {

		// UIクラス生成
		ui = std::make_shared<UI>();
		// sp
		sp = std::make_shared<BattleSP>();

		// ステージ
		stage = Manager::Instance().getStageScreen();

		// 音源
		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		// フェード
		fade = Manager::Instance().getFade();

		// ID
		e_id = Manager::Instance().getEnemyID(stage);

		// エネミー生成
		auto enemy = CharacterFactory::Instance().CreateCharacter(e_id);
		enemies.push_back(enemy);

		cal = std::make_shared<Calculation>();

		// キャラクターリセット
		for (auto& p : Manager::Instance().getParty()) {
			p->hpReset();
			p->BuffReset();
			p->setAnimType(WAIT);
			p->effectrReset();
		}
		for (auto& e : enemies) {
			e->hpReset();
			e->BuffReset();
			e->effectrReset();
		}

		// キャラクターをインデックス。
		currentTurnIndex = 0;

		// ソートを行う。
		TurnOrder();

		// ターン
		turn = 1;

		// SPの初期化
		sp->ResetSP();

		// 勝敗判定のリセット
		Manager::Instance().setBattleResult(BattleResult::BATTLE);

		// bgmの再生
		bgm->PlayBgm(CLoad::Instance().getBgmHandle(1 + stage));

		endFrame = 120;
	}
	
	void Delete() {

		enemies.clear();
	}

	// バトル終了確認関数
	bool BattleOver() {
		// プレイヤーが生きているか
		bool allPlayersDefeated = true;
		for (auto& p : Manager::Instance().getParty()) {
			if (p->getAlive()) {
				allPlayersDefeated = false;
				break;
			}
			else {
				// 敗北
				Manager::Instance().setBattleResult(BattleResult::DEFEAT);
			}
		}

		// エネミーが生きているか
		bool EnemiesDefeated = true;
		for (const auto& e : enemies) {
			if (e->getAlive()) {
				EnemiesDefeated = false;
				break;
			}
			else {
				// 勝利
				Manager::Instance().setBattleResult(BattleResult::VICTORY);
				Manager::Instance().setExp(e->getExp());
				// 音源の停止
			}
		}

		// 双方falseならバトルは続行
		// どちらかがtrueならバトル終了
		return allPlayersDefeated || EnemiesDefeated;
	}

	// ソート処理
	//template<typename T>
	//void sort(T* array, std::size_t size) {
	//	for (std::size_t i = 0; i < size - 1; ++i) {
	//		bool swapped = false;
	//		for (std::size_t j = 0; j < size - 1 - i; ++j) {
	//			int j_speed = cal->BuffCal(array[j]->getSpeed(), array[j]->getAgrbuff());
	//			int j_next_speed = cal->BuffCal(array[j + 1]->getSpeed(), array[j + 1]->getAgrbuff());
	//			if (j_speed < j_next_speed) {
	//				std::swap(array[j], array[j + 1]);
	//				swapped = true;
	//			}
	//		}
	//		if (!swapped) break;
	//	}
	//}

	// 毎ターン行うソート
	void TurnOrder() {

		// 過去のソート情報を削除
		turnOrder.clear();
		// 生きているキャラをソート対象に
		for (const auto& p : Manager::Instance().getParty()) {
			if (p->getAlive() == true) turnOrder.push_back(p);
		}
		for (const auto& e : enemies) {
			if (e->getAlive() == true) turnOrder.push_back(e);
		}

		//// 素早さでソート（降順）
		//if (!turnOrder.empty()) {
		//	// ポインタ配列として扱う
		//	std::shared_ptr<Character>* array = turnOrder.data();
		//	std::size_t size = turnOrder.size();
		//	sort(array, size);
		//}

		 // 素早さでソート（降順）
		if (!turnOrder.empty()) {
			// std::sort を使ってソート
			std::sort(turnOrder.begin(), turnOrder.end(), [this](const std::shared_ptr<Character>& a, const std::shared_ptr<Character>& b) {
				int a_speed = cal->BuffCal(a->getSpeed(), a->getAgrbuff());
				int b_speed = cal->BuffCal(b->getSpeed(), b->getAgrbuff());
				return a_speed > b_speed; // 降順にソート
			});
		}
	}

	// ソートした物をアイコン上に
	void DrawTurnOrderIcons(int x, int y) const
	{
		const int ICON_SIZE = 60;	// アイコンの表示サイズ
		const int PADDING = 10;		// アイコンの間隔
		int drawX = x;				//　スタート地点

		for (size_t i = 0; i < turnOrder.size(); ++i) {
			const auto& ch = turnOrder[i];
			int handle = ch->getFaceHandle();

			// アイコンを描画
			DrawExtendGraph(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, handle, TRUE);

			// 現在のターンキャラに枠を表示
			if (i == static_cast<size_t>(currentTurnIndex)) {
				DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(255, 255, 0), FALSE);
			}
			else {
				// 非行動者
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(100, 100, 100), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			// 名前または略称を下に表示
			SetFontSize(FONT_MINSIZE);
			std::string name = ch->getName();
			if (name.size() > nameMax) {
				name = name.substr(0, nameMax);
			}
			DrawFormatString(drawX, y + ICON_SIZE + 4, GetColor(255, 255, 255), "%s",name.c_str()); 
			// 次のキャラの描画位置を横にずらす
			drawX += ICON_SIZE + PADDING;
		}
	}

	// リストを空にする
	void ListClea() {
		TargetList.clear();
		v.clear();
	}

	// spの増減
	void updataSP(std::shared_ptr<Character> arg_character) {
		// spの増減
		if (arg_character->getActionChoice() == ATTACK) {
			// sp増加
			sp->recoverSP();
		}
		else {
			// SP消費
			sp->consumeSP();
		}

	}

	// プレイヤー
	//　攻撃かスキルか選択(入力)
	void ActionChoice(std::shared_ptr<Character> arg_character) {

		//　通常攻撃orスキル選択（クリック）
		if (CheckBoxClick(atButtonX, ButtonY, ButtonSizeX,ButtonSizeY)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			arg_character->setActionChoice(ATTACK);
			targetInput = TargetInput::LISTCREATE;
		}
		else if (CheckBoxClick(skButtonX, ButtonY, ButtonSizeX, ButtonSizeY)) {

			// spが0より大きいなら（1以上なら）
			if (sp->getSP() > 0) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				arg_character->setActionChoice(SKILL);
				skdescDraw = true;
				targetInput = TargetInput::LISTCREATE;
			}
			else {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}
		}

		
	}

	//　ターゲットリスト作成
	void TargetListCreate(std::shared_ptr<Character> arg_character) {

		TargetList.clear();

		// 攻撃 or スキルで敵/味方をリストアップ
		if (arg_character->getActionChoice() == ATTACK ||
			arg_character->getTargetType() == SkillTargetType::SINGLE_ENEMY ||
			arg_character->getTargetType() == SkillTargetType::ALL_ENEMY) {

			for (auto& e : enemies)
				if (e->getAlive()) TargetList.push_back(e);
		}
		else if (arg_character->getTargetType() == SkillTargetType::SINGLE_ALLY ||
			arg_character->getTargetType() == SkillTargetType::ALL_ALLY) {

			for (auto& p : Manager::Instance().getParty())
				if (p->getAlive()) TargetList.push_back(p);
		}
		else if (arg_character->getTargetType() == SkillTargetType::SELF) {
			TargetList.push_back(arg_character);
		}

		// ターゲット選択モードへ
		targetInput = TargetInput::TARGETCHOICE;
	}

	// ターゲットの選択
	void TargetChoice(std::shared_ptr<Character> arg_character) {
		
		// 行動キャンセル処理
		if (CheckBoxClick(boxX, boxY, sizeW, sizeH)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			targetInput = TargetInput::ACTIONCHOICE;
			skdescDraw = false;
			return;
		}

		// 各ターゲットに対してクリック判定
		for (auto& tar : TargetList) {
			Position pos = GetCharacterCenter(tar);

			if (CheckCircleClick(pos.x, pos.y, 40.0f)) {

				// 決定音
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

				// 対象者決定
				selectTarget = tar;
				TypeConversion();

				// SP確認（スキル時のみ）
				if (arg_character->getActionChoice() == SKILL && !sp->comfirmSP()) return;

				// 実際の対象リスト
				std::vector<std::shared_ptr<Character>> actTargets;

				if (arg_character->getTargetType() == SkillTargetType::ALL_ENEMY ||
					arg_character->getTargetType() == SkillTargetType::ALL_ALLY) {
					actTargets = TargetList;
				}
				else {
					actTargets.push_back(selectTarget);
				}

				// アクション実行
				arg_character->takeAction(actTargets);

				// エフェクト再生
				PlayEffectByType(arg_character, actTargets);

				// ターゲット選択モードをオフ
				targetInput = TargetInput::END;
				break;
			}
		}
	}

	// 再生するエフェクト
	void PlayEffectByType(std::shared_ptr<Character> actor,
		const std::vector<std::shared_ptr<Character>>& targets) {

		EffectType efType = actor->getEfType();

		if (actor->getActionChoice() == SKILL) {
			switch (efType) {

				// 単体・全体スキル1/2
			case EffectType::EF_SKILL1:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 300, 250);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL1));
				break;

			case EffectType::EF_SKILL2:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 350, 250);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
				break;

				// 全体スキル
			case EffectType::EF_ALL_SKILL1:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 250, 250);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
				break;

			case EffectType::EF_ALL_SKILL2:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 250, 350);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
				break;

				// 回復系
			case EffectType::EF_HEAL:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 30, 40);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_HEAL));
				break;

				// バフ・デバフ系
			case EffectType::EF_BUFF:
			case EffectType::EF_DEBUFF:
				for (auto& t : targets)
					actor->PlaySkillEffect(t->getPosX(), t->getPosY(), 50, 40);
				se->PlaySe(CLoad::Instance().getSeHandle(SE_BUFF));
				break;

			default:
				break;
			}
		}
		else {
			// 通常攻撃
			for (auto& t : targets)
				actor->PlayAttackEffect(t->getPosX(), t->getPosY(), 50, 40);
			se->PlaySe(CLoad::Instance().getSeHandle(SE_ATTACK));

		}
	}

	// エネミー
	void EnemyAction(std::shared_ptr<Character> arg_character) {
		for (auto& p : Manager::Instance().getParty()) {
			if (p->getAlive()) TargetList.push_back(p);
		}

		int index = rand() % TargetList.size();
		selectTarget = TargetList[index];
		TypeConversion();
		//　アクションに移行
		arg_character->takeAction(v);
		// プレイヤー攻撃時にアニメーション切り替え
		auto& p =  Manager::Instance().getParty();
		for (size_t i = 0; i < Manager::Instance().getParty().size(); i++) {
			if (TargetList[index]->getId() == p[i]->getId())
				p[i]->setAnimType(DAMAGE);
		}
		// se再生
		se->PlaySe(CLoad::Instance().getSeHandle(SE_ATTACK));

		actionMode = ActionMode::END;

	}

	// キャラクターのターン終了
	void PEnd(std::shared_ptr<Character> arg_character) {
		
		for (auto& p : Manager::Instance().getParty()) {
			// アニメーションをWAITにする
			p->setAnimType(WAIT);
			// チェンジモードをfalseに
			p->setAnimChange(false);
		}

		// 説明の非表示
		skdescDraw = false;

		// ターゲット選択モードをオフ
		targetInput = TargetInput::P_NONE;
		// spの更新
		updataSP(arg_character);
		// インデックスをインクリメントして次のキャラクターの行動へ		
		currentTurnIndex++;
		if (currentTurnIndex >= turnOrder.size()) {
			TurnEnd();
			return;
		}
	}

	void EEnd(std::shared_ptr<Character> arg_characte) {
		for (auto& p : Manager::Instance().getParty()) {
			// アニメーションをWAITにする
			p->setAnimType(WAIT);
			// チェンジモードをfalseに
			p->setAnimChange(false);
		}

		actionMode = ActionMode::E_NONE;
		// アクション終了後インデックスをインクリメント
		currentTurnIndex++;
		if (currentTurnIndex >= turnOrder.size()) {
			TurnEnd();
			return;
		}
	}

	// ターン終了時関数
	void TurnEnd() {
	
		// バフデバフの更新
		for (auto& p : Manager::Instance().getParty()) {
			p->updateBuff();
		}
		// エネミー
		for (auto& e : enemies) {
			e->updateBuff();
		}

		// ターンオーダーの中身をリセット
		turnOrder.clear();
		// インデックスをリセット
		currentTurnIndex = 0;

		// ターンの増加
		this->turn++;
		
	}

	// smartpointerをベクター型に変換
	void TypeConversion() {
		v = { selectTarget };
	}

	// プレイヤーの画像中央座標を取得する関数
	// ターゲット候補者の円の描画に使用
	Position GetCharacterCenter(std::shared_ptr<Character> c) {
		// 元座標を取得
		int x = c->getPosX();
		int y = c->getPosY();

		if (std::dynamic_pointer_cast<Player>(c)) {
			// プレイヤーの場合：左上基準 → 中心に補正
			x += SIZE_W / 2;
			y += SIZE_H / 2;
		}
		// エネミーは中心基準なので補正不要
		return { x, y };
	}

	// 移動用関数
	void playerMove() {
		// プレイヤー、エネミーの座標移動
		for (size_t i = 0; i < Manager::Instance().getParty().size(); i++) {

			auto& p = Manager::Instance().getParty();

			if (turnOrder[currentTurnIndex]->getId() == p[i]->getId()) {
				pPosX[i] += 10;
				if (pPosX[i] >= WINDOW_W / 2 - 150) {
					pPosX[i] = WINDOW_W / 2 - 150;
				}
			}
			else {
				pPosX[i] -= 10;
				if (pPosX[i] <= 300 + (i % 2) * 50) {
					pPosX[i] = 300 + (i % 2) * 50;
				}
			}
		}

	}
	// 移動用関数
	void EnemyMove() {
		for (auto& enemy : enemies) {
			// 移動
			if (enemy->getMoveCheck()) {
				ePosX -= speed;
			}
			// 移動上限
			if (speed >= 0 && ePosX <= WINDOW_W / 2 + 280) {
				speed *= -1;
			}
			if (speed <= 0 && ePosX >= WINDOW_W / 2 + 300) {
				speed *= -1;
				enemy->setMoveCheck(false);
			}
		}

	}

	// メインループ関数
	// バトル
	void BattleMain() {

		// インデックスが0の時、ソートを行う。
		if (turnOrder.empty()) TurnOrder();

		if (targetInput == TargetInput::P_NONE) targetInput = TargetInput::ACTIONCHOICE;
		// 行動選択と実行
		if (currentTurnIndex < turnOrder.size()) {

			auto character = turnOrder[currentTurnIndex];

			// 移動する
			playerMove();
			EnemyMove();

			if (character->getAlive() && !BattleOver()) { // 行動中に戦闘が終了していないかチェック

				// プレイヤーとエネミーの型判定を安全にしたいためdynamic
				if (std::dynamic_pointer_cast<Player>(character)) {	// プレイヤー					

					// 入力待ち状態
					if (targetInput == TargetInput::ACTIONCHOICE) {
						// 実行前にリストを空にする。
						ListClea();
						ActionChoice(character);
					}
					// ターゲットリスト制作
					if (targetInput == TargetInput::LISTCREATE)TargetListCreate(character);
					//　ターゲット選択
					if (targetInput == TargetInput::TARGETCHOICE) TargetChoice(character);
					// 終了
					for (auto& p : Manager::Instance().getParty()) {
						if (p->getAnimChange()) {
							if (targetInput == TargetInput::END) PEnd(character);
						}
					}

				}
				else {	// エネミー
					// 敵が行動する場合のターゲットはプレイヤー
					// 自動でターゲットを決める
					if (actionMode == ActionMode::E_NONE) {
						ListClea();
						actionMode = ActionMode::TARGETRANDOM;
					}

					if (actionMode == ActionMode::TARGETRANDOM)EnemyAction(character);
					if (actionMode == ActionMode::END)EEnd(character);


				}
			}
			else {
				// 死亡していたら即ターン終了
				PEnd(character);
			}

		}
		else {
			if (turnOrder.empty() || currentTurnIndex >= turnOrder.size()) {
				TurnEnd(); // ターン終了へ
				return;
			}
		}

		// バトル進行関数
		// バトルの終了をチェック。
		if (BattleOver()) {

			state = State::DEAD_EFFECT;

		}


		// メニューバーが押されたとき、モードの切り替え
		if (CheckBoxClick(WINDOW_W - 250, 10, 250, 150)) {
			state = State::MENU;
		}

	}

	// メニュー
	void BattleMenu() {

		switch (menuState)
		{
		case CBattle::MenuState::NONE:

			// 選択画面の表示
			// バトル再開
			if (CheckBoxClick(buttonX, buttonY, sizeW, sizeH)) {
				state = State::BATTLE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// 再挑戦
			if (CheckBoxClick(buttonX, buttonY + 100, sizeW, sizeH)) {
				// モード変更
				menuState = MenuState::RETRY;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// リタイア
			if (CheckBoxClick(buttonX, buttonY + 200, sizeW, sizeH)) {
				// モード変更
				menuState = MenuState::RETIRE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}
			// （設定）
			if (CheckBoxClick(buttonX, buttonY + 300, sizeW, sizeH)) {
				menuState = MenuState::SETTING;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

			}


			break;
		case CBattle::MenuState::RETRY:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), sizeW, sizeH)) {
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);

				// 再挑戦
				// 敵を削除
				Delete();
				// 再初期化
				BattleInit();
				// バトル画面に切り替える
				state = State::BATTLE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));

			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), sizeW, sizeH)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}


			break;
		case CBattle::MenuState::RETIRE:

			// 再確認
			// はい
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 6), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), sizeW, sizeH)) {

				// Party全員を死亡判定に
				for (auto& p : Manager::Instance().getParty()) {
					p->setAlive(false);
				}
				// fadeを軌道
				fade->fadeStart(fade->FADE_CLAUSE);
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_DECISION));
				// bgmの停止
				bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + Manager::Instance().getStageScreen()));
			}
			// いいえ
			if (CheckBoxClick(WINDOW_W / 2 + (FONT_BIGSIZE * 2.5), WINDOW_H / 2 + (FONT_BIGSIZE * 1.5), sizeW, sizeH)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));

			}

			break;

		case CBattle::MenuState::SETTING:

			if (clickFrame > 0) {
				clickFrame--;
			}

			// 音量変更
			for (int i = 0; i < BUTTAN_MAX; i++) {

				int size;
				if (i % 2 == 0) { size = MAX_VOL / 10; }
				else { size = -MAX_VOL / 10; }

				if (clickFrame == 0 && CheckCircleClick(trPosX[i], trPosY[i] + size, 50)) {

					int bgmVol = Manager::Instance().getBgmVolume();
					int seVol = Manager::Instance().getSeVolume();

					clickFrame = 10;
					se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));

					switch (i)
					{
					case BGM_UP:

						Manager::Instance().setBgmVolume(bgmVol + 1);

						break;

					case BGM_DOWN:
						Manager::Instance().setBgmVolume(bgmVol - 1);

						break;

					case SE_UP:
						Manager::Instance().setSeVolume(seVol + 1);

						break;

					case SE_DOWN:
						Manager::Instance().setSeVolume(seVol - 1);

						break;

					default:
						break;
					}

				}

			}

			// 決定ボタンを押した際
			if (CheckBoxClick(WINDOW_W / 2 - (FONT_BIGSIZE * 2), WINDOW_H - 100, sizeW, sizeH)) {
				// MENUの選択画面に戻る
				menuState = MenuState::NONE;
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			}

			break;
		default:
			break;
		}

	}

	// 死亡時エフェクト
	void PlayEndEffect() {

		endFrame--;

		for (auto& e : enemies) {

			if (!e->getAlive()) {

				for (auto& p : Manager::Instance().getParty()) {
					p->setAnimType(GRTYPE::WIN);
				}

			}
			else {

				for (auto& p : Manager::Instance().getParty()) {
					p->setAnimType(GRTYPE::LOSE);
				}

			}

		}

		if (endFrame <= 0) {
			state = State::END;
		}
	}

	// 終了
	void BattleEnd() {

		// falseならfadeを軌道。
		fade->fadeStart(fade->FADE_CLAUSE);
		// 音源の停止
		bgm->stopBgm(CLoad::Instance().getBgmHandle(1 + stage));
		for (int i = 0; i < SE_MAX; i++) {
			se->stopSe(CLoad::Instance().getSeHandle(i));
		}

	}

};