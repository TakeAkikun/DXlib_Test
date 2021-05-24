
//�w�b�_�t�@�C���ǂݍ���
#include "geme.h"       //�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"   //�L�[�{�[�h�̏���
#include "FPS.h"        //FPS�̏���

//�\���̂̒�`
struct CHARACTOR
{
	int handle = -1;  //�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];   //�摜�̏ꏊ�i�p�X�j

	int X;            //X�ʒu
	int Y;            //Y�ʒu
	int width;        //��
	int height;       //����
	int Xspead = 1;   //�ړ����x
	int Yspead = 1;   //�ړ����x

	RECT coll;          //�����蔻��̗̈�i�l�p�j
	BOOL IsDraw = FALSE;//�摜���`��ł���H
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

//�v���C���[�̍\����
CHARACTOR Player;

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

VOID CollUpdate(CHARACTOR* chara);                               //�����蔻��̗̈���X�V

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

	//�v���C���[�̉摜��ǂݍ���
	strcpyDx(Player.path, ".\\image\\Player.jpeg");  //�p�X�̃R�s�[
	Player.handle = LoadGraph(Player.path);          //�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(�|1)������
	if (Player.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),   //���C���̃E�B���h�E�^�C�g��
			Player.path,             //���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[",    //���b�Z�[�W�^�C�g��
			MB_OK                    //�{�^��
		);
		DxLib_End();                 //�����I��
		return -1;                   //�G���[�I��
	}

	//��ʂ̕��ƍ������擾
	GetGraphSize(Player.handle, &Player.width, &Player.height);

	//�����蔻����X�V
	CollUpdate(&Player);  //�v���C���[�̓����蔻��̃A�h���X

	//�v���C���[��������
	Player.X = GAME_WIDTH / 2 - Player.width / 2;
	Player.Y = GAME_HEIGHT / 2 - Player.height / 2;
	Player.Xspead = 5;
	Player.Yspead = 5;
	Player.IsDraw = TRUE;

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
	DeleteGraph(Player.handle); //�摜���������ォ��폜

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
	//�G���h�V�[���ɐ؂�ւ�
	if (KeyClick(KEY_INPUT_RETURN) == TRUE) {
		//�V�[���؂�ւ�
		//���̃V�[���̏��������R�R�ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	//�v���C���[�̑���
		//�ǂ�˂������Ȃ��悤��if���𒲐�
	if (KeyDown(KEY_INPUT_UP) == TRUE && Player.Y > 0)
	{
		Player.Y -= Player.Yspead;   //��Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.Y < 0)
		{
			Player.Y = 0;
		}
	}
	if (KeyDown(KEY_INPUT_DOWN) == TRUE && Player.Y < GAME_HEIGHT - Player.height)
	{
		Player.Y += Player.Yspead;   //���Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.Y > GAME_HEIGHT - Player.height)
		{
			Player.Y = GAME_HEIGHT - Player.height;
		}
	}
	if (KeyDown(KEY_INPUT_LEFT) == TRUE && Player.X > 0)
	{
		Player.X -= Player.Xspead;   //���Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.X < 0)
		{
			Player.X = 0;
		}
	}
	if (KeyDown(KEY_INPUT_RIGHT) == TRUE && Player.X < GAME_WIDTH - Player.width)
	{
		Player.X += Player.Xspead;   //�E�Ɉړ�
		//�X�s�[�h�������Ă߂荞�ނ̂�h�~
		if (Player.X > GAME_WIDTH - Player.width)
		{
			Player.X = GAME_WIDTH - Player.width;
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

	//�����蔻����X�V
	CollUpdate(&Player);

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

	//�v���C��ʂ̕`��
	if (Player.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Player.X, Player.Y, Player.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Player.coll.left, Player.coll.top, Player.coll.right, Player.coll.bottom, GetColor(255, 0, 0), FALSE);
		}
	}

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


/// <summary>
/// �����蔻��̗̈�X�V
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->X;
	chara->coll.top = chara->Y;
	chara->coll.right = chara->X + chara->width;
	chara->coll.bottom = chara->Y + chara->height;

	return;
}