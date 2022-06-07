//======================================================================
//											CPlayerInput.cpp
//	プレイヤー移動
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/16	作成 宇佐美編集
//
//======================================================================


//===== インクルード部 =====
#include "CPlayerInput.h"
#include "input.h"
#include "Camera.h"
#include "Light.h"
#include "Sound.h"
#include "Renderer.h"


//===== マクロ定義 =====
#define MOVE_SE_FRAME (5)

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CPlayerInput::Start()
{
	m_playerAction = m_pParent->GetComponent<CPlayerActionComponent>();
	m_shot = m_pParent->GetComponent<CEnergyShot>();
	m_product = m_pParent->GetComponent<CProductEnergy>();

	// レンダラー取得
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();

	// カメラターゲットのセット
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

	// アニメーション
	m_renderer.lock()->SetAnimIndex(eRun);
	m_renderer.lock()->PlayAnim();

	// コントローラーの数
	m_nJoyCount = GetJoyCount();
}


//===================================
//
//	更新関数
//
//===================================
void CPlayerInput::Update()
{
	if (m_playerAction.expired()) return;

	// 向きの初期化
	m_playerAction.lock()->ResetDir();

	// 移動
	float JoyX = (float)GetJoyX(JOYSTICKID1);
	float JoyY = (float)GetJoyY(JOYSTICKID1);

	Vector3 dir = { JoyX, -JoyY, 0 };
	dir /= 32767.0f;
	if (m_nJoyCount <= 0)
	{
		dir = Vector3{ 0,0,0 };

	}

	float speed = 1.0f;
	// 停止
	if (GetJoyButton(JOYSTICKID1, JOYBUTTON_LB) ||
		GetJoyButton(JOYSTICKID1, JOYBUTTON_RB))
	{
		speed = 0.0f;
	}
	
	if (GetKeyPress(VK_RIGHT))
	{
		m_playerAction.lock()->RightMove();
	}
	else if (GetKeyPress(VK_LEFT))
	{
		m_playerAction.lock()->LeftMove();
	}
	else if (dir->x > 0.3f)
	{
		m_playerAction.lock()->RightMove(dir->x * speed);
	}
	else if (dir->x < -0.3f)
	{
		m_playerAction.lock()->LeftMove(dir->x * speed);
	}
	else
	{
		m_playerAction.lock()->StopSE();
	}

	// 弾の向き
	if (m_nJoyCount > 0 && dir.magnitude() > 0.3f)
	{
		m_playerAction.lock()->SetDir(dir);
	}

	if (GetKeyPress(VK_UP))
	{
		m_playerAction.lock()->UpDir();
	}
	if (GetKeyPress(VK_DOWN))
	{
		m_playerAction.lock()->DownDir();
	}

	// 向きの修正
	m_playerAction.lock()->CheckDir();

	// ジャンプ
	if (GetKeyTrigger(VK_SPACE) ||
		GetJoyRelease(JOYSTICKID1, JOYBUTTON_A))
	{
		if (m_playerAction.lock()->Jump())
		{
			// アニメーション
			m_renderer.lock()->SetAnimIndex(eJump);
		}
	}
	m_playerAction.lock()->SetJumpFlg(false);

	// ショット
	if (GetKeyTrigger(VK_X)
		|| GetJoyRelease(JOYSTICKID1, JOYBUTTON_X))
	{
		if (m_shot.lock()->CreateShot())
		{
			// アニメーション
			m_renderer.lock()->SetAnimIndex(eAttack);
		}
	}

	// チャージ
	float JoyZ = (float)GetJoyZ(JOYSTICKID1);
	JoyZ /= 32767.0f;
	if (m_nJoyCount <= 0) JoyZ = 0.0f;

	if (GetKeyTrigger(VK_C) || JoyZ > 0.3f)
	{
		if (m_product.lock()->GenerateEnergyRight())
		{
			// アニメーション
			//m_renderer.lock()->SetAnimIndex(eGeneration);
		}
	}
	else if (GetKeyTrigger(VK_Z) || JoyZ < -0.3f)
	{
		if (m_product.lock()->GenerateEnergyLeft())
		{
			// アニメーション
			//m_renderer.lock()->SetAnimIndex(eGeneration);
		}
	}
	else
	{
		m_product.lock()->ExpenseEnergy();
	}
}


//===================================
//
//	後更新関数
//
//===================================
void CPlayerInput::LateUpdate()
{
	//CLight::GetMainLight()->SetTargetPos(m_pParent->m_pTrans->m_pos.GetFloat3());
	// カメラターゲットのセット
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);

}


//===================================
//
//	終了関数
//
//===================================
void CPlayerInput::End()
{

}


//===================================
//
//	2D当たり判定(Enter) コールバック関数
//
//===================================
void CPlayerInput::OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d)
{

}

//============================================
//
//	3Dアニメーション終了時コールバック関数
//
//============================================
void CPlayerInput::OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& renderer)
{
	// アニメーション
	if (renderer->GetCurrentAnimIndex() == eJump)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eGeneration)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eAttack)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
	else if (renderer->GetCurrentAnimIndex() == eRun)
	{
		m_renderer.lock()->SetAnimIndex(eRun);
	}
}