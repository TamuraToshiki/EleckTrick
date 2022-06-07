//======================================================================
//											CDoorComponent.cpp
//	コンポーネント作成用のベースレイアウト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CDoorComponent.h"
#include "Camera.h"

#include "Sound.h"

//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CDoorComponent::Start()
{
	MaxSwitch = 0;
	nMaxPoint = 0;
	CurrentRoot = 0;
	m_nDoorType = 0;
	fDoorSpd = 1.5f;
	vec = Vector3{ 0,0,0 };
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_bIsMove = false;

	// ドアの角度から移動位置を取得
	float m_angle = m_pParent->m_pTrans->m_rot->z;
	Vector3 pos = m_pParent->m_pTrans->m_pos;

	if (m_angle == 0)		pos->y += 200;
	else if(m_angle ==  90)	pos->x -= 200;
	else if(m_angle == 180)	pos->y -= 200;
	else if(m_angle == 270)	pos->x += 200;

	Doortype(ALLCHECK, pos);

	// プレイヤーの取得
	m_player = CObjectManager::SearchObjectTag("Player");

	m_energyOffTexture = "data/model/Door/Door.png";
	m_energyOnTexture = "data/model/Door/Door_Change.png";

	// 初期座標
	m_initPos = m_pParent->m_pTrans->m_pos;
}

//===================================
//
//	リスタート
//
//===================================
void CDoorComponent::Restart()
{
	m_bIsMove = false;
	CurrentRoot = 0;
	m_nDoorType = ALLCHECK;
	m_pParent->m_pTrans->m_pos = m_initPos;
	// テクスチャの変更
	m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());

	// スイッチ
	for (const auto& switch_w : aSwitchCom)
	{
		const auto& switch_s = switch_w.lock();
		if (switch_s)
		{
			switch_s->SetSwitchFlg(false);
			switch_s->ResetResource();
		}
	}
}

//===================================
//
//	更新関数
//
//===================================
void CDoorComponent::Update()
{

}


//===================================
//
//	後更新関数
//
//===================================
void CDoorComponent::LateUpdate()
{
	// ドア動作
	switch (m_nDoorType)
	{
	case ALLCHECK:
		if (AllCheckSwitch())
		{
			// カメラ同期
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
			// プレイヤー停止
			if (!m_player.expired())
			{
				m_player.lock()->SetStop(true);
			}

			if (!m_bIsMove)
			{
				m_bIsMove = true;
				// 移動音
				CSound::PlaySE("DoorMove.wav");
				// テクスチャの変更
				m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
			}

			// ベクトル生成
			vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();

			// 移動終了時
			if (Vector3::Length(vec) <= 1.0f)
			{
				// 移動量をゼロ
				vec = Vector3{ 0,0,0 };
				// 移動完了状態へ
				m_nDoorType = MOVE_END;
				// 到着SE
				CSound::PlaySE("DoorStop.wav");
				// 移動音停止
				CSound::StopSE("DoorMove.wav");


				// プレイヤー
				if (!m_player.expired())
				{
					// プレイヤーの提出解除
					m_player.lock()->SetStop(false);
					// カメラ同期終了 プレイヤーに戻す
					CCamera::GetMainCamera()->SetCameraTarget(m_player.lock()->m_pTrans);
				}
			}

			vec = Vector3::Normalize(vec);

			m_rb.lock()->SetForce(vec * fDoorSpd);
		}
		break;

	case PIECESCHECK:
		if (PiecesCheckSwitch())
		{
			vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();
			if (Vector3::Length(vec) <= 1.0f)
			{
				if (CurrentRoot != nMaxPoint - 1)
				{
					CurrentRoot++;
					vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();
				}
				else
				{
					vec = Vector3{ 0,0,0 };
				}
			}
			vec = Vector3::Normalize(vec);

			m_rb.lock()->SetForce(vec * fDoorSpd);
		}
		break;

		// 移動終了
	case MOVE_END:


		break;

	default:
		break;
	}
}


//===================================
//
//	終了関数
//
//===================================
void CDoorComponent::End()
{
	for (const auto& sw_com : aSwitchCom)
	{
		const auto& sw_s = sw_com.lock()->m_pParent->tish().lock();
		if (sw_s)
		{
			CObjectManager::DestroyObject(sw_s);
		}
	}
	aSwitchCom.clear();
}


void CDoorComponent::CreateSwitch(Vector3 pos)
{
	MaxSwitch++;

	const auto& swi = CObjectManager::CreateObject<CSwitch>(pos);
	aSwitchCom.push_back(swi->GetComponent<CSwitchComponent>());
}

void CDoorComponent::Doortype(int nType, Vector3 pos)
{
	m_nDoorType = nType;
	nMaxPoint++;
	DoorMovePoint.push_back(pos);
}

bool CDoorComponent::AllCheckSwitch()
{
	if (aSwitchCom.size() == 0) return false;

	bool b = true;

	for (const auto& sw : aSwitchCom)
	{
		const auto& s = sw.lock();
		if (s)
		{
			if (!s->GetSwitchflg())
			{
				b = false;
			}
		}
	}

	return b;
}

int CDoorComponent::PiecesCheckSwitch()
{
	int n = 0;

	for (const auto& sw : aSwitchCom)
	{
		const auto& s = sw.lock();
		if (s)
		{
			if (s->GetSwitchflg())
			{
				n++;
			}
		}
	}

	return n;
}

const auto & CDoorComponent::GetSwitch(unsigned int nElement)
{
	if (MaxSwitch > nElement)
		return aSwitchCom[nElement];
	else {
		aSwitchCom.end();
	}
}

std::vector<std::weak_ptr<CSwitchComponent>>& CDoorComponent::GetSwitchContainer()
{
	return aSwitchCom;
}
