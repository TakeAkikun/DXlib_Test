
//ヘッダファイル読み込み
#include "geme.h"       //ゲーム全体のヘッダファイル
#include "keyboard.h"   //キーボードの処理
#include "FPS.h"        //FPSの処理

//構造体の定義
//キャラクターの構造体
struct CHARACTOR
{
	int handle = -1;  //画像のハンドル（管理番号）
	char path[255];   //画像の場所（パス）

	int X;            //X位置
	int Y;            //Y位置
	int width;        //幅
	int height;       //高さ
	int Xspead = 1;   //移動速度
	int Yspead = 1;   //移動速度

	RECT coll;          //当たり判定の領域（四角）
	BOOL IsDraw = FALSE;//画像が描画できる？
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
int fadeInCutMax = fadeTimeMax;             //フェードインのカウンタXAX

//シーンを管理する変数
GAME_SCENE GameScene;       //現在のゲームのシーン
GAME_SCENE OldGameScene;    //前回のゲームのシーン
GAME_SCENE NextGameScene;   //次回のゲームのシーン

//プレイヤー
CHARACTOR Player;

//ゴール
CHARACTOR Goal;

//プレイ画面の背景の動画
MOVIE playMovie;

//プロトタイプ宣言
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

VOID ChangeScene(GAME_SCENE seane);                              //シーン切り替え

VOID CollUpdatePlayer(CHARACTOR* chara);                         //当たり判定の領域を更新(プレイヤー)
VOID CollUpdate(CHARACTOR* chara);                               //当たり判定の領域を更新

BOOL OnCollision(RECT coll1 , RECT coll2);                       //当たっているかを調べる

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


	//最初のシーンはタイトル画面から
	GameScene = GAME_SCENE_TITLE;

	//全体の初期化

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
		DxLib_End();                 //強制終了
		return -1;                   //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//動画のボリューム
	playMovie.Volume = 255;

	//プレイヤーの画像を読み込み
	strcpyDx(Player.path, ".\\image\\Player.png");   //パスのコピー
	Player.handle = LoadGraph(Player.path);          //画像の読み込み

	//画像が読み込めなかったときは、エラー(－1)が入る
	if (Player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //メインのウィンドウタイトル
			Player.path,             //メッセージ本文
			"画像読み込みエラー",    //メッセージタイトル
			MB_OK                    //ボタン
		);
		DxLib_End();                 //強制終了
		return -1;                   //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(Player.handle, &Player.width, &Player.height);

	//プレイヤーを初期化
	Player.X = 0;
	Player.Y = 0;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.IsDraw = TRUE;

	//当たり判定を更新
	CollUpdatePlayer(&Player);  //プレイヤーの当たり判定のアドレス

	//ゴールの画像を読み込み
	strcpyDx(Goal.path, ".\\image\\Goal.png");  //パスのコピー
	Goal.handle = LoadGraph(Goal.path);          //画像の読み込み

	//画像が読み込めなかったときは、エラー(－1)が入る
	if (Goal.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //メインのウィンドウタイトル
			Goal.path,               //メッセージ本文
			"画像読み込みエラー",    //メッセージタイトル
			MB_OK                    //ボタン
		);
		DxLib_End();                 //強制終了
		return -1;                   //エラー終了
	}

	//画面の幅と高さを取得
	GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

	//ゴールを初期化
	Goal.X = GAME_WIDTH - Goal.width;
	Goal.Y = GAME_HEIGHT - Goal.height;
	Goal.Xspead = 300;
	Goal.Yspead = 300;
	Goal.IsDraw = TRUE;

	//当たり判定を更新
	CollUpdate(&Goal);  //ゴールの当たり判定のアドレス

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
	DeleteGraph(Player.handle);      //画像をメモリ上から削除
	DeleteGraph(Goal.handle);        //画像をメモリ上から削除
	DeleteGraph(playMovie.handle);   //動画をメモリ上から削除

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
	DrawString(GAME_HEIGHT / 2, GAME_WIDTH / 2, "Enterキーでゲーム開始", GetColor(255, 0, 0));
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
	//プレイヤーの操作
		//壁を突き抜けないようにif文を調整
	if (KeyDown(KEY_INPUT_UP) == TRUE && Player.Y > 0)
	{
		Player.Y -= Player.Yspead * fps.DeltaTime;   //上に移動

		//スピード高すぎてめり込むのを防止
		if (Player.Y < 0)
		{
			Player.Y = 0;
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.Y < GAME_HEIGHT - Player.height)
	{
		Player.Y += Player.Yspead * fps.DeltaTime;   //下に移動

		//スピード高すぎてめり込むのを防止
		if (Player.Y > GAME_HEIGHT - Player.height)
		{
			Player.Y = GAME_HEIGHT - Player.height;
		}
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.X > 0)
	{
		Player.X -= Player.Xspead * fps.DeltaTime;   //左に移動

		//スピード高すぎてめり込むのを防止
		if (Player.X < 0)
		{
			Player.X = 0;
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.X < GAME_WIDTH - Player.width)
	{
		Player.X += Player.Xspead * fps.DeltaTime;   //右に移動

		//スピード高すぎてめり込むのを防止
		if (Player.X > GAME_WIDTH - Player.width)
		{
			Player.X = GAME_WIDTH - Player.width;
		}
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
	Player.coll = { Player.X,Player.Y,Player.X + Player.width,Player.Y + Player.height };
	Goal.coll = { Goal.X,Goal.Y,Goal.X + Goal.width,Goal.Y + Goal.height };

	//ゴール判定
	if (Player.IsDraw == TRUE && OnCollision(Player.coll, Goal.coll) == TRUE)
	{
		//シーン切り替え
		//次のシーンの初期化をココで行うと楽

		//エンド画面に切り替え
		ChangeScene(GAME_SCENE_END);

		//プレイヤーを初期化
		Player.X = 0;
		Player.Y = 0;
		Player.Xspead = 300;
		Player.Yspead = 300;
		Player.IsDraw = TRUE;

		//処理を強制終了
		return;
	}

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
	if (Goal.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Goal.X, Goal.Y, Goal.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//プレイヤーの描画
	if (Player.IsDraw == TRUE)
	{
		//画像を描画
		DrawGraph(Player.X, Player.Y, Player.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//四角を描画
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	DrawString(0, 20, "画面右端までたどり着こう！", GetColor(0, 0, 0));
	DrawString(0, 40, "（1でスピードアップ：2でスピードダウン）", GetColor(0, 0, 0));

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
	DrawString(GAME_HEIGHT / 2, GAME_WIDTH / 2, "クリアおめでとう！", GetColor(255, 0, 0));
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


/// <summary>
/// 当たり判定の領域更新(プレイヤー)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->X;
	chara->coll.top = chara->Y;
	chara->coll.right = chara->X + chara->width;
	chara->coll.bottom = chara->Y + chara->height;

	return;
}

/// <summary>
/// 当たり判定の領域更新
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->X;
	chara->coll.top = chara->Y;
	chara->coll.right = chara->X + chara->width;
	chara->coll.bottom = chara->Y + chara->height;

	return;
}

/// <summary>
/// 当たっているかを調べる
/// </summary>
/// <param name=""></param>
/// <returns>当たってる・・・TRUE｜当たっていない・・・FALSE</returns>
BOOL OnCollision(RECT coll1 , RECT coll2)
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