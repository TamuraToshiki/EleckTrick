//======================================================================
//											CProductEnergy.h
//	発電・消費
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/11/20	作り直し
//	2020/12/10  ソース整理・バグ取り
//	2020/12/11	10日に引き続きバグ取り
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "input.h"
#include "CResourceEnergy.h"
#include "Renderer.h"
#include "CSpriteObject.h"
#include "CRigidbody2D.h"

//===== クラス定義 =====
class CProductEnergy : public CComponent
{
private:
	// --- 変数宣言 ---
	float m_fAdd;					//加算量
	float m_fSub;					//減算量
	bool m_bTrigger_Z;				//ボタンフラグ＿Z
	bool m_bTrigger_C;				//ボタンフラグ＿C
	std::weak_ptr<CResourceEnergy> m_resource;

	// モーターアニメーション
	std::weak_ptr<CAssimpRenderer> m_generater;
	// プロペラ
	std::weak_ptr<CRigidbody2D> m_propeller;

	// 回転数
	float m_fRotSpeed;
	float m_fMaxRotSpeed;
	float m_fAddRotSpeed;
	float m_fSubRotSpeed;
	int m_nEffectCount;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	//-- 終了処理 --//
	void End() override;

	//--------------------------------------------------
	//	加算量セット
	//--------------------------------------------------
	void SetAdd(float fadd = 1.0f);

	//--------------------------------------------------
	//	減算量セット
	//--------------------------------------------------
	void SetSub(float fsub = 0.01f);

	//--------------------------------------------------
	//	発電	
	//--------------------------------------------------
	bool GenerateEnergyRight(void);

	bool GenerateEnergyLeft(void);


	//--------------------------------------------------
	//	消費
	//--------------------------------------------------
	void ExpenseEnergy(void);
};



