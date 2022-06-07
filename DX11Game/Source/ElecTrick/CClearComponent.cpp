//======================================================================
//											CClearComponent.cpp
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CClearComponent.h"
#include "CObjectManager.h"
#include "CStarParticleComponent.h"
#include "Sound.h"

//===== �}�N����` =====
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)

#define FADE_IN_SPEED	(1.2f / 60.0f)	// 60/60��1�t���[�� 1/60��1�b
#define FADE_OUT_SPEED	(0.8f / 60.0f)
#define FADE_KEEP	(2.0f)	// �b

#define CLEAR_SIZE_X	(1000)
#define CLEAR_SIZE_Y	(CLEAR_SIZE_X / 2)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CClearComponent::Start()
{
	m_pParent->m_pTrans->m_pos = Vector3{ 0,0,0 };
	m_pParent->m_pTrans->m_scale = Vector3{ CLEAR_SIZE_X,CLEAR_SIZE_Y,0 };

	// �����_���[
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/HEWclear.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI5);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);

	m_fAlpha = 0.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;
	m_nStarCnt = 0;

	m_renderer.lock()->SetAlpha(m_fAlpha);

	// �T�E���h
	CSound::PlaySE("StageClear.mp3");

}


//===================================
//
//	�X�V�֐�CClearComponent
//
//===================================
void CClearComponent::Update()
{
	m_nStarCnt++;
	if (m_nStarCnt >= 45 && m_curFade != eFadeOut)
	{

		for (int i = 0; i < 30; i++)
		{
			const auto& star = CObjectManager::CreateObject<CObject>(m_pParent->m_pTrans->GetPos());
			star->AddComponent<CStarParticleComponent>();
			star->GetComponent<CStarParticleComponent>()->SetStarAlphe(m_fAlpha);
		}
		m_nStarCnt = 0;
	}

	switch (m_curFade)
	{
	case CClearComponent::eNone:
		m_curFade = eFadeIn;
		break;

	case CClearComponent::eFadeIn:
		if (m_fAlpha <= 1)
		{
			m_fAlpha += FADE_IN_SPEED;
		}
		else if (m_fAlpha >= 1)
		{
			m_curFade = eFadeKeep;
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		break;

	case CClearComponent::eFadeKeep:
		m_nNumberCnt++;
		if (m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CClearComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_OUT_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		break;
	case CClearComponent::eMaxState:

		break;
	default:
		break;
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CClearComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CClearComponent::End()
{

}


//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CClearComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d)
	{

	}
}

