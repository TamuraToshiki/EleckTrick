//======================================================================
//											CPlayerActionComponent.h
//	プレイヤーのアクションコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "Renderer.h"


//===== クラス定義 =====
class CPlayerActionComponent : public CComponent
{
private:
	// --- 変数宣言 ---
		// 移動速度
	float m_speed;
	float m_jump;			//ジャンプ速度
	bool m_bjunpFlag;		//ジャンプフラグ
	Vector3 m_dir;			//向き
	std::weak_ptr<CRigidbody2D> m_rb;
	bool m_right;
	int m_nMoveSECnt;

	// キャタピラアニメーション
	std::weak_ptr<CAssimpRenderer> m_caterpollar;
	// ギアアニメーション
	std::weak_ptr<CAssimpRenderer> m_gear;

public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	//-------------------------------------------//
	//	プレイヤーの向き取得
	//-------------------------------------------//
	Vector3 GetPlayerDir(void)
	{
		m_dir = m_dir.normalized();
		return m_dir;
	}

	void ResetDir() {
		m_dir = Vector3{ 0,0,0 };
	}

	void CheckDir()
	{
		if (m_dir->x == 0.0f && m_dir->y == 0.0f)
		{
			if (m_right) m_dir->x = 1.0f;
			else m_dir->x = -1.0f;
		}
	}

	void UpDir()	{ m_dir->y += 1.0f; }
	void DownDir()	{ m_dir->y -= 1.0f; }

	void SetDir(Vector3 dir) { m_dir = dir; }

	void SetJumpFlg(bool bJump) { m_bjunpFlag = bJump; }

	// 移動関数
	// 右移動
	void RightMove(float fSpeed = 1.0f);
	// 左移動
	void LeftMove(float fSpeed = -1.0f);
	// 前移動
	void FrontMove();
	// 奥移動
	void BackMove();

	// 移動ストップ
	void StopMove();
	// 自由ムーブ
	void FreeMove(Vector3 force);

	// ジャンプ
	bool Jump();

	// サウンド停止
	void StopSE();
};



