//==================================================================
//								CHierarchyComponent.h
//	�^�C�g���ł̃����\��UI
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2021/01/15	�^�C�g�����\��UI�N���X�쐬
//
//===================================================================


//====== �C���N���[�h�� ======

// ���g
#include "CHierarchyComponent.h"
#include "CObjectManager.h"
#include "CGearUIComponent.h"

//===== �}�N����` =====
#define ANIM_NUMBER_SPLIT_X (10)  // �摜�̕�����(��)
#define ANIM_NUMBER_SPLIT_Y (1)  // �摜�̕�����(�c)
#define SCREEN_SCALE_X	(SCREEN_WIDTH / 1920.0f)
#define SCREEN_SCALE_Y	(SCREEN_HEIGHT / 1080.0f)

#define NUMBER_SIZE_X	(85*3 * SCREEN_SCALE_X)
#define NUMBER_SIZE_Y	(95*3 * SCREEN_SCALE_Y)
#define NUMBER_OFFSET_X	(NUMBER_SIZE_X)
#define NUMBER_OFFSET_Y	(NUMBER_SIZE_Y / 3)

#define MAX_NUMBER (2)

#define BEST_SCORE_SIZE_X (2400 / 5 * SCREEN_SCALE_X)
#define BEST_SCORE_SIZE_Y (NUMBER_SIZE_Y)

#define FADE_SPEED	(0.6f / 60.0f)	// 60/60��1�t���[�� 1/60��1�b
#define FADE_KEEP	(0.5f)	// �b


//******************** �X�N���v�g���\�b�h ********************

//========================================
//
//	�J�n��	01
//
//========================================
void CHierarchyComponentData::Start()
{
	// �g�����X�t�H�[��
	m_pParent->m_pTrans->m_scale = Vector3{ NUMBER_SIZE_X, NUMBER_SIZE_Y, 1 };

	// �����_���[
	const auto& renderer = m_pParent->AddComponent<CSpriteRenderer>();
	renderer->SetTexture("data/texture/number.png");
	renderer->SetLayer(renderer->eUI5);
	renderer->SetTexSize(Vector3{ 1.0f / (float)ANIM_NUMBER_SPLIT_X, 1.0f / (float)ANIM_NUMBER_SPLIT_Y , 0 });
	renderer->SetMeshBlendState(BS_ALPHABLEND);
	m_sprite = renderer;

	// �A�j���[�V�����i���o�[
	m_nAnimNo = 0;
}

//========================================
//
//	�J�n��	-
//
//========================================
void CHyphenComponent::Start()
{
	// �g�����X�t�H�[��
	m_pParent->m_pTrans->m_scale = Vector3{ NUMBER_SIZE_X, NUMBER_SIZE_Y, 1 };

	// �����_���[
	const auto& renderer = m_pParent->AddComponent<CSpriteRenderer>();
	renderer->SetTexture("data/texture/a.png");
	renderer->SetLayer(renderer->eUI5);
	renderer->SetMeshBlendState(BS_ALPHABLEND);
	m_sprite = renderer;
}

//========================================
//
//	�J�n��	F
//
//========================================
void CHierarchyComponent::Start()
{
	// �g�����X�t�H�[��
	m_pParent->m_pTrans->m_pos = Vector3{ 0 - NUMBER_OFFSET_X * (1.5f),
		-50 + NUMBER_OFFSET_Y, 1 };
	m_pParent->m_pTrans->m_scale = Vector3{ NUMBER_SIZE_X , NUMBER_SIZE_Y, 1 };

	// �����_���[
	m_renderer = m_pParent->AddComponent<CSpriteRenderer>();
	m_renderer.lock()->SetTexture("data/texture/f.png");
	m_renderer.lock()->SetLayer(m_renderer.lock()->eUI5);
	m_renderer.lock()->SetMeshBlendState(BS_ALPHABLEND);

	m_fAlpha = 0.0f;
	m_nNumberCnt = 0;
	m_curFade = eNone;

	m_renderer.lock()->SetAlpha(m_fAlpha);


	// �����f�[�^�̐���
	for (int i = 0; i < MAX_NUMBER; i++)
	{
		Vector3 pos = m_pParent->m_pTrans->m_pos;
		pos->x += (MAX_NUMBER - i) * NUMBER_OFFSET_X + NUMBER_OFFSET_X;

		const auto& obj = CObjectManager::CreateObject<CObject>(pos);
		m_data.push_back(obj->AddComponent<CHierarchyComponentData>());
	}

	Vector3 pos = m_pParent->m_pTrans->m_pos;
	pos->x += NUMBER_OFFSET_X;
	const auto& hyphen = CObjectManager::CreateObject<CObject>(pos);
	m_hyphen = hyphen->AddComponent<CHyphenComponent>();


	pos = Vector3{ -140,80,0 };
	Vector3 scale = Vector3{ 200,200,0 };
	Vector3 rot = Vector3{ 0,0,0 };
	for (int i = 0 , rotflow = 1; i <= 2; i++)
	{
		const auto& gear = CObjectManager::CreateObject<CObject>(pos, rot, scale);
		gear->AddComponent<CGearUIComponent>()->SetGearRot(rotflow);
		pos *= -1;
		rotflow *= -1;
	}

	pos = Vector3{ -40,-30,0 };
	scale = Vector3{ 100,100,0 };
	for (int i = 0, rotflow = 1; i <= 2; i++)
	{
		const auto& gear = CObjectManager::CreateObject<CObject>(pos, rot, scale);
		gear->AddComponent<CGearUIComponent>()->SetGearRot(rotflow);
		pos *= -1;
		rotflow *= -1;
	}

	//// BestScore
	//Vector3 pos = m_pParent->m_pTrans->m_pos;
	//pos->x -= (MAX_NUMBER)*NUMBER_OFFSET_X + BEST_SCORE_SIZE_X / 2 + NUMBER_OFFSET_X / 2;
	//const auto& best = Instantiate<GameObject>(pos);
	//const auto& bestSpr = best->AddComponent<SpriteRenderer>();
	//bestSpr->SetDiffuseTexture("data/texture/BestScore.png");
	//bestSpr->SetLayer(bestSpr->eUI1);
	//bestSpr->SetRendererBlendState(BS_ALPHABLEND);
	//best->m_pParent->m_pTrans->m_scale = Vector3{ BEST_SCORE_SIZE_X, BEST_SCORE_SIZE_Y, 1 };

	// �X�e�[�W�����擾
	m_nScore = CEditorComponent::GetCurrentStageNum() + 1;

	// ���F�̍X�V
	UpdateTexCoord();
}

//========================================
//
//	�X�V��
//
//========================================
void CHierarchyComponent::Update()
{
	//�����n�C���C�g
	static int nCnt = 10;
	nCnt += 2;

	const auto& hyphen = m_hyphen.lock();
	if (!hyphen) return;

	m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));
	hyphen->m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));

	switch (m_curFade)
	{
	case CHierarchyComponent::eNone:
		m_curFade = eFadeIn;
		break;

	case CHierarchyComponent::eFadeIn:
		if (m_fAlpha <= 1)
		{
			m_fAlpha += FADE_SPEED;
		}
		else if (m_fAlpha >= 1)
		{
			m_curFade = eFadeKeep;
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		hyphen->m_sprite.lock()->SetAlpha(m_fAlpha);
		break;

	case CHierarchyComponent::eFadeKeep:
		m_nNumberCnt++;
		if(m_nNumberCnt >= (FADE_KEEP * 60))
		{
			m_curFade = eFadeOut;
			m_nNumberCnt = 0;
		}
		break;

	case CHierarchyComponent::eFadeOut:
		if (m_fAlpha >= 0)
		{
			m_fAlpha -= FADE_SPEED;
		}
		else if (m_fAlpha <= 0)
		{
			CObjectManager::DestroyObject(m_pParent);
		}
		m_renderer.lock()->SetAlpha(m_fAlpha);
		hyphen->m_sprite.lock()->SetAlpha(m_fAlpha);
		break;
	case CHierarchyComponent::eMaxState:

		break;
	default:
		break;
	}
}

//========================================
//
//	��X�V��
//
//========================================
void CHierarchyComponent::LateUpdate()
{
	// ���F�̍X�V
	UpdateTexCoord();
}

//========================================
//
//	�I����
//
//========================================
void CHierarchyComponent::End()
{
}

//========================================
//
//	UV�X�V
//
//========================================
void CHierarchyComponent::UpdateTexCoord()
{
	int tmp = m_nScore;
	int nCnt = 0;
	static int nTexCnt = 10;
	nTexCnt += 2;

	// �����𐔂���
	while (tmp > 0)
	{
		tmp /= 10;
		nCnt++;
	}

	// UV�X�V
	tmp = m_nScore;
	for (int i = 0; m_data.size() - 1 >= i; i++)
	{
		const auto& data = m_data[i].lock();
		if (!data) continue;

		data->m_nAnimNo = tmp % 10;

		data->m_sprite.lock()->SetTexPos(
			Vector3{ data->m_sprite.lock()->GetTexSize()->x * (data->m_nAnimNo % ANIM_NUMBER_SPLIT_X) ,
					 data->m_sprite.lock()->GetTexSize()->y * (data->m_nAnimNo / ANIM_NUMBER_SPLIT_X),
					 0 });

		//�����n�C���C�g
		data->m_pParent->m_pTrans->m_pos->y += ((sinf(nTexCnt * 3.1415f / 100)));


		/*if (data->m_nAnimNo == 0 && (m_data.size() - i) > nCnt)
		{
			data->m_sprite.lock()->SetAlpha(0);
		}
		else
		{
			data->m_sprite.lock()->SetAlpha(1);
		}*/
		switch (m_curFade)
		{
		case CHierarchyComponent::eNone:
			m_curFade = eFadeIn;
			break;

		case CHierarchyComponent::eFadeIn:
			if (m_fAlpha >= 1)
			{
				m_curFade = eFadeKeep;
			}
			data->m_sprite.lock()->SetAlpha(m_fAlpha);
			break;

		case CHierarchyComponent::eFadeKeep:
			if (m_nNumberCnt >= (FADE_KEEP * 60))
			{
				m_curFade = eFadeOut;
				m_nNumberCnt = 0;
			}
			break;

		case CHierarchyComponent::eFadeOut:
			if (m_fAlpha <= 0)
			{
				CObjectManager::DestroyObject(m_pParent);
			}
			data->m_sprite.lock()->SetAlpha(m_fAlpha);
			break;
		case CHierarchyComponent::eMaxState:

			break;
		default:
			break;
		}

		tmp /= 10;
	}
}
