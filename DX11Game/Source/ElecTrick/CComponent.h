//==================================================================
//								CComponent.h
//	コンポーネントベースクラス
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/09/23	コンポーネントクラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "uniqueTypeID.h"
#include <memory>
#include "Math.h"

//===== マクロ定義 =====


//===== クラス定義 =====

//===== 前定義 =====
class CObject;
class CCollision2D;
class CAnimator2D;
class CAssimpRenderer;

class CComponent
{
public:
	CComponent() 
	{
		// 初期化
		m_nTypeID = -1;
		m_pParent = nullptr;
	}
	virtual ~CComponent() {}

	// 仮想関数
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void End() {}

	// 親オブジェクト
	void SetParent(CObject* pObjct) { m_pParent = pObjct; }

	// ID
	// IDの登録
	template <class T>
	void SetTypeID()
	{
		m_nTypeID = CUniqueTypeID::Get<T>();
	}
	// IDの取得
	int GetID()
	{
		return m_nTypeID;
	}
	CObject* GetParent() { return m_pParent; }

	//--- コールバック関数
	// 当たり判定コールバック関数
	virtual void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	virtual void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	virtual void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	// 2Dアニメーションコールバック関数
	virtual void OnAnimationEnter2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	virtual void OnAnimationStay2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	virtual void OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	// 3Dアニメーションコールバック関数
	virtual void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& assimp){}
	
	CObject* m_pParent;

protected:

private:
	int m_nTypeID;

};

