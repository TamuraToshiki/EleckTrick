//======================================================================
//											CGearCaterpillarComponent.h
//	ギア・キャタピラ同期コンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2021/01/20	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"


//===== クラス定義 =====
class CGearCaterpillarComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	std::weak_ptr<CTransform> m_player;


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

	// プレイヤーのセット
	void SetPlayer(std::weak_ptr<CTransform> player) { m_player = player; }
};



