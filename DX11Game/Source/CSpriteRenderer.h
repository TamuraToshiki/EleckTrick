//======================================================================
//											CSpriteRenderer.h
//	スプライトレンダラー
//
//======================================================================
//	author :	AT12A 05 宇佐美晃之
//======================================================================
//	開発履歴
//
//	2020/12/06	作成
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "Renderer.h"
#include "polygon.h"


//===== 構造体定義 =====


//===== クラス定義 =====
class CSpriteRenderer : public CRenderer
{
public:
	// レイヤー
	enum ESpriteLayer
	{
		// 先頭
		eTop,

		// フェード
		eFade3 = 1,
		eFade2,
		eFade1,

		// UI
		eUI5 = 5,
		eUI4,
		eUI3,
		eUI2,
		eUI1,

		eDefault,

		// 背景
		eBG3 = (int)VIEW_FAR_Z + 1,
		eBG2,
		eBG1,

		// 最後
		eBottom,

		eMaxLayer,
	};

public:
	CSpriteRenderer()
	{
		m_fLayer = eDefault;
		m_pTexture = nullptr;
		// 色
		m_color = Vector3{ 1,1,1 };
		m_fAlpha = 1.0f;

		// ブレンドステート
		m_eState = BS_ALPHABLEND;

		// カリングオフ
		m_bCull = false;

		// レンダラータイプ
		m_eRendererType = Renderer::eUI;
	}
	~CSpriteRenderer(){}

	// 作成時に呼ばれます
	void Start() override
	{
		// ワールドマトリックスを取得
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();
	}

	// レイヤーを格納
	void LateUpdate() override
	{
		// 現状なし

	}

	void EarlyDraw(ID3D11DeviceContext* pDC) override
	{
		// 影描画なし
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		// 判定
		const auto& trans = m_pParent->m_pTrans;
		if (!trans) return;

		// Zバッファ無効
		SetZBuffer(false);

		// 座標
		SetPolygonPos(trans->m_pos->x, trans->m_pos->y);
		// 角度
		SetPolygonAngle(trans->m_rot->z);
		// サイズ
		SetPolygonSize(trans->m_scale->x, trans->m_scale->y);
		// カラー
		SetPolygonColor(m_color->x, m_color->y, m_color->z);
		// アルファ
		SetPolygonAlpha(m_fAlpha);
		// テクスチャ座標
		SetPolygonUV(m_texPos->x, m_texPos->y);
		// テクスチャサイズ
		SetPolygonFrameSize(m_texSize->x, m_texSize->y);
		// テクスチャ
		SetPolygonTexture(m_pTexture);

		// 描画
		DrawPolygon(pDC);

		// Zバッファ有効
		SetZBuffer(true);
	}

	//--- マテリアル
	// カラー変更
	void SetColor(Vector3 color) { m_color = color; }
	void SetColor(float r, float g, float b) { 
		m_color->x = r; 
		m_color->y = g;
		m_color->z = b;
	}
	void SetAlpha(float alpha) { m_fAlpha = alpha; }

	// テクスチャのセット
	void SetTexture(const char* filename)
	{
		// テクスチャのセット
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// テクスチャのセット
	void SetTexture(const wchar_t* filename)
	{
		// テクスチャのセット
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// レイヤー
	void SetLayer(ESpriteLayer eLayer) { m_fLayer = (float)eLayer; }
	void SetLayer(int nLayer) { m_fLayer = (float)nLayer; }


	// カラー取得
	Vector3 GetColor() { return m_color; }
	// アルファ取得
	float GetAlpha() { return m_fAlpha; }
	// レイヤー取得
	int GetLayer() { return (int)m_fLayer; }

private:
	// テクスチャ
	ID3D11ShaderResourceView* m_pTexture;
	// カラー
	Vector3 m_color;
	// アルファ
	float m_fAlpha;
};


//===== プロトタイプ宣言 =====


