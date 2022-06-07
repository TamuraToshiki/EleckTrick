//==================================================================
//								CSceneManager.h
//	�V�[���Ǘ�
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/16	�V�[���Ǘ��N���X�쐬
//
//===================================================================


//====== �C���N���[�h�� ======
#include "CSceneManager.h"

#include "CSceneTitle.h"
#include "CSceneGame.h"

#include "CObjectManager.h"
#include "CCollision2D.h"


//===== �}�N����` =====


//===== �O���[�o���ϐ� =====


//===== �ÓI�����o =====


//========================================
//
//	�R���X�g���N�^
//
//========================================
CSceneManager::CSceneManager()
{
	// �ŏ��̃V�[��
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;

	m_bStop = false;
	m_bSceneChange = false;

}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CSceneManager::~CSceneManager()
{
	// �ŏ��̃V�[��
	m_pCurrentScene = nullptr;
	m_pNextScene = nullptr;

	m_bStop = false;
	m_bSceneChange = false;
}


//========================================
//
//	������
//
//========================================
void CSceneManager::Init()
{
	// �X�g�b�v�t���O
	m_bStop = false;
	// �V�[���ύX
	m_bSceneChange = false;

	// �t�F�[�h�̏�����
	for (const auto& com : m_fade.GetComponentList()) com->Start();

	// �ŏ��̃V�[���̃Z�b�g
	m_pCurrentScene	= new CSceneTitle();
	m_pNextScene	= m_pCurrentScene;


	// �I�u�W�F�N�g�N���A
	CObjectManager::ClearObjectList();

	// �ŏ��̃V�[���̏�����
	m_pCurrentScene->Init();

}


//========================================
//
//	�I������
//
//========================================
void CSceneManager::Uninit()
{
	// �V�[���̏I������
	m_pCurrentScene->Uninit();

	CObjectManager::ClearObjectList();
}


//========================================
//
//	�S�ẴI�u�W�F�N�g�̍X�V
//
//========================================
void CSceneManager::Update()
{
	// ���݂̃V�[���̍X�V����
	m_pCurrentScene->Update();

	// �I�u�W�F�N�g�̍X�V
	CObjectManager::Update();

	// �����蔻��
	CCollision2D::CollisionUpdate();

	// �I�u�W�F�N�g�̌�X�V
	CObjectManager::LateUpdate();

	// �I�u�W�F�N�g�̏���
	CObjectManager::DestroyUpdate();

	// �t�F�[�h�̍X�V
	m_fade.Update();
	m_fade.LateUpdate();
}


//========================================
//
//	�S�ẴI�u�W�F�N�g�̕`��
//
//========================================
void CSceneManager::Draw()
{
	// ���݂̃V�[���̕`�揈��
	m_pCurrentScene->Draw();

	// �����蔻��̕`��
	CCollision2D::CollisionDraw();
}



//========================================================
//
//	���̃V�[���ֈړ�
//
//========================================================
void CSceneManager::SetSceneChange()
{
	// ���݂̃V�[���̏I���������Ă�
	m_pCurrentScene->Uninit();

	// �I�u�W�F�N�g�N���A
	CObjectManager::ClearObjectList();

	// ���݂̃V�[���̏���
	delete m_pCurrentScene;

	// ���݂̃V�[���̕ύX
	m_pCurrentScene = m_pNextScene;

	// ���̃V�[���̏�����
	m_pCurrentScene->Init();

	// �V�[���ύX�t���O�I�t
	m_bSceneChange = false;
}
