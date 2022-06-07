//======================================================================
//												CLift.h
//	リフト
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/16	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CAssimpObject.h"
#include "CRigidbody2D.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CLift : public CAssimpObject
{
public:
	CLift();
	~CLift();
protected:

private:
	//void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;
	
}; 


//===== プロトタイプ宣言 =====


#pragma once
