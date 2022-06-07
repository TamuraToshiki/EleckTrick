//==================================================================
//								CSceneGame.cpp
//	�Q�[���V�[��
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/12/14	�쐬
//
//===================================================================



//====== �C���N���[�h�� ======
#include "CSceneGame.h"
#include "CObjectManager.h"
#include "transform.h"
#include "input.h"
#include "camera.h"
#include "CCollision2D.h"
#include "CObjectBaseLayout.h"
#include "CPlayer.h"
#include "CBlock.h"
#include "CBg.h"
#include "CEnergyBullet.h"
#include "CStartElevator.h"
#include "CEndElevator.h"
#include "CDebugObject.h"
#include "CEditorObject.h"

#include "CGameManagerObject.h"

#include "Sound.h"


//===== �}�N����` =====


//===== �O���[�o���ϐ� =====

//========================================
//
//	�R���X�g���N�^
//
//========================================
CSceneGame::CSceneGame()
{
	m_sceneName = "Game";
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CSceneGame::~CSceneGame()
{

}


//========================================
//
//	������
//
//========================================
void CSceneGame::Init()
{
	// �I�u�W�F�N�g�}�l�[�W���[
	CObjectManager::CreateObject<CGameManagerObject>();

	// BGM
	CSound::PlayBGM("BGM.mp3", 0.5f);

	// �w�i�̐���
	CObjectManager::CreateObject<CBg>({ 50, 50, 150 });

	// �o���b�g
	CObjectManager::CreateObject<CEnergyBullet>({ 10000, 10000, 1000 });
}
//========================================
//
//	�I������
//
//========================================
void CSceneGame::Uninit()
{

}

//========================================
//
//	�S�ẴI�u�W�F�N�g�̍X�V
//
//========================================
void CSceneGame::Update()
{

}


//========================================
//
//	�S�ẴI�u�W�F�N�g�̕`��
//
//========================================
void CSceneGame::Draw()
{
}

