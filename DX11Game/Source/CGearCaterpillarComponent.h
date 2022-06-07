//======================================================================
//											CGearCaterpillarComponent.h
//	�M�A�E�L���^�s�������R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2021/01/20	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"


//===== �N���X��` =====
class CGearCaterpillarComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CTransform> m_player;


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

	// �v���C���[�̃Z�b�g
	void SetPlayer(std::weak_ptr<CTransform> player) { m_player = player; }
};



