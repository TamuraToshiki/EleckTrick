//======================================================================
//											CPlayerUIComponent.h
//	�v���C���[UI�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/12	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CPlayerUIComponent.h"
#include "CObjectManager.h"
#include "input.h"
#include "CSpriteEffectComponent.h"


//===== �}�N����` =====
#define UI_TEXTURE ("data/texture/PlayerUI3.png")


#define UI_SIZE		(2.0f)
#define UI_SIZE_X (1200.0f / UI_SIZE)
#define UI_SIZE_Y ( 360.0f / UI_SIZE)

#define GAUGE_SIZE_X (UI_SIZE_X * 0.6975f)
#define GAUGE_SIZE_Y (UI_SIZE_Y * 0.6975f)
#define UI_GAUGE_OFFSET_X (830.f / UI_SIZE / 2.0f)

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CPlayerUIComponent::Start()
{
	// UI�g�����X�t�H�[��
	Vector3 pos = {-SCREEN_CENTER_X + UI_SIZE_X / 6 * 5, SCREEN_CENTER_Y - UI_SIZE_Y / 3 * 2, 0.0f};
	Vector3 rot;
	Vector3 UIscale = {UI_SIZE_X, UI_SIZE_Y, 1.0f};

	// �Q�[�W�g�����X�t�H�[��
	Vector3 gaugeScale = { GAUGE_SIZE_X, GAUGE_SIZE_Y, 1.0f };

	//===== �Q�[�W =====
	// �I�u�W�F�N�g
	const auto& gauge = CObjectManager::CreateObject<CObject>(pos,rot,gaugeScale);
	m_gaugeObj = gauge;

	// �X�v���C�g
	const auto& gaugeSpr = gauge->AddComponent<CSpriteRenderer>();
	gaugeSpr->SetLayer(CSpriteRenderer::eUI1);
	gaugeSpr->SetMeshBlendState(BS_ALPHABLEND);
	gaugeSpr->SetColor(Vector3{ 0,1,0 });	// �J���[
	gaugeSpr->SetAlpha(0.8f);
	m_gaugeSprite = gaugeSpr;


	//===== UI�{�� =====
	// �I�u�W�F�N�g
	const auto& ui = CObjectManager::CreateObject<CSpriteObject>(pos, rot, UIscale);
	m_UIObj = ui;
	// �X�v���C�g
	const auto& uiSpr = ui->GetComponent<CSpriteRenderer>();
	uiSpr->SetLayer(CSpriteRenderer::eUI2);
	uiSpr->SetMeshBlendState(BS_ALPHABLEND);
	uiSpr->SetTexture(UI_TEXTURE);	// �e�N�X�`��
	m_UISprite = uiSpr;


	//===== �Q�[�W�G�t�F�N�g =====
	Vector3 pos2 = pos;
	pos2->y += 20;
	const auto& gaugeEffect = CObjectManager::CreateObject<CObject>(pos2, rot, gaugeScale * 1.2f);
	m_gaugeEffectObj = gaugeEffect;
		// �A�j���[�V����
	gaugeEffect->AddComponent<CSpriteEffectComponent>()->
		SetAnyEffect("data/texture/DamageTexture/Elect1.png", 4, 4, 1, true, 0);
	gaugeEffect->AddComponent<CSpriteEffectComponent>()->
		SetAnyEffect("data/texture/DamageTexture/Elect2.png", 4, 4, 2, true, 4);
	gaugeEffect->AddComponent<CSpriteEffectComponent>()->
		SetAnyEffect("data/texture/DamageTexture/Elect3.png", 4, 4, 1, true, 8);
	gaugeEffect->AddComponent<CSpriteEffectComponent>()->
		SetAnyEffect("data/texture/DamageTexture/Elect4.png", 4, 4, 2, true, 12);


	//===== ���\�[�X =====
	m_resource = m_pParent->GetComponent<CResourceEnergy>();


	m_fResourceNum = 0.0f;

	// ����UI
	const auto& opeUI = CObjectManager::CreateObject<CSpriteObject>();
	opeUI->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH, SCREEN_HEIGHT, 1 };
	const auto& opeSpr = opeUI->GetComponent<CSpriteRenderer>();
	opeSpr->SetTexture("data/texture/operationUI.png");
	opeSpr->SetLayer(CSpriteRenderer::eUI3);
	opeSpr->SetMeshBlendState(BS_ALPHABLEND);
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CPlayerUIComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CPlayerUIComponent::LateUpdate()
{
	// ���\�[�X���Q��
	const auto& resource = m_resource.lock();
	if (!resource) return;

	// ���\�[�X���擾
	m_fResourceNum = resource->GetResource();
	float maxResource = resource->GetMaxResource();

	// �J�����Ɠ���
	CCamera::GetMainCamera()->SetLengthOffset(m_fResourceNum * 5.0f);

	// �F�ύX
	const auto& gaugeSpr = m_gaugeSprite.lock();
	if (gaugeSpr)
	{
		int level = resource->GetLevel();
		if (level == 0) gaugeSpr->SetColor(1, 0, 0);
		else if (level == 1) gaugeSpr->SetColor(1, 1, 0);
		else if (level == 2) gaugeSpr->SetColor(0, 1, 0);
		else if (level == 3) gaugeSpr->SetColor(0, 1, 0.5f);
	}

	// �v�Z
	m_fResourceNum *= GAUGE_SIZE_X / maxResource;

	// ���~�b�g
	if (m_fResourceNum > GAUGE_SIZE_X)
		m_fResourceNum = GAUGE_SIZE_X;
	if (m_fResourceNum < 0)
		m_fResourceNum = 0;

	// �Q�[�W�̕ό`
	Vector3 pos = m_UIObj.lock()->m_pTrans->m_pos;
	m_gaugeObj.lock()->m_pTrans->m_pos->x = pos->x - UI_GAUGE_OFFSET_X + m_fResourceNum / 2;
	m_gaugeObj.lock()->m_pTrans->m_scale->x = m_fResourceNum;

	// �Q�[�W�G�t�F�N�g
	m_gaugeEffectObj.lock()->m_pTrans->m_pos->x = pos->x - UI_GAUGE_OFFSET_X + m_fResourceNum / 2;
	m_gaugeEffectObj.lock()->m_pTrans->m_scale->x = m_fResourceNum;
}


//===================================
//
//	�I���֐�
//
//===================================
void CPlayerUIComponent::End()
{
	//===== �Q�[�W =====
	const auto& gauge = m_gaugeObj.lock();
	if (gauge)
	{
		CObjectManager::DestroyObject(gauge);
	}

	//===== UI =====
	const auto& ui = m_UIObj.lock();
	if (ui)
	{
		CObjectManager::DestroyObject(ui);
	}

}

