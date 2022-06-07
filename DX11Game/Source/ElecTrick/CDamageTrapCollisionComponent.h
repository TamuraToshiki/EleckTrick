//======================================================================
//											CDamageTrapCollisionComponent.h
//	�_���[�W�g���b�v
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/12/13	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CObjectManager.h"
#include "CDamageTrapObj.h"
#include "CRespawnComponent.h"
#include <vector>

#include "debugproc.h"


//===== �N���X��` =====
class CDamageTrapCollisionComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CCollision2D> m_col;		//���W�b�h�{�f�B�p
	std::weak_ptr<CRespawnComponent> m_res;

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

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

};



