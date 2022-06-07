//======================================================================
//											CMeshObject.h
//	MeshObjectBase
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
class CMeshObject : public CObject
{
public:
	CMeshObject();
	virtual ~CMeshObject();

protected:
	std::weak_ptr<CMeshRenderer> m_renderer;

private:
};


//===== �v���g�^�C�v�錾 =====


