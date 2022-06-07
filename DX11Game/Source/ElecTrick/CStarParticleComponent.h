//======================================================================
//											CStarParticleComponent.h
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
#include "CSpriteRenderer.h"
#include "CRigidbody2D.h"
#include "CClearComponent.h"

//===== �N���X��` =====
class CStarParticleComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---

	int m_nNumberCnt;	// �L�[�v����
	float m_fRoll;
	float m_fAlpha;		// �����x

	std::weak_ptr<CSpriteRenderer> m_renderer;
	std::weak_ptr<CRigidbody2D>	m_rb;

	enum NUMBER_FADE
	{
		eNone,
		eFadeKeep,
		eFadeOut,

		eMaxState,
	};

	NUMBER_FADE m_curFade;

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

	void SetStarAlphe(float alpha);

	//void SetStarForce(Vector3);

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
};

