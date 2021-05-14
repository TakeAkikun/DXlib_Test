#include "DxLib.h"      //DxLib�̃w�b�_�t�@�C���i�K�{�j
#include "keyboard.h"   //�L�[�{�[�h�̏���


//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��" //�Q�[���^�C�g��
#define GAME_WIDTH   1280            //�Q�[����ʂ̕�
#define GAME_HEIGHT  720             //�Q�[����ʂ̕�
#define GAME_COLOR   32              //�Q�[����ʂ̐F��

#define GAME_ICON_ID 333             //�Q�[���̃A�C�R����ID

#define GAME_WINDOW_BAR 0            //�E�B���h�E�o�[�̎��

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

	//�~�̒��S�_
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;
	//�~�̔��a
	int radius = 50;

	//�X�s�[�h
	int Xspead = 5;
	int Yspead = 5;


	//�������[�v
	while (1)
	{
	    
		if (ProcessMessage() != 0) { break; }  //���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; } //��ʂ��N���A

		AllKeyUpdate();         //�L�[�{�[�h���͂̍X�V

		//�L�[����
		if (KeyDown(KEY_INPUT_UP) == TRUE && Y > 0 + radius)
		{
			Y -= Yspead;   //��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE && Y < GAME_HEIGHT - radius)
		{
			Y += Yspead;   //���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE && X > 0 + radius)
		{
			X -= Xspead;   //���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE && X < GAME_WIDTH - radius)
		{
			X += Xspead;   //�E�Ɉړ�
		}

		// �P�ŃX�s�[�hUP�E�Q�ŃX�s�[�hDOWN
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

		ScreenFlip();           //�_�u���o�b�t�@�����O������ʂ�`��
	}


	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				    // �\�t�g�̏I�� 
}