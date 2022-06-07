//======================================================================
//											CRespawnComponent.h
//	プレイヤーリスポーン用コンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/30	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CPlayer.h"
#include "CResourceEnergy.h"
#include "CPlayerInput.h"
#include "CEditorComponent.h"

#include "debugproc.h"


//===== クラス定義 =====
class CRespawnComponent : public CComponent
{
private:
	// --- 変数宣言 ---
	//CSceneTamura InitScene;
	Vector3	  RespawnPoint;				// リスポーンポイント			
	//std::weak_ptr<CPlayerInput> m_pi;	// 初期化するために取得
	//std::weak_ptr<CResourceEnergy> m_pr;// 初期化するために取得
	//std::weak_ptr<CRigidbody2D> m_rb;
	std::weak_ptr<CEditorComponent> m_edit;

	bool m_bNowFade;
public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// リスポーンポイントを変更する関数(引数:リスポーンポイント)
	void SetRespawnPoint(Vector3 pos);

	void Respawn();
};




