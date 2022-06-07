//======================================================================
//											CProductEnergy.h
//	���d�E����
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/20	��蒼��
//	2020/12/10  �\�[�X�����E�o�O���
//	2020/12/11	10���Ɉ��������o�O���
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "input.h"
#include "CResourceEnergy.h"
#include "Renderer.h"
#include "CSpriteObject.h"
#include "CRigidbody2D.h"

//===== �N���X��` =====
class CProductEnergy : public CComponent
{
private:
	// --- �ϐ��錾 ---
	float m_fAdd;					//���Z��
	float m_fSub;					//���Z��
	bool m_bTrigger_Z;				//�{�^���t���O�QZ
	bool m_bTrigger_C;				//�{�^���t���O�QC
	std::weak_ptr<CResourceEnergy> m_resource;

	// ���[�^�[�A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_generater;
	// �v���y��
	std::weak_ptr<CRigidbody2D> m_propeller;

	// ��]��
	float m_fRotSpeed;
	float m_fMaxRotSpeed;
	float m_fAddRotSpeed;
	float m_fSubRotSpeed;
	int m_nEffectCount;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	//-- �I������ --//
	void End() override;

	//--------------------------------------------------
	//	���Z�ʃZ�b�g
	//--------------------------------------------------
	void SetAdd(float fadd = 1.0f);

	//--------------------------------------------------
	//	���Z�ʃZ�b�g
	//--------------------------------------------------
	void SetSub(float fsub = 0.01f);

	//--------------------------------------------------
	//	���d	
	//--------------------------------------------------
	bool GenerateEnergyRight(void);

	bool GenerateEnergyLeft(void);


	//--------------------------------------------------
	//	����
	//--------------------------------------------------
	void ExpenseEnergy(void);
};



