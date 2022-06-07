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

#pragma once

//====== �C���N���[�h�� ======
#include "CScene.h"
#include "CFadeObject.h"
#include "CFadeComponent.h"
#include "CObjectManager.h"


//===== �N���X��` =====

class CSceneManager
{
public:
	// �l�又��
	void Init();
	void Uninit();
	void Update();
	void Draw();

	// �V�[���̃Z�b�g
	template <class T>
	void SetNextScene();

	void SetSceneChange();

	// �X�V�X�g�b�v�t���O
	void SetStopFlag(bool bStop) { m_bStop = bStop; }

	static CSceneManager* Get() {
		static CSceneManager Instance;
		return &Instance;
	}

	CScene* GetCurrentScene() { return m_pCurrentScene; }

	void StartFade(std::function<void(void)> endFadeFunc)
	{
		// �֐��I�u�W�F�N�g�Z�b�g
		m_fade.GetComponent<CFadeComponent>()->SetFadeEndFunc(endFadeFunc);

		// �t�F�[�h�X�^�[�g
		m_fade.GetComponent<CFadeComponent>()->StartFadeOut();
	}

	bool GetNowFade() { return m_fade.GetComponent<CFadeComponent>()->GetIsFade(); }

private:
	CScene* m_pCurrentScene;
	CScene* m_pNextScene;

	bool m_bStop;
	bool m_bSceneChange;

	CFadeObject m_fade;

	CSceneManager();
	~CSceneManager();
};

//========================================================
//
//	���̃V�[�����Z�b�g
//
//========================================================
template <class T>
void CSceneManager::SetNextScene()
{
	// �V�[���ύX��
	if (m_bSceneChange) return;

	// ���̃V�[���̐����E�i�[
	m_pNextScene = new T();

	// �V�[���ύX�t���O�I��
	m_bSceneChange = true;

	// �֐��I�u�W�F�N�g�Z�b�g
	m_fade.GetComponent<CFadeComponent>()->SetFadeEndFunc([this]() { this->SetSceneChange(); });

	// �t�F�[�h�X�^�[�g
	m_fade.GetComponent<CFadeComponent>()->StartFadeOut();
}