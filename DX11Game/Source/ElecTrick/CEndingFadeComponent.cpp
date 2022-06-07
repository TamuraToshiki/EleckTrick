//======================================================================
//											CEndingFadeComponent.cpp
//	�G���f�B���O���o�p�̃t�F�[�h�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2021/01/21 �쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CEndingFadeComponent.h"
#include "CObjectManager.h"
#include "input.h"
#include "Sound.h"

#include "CElvLeftDoorObj.h"
#include "CElvRightDoorObj.h"
#include "CWhiteTileObj.h"
#include "CEndingPicObj.h"
#include "CContenueObj.h"

#include "CSceneManager.h"
#include "CSceneTitle.h"

#include "debugproc.h"


//===== �}�N����` =====
#define TIME_UNTILL_START	(6.0f)		//�h�A�������o���܂ł̎���
#define TIME_UNTILL_FADE	(12.0f)		//�t�F�[�h�̎���
#define ADD_ALPHA			(0.1f)		//1�t���[���ɉ��Z����郿�l
#define ADD_TIMER			(0.1f)		//�^�C�}�[�ւ̉��Z�l

#define DOOR_MOVE_X			(4.0f)		//�h�A�ړ��� / f

#define DOOR_SCALE_X		(1280)		//�h�A�X�P�[��
#define DOOR_SCALE_Y		(720)
#define ENDING_SCALE_X		(1280)		//�ꖇ�G�X�P�[��
#define ENDING_SCALE_Y		(720)
#define CONTENUE_SCALE_X	(350.0f)	//Contenue�X�P�[��
#define CONTENUE_SCALE_Y	(100.0f)

#define CONTENUE_POS_X		(450.0f)	//Contenue�ʒu
#define CONTENUE_POS_Y		(-300.0f)

#define ALPHA3			(0.01f)

//�e�N�X�`���p�X
#define ELVR_TEXTURE_PATH		"data/texture/Ending/Right.png"
#define ELVL_TEXTURE_PATH		"data/texture/Ending/Left.png"
#define WHITE_TEXTURE_PATH		"data/texture/Ending/WhiteTile.png"
#define ENDING_TEXTURE_PATH		"data/texture/Ending/Ending.png"
#define BACK_TEXTURE_PATH		"data/texture/Ending/EndingBack.png"
#define CONTENUE_TEXTURE_PATH	"data/texture/Ending/ToBeContenued.png"

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====

//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CEndingFadeComponent::Start()
{
	//������
	m_fTimer = 0.0f;
	m_fAlpha = 1.0f;
	m_fAlpha2 = 0.0f;
	m_fAlpha3 = 0.0f;
	m_bFadeEnd = false;	
	m_bMoveEnd = false;

	//���o�p�I�u�W�F�N�g�̐���
	//�����_���[�擾
	m_SpriteEnding[0] = CObjectManager::CreateObject<CElvRightDoorObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ DOOR_SCALE_X,DOOR_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[1] = CObjectManager::CreateObject<CElvLeftDoorObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ DOOR_SCALE_X,DOOR_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[2] = CObjectManager::CreateObject<CWhiteTileObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ ENDING_SCALE_X,ENDING_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[3] = CObjectManager::CreateObject<CEndingPicObj>(
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ ENDING_SCALE_X,ENDING_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	m_SpriteEnding[4] = CObjectManager::CreateObject<CContenueObj>(
		{ CONTENUE_POS_X,CONTENUE_POS_Y,0.0f },
		{ 0.0f,0.0f,0.0f },
		{ CONTENUE_SCALE_X,CONTENUE_SCALE_Y,0.0f })
		->GetComponent<CSpriteRenderer>();

	

	//���C���[�Z�b�g
	m_SpriteEnding[0].lock()->SetLayer(CSpriteRenderer::eUI5);		//�G���x�[�^�[�h�AR
	m_SpriteEnding[1].lock()->SetLayer(CSpriteRenderer::eUI5);		//�G���x�[�^�[�h�AL
	m_SpriteEnding[2].lock()->SetLayer(CSpriteRenderer::eUI4);		//���w�i
	m_SpriteEnding[3].lock()->SetLayer(CSpriteRenderer::eUI4);		//�r�p���E
	m_SpriteEnding[4].lock()->SetLayer(CSpriteRenderer::eUI4);		//Contenue����

	//�e�N�X�`���Z�b�g
	m_SpriteEnding[0].lock()->SetTexture(ELVR_TEXTURE_PATH);
	m_SpriteEnding[1].lock()->SetTexture(ELVL_TEXTURE_PATH);
	m_SpriteEnding[2].lock()->SetTexture(WHITE_TEXTURE_PATH);
	m_SpriteEnding[3].lock()->SetTexture(ENDING_TEXTURE_PATH);
	m_SpriteEnding[4].lock()->SetTexture(CONTENUE_TEXTURE_PATH);
	

	//�A���t�@�l�Z�b�g
	m_SpriteEnding[0].lock()->SetAlpha(m_fAlpha);
	m_SpriteEnding[1].lock()->SetAlpha(m_fAlpha);
	m_SpriteEnding[2].lock()->SetAlpha(1.0f);
	m_SpriteEnding[3].lock()->SetAlpha(m_fAlpha2);
	m_SpriteEnding[4].lock()->SetAlpha(0.0f);
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CEndingFadeComponent::Update()
{
	//�^�C�}�[�ɉ��Z
	m_fTimer += ADD_TIMER;

	//�����n�C���C�g
	static int nCnt = 0;
	nCnt += 1;
	m_fTextColor = abs(sinf(nCnt * 3.1415f / 100));

	//�^�C�}�[���ݒ�l�𒴂�����
	if (m_fTimer > TIME_UNTILL_START)
	{
		//�h�A�ړ�
		m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x += DOOR_MOVE_X;
		m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x -= DOOR_MOVE_X;
		CSound::PlayBGM("EndingDoorMove.mp3");

		if (m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x == SCREEN_CENTER_X &&
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x == -SCREEN_CENTER_X)
		{
			m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x =  SCREEN_CENTER_X;
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x = -SCREEN_CENTER_X;
		} 
		
		//�ꖇ�G�\��
		if (m_SpriteEnding[0].lock()->m_pParent->m_pTrans->m_pos->x > SCREEN_CENTER_X &&
			m_SpriteEnding[1].lock()->m_pParent->m_pTrans->m_pos->x < -SCREEN_CENTER_X)
		{
			CSound::StopBGM("EndingDoorMove.mp3");
			CSound::PlayBGM("Strong_Wind.mp3");
			m_fAlpha2 += 0.01f;
			m_SpriteEnding[3].lock()->SetAlpha(m_fAlpha2);
			if (m_fAlpha2 > 1.0f)
			{
				m_fAlpha2 = 1.0f;
				m_bFadeEnd = true;
			}
		}
	}

	//�\�����I�������
	if (m_bFadeEnd)
	{
		m_fAlpha3 += ALPHA3;
		m_SpriteEnding[4].lock()->SetAlpha(m_fAlpha3);
		m_SpriteEnding[4].lock()->SetColor(m_fTextColor, m_fTextColor, m_fTextColor);
		m_bMoveEnd = true;
	}

	if (m_bMoveEnd)
	{
		if (GetKeyTrigger(VK_RETURN) || GetKeyTrigger(VK_SPACE) ||
			GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
		{
			//�^�C�g���V�[���ֈڍs
			CSceneManager::Get()->SetNextScene<CSceneTitle>();
			CObjectManager::DestroyObject(m_pParent);
			CSound::StopBGM("Strong_Wind.mp3");
		}
	}
	
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CEndingFadeComponent::LateUpdate()
{

}


//===================================
//
//	�I���֐�
//
//===================================
void CEndingFadeComponent::End()
{

}


