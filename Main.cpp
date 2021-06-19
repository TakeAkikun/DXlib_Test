
//ヘッダファイル読み込み
#include "geme.h"       //ゲーム全体のヘッダファイル
#include "keyboard.h"   //キーボードの処理
#include "FPS.h"        //FPSの処理

//=========================================================
//     構造体    
//=========================================================

//画像の構造体
struct IMAGE
{
	int handle = -1;  //画像のハンドル（管理番号）
	char path[255];   //画像の場所（パス）

	int X;            //X位置
	int Y;            //Y位置
	int width;        //幅
	int height;       //高さ

	BOOL IsDraw = FALSE;//画像が描画できる？
};

//キャラクターの構造体
struct CHARACTOR
{
	IMAGE img;        //画像の構造体
	int Xspead = 1;   //移動速度
	int Yspead = 1;   //移動速度
	RECT coll;        //当たり判定の領域(四角)
};

//動画の構造体
struct MOVIE
{
	int handle = -1;    //動画のハンドル
	char path[25];      //動画のパス

	int x;              //X位置
	int y;              //Y位置
	int width;          //幅
	int height;         //高さ

	int Volume = 255;   //ボリューム（0～255）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;    //音楽のハンドル
	char path[255];     //音楽のパス

	int Volume = -1;    //ボリューム（MIN 0～255 MAX）
	int playType = -1;  //BGM or SE
};

//=========================================================
//     変数作成     
//=========================================================

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
int fadeInCutMax = fadeTimeMax;             //フェードインのカウンタXAX

//タイトルロゴのだんだん濃くなるアレ
float TitleLogoCnt = 0;           //カウンタ
BOOL TitleLogoBrink = FALSE;      //完全に表示されたか？

//PushEnterの点滅
int PushEnterCnt = 0;           //カウンタ
const int PushEnterCntMax = 60; //カウンタMAX
BOOL PushEnterBrink = FALSE;    //点滅しているか？

//シーンを管理する変数
GAME_SCENE GameScene;       //現在のゲームのシーン
GAME_SCENE OldGameScene;    //前回のゲームのシーン
GAME_SCENE NextGameScene;   //次回のゲームのシーン

//メニュー欄の表示フラグ
BOOL MenuFlag = FALSE;    //メニュー欄を表示するか？

//ヒント欄の表示フラグ
BOOL TipsFlag = FALSE;    //ヒント欄を表示するか？

//エンド画面の時のフラグ
int GameEndFlag = -1;

//プレイヤー
CHARACTOR Player;

//敵
CHARACTOR Enemy1;
CHARACTOR Enemy2;
CHARACTOR Enemy3;

//ゴール
CHARACTOR Goal;

//ロゴと背景
IMAGE TitleLogo;        //タイトルロゴ
IMAGE TitleEnter;       //PushEnterのロゴ
IMAGE TitleImg;         //タイトル画面の背景
IMAGE EndClear;         //GameClearのロゴ
IMAGE EndClearImg;      //ゲームクリア画面の背景
IMAGE EndOver;          //GameOverのロゴ
IMAGE EndOverImg;       //ゲームオーバー画面の背景
IMAGE MenuIcon;         //メニューのアイコン
IMAGE MenuIconPushVer;  //メニュー画面を開いている時のアイコン
IMAGE MenuScreen;       //メニュー画面
IMAGE VolumePointer;    //音量調整の時の移動するひし形のアレ
IMAGE TipsIcon;         //ヒントのアイコン
IMAGE TipsIconPushVer;  //ヒント画面を開いている時のアイコン
IMAGE TipsScreen;       //ヒント画面

//プレイ画面の背景の動画
MOVIE playMovie;

//音楽
AUDIO TitleBGM;         //タイトルのBGM
AUDIO PlayBGM;          //プレイのBGM
AUDIO EndBGM;           //エンドのBGM
AUDIO moveSE;           //プレイヤーが動く時のSE
AUDIO PushEnterSE;      //シーン推移の時のSE
AUDIO menuSE;           //メニュー画面を開く(閉じる)時のSE
AUDIO tipsSE;           //ヒント画面を開く(閉じる)時のSE

//=========================================================
//     関数     
//=========================================================

//キャメルケース・・・単語の先頭を大文字にする命名規則
//スネークケース・・・単語をアンダーバーでつなぐ命名規則

VOID Title(VOID);     //タイトル画面
VOID TitleProc(VOID); //タイトル画面（処理）
VOID TitleDraw(VOID); //タイトル画面（描画）

VOID Play(VOID);					                             //プレイ画面
VOID PlayProc(VOID);				                             //プレイ画面（処理）
VOID PlayDraw(VOID);				                             //プレイ画面（描画）

VOID End(VOID);						                             //エンド画面
VOID EndProc(VOID);					                             //エンド画面（処理）
VOID EndDraw(VOID);					                             //エンド画面（描画）

VOID Change(VOID);					                             //切り替え画面
VOID ChangeProc(VOID);				                             //切り替え画面（処理）
VOID ChangeDraw(VOID);				                             //切り替え画面（描画）

VOID DrawMenu(int Volum);			                             //メニュー画面（描画）
VOID DrawTips(VOID);				                             //ヒント画面（描画）

VOID ChangeScene(GAME_SCENE seane, AUDIO* OldBGM, AUDIO* NextBGM);//シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);                         //当たり判定の領域を更新(プレイヤー)
VOID CollUpdate(CHARACTOR* chara);                               //当たり判定の領域を更新

BOOL OnCollision(RECT coll1, RECT coll2);                       //当たっているかを調べる

VOID ChangeBGM(AUDIO* music);                                    //BGMの音量変更

BOOL GameLoad(VOID);                                             //ゲーム全体のデータを読み込み
VOID GameInit(VOID);                                             //ゲームデータの初期化
BOOL ImageInput(IMAGE* Image, const char* path);                 //ゲームの画像読み込み
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType);//ゲームのBGM読み込み

//=====================================================================================================================
//          ココからメインプログラム          
//=====================================================================================================================

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

	//ゲーム全体の初期化
	if (!GameLoad())
	{
		//ゲームデータの読み込みに失敗したとき
		DxLib_End();  //DxLib終了
		return -1;    //異常終了
	}

	//ゲームデータの初期化
	GameInit();

	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//無限ループ
	while (1)
	{

		if (ProcessMessage() != 0) { break; }  //メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; } //画面をクリア

		AllKeyUpdate();         //キーボード入力の更新

		//FPS値の更新
		FPSUpdate();

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

		//FPS値を描画
		FPSDraw();

		//FPS値を待つ
		FPSWait();

		ScreenFlip();           //ダブルバッファリングした画面を描画
	}

	//終わるときの処理
	DeleteGraph(Player.img.handle);      //画像をメモリ上から削除
	DeleteGraph(Enemy1.img.handle);      //
	DeleteGraph(Enemy2.img.handle);      //
	DeleteGraph(Enemy3.img.handle);      //
	DeleteGraph(Goal.img.handle);        //
	DeleteGraph(TitleImg.handle);        //
	DeleteGraph(EndClearImg.handle);     //
	DeleteGraph(EndOverImg.handle);      //
	DeleteGraph(MenuIcon.handle);        //
	DeleteGraph(MenuIconPushVer.handle); //
	DeleteGraph(MenuScreen.handle);      //
	DeleteGraph(VolumePointer.handle);   //
	DeleteGraph(TipsScreen.handle);      //
	DeleteGraph(TipsIcon.handle);        //
	DeleteGraph(TipsIconPushVer.handle); //
	DeleteGraph(playMovie.handle);       //動画をメモリ上から削除
	DeleteGraph(TitleBGM.handle);        //音楽をメモリ上から削除
	DeleteGraph(PlayBGM.handle);         //
	DeleteGraph(EndBGM.handle);          //
	DeleteGraph(moveSE.handle);          //SEをメモリ上から削除
	DeleteGraph(PushEnterSE.handle);     //
	DeleteGraph(menuSE.handle);          //
	DeleteGraph(tipsSE.handle);          //

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				    // ソフトの終了 
}


//=====================================================================================================================
//          ココから初期化関係          
//=====================================================================================================================


/// <summary>
/// ゲームデータの初期化
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//プレイヤーを初期化
	Player.img.X = 0;
	Player.img.Y = 0;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;

	//当たり判定を更新
	CollUpdatePlayer(&Player);  //プレイヤーの当たり判定のアドレス

	//敵１を初期化
	Enemy1.img.X = 350;
	Enemy1.img.Y = 200;
	Enemy1.Xspead = 10;
	Enemy1.Yspead = 10;
	Enemy1.img.IsDraw = TRUE;

	//当たり判定の更新
	CollUpdatePlayer(&Enemy1);  //敵の当たり判定のアドレス

	//敵２を初期化
	Enemy2.img.X = 500;
	Enemy2.img.Y = 400;
	Enemy2.Xspead = 10;
	Enemy2.Yspead = 10;
	Enemy2.img.IsDraw = TRUE;

	//当たり判定の更新
	CollUpdatePlayer(&Enemy2);  //敵の当たり判定のアドレス

	//敵３を初期化
	Enemy3.img.X = 900;
	Enemy3.img.Y = 500;
	Enemy3.Xspead = 10;
	Enemy3.Yspead = 10;
	Enemy3.img.IsDraw = TRUE;

	//当たり判定の更新
	CollUpdatePlayer(&Enemy3);  //敵の当たり判定のアドレス

	//ゴールを初期化
	Goal.img.X = GAME_WIDTH - Goal.img.width;
	Goal.img.Y = GAME_HEIGHT - Goal.img.height;
	Goal.Xspead = 300;
	Goal.Yspead = 300;
	Goal.img.IsDraw = TRUE;

	//当たり判定を更新
	CollUpdate(&Goal);  //ゴールの当たり判定のアドレス

	//タイトルロゴの位置
	TitleLogo.X = GAME_WIDTH / 2 - TitleLogo.width / 2;  //中央揃え
	TitleLogo.Y = 10;

	//タイトルロゴのだんだん濃くなるアレ
	TitleLogoCnt = 0;          //カウンタ
	TitleLogoBrink = FALSE;    //完全に表示されたか？

	//PushEnterの位置
	TitleEnter.X = GAME_WIDTH / 2 - TitleEnter.width / 2;//中央揃え
	TitleEnter.Y = GAME_HEIGHT - TitleEnter.height - 100;

	//PushEnterの点滅
	PushEnterCnt = 0;         //カウンタ
	PushEnterBrink = FALSE;  //点滅しているか？

	//クリアロゴの位置
	EndClear.X = GAME_WIDTH / 2 - EndClear.width / 2;       //中央揃え
	EndClear.Y = GAME_WIDTH / 2 - EndClear.height / 2 - 300;//中央揃え

	//オーバーロゴの位置
	EndOver.X = GAME_WIDTH / 2 - EndOver.width / 2;         //中央揃え
	EndOver.Y = GAME_WIDTH / 2 - EndOver.height / 2 - 300;  //中央揃え

	//メニューアイコンの位置
	MenuIcon.X = GAME_WIDTH - MenuIcon.width - 10;          //右上
	MenuIcon.Y = 10;                                        //

	//メニュー画面が開いているときのメニューアイコンの位置
	MenuIconPushVer.X = GAME_WIDTH - MenuIconPushVer.width - 10;   //メニューアイコンと同じ位置
	MenuIconPushVer.Y = 10;                                        //

	//メニュー画面の位置
	MenuScreen.X = GAME_WIDTH / 2 +112;                     //大分右
	MenuScreen.Y = 0;                                       //

	//ヒントアイコンの位置
	TipsIcon.X = GAME_WIDTH - MenuIcon.width - TipsIcon.width - 20;//メニューアイコンの左隣
	TipsIcon.Y = 10;                                               //

	//ヒント画面が開いているときのヒントアイコンの位置
	TipsIconPushVer.X = GAME_WIDTH - MenuIcon.width - TipsIconPushVer.width - 20;//ヒントアイコンと同じ位置
	TipsIconPushVer.Y = 10;                                                      //

	//ヒント画面の位置
	TipsScreen.X = GAME_WIDTH / 2 + 100;                    //メニュー画面と大体同じ位置
	TipsScreen.Y = 0;                                       //

	//音量調整の動くヤツの位置
	VolumePointer.X = GAME_WIDTH / 2 + 320;                 //丁度Volumeのバーの一番右に重なるように
	VolumePointer.Y = 215;                                  //
}


/// <summary>
/// ゲーム全体の初期化
/// </summary>
/// <returns>読み込み成功・・・TRUE｜読み込み失敗・・・FALSE</returns>
BOOL GameLoad()
{
	//プレイ動画の背景を読み込み
	strcpyDx(playMovie.path, ".\\movie\\PlayMovie.mp4");  //パスのコピー
	playMovie.handle = LoadGraph(playMovie.path);        //動画の読み込み

	//画像が読み込めなかったときは、エラー(－1)が入る
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //メインのウィンドウタイトル
			playMovie.path,          //メッセージ本文
			"画像読み込みエラー",    //メッセージタイトル
			MB_OK                    //ボタン
		);

		return FALSE;                //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;

	//画像を読み込み
	if (!ImageInput(&TitleLogo, ".\\image\\Title.png")) { FALSE; }
	if (!ImageInput(&TitleEnter, ".\\image\\PushEnter.png")) { FALSE; }
	if (!ImageInput(&TitleImg, ".\\image\\TitleImg.jpeg")) { FALSE; }
	if (!ImageInput(&EndClear, ".\\image\\GameClear.png")) { FALSE; }
	if (!ImageInput(&EndClearImg, ".\\image\\EndClearImg.jpg")) { FALSE; }
	if (!ImageInput(&EndOver, ".\\image\\GameOver.png")) { FALSE; }
	if (!ImageInput(&EndOverImg, ".\\image\\EndOverImg.jpg")) { FALSE; }
	if (!ImageInput(&MenuIcon, ".\\image\\gear.png")) { FALSE; }
	if (!ImageInput(&MenuIconPushVer, ".\\image\\gearPush.png")) { FALSE; }
	if (!ImageInput(&MenuScreen, ".\\image\\menu.png")) { FALSE; }
	if (!ImageInput(&TipsIcon, ".\\image\\TipsIcon.png")) { FALSE; }
	if (!ImageInput(&TipsIconPushVer, ".\\image\\TipsIconPushVer.png")) { FALSE; }
	if (!ImageInput(&TipsScreen, ".\\image\\Tips.png")) { FALSE; }
	if (!ImageInput(&VolumePointer, ".\\image\\volumePointer.png")) { FALSE; }
	if (!ImageInput(&Player.img, ".\\image\\Player.png")) { FALSE; }
	if (!ImageInput(&Enemy1.img, ".\\image\\Enemy1.png")) { FALSE; }
	if (!ImageInput(&Enemy2.img, ".\\image\\Enemy1.png")) { FALSE; }
	if (!ImageInput(&Enemy3.img, ".\\image\\Enemy1.png")) { FALSE; }
	if (!ImageInput(&Goal.img, ".\\image\\Goal.png")) { FALSE; }

	//音楽を読み込み
	if (!MusicInput(&TitleBGM, ".\\audio\\OP.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&PlayBGM, ".\\audio\\PLAY.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&EndBGM, ".\\audio\\ED.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&moveSE, ".\\SE\\move.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&PushEnterSE, ".\\SE\\push.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&menuSE, ".\\SE\\menu.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&tipsSE, ".\\SE\\tips.mp3", 150, DX_PLAYTYPE_BACK)) { return FALSE; }


	return TRUE;        //全部読み込めたらTRUE
}


//=====================================================================================================================
//          ココから読み込み関係          
//=====================================================================================================================


/// <summary>
/// 画像をメモリに読み込み
/// </summary>
/// <param name="Image">Image構造体のアドレス</param>
/// <param name="path">Imageの画像パス</param>
/// <returns>TRUE or FALSE</returns>
BOOL ImageInput(IMAGE* Image, const char* path)
{
	//ゴールの画像を読み込み
	strcpyDx(Image->path, path);  //パスのコピー
	Image->handle = LoadGraph(Image->path);          //画像の読み込み

	//画像が読み込めなかったときは、エラー(－1)が入る
	if (Image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //メインのウィンドウタイトル
			Image->path,             //メッセージ本文
			"画像読み込みエラー",    //メッセージタイトル
			MB_OK                    //ボタン
		);

		return FALSE;                //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(Image->handle, &Image->width, &Image->height);
}


/// <summary>
/// 音楽をメモリに読み込み
/// </summary>
/// <param name="music">Audio構造体のアドレス</param>
/// <param name="path">Audioの音楽パス</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns>TRUE or FALSE</returns>
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType)
{
	//音楽を読み込み
	strcpyDx(music->path, path);  //パスのコピー
	music->handle = LoadSoundMem(music->path);          //音楽の読み込み

	//音楽が読み込めなかったときは、エラー(－1)が入る
	if (music->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),       //メインのウィンドウタイトル
			music->path,                 //メッセージ本文
			"音楽読み込みエラー",        //メッセージタイトル
			MB_OK                        //ボタン
		);

		return FALSE;                    //エラー終了
	}

	music->playType = playType;                          //音楽をループさせる
	music->Volume = volume;                              //MAXが255
	ChangeVolumeSoundMem(music->Volume, music->handle);  //BGMのボリュームを変更

	return TRUE;
}


//=========================================================
//     下記は切り替え関数     
//=========================================================


/// <summary>
/// シーンを切り替える関数
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene, AUDIO* OldBGM, AUDIO* NextBGM)
{
	GameScene = scene;  //シーン切り替え
	IsFadeIn = FALSE;             //フェードインしない
	IsFadeOut = TRUE;             //フェードアウトする

	//前のシーンのBGMの音量を次のBGMに引継ぎ
	NextBGM->Volume = OldBGM->Volume;
	ChangeVolumeSoundMem(NextBGM->Volume, NextBGM->handle);

	return;
}


//=====================================================================================================================
//           ココからタイトル          
//=====================================================================================================================

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
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//タイトルロゴの降りてくる演出
		//Max値まで待つ
		if (TitleLogoCnt < TitleLogo.Y)
		{
			TitleLogoCnt += 0.1;
		}
		else
		{

			TitleLogoBrink = TRUE;
		}


		//タイトルロゴの演出が終わった後に表示
		if (TitleLogoBrink == TRUE)
		{

			//PushEnterの点滅
			//MAX値まで待つ
			if (PushEnterCnt < PushEnterCntMax)
			{
				PushEnterCnt++;
			}
			else
			{
				if (PushEnterBrink == TRUE) PushEnterBrink = FALSE;
				else if (PushEnterBrink == FALSE) PushEnterBrink = TRUE;

				PushEnterCnt = 0;
			}

		}

		//プレイシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//SE
			PlaySoundMem(PushEnterSE.handle, PushEnterSE.playType);

			//BGMを止める
			StopSoundMem(TitleBGM.handle);

			//ゲームデータの初期化
			GameInit();

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_PLAY, &TitleBGM, &PlayBGM);

			return;
		}
	}

	//BGMが流れていない時
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	//音量の変更
	ChangeBGM(&TitleBGM);

	return;
}

/// <summary>
/// タイトル画面の描画
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	//背景を描画
	DrawGraph(TitleImg.X, TitleImg.Y, TitleImg.handle, TRUE);

	//タイトルロゴの描画

	if (TitleLogoBrink == FALSE)
	{
		//半透明にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)TitleLogoCnt / TitleLogo.Y) * 255);
		//ロゴを描画
		DrawGraph(TitleLogo.X, TitleLogoCnt, TitleLogo.handle, TRUE);
		//半透明終了
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
	//ロゴを描画
	if (TitleLogoBrink == TRUE)
	{
		DrawGraph(TitleLogo.X, TitleLogoCnt, TitleLogo.handle, TRUE);
	}


	//タイトルロゴの演出が終わった後に表示
	if (TitleLogoBrink == TRUE)
	{

		//PushEnterを点滅

		if (PushEnterBrink == TRUE)
		{
			//半透明にする
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMax) * 255);

			//ロゴを描画
			DrawGraph(TitleEnter.X, TitleEnter.Y, TitleEnter.handle, TRUE);

			//半透明終了
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}

		if (PushEnterBrink == FALSE)
		{
			//半透明にする
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMax - PushEnterCnt) / PushEnterCntMax) * 255);

			//ロゴを描画
			DrawGraph(TitleEnter.X, TitleEnter.Y, TitleEnter.handle, TRUE);

			//半透明終了
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}

	}

	//フラグが立ったらメニューを表示
	DrawMenu(TitleBGM.Volume);

	//フラグが立ったらヒントを表示
	DrawTips();

	DrawString(0, 0, "タイトル画面", GetColor(0, 0, 0));

	return;
}


//=====================================================================================================================
//          ココからプレイ          
//=====================================================================================================================

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
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//プレイヤーの操作

			//壁を突き抜けないようにif文を調整
		if (KeyDown(KEY_INPUT_UP) == TRUE && Player.img.Y > 0)
		{
			Player.img.Y -= Player.Yspead * fps.DeltaTime;   //上に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.Y < 0)
			{
				Player.img.Y = 0;
			}

			//効果音
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.img.Y < GAME_HEIGHT - Player.img.height)
		{
			Player.img.Y += Player.Yspead * fps.DeltaTime;   //下に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.Y > GAME_HEIGHT - Player.img.height)
			{
				Player.img.Y = GAME_HEIGHT - Player.img.height;
			}

			//効果音
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.img.X > 0)
		{
			Player.img.X -= Player.Xspead * fps.DeltaTime;   //左に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.X < 0)
			{
				Player.img.X = 0;
			}

			//効果音
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.img.X < GAME_WIDTH - Player.img.width)
		{
			Player.img.X += Player.Xspead * fps.DeltaTime;   //右に移動

			//スピード高すぎてめり込むのを防止
			if (Player.img.X > GAME_WIDTH - Player.img.width)
			{
				Player.img.X = GAME_WIDTH - Player.img.width;
			}

			//効果音
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		//敵の動き

		//敵1
		Enemy1.img.X += Enemy1.Xspead;
		if (Enemy1.img.X < 0 || Enemy1.img.X + Enemy1.img.width > GAME_WIDTH)
		{
			Enemy1.Xspead = -Enemy1.Xspead;
		}

		//敵2
		Enemy2.img.Y += Enemy2.Yspead;
		if (Enemy2.img.Y < 0 || Enemy2.img.Y + Enemy2.img.width > GAME_HEIGHT)
		{
			Enemy2.Yspead = -Enemy2.Yspead;
		}

		//敵3
		Enemy3.img.Y += Enemy3.Yspead;
		if (Enemy3.img.Y < 0 || Enemy3.img.Y + Enemy3.img.width > GAME_HEIGHT)
		{
			Enemy3.Yspead = -Enemy3.Yspead;
		}

		// １でスピードUP・２でスピードDOWN（0の時はもう下がらない）
		if (KeyDown(KEY_INPUT_1) == TRUE)
		{
			Player.Xspead += 5;
			Player.Yspead += 5;
		}
		if (KeyDown(KEY_INPUT_2) == TRUE && Player.Xspead > 0 && Player.Yspead > 0)
		{
			Player.Xspead -= 5;
			Player.Yspead -= 5;
		}

		//当たり判定を更新
		CollUpdatePlayer(&Player);
		CollUpdatePlayer(&Enemy1);
		CollUpdatePlayer(&Enemy2);
		CollUpdatePlayer(&Enemy3);
		CollUpdate(&Goal);

		/*
		//エンドシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//プレイ画面に切り替え
			ChangeScene(GAME_SCENE_END);
		}
		*/

		//当たり判定のRECT（プレイヤーとゴール）
		Player.coll = { Player.img.X,Player.img.Y,Player.img.X + Player.img.width,Player.img.Y + Player.img.height };
		Enemy1.coll = { Enemy1.img.X,Enemy1.img.Y,Enemy1.img.X + Enemy1.img.width,Enemy1.img.Y + Enemy1.img.height };
		Enemy2.coll = { Enemy2.img.X,Enemy2.img.Y,Enemy2.img.X + Enemy2.img.width,Enemy2.img.Y + Enemy2.img.height };
		Enemy3.coll = { Enemy3.img.X,Enemy3.img.Y,Enemy3.img.X + Enemy3.img.width,Enemy3.img.Y + Enemy3.img.height };
		Goal.coll = { Goal.img.X,Goal.img.Y,Goal.img.X + Goal.img.width,Goal.img.Y + Goal.img.height };

		//ゴール判定
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Goal.coll) == TRUE)
		{
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームクリアの時のフラグを入れる
			GameEndFlag = GAME_CLEAR;

			//BGMを止める
			StopSoundMem(PlayBGM.handle);

			//SEが流れていたら止める
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//処理を強制終了
			return;
		}

		//ゲームオーバー判定
		//敵１
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy1.coll) == TRUE)
		{
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームオーバーの時のフラグを入れる
			GameEndFlag = GAME_OVER;

			//BGMを止める
			StopSoundMem(PlayBGM.handle);

			//SEが流れていたら止める
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//処理を強制終了
			return;
		}
		//敵２
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy2.coll) == TRUE)
		{
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームオーバーの時のフラグを入れる
			GameEndFlag = GAME_OVER;

			//BGMを止める
			StopSoundMem(PlayBGM.handle);

			//SEが流れていたら止める
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//処理を強制終了
			return;
		}
		//敵３
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy3.coll) == TRUE)
		{
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//ゲームオーバーの時のフラグを入れる
			GameEndFlag = GAME_OVER;

			//BGMを止める
			StopSoundMem(PlayBGM.handle);

			//SEが流れていたら止める
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//エンド画面に切り替え
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//処理を強制終了
			return;
		}

	}

	//BGMが流れていない時
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//音量の変更
	ChangeBGM(&PlayBGM);

	return;
}

/// <summary>
/// プレイ画面の描画
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//背景動画の描画
	//もし動画が再生されていなかったら
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//再生する
		SeekMovieToGraph(playMovie.handle, 0);   //シークバーを最初に戻す
		PlayMovieToGraph(playMovie.handle);      //動画を再生
	}

	//動画を描画(画面に合わせて画像を引き延ばす)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//ゴールの描画
	if (Goal.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Goal.img.X, Goal.img.Y, Goal.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//プレイヤーの描画
	if (Player.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Player.img.X, Player.img.Y, Player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//敵１の描画
	if (Enemy1.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Enemy1.img.X, Enemy1.img.Y, Enemy1.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Enemy1.coll.left, Enemy1.coll.top, Enemy1.coll.right, Enemy1.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//敵２の描画
	if (Enemy2.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Enemy2.img.X, Enemy2.img.Y, Enemy2.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Enemy2.coll.left, Enemy2.coll.top, Enemy2.coll.right, Enemy2.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//敵３の描画
	if (Enemy3.img.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Enemy3.img.X, Enemy3.img.Y, Enemy3.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Enemy3.coll.left, Enemy3.coll.top, Enemy3.coll.right, Enemy3.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}
	
	//フラグが立ったらメニューを表示
	DrawMenu(PlayBGM.Volume);

	//フラグが立ったらヒントを表示
	DrawTips();

	DrawString(0, 0, "プレイ画面", GetColor(255, 255, 255));

	return;
}


//=====================================================================================================================
//          ココからエンド          
//=====================================================================================================================

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
	//メニュー画面とヒント画面が開いていない時限定
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{
		//タイトルシーンに切り替え
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//シーン切り替え
			//次のシーンの初期化をココで行うと楽

			//SE
			PlaySoundMem(PushEnterSE.handle, PushEnterSE.playType);

			//BGMを止める
			StopSoundMem(EndBGM.handle);

			//エンド画面のフラグをリセット
			GameEndFlag = -1;

			//タイトル画面に切り替え
			ChangeScene(GAME_SCENE_TITLE, &EndBGM, &TitleBGM);

			return;
		}

	}

	//BGMが流れていない時
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGMを流す
		PlaySoundMem(EndBGM.handle, TitleBGM.playType);
	}

	//音量の変更
	ChangeBGM(&EndBGM);

	return;
}

/// <summary>
/// エンド画面の描画
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	switch (GameEndFlag)
	{
	case GAME_CLEAR:
		//=======================================================
		//     ゲームクリアの描画     
		//=======================================================

		//背景を描画
		DrawGraph(EndClearImg.X, EndClearImg.Y, EndClearImg.handle, TRUE);

		//画像を描画
		DrawGraph(EndClear.X, EndClear.Y, EndClear.handle, TRUE);

		break;


	case GAME_OVER:
		//=======================================================
		//     ゲームオーバーの描画     
		//=======================================================

		//背景を描画
		DrawGraph(EndOverImg.X, EndOverImg.Y, EndOverImg.handle, TRUE);

		//画像を描画
		DrawGraph(EndOver.X, EndOver.Y, EndOver.handle, TRUE);

		break;


	default:
		break;
	}

	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));

	//フラグが立ったらメニューを表示
	DrawMenu(EndBGM.Volume);

	//フラグが立ったらヒントを表示
	DrawTips();

	return;
}


//=====================================================================================================================
//          ココからメニュー画面          
//=====================================================================================================================

/// <summary>
/// メニュー欄の表示
/// </summary>
/// <param name="Volum">かかっているBGMの音量</param>
VOID DrawMenu(int Volum)
{
	//メニューアイコンを表示(メニュー画面が開いているかどうかでアイコンを変える)
	if (MenuFlag == FALSE)
	{
		DrawGraph(MenuIcon.X, MenuIcon.Y, MenuIcon.handle, TRUE);
	}
	else if (MenuFlag == TRUE)
	{
		DrawGraph(MenuIconPushVer.X, MenuIconPushVer.Y, MenuIconPushVer.handle, TRUE);
	}

	//Mキーが押されたときフラグを反転させる
	if (KeyClick(KEY_INPUT_M) == TRUE)
	{
		if (MenuFlag == TRUE) MenuFlag = FALSE;
		else if (MenuFlag == FALSE) MenuFlag = TRUE;

		//ついでにSEも流す
		PlaySoundMem(menuSE.handle, menuSE.playType);
	}

	//メニューキーが立っているときに描画
	if (MenuFlag == TRUE)
	{
		//外枠を表示
		DrawGraph(MenuScreen.X, MenuScreen.Y, MenuScreen.handle, TRUE);

		//volumeを調整するポインタを表示
		DrawGraph(VolumePointer.X - (((float)Volum / 255) * 200), VolumePointer.Y, VolumePointer.handle, TRUE);

		//文字を表示
		DrawString(MenuScreen.X + 40, MenuScreen.Y + 197, "Volume", GetColor(0, 0, 0));

		//説明書きを表示
		DrawString(MenuScreen.X + 30, MenuScreen.Y + 137, "O:音量UP　P:音量DOWN", GetColor(0, 0, 0));
	}
	return;
}


//=====================================================================================================================
//          ココからヒント画面          
//=====================================================================================================================

VOID DrawTips(VOID)
{
	//ヒントアイコンを表示(ヒント画面が開いているかどうかでアイコンを変える)
	if (TipsFlag == FALSE)
	{
		DrawGraph(TipsIcon.X, TipsIcon.Y, TipsIcon.handle, TRUE);
	}
	else if (TipsFlag == TRUE)
	{
		DrawGraph(TipsIconPushVer.X, TipsIconPushVer.Y, TipsIconPushVer.handle, TRUE);
	}

	//Hキーが押されたときフラグを反転させる
	if (KeyClick(KEY_INPUT_H) == TRUE)
	{
		if (TipsFlag == TRUE) TipsFlag = FALSE;
		else if (TipsFlag == FALSE) TipsFlag = TRUE;

		//ついでにSEも流す
		PlaySoundMem(tipsSE.handle, tipsSE.playType);
	}

	if (TipsFlag == TRUE)
	{
		//ヒント画面を表示
		DrawGraph(TipsScreen.X, TipsScreen.Y, TipsScreen.handle, TRUE);

		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			//=============================================
			//     タイトルの文章     
			//=============================================

			DrawString(GAME_WIDTH / 2 + 170, 100, "初めまして、僕は猫だ、", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 120, "名前はまだない。", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 160, "突然だが、手伝いをしてもらえるかい？", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 180, "見ての通り、僕はしがない猫でね", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 200, "あまり目が良くないんだ", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 240, "そう緊張しなくても大丈夫、", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 260, "僕だって風を起こすぐらいはできるから", GetColor(0, 0, 0), TRUE);

			break;
		case GAME_SCENE_PLAY:
			//=============================================
			//     プレイの文章     
			//=============================================

			DrawString(GAME_WIDTH / 2 + 170, 100, "それじゃあ君には、ウランを風に乗せ", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 120, "原子力発電所まで送り届けてもらおうかな", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 160, "動いてる中性子には", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 180, "危険だから当たらないでね", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 220, "ん？なんで僕が風をおこせるのかって？", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 240, "先人の知恵とでも言っておこうかな", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 280, "十字キー:移動", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 320, "1:スピードUP　2:スピードDOWN", GetColor(0, 0, 0), TRUE);

			break;
		case GAME_SCENE_END:
			//=============================================
			//     エンドの文章     
			//=============================================

			//ゲームクリア時の文章
			if (GameEndFlag == GAME_CLEAR)
			{
				DrawString(GAME_WIDTH / 2 + 170, 100, "クリアおめでとう", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 140, "あなたのおかげで文明は発展した", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 180, "風を送っていただけのしがない猫でも", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 200, "この結果には誇らしく思う", GetColor(0, 0, 0), TRUE);
			}

			//ゲームオーバーの時の文章
			if (GameEndFlag == GAME_OVER)
			{
				DrawString(GAME_WIDTH / 2 + 170, 100, "なに、少しぐらいへまをしったて大丈夫さ", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 120, "なにせ僕も失敗続きだからね", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 160, "・・・僕の失敗談でも", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 180, "しようと思ったのだけれど・・・", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 240, "はて、沼の猫は何匹だったかな・・？", GetColor(0, 0, 0), TRUE);
			}

			break;
		default:
			break;
		}
	}
	return;
}


//=====================================================================================================================
//          ココから切り替え          
//=====================================================================================================================

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


//=====================================================================================================================
//          ココから当たり判定関係          
//=====================================================================================================================

/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// 当たっているかを調べる
/// </summary>
/// <param name=""></param>
/// <returns>当たってる・・・TRUE｜当たっていない・・・FALSE</returns>
BOOL OnCollision(RECT coll1, RECT coll2)
{
	//当たっていたらTRUE
	if (coll1.left < coll2.right &&
		coll1.right > coll2.left &&
		coll1.top < coll2.bottom &&
		coll1.bottom > coll2.top)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//=====================================================================================================================
//          ココからBGM関係          
//=====================================================================================================================

/// <summary>
/// BGMの音量を変更する
/// </summary>
/// <param name="music">変更したいBGMのAUDIO構造体のアドレス</param>
VOID ChangeBGM(AUDIO* music)
{
	//音量が０～２５５の時に作動
	if (0 <= music->Volume && music->Volume <= 255)
	{
		if (KeyDown(KEY_INPUT_O) == TRUE)
		{
			ChangeVolumeSoundMem(music->Volume + 5, music->handle);
			music->Volume += 5;
		}

		if (KeyDown(KEY_INPUT_P) == TRUE)
		{
			ChangeVolumeSoundMem(music->Volume - 5, music->handle);
			music->Volume -= 5;
		}
	}

	//音量が範囲外だったら範囲内に戻す
	if (0 >= music->Volume)
	{
		ChangeVolumeSoundMem(0, music->handle);
		music->Volume = 0;
	}

	if (music->Volume >= 255)
	{
		ChangeVolumeSoundMem(255, music->handle);
		music->Volume = 255;
	}

	

	return;
}