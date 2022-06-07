//==================================================================
//								CObject.h
//	オブジェクトベースクラス
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/09/23	オブジェクトクラス作成
//
//===================================================================


//===== インクルード部 =====
#include "CObject.h"
#include "CObjectManager.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	コンストラクタ
//
//===================================
CObject::CObject()
{
	m_pTrans = this->AddComponent<CTransform>();
	// ここで座標を入れたい!
	m_pTrans->m_pos = CObjectManager::m_posBuffer;
	m_pTrans->m_rot = CObjectManager::m_rotBuffer;
	m_pTrans->m_scale = CObjectManager::m_scaleBuffer;

	m_name = m_tag = "Object";
	m_bStop = false;
}

//===================================
//
//	デストラクタ
//
//===================================
CObject::~CObject()
{
	// リストをクリア
	m_ComponentList.clear();
	m_DestroyComponentList.clear();
}

