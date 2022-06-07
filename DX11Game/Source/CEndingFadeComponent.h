//======================================================================
//											CEndingFadeComponent.h
//	�G���f�B���O���o�p�̃t�F�[�h�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2021/01/21 �쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CSpriteRenderer.h"
#include <functional>


//===== �N���X��` =====
class CEndingFadeComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	float m_fTimer;			//�^�C�}�[
	float m_fAlpha;			//���l�p
	float m_fAlpha2;		//���l�p2
	float m_fAlpha3;		//���l�p3
	float m_fTextColor;		//�n�C���C�g�p

	bool m_bMoveEnd;		//�ړ��I���t���O
	bool m_bFadeEnd;		//�t�F�[�h�I���t���O
	
	std::weak_ptr<CSpriteRenderer> m_SpriteEnding[5];	//�����_���[

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �������ɌĂ΂�܂�
	void End() override;

};



