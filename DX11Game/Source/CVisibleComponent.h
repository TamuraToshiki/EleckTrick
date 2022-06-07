//======================================================================
//											CVisibleComponent.h
//	���͈͗p�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/04	�쐬
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
class CVisibleComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	// �����蔻��t���O
	bool m_bVisibleflg;
	bool m_bTouchflg;

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
	void SetTargetEnemy(std::weak_ptr<CEnemyBase> enemy, Vector3 scale = Vector3{ -100,50,0 }, Vector3 offset = Vector3{ -100,50,0 });

	// �����ύX�p�ϐ�
	void SetDir(Vector3 offset);

	Vector3 GetDir();

	// �����蔻��擾�֐�
	bool GetVisibleflg();


	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};



