//==================================================================
//									CRigidbody2D.h
//	リジッドボディ
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/12	リジッドボディクラス作成
//
//===================================================================

//====== インクルード部 ======
#include "CRigidbody2D.h"
#include "CObject.h"
#include "CCollision2D.h"

//===== マクロ定義 =====
#define GRAVITY (-9.8f / 60.0f)
#define DRAG (1.0f)

#define MAX_VELOCITY (48)

//===== クラス定義 =====



//===== プロトタイプ宣言 =====



//========================================
//
//	コンストラクタ
//
//========================================
CRigidbody2D::CRigidbody2D()
{
	m_fMass = 1.0f;

	m_force = Vector3{ 0, 0 ,0 };
	m_torque = Vector3{ 0, 0 ,0 };
	m_velocity = Vector3{ 0, 0, 0 };
	m_angularVelocity = Vector3{ 0, 0, 0 };
	m_drag = Vector3{ DRAG, 0.0f, DRAG };
	m_torqueDrag = Vector3{ 0, 0, 0 };

	m_bUseGravity = true;
	m_bSolid = false;
	m_enHitDir = DIRECTION::MAX_DIRECTION;
	m_dir = Vector3{ 0, 0, 0 };
	m_fGraviyForce = GRAVITY;
	m_bTrigger = false;
	m_pTrans = nullptr;
	m_bMoveLimit = true;
}


//========================================
//
//	デストラクタ
//
//========================================
CRigidbody2D::~CRigidbody2D()
{

}

//========================================
//
//	更新
//
//========================================
void CRigidbody2D::Start()
{
	m_pTrans = &*m_pParent->m_pTrans;

	// コリジョンがあったら
	const auto& col = m_pParent->GetComponent<CCollision2D>();
	if (col)
	{
		col->SetRb(m_pParent->GetComponent<CRigidbody2D>());
		m_col = col;
	}
}

//========================================
//
//	更新
//
//========================================
void CRigidbody2D::Update()
{

	CRigidbody2D *pRb;
	CTransform *pTrans;
	Vector3 pos;
	Vector3 rot;
	// リジッドボディ取得
	pRb = this;
	// オブジェクトの取得
	pTrans = &(*m_pParent->m_pTrans);

	// カメラ外ならはじく


	// ストップがオンならとばす
	//if (pRb->m_bStop) return;

	//if (pRb->m_bSolid) return;

	//===== 回転 =====

	// 移動量
	pRb->m_angularVelocity->x = 0;
	pRb->m_angularVelocity->y = 0;
	pRb->m_angularVelocity->z = 0;

	//// 外力
	pRb->m_angularVelocity->x += pRb->m_torque->x;
	pRb->m_angularVelocity->y += pRb->m_torque->y;
	pRb->m_angularVelocity->z += pRb->m_torque->z;

	// ここで速度から向きを計算
	//pRb->m_dir = Normalize(pRb->m_angularVelocity);

	// 位置の更新
	rot = pTrans->GetRotation();
	rot->x += pRb->m_angularVelocity->x;
	rot->y += pRb->m_angularVelocity->y;
	rot->z += pRb->m_angularVelocity->z;
	pTrans->SetRotation(rot);

	// 抵抗で加速度を減らす	fDrag(0.0f ～ 1.0f)
	pRb->m_torque->x *= (1.0f - pRb->m_torqueDrag->x);
	if (fabsf(pRb->m_torque->x) < 0.01f) pRb->m_torque->x = 0.0f;
	pRb->m_torque->y *= (1.0f - pRb->m_torqueDrag->y);
	if (fabsf(pRb->m_torque->y) < 0.01f) pRb->m_torque->y = 0.0f;
	pRb->m_torque->z *= (1.0f - pRb->m_torqueDrag->z);
	if (fabsf(pRb->m_torque->z) < 0.01f) pRb->m_torque->z = 0.0f;


	//===== 移動 =====

	// 移動量
	pRb->m_velocity->x = 0;
	pRb->m_velocity->y = 0;
	pRb->m_velocity->z = 0;

	// 重力
	if (pRb->m_bUseGravity)
	{
		pRb->m_force->y += pRb->m_fGraviyForce;
	}
	// 重力をオン
	if(!pRb->m_bSolid)
		pRb->m_bUseGravity = true;

	//// 外力	外力はAddForceに
	pRb->m_velocity->x += pRb->m_force->x;
	pRb->m_velocity->y += pRb->m_force->y;
	pRb->m_velocity->z += pRb->m_force->z;

	// ここで速度から向きを計算
	//pRb->m_dir = Normalize(pRb->m_velocity);

	// 速度限界
	if (pRb->m_velocity->x > MAX_VELOCITY) pRb->m_velocity->x = MAX_VELOCITY;
	if (pRb->m_velocity->y > MAX_VELOCITY) pRb->m_velocity->y = MAX_VELOCITY;
	if (pRb->m_velocity->z > MAX_VELOCITY) pRb->m_velocity->z = MAX_VELOCITY;
	if (pRb->m_velocity->x < -MAX_VELOCITY) pRb->m_velocity->x = -MAX_VELOCITY;
	if (pRb->m_velocity->y < -MAX_VELOCITY) pRb->m_velocity->y = -MAX_VELOCITY;
	if (pRb->m_velocity->z < -MAX_VELOCITY) pRb->m_velocity->z = -MAX_VELOCITY;

	// 位置の更新
	pos = pTrans->GetPos();
	pos->x += pRb->m_velocity->x;
	pos->y += pRb->m_velocity->y;
	pos->z += pRb->m_velocity->z;
	// 移動限界
	if (m_bMoveLimit)
	{
		if (pos->x < 0.0f) pos->x = 0.0f;
		if (pos->y < 0.0f) pos->y = 0.0f;
	}
	pTrans->SetPos(pos);

	// 抵抗で加速度を減らす	fDrag(0.0f ～ 1.0f)
	pRb->m_force->x *= (1.0f - pRb->m_drag->x);
	if (fabsf(pRb->m_force->x) < 0.01f) pRb->m_force->x = 0.0f;
	//pRb->m_force->y *= (1.0f - pRb->m_fDrag);
	if (fabsf(pRb->m_force->y) < 0.01f) pRb->m_force->y = 0.0f;
	pRb->m_force->z *= (1.0f - pRb->m_drag->z);
	if (fabsf(pRb->m_force->z) < 0.01f) pRb->m_force->z = 0.0f;

}

//========================================
//
//	押し出し処理
//
//========================================
void CRigidbody2D::Extrusion(const std::shared_ptr<CRigidbody2D>& pOther)
{
	// ソリッドなら
	if (m_bSolid) return;

	// 最短
	float fMinLenX;
	float fMinLenY;
	float fLenX;
	float fLenY;
	// 衝突後の力
	float fForceX;
	float fForceY;
	// 座標
	Vector3 main_pos = m_pTrans->GetPos() + m_col.lock()->GetOffSet();
	Vector3 sub_pos = pOther->m_pTrans->GetPos() + pOther->m_col.lock()->GetOffSet();
	// スケール
	Vector3 main_scale = m_col.lock()->GetScale();
	Vector3 sub_scale = pOther->m_col.lock()->GetScale();
	// オフセット
	Vector3 main_off = m_col.lock()->GetOffSet();
	Vector3 sub_off = pOther->m_col.lock()->GetOffSet();

	// 向きの初期化
	m_enHitDir = DIRECTION::MAX_DIRECTION;

	//--- 最短位置の計算 ---
	// 左
	fLenX = sub_pos->x - sub_scale->x / 2 - main_pos->x - main_scale->x / 2;
	fMinLenX = fLenX;

	// 右
	fLenX = sub_pos->x + sub_scale->x / 2 - main_pos->x + main_scale->x / 2;
	if (fabsf(fLenX) < fabsf(fMinLenX))
	{
		fMinLenX = fLenX;
	}

	// 上
	fLenY = sub_pos->y - sub_scale->y / 2 - main_pos->y - main_scale->y / 2;
	fMinLenY = fLenY;

	// 下
	fLenY = sub_pos->y + sub_scale->y / 2 - main_pos->y + main_scale->y / 2;
	if (fabsf(fLenY) < fabsf(fMinLenY))
	{
		fMinLenY = fLenY;
	}

	// Ｘ，Ｙの最短比較
	if (fabsf(fMinLenX) < fabsf(fMinLenY))	// ここでぶつかった方向も分かる!!
	{	// Ｘ側

		// 方向
		if (fMinLenX < 0)
		{
			// 左
			m_enHitDir = DIRECTION::LEFT;
			pOther->m_enHitDir = DIRECTION::RIGHT;
		}
		else
		{
			// 右
			m_enHitDir = DIRECTION::RIGHT;
			pOther->m_enHitDir = DIRECTION::LEFT;
		}

		// トリガーなら押し出ししない
		if (m_bTrigger) return;
		if (pOther->m_bTrigger) return;
		
		// お互いソリッドじゃない
		if (!pOther->m_bSolid)
		{
			m_velocity->x += fMinLenX;
			// 当たった時の力積を求める
			fForceX = ((m_fMass - pOther->m_fMass) * m_velocity->x + pOther->m_fMass * pOther->m_velocity->x) / (m_fMass + pOther->m_fMass);
			m_force->x = -fForceX;

			m_velocity->x += m_force->x;
			m_pTrans->SetPos(Vector3{ main_pos->x + m_velocity->x - main_off->x, main_pos->y - main_off->y, main_pos->z });
			return;
		}

		// 相手側ソリッドなら
		if (fMinLenX > 0)
		{
			// 右から
			// 押し出し
			m_pTrans->SetPos(Vector3{ sub_pos->x + sub_scale->x / 2 + main_scale->x / 2 - main_off->x, 
				main_pos->y - main_off->y, main_pos->z });
		}
		else if(fMinLenX < 0)
		{
			// 左から
			// 押し出し
			m_pTrans->SetPos(Vector3{ sub_pos->x - sub_scale->x / 2 - main_scale->x / 2 - main_off->x,
				main_pos->y - main_off->y, main_pos->z });
		}
	}
	else
	{	// Ｙ側
		
		// 上からぶつかった時はデフォルトで重力を消す
		if (fMinLenY < 0)
		{
			m_enHitDir = DIRECTION::UP;
			pOther->m_enHitDir = DIRECTION::DOWN;
			// トリガーなら押し出ししない
			if (m_bTrigger) return;
			if (pOther->m_bTrigger) return;
			// 上
			m_bUseGravity = false;
			if (m_force->y > 0)
				m_force->y = 0;
		}
		else
		{
			m_enHitDir = DIRECTION::DOWN;
			pOther->m_enHitDir = DIRECTION::UP;
			// トリガーなら押し出ししない
			if (m_bTrigger) return;
			if (pOther->m_bTrigger) return;
			// 下
			if (m_force->y < 0)
				m_force->y = 0;
		}

		// お互いソリッドじゃない
		if (!pOther->m_bSolid)
		{
			m_velocity->y += fMinLenY;
			// 当たった時の力積を求める
			fForceY = ((m_fMass - pOther->m_fMass) * m_velocity->y + pOther->m_fMass * pOther->m_velocity->y) / (m_fMass + pOther->m_fMass);
			m_force->y = -fForceY;
			
			m_velocity->y += m_force->y;
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x, main_pos->y + m_velocity->y - main_off->y, main_pos->z });
			return;
		}

		// 相手側ソリッドなら
		if (fMinLenY > 0)
		{
			// 下から
			// 押し出し
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x,
				sub_pos->y + sub_scale->y / 2 + main_scale->y / 2 - main_off->y, main_pos->z });
		}
		else if(fMinLenY < 0)
		{
			// 上から
			// 押し出し
			m_pTrans->SetPos(Vector3{ main_pos->x - main_off->x,
				sub_pos->y - sub_scale->y / 2 - main_scale->y / 2 - main_off->y, main_pos->z });
		}
	}
}

//========================================
//
//	押し出し処理 円と円
//
//========================================
void CRigidbody2D::ExtrusionCircleToCircle(CRigidbody2D *pOther)
{
//	float TotalWeight = m_fMass + pOther->m_fMass; // 質量の合計
//	float RefRate = (1.0f);// + res_A * res_B); // 反発率
//
//	Vector3 C = pOther->GetObj()->GetPos(); // 衝突軸ベクトル
//	C->x -= main_pos->x;
//	C->y -= main_pos->y;
//	C->z -= main_pos->z;
//	// 正規化
////	C = Normalize(C);
//
//	Vector3 velowork = m_velocity;
//	velowork->x -= pOther->m_velocity->x;
//	velowork->y -= pOther->m_velocity->y;
//	velowork->z -= pOther->m_velocity->z;
////	float fDot = Dot(velowork, C); // 内積算出
//
//	Vector3 ConstVec;
////	ConstVec->x = RefRate * fDot / TotalWeight * C->x; // 定数ベクトル
////	ConstVec->y = RefRate * fDot / TotalWeight * C->y; // 定数ベクトル
////	ConstVec->z = RefRate * fDot / TotalWeight * C->z; // 定数ベクトル
//
//	
//	// 衝突後速度ベクトルの算出
//	Vector3 veloA;
//	Vector3 veloB;
//
//	veloA->x = -pOther->m_fMass * ConstVec->x + m_velocity->x;
//	veloA->y = -pOther->m_fMass * ConstVec->y + m_velocity->y;
//	veloA->z = -pOther->m_fMass * ConstVec->z + m_velocity->z;
//
//	veloB->x = m_fMass * ConstVec->x + pOther->m_velocity->x;
//	veloB->y = m_fMass * ConstVec->y + pOther->m_velocity->y;
//	veloB->z = m_fMass * ConstVec->z + pOther->m_velocity->z;
//
//	m_velocity			= veloA;
//	pOther->m_velocity	= veloB;
//
//	
//	// 衝突後位置の算出
//	Vector3 posA = main_pos;
//	Vector3 posB = pOther->GetObj()->GetPos();
//
//	posA->x += veloA->x;
//	posA->y += veloA->y;
//	posA->z += veloA->z;
//
//	posB->x += veloB->x;
//	posB->y += veloB->y;
//	posB->z += veloB->z;
//
//	m_pTrans->SetPos(posA);
//	pOther->GetObj()->SetPos(posB);
//
//	return;
}
