//======================================================================
//											CBillboardObject.h
//	BillboardObjectBase
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/19	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "Renderer.h"


//===== �\���̒�` =====


//===== �N���X��` =====
class CBillboardObject : public CObject
{
public:
	CBillboardObject();
	virtual ~CBillboardObject();

protected:
	std::weak_ptr<CBillboardRenderer> m_renderer;

private:
};


//===== �v���g�^�C�v�錾 =====


