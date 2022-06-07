//======================================================================
//											CPlayerUIComponent.h
//	�v���C���[UI�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/12	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CSpriteObject.h"
#include "CSpriteRenderer.h"
#include "CResourceEnergy.h"


//===== �N���X��` =====
class CPlayerUIComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---

	// �Q�[�W
	std::weak_ptr<CObject> m_gaugeObj;
	std::weak_ptr<CSpriteRenderer> m_gaugeSprite;

	// UI�{��
	std::weak_ptr<CObject> m_UIObj;
	std::weak_ptr<CSpriteRenderer> m_UISprite;

	// �Q�[�W�G�t�F�N�g
	std::weak_ptr<CObject> m_gaugeEffectObj;

	// ���\�[�X
	std::weak_ptr<CResourceEnergy> m_resource;


	// �l
	float m_fResourceNum;

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



