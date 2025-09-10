#pragma once
#include "CStage.h"
#include "UI.h"

#include "CharacterFactory.h"
#include "Character.h"


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

	// バトルモード		バトル本編、MENU展開、再挑戦、リタイア、設定
	enum class PauseState {NONE,MENU,RETRY, RETIRE, SETTING};
	PauseState pauseState = PauseState::NONE;

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

	// 現在ターンのインデックス
	int currentTurnIndex = 0;
	// ターン数
	int turn = -1;
	// 生成するステージ
	int stage = -1;

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
	int ButtonX = -1;
	int atButtonY = -1;
	int skButtonY = -1;
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

public:

	// 初期化
	CBattle() {

		// UIクラス生成
		ui = std::make_shared<UI>();
		// sp
		sp = std::make_shared<BattleSP>();

		// ステージ
		stage = Manager::Instance().getStageScreen();

		// エネミーを生成
		enemies.push_back(CharacterFactory::CreateEnemy(stage));

		bgm = Manager::Instance().getbgm();
		se = Manager::Instance().getSe();

		fade = Manager::Instance().getFade();
		//retryFade = std::make_shared<Fade>();
		
		// 座標変数
		for (int i = 0; i < PLAYER_MAX; i++) { pPosX[i] = 300 + (i % 2) * 50; pPosY[i] = WINDOW_H / 2 + 50; }
		for (int i = 0; i < ENEMY_MAX; i++) { ePosX = WINDOW_W / 2 + 300; ePosY = WINDOW_H / 2 + 150; }
		
		// ボタン座標
		ButtonX = 50;
		atButtonY = WINDOW_H / 2;
		skButtonY = atButtonY + (50 * 2);
		ButtonSizeX = 200;
		ButtonSizeY = 50;
		speed = 5;

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

		clickFrame = 0;
		nameMax = 6;
	};

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:

	// バトル初期化（再挑戦時使用)
	void BattleInit() {

		//if (!fade->checkOpen() && fade->getRadius() >= WINDOW_W) {
		//	fade->fadeStart(fade->FADE_OPEN);
		//}

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
	template<typename T>
	void sort(T* array, std::size_t size) {
		for (std::size_t i = 0; i < size - 1; ++i) {
			bool swapped = false;
			for (std::size_t j = 0; j < size - 1 - i; ++j) {
				if ((array[j]->getSpeed() * array[j]->getAgrbuff()) < (array[j + 1]->getSpeed() * array[j + 1]->getAgrbuff())) {
					std::swap(array[j], array[j + 1]);
					swapped = true;
				}
			}
			if (!swapped) break;
		}
	}

	// 毎ターン行うソート
	void TurnOrder() {

		turnOrder.clear();

		for (const auto& p : Manager::Instance().getParty()) {
			if (p->getAlive() == true) turnOrder.push_back(p);
		}
		for (const auto& e : enemies) {
			if (e->getAlive() == true) turnOrder.push_back(e);
		}

		// 素早さでソート（降順）
		if (!turnOrder.empty()) {
			// ポインタ配列として扱う
			std::shared_ptr<Character>* array = turnOrder.data();
			std::size_t size = turnOrder.size();
			sort(array, size);
		}
	}

	// ソートした物をアイコン上に
	void DrawTurnOrderIcons(int x, int y) const
	{
		const int ICON_SIZE = 60;  // アイコンの表示サイズ
		const int PADDING = 10;    // アイコンの間隔
		int drawX = x;

		for (size_t i = 0; i < turnOrder.size(); ++i) {
			const auto& ch = turnOrder[i];
			int handle = ch->getFaceHandle();

			// アイコンを描画
			DrawExtendGraph(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, handle, TRUE);

			// 現在のターンキャラに枠を表示
			if (i == static_cast<size_t>(currentTurnIndex)) {
				DrawBox(drawX, y, drawX + ICON_SIZE, y + ICON_SIZE, GetColor(255, 255, 0), FALSE);
			}

			// 名前または略称を下に表示
			SetFontSize(FONT_MINSIZE);
			std::string name = ch->getName();
			if (name.size() > nameMax) {
				name = name.substr(0, nameMax);
			}
			DrawFormatString(drawX, y + ICON_SIZE + 4, GetColor(255, 255, 255), "%s",name.c_str()); 
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
		if (CheckBoxClick(ButtonX, atButtonY, ButtonSizeX,ButtonSizeY)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
			arg_character->setActionChoice(ATTACK);
			targetInput = TargetInput::LISTCREATE;
		}
		else if (CheckBoxClick(ButtonX, skButtonY, ButtonSizeX, ButtonSizeY)) {

			// spが0より大きいなら（1以上なら）
			if (sp->getSP() > 0) {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				arg_character->setActionChoice(SKILL);
				targetInput = TargetInput::LISTCREATE;
			}
			else {
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			}
		}

		
	}

	//　ターゲットリスト作成
	void TargetListCreate(std::shared_ptr<Character> arg_character) {

		// ターゲット選択
		if (arg_character->getActionChoice() == ATTACK) {		// 通常攻撃時

			// リスト生成
			for (auto& e : enemies) {
				// ターゲット決定
				if (e->getAlive()) TargetList.push_back(e);
			}
			// ターゲット選択モードオン
			targetInput = TargetInput::TARGETCHOICE;
		}
		else {												// スキル時

			// ターゲットタイプ分け処理
			switch (arg_character->getTargetType()) {

			case SkillTargetType::SINGLE_ENEMY:

				// リスト生成
				for (auto& e : enemies) {
					if (e->getAlive()) TargetList.push_back(e);
				}
				// ターゲット選択モードオン
				targetInput = TargetInput::TARGETCHOICE;

				break;

			case SkillTargetType::SINGLE_ALLY:

				// リスト生成
				for (auto& p : Manager::Instance().getParty()) {
					if (p->getAlive()) TargetList.push_back(p);
				}
				// ターゲット選択モードオン
				targetInput = TargetInput::TARGETCHOICE;

				break;

			case SkillTargetType::SELF:

				// リスト生成
				TargetList.push_back(arg_character);
				// 対象者決定
				selectTarget = arg_character;
				// 型変換
				TypeConversion();
				//　アクションに移行
				arg_character->takeAction(v);
				// spの更新
				updataSP(arg_character);
				// ターゲット選択モードをオフ
				targetInput = TargetInput::END;

				break;

			case SkillTargetType::ALL_ENEMY:

				for (auto& e : enemies) {
					if (e->getAlive()) TargetList.push_back(e);
				}
				//　アクションに移行
				arg_character->takeAction(TargetList);
				// spの更新
				updataSP(arg_character);
				// ターゲット選択モードをオフ
				targetInput = TargetInput::END;


				break;

			case SkillTargetType::ALL_ALLY:

				for (auto& p : Manager::Instance().getParty()) {
					if (p->getAlive()) TargetList.push_back(p);
				}
				//　アクションに移行
				arg_character->takeAction(TargetList);
				// spの更新
				updataSP(arg_character);
				// ターゲット選択モードをオフ
				targetInput = TargetInput::END;

				break;

			}

		}

	}

	// ターゲットの選択
	void TargetChoice(std::shared_ptr<Character> arg_character) {
		
		// 行動キャンセル
		// 指定の場所をクリックした際（またはescキー）、行動選択に戻る。
		if (CheckBoxClick(WINDOW_W - 130, 70, FONT_BIGSIZE * 2, FONT_BIGSIZE)) {
			se->PlaySe(CLoad::Instance().getSeHandle(SE_CANCEL));
			targetInput = TargetInput::ACTIONCHOICE;
		}

		// 単一ターゲット選択
		for (auto& tar : TargetList) {
			// クリック入力待ち状態
			// クリックが入力されたら対象を決定
			Position pos = GetCharacterCenter(tar);

			if (CheckCircleClick(pos.x, pos.y, 40.0f)) {
				// se再生
				se->PlaySe(CLoad::Instance().getSeHandle(SE_CLICK));
				// 対象者決定
				selectTarget = tar;
				// 型変換
				TypeConversion();
				// スキル使用可能か
				if (arg_character->getActionChoice() == SKILL && !sp->comfirmSP()) { return; }
				//　アクションに移行
				arg_character->takeAction(v);

				if (arg_character->getActionChoice() == SKILL) {
					switch (arg_character->getEfType())
					{
					case EffectType::EF_NONE:
						break;
					case EffectType::EF_SKILL1:
						arg_character->PlaySkillEffect(selectTarget->getPosX(), selectTarget->getPosY(), 300, 250);
						// se再生
						se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL1));
						break;
					case EffectType::EF_SKILL2:
						arg_character->PlaySkillEffect(selectTarget->getPosX(), selectTarget->getPosY(), 250, 250);
						// se再生
						se->PlaySe(CLoad::Instance().getSeHandle(SE_SKILL2));
						break;
					case EffectType::EF_HEAL:
						arg_character->PlaySkillEffect(selectTarget->getPosX(), selectTarget->getPosY(), 30, 40);
						// se再生
						se->PlaySe(CLoad::Instance().getSeHandle(SE_HEAL));
						break;
					case EffectType::EF_BUFF:
						arg_character->PlaySkillEffect(selectTarget->getPosX(), selectTarget->getPosY(), 50, 40);
						// se再生
						se->PlaySe(CLoad::Instance().getSeHandle(SE_BUFF));
						break;

					default:
						break;
					}
				}
				else {
					arg_character->PlayAttackEffect(selectTarget->getPosX(), selectTarget->getPosY(), 50, 40);
					// se再生
					se->PlaySe(CLoad::Instance().getSeHandle(SE_ATTACK));
				}
				// ターゲット選択モードをオフ
				targetInput = TargetInput::END;

				break;
			}
		}

	}

	// エネミー
	void EnemyAction(std::shared_ptr<Character> arg_character) {
		for (auto& p : Manager::Instance().getParty()) {
			if (p->getAlive()) TargetList.push_back(p);
		}

		// 疑似乱数設定
		int index = rand() % TargetList.size();
		selectTarget = TargetList[index];
		TypeConversion();
		//　アクションに移行
		arg_character->takeAction(v);

		// 
		auto& p =  Manager::Instance().getParty();
		for (size_t i = 0; i < Manager::Instance().getParty().size(); i++) {
			if (TargetList[index]->getId() == p[i]->getId()) {
				// ダメージを与える。
				p[i]->setAnimType(DAMAGE);
			}
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

		// ターゲット選択モードをオフ
		targetInput = TargetInput::P_NONE;
		// spの更新
		updataSP(arg_character);
		// インデックスをインクリメントして次のキャラクターの行動へ		
		currentTurnIndex++;
			
		

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
		if (!turnOrder.empty())turnOrder.clear();
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

};