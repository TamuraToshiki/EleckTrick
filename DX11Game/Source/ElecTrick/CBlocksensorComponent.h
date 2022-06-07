//======================================================================
//											CBlocksensorComponent.h
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CCollision2D.h"
#include "CEnemyBase.h"

//===== �N���X�̑O���錾 =====
class CEnemyBase;


//===== �N���X��` =====
class CBlocksensorComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	bool m_bWallflg;

	std::weak_ptr<CEnemyBase> m_enemy;
	std::weak_ptr<CCollision2D> m_col;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// ���͈͂̑傫���ƕ����ύX�֐�
	void SetBlocksensorComponent(std::weak_ptr<CEnemyBase> comp);

	// �n�ʐڐG����擾�֐�
	bool GetWallflg();

	// �����ύX�p�ϐ�
	void SetDir(Vector3 offset);

	Vector3 GetDir();

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};
