//======================================================================
//											CSpriteObject.h
//	SpriteObjectBase
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/12/06	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CSpriteRenderer.h"


//===== �\���̒�` =====


//===== �N���X��` =====
class CSpriteObject : public CObject
{
public:
	CSpriteObject();
	virtual ~CSpriteObject();

protected:
	std::weak_ptr<CSpriteRenderer> m_renderer;

private:
};


//===== �v���g�^�C�v�錾 =====


