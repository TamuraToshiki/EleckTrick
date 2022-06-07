//======================================================================
//											CPlayerActionComponent.cpp
//	プレイヤーのアクションコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/16	作成
//
//======================================================================


//===== インクルード部 =====
#include "CPlayerActionComponent.h"
#include "Sound.h"
#include "CCollision2D.h"
#include "CObjectManager.h"
#include "CAssimpObject.h"
#include "CGearCaterpillarComponent.h"


//===== マクロ定義 =====
#define MOVE_SE_FRAME (5)


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CPlayerActionComponent::Start()
{
	m_speed = 4.0f;
	m_jump = 7.5f;
	m_right = true;
	m_bjunpFlag = true;
	m_nMoveSECnt = 0;

	m_rb = m_pParent->GetComponent<CRigidbody2D>();


	// キャタピラの生成
	const auto& cater = CObjectManager::CreateObject<CAssimpObject>();
	cater->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_caterpollar = cater->GetComponent<CAssimpRenderer>();
	m_caterpollar.lock()->ModelLoad("data/model/MainAnim_Blender/Caterpillar_Anim.fbx");
	m_caterpollar.lock()->SetDiffuseTexture("data/model/Model_Main/texture_10.png");
	m_caterpollar.lock()->SetAnimIndex(0);
	m_caterpollar.lock()->SetAnimSpeed(2);
	m_caterpollar.lock()->PlayAnim();

	// ギアの生成
	const auto& gear = CObjectManager::CreateObject<CAssimpObject>();
	gear->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_gear = gear->GetComponent<CAssimpRenderer>();
	m_gear.lock()->ModelLoad("data/model/MainAnim_Blender/Gear_Anim_Blender.fbx");
	m_gear.lock()->SetDiffuseTexture("data/model/Model_Main/texture_10.png");
	m_gear.lock()->SetAnimIndex(1);
	m_gear.lock()->SetAnimSpeed(2);
	m_gear.lock()->PlayAnim();

}


//===================================
//
//	更新関数
//
//===================================
void CPlayerActionComponent::Update()
{
	// 座標同期
	m_caterpollar.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
	m_gear.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
}


//===================================
//
//	後更新関数
//
//===================================
void CPlayerActionComponent::LateUpdate()
{
	if (m_nMoveSECnt >= 0)
	{
		m_nMoveSECnt--;
	}

	// 座標同期
	m_caterpollar.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;
	m_gear.lock()->m_pParent->m_pTrans->m_pos = m_pParent->m_pTrans->m_pos;


	// 速度
	Vector3 velo = m_rb.lock()->GetVelocity();

	// アニメーション
	float animSpeed = velo->x + velo->z;
	animSpeed = fabsf(animSpeed);

	m_caterpollar.lock()->SetAnimSpeed(animSpeed);
	m_gear.lock()->SetAnimSpeed(animSpeed);
}


//===================================
//
//	終了関数
//
//===================================
void CPlayerActionComponent::End()
{

}


//===== 使わない時は消してください() =====
//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CPlayerActionComponent::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{
	if (collsion2d->m_pParent->GetTag() != "Bullet")
	{
		const auto& rb = collsion2d->m_pParent->GetComponent<CRigidbody2D>();
		if (rb)
		{
			if (rb->GetHitDir() == DIRECTION::UP)
			{
				m_bjunpFlag = true;
			}
		}
	}
}


// 右移動
void CPlayerActionComponent::RightMove(float fSpeed)
{
	m_rb.lock()->AddForceX(m_speed * fSpeed);
	m_dir->x = 1.0f;
	m_right = true;
	m_pParent->m_pTrans->m_rot->y = -90;

	if (m_nMoveSECnt < 0)
	{
		// サウンド
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// 左移動
void CPlayerActionComponent::LeftMove(float fSpeed)
{
	m_rb.lock()->AddForceX(m_speed * fSpeed);
	m_dir->x = -1.0f;
	m_right = false;
	m_pParent->m_pTrans->m_rot->y = 90;

	if (m_nMoveSECnt < 0)
	{
		// サウンド
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// 前移動
void CPlayerActionComponent::FrontMove()
{
	m_rb.lock()->AddForceZ(-m_speed * 0.2f);
	m_pParent->m_pTrans->m_rot->y = 0;

	if (m_nMoveSECnt < 0)
	{
		// サウンド
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// 奥移動
void CPlayerActionComponent::BackMove()
{
	m_rb.lock()->AddForceZ(m_speed * 0.2f);
	m_pParent->m_pTrans->m_rot->y = 180;

	if (m_nMoveSECnt < 0)
	{
		// サウンド
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// 移動ストップ
void CPlayerActionComponent::StopMove()
{
	m_rb.lock()->SetForce({ 0,0,0 });
	StopSE();
}

// サウンド停止
void CPlayerActionComponent::StopSE()
{
	CSound::StopSE("PlayerMove.wav");
	m_nMoveSECnt = 0;
}

// 自由ムーブ
void CPlayerActionComponent::FreeMove(Vector3 force)
{
	m_rb.lock()->AddForce(force);

	if (m_nMoveSECnt < 0)
	{
		// サウンド
		CSound::PlaySE("PlayerMove.wav");
		m_nMoveSECnt = MOVE_SE_FRAME;
	}
}

// ジャンプ
bool CPlayerActionComponent::Jump()
{
	if (!m_bjunpFlag) return false;

	//地面に設置している時のみ
	m_rb.lock()->SetForceY(m_jump);
	m_bjunpFlag = false;
	// サウンド
	CSound::PlaySE("Jump.mp3");

	return true;
}

