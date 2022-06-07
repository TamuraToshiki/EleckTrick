//======================================================================
//											CFadeComponent.h
//	�t�F�[�h�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/06	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CSpriteObject.h"
#include <functional>


//===== �N���X��` =====
class CFadeComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CSpriteRenderer> m_render;
	std::shared_ptr<CObject> m_obj[4];

	float m_fScale;	// �t�F�[�h�p�|���S���̃A���t�@�l(0�`255)
	int m_nState;	// ���
	int m_nFrame;	// �t�F�[�h�ɂ�����t���[����

	bool m_bNowFade;// �t�F�[�h����

	std::function<void(void)> m_fadeEndFunc;

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

	// �t�F�[�h�A�E�g�̊J�n
	void StartFadeOut(int nFrame = 60);

	// �t�F�[�h����
	bool GetIsFade() { return m_bNowFade; }

	// �t�F�[�h��̊֐��I�u�W�F�N�g��o�^
	void SetFadeEndFunc(std::function<void(void)> func) { m_fadeEndFunc = func; }

};



