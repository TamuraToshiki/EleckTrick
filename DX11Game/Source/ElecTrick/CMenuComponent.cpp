//======================================================================
//											CMenuComponent.cpp
//	���j���[�p�̃R���|�[�l���g
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2021/01/19 �쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CMenuComponent.h"
#include "input.h"
#include "Sound.h"

#include "CSceneManager.h"
#include "CSceneGame.h"

#include "CEditorComponent.h"
#include "CGameStartObj.h"
#include "CEditorModeObj.h"
#include "CExitObj.h"


//===== �}�N����` =====
#define MIN_INDEX (0)
#define MAX_INDEX (2)

#define TIME_INTERVAL	(2.0f)		//�A�����͖h�~�Ԋu
#define TIME_INTERVAL2	(100.0f)	

#define C_POS_Y		(-230.0f)		//�I�����ʒuY
#define C_SIZE_X	(280.0f)		//�I�����T�C�YX
#define C_SIZE_Y	(159.0f)		//�I�����T�C�YY

#define EDIT_SIZE_X	(280.0f * 1.2)		//�͂ĂȃT�C�YX
#define EDIT_SIZE_Y	(159.0f * 1.2)		//�͂ĂȃT�C�YY

#define MAP_NUM		(-1)					//�X�e�[�W�ԍ�

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CMenuComponent::Start()
{
	m_fTimer = 0.0f;
	m_bOpen = true;
	m_bButton = false;

	if (m_bOpen)
	{
		m_nMenuIndex = 0;
	}


	//�I�����̃I�u�W�F�N�g�𐶐�
	//�I�u�W�F�N�g��z��֊i�[
	m_SpriteMenu[0] =
		CObjectManager::CreateObject<CGameStartObj>({ -370.0f,C_POS_Y,0.0f }, 
													{ 0.0f,0.0f,0.0f }, 
													{ C_SIZE_X,C_SIZE_Y,1.0f })
		->GetComponent<CSpriteRenderer>();
	
	m_SpriteMenu[1] = 
		CObjectManager::CreateObject<CEditorModeObj>({20.0f,C_POS_Y,0.0f},
													 {0.0f,0.0f,0.0f}, 
													 { EDIT_SIZE_X,EDIT_SIZE_Y,1.0f})
		->GetComponent<CSpriteRenderer>();
	
	m_SpriteMenu[2] =
		CObjectManager::CreateObject<CExitObj>({ 400.0f,C_POS_Y,0.0f }, 
											   { 0.0f,0.0f,0.0f }, 
											   { C_SIZE_X,C_SIZE_Y,1.0f })
		->GetComponent<CSpriteRenderer>();
	
	//���C���[�̃Z�b�g
	m_SpriteMenu[0].lock()->SetLayer(4);
	m_SpriteMenu[1].lock()->SetLayer(4);
	m_SpriteMenu[2].lock()->SetLayer(4);
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CMenuComponent::Update()
{
	m_fTimer += 1.0f;

	//�����n�C���C�g
	static int nCnt = 0;
	nCnt += 2;
	m_fChoicesColor = abs(sinf(nCnt * 3.1415f / 100));

	float JoyX = (float)GetJoyX(JOYSTICKID1);
	JoyX /= 32767.0f;
	if (GetJoyCount() <= 0)
	{
		JoyX = 0;
	}
	if (fabsf(JoyX) < 0.3f)
	{
		m_bStick = true;
	}


	//���E���͂őI������ς���
	if (GetKeyTrigger(VK_LEFT) || (JoyX < -0.8f && m_bStick))
	{
		m_nMenuIndex--;
		if (m_nMenuIndex < MIN_INDEX)
			m_nMenuIndex = MIN_INDEX;
		else
			CSound::PlaySE("CursorMove.mp3");

		m_bStick = false;
	}
	if (GetKeyTrigger(VK_RIGHT) || (JoyX > 0.8f && m_bStick))
	{
		m_nMenuIndex++;
		if (m_nMenuIndex > MAX_INDEX)
			m_nMenuIndex = MAX_INDEX;
		else
			CSound::PlaySE("CursorMove.mp3");

		m_bStick = false;
	}

	
	switch (m_nMenuIndex)
	{
	case 0:
		//���C���Q�[���̃I�u�W�F�N�g�������Ă���
		//�F��ς�����E�n�C���C�g�\������
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		break;
	case 1:
		//Edit���[�h�̃I�u�W�F�N�g�������Ă���
		//�n�C���C�g�\��
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		
		break;
	case 2:
		//�Q�[���I���I�u�W�F�N�g�������Ă���
		//�n�C���C�g�\��
		m_SpriteMenu[0].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[1].lock()->SetColor({ 1.0f, 1.0f, 1.0f });
		m_SpriteMenu[2].lock()->SetColor({ 1.0f, 1.0f, m_fChoicesColor });
		break;
	}
	
	
	if (m_fTimer > TIME_INTERVAL)
	{
 		if (GetKeyTrigger(VK_SPACE) || GetKeyTrigger(VK_RETURN) ||
			GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
		{
   			if (!m_bButton)
			{
				//SE�Đ�
				CSound::PlaySE("ChoiseDecide.mp3");

				
				switch (m_nMenuIndex)
				{
				case 0:
					//���C���Q�[��
					m_bButton = true;
					CEditorComponent::SetCurrentStageNum(MAP_NUM);
					CSceneManager::Get()->SetNextScene<CSceneGame>();
					break;

				case 1:
					//Edit���[�h
					//CSceneManager::Get()->SetNextScene<CSceneEdit>();
					break;

				case 2:
					m_bButton = true;
					//Exit����(�Q�[���̏I��)
					HWND hWnd = GetMainWnd();
					PostMessage(hWnd, WM_CLOSE, 0, 0);	// [x]�������ꂽ�悤�ɐU����
					break;
				}
			}
		}
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CMenuComponent::LateUpdate()
{
	
}


//===================================
//
//	�I���֐�
//
//===================================
void CMenuComponent::End()
{
	m_bOpen = false;
}

//�C���f�b�N�X�擾
int CMenuComponent::GetIndex()
{
	return m_nMenuIndex;
}

//�t���O�擾
bool CMenuComponent::GetBOpen()
{
	return m_bOpen;
}

//�t���O�Z�b�g
void CMenuComponent::SetFlag(bool bFlag)
{
	m_bOpen = bFlag;
}