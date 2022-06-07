//======================================================================
//											CFadeComponent.cpp
//	フェードコンポーネント
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//	2020/12/06	作成
//
//======================================================================


//===== インクルード部 =====
#include "CFadeComponent.h"
#include "CObjectManager.h"
#include "input.h"
#include "Sound.h"

#include "debugproc.h"


//===== マクロ定義 =====
#define SCALE_SPEED_X (0.05f * SCREEN_WIDTH)
#define SCALE_SPEED_Y (0.05f * SCREEN_HEIGHT)

#define MAX_SCALE (2.5f)

//===== 列挙 =====
namespace
{
	enum E_STATE_FADE	// フェードの状態の列挙
	{
		E_STATE_FADE_NODE = 0,
		E_STATE_FADE_OUT,
		E_STATE_FADE_IN,

		E_STATE_FADE_MAX,
	};
}


//===== プロトタイプ宣言 =====


//===== グローバル変数 =====


//===================================
//
//	スタート関数
//
//===================================
void CFadeComponent::Start()
{
	// スケール
	m_fScale = MAX_SCALE;
	// ステート
	m_nState = E_STATE_FADE_NODE;
	// フレーム
	m_nFrame = 60;
	// 関数オブジェクト
	m_fadeEndFunc = nullptr;


	// レンダラー取得
	const auto& render = m_pParent->GetComponent<CSpriteRenderer>();
	m_render = render;
	render->m_pParent->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH * MAX_SCALE, SCREEN_HEIGHT * MAX_SCALE, 1 };

	// ４つの■を生成
	for (int i = 0; i < 4; ++i)
	{
		m_obj[i] = CObjectManager::CreateObject<CSpriteObject>();
		const auto& spr = m_obj[i]->GetComponent<CSpriteRenderer>();

		spr->SetColor(render->GetColor());
		spr->SetAlpha(render->GetAlpha());
		spr->SetLayer(render->GetLayer());
		spr->SetMeshBlendState(render->GetMeshBlendState());
	}

	// 右
	m_obj[0]->m_pTrans->m_pos = Vector3{ SCREEN_CENTER_X * MAX_SCALE, 0, 0 };
	m_obj[0]->m_pTrans->m_scale = Vector3{ 0, SCREEN_HEIGHT, 0 };
	// 左
	m_obj[1]->m_pTrans->m_pos = Vector3{ -SCREEN_CENTER_X * MAX_SCALE, 0, 0 };
	m_obj[1]->m_pTrans->m_scale = Vector3{ 0, SCREEN_HEIGHT, 0 };

	// 上
	m_obj[2]->m_pTrans->m_pos = Vector3{ 0, SCREEN_CENTER_Y * MAX_SCALE, 0 };
	m_obj[2]->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH, 0, 0 };
	// 下
	m_obj[3]->m_pTrans->m_pos = Vector3{ 0, -SCREEN_CENTER_Y * MAX_SCALE, 0 };
	m_obj[3]->m_pTrans->m_scale = Vector3{ SCREEN_WIDTH, 0, 0 };
}


//===================================
//
//	更新関数
//
//===================================
void CFadeComponent::Update()
{
	//if (GetKeyPress(VK_J))
	//{
	//	m_pParent->m_pTrans->m_scale->x -= SCALE_SPEED_X;
	//	m_pParent->m_pTrans->m_scale->y -= SCALE_SPEED_Y;

	//	// リミット
	//	if (m_pParent->m_pTrans->m_scale->x < 0.0f)
	//	{
	//		m_pParent->m_pTrans->m_scale->x = 0.0f;
	//		m_pParent->m_pTrans->m_scale->y = 0.0f;
	//		return;
	//	}

	//	// 右・左
	//	m_obj[0]->m_pTrans->m_scale->x += SCALE_SPEED_X;
	//	m_obj[1]->m_pTrans->m_scale->x += SCALE_SPEED_X;

	//	//上・下
	//	m_obj[2]->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//	m_obj[3]->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//}
	//if (GetKeyPress(VK_L))
	//{
	//	m_pParent->m_pTrans->m_scale->x += SCALE_SPEED_X;
	//	m_pParent->m_pTrans->m_scale->y += SCALE_SPEED_Y;
	//	
	//	// リミット
	//	if (m_pParent->m_pTrans->m_scale->x > SCREEN_WIDTH * 2.5f)
	//	{
	//		m_pParent->m_pTrans->m_scale->x = SCREEN_WIDTH *  2.5f;
	//		m_pParent->m_pTrans->m_scale->y = SCREEN_HEIGHT * 2.5f;
	//		return;
	//	}

	//	// 右・左
	//	m_obj[0]->m_pTrans->m_scale->x -= SCALE_SPEED_X;
	//	m_obj[1]->m_pTrans->m_scale->x -= SCALE_SPEED_X;

	//	//上・下
	//	m_obj[2]->m_pTrans->m_scale->y -= SCALE_SPEED_Y;
	//	m_obj[3]->m_pTrans->m_scale->y -= SCALE_SPEED_Y;
	//}


	//if (GetKeyPress(VK_RETURN))
	//{
	//	StartFadeOut();
	//}

}


//===================================
//
//	後更新関数
//
//===================================
void CFadeComponent::LateUpdate()
{
	// 状態により分岐
	switch (m_nState)
	{
	case(E_STATE_FADE_IN):		// フェードイン
		// α値の減少
		m_fScale -= (MAX_SCALE / m_nFrame);

		// α値が0より小さくなったら終了
		if (m_fScale < 0)
		{
			m_fScale = 0;
			m_nState = E_STATE_FADE_NODE;

			// フェードイン終了後の処理
			m_bNowFade = false;

		}
		else if (m_fScale < MAX_SCALE / 2)
		{
			// フェードイン終了まで画面を止める
			//CSceneManager:: SetStopFlag(false);
		}

		// 各スケールの更新
		// 円
		m_pParent->m_pTrans->m_scale = 
			Vector3{ (MAX_SCALE - m_fScale) * SCREEN_WIDTH, (MAX_SCALE - m_fScale) * SCREEN_HEIGHT, 1.0f };

		// 右・左
		m_obj[0]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;
		m_obj[1]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;

		//上・下
		m_obj[2]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;
		m_obj[3]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;

		break;

	case(E_STATE_FADE_OUT):		// フェードアウト
		// α値の増加
		m_fScale += (MAX_SCALE / m_nFrame);

		// α値がMAX_SCALEを超えたらフェードインへ
		if (m_fScale > MAX_SCALE)
		{
			m_fScale = MAX_SCALE + 1.0f;
			m_nState = E_STATE_FADE_IN;

			// フェードアウトが終わったときに実装する処理
			// シーンの移動
			if (m_fadeEndFunc) m_fadeEndFunc();

			// サウンド
			CSound::PlaySE("fadein.wav");

			// フェードの消去
			CObjectManager::DestroyObject(m_pParent);
		}

		// 各スケールの更新
		// 円
		m_pParent->m_pTrans->m_scale = 
			Vector3{ (MAX_SCALE - m_fScale) * SCREEN_WIDTH, (MAX_SCALE - m_fScale) * SCREEN_HEIGHT, 1.0f };

		// 右・左
		m_obj[0]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;
		m_obj[1]->m_pTrans->m_scale->x = m_fScale * SCREEN_WIDTH;

		//上・下
		m_obj[2]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;
		m_obj[3]->m_pTrans->m_scale->y = m_fScale * SCREEN_HEIGHT;

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
void CFadeComponent::End()
{
	for (int i = 0; i < 4; ++i)
	{
		CObjectManager::DestroyObject(m_obj[i]);
	}
}


//======================================================
//
//	フェードアウトの開始
//	フェードアウト開始時間に外からの呼び出す関数
//
//======================================================
void CFadeComponent::StartFadeOut(int nFrame)
{
	// 既にフェード中の時は処理しない
	if (E_STATE_FADE_NODE != m_nState)
	{
		return;
	}

	m_nFrame = nFrame;
	m_fScale = 0;
	m_nState = E_STATE_FADE_OUT;
	m_bNowFade = true;
}
