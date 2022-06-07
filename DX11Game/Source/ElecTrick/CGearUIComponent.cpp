//======================================================================
//											CGearUIComponent.cpp
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
#include "CGearUIComponent.h"
#include "CObjectManager.h"

//===== �}�N����` =====
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)
#define SCREEN_CENETR_X	(SCREEN_WIDTH / 2)
#define SCREEN_CENETR_Y	(SCREEN_HEIGHT / 2)

#define FADE_SPEED	(0.6f / 60.0f)	// 60/60��1�t���[�� 1/60��1�b
#define FADE_KEEP	(0.5f)	// �b

#define STAR_SIZE_MAX	(70)

#define ROLL_SPEED	(300)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CGearUIComponent::Start()
{
	// �����_���[
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/gear2.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI4);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);
	m_fRoll = 1.0f;

	m_fAlpha = 0.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;

	m_renderer.lock()->SetAlpha(m_fAlpha);
}


//===================================
//
//	�X�V�֐�CGearUIComponent
//
//===================================
void CGearUIComponent::Update()
{
	//�����n�C���C�g
	static int nCnt = 10;
	nCnt += 1;
	m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));
	m_fRoll += (ROLL_SPEED / m_pParent->m_pTrans->GetScale()->x) * m_nrot;
	m_pParent->m_pTrans->SetRotation({ 0, 0, m_fRoll });
	m_renderer.lock()->SetAlpha(m_fAlpha);

	switch (m_curFade)
	{
	case CGearUIComponent::eNone:
		m_curFade = eFadeIn;
		break;

	case CGearUIComponent::eFadeIn:
		if (m_fAlpha <= 1)
		{
			m_fAlpha += FADE_SPEED;
		}
		else if (m_fAlpha >= 1)
		{
			m_curFade = eFadeKeep;
		}
		break;

	case CGearUIComponent::eFadeKeep:
		m_nNumberCnt++;
		if (m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CGearUIComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		break;
	case CGearUIComponent::eMaxState:

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
void CGearUIComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CGearUIComponent::End()
{

}

void CGearUIComponent::SetGearRot(int rot)
{
	m_nrot = rot;
}

//void CGearUIComponent::SetStarAlphe(float alpha)
//{
//	m_fAlpha = alpha;
//}

//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CGearUIComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d)
	{

	}
}

