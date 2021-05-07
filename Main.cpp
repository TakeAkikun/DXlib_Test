//DxLib�̃w�b�_�t�@�C���i�K�{�j
#include "DxLib.h"

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



	//�~�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	//�l�p�`�̑傫�������߂�
	int width = 10;
	int height = 10;

	//�~�̔��a�����߂�
	int radius = 25;

	//�������x�����߂�
	int Xspead = 1;
	int Yspead = 1;

	//�X�s�[�h�A�b�v����񐔂����߂�
	int Scount = 3;  //�c���v3��

	//
	SetDrawScreen(DX_SCREEN_BACK);

	
	//�������[�v
	while (1)
	{
		                        //��������L�[�������ꂽ�烋�[�v�𔲂���
		if (CheckHitKeyAll() != 0)
		{
			break;
		}
		                        //���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0) 
		{
			break;              //�G���[��E�B���h�E������ꂽ�烋�[�v�𔲂���
		}

		//��ʂ��N���A
		if (ClearDrawScreen() != 0) { break; }

		//�l�p�`��`��
		//DrawBox(
		//	X, Y, X + width, Y + height,
		//	GetColor(255, 0, 0),
		//	TRUE                //�h��Ԃ����ǂ���
		//);

		//�~��`��
		DrawCircle(
			X, Y, radius,
			GetColor(0, 255, 0),
			FALSE,5
		);

			X += Xspead;         //�l�p�`���E���ɓ�����
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
			X++;                 //�l�p�`���E���ɓ�����
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

		ScreenFlip();           //�_�u���o�b�t�@�����O������ʂ�`��
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				    // �\�t�g�̏I�� 
}