#pragma once

//�w�b�_�t�@�C����ǂݍ���
#include "DxLib.h"

//�L�[�{�[�h�̃w�b�_�t�@�C��

//�}�N����`
#define KEY_KIND_MAX 256 //�擾����L�[�̎��

//�L�[�{�[�h�\����
struct KEYBOARD
{
	char TempKeyState[KEY_KIND_MAX];     //���͏�Ԃ��Ǘ�
	int AllKeyState[KEY_KIND_MAX];       //���ׂẴL�[�̓��͎��Ԃ��Ǘ�
	int OldKeyState[KEY_KIND_MAX];       //�ȑO�̂��ׂẴL�[�̓��͎��Ԃ��Ǘ�
};

//�O���̃O���[�o���ϐ�
extern KEYBOARD keyboard;

//�v���g�^�C�v�錾
extern VOID AllKeyUpdate(VOID);                         //�S�ẴL�[�̏�Ԃ��擾����
extern BOOL KeyDown(int KEY_INPUT);                     //����̃L�[�������Ă��邩�H
extern BOOL KeyClick(int KEY_INPUT);                    //����̃L�[�������Ă��邩�H
extern BOOL KeyDownKeep(int KEY_INPUT, int millSec);    //����̃L�[���w��̃~���b���������Ă��邩�H