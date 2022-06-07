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


//===== �C���N���[�h�� =====
#include "CEffectComponent.h"
#include "CObjectManager.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CEffectComponent::Start()
{
	m_pParent->m_pTrans->m_scale = Vector3{ 100,100,100 };

	// �����_���[�̎w��
	m_renderer = m_pParent->AddComponent<CBillboardRenderer>();
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);

	// �A�j���[�^�[�̎擾
	m_animator = m_pParent->AddComponent<CAnimator2D>();

	// ���[�v�w��
	m_bLoop = false;
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CEffectComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CEffectComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CEffectComponent::End()
{

}


//===================================
//
// �G�t�F�N�g�̎w��
//
//===================================
void CEffectComponent::SetEffect(EEffectType eType)
{
	switch (eType)
	{
	case CEffectComponent::eHitEffect:

		// �e�N�X�`��
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/HitEffect.png");

		// �A�j���[�V����
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(2);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;
	case CEffectComponent::eSaveEffect:
		// �e�N�X�`��
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/SaveEffect.png");

		// �A�j���[�V����
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(3);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;

	case CEffectComponent::eSmokeEffect:
		// �e�N�X�`��
		m_renderer.lock()->SetDiffuseTexture("data/texture/Effect/SmokeEffect01.png");
		m_renderer.lock()->SetDiffuseColor({ 0.5f,0.5f,0.5f,0.5f });

		// �A�j���[�V����
		m_animator.lock()->SetSplit(4, 1);
		m_animator.lock()->AddAnimationDefault("Effect", 4 * 1);
		m_animator.lock()->SetAnimationSpeed(32 / 4);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;

	case CEffectComponent::eMaxEffect:
		break;
	default:
		break;
	}

	// �}�g���b�N�X���X�V
	m_renderer.lock()->UpdateTexMatrix();

}

//===================================
//
// �C�ӃG�t�F�N�g�̎w��
//
//===================================
void CEffectComponent::SetAnyEffect(std::string filename, int nSplitX, int nSplitY, int nSpeed, bool bLoop)
{
	// �e�N�X�`��
	m_renderer.lock()->SetDiffuseTexture(filename.c_str());

	// �A�j���[�V����
	m_animator.lock()->SetSplit(nSplitX, nSplitY);
	m_animator.lock()->AddAnimationDefault("Effect", nSplitX * nSplitY);
	m_animator.lock()->SetAnimationSpeed(nSpeed);
	m_animator.lock()->SetAnimation("Effect");

	// ���[�v�w��
	m_bLoop = bLoop;

	// �}�g���b�N�X���X�V
	m_renderer.lock()->UpdateTexMatrix();
}


//===================================
//
//	2D�A�j���[�V�����I���� �R�[���o�b�N�֐�
//
//===================================
void CEffectComponent::OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d)
{
	// ���[�v��
	if (m_bLoop) return;

	// �j��
	CObjectManager::DestroyObject(m_pParent);
}

