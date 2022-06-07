//==================================================================
//								CSceneTitle.cpp
//	�^�C�g���V�[��
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/12/15	�쐬
//
//===================================================================



//====== �C���N���[�h�� ======
#include "CSceneTitle.h"
#include "CObjectManager.h"
#include "input.h"
#include "camera.h"
#include "CBg.h"
#include "CSceneManager.h"
#include "CSceneGame.h"
#include "Sound.h"

#include "CTitleMenuObj.h"
#include "CEndingObj.h"
#include "CGameManagerObject.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"
#include "CSecurityRobo.h"


//===== �}�N����` =====
#define MAP_NUM		(100)
#define TITLE_TEXTURE_PATH		"data/texture/title.png"
#define PUSH_TEXTURE_PATH		"data/texture/Title_menu/PUSH_BUTTON.png"

//===== �O���[�o���ϐ� =====


//========================================
//
//	�R���X�g���N�^
//
//========================================
CSceneTitle::CSceneTitle()
{
	m_sceneName = "Title";
	m_bOpen = false;
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CSceneTitle::~CSceneTitle()
{

}


//========================================
//
//	������
//
//========================================
void CSceneTitle::Init()
{
	// BGM
	CSound::PlayBGM("TitleBGM.wav", 0.3f);

	//�X�e�[�W�ԍ��ݒ�
	CEditorComponent::SetCurrentStageNum(MAP_NUM);

	//�w�i����
	CObjectManager::CreateObject<CBg>({ 50, 50, 150 });
	const auto& editor = CObjectManager::CreateObject<CEditorObject>();
	editor->SetStop(true);

	//�Z�L�����e�B���{�ݒ�
	const auto& robo = CObjectManager::CreateObject<CSecurityRobo>();
	robo.get()->m_pTrans->m_pos = Vector3({ 1600.0f,1000.0f,0.0f });

	//�J�����ݒ�
	CCamera::GetMainCamera()->SetCameraTarget(robo->m_pTrans);
	CCamera::GetMainCamera()->UpdateCameraPos(100);
	CCamera::GetMainCamera()->SetOffSet({ 0.0f,200.0f, 0.0f });

	// �^�C�g��
	const auto& title = CObjectManager::CreateObject<CSpriteObject>();
	const auto& titleSpr = title->GetComponent<CSpriteRenderer>();
	const auto& push = CObjectManager::CreateObject<CSpriteObject>();
	const auto& pushSpr = push->GetComponent<CSpriteRenderer>();
	
	//�����_���[���
	m_Sprite = titleSpr;
	m_Push = pushSpr;

	// �^�C�g���ݒ�
	title->m_pTrans->SetScale({ SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f });
	titleSpr->m_pParent->m_pTrans->m_pos = Vector3{ 0.0f,100.0f,0.0f };
	titleSpr->SetLayer(titleSpr->eUI1);
	titleSpr->SetTexture(TITLE_TEXTURE_PATH);

	//�{�^�������ݒ�
	push->m_pTrans->SetScale({ 860.0f,82.0f,1.0f });
	pushSpr->SetTexture(PUSH_TEXTURE_PATH);
	pushSpr->m_pParent->m_pTrans->m_pos->y = -230.0f;



}
//========================================
//
//	�I������
//
//========================================
void CSceneTitle::Uninit()
{
	CSound::StopBGM("TitleBGM.wav");
}

//========================================
//
//	�S�ẴI�u�W�F�N�g�̍X�V
//
//========================================
void CSceneTitle::Update()
{
	//�^�C�g���㉺
	static int nCnt = 10;
	nCnt += 2;
	m_Sprite.lock()->m_pParent->m_pTrans->m_pos->y += ((sinf(nCnt * 3.1415f / 100)));

	
	static int nTime = 0;
	nTime += 1;
	m_fAlpha = abs((sinf(nTime * 3.1415f / 100)));
	if(!m_bAlpha)
	m_Push.lock()->SetAlpha(m_fAlpha);

	//�V�[���ύX
	if (GetKeyTrigger(VK_SPACE) || GetKeyTrigger(VK_RETURN) ||
		GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
	{
		m_Push.lock()->SetAlpha(0.0f);
		m_bAlpha = true;
		CSound::PlaySE("ChoiseDecide.mp3");
		if(!m_bOpen)
		CObjectManager::CreateObject<CTitleMenuObj>();

		m_bOpen = true;
	}

	//�G���f�B���O��
#ifdef _DEBUG
	if (GetKeyTrigger(VK_P))
	{
		CSound::StopBGM("TitleBGM.wav");
		CObjectManager::CreateObject<CEndingObj>();
	}
#endif


}


//========================================
//
//	�S�ẴI�u�W�F�N�g�̕`��
//
//========================================
void CSceneTitle::Draw()
{
}

