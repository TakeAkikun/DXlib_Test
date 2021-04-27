//DxLibのヘッダファイル（必須）
#include "DxLib.h"

//マクロ定義
#define GAME_TITLE "ゲームタイトル" //ゲームタイトル
#define GAME_WIDTH   1280            //ゲーム画面の幅
#define GAME_HEIGHT  720             //ゲーム画面の幅
#define GAME_COLOR   32              //ゲーム画面の色数

#define GAME_ICON_ID 333             //ゲームのアイコンのID

// プログラムは WinMain から始まります
// Windousのプログラミング方法で動いている。（WinAPI）
// DxLibはDirectXというゲームプログラミングを簡単に使える仕組み
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{


	ChangeWindowMode(TRUE);                              //ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);                       //ゲームタイトルを設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);   //ゲームの解像度の設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);              //ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);                   //ウィンドウのデフォの色を設定
	SetWindowIconID(GAME_ICON_ID);                       //アイコンファイルを読み込み



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			    // エラーが起きたら直ちに終了
	}
	
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
			break;            //エラーやウィンドウが閉じられたらループを抜ける
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				    // ソフトの終了 
}