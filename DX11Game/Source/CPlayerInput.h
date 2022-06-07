//======================================================================
//											CPlayerInput.h
//	�v���C���[�ړ�
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/11/16	�쐬
//	2020/12/16	�F�����ҏW
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CPlayerActionComponent.h"
#include "Renderer.h"
#include "CEnergyShot.h"
#include "CProductEnergy.h"


//===== �N���X��` =====
class CPlayerInput : public CComponent
{
private:
	// �v���C���[�̃A�N�V����
	std::weak_ptr<CPlayerActionComponent> m_playerAction;
	std::weak_ptr<CEnergyShot> m_shot;
	std::weak_ptr<CProductEnergy> m_product;

	// �{�̃A�j���[�V����
	std::weak_ptr<CAssimpRenderer> m_renderer;

	// �R���g���[���[�����邩
	int m_nJoyCount;

	enum EPlayerAnimation
	{
		eAttack,
		eGeneration,
		eJump,
		eRun,

		eMaxAnim,
	};

public:

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �������ɌĂ΂�܂�
	void End() override;

	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	// 3D�A�j���[�V����
	void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer) override;
};



