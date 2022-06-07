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
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "Renderer.h"


//===== クラス定義 =====
class CBlockComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CMeshRenderer> m_renderer;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	// テクスチャの指定
	void SetTexture(int nTexNo);

};



