//======================================================================
//											CInputMoveComponent.h
//	入力行動コンポーネントテスト
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "input.h"
#include "CRigidbody2D.h"



//===== クラス定義 =====
class CInputMoveComponent : public CComponent
{
private:
	// 移動速度
	float m_speed;
	float m_jump;
	std::weak_ptr<CRigidbody2D> rb;

public:

	// 作成時に呼ばれます
	void Start() override
	{
		m_speed = 2.0f;
		m_jump = 3.0f;
		rb = m_pParent->GetComponent<CRigidbody2D>();
	}

	// 毎フレーム呼ばれます
	void Update() override
	{
		if (GetKeyPress(VK_RIGHT))
		{
			//m_pParent->m_pTrans->m_pos->x += m_speed;
			rb.lock()->AddForceX(m_speed);
		}
		if (GetKeyPress(VK_LEFT))
		{
			//m_pParent->m_pTrans->m_pos->x -= m_speed;
			rb.lock()->AddForceX(-m_speed);
		}

		if (GetKeyPress(VK_SPACE))
		{
			rb.lock()->SetForceY(m_jump);
		}
	}

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override
	{

	}
};



