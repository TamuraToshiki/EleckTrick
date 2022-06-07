//======================================================================
//											CDamageTrapObjComponent.h
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
#include "CDamageTrapCollisionComponent.h"
#include "CRespawnComponent.h"
#include <vector>

#include "debugproc.h"


//===== �N���X��` =====
class CDamageTrapObjComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CCollision2D> m_col;		//���W�b�h�{�f�B�p
	std::vector<Vector3> m_vTrapPoint;		//�_���[�W�g���b�v�̃|�C���g
	int m_nCurrentRoot;						//�_���[�W�g���b�v�̔z��v�f��
	int m_MaxTrapPoint;						//�g���b�v�|�C���g�̍ő吔
	Vector3 pos;							//�g���b�v�̌���
	Vector3 scale;		//�傫��
	Vector3 m_Radian;	//�p�x
	
	std::weak_ptr<CDamageTrapCollisionComponent> m_DamegeTrap;
	std::vector<std::weak_ptr<CObject>> m_aTrapPoint;
	std::vector<std::weak_ptr<CObject>> m_aTrapcol;
	std::vector<std::weak_ptr<CObject>> m_aTrapEffect;
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
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	//-- �_���[�W�g���b�v�|�C���g�쐬 --//
	void CreateTrapPoint(Vector3);

	//-- �_���[�W�g���b�v�쐬 --//
	void CreateTrap();

	// �p�x�����߂�֐�
	Vector3 GetRadian(Vector3, Vector3);

	// �g���b�v�|�C���g
	std::vector<std::weak_ptr<CObject>>& GetTrapPoint() { return m_aTrapPoint; }
};



