//======================================================================
//											CSpriteEffectComponent.h
//	�X�v���C�g�G�t�F�N�g�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2021/01/24	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CSpriteEffectComponent.h"
#include "CObjectManager.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CSpriteEffectComponent::Start()
{
	//m_pParent->m_pTrans->m_scale = Vector3{ 100,100,100 };

	// �����_���[�̎w��
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetMeshBlendState(BS_ADDITIVE);
	m_renderer.lock()->SetLayer(CSpriteRenderer::eUI3);

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
void CSpriteEffectComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CSpriteEffectComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CSpriteEffectComponent::End()
{

}


//===================================
//
// �G�t�F�N�g�̎w��
//
//===================================
void CSpriteEffectComponent::SetEffect(EEffectType eType)
{
	switch (eType)
	{
	case CSpriteEffectComponent::eHitEffect:

		// �e�N�X�`��
		m_renderer.lock()->SetTexture("data/texture/Effect/HitEffect.png");

		// �A�j���[�V����
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(2);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;
	case CSpriteEffectComponent::eSaveEffect:
		// �e�N�X�`��
		m_renderer.lock()->SetTexture("data/texture/Effect/SaveEffect.png");

		// �A�j���[�V����
		m_animator.lock()->SetSplit(5, 2);
		m_animator.lock()->AddAnimationDefault("Effect", 5 * 2);
		m_animator.lock()->SetAnimationSpeed(3);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;

	case CSpriteEffectComponent::eSmokeEffect:
		// �e�N�X�`��
		m_renderer.lock()->SetTexture("data/texture/Effect/SmokeEffect01.png");
		m_renderer.lock()->SetColor({ 0.5f,0.5f,0.5f});

		// �A�j���[�V����
		m_animator.lock()->SetSplit(4, 1);
		m_animator.lock()->AddAnimationDefault("Effect", 4 * 1);
		m_animator.lock()->SetAnimationSpeed(32 / 4);
		m_animator.lock()->SetAnimation("Effect");

		// ���[�v�w��
		m_bLoop = false;

		break;

	case CSpriteEffectComponent::eMaxEffect:
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
void CSpriteEffectComponent::SetAnyEffect(
	std::string filename, int nSplitX, int nSplitY, int nSpeed, bool bLoop, int nStartNo)
{
	// �e�N�X�`��
	m_renderer.lock()->SetTexture(filename.c_str());

	// �A�j���[�V����
	m_animator.lock()->SetSplit(nSplitX, nSplitY);
	m_animator.lock()->AddAnimationDefault("Effect", nSplitX * nSplitY);
	m_animator.lock()->SetAnimationSpeed(nSpeed);
	m_animator.lock()->SetAnimation("Effect");
	m_animator.lock()->SetStartAnimNo(nStartNo);

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
void CSpriteEffectComponent::OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d)
{
	// ���[�v��
	if (m_bLoop) return;

	// �j��
	CObjectManager::DestroyObject(m_pParent);
}

