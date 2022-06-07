//======================================================================
//											CEndElevatorComponent.h
//	�G���h�G���x�[�^�[�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/03	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "Renderer.h"


//===== �N���X��` =====
class CEndElevatorComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CAssimpRenderer> m_render;
	std::weak_ptr<CObject> m_player;

	std::vector<std::weak_ptr<CObject>> m_aEVRoad;


	// �����ʒu
	Vector3 m_startPos;
	// �����ʒu
	Vector3 m_goalPos;

	enum EState
	{
		eWait,
		eGaolPosMove,
		eClearMove,
		eOpenDoor,
		eInMove,
		eCloseDoor,
		eEndMove,

		eMaxState,
	};

	// ���݂̏��
	EState m_curState;

public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �I���֐�
	void End() override;


	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
	void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& collsion2d) override;

};



