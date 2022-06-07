//======================================================================
//											CBlockComponent.h
//	ブロックコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2021/01/21	作成
//
//======================================================================


//===== インクルード部 =====
#include "CBlockComponent.h"

//===== マクロ定義 =====
#define TEXTURE_PATH0			"data/texture/block/0.png"
#define TEXTURE_PATH1			"data/texture/block/1.png"
#define TEXTURE_PATH2			"data/texture/block/2.png"
#define TEXTURE_PATH3			"data/texture/block/3.png"
#define TEXTURE_PATH4			"data/texture/block/4.png"
#define TEXTURE_PATH5			"data/texture/block/5.png"
#define TEXTURE_PATH6			"data/texture/block/6.png"
#define TEXTURE_PATH7			"data/texture/block/7.png"
#define TEXTURE_PATH8			"data/texture/block/8.png"

#define NORMAL_TEXTURE_PATH0			"data/texture/block/normal/0.png"
#define NORMAL_TEXTURE_PATH1			"data/texture/block/normal/1.png"
#define NORMAL_TEXTURE_PATH2			"data/texture/block/normal/2.png"
#define NORMAL_TEXTURE_PATH3			"data/texture/block/normal/3.png"
#define NORMAL_TEXTURE_PATH4			"data/texture/block/normal/4.png"
#define NORMAL_TEXTURE_PATH5			"data/texture/block/normal/5.png"
#define NORMAL_TEXTURE_PATH6			"data/texture/block/normal/6.png"
#define NORMAL_TEXTURE_PATH7			"data/texture/block/normal/7.png"
#define NORMAL_TEXTURE_PATH8			"data/texture/block/normal/8.png"

#define AMBIENT_TEXTURE_PATH "data/texture/skydome.png"

#define MAX_BLOCK_TEXTURE (9)

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CBlockComponent::Start()
{
	m_renderer = m_pParent->GetComponent<CMeshRenderer>();
}


//===================================
//
//	更新関数
//
//===================================
void CBlockComponent::Update()
{

}


//===================================
//
//	後更新関数
//
//===================================
void CBlockComponent::LateUpdate()
{

}


//===================================
//
//	終了関数
//
//===================================
void CBlockComponent::End()
{

}

void CBlockComponent::SetTexture(int nTexNo)
{
	// ベースカラー
	if (nTexNo % MAX_BLOCK_TEXTURE == 0)
	{
		m_renderer.lock()->MakeBlock("Block0");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH0);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH0);

	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 1)
	{
		m_renderer.lock()->MakeBlock("Block1");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH1);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH1);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 2)
	{
		m_renderer.lock()->MakeBlock("Block2");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH2);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH2);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 3)
	{
		m_renderer.lock()->MakeBlock("Block3");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH3);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH3);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 4)
	{
		m_renderer.lock()->MakeBlock("Block4");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH4);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH4);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 5)
	{
		m_renderer.lock()->MakeBlock("Block5");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH5);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH5);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 6)
	{
		m_renderer.lock()->MakeBlock("Block6");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH6);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH6);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 7)
	{
		m_renderer.lock()->MakeBlock("Block7");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH7);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH7);
	}
	else if (nTexNo % MAX_BLOCK_TEXTURE == 8)
	{
		m_renderer.lock()->MakeBlock("Block8");
		m_renderer.lock()->SetDiffuseTexture(TEXTURE_PATH8);
		//m_renderer.lock()->SetNormalTexture(NORMAL_TEXTURE_PATH8);
	}

	// 環境
	//m_renderer.lock()->SetAmbientTexture(AMBIENT_TEXTURE_PATH);
	// ノーマル
	//m_renderer.lock()->SetNormalTexture("data/texture/normal/Plate_6.png");

}