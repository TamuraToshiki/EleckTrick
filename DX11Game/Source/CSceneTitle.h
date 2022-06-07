//==================================================================
//								CSceneTitle.h
//	�^�C�g���V�[��
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/12/15	�쐬
//
//===================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CScene.h"
#include "CSpriteRenderer.h"

//===== �N���X��` =====
class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	~CSceneTitle();

	// �l�又��
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:
	std::weak_ptr<CSpriteRenderer> m_Sprite;
	std::weak_ptr<CSpriteRenderer> m_Push;

	float m_fAlpha;	//PUSHBUTTON�̃A���t�@�l

	bool m_bOpen = false;
	bool m_bAlpha = false;

};


