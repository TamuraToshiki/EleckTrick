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
#pragma once


//====== �C���N���[�h�� ======
#include "CScene.h"

//===== �N���X��` =====
class CSceneEnding : public CScene
{
public:
	CSceneEnding();
	~CSceneEnding();

	// �l�又��
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:


};


