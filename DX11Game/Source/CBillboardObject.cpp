//======================================================================
//											CBillboardObject.cpp
//	BillboardObjectBase
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/19	作成
//
//======================================================================


//===== インクルード部 =====
#include "CBillboardObject.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CBillboardObject::CBillboardObject()
{
	m_renderer = AddComponent<CBillboardRenderer>();
}

//===================================
//
//	デストラクタ
//
//===================================
CBillboardObject::~CBillboardObject()
{

}

