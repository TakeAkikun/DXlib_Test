#include "DxLib.h"      //DxLib�̃w�b�_�t�@�C���i�K�{�j
#include "keyboard.h"   //�L�[�{�[�h�̏���


//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��" //�Q�[���^�C�g��
#define GAME_WIDTH   1280            //�Q�[����ʂ̕�
#define GAME_HEIGHT  720             //�Q�[����ʂ̕�
#define GAME_COLOR   32              //�Q�[����ʂ̐F��

#define GAME_ICON_ID 333             //�Q�[���̃A�C�R����ID

#define GAME_WINDOW_BAR 0            //�E�B���h�E�o�[�̎��

enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};  //�Q�[���̃V�[��

//��l���i�{�[���j
struct PLAYER
{
	int X;
	int Y;
	int radius;
	int Xspead;
	int Yspead;
};

//�v���C��ʂ̃M�~�b�N
struct GIMIC
{
	int Xwall;
	int Ywall;
	int XwallSize;
	int YwallSize;
	int speadwall;
};

//�O���[�o���ϐ�

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
int fadeInCutMax = fadeTimeMax;                       //�t�F�[�h�C���̃J�E���^XAX

//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;       //���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;    //�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;   //����̃Q�[���̃V�[��

//�M�~�b�N�̕ϐ�
GIMIC sikaku = { 1000,300,20,20,20 };
PLAYER Player = { GAME_WIDTH / 2,GAME_HEIGHT / 2,10,5,5 };

//�v���g�^�C�v�錾
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

VOID ChangeScene(GAME_SCENE seane);                              //�V�[���؂�ւ�


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


	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�������[�v
	while (1)
	{
	    
		if (ProcessMessage() != 0) { break; }  //���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; } //��ʂ��N���A

		AllKeyUpdate();         //�L�[�{�[�h���͂̍X�V

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

		ScreenFlip();           //�_�u���o�b�t�@�����O������ʂ�`��
	}


	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				    // �\�t�g�̏I�� 
}


/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene"></param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;  //�V�[���؂�ւ�
	IsFadeIn = FALSE;             //�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;             //�t�F�[�h�A�E�g����

	return;
}

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
	//�v���C�V�[���ɐ؂�ւ�
	if (KeyClick(KEY_INPUT_RETURN) == TRUE){
		//�V�[���؂�ւ�
		//���̃V�[���̏��������R�R�ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	DrawString(GAME_HEIGHT / 2, GAME_WIDTH / 2, "Enter�L�[�ŃQ�[���J�n", GetColor(255, 0, 0));
	return;
}


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
	//�L�[����
		//�ǂ�˂������Ȃ��悤��if���𒲐�
	if (KeyDown(KEY_INPUT_UP) == TRUE && Player.Y > 0 + Player.radius)
	{
		Player.Y -= Player.Yspead;   //��Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.Y < 0 + Player.radius)
		{
			Player.Y = 0 + Player.radius;
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.Y < GAME_HEIGHT - Player.radius)
	{
		Player.Y += Player.Yspead;   //���Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.Y > GAME_HEIGHT - Player.radius)
		{
			Player.Y = GAME_HEIGHT - Player.radius;
		}
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.X > 0 + Player.radius)
	{
		Player.X -= Player.Xspead;   //���Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.X < 0 + Player.radius)
		{
			Player.X = 0 + Player.radius;
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.X < GAME_WIDTH - Player.radius)
	{
		Player.X += Player.Xspead;   //�E�Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.X > GAME_WIDTH - Player.radius)
		{
			Player.X = GAME_WIDTH - Player.radius;
		}
	}

	// �P�ŃX�s�[�hUP�E�Q�ŃX�s�[�hDOWN�i0�̎��͂���������Ȃ��j
	if (KeyDown(KEY_INPUT_1) == TRUE)
	{
		Player.Xspead++;
		Player.Yspead++;
	}
	if (KeyDown(KEY_INPUT_2) == TRUE && Player.Xspead > 0 && Player.Yspead > 0)
	{
		Player.Xspead--;
		Player.Yspead--;
	}

	//�l�p�̓���
	sikaku.Ywall += sikaku.speadwall;
	if (sikaku.Ywall == 0 || sikaku.Ywall == GAME_HEIGHT - sikaku.YwallSize)
	{
		sikaku.speadwall = -sikaku.speadwall;
	}

	//�G���h�V�[���ɐ؂�ւ�
	if (TRUE && Player.X >= GAME_WIDTH - Player.radius) {
		//�V�[���؂�ւ�
		//���̃V�[���̏��������R�R�ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�G���h�V�[���ɐ؂�ւ��i�ی��j
	if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
		//�V�[���؂�ւ�
		//���̃V�[���̏��������R�R�ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	DrawString(0, 20, "��ʉE�[�܂ł��ǂ蒅�����I", GetColor(0, 0, 0));
	DrawString(0, 40, "�i1�ŃX�s�[�h�A�b�v�F2�ŃX�s�[�h�_�E���j", GetColor(0, 0, 0));

	//�ۂ̕`��
	DrawCircle(Player.X, Player.Y, Player.radius, GetColor(0, 255, 0), TRUE);

	//�l�p�̕`��
	DrawBox(sikaku.Xwall, sikaku.Ywall, sikaku.Xwall + sikaku.XwallSize, sikaku.Ywall + sikaku.YwallSize, GetColor(0, 0, 0), TRUE);

	return;
}


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
	//�^�C�g���V�[���ɐ؂�ւ�
	if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
		//�V�[���؂�ւ�
		//���̃V�[���̏��������R�R�ōs���Ɗy

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
/// <param name=""></param>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	DrawString(GAME_HEIGHT / 2, GAME_WIDTH / 2, "�N���A���߂łƂ��I", GetColor(255, 0, 0));
	return;
}


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