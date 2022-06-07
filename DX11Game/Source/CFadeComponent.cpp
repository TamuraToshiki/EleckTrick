//======================================================================
//											CFadeComponent.cpp
//	�t�F�[�h�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/06	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CFadeComponent.h"
#include "CObjectManager.h"
#include "input.h"
#include "Sound.h"

#include "debugproc.h"


//===== �}�N����` =====
#define SCALE_SPEED_X (0.05f * SCREEN_WIDTH)
#define SCALE_SPEED_Y (0.05f * SCREEN_HEIGHT)

#define MAX_SCALE (2.5f)

//===== �� =====
namespace
{
	enum E_STATE_FADE	// �t�F�[�h�̏�Ԃ̗�
	{
		E_STATE_FADE_NODE = 0,
		E_STATE_FADE_OUT,
		E_STATE_FADE_IN,

		E_STATE_FADE_MAX,
	};
}


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CFadeComponent::Start()
{
	// �X�P�[��
	m_fScale = MAX_SCALE;
	// �X�e�[�g
	m_nState = E_STATE_FADE_NODE;
	// �t���[��
	m_nFrame = 60;
	// �֐��I�u�W�F�N�g
	m_fadeEndFunc = nullptr;


	// �����_���[�擾
	const auto& render = m_pParent->GetComponent<CSpriteRenderer>();
	m_render = render;
	render->m_pParent->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH * MAX_SCALE, SCREEN_HEIGHT * MAX_SCALE, 1 };

	// �S�́��𐶐�
	for (int i = 0; i < 4; ++i)
	{
		m_obj[i] = CObjectManager::CreateObject<CSpriteObject>();
		const auto& spr = m_obj[i]->GetComponent<CSpriteRenderer>();

		spr->SetColor(render->GetColor());
		spr->SetAlpha(render->GetAlpha());
		spr->SetLayer(render->GetLayer());
		spr->SetMeshBlendState(render->GetMeshBlendState());
	}

	// �E
	m_obj[0]->m_pTrans->m_pos = Vector3{ SCREEN_CENTER_X * MAX_SCALE, 0, 0 };
	m_obj[0]->m_pTrans->m_scale = Vector3{ 0, SCREEN_HEIGHT, 0 };
	// ��
	m_obj[1]->m_pTrans->m_pos = Vector3{ -SCREEN_CENTER_X * MAX_SCALE, 0, 0 };
	m_obj[1]->m_pTrans->m_scale = Vector3{ 0, SCREEN_HEIGHT, 0 };

	// ��
	m_obj[2]->m_pTrans->m_pos = Vector3{ 0, SCREEN_CENTER_Y * MAX_SCALE, 0 };
	m_obj[2]->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH, 0, 0 };
	// ��
	m_obj[3]->m_pTrans->m_pos = Vector3{ 0, -SCREEN_CENTER_Y * MAX_SCALE, 0 };
	m_obj[3]->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH, 0, 0 };
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CFadeComponent::Update()
{
	//if (GetKeyPress(VK_J))
	//{
	//	m_pParent->m_pTrans->m_scale->x -= SCALE_SPEED_X;
	//	m_pParent->m_pTrans->m_scale->y -= SCALE_SPEED_Y;

	//	// ���~�b�g
	//	if (m_pParent->m_pTrans->m_scale->x < 0.0f)
	//	{
	//		m_pParent->m_pTrans->m_scale->x = 0.0f;
	//		m_pParent->m_pTrans->m_scale->y = 0.0f;
	//		return;
	//	}

	//	// �E�E��
	//	m_obj[0]->m_pTrans->m_scale->x += SCALE_SPEED_X;
	//	m_obj[1]->m_pTrans->m_scale->x += SCALE_SPEED_X;

	//	//��E��
	//	m_obj[2]->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//	m_obj[3]->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//}
	//if (GetKeyPress(VK_L))
	//{
	//	m_pParent->m_pTrans->m_scale->x += SCALE_SPEED_X;
	//	m_pParent->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//	
	//	// ���~�b�g
	//	if (m_pParent->m_pTrans->m_scale->x > SCREEN_WIDTH * 2.5f)
	//	{
	//		m_pParent->m_pTrans->m_scale->x = SCREEN_WIDTH *  2.5f;
	//		m_pParent->m_pTrans->m_scale->y = SCREEN_HEIGHT * 2.5f;
	//		return;
	//	}

	//	// �E�E��
	//	m_obj[0]->m_pTrans->m_scale->x -= SCALE_SPEED_X;
	//	m_obj[1]->m_pTrans->m_scale->x -= SCALE_SPEED_X;

	//	//��E��
	//	m_obj[2]->m_pTrans->m_scale->y -= SCALE_SPEED_Y;
	//	m_obj[3]->m_pTrans->m_scale->y -= SCALE_SPEED_Y;
	//}


	//if (GetKeyPress(VK_RETURN))
	//{
	//	StartFadeOut();
	//}

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CFadeComponent::LateUpdate()
{
	// ��Ԃɂ�蕪��
	switch (m_nState)
	{
	case(E_STATE_FADE_IN):		// �t�F�[�h�C��
		// ���l�̌���
		m_fScale -= (MAX_SCALE / m_nFrame);

		// ���l��0��菬�����Ȃ�����I��
		if (m_fScale < 0)
		{
			m_fScale = 0;
			m_nState = E_STATE_FADE_NODE;

			// �t�F�[�h�C���I����̏���
			m_bNowFade = false;

		}
		else if (m_fScale < MAX_SCALE / 2)
		{
			// �t�F�[�h�C���I���܂ŉ�ʂ��~�߂�
			//CSceneManager:: SetStopFlag(false);
		}

		// �e�X�P�[���̍X�V
		// �~
		m_pParent->m_pTrans->m_scale = 
			Vector3{ (MAX_SCALE - m_fScale) * SCREEN_WIDTH, (MAX_SCALE - m_fScale) * SCREEN_HEIGHT, 1.0f };

		// �E�E��
		m_obj[0]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;
		m_obj[1]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;

		//��E��
		m_obj[2]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;
		m_obj[3]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;

		break;

	case(E_STATE_FADE_OUT):		// �t�F�[�h�A�E�g
		// ���l�̑���
		m_fScale += (MAX_SCALE / m_nFrame);

		// ���l��MAX_SCALE�𒴂�����t�F�[�h�C����
		if (m_fScale > MAX_SCALE)
		{
			m_fScale = MAX_SCALE + 1.0f;
			m_nState = E_STATE_FADE_IN;

			// �t�F�[�h�A�E�g���I������Ƃ��Ɏ������鏈��
			// �V�[���̈ړ�
			if (m_fadeEndFunc) m_fadeEndFunc();

			// �T�E���h
			CSound::PlaySE("fadein.wav");

			// �t�F�[�h�̏���
			CObjectManager::DestroyObject(m_pParent);
		}

		// �e�X�P�[���̍X�V
		// �~
		m_pParent->m_pTrans->m_scale = 
			Vector3{ (MAX_SCALE - m_fScale) * SCREEN_WIDTH, (MAX_SCALE - m_fScale) * SCREEN_HEIGHT, 1.0f };

		// �E�E��
		m_obj[0]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;
		m_obj[1]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;

		//��E��
		m_obj[2]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;
		m_obj[3]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;

		break;

	default:
		break;
	}
}


//===================================
//
//	�I���֐�
//
//===================================
void CFadeComponent::End()
{
	for (int i = 0; i < 4; ++i)
	{
		CObjectManager::DestroyObject(m_obj[i]);
	}
}


//======================================================
//
//	�t�F�[�h�A�E�g�̊J�n
//	�t�F�[�h�A�E�g�J�n���ԂɊO����̌Ăяo���֐�
//
//======================================================
void CFadeComponent::StartFadeOut(int nFrame)
{
	// ���Ƀt�F�[�h���̎��͏������Ȃ�
	if (E_STATE_FADE_NODE != m_nState)
	{
		return;
	}

	m_nFrame = nFrame;
	m_fScale = 0;
	m_nState = E_STATE_FADE_OUT;
	m_bNowFade = true;
}
