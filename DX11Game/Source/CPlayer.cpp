//======================================================================
//												CPlayer.cpp
//	�v���C���[
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CPlayer.h"
#include "CCollision2D.h"
#include "CRigidbody2D.h"

#include "CPlayerActionComponent.h"
#include "CPlayerInput.h"
#include "CEnergyShot.h"
#include "CProductEnergy.h"
#include "CRespawnComponent.h"

#include "CPlayerUIComponent.h"


//===== �}�N����` =====
#define DEFAULT_MODEL			"data/model/MainAnim_Blender/Main_Re_Blender.fbx"
#define TEXTURE_PATH			"data/model/Model_Main/texture_10.png"


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�R���X�g���N�^
//
//===================================
CPlayer::CPlayer()
{
	// �g�����X�t�H�[��
	//m_pTrans->SetScale({ 100, 50, 50 });


	// �^�O�̐ݒ�
	m_name = m_tag = "Player";

	// ���f���̃��[�h
	m_renderer.lock()->ModelLoad(DEFAULT_MODEL);
	m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH);


	//--- �R���|�[�l���g�̒ǉ�
	const auto& col = AddComponent<CCollision2D>();
	AddComponent<CRigidbody2D>();

	AddComponent<CResourceEnergy>();
	AddComponent<CProductEnergy>();
	AddComponent<CRespawnComponent>();
	
	AddComponent<CPlayerActionComponent>();
	AddComponent<CEnergyShot>();
	AddComponent<CPlayerInput>();

	AddComponent<CPlayerUIComponent>();

	// �R���W����
	col->SetScale({ 90, 90, 90 });
	col->SetOffSet({ 0, 40, 0 });
}

//===================================
//
//	�f�X�g���N�^
//
//===================================
CPlayer::~CPlayer()
{

}

