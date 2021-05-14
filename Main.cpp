#include "DxLib.h"      //DxLibのヘッダファイル（必須）
#include "keyboard.h"   //キーボードの処理


//マクロ定義
#define GAME_TITLE "ゲームタイトル" //ゲームタイトル
#define GAME_WIDTH   1280            //ゲーム画面の幅
#define GAME_HEIGHT  720             //ゲーム画面の幅
#define GAME_COLOR   32              //ゲーム画面の色数

#define GAME_ICON_ID 333             //ゲームのアイコンのID

#define GAME_WINDOW_BAR 0            //ウィンドウバーの種類

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


	//無限ループ
	while (1)
	{
	    
		if (ProcessMessage() != 0) { break; }  //メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; } //画面をクリア

		AllKeyUpdate();         //キーボード入力の更新

		//キー入力
		if (KeyDown(KEY_INPUT_UP) == TRUE && Y > 0 + radius)
		{
			Y -= Yspead;   //上に移動
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Y < GAME_HEIGHT - radius)
		{
			Y += Yspead;   //下に移動
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE && X > 0 + radius)
		{
			X -= Xspead;   //左に移動
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && X < GAME_WIDTH - radius)
		{
			X += Xspead;   //右に移動
		}

		// １でスピードUP・２でスピードDOWN
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