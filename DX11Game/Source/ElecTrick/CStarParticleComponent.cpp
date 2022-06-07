//======================================================================
//											CStarParticleComponent.cpp
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
#include "CStarParticleComponent.h"
#include "CObjectManager.h"
#include "CClearComponent.h"

//===== �}�N����` =====
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)
#define SCREEN_CENETR_X	(SCREEN_WIDTH / 2)
#define SCREEN_CENETR_Y	(SCREEN_HEIGHT / 2)

#define FADE_SPEED	(0.8f / 60.0f)	// 60/60��1�t���[�� 1/60��1�b
#define FADE_KEEP	(0.5f)	// �b

#define STAR_SIZE_MAX	(70)

#define ROLL_SPEED	(3)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CStarParticleComponent::Start()
{
	Vector3 vec;
	float spd = 5;
	int size = rand() % STAR_SIZE_MAX;
	vec = Vector3{CosDeg(rand() % 180) , SinDeg(rand() % 90) , 0.0f };
	if (vec->y <= SinDeg(65))
	{
		vec->y *= 2;
	}

	m_pParent->m_pTrans->m_scale = Vector3{ (float)size,(float)size,0 };

	// �����_���[
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/star.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI4);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);
	m_rb = m_pParent->AddComponent<CRigidbody2D>();
	m_fRoll = 0.0f;
	m_rb.lock()->SetDragX(0);
	m_rb.lock()->SetForce(vec * spd);
	m_rb.lock()->SetMoveLimit(false);

	m_fAlpha = 1.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;

	m_renderer.lock()->SetAlpha(m_fAlpha);
}


//===================================
//
//	�X�V�֐�CStarParticleComponent
//
//===================================
void CStarParticleComponent::Update()
{
	m_fRoll += ROLL_SPEED;
	m_pParent->m_pTrans->SetRotation({ 0, 0, m_fRoll });
	m_renderer.lock()->SetAlpha(m_fAlpha);

	if (m_pParent->m_pTrans->GetPos()->y <= -(SCREEN_CENETR_Y))
	{
		m_rb.lock()->SetForceY(m_rb.lock()->GetForce()->y * -1);
	}

	switch (m_curFade)
	{
	case CStarParticleComponent::eNone:
		m_curFade = eFadeKeep;
		break;

	case CStarParticleComponent::eFadeKeep:
		m_nNumberCnt++;
		if (m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CStarParticleComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		break;
	case CStarParticleComponent::eMaxState:

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
void CStarParticleComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CStarParticleComponent::End()
{

}

void CStarParticleComponent::SetStarAlphe(float alpha)
{
	m_fAlpha = alpha;
}

//void CStarParticleComponent::SetStarForce(Vector3 force)
//{
//	const auto& rb = m_rb.lock();
//	if (rb)	return;
//
//	rb->SetForce(force);
//}


//===== �g��Ȃ����͏����Ă�������() =====
//===================================
//
//	2D�����蔻��(Enter) �R�[���o�b�N�֐�
//
//===================================
void CStarParticleComponent::OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	// �^�O�ȂǂŔ���
	if (collsion2d)
	{

	}
}

