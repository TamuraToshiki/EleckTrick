//==================================================================
//								CSceneGame.h
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
#pragma once


//====== �C���N���[�h�� ======
#include "CScene.h"

//===== �N���X��` =====
class CSceneGame : public CScene
{
public:
	CSceneGame();
	~CSceneGame();

	// �l�又��
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:

};


