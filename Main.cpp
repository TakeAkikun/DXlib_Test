//DxLibのヘッダファイル（必須）
#include "DxLib.h"

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



	//円の位置を決める
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//四角形の大きさを決める
	int width = 10;
	int height = 10;

	//円の半径を決める
	int radius = 25;

	//動く速度を決める
	int Xspead = 1;
	int Yspead = 1;

	//スピードアップする回数を決める
	int Scount = 3;  //縦横計3回

	//
	SetDrawScreen(DX_SCREEN_BACK);

	
	//無限ループ
	while (1)
	{
		                        //何かしらキーが押されたらループを抜ける
		if (CheckHitKeyAll() != 0)
		{
			break;
		}
		                        //メッセージを受け取り続ける
		if (ProcessMessage() != 0) 
		{
			break;              //エラーやウィンドウが閉じられたらループを抜ける
		}

		//画面をクリア
		if (ClearDrawScreen() != 0) { break; }

		//四角形を描く
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 0, 0),
		//	TRUE                //塗りつぶすかどうか
		//);

		//円を描く
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
			FALSE,5
		);

			X += Xspead;         //四角形を右下に動かす
			Y += Yspead;

			if (X - radius < 0 || X > GAME_WIDTH - radius)
			{
				Xspead = -Xspead;

				if (Scount > 0)
				{
					if (Xspead > 0) { Xspead += 2; }
					else if (Xspead < 0) { Xspead -= 2; }
					Scount--;
				}
			}
		
			if (Y - radius < 0 || Y > GAME_HEIGHT - radius)
			{
				Yspead = -Yspead;

				if (Scount > 0)
				{
					if (Yspead > 0) { Yspead += 2; }
					else if (Yspead < 0) { Yspead -= 2; }
					Scount--;
				}
			}

		/*
		if (X <= 250 && Y <= 250)
		{
			X++;                 //四角形を右下に動かす
			Y++;
		}

		if (X > 250 || Y > 250)
		{
			X++;
			if (Y > 150 && Y < 350)
			{
				Y--;
			}
			
		}
		*/

		ScreenFlip();           //ダブルバッファリングした画面を描画
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				    // ソフトの終了 
}