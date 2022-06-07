//======================================================================
//											CMeshObject.h
//	MeshObjectBase
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/15	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "Renderer.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CMeshObject : public CObject
{
public:
	CMeshObject();
	virtual ~CMeshObject();

protected:
	std::weak_ptr<CMeshRenderer> m_renderer;

private:
};


//===== プロトタイプ宣言 =====


