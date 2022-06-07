//==================================================================
//								CSceneEnding.h
//	�G���f�B���O�V�[��
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2021/01/23	�쐬
//
//===================================================================



//====== �C���N���[�h�� ======
#include "CSceneEnding.h"
#include "CObjectManager.h"
#include "input.h"
#include "camera.h"
#include "CBg.h"
#include "CSpriteObject.h"
#include "CSceneManager.h"
#include "CSceneGame.h"
#include "Sound.h"

#include "CEndingObj.h"
#include "CGameManagerObject.h"
#include "CEditorObject.h"
#include "CEditorComponent.h"


//===== �}�N����` =====


//===== �O���[�o���ϐ� =====


//========================================
//
//	�R���X�g���N�^
//
//========================================
CSceneEnding::CSceneEnding()
{
	m_sceneName = "Ending";
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CSceneEnding::~CSceneEnding()
{

}


//========================================
//
//	������
//
//========================================
void CSceneEnding::Init()
{
	CSound::StopBGM("BGM.mp3");

	// �G���f�B���O
	CObjectManager::CreateObject<CEndingObj>();

	// �X�e�[�W��������
	CEditorComponent::SetCurrentStageNum(0);
}

//========================================
//
//	�I������
//
//========================================
void CSceneEnding::Uninit()
{
}

//========================================
//
//	�S�ẴI�u�W�F�N�g�̍X�V
//
//========================================
void CSceneEnding::Update()
{
}


//========================================
//
//	�S�ẴI�u�W�F�N�g�̕`��
//
//========================================
void CSceneEnding::Draw()
{
}

