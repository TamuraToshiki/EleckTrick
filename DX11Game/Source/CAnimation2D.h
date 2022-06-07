//==================================================================
//									animation.h
//	�A�j���[�V����
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/10	�A�j���[�V�����N���X�쐬
//
//===================================================================
#pragma once

//====== �C���N���[�h�� ======
#include <stdarg.h>

//===== �}�N����` =====


//===== �N���X��` =====

// �����F �ėp�^�ɂ���
class CAnimation2D
{
friend CAnimator2D;
public:
	CAnimation2D();
	~CAnimation2D();

	// �Q�b�g�֐�
	std::string GetName() { return m_name; }
	int* GetAnimNoArray() { return m_pAnimNoArray; }
	int GetArraySize() { return m_nArraySize; }
private:
	int* m_pAnimNoArray;
	int m_nArraySize;
	std::string m_name;
};

