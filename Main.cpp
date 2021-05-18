#include "DxLib.h"      //DxLibのヘッダファイル（必須）
#include "keyboard.h"   //キーボードの処理


//マクロ定義
#define GAME_TITLE "ゲームタイトル" //ゲームタイトル
#define GAME_WIDTH   1280            //ゲーム画面の幅
#define GAME_HEIGHT  720             //ゲーム画面の幅
#define GAME_COLOR   32              //ゲーム画面の色数

#define GAME_ICON_ID 333             //ゲームのアイコンのID

#define GAME_WINDOW_BAR 0            //ウィンドウバーの種類

enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};  //ゲームのシーン

//グローバル変数

//画面の切り替え
BOOL IsFadeOut = FALSE;   //フェードアウト
BOOL IsFadeIn = FALSE;    //フェードイン

int fadeTimeMill = 2000;                    //切り替えミリ秒
int fadeTimeMax = fadeTimeMill / 1000 * 60; //ミリ秒をフレーム秒に変換

//フェードアウト
int fadeOutCntInit = 0;                     //初期値
int fadeOutCnt = fadeOutCntInit;            //フェードアウトのカウンタ
int fadeOutCutMax = fadeTimeMax;            //フェードアウトのカウンタXAX

//フェードイン
int fadeInCntInit = fadeTimeMax;            //初期値
int fadeInCnt = fadeInCntInit;              //フェードインのカウンタ
int fadeInCutMax = fadeTimeMax;                       //フェードインのカウンタXAX

//シーンを管理する変数
GAME_SCENE GameScene;       //現在のゲームのシーン
GAME_SCENE OldGameScene;    //前回のゲームのシーン
GAME_SCENE NextGameScene;   //次回のゲームのシーン

//プロトタイプ宣言
VOID Title(VOID);     //タイトル画面
VOID TitleProc(VOID); //タイトル画面（処理）
VOID TitleDraw(VOID); //タイトル画面（描画）

VOID Play(VOID);					 //プレイ画面
VOID PlayProc(VOID);				 //プレイ画面（処理）
VOID PlayDraw(VOID);				 //プレイ画面（描画）

VOID End(VOID);						 //エンド画面
VOID EndProc(VOID);					 //エンド画面（処理）
VOID EndDraw(VOID);					 //エンド画面（描画）

VOID Change(VOID);				     //切り替え画面
VOID ChangeProc(VOID);				 //切り替え画面（処理）
VOID ChangeDraw(VOID);				 //切り替え画面（描画）

VOID ChangeScene(GAME_SCENE seane);  //シーン切り替え


// プログラムは WinMain から始まります
// Windousのプログラミング方法で動いている。（WinAPI）
// DxLibはDirectXというゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//こいつらは必ずInitを行う前に入力する
	SetOutApplicationLogValidFlag(FALSE);                //Log.txtを出力しない
	ChangeWindowMode(TRUE);                              //ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);                       //ゲームタイトルを設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);   //ゲームの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);              //ウィンドウの大きさを設定
	SetWindowStyleMode(GAME_WINDOW_BAR);                 //ウィンドウバーの状態
	SetBackgroundColor(255, 255, 255);                   //ウィンドウのデフォの色を設定
	SetWindowIconID(GAME_ICON_ID);                       //アイコンファイルを読み込み
	SetWaitVSyncFlag(TRUE);                              //ディスプレイの垂直同期を有効にする
	SetAlwaysRunFlag(TRUE);                              //ウィンドウをずっとアクティブにする



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			    // エラーが起きたら直ちに終了
	}

	//ダブルバッファリング有効化
	SetDrawScreen(DX_SCREEN_BACK);

	//円の中心点
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//円の半径
	int radius = 50;

	//スピード
	int Xspead = 5;
	int Yspead = 5;


	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//無限ループ
	while (1)
	{
	    
		if (ProcessMessage() != 0) { break; }  //メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; } //画面をクリア

		AllKeyUpdate();         //キーボード入力の更新

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//以前のシーンを取得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();            //タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();             //プレイ画面
			break;
		case GAME_SCENE_END:
			End();              //エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();           //切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//以前のシーンが切り替え画面でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;     //次のシーンを保存
				GameScene = GAME_SCENE_CHANGE; //画面切り替えシーンに変える
			}
		}


		//キー入力
		if (KeyDown(KEY_INPUT_UP) == TRUE && Y > 0 + radius)
		{
			Y -= Yspead;   //上に移動
			if (Y < 0 + radius)
			{
				Y = 0 + radius;
			}
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Y < GAME_HEIGHT - radius)
		{
			Y += Yspead;   //下に移動
			if (Y > GAME_HEIGHT - radius)
			{
				Y = GAME_HEIGHT - radius;
			}
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE && X > 0 + radius)
		{
			X -= Xspead;   //左に移動
			if (X < 0 + radius)
			{
				X = 0 + radius;
			}
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && X < GAME_WIDTH - radius)
		{
			X += Xspead;   //右に移動
			if (X > GAME_WIDTH - radius)
			{
				X = GAME_WIDTH - radius;
			}
		}

		// １でスピードUP・２でスピードDOWN（0の時はもう下がらない）
		if (KeyDown(KEY_INPUT_1) == TRUE)
		{
			Xspead++;
			Yspead++;
		}
		if (KeyDown(KEY_INPUT_2) == TRUE && Xspead > 0 && Yspead > 0)
		{
			Xspead--;
			Yspead--;
		}

		DrawCircle(X, Y, radius, GetColor(0, 255, 0), TRUE);

		ScreenFlip();           //ダブルバッファリングした画面を描画
	}


	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				    // ソフトの終了 
}


/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;  //シーン切り替え
	IsFadeIn = FALSE;             //フェードインしない
	IsFadeOut = TRUE;             //フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();    //処理
	TitleDraw();    //描画

	return;
}

/// <summary>
/// タイトル画面の処理
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	//プレイシーンに切り替え
	if (KeyClick(KEY_INPUT_RETURN) == TRUE){
		//シーン切り替え
		//次のシーンの初期化をココで行うと楽

		//プレイ画面に切り替え
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// プレイ画面
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();    //処理
	PlayDraw();    //描画

	return;
}

/// <summary>
/// プレイ画面の処理
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	//エンドシーンに切り替え
	if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
		//シーン切り替え
		//次のシーンの初期化をココで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);
	}

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	DrawString(0, 20, "1でスピードアップ：2でスピードダウン", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// エンド画面
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();    //処理
	EndDraw();    //描画

	return;
}

/// <summary>
/// エンド画面の処理
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	//タイトルシーンに切り替え
	if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
		//シーン切り替え
		//次のシーンの初期化をココで行うと楽

		//タイトル画面に切り替え
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// 切り替え画面
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();    //処理
	ChangeDraw();    //描画

	return;
}

/// <summary>
/// 切り替え画面の処理
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCutMax)
		{
			fadeInCnt--;               //カウントを減らす
		}
		else
		{
			//フェードイン処理が終わった
			fadeInCnt = fadeInCntInit; //カウンタ初期化
			IsFadeIn = FALSE;          //フェードイン処理終了
		}
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCutMax)
		{
			fadeOutCnt++;               //カウントを減らす
		}
		else
		{
			//フェードアウト処理が終わった
			fadeOutCnt = fadeOutCntInit; //カウンタ初期化
			IsFadeOut = FALSE;          //フェードアウト処理終了
		}
	}

	//切り替え処理終了か？
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードインもフェードアウトもしていない時
		GameScene = NextGameScene;  //次のシーンに切り替え
		OldGameScene = GameScene;   //以前のゲームシーン更新
	}

	return;
}

/// <summary>
/// 切り替え画面の描画
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();      //タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();       //プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();        //エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCutMax) * 255);
	}

	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCutMax) * 255);
	}

	//四角形を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}