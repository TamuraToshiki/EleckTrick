//======================================================================
//											CResourceEnergy.cpp
//	エネルギーリソース
//
//======================================================================
//	author :	AT12A 15 榊原龍我
//======================================================================
//	開発履歴
//	2020/12/11	作成
//				ヘッダの関数などをこちらへ
//======================================================================


//===== インクルード部 =====
#include "CResourceEnergy.h"

//===== マクロ定義 =====

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====

//===================================
//
//	スタート関数
//
//===================================
void CResourceEnergy::Start()
{
	m_fResource = 0.0f;
	m_enLevel = ZERO;
	m_bFlag = false;
	m_bEnergyUse = false;
}

//===================================
//
//	更新関数
//
//===================================
void CResourceEnergy::Update()
{
	AddEnergy();
	SubEnergy();
	CheckUse();		// リソースが使われているのかの判断
	DecideCharged();
	DecideLevel();
}

//===================================
//
//	後更新関数
//
//===================================
void CResourceEnergy::LateUpdate()
{
	CheckStop();
}


//===================================
//
//	終了関数
//
//===================================
void CResourceEnergy::End()
{

}

//--------------------------------------------------
//	エネルギー量を取得
//--------------------------------------------------
float CResourceEnergy::GetResource(void)
{
	DecideLevel();
	return m_fResource;
}

//--------------------------------------------------
//	エネルギーレベルを取得
//--------------------------------------------------
int CResourceEnergy::GetLevel(void)
{
	return m_enLevel;
}

//--------------------------------------------------
//	帯電状態を取得
//--------------------------------------------------
bool CResourceEnergy::GetFlag(void)
{
	return m_bFlag;
}

//--------------------------------------------------
//	エネルギーレベルを判別
//--------------------------------------------------
void CResourceEnergy::DecideLevel(void)
{

	if (m_fResource < 10)	m_enLevel = ZERO;
	else if (m_fResource < 20)	m_enLevel = ONE;
	else if (m_fResource < 30)	m_enLevel = TWO;
	else if (m_fResource <= 30)	m_enLevel = THREE;

	//if (m_fResource < 20)	m_enLevel = ZERO;
	//if (m_fResource > 20)	m_enLevel = ONE;
	//if (m_fResource > 40)	m_enLevel = TWO;
	//if (m_fResource > 60)	m_enLevel = THREE;

}

//--------------------------------------------------
//  エネルギーのセット	
//--------------------------------------------------
void CResourceEnergy::SetEnergy(float setAmount)
{
	m_fResource = setAmount;
}

//--------------------------------------------------
//  エネルギーの加算	
//--------------------------------------------------
void CResourceEnergy::AddEnergy(float addAmount)
{
	m_fResource += addAmount;
	if (m_fResource > m_fMaxResource)
	{
		m_fResource = m_fMaxResource;
	}
}

//--------------------------------------------------
//	エネルギーの減算
//--------------------------------------------------
void CResourceEnergy::SubEnergy(float subAmount)
{
	m_fResource -= subAmount;
	if (m_fResource < m_fMinResource)
	{
		m_fResource = m_fMinResource;
	}

}

//--------------------------------------------------
//	帯電判別
//	判別値は後
//--------------------------------------------------
void CResourceEnergy::DecideCharged(void)
{
	if (m_enLevel > 0)	m_bFlag = true;
	else m_bFlag = false;
}

//--------------------------------------------------------------------//
//	エネルギーの使用判定
//--------------------------------------------------------------------//
void CResourceEnergy::CheckUse(void)
{
	if (!m_bEnergyUse) // エネルギーが使われていないかの判断
	{
		if (m_fResource <= m_nEnergyStop)	// エネルギーリソースがストッパーを下回るようなら
		{
			m_fResource = m_nEnergyStop;	// リソースをストッパーの値にする
		}
	}
}

//--------------------------------------------------------------------//
//	エネルギーのストップ判定
//--------------------------------------------------------------------//
void CResourceEnergy::CheckStop(void)
{
	if (m_fResource >= m_fMaxResource)
		m_nEnergyStop = m_fMaxResource;
	// Updateに入れると、ストッパーが効果を発揮しなくなるためLateUpdateに
	m_nEnergyStop = m_fResource / 10;	// 小数点以下を切り捨てて整数型に代入
	m_nEnergyStop *= 10;				// 10の単位にするため
}