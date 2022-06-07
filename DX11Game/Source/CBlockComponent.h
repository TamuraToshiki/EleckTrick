//======================================================================
//											CBlockComponent.h
//	�u���b�N�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2021/01/21	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "Renderer.h"


//===== �N���X��` =====
class CBlockComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CMeshRenderer> m_renderer;

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

	// �e�N�X�`���̎w��
	void SetTexture(int nTexNo);

};



