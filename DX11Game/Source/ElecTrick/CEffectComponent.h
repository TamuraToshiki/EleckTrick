//======================================================================
//											CEffectComponent.h
//	�G�t�F�N�g�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2021/01/23	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "Renderer.h"
#include "CAnimater2D.h"


//===== �N���X��` =====
class CEffectComponent : public CComponent
{
public:
	// �G�t�F�N�g�̎��
	enum EEffectType
	{
		eHitEffect,
		eSaveEffect,
		eSmokeEffect,

		eMaxEffect,
	};

private:
	// --- �ϐ��錾 ---

	// �����_���[
	std::weak_ptr<CBillboardRenderer> m_renderer;

	// �A�j���[�^�[
	std::weak_ptr<CAnimator2D> m_animator;

	// ���[�v
	bool m_bLoop;

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

	// �G�t�F�N�g�̎w��
	void SetEffect(EEffectType eType);

	// �C�ӃG�t�F�N�g�̎w��
	void SetAnyEffect(std::string filename, int nSplitX, int nSplitY, int nSpeed = 2, bool bLoop = false);

	//--- �R�[���o�b�N�֐� ---
	// �A�j���[�V����
	void OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d) override;
};



