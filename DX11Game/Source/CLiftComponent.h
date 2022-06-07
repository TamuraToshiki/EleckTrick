//======================================================================
//											CLiftComponent.h
//	���t�g�𓮂����R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/19	�쐬
//		 11/20�@����
//		 11/28�@ChargePlayerEnter,ChargePlayerStay,ChargePlayerExit�ǉ�
//		 11/30  CreatePoint�����O���������N�����\�������邽�߁A
//				CreateLiftPoint�ɕύX
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CResourceEnergy.h"
#include "CElecGimmickBase.h"

#include "debugproc.h"


//===== �N���X��` =====
class CLiftComponent : public CElecGimmickBase
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CResourceEnergy> m_re;
	int currentRoot;						// ���t�g�|�C���g�z��̗v�f
	int OldcurrentRoot;
	std::vector<Vector3> wayPoints;			// ���t�g�|�C���g(���t�g�̖ڎw���|�C���g)
	int MaxWayPoint;		// ���t�g�|�C���g�̍ő�v�f��
	Vector3 vec;							// ���t�g����
	float speed;							// ���t�g�X�s�[�h
	bool bLiftMoveflg;
	bool m_bTurnFlg;						// �ʓd�t���O
	bool m_bBulletFlg;

	std::vector<std::weak_ptr<CObject>> m_aLiftPint;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	void End() override;

	void GimmickUpdate() override;

	void ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d) override;

	void CreateLiftPoint(Vector3 pos);

	// �I�[�o�[���C�h
	// �ѓd�M�~�b�N����
	void ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d) override;

	// �v���C���[�����t�g�Ɨ��ꂽ�Ƃ�
	void ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d) override;

	// �e�M�~�b�N����
	void EnergyOn() override;

	void EnergyOff() override;

	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	std::vector<std::weak_ptr<CObject>>GetLiftPoint() { return m_aLiftPint; }
};

