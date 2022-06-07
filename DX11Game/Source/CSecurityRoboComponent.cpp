//======================================================================
//											CSecurityRoboComponent.h
//	警備ロボコンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/12/04	作成(優先度を下げたため後回し)
//
//======================================================================

//====== インクルード部 ======
#include "CObject.h"
#include "CObjectManager.h"
#include "CSecurityRoboComponent.h"
#include "CVisible.h"
#include "Sound.h"
#include "CGearCaterpillarComponent.h"

#include "CEffectComponent.h"


//===== マクロ定義 =====


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===== 列挙体宣言 =====

//===== 定数定義 =====
#define EyeScale	Vector3{ 200.0f,50.0f,100.0f }
#define EyeOffset	Vector3{ (EyeScale->x * -1),50.0f,0.0f }
#define U_turn_OFFSET	Vector3{ -70.0f,50.0f,0.0f}


//===================================
//
//	スタート関数
//
//===================================
void CSecurityRoboComponent::Start()
{
	CEnemyBase::Start();

	// 警備ロボに目、スタンド、センサーをセットするプログラム
	m_visi.lock()->SetTargetEnemy(m_pParent->GetComponent<CSecurityRoboComponent>(), EyeScale);
	m_stand.lock()->SetStandStick(m_pParent->GetComponent<CSecurityRoboComponent>());
	m_blsen.lock()->SetBlocksensorComponent(m_pParent->GetComponent<CSecurityRoboComponent>());

	npt_cnt = 0;										// パターンカウント
	m_bDelayStand = true;								// 崖で反転するフラグ
	spd = 2;											// エネミーの速さ
	m_rb = m_pParent->GetComponent<CRigidbody2D>();		// リジットボディセット
	vec = Vector3{ -1.0f,0.0f,0.0f };					// ベクトル
	Eye = EyeOffset;									// 目の方向
	m_U_turn = U_turn_OFFSET;
	m_pParent->m_pTrans->m_rot->y = 90;					// 初期ロボットの向いてる方向
	m_visi.lock()->SetDir(Eye);
	m_blsen.lock()->SetDir(m_U_turn);

	// アニメーション
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();
	m_renderer.lock()->StopAnim();

	m_energyOffTexture = "data/model/EnemyA_Mix/EnemyA_Tex4.png";
	m_energyOnTexture  = "data/model/EnemyA_Mix/EnemyA_Tex4.png";

	// 初期座標
	m_initPos = m_pParent->m_pTrans->m_pos;


	//　ギア、キャタピラ
	// キャタピラの生成
	const auto& cater = CObjectManager::CreateObject<CAssimpObject>();
	cater->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_caterpollar = cater->GetComponent<CAssimpRenderer>();
	m_caterpollar.lock()->ModelLoad("data/model/Enemy_Anim_Blender/Enemy_Caterpillar_Anim.fbx");
	m_caterpollar.lock()->SetDiffuseTexture("data/model/EnemyA_Mix/EnemyA_Tex4.png");
	m_caterpollar.lock()->SetAnimIndex(0);
	m_caterpollar.lock()->SetAnimSpeed(2);
	m_caterpollar.lock()->PlayAnim();

	// ギアの生成
	const auto& gear = CObjectManager::CreateObject<CAssimpObject>();
	gear->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_gear = gear->GetComponent<CAssimpRenderer>();
	m_gear.lock()->ModelLoad("data/model/Enemy_Anim_Blender/Gear_Anim_Blender.fbx");
	m_gear.lock()->SetAnimIndex(1);
	m_gear.lock()->SetAnimSpeed(2);
	m_gear.lock()->PlayAnim();
}

//===================================
//
//	リスタート
//
//===================================
void CSecurityRoboComponent::Restart()
{
	spd = 2;
	vec = Vector3{ -1.0f,0.0f,0.0f };					// ベクトル

	m_pParent->m_pTrans->m_pos = m_initPos;

}

//===================================
//
//	後更新
//
//===================================
void CSecurityRoboComponent::LateUpdate()
{
	// アニメーション更新処理

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
//	終了
//
//===================================
void CSecurityRoboComponent::End()
{
	const auto& cata = m_caterpollar.lock();
	if (cata)
		CObjectManager::DestroyObject(cata->m_pParent);

	const auto& gaer = m_gear.lock();
	if (gaer)
		CObjectManager::DestroyObject(gaer->m_pParent);

	CEnemyBase::End();
}

//===================================
//
//	交戦関数
//
//===================================
void CSecurityRoboComponent::EngagementEnemy()
{

	if (++npt_cnt == 1)// 1f　発見したときに
	{
		m_rb.lock()->SetForce({ 0.0f,2.0f,0.0f });	// 飛び上がる
		// サウンド
		CSound::PlaySE("EnemyDiscovery.wav");
	}
	if (npt_cnt >= 30)// 0.5秒後に追いかける
	{
		vec = m_player.lock()->m_pTrans->GetPos() - m_pParent->m_pTrans->GetPos();	// 距離 = プレイヤー - 自身 
		vec = Vector3::Normalize(vec);	// 距離を正規化
		m_rb.lock()->SetForceX((spd * 2) * vec->x);	// X軸に速さ * 2 で追いかける
		// アニメーション
		m_renderer.lock()->PlayAnim();
	}
}

//===================================
//
//	通常移動関数
//
//===================================
void CSecurityRoboComponent::NormalEnemy()
{
	if (m_rb.lock()->GetHitDir() == DIRECTION::DOWN)	// 地面に接してるなら
	{
		m_rb.lock()->SetForceX(vec->x * spd);	// 方向 * 速さ
		if (m_bStandflg)	// スタンドが地面に接してるなら
		{
			m_bDelayStand = false;	// 反転してないことにする
			npt_cnt = 0;	// パターンを初期化する
			if (m_bBlockflg)	// センサー壁に接してるなら
			{
				m_pParent->m_pTrans->m_rot->y *= -1;	// エネミーの向きを反転する
				vec->x *= -1;	// 移動方向を反転します
				Eye->x *= -1;	// 目の方向を反転します
				m_U_turn->x *= -1;
				m_visi.lock()->SetDir(Eye);	// 反転した目の方向をセットします
				m_blsen.lock()->SetDir(m_U_turn);
			}
		}
		else
		{
			if (!m_bDelayStand)
			{
				m_pParent->m_pTrans->m_rot->y *= -1;// エネミーの向きを反転する
				vec->x *= -1;	// 移動方向を反転します
				Eye->x *= -1;	// 目の方向を反転します
				m_U_turn->x *= -1;
				m_visi.lock()->SetDir(Eye);	// 反転した目の方向をセットします
				m_blsen.lock()->SetDir(m_U_turn);
				m_bDelayStand = true;	// 反転したことにする
			}
		}

		// 警備ロボの後ろ歩き防止プログラム
		if (vec->x > 0 && m_pParent->m_pTrans->m_rot->y > 0)
		{
			vec->x *= -1;
		}
		else if (vec->x < 0 && m_pParent->m_pTrans->m_rot->y < 0)
		{
			vec->x *= -1;
		}
	}

	// アニメーション
	m_renderer.lock()->StopAnim();
}

//===================================
//
//	機能停止関数
//
//===================================
void CSecurityRoboComponent::Outage()
{
	m_rb.lock()->SetForce({ 0,0,0 });
	m_fUseResource = 0.1f;

	// エフェクトの生成
	Vector3 pos = m_pParent->m_pTrans->m_pos + 
		Vector3{(float)(rand() % 100) - 50, (float)(rand() % 100) - 50, 0};

	const auto& obj = CObjectManager::CreateObject<CObject>(pos);
	obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eSmokeEffect);
	const auto& rb = obj->AddComponent<CRigidbody2D>();

	rb->SetGravityForce(0.0f);
	rb->SetForceY(2.0f);

}
