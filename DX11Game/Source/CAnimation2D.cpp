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

//====== �C���N���[�h�� ======
#include "CAnimater2D.h"
#include "CAnimation2D.h"


//===== �}�N����` =====


//===== �N���X��` =====



//===== �v���g�^�C�v�錾 =====




//========================================
//
//	�R���X�g���N�^
//
//========================================
CAnimation2D::CAnimation2D()
{
	m_nArraySize = 0;
	m_pAnimNoArray = nullptr;
	m_name = "\0";
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CAnimation2D::~CAnimation2D()
{
	if (m_pAnimNoArray != nullptr)
	{
		delete[] this->m_pAnimNoArray;
	}
}

//
////========================================
////
////	����
////
////========================================
//CAnimation2D* CAnimation2D::Create(std::string pszName, int nMaxFrame, ...)
//{
//	// �ψ������X�g
//	va_list args;
//	int nCount;
//	
//	// ���I�ɃA�j���[�V�����N���X�̊m��
//	CAnimation2D *pAnim = new CAnimation2D;
//
//	// �ψ������X�g�쐬
//	va_start(args, nMaxFrame);
//
//	// �ψ����̐����v�Z
//	nCount = nMaxFrame;
//
//	// �ψ������������m��
//	pAnim->m_pAnimNoArray = new int[nCount];
//
//	// �z���nAnimNo�����ԂɊi�[
//	for (int i = 0; i < nCount; i++)
//	{
//		pAnim->m_pAnimNoArray[i] = va_arg(args, int);
//	}
//
//	// �ψ������X�g�I��
//	va_end(args);
//
//	// �A�j���[�V�����̖��O�̊i�[
//	pAnim->m_pszName = pszName;
//	// �z��(�A�j���[�V�����̃R�})�̐����i�[
//	pAnim->m_nArraySize = nCount;
//
//	// �Ō�ɃA�j���[�^�[�̃��X�g�ɐV����������A�j���[�V�������i�[
//	pAnimator->AddAnimation(pAnim);
//
//	return pAnim;
//}
//
////========================================
////
////	���� (�����ŃR�}�ԍ��U�蕪��)
////
////========================================
//CAnimation2D* CAnimation2D::Create(CAnimator* pAnimator, const char *pszName, unsigned int nMaxFrame)
//{
//	// ���I�ɃA�j���[�V�����N���X�̊m��
//	CAnimation2D *pAnim = new CAnimation2D;
//
//	// �������m��
//	pAnim->m_pAnimNoArray = new int[nMaxFrame];
//
//	// �z���nAnimNo�����ԂɊi�[
//	for (int i = 0; i < (int)nMaxFrame; i++)
//	{
//		pAnim->m_pAnimNoArray[i] = i;
//	}
//
//	// �A�j���[�V�����̖��O�̊i�[
//	pAnim->m_pszName = pszName;
//	// �z��(�A�j���[�V�����̃R�})�̐����i�[
//	pAnim->m_nArraySize = nMaxFrame;
//
//	// �Ō�ɃA�j���[�^�[�̃��X�g�ɐV����������A�j���[�V�������i�[
//	pAnimator->AddAnimation(pAnim);
//	pAnim->m_pAnimator = pAnimator;
//
//	return pAnim;
//}