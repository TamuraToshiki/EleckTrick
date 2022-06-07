//======================================================================
//											CMenuComponent.h
//	���j���[�p�̃R���|�[�l���g
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CSpriteRenderer.h"

//�萔��`
#define MAX_MENU	(3)

//===== �N���X��` =====
class CMenuComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	bool m_bOpen;				//���j���[�\���t���O
	bool m_bButton;				//����t���O
	int m_nMenuIndex;			//���j���[�I����
	float m_fChoicesColor;		//�n�C���C�g�p
	float m_fTimer;				//�A�����͖h�~�^�C�}�[
	std::weak_ptr<CSpriteRenderer> m_SpriteMenu[MAX_MENU];	//�����_���[�󂯎��p

	// �R���g���[���[�̃X�e�B�b�N�p
	bool m_bStick;

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

	//�C���f�b�N�X�擾
	int GetIndex();

	//�t���O�擾
	bool GetBOpen();

	//�t���O�Z�b�g
	void SetFlag(bool);
};
