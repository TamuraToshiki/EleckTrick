//==================================================================
//								CHierarchyComponent.h
//	�^�C�g���ł̃����\��UI
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2021/01/15	�^�C�g�����\��UI�N���X�쐬
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "CSpriteRenderer.h"
#include "CComponent.h"
#include "CEditorComponent.h"

//===== �}�N����` =====


//===== �N���X��` =====

class CHierarchyComponent;

class CHierarchyComponentData :public CComponent
{
	friend CHierarchyComponent;
private:
	// ----- �����o -----
	std::weak_ptr<CSpriteRenderer> m_sprite;

	int m_nAnimNo;

public:
	// ----- ���\�b�h -----

	// �J�n���ɌĂ΂�܂�
	void Start() override;

};

class CHyphenComponent :public CComponent
{
	friend CHierarchyComponent;
private:
	// ----- �����o -----
	std::weak_ptr<CSpriteRenderer> m_sprite;

public:
	// ----- ���\�b�h -----

	// �J�n���ɌĂ΂�܂�
	void Start() override;
};

class CHierarchyComponent : public CComponent
{
private:
	// ----- �����o -----

	int m_nScore;
	int m_nNumberCnt;
	float m_fAlpha;

	std::weak_ptr<CSpriteRenderer> m_renderer;

	enum NUMBER_FADE
	{
		eNone,
		eFadeIn,
		eFadeKeep,
		eFadeOut,

		eMaxState,
	};

	NUMBER_FADE m_curFade;

	// ����
	std::vector<std::weak_ptr<CHierarchyComponentData>> m_data;
	std::weak_ptr<CHyphenComponent> m_hyphen;

public:
	// �X�R�A�̃Z�b�g
	void SetScore(int nScore) { m_nScore = nScore; }

	// ----- ���\�b�h -----

	// �J�n���ɌĂ΂�܂�
	void Start() override;
	// ���t���[���Ă΂�܂�
	void Update() override;
	// ���t���[���X�V��ɌĂ΂�܂�
	void LateUpdate() override;
	// �I�����ɌĂ΂�܂�
	void End() override;

	// UV�X�V
	void UpdateTexCoord();
};
