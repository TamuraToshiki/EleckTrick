//==================================================================
//								CScene.h
//	�V�[��
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/16	�V�[���N���X�쐬
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include <string>

//===== �N���X��` =====
class CScene
{
public:
	CScene();
	virtual ~CScene();

	// �l�又��
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	std::string& GetSceneName() { return m_sceneName; }

protected:
	std::string m_sceneName;
};


