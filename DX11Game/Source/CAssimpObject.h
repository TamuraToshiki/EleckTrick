//======================================================================
//											CAssimpObject.h
//	AssimpObjectBase
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/15	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "Renderer.h"


//===== �\���̒�` =====


//===== �N���X��` =====
class CAssimpObject : public CObject
{
public:
	CAssimpObject();
	virtual ~CAssimpObject();

protected:
	std::weak_ptr<CAssimpRenderer> m_renderer;

private:
};


//===== �v���g�^�C�v�錾 =====


