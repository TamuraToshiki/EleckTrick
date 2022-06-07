//======================================================================
//											CRespawnComponent.h
//	�v���C���[���X�|�[���p�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/30	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CPlayer.h"
#include "CResourceEnergy.h"
#include "CPlayerInput.h"
#include "CEditorComponent.h"

#include "debugproc.h"


//===== �N���X��` =====
class CRespawnComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	//CSceneTamura InitScene;
	Vector3	  RespawnPoint;				// ���X�|�[���|�C���g			
	//std::weak_ptr<CPlayerInput> m_pi;	// ���������邽�߂Ɏ擾
	//std::weak_ptr<CResourceEnergy> m_pr;// ���������邽�߂Ɏ擾
	//std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CEditorComponent> m_edit;

	bool m_bNowFade;
public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// ���X�|�[���|�C���g��ύX����֐�(����:���X�|�[���|�C���g)
	void SetRespawnPoint(Vector3 pos);

	void Respawn();
};




