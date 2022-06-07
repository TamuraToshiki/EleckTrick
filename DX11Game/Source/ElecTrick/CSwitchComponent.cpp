//======================================================================
//											CSwitchComponent.h
//	ギミックベースレイアウト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/01	
//
//======================================================================

//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CCollision2D.h"
#include "CSwitchComponent.h"

#include "Sound.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CSwitchComponent::Start()
{
	bSwitchflg = false;
	m_fUseResource = 0.1f;

	m_energyOffTexture = "data/model/Electrode/Electrode.png";
	m_energyOnTexture = "data/model/Electrode/Electrode_Change.png";
}

void CSwitchComponent::LateUpdate()
{
	CElecGimmickBase::LateUpdate();

	//カラー変更
	if (bSwitchflg)
	{
		//m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1.0f, 1.0f, 0.0f, 1.0f });
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
	}
	else
	{
		//m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());
	}
}

//===================================
//
//	帯電侵入関数
//
//===================================
void CSwitchComponent::ChargePlayerEnter(std::weak_ptr<CCollision2D> collsion2d)
{
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	帯電滞在関数
//
//===================================
void CSwitchComponent::ChargePlayerStay(std::weak_ptr<CCollision2D> collsion2d)
{
	// それぞれのギミックの処理
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	帯電撤退関数
//
//===================================
void CSwitchComponent::ChargePlayerExit(std::weak_ptr<CCollision2D> collsion2d)
{

}

//===================================
//
//	電気弾稼働関数
//
//===================================
void CSwitchComponent::EnergyOn()
{
	if (!bSwitchflg)
	{
		bSwitchflg = true;
		m_fUseResource = 0;
		CSound::PlaySE("SwitchOn.mp3");
	}
}

//===================================
//
//	電気弾休止関数
//
//===================================
void CSwitchComponent::EnergyOff()
{

}

bool CSwitchComponent::GetSwitchflg()
{
	return bSwitchflg;
}
