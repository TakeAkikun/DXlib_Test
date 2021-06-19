
//�w�b�_�t�@�C���ǂݍ���
#include "geme.h"       //�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"   //�L�[�{�[�h�̏���
#include "FPS.h"        //FPS�̏���

//=========================================================
//     �\����    
//=========================================================

//�摜�̍\����
struct IMAGE
{
	int handle = -1;  //�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];   //�摜�̏ꏊ�i�p�X�j

	int X;            //X�ʒu
	int Y;            //Y�ʒu
	int width;        //��
	int height;       //����

	BOOL IsDraw = FALSE;//�摜���`��ł���H
};

//�L�����N�^�[�̍\����
struct CHARACTOR
{
	IMAGE img;        //�摜�̍\����
	int Xspead = 1;   //�ړ����x
	int Yspead = 1;   //�ړ����x
	RECT coll;        //�����蔻��̗̈�(�l�p)
};

//����̍\����
struct MOVIE
{
	int handle = -1;    //����̃n���h��
	char path[25];      //����̃p�X

	int x;              //X�ʒu
	int y;              //Y�ʒu
	int width;          //��
	int height;         //����

	int Volume = 255;   //�{�����[���i0�`255�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;    //���y�̃n���h��
	char path[255];     //���y�̃p�X

	int Volume = -1;    //�{�����[���iMIN 0�`255 MAX�j
	int playType = -1;  //BGM or SE
};

//=========================================================
//     �ϐ��쐬     
//=========================================================

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;   //�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;    //�t�F�[�h�C��

int fadeTimeMill = 2000;                    //�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60; //�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;                     //�����l
int fadeOutCnt = fadeOutCntInit;            //�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCutMax = fadeTimeMax;            //�t�F�[�h�A�E�g�̃J�E���^XAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;            //�����l
int fadeInCnt = fadeInCntInit;              //�t�F�[�h�C���̃J�E���^
int fadeInCutMax = fadeTimeMax;             //�t�F�[�h�C���̃J�E���^XAX

//�^�C�g�����S�̂��񂾂�Z���Ȃ�A��
float TitleLogoCnt = 0;           //�J�E���^
BOOL TitleLogoBrink = FALSE;      //���S�ɕ\�����ꂽ���H

//PushEnter�̓_��
int PushEnterCnt = 0;           //�J�E���^
const int PushEnterCntMax = 60; //�J�E���^MAX
BOOL PushEnterBrink = FALSE;    //�_�ł��Ă��邩�H

//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;       //���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;    //�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;   //����̃Q�[���̃V�[��

//���j���[���̕\���t���O
BOOL MenuFlag = FALSE;    //���j���[����\�����邩�H

//�q���g���̕\���t���O
BOOL TipsFlag = FALSE;    //�q���g����\�����邩�H

//�G���h��ʂ̎��̃t���O
int GameEndFlag = -1;

//�v���C���[
CHARACTOR Player;

//�G
CHARACTOR Enemy1;
CHARACTOR Enemy2;
CHARACTOR Enemy3;

//�S�[��
CHARACTOR Goal;

//���S�Ɣw�i
IMAGE TitleLogo;        //�^�C�g�����S
IMAGE TitleEnter;       //PushEnter�̃��S
IMAGE TitleImg;         //�^�C�g����ʂ̔w�i
IMAGE EndClear;         //GameClear�̃��S
IMAGE EndClearImg;      //�Q�[���N���A��ʂ̔w�i
IMAGE EndOver;          //GameOver�̃��S
IMAGE EndOverImg;       //�Q�[���I�[�o�[��ʂ̔w�i
IMAGE MenuIcon;         //���j���[�̃A�C�R��
IMAGE MenuIconPushVer;  //���j���[��ʂ��J���Ă��鎞�̃A�C�R��
IMAGE MenuScreen;       //���j���[���
IMAGE VolumePointer;    //���ʒ����̎��̈ړ�����Ђ��`�̃A��
IMAGE TipsIcon;         //�q���g�̃A�C�R��
IMAGE TipsIconPushVer;  //�q���g��ʂ��J���Ă��鎞�̃A�C�R��
IMAGE TipsScreen;       //�q���g���

//�v���C��ʂ̔w�i�̓���
MOVIE playMovie;

//���y
AUDIO TitleBGM;         //�^�C�g����BGM
AUDIO PlayBGM;          //�v���C��BGM
AUDIO EndBGM;           //�G���h��BGM
AUDIO moveSE;           //�v���C���[����������SE
AUDIO PushEnterSE;      //�V�[�����ڂ̎���SE
AUDIO menuSE;           //���j���[��ʂ��J��(����)����SE
AUDIO tipsSE;           //�q���g��ʂ��J��(����)����SE

//=========================================================
//     �֐�     
//=========================================================

//�L�������P�[�X�E�E�E�P��̐擪��啶���ɂ��閽���K��
//�X�l�[�N�P�[�X�E�E�E�P����A���_�[�o�[�łȂ������K��

VOID Title(VOID);     //�^�C�g�����
VOID TitleProc(VOID); //�^�C�g����ʁi�����j
VOID TitleDraw(VOID); //�^�C�g����ʁi�`��j

VOID Play(VOID);					                             //�v���C���
VOID PlayProc(VOID);				                             //�v���C��ʁi�����j
VOID PlayDraw(VOID);				                             //�v���C��ʁi�`��j

VOID End(VOID);						                             //�G���h���
VOID EndProc(VOID);					                             //�G���h��ʁi�����j
VOID EndDraw(VOID);					                             //�G���h��ʁi�`��j

VOID Change(VOID);					                             //�؂�ւ����
VOID ChangeProc(VOID);				                             //�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);				                             //�؂�ւ���ʁi�`��j

VOID DrawMenu(int Volum);			                             //���j���[��ʁi�`��j
VOID DrawTips(VOID);				                             //�q���g��ʁi�`��j

VOID ChangeScene(GAME_SCENE seane, AUDIO* OldBGM, AUDIO* NextBGM);//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);                         //�����蔻��̗̈���X�V(�v���C���[)
VOID CollUpdate(CHARACTOR* chara);                               //�����蔻��̗̈���X�V

BOOL OnCollision(RECT coll1, RECT coll2);                       //�������Ă��邩�𒲂ׂ�

VOID ChangeBGM(AUDIO* music);                                    //BGM�̉��ʕύX

BOOL GameLoad(VOID);                                             //�Q�[���S�̂̃f�[�^��ǂݍ���
VOID GameInit(VOID);                                             //�Q�[���f�[�^�̏�����
BOOL ImageInput(IMAGE* Image, const char* path);                 //�Q�[���̉摜�ǂݍ���
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType);//�Q�[����BGM�ǂݍ���

//=====================================================================================================================
//          �R�R���烁�C���v���O����          
//=====================================================================================================================

// �v���O������ WinMain ����n�܂�܂�
// Windous�̃v���O���~���O���@�œ����Ă���B�iWinAPI�j
// DxLib��DirectX�Ƃ����Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//������͕K��Init���s���O�ɓ��͂���
	SetOutApplicationLogValidFlag(FALSE);                //Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);                              //�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);                       //�Q�[���^�C�g����ݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);   //�Q�[���̉𑜓x�̐ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);              //�E�B���h�E�̑傫����ݒ�
	SetWindowStyleMode(GAME_WINDOW_BAR);                 //�E�B���h�E�o�[�̏��
	SetBackgroundColor(255, 255, 255);                   //�E�B���h�E�̃f�t�H�̐F��ݒ�
	SetWindowIconID(GAME_ICON_ID);                       //�A�C�R���t�@�C����ǂݍ���
	SetWaitVSyncFlag(TRUE);                              //�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);                              //�E�B���h�E�������ƃA�N�e�B�u�ɂ���



	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			    // �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�Q�[���S�̂̏�����
	if (!GameLoad())
	{
		//�Q�[���f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();  //DxLib�I��
		return -1;    //�ُ�I��
	}

	//�Q�[���f�[�^�̏�����
	GameInit();

	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0) { break; }  //���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; } //��ʂ��N���A

		AllKeyUpdate();         //�L�[�{�[�h���͂̍X�V

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();            //�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();             //�v���C���
			break;
		case GAME_SCENE_END:
			End();              //�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();           //�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//�ȑO�̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;     //���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE; //��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();

		ScreenFlip();           //�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(Player.img.handle);      //�摜���������ォ��폜
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
	DeleteGraph(playMovie.handle);       //������������ォ��폜
	DeleteGraph(TitleBGM.handle);        //���y���������ォ��폜
	DeleteGraph(PlayBGM.handle);         //
	DeleteGraph(EndBGM.handle);          //
	DeleteGraph(moveSE.handle);          //SE���������ォ��폜
	DeleteGraph(PushEnterSE.handle);     //
	DeleteGraph(menuSE.handle);          //
	DeleteGraph(tipsSE.handle);          //

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				    // �\�t�g�̏I�� 
}


//=====================================================================================================================
//          �R�R���珉�����֌W          
//=====================================================================================================================


/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�v���C���[��������
	Player.img.X = 0;
	Player.img.Y = 0;
	Player.Xspead = 300;
	Player.Yspead = 300;
	Player.img.IsDraw = TRUE;

	//�����蔻����X�V
	CollUpdatePlayer(&Player);  //�v���C���[�̓����蔻��̃A�h���X

	//�G�P��������
	Enemy1.img.X = 350;
	Enemy1.img.Y = 200;
	Enemy1.Xspead = 10;
	Enemy1.Yspead = 10;
	Enemy1.img.IsDraw = TRUE;

	//�����蔻��̍X�V
	CollUpdatePlayer(&Enemy1);  //�G�̓����蔻��̃A�h���X

	//�G�Q��������
	Enemy2.img.X = 500;
	Enemy2.img.Y = 400;
	Enemy2.Xspead = 10;
	Enemy2.Yspead = 10;
	Enemy2.img.IsDraw = TRUE;

	//�����蔻��̍X�V
	CollUpdatePlayer(&Enemy2);  //�G�̓����蔻��̃A�h���X

	//�G�R��������
	Enemy3.img.X = 900;
	Enemy3.img.Y = 500;
	Enemy3.Xspead = 10;
	Enemy3.Yspead = 10;
	Enemy3.img.IsDraw = TRUE;

	//�����蔻��̍X�V
	CollUpdatePlayer(&Enemy3);  //�G�̓����蔻��̃A�h���X

	//�S�[����������
	Goal.img.X = GAME_WIDTH - Goal.img.width;
	Goal.img.Y = GAME_HEIGHT - Goal.img.height;
	Goal.Xspead = 300;
	Goal.Yspead = 300;
	Goal.img.IsDraw = TRUE;

	//�����蔻����X�V
	CollUpdate(&Goal);  //�S�[���̓����蔻��̃A�h���X

	//�^�C�g�����S�̈ʒu
	TitleLogo.X = GAME_WIDTH / 2 - TitleLogo.width / 2;  //��������
	TitleLogo.Y = 10;

	//�^�C�g�����S�̂��񂾂�Z���Ȃ�A��
	TitleLogoCnt = 0;          //�J�E���^
	TitleLogoBrink = FALSE;    //���S�ɕ\�����ꂽ���H

	//PushEnter�̈ʒu
	TitleEnter.X = GAME_WIDTH / 2 - TitleEnter.width / 2;//��������
	TitleEnter.Y = GAME_HEIGHT - TitleEnter.height - 100;

	//PushEnter�̓_��
	PushEnterCnt = 0;         //�J�E���^
	PushEnterBrink = FALSE;  //�_�ł��Ă��邩�H

	//�N���A���S�̈ʒu
	EndClear.X = GAME_WIDTH / 2 - EndClear.width / 2;       //��������
	EndClear.Y = GAME_WIDTH / 2 - EndClear.height / 2 - 300;//��������

	//�I�[�o�[���S�̈ʒu
	EndOver.X = GAME_WIDTH / 2 - EndOver.width / 2;         //��������
	EndOver.Y = GAME_WIDTH / 2 - EndOver.height / 2 - 300;  //��������

	//���j���[�A�C�R���̈ʒu
	MenuIcon.X = GAME_WIDTH - MenuIcon.width - 10;          //�E��
	MenuIcon.Y = 10;                                        //

	//���j���[��ʂ��J���Ă���Ƃ��̃��j���[�A�C�R���̈ʒu
	MenuIconPushVer.X = GAME_WIDTH - MenuIconPushVer.width - 10;   //���j���[�A�C�R���Ɠ����ʒu
	MenuIconPushVer.Y = 10;                                        //

	//���j���[��ʂ̈ʒu
	MenuScreen.X = GAME_WIDTH / 2 +112;                     //�啪�E
	MenuScreen.Y = 0;                                       //

	//�q���g�A�C�R���̈ʒu
	TipsIcon.X = GAME_WIDTH - MenuIcon.width - TipsIcon.width - 20;//���j���[�A�C�R���̍���
	TipsIcon.Y = 10;                                               //

	//�q���g��ʂ��J���Ă���Ƃ��̃q���g�A�C�R���̈ʒu
	TipsIconPushVer.X = GAME_WIDTH - MenuIcon.width - TipsIconPushVer.width - 20;//�q���g�A�C�R���Ɠ����ʒu
	TipsIconPushVer.Y = 10;                                                      //

	//�q���g��ʂ̈ʒu
	TipsScreen.X = GAME_WIDTH / 2 + 100;                    //���j���[��ʂƑ�̓����ʒu
	TipsScreen.Y = 0;                                       //

	//���ʒ����̓������c�̈ʒu
	VolumePointer.X = GAME_WIDTH / 2 + 320;                 //���xVolume�̃o�[�̈�ԉE�ɏd�Ȃ�悤��
	VolumePointer.Y = 215;                                  //
}


/// <summary>
/// �Q�[���S�̂̏�����
/// </summary>
/// <returns>�ǂݍ��ݐ����E�E�ETRUE�b�ǂݍ��ݎ��s�E�E�EFALSE</returns>
BOOL GameLoad()
{
	//�v���C����̔w�i��ǂݍ���
	strcpyDx(playMovie.path, ".\\movie\\PlayMovie.mp4");  //�p�X�̃R�s�[
	playMovie.handle = LoadGraph(playMovie.path);        //����̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (playMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //���C���̃E�B���h�E�^�C�g��
			playMovie.path,          //���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",    //���b�Z�[�W�^�C�g��
			MB_OK                    //�{�^��
		);

		return FALSE;                //�G���[�I��
	}

	//��ʂ̕��ƍ������擾
	GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

	//����̃{�����[��
	playMovie.Volume = 255;

	//�摜��ǂݍ���
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

	//���y��ǂݍ���
	if (!MusicInput(&TitleBGM, ".\\audio\\OP.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&PlayBGM, ".\\audio\\PLAY.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&EndBGM, ".\\audio\\ED.mp3", 150, DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!MusicInput(&moveSE, ".\\SE\\move.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&PushEnterSE, ".\\SE\\push.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&menuSE, ".\\SE\\menu.mp3", 100, DX_PLAYTYPE_BACK)) { return FALSE; }
	if (!MusicInput(&tipsSE, ".\\SE\\tips.mp3", 150, DX_PLAYTYPE_BACK)) { return FALSE; }


	return TRUE;        //�S���ǂݍ��߂���TRUE
}


//=====================================================================================================================
//          �R�R����ǂݍ��݊֌W          
//=====================================================================================================================


/// <summary>
/// �摜���������ɓǂݍ���
/// </summary>
/// <param name="Image">Image�\���̂̃A�h���X</param>
/// <param name="path">Image�̉摜�p�X</param>
/// <returns>TRUE or FALSE</returns>
BOOL ImageInput(IMAGE* Image, const char* path)
{
	//�S�[���̉摜��ǂݍ���
	strcpyDx(Image->path, path);  //�p�X�̃R�s�[
	Image->handle = LoadGraph(Image->path);          //�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (Image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //���C���̃E�B���h�E�^�C�g��
			Image->path,             //���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",    //���b�Z�[�W�^�C�g��
			MB_OK                    //�{�^��
		);

		return FALSE;                //�G���[�I��
	}

	//��ʂ̕��ƍ������擾
	GetGraphSize(Image->handle, &Image->width, &Image->height);
}


/// <summary>
/// ���y���������ɓǂݍ���
/// </summary>
/// <param name="music">Audio�\���̂̃A�h���X</param>
/// <param name="path">Audio�̉��y�p�X</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns>TRUE or FALSE</returns>
BOOL MusicInput(AUDIO* music, const char* path, int volume, int playType)
{
	//���y��ǂݍ���
	strcpyDx(music->path, path);  //�p�X�̃R�s�[
	music->handle = LoadSoundMem(music->path);          //���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (music->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),       //���C���̃E�B���h�E�^�C�g��
			music->path,                 //���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",        //���b�Z�[�W�^�C�g��
			MB_OK                        //�{�^��
		);

		return FALSE;                    //�G���[�I��
	}

	music->playType = playType;                          //���y�����[�v������
	music->Volume = volume;                              //MAX��255
	ChangeVolumeSoundMem(music->Volume, music->handle);  //BGM�̃{�����[����ύX

	return TRUE;
}


//=========================================================
//     ���L�͐؂�ւ��֐�     
//=========================================================


/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene, AUDIO* OldBGM, AUDIO* NextBGM)
{
	GameScene = scene;  //�V�[���؂�ւ�
	IsFadeIn = FALSE;             //�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;             //�t�F�[�h�A�E�g����

	//�O�̃V�[����BGM�̉��ʂ�����BGM�Ɉ��p��
	NextBGM->Volume = OldBGM->Volume;
	ChangeVolumeSoundMem(NextBGM->Volume, NextBGM->handle);

	return;
}


//=====================================================================================================================
//           �R�R����^�C�g��          
//=====================================================================================================================

/// <summary>
/// �^�C�g�����
/// </summary>
/// <param name=""></param>
VOID Title(VOID)
{
	TitleProc();    //����
	TitleDraw();    //�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
/// <param name=""></param>
VOID TitleProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//�^�C�g�����S�̍~��Ă��鉉�o
		//Max�l�܂ő҂�
		if (TitleLogoCnt < TitleLogo.Y)
		{
			TitleLogoCnt += 0.1;
		}
		else
		{

			TitleLogoBrink = TRUE;
		}


		//�^�C�g�����S�̉��o���I�������ɕ\��
		if (TitleLogoBrink == TRUE)
		{

			//PushEnter�̓_��
			//MAX�l�܂ő҂�
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

		//�v���C�V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//SE
			PlaySoundMem(PushEnterSE.handle, PushEnterSE.playType);

			//BGM���~�߂�
			StopSoundMem(TitleBGM.handle);

			//�Q�[���f�[�^�̏�����
			GameInit();

			//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_PLAY, &TitleBGM, &PlayBGM);

			return;
		}
	}

	//BGM������Ă��Ȃ���
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	//���ʂ̕ύX
	ChangeBGM(&TitleBGM);

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	//�w�i��`��
	DrawGraph(TitleImg.X, TitleImg.Y, TitleImg.handle, TRUE);

	//�^�C�g�����S�̕`��

	if (TitleLogoBrink == FALSE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)TitleLogoCnt / TitleLogo.Y) * 255);
		//���S��`��
		DrawGraph(TitleLogo.X, TitleLogoCnt, TitleLogo.handle, TRUE);
		//�������I��
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
	//���S��`��
	if (TitleLogoBrink == TRUE)
	{
		DrawGraph(TitleLogo.X, TitleLogoCnt, TitleLogo.handle, TRUE);
	}


	//�^�C�g�����S�̉��o���I�������ɕ\��
	if (TitleLogoBrink == TRUE)
	{

		//PushEnter��_��

		if (PushEnterBrink == TRUE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMax) * 255);

			//���S��`��
			DrawGraph(TitleEnter.X, TitleEnter.Y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}

		if (PushEnterBrink == FALSE)
		{
			//�������ɂ���
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMax - PushEnterCnt) / PushEnterCntMax) * 255);

			//���S��`��
			DrawGraph(TitleEnter.X, TitleEnter.Y, TitleEnter.handle, TRUE);

			//�������I��
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}

	}

	//�t���O���������烁�j���[��\��
	DrawMenu(TitleBGM.Volume);

	//�t���O����������q���g��\��
	DrawTips();

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
}


//=====================================================================================================================
//          �R�R����v���C          
//=====================================================================================================================

/// <summary>
/// �v���C���
/// </summary>
/// <param name=""></param>
VOID Play(VOID)
{
	PlayProc();    //����
	PlayDraw();    //�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID PlayProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{

		//�v���C���[�̑���

			//�ǂ�˂������Ȃ��悤��if���𒲐�
		if (KeyDown(KEY_INPUT_UP) == TRUE && Player.img.Y > 0)
		{
			Player.img.Y -= Player.Yspead * fps.DeltaTime;   //��Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.Y < 0)
			{
				Player.img.Y = 0;
			}

			//���ʉ�
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.img.Y < GAME_HEIGHT - Player.img.height)
		{
			Player.img.Y += Player.Yspead * fps.DeltaTime;   //���Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.Y > GAME_HEIGHT - Player.img.height)
			{
				Player.img.Y = GAME_HEIGHT - Player.img.height;
			}

			//���ʉ�
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.img.X > 0)
		{
			Player.img.X -= Player.Xspead * fps.DeltaTime;   //���Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.X < 0)
			{
				Player.img.X = 0;
			}

			//���ʉ�
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.img.X < GAME_WIDTH - Player.img.width)
		{
			Player.img.X += Player.Xspead * fps.DeltaTime;   //�E�Ɉړ�

			//�X�s�[�h�������Ă߂荞�ނ̂�h�~
			if (Player.img.X > GAME_WIDTH - Player.img.width)
			{
				Player.img.X = GAME_WIDTH - Player.img.width;
			}

			//���ʉ�
			if (CheckSoundMem(moveSE.handle) == 0)
			{
				PlaySoundMem(moveSE.handle, moveSE.playType);
			}
		}

		//�G�̓���

		//�G1
		Enemy1.img.X += Enemy1.Xspead;
		if (Enemy1.img.X < 0 || Enemy1.img.X + Enemy1.img.width > GAME_WIDTH)
		{
			Enemy1.Xspead = -Enemy1.Xspead;
		}

		//�G2
		Enemy2.img.Y += Enemy2.Yspead;
		if (Enemy2.img.Y < 0 || Enemy2.img.Y + Enemy2.img.width > GAME_HEIGHT)
		{
			Enemy2.Yspead = -Enemy2.Yspead;
		}

		//�G3
		Enemy3.img.Y += Enemy3.Yspead;
		if (Enemy3.img.Y < 0 || Enemy3.img.Y + Enemy3.img.width > GAME_HEIGHT)
		{
			Enemy3.Yspead = -Enemy3.Yspead;
		}

		// �P�ŃX�s�[�hUP�E�Q�ŃX�s�[�hDOWN�i0�̎��͂���������Ȃ��j
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

		//�����蔻����X�V
		CollUpdatePlayer(&Player);
		CollUpdatePlayer(&Enemy1);
		CollUpdatePlayer(&Enemy2);
		CollUpdatePlayer(&Enemy3);
		CollUpdate(&Goal);

		/*
		//�G���h�V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�v���C��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END);
		}
		*/

		//�����蔻���RECT�i�v���C���[�ƃS�[���j
		Player.coll = { Player.img.X,Player.img.Y,Player.img.X + Player.img.width,Player.img.Y + Player.img.height };
		Enemy1.coll = { Enemy1.img.X,Enemy1.img.Y,Enemy1.img.X + Enemy1.img.width,Enemy1.img.Y + Enemy1.img.height };
		Enemy2.coll = { Enemy2.img.X,Enemy2.img.Y,Enemy2.img.X + Enemy2.img.width,Enemy2.img.Y + Enemy2.img.height };
		Enemy3.coll = { Enemy3.img.X,Enemy3.img.Y,Enemy3.img.X + Enemy3.img.width,Enemy3.img.Y + Enemy3.img.height };
		Goal.coll = { Goal.img.X,Goal.img.Y,Goal.img.X + Goal.img.width,Goal.img.Y + Goal.img.height };

		//�S�[������
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Goal.coll) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���N���A�̎��̃t���O������
			GameEndFlag = GAME_CLEAR;

			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//SE������Ă�����~�߂�
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//�����������I��
			return;
		}

		//�Q�[���I�[�o�[����
		//�G�P
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy1.coll) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���I�[�o�[�̎��̃t���O������
			GameEndFlag = GAME_OVER;

			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//SE������Ă�����~�߂�
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//�����������I��
			return;
		}
		//�G�Q
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy2.coll) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���I�[�o�[�̎��̃t���O������
			GameEndFlag = GAME_OVER;

			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//SE������Ă�����~�߂�
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//�����������I��
			return;
		}
		//�G�R
		if (Player.img.IsDraw == TRUE && OnCollision(Player.coll, Enemy3.coll) == TRUE)
		{
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//�Q�[���I�[�o�[�̎��̃t���O������
			GameEndFlag = GAME_OVER;

			//BGM���~�߂�
			StopSoundMem(PlayBGM.handle);

			//SE������Ă�����~�߂�
			if (CheckSoundMem(moveSE.handle)) { StopSoundMem(moveSE.handle); }

			//�G���h��ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_END, &PlayBGM, &EndBGM);

			//�����������I��
			return;
		}

	}

	//BGM������Ă��Ȃ���
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//���ʂ̕ύX
	ChangeBGM(&PlayBGM);

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	//�w�i����̕`��
	//�������悪�Đ�����Ă��Ȃ�������
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);   //�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);      //������Đ�
	}

	//�����`��(��ʂɍ��킹�ĉ摜���������΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�S�[���̕`��
	if (Goal.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Goal.img.X, Goal.img.Y, Goal.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�v���C���[�̕`��
	if (Player.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Player.img.X, Player.img.Y, Player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�G�P�̕`��
	if (Enemy1.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Enemy1.img.X, Enemy1.img.Y, Enemy1.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Enemy1.coll.left, Enemy1.coll.top, Enemy1.coll.right, Enemy1.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�G�Q�̕`��
	if (Enemy2.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Enemy2.img.X, Enemy2.img.Y, Enemy2.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Enemy2.coll.left, Enemy2.coll.top, Enemy2.coll.right, Enemy2.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

	//�G�R�̕`��
	if (Enemy3.img.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Enemy3.img.X, Enemy3.img.Y, Enemy3.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Enemy3.coll.left, Enemy3.coll.top, Enemy3.coll.right, Enemy3.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}
	
	//�t���O���������烁�j���[��\��
	DrawMenu(PlayBGM.Volume);

	//�t���O����������q���g��\��
	DrawTips();

	DrawString(0, 0, "�v���C���", GetColor(255, 255, 255));

	return;
}


//=====================================================================================================================
//          �R�R����G���h          
//=====================================================================================================================

/// <summary>
/// �G���h���
/// </summary>
/// <param name=""></param>
VOID End(VOID)
{
	EndProc();    //����
	EndDraw();    //�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
/// <param name=""></param>
VOID EndProc(VOID)
{
	//���j���[��ʂƃq���g��ʂ��J���Ă��Ȃ�������
	if (MenuFlag == FALSE && TipsFlag == FALSE)
	{
		//�^�C�g���V�[���ɐ؂�ւ�
		if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
			//�V�[���؂�ւ�
			//���̃V�[���̏��������R�R�ōs���Ɗy

			//SE
			PlaySoundMem(PushEnterSE.handle, PushEnterSE.playType);

			//BGM���~�߂�
			StopSoundMem(EndBGM.handle);

			//�G���h��ʂ̃t���O�����Z�b�g
			GameEndFlag = -1;

			//�^�C�g����ʂɐ؂�ւ�
			ChangeScene(GAME_SCENE_TITLE, &EndBGM, &TitleBGM);

			return;
		}

	}

	//BGM������Ă��Ȃ���
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(EndBGM.handle, TitleBGM.playType);
	}

	//���ʂ̕ύX
	ChangeBGM(&EndBGM);

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	switch (GameEndFlag)
	{
	case GAME_CLEAR:
		//=======================================================
		//     �Q�[���N���A�̕`��     
		//=======================================================

		//�w�i��`��
		DrawGraph(EndClearImg.X, EndClearImg.Y, EndClearImg.handle, TRUE);

		//�摜��`��
		DrawGraph(EndClear.X, EndClear.Y, EndClear.handle, TRUE);

		break;


	case GAME_OVER:
		//=======================================================
		//     �Q�[���I�[�o�[�̕`��     
		//=======================================================

		//�w�i��`��
		DrawGraph(EndOverImg.X, EndOverImg.Y, EndOverImg.handle, TRUE);

		//�摜��`��
		DrawGraph(EndOver.X, EndOver.Y, EndOver.handle, TRUE);

		break;


	default:
		break;
	}

	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));

	//�t���O���������烁�j���[��\��
	DrawMenu(EndBGM.Volume);

	//�t���O����������q���g��\��
	DrawTips();

	return;
}


//=====================================================================================================================
//          �R�R���烁�j���[���          
//=====================================================================================================================

/// <summary>
/// ���j���[���̕\��
/// </summary>
/// <param name="Volum">�������Ă���BGM�̉���</param>
VOID DrawMenu(int Volum)
{
	//���j���[�A�C�R����\��(���j���[��ʂ��J���Ă��邩�ǂ����ŃA�C�R����ς���)
	if (MenuFlag == FALSE)
	{
		DrawGraph(MenuIcon.X, MenuIcon.Y, MenuIcon.handle, TRUE);
	}
	else if (MenuFlag == TRUE)
	{
		DrawGraph(MenuIconPushVer.X, MenuIconPushVer.Y, MenuIconPushVer.handle, TRUE);
	}

	//M�L�[�������ꂽ�Ƃ��t���O�𔽓]������
	if (KeyClick(KEY_INPUT_M) == TRUE)
	{
		if (MenuFlag == TRUE) MenuFlag = FALSE;
		else if (MenuFlag == FALSE) MenuFlag = TRUE;

		//���ł�SE������
		PlaySoundMem(menuSE.handle, menuSE.playType);
	}

	//���j���[�L�[�������Ă���Ƃ��ɕ`��
	if (MenuFlag == TRUE)
	{
		//�O�g��\��
		DrawGraph(MenuScreen.X, MenuScreen.Y, MenuScreen.handle, TRUE);

		//volume�𒲐�����|�C���^��\��
		DrawGraph(VolumePointer.X - (((float)Volum / 255) * 200), VolumePointer.Y, VolumePointer.handle, TRUE);

		//������\��
		DrawString(MenuScreen.X + 40, MenuScreen.Y + 197, "Volume", GetColor(0, 0, 0));

		//����������\��
		DrawString(MenuScreen.X + 30, MenuScreen.Y + 137, "O:����UP�@P:����DOWN", GetColor(0, 0, 0));
	}
	return;
}


//=====================================================================================================================
//          �R�R����q���g���          
//=====================================================================================================================

VOID DrawTips(VOID)
{
	//�q���g�A�C�R����\��(�q���g��ʂ��J���Ă��邩�ǂ����ŃA�C�R����ς���)
	if (TipsFlag == FALSE)
	{
		DrawGraph(TipsIcon.X, TipsIcon.Y, TipsIcon.handle, TRUE);
	}
	else if (TipsFlag == TRUE)
	{
		DrawGraph(TipsIconPushVer.X, TipsIconPushVer.Y, TipsIconPushVer.handle, TRUE);
	}

	//H�L�[�������ꂽ�Ƃ��t���O�𔽓]������
	if (KeyClick(KEY_INPUT_H) == TRUE)
	{
		if (TipsFlag == TRUE) TipsFlag = FALSE;
		else if (TipsFlag == FALSE) TipsFlag = TRUE;

		//���ł�SE������
		PlaySoundMem(tipsSE.handle, tipsSE.playType);
	}

	if (TipsFlag == TRUE)
	{
		//�q���g��ʂ�\��
		DrawGraph(TipsScreen.X, TipsScreen.Y, TipsScreen.handle, TRUE);

		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			//=============================================
			//     �^�C�g���̕���     
			//=============================================

			DrawString(GAME_WIDTH / 2 + 170, 100, "���߂܂��āA�l�͔L���A", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 120, "���O�͂܂��Ȃ��B", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 160, "�ˑR�����A��`�������Ă��炦�邩���H", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 180, "���Ă̒ʂ�A�l�͂����Ȃ��L�ł�", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 200, "���܂�ڂ��ǂ��Ȃ���", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 240, "�����ْ����Ȃ��Ă����v�A", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 260, "�l�����ĕ����N�������炢�͂ł��邩��", GetColor(0, 0, 0), TRUE);

			break;
		case GAME_SCENE_PLAY:
			//=============================================
			//     �v���C�̕���     
			//=============================================

			DrawString(GAME_WIDTH / 2 + 170, 100, "���ꂶ�Ⴀ�N�ɂ́A�E�����𕗂ɏ悹", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 120, "���q�͔��d���܂ő���͂��Ă��炨������", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 160, "�����Ă钆���q�ɂ�", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 180, "�댯�����瓖����Ȃ��ł�", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 220, "��H�Ȃ�Ŗl��������������̂����āH", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 240, "��l�̒m�b�Ƃł������Ă���������", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 280, "�\���L�[:�ړ�", GetColor(0, 0, 0), TRUE);
			DrawString(GAME_WIDTH / 2 + 170, 320, "1:�X�s�[�hUP�@2:�X�s�[�hDOWN", GetColor(0, 0, 0), TRUE);

			break;
		case GAME_SCENE_END:
			//=============================================
			//     �G���h�̕���     
			//=============================================

			//�Q�[���N���A���̕���
			if (GameEndFlag == GAME_CLEAR)
			{
				DrawString(GAME_WIDTH / 2 + 170, 100, "�N���A���߂łƂ�", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 140, "���Ȃ��̂������ŕ����͔��W����", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 180, "���𑗂��Ă��������̂����Ȃ��L�ł�", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 200, "���̌��ʂɂ͌ւ炵���v��", GetColor(0, 0, 0), TRUE);
			}

			//�Q�[���I�[�o�[�̎��̕���
			if (GameEndFlag == GAME_OVER)
			{
				DrawString(GAME_WIDTH / 2 + 170, 100, "�ȂɁA�������炢�ւ܂��������đ��v��", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 120, "�Ȃɂ��l�����s�����������", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 160, "�E�E�E�l�̎��s�k�ł�", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 180, "���悤�Ǝv�����̂�����ǁE�E�E", GetColor(0, 0, 0), TRUE);
				DrawString(GAME_WIDTH / 2 + 170, 240, "�͂āA���̔L�͉��C���������ȁE�E�H", GetColor(0, 0, 0), TRUE);
			}

			break;
		default:
			break;
		}
	}
	return;
}


//=====================================================================================================================
//          �R�R����؂�ւ�          
//=====================================================================================================================

/// <summary>
/// �؂�ւ����
/// </summary>
/// <param name=""></param>
VOID Change(VOID)
{
	ChangeProc();    //����
	ChangeDraw();    //�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
/// <param name=""></param>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCutMax)
		{
			fadeInCnt--;               //�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����
			fadeInCnt = fadeInCntInit; //�J�E���^������
			IsFadeIn = FALSE;          //�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCutMax)
		{
			fadeOutCnt++;               //�J�E���g�����炷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����
			fadeOutCnt = fadeOutCntInit; //�J�E���^������
			IsFadeOut = FALSE;          //�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I�����H
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����t�F�[�h�A�E�g�����Ă��Ȃ���
		GameScene = NextGameScene;  //���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;   //�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();      //�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();       //�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();        //�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCutMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCutMax) * 255);
	}

	//�l�p�`��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));

	return;
}


//=====================================================================================================================
//          �R�R���瓖���蔻��֌W          
//=====================================================================================================================

/// <summary>
/// �����蔻��̗̈�X�V(�v���C���[)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.X;
	chara->coll.top = chara->img.Y;
	chara->coll.right = chara->img.X + chara->img.width;
	chara->coll.bottom = chara->img.Y + chara->img.height;

	return;
}

/// <summary>
/// �������Ă��邩�𒲂ׂ�
/// </summary>
/// <param name=""></param>
/// <returns>�������Ă�E�E�ETRUE�b�������Ă��Ȃ��E�E�EFALSE</returns>
BOOL OnCollision(RECT coll1, RECT coll2)
{
	//�������Ă�����TRUE
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
//          �R�R����BGM�֌W          
//=====================================================================================================================

/// <summary>
/// BGM�̉��ʂ�ύX����
/// </summary>
/// <param name="music">�ύX������BGM��AUDIO�\���̂̃A�h���X</param>
VOID ChangeBGM(AUDIO* music)
{
	//���ʂ��O�`�Q�T�T�̎��ɍ쓮
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

	//���ʂ��͈͊O��������͈͓��ɖ߂�
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