//==================================================================
//									animation.h
//	アニメーション
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/07/10	アニメーションクラス作成
//
//===================================================================
#pragma once

//====== インクルード部 ======
#include <stdarg.h>

//===== マクロ定義 =====


//===== クラス定義 =====

// メモ： 汎用型にして
class CAnimation2D
{
friend CAnimator2D;
public:
	CAnimation2D();
	~CAnimation2D();

	// ゲット関数
	std::string GetName() { return m_name; }
	int* GetAnimNoArray() { return m_pAnimNoArray; }
	int GetArraySize() { return m_nArraySize; }
private:
	int* m_pAnimNoArray;
	int m_nArraySize;
	std::string m_name;
};

