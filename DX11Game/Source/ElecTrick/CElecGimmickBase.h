//======================================================================
//											CElecGimmickBase.h
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/26	�쐬
//		 11/27	Enter,Stay,Exit�̏������z�֐� �ǉ�
//				�ѓdtrue��false�ɂȂ����ۂɃM�~�b�N���~�܂�Ȃ��o�O��������������
//		 11/29	ChargeGimmick��������ɂ����������߁AChargePlayer�ɕύX
//				����ɔ����p�����Ă�<CLiftComponent><CSwitchComponent>���ύX
//		 11/30  �o�O�̌����̂��߁AChargePlayerExit�̃��\�[�X����r��
//				m_fEnergyBulletResource�̎c�ʂ�0�ɂȂ����ۂɃM�~�b�N���~�܂�Ȃ��o�O�𔭌�
//				�������z�֐�EnergyOff()��ǉ����A���ʂ�}�邽�߁AEnergyGimmick��EnergyOn�ɖ��̕ύX
//				
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"
#include "CResourceEnergy.h"
#include "CEnergyShot.h"


//===== �N���X��` =====
class CElecGimmickBase : public CComponent
{
private:
	// --- �ϐ��錾 ---
	bool m_bCurChargeflg;
	bool m_bOldChargeflg;

protected:
	float m_fUseResource;	// �����
	float m_fEnergyBulletResource;		// �G�l���M�[�ʂ�ۊǂ���ϐ�

	std::string m_energyOnTexture;		// �ʓd���̃e�N�X�`��
	std::string m_energyOffTexture;		// ��ʓd���̃e�N�X�`��

public:
	// --- �֐��錾 ---
	// �ѓd�������z�֐�
	virtual void ChargePlayerEnter(std::weak_ptr<CCollision2D>) = 0;
	virtual void ChargePlayerStay(std::weak_ptr<CCollision2D>) = 0;
	virtual void ChargePlayerExit(std::weak_ptr<CCollision2D>) = 0;
	// �d�C�e�������z�֐�
	virtual void EnergyOn() = 0;
	virtual void EnergyOff() = 0;
	// �M�~�b�N�A�b�v�f�[�g
	virtual void GimmickUpdate() {};

	// �ѓd

	// �쐬���ɌĂ΂�܂�
	virtual void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() final override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	virtual void LateUpdate() override;

	// �M�~�b�N�̃��X�^�[�g
	virtual void Restart(){}


	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

};



