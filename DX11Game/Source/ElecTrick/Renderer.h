#pragma once

#include "AssimpModel.h"
#include "mesh.h"
#include "CComponent.h"
#include "CObject.h"
#include "Texture.h"
#include "Camera.h"

#include <map>
#include <vector>
#include <list>

namespace Renderer
{
	enum ERendererType
	{
		eOpaque,
		eTransparent,
		eSkyBox,
		ePostProcessing,
		eUI,

		eMaxType,
	};
}

class CRenderer : public CComponent
{
public:
	CRenderer() 
	{
		// リストに格納
		m_meshList.push_back(this);

		// ワールドマトリックス
		m_mtxWorld = nullptr;

		// テクスチャ座標
		m_texPos = Vector3(0.0f, 0.0f, 0.0f);
		m_texSize = Vector3(1.0f, 1.0f, 1.0f);
		XMMATRIX mTex = XMMatrixScaling(m_texSize->x, m_texSize->y, 0.0f);
		mTex *= XMMatrixTranslation(m_texPos->x, m_texPos->y, 1.0f);
		XMStoreFloat4x4(&m_mtxTexture, mTex);

		// ブレンドステート
		m_eState = BS_NONE;

		// レンダラータイプ
		m_eRendererType = Renderer::eOpaque;

		// カリング
		m_bCull = true;
	}
	virtual ~CRenderer()
	{
		auto itr = std::find(m_meshList.begin(), m_meshList.end(), this);

		if (m_meshList.end() != itr)
		{
			m_meshList.erase(itr);
		}
	}

	// カメラからの距離を格納用
	void LateUpdate() override
	{
		// カメラ座標
		Vector3 cameraPos = CCamera::GetMainCamera()->GetPos();
		// 距離を格納
		m_fLayer = Vector3::Length(m_pParent->m_pTrans->m_pos - cameraPos);
	}

	virtual void EarlyDraw(ID3D11DeviceContext* pDC) = 0;
	virtual void LateDraw(ID3D11DeviceContext* pDC) = 0;

	// ソート用
	static bool swapR(const CRenderer* lhs, const CRenderer* rhs) {
		return lhs->m_fLayer > rhs->m_fLayer; // 降順にしてみる
	}

	// 描画
	static void ListDraw(ID3D11DeviceContext* pDC);


	// テクスチャマトリックスの更新
	void UpdateTexMatrix()
	{
		XMMATRIX mTex = XMMatrixScaling(m_texSize->x, m_texSize->y, 0.0f);
		mTex *= XMMatrixTranslation(m_texPos->x, m_texPos->y, 1.0f);
		XMStoreFloat4x4(&m_mtxTexture, mTex);
	}


	// UV
	Vector3 GetTexPos() { return m_texPos; }
	Vector3 GetTexSize() { return m_texSize; }
	void SetTexPos(Vector3 pos) { m_texPos = pos; }
	void SetTexSize(Vector3 size) { m_texSize = size; }

	// ブレンドステート
	void SetMeshBlendState(EBlendState eBlendState) { m_eState = eBlendState; }
	EBlendState GetMeshBlendState() { return m_eState; }

	// カリング
	void SetCullMode(bool bCull) { m_bCull = bCull; }

	// 

private:
	static std::list<CRenderer*> m_meshList;


	static std::list<CRenderer*> m_opaqueList;
	static std::list<CRenderer*> m_UIList;


protected:
	XMFLOAT4X4* m_mtxWorld;					// ワールドマトリックス
	XMFLOAT4X4 m_mtxTexture;				// テクスチャ マトリックス

	Vector3 m_texPos;
	Vector3 m_texSize;

	// ブレンドステート
	EBlendState m_eState;

	// レンダラータイプ
	Renderer::ERendererType m_eRendererType;

	float m_fLayer;

	// カリングフラグ
	bool m_bCull;

};


class CAssimpRenderer final : public CRenderer
{
public:
	CAssimpRenderer()
	{
		m_model = nullptr;
		m_filename = "\0";
		m_diffuseColor = { 1,1,1,1 };

		m_eRendererType = Renderer::eOpaque;
	}
	~CAssimpRenderer() 
	{
		
	}

	void Start(void) override
	{
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();
		m_this = m_pParent->GetComponent<CAssimpRenderer>();
	}

	bool ModelLoad(std::string filename)
	{
		ID3D11Device* pDevice = GetDevice();
		ID3D11DeviceContext* pDC = GetDeviceContext();

		// プールから検索
		const auto& itr = m_assimpModelPool.find(filename);

		// 既にプールにあったら
		if (m_assimpModelPool.end() != itr)
		{
			m_model = itr->second;
			m_filename = filename;
			m_pTexture = GetMaterial()->pTexture;

			return true;
		}

		// なかったら新しく生成する
		const auto& am = new CAssimpModel();

		// モデルのロード
		bool b = am->Load(pDevice, pDC, filename);

		// プールに登録
		m_assimpModelPool.emplace(filename, am);

		// 格納
		m_model = am;
		m_filename = filename;
		m_pTexture = GetMaterial()->pTexture;

		// ライトとカメラ
		m_model->SetLight(CLight::GetMainLight());
		m_model->SetCamera(CCamera::GetMainCamera());

		return b;
	}

	void EarlyDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& model = m_model;

		if (model)
		{
			model->Draw(pDC, *m_mtxWorld, eNoAffect, true);
		}
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& model = m_model;

		if (model)
		{
			// カラー・テクスチャ変更
			TAssimpMaterial* pMat = m_model->GetMaterial();
			if (pMat)
			{
				pMat->Kd = m_diffuseColor;
				pMat->pTexture = m_pTexture;
			}

			bool bExitFlg = model->Draw(pDC, *m_mtxWorld, eNoAffect, false);


			// ここでコールバック関数の処理？
			if (bExitFlg)
			{
				// コールバック関数
				for (const auto& com : m_pParent->GetComponentList())
				{
					// アニメーション終了
					com->OnAnimationExit3D(m_this.lock());
				}
			}
		}
	}

	void ModelRelese()
	{
		const auto& model = m_model;

		if (model)
		{
			model->Release();
		}

		// プールから検索
		const auto& itr = m_assimpModelPool.find(m_filename);

		// プールにあったら
		if (m_assimpModelPool.end() != itr)
		{
			m_assimpModelPool.erase(itr);
		}

		if (m_model)
		{
			delete m_model;
			m_model = nullptr;
			m_filename = "\0";
		}
	}

	//--- マテリアル
	TAssimpMaterial* GetMaterial()
	{
		// モデルがなかったら
		if (!m_model) return nullptr;

		//// マテリアルがあったら
		//if (m_model->GetMeshList()->back().GetMaterial())
		//{
		//	m_pTexture = m_model->GetMeshList()->back().GetMaterial()->pTexture;
		//	return m_model->GetMeshList()->back().GetMaterial();
		//}
		if (m_model->GetMaterial()) return m_model->GetMaterial();
		

		// マテリアル検索
		const auto& itr = m_materialPool.find(m_model);

		// マテリアルがプールにあったら
		if (m_materialPool.end() != itr)
		{
			return itr->second;
		}

		// 新規作成
		auto pMat = new TAssimpMaterial();

		// モデルに格納
		m_model->SetMaterial(pMat);

		// プールに格納
		m_materialPool.emplace(m_model, pMat);

		return pMat;
	}

	// カラー変更
	void SetDiffuseColor(XMFLOAT4 color) 
	{
		m_diffuseColor = color;

		//// マテリアル取得
		//auto pMat = GetMaterial();
		//
		//// 色の変更
		//if (pMat)
		//{
		//	pMat->Kd = color;
		//}
	}

	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const char* filename)
	{
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);

		//// マテリアル取得
		//auto pMat = GetMaterial();
		//// テクスチャ
		//if (pMat)
		//	pMat->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const wchar_t* filename)
	{
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);

		//// マテリアル取得
		//auto pMat = GetMaterial();
		//// テクスチャ
		//if (pMat)
		//	pMat->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}


	//CAssimpModel* GetModel()
	//{
	//	return m_model;
	//}

	// アニメーションインデックス
	void SetAnimIndex(int nIndex)
	{
		if (m_model)
		{
			m_model->SetAnimIndex(nIndex);
		}
	}

	// 現在のアニメーションインデックスを取得
	int GetCurrentAnimIndex()
	{
		if (m_model)
		{
			return m_model->GetCurrentAnimIndex();
		}
		return -1;
	}

	// アニメーションスピード
	void SetAnimSpeed(double dSpeed)
	{
		if (m_model)
		{
			m_model->SetAnimSpeed(dSpeed);
		}
	}
	// アニメーションの再生場所
	void SetStartAnimTime(double dTime)
	{
		if (m_model)
		{
			m_model->SetStartAnimTime(dTime);
		}
	}

	// アニメーションの再生
	void PlayAnim() {
		if (m_model) {
			m_model->SetStopAnim(false);
		}
	}
	// アニメーションのストップ
	void StopAnim() {
		if (m_model) {
			m_model->SetStopAnim(true);
		}
	}

private:
	CAssimpModel* m_model;
	std::string m_filename;
	XMFLOAT4X4* m_mtxWorld;
	XMFLOAT4 m_diffuseColor;
	ID3D11ShaderResourceView* m_pTexture = nullptr;		// 拡散テクスチャ

	std::weak_ptr<CAssimpRenderer> m_this;

	// AssimpModelプール
	static std::map<const std::string, CAssimpModel*, std::less<>> m_assimpModelPool;
	// マテリアルプール
	static std::map<const CAssimpModel*, TAssimpMaterial*, std::less<>> m_materialPool;
};


class CMeshRenderer final : public CRenderer
{
private:

public:
	CMeshRenderer()
	{
		m_mesh = nullptr;

		m_material.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.Ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_material.Power = 50.0f;

		// レンダラータイプ
		m_eRendererType = Renderer::eOpaque;
	}
	~CMeshRenderer()
	{

	}

	void Start(void) override
	{
		// ワールドマトリックスを取得
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();
	}

	// 平面メッシュの生成
	HRESULT MakePlane(const std::string tag, int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f)
	{
		// メッシュの検索
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// 新規作成
		m_mesh = new MESH();


		// プールに格納
		m_meshPool.emplace(tag, m_mesh);

		// プリミティブ種別設定
		m_mesh->primitiveType = PT_TRIANGLESTRIP;
		// 頂点数の設定
		m_mesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);
		// インデックス数の設定(縮退ポリゴン用を考慮する)
		m_mesh->nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;
		// 頂点配列の作成
		VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh->nNumVertex];
		// インデックス配列の作成
		int* pIndexWk = new int[m_mesh->nNumIndex];
		// 頂点配列の中身を埋める
		VERTEX_3D* pVtx = pVertexWk;
		for (int z = 0; z < nNumBlockZ + 1; ++z) {
			for (int x = 0; x < nNumBlockX + 1; ++x) {

				// 頂点座標の設定
				pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
				pVtx->vtx.y = 0.0f;
				pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;
				// 法線の設定
				pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
				// 反射光の設定
				pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				// テクスチャ座標の設定
				pVtx->tex.x = fTexSizeX * x;
				pVtx->tex.y = fTexSizeZ * z;
				++pVtx;
			}
		}
		//インデックス配列の中身を埋める
		int* pIdx = pIndexWk;
		for (int z = 0; z < nNumBlockZ; ++z) {
			if (z > 0) {
				// 縮退ポリゴンのためのダブりの設定
				*pIdx++ = (z + 1) * (nNumBlockX + 1);
			}
			for (int x = 0; x < nNumBlockX + 1; ++x) {
				*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
				*pIdx++ = z * (nNumBlockX + 1) + x;
			}
			if (z < nNumBlockZ - 1) {
				// 縮退ポリゴンのためのダブりの設定
				*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
			}
		}

		ID3D11Device* pDevice = GetDevice();
		// 頂点バッファ/インデックス バッファ生成
		HRESULT hr = MakeMeshVertex(pDevice, m_mesh, pVertexWk, pIndexWk);
		// 一時配列の解放
		delete[] pVertexWk;
		delete[] pIndexWk;
		return hr;
	}

	// キューブ
	HRESULT MakeBlock(const std::string tag)
	{
		// メッシュの検索
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// 新規作成
		m_mesh = new MESH();


		// プールに格納
		m_meshPool.emplace(tag, m_mesh);


#define	SIZE_X			(0.5f)											// 立方体のサイズ(X方向)
#define	SIZE_Y			(0.5f)											// 立方体のサイズ(Y方向)
#define	SIZE_Z			(0.5f)											// 立方体のサイズ(Z方向)

#define CUBE_VERTEX		(24)
#define CUBE_INDEX		(36)

		// プリミティブ設定
		m_mesh->primitiveType = PT_TRIANGLE;

		VERTEX_3D	vertexWk[CUBE_VERTEX];	// 頂点情報格納ワーク
		int			indexWk[CUBE_INDEX];	// インデックス格納ワーク

		// 頂点数
		m_mesh->nNumVertex = CUBE_VERTEX;

		// 頂点座標の設定
		// 前
		vertexWk[0].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[1].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[3].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		// 右
		vertexWk[4].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[5].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[6].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[7].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		// 上
		vertexWk[8].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[9].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[10].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[11].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		// 後
		vertexWk[12].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[13].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[14].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[15].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		// 左
		vertexWk[16].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[17].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[18].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[19].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		// 下
		vertexWk[20].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[21].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[22].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[23].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);


		// 法線ベクトルの設定
		// 前
		vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		// 右
		vertexWk[4].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[5].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[6].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[7].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		// 上
		vertexWk[8].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[9].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[10].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[11].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		// 後
		vertexWk[12].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[13].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[14].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[15].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		// 左
		vertexWk[16].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[17].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[18].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[19].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// 下
		vertexWk[20].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[21].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[22].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[23].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);


		// 拡散反射光の設定
		for (int i = 0; i < CUBE_VERTEX; i++)
		{
			vertexWk[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// テクスチャ座標の設定
		float texSize = 1.0f / 6;
		for (int i = 0; i < CUBE_VERTEX; i += 4)
		{
			vertexWk[0 + i].tex = XMFLOAT2(texSize * (i / 4)			, 0.0f);	// 左上
			vertexWk[1 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 0.0f);	// 右上

			if (i / 4 == 2 || i / 4 == 5)
			{
				vertexWk[2 + i].tex = XMFLOAT2(texSize * (i / 4)			, 1.0f * 3);	// 左下
				vertexWk[3 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 1.0f * 3);	// 右下
			}
			else
			{
				vertexWk[2 + i].tex = XMFLOAT2(texSize * (i / 4)			, 1.0f);	// 左下
				vertexWk[3 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 1.0f);	// 右下
			}
		}

		// インデックス数
		m_mesh->nNumIndex = CUBE_INDEX;

		// インデックス配列の設定
		for (int i = 0, j = 0; i < CUBE_INDEX; i += 6, j += 4)
		{
			indexWk[0 + i] = 0 + j;
			indexWk[1 + i] = 1 + j;
			indexWk[2 + i] = 2 + j;
			indexWk[3 + i] = 2 + j;
			indexWk[4 + i] = 1 + j;
			indexWk[5 + i] = 3 + j;
		}

		ID3D11Device* pDevice = GetDevice();
		return MakeMeshVertex(pDevice, m_mesh, vertexWk, indexWk);
		return S_OK;
	}

	// ブロック
	HRESULT MakeCube(const std::string tag)
	{
		// メッシュの検索
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// 新規作成
		m_mesh = new MESH();


		// プールに格納
		m_meshPool.emplace(tag, m_mesh);


#define	SIZE_X			(0.5f)											// 立方体のサイズ(X方向)
#define	SIZE_Y			(0.5f)											// 立方体のサイズ(Y方向)
#define	SIZE_Z			(0.5f)											// 立方体のサイズ(Z方向)

#define CUBE_VERTEX		(24)
#define CUBE_INDEX		(36)

		// プリミティブ設定
		m_mesh->primitiveType = PT_TRIANGLE;

		VERTEX_3D	vertexWk[CUBE_VERTEX];	// 頂点情報格納ワーク
		int			indexWk[CUBE_INDEX];	// インデックス格納ワーク

		// 頂点数
		m_mesh->nNumVertex = CUBE_VERTEX;

		// 頂点座標の設定
		// 前
		vertexWk[0].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[1].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[3].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		// 右
		vertexWk[4].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[5].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[6].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[7].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		// 上
		vertexWk[8].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[9].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[10].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[11].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		// 後
		vertexWk[12].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[13].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[14].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[15].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		// 左
		vertexWk[16].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[17].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[18].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[19].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		// 下
		vertexWk[20].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[21].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[22].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[23].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);


		// 法線ベクトルの設定
		// 前
		vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		// 右
		vertexWk[4].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[5].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[6].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[7].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		// 上
		vertexWk[8].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[9].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[10].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[11].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		// 後
		vertexWk[12].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[13].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[14].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[15].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		// 左
		vertexWk[16].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[17].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[18].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[19].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// 下
		vertexWk[20].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[21].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[22].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[23].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);


		// 拡散反射光の設定
		for (int i = 0; i < CUBE_VERTEX; i++)
		{
			vertexWk[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}



		// テクスチャ座標の設定
		for (int i = 0; i < CUBE_VERTEX; i += 4)
		{
			vertexWk[0 + i].tex = XMFLOAT2(0.0f, 0.0f);
			vertexWk[1 + i].tex = XMFLOAT2(1.0f, 0.0f);
			vertexWk[2 + i].tex = XMFLOAT2(0.0f, 1.0f);
			vertexWk[3 + i].tex = XMFLOAT2(1.0f, 1.0f);
		}

		// インデックス数
		m_mesh->nNumIndex = CUBE_INDEX;

		// インデックス配列の設定
		for (int i = 0, j = 0; i < CUBE_INDEX; i += 6, j += 4)
		{
			indexWk[0 + i] = 0 + j;
			indexWk[1 + i] = 1 + j;
			indexWk[2 + i] = 2 + j;
			indexWk[3 + i] = 2 + j;
			indexWk[4 + i] = 1 + j;
			indexWk[5 + i] = 3 + j;
		}

		ID3D11Device* pDevice = GetDevice();
		return MakeMeshVertex(pDevice, m_mesh, vertexWk, indexWk);
		return S_OK;
	}

	void EarlyDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& mesh = m_mesh;

		if (mesh)
		{
			m_mesh->mtxWorld = *m_mtxWorld;
			m_mesh->mtxTexture = m_mtxTexture;
			m_mesh->pMaterial = &m_material;

			DrawMeshShadow(pDC, mesh);
		}
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& mesh = m_mesh;

		if (mesh)
		{
			m_mesh->mtxWorld = *m_mtxWorld;
			m_mesh->mtxTexture = m_mtxTexture;
			m_mesh->pMaterial = &m_material;

			SetZBuffer(m_bZBuffer);

			DrawMesh(pDC, mesh, m_eTranslucentType);
		}
	}


	//--- マテリアル
	// カラー変更
	void SetDiffuseColor(XMFLOAT4 color) { m_material.Diffuse = color; }

	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const char* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const wchar_t* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ノーマルテクスチャのセット
	void SetNormalTexture(const char* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pNormalTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ノーマルテクスチャのセット
	void SetNormalTexture(const wchar_t* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pNormalTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// 環境テクスチャのセット
	void SetAmbientTexture(const char* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pAmbientTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// 環境テクスチャのセット
	void SetAmbientTexture(const wchar_t* filename)
	{
		// メッシュ取得
		auto pMesh = m_mesh;
		// テクスチャ
		if (pMesh)
			pMesh->pAmbientTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// 描画タイプ
	void SetTranslucentType(eTranslucentType eType) { m_eTranslucentType = eType; }
	// プリミティブ
	void SetPrimitiveType(ePrimitiveType eType) { if (m_mesh) m_mesh->primitiveType = eType; }

	void SetWorldMatrix(XMFLOAT4X4* pWorld) { m_mtxWorld = pWorld; }


	// Zバッファー
	void SetZBufferFlg(bool bZBuffer)
	{
		m_bZBuffer = bZBuffer;
	}

private:
	MESH* m_mesh;
	MATERIAL m_material;

	// 描画タイプ
	eTranslucentType m_eTranslucentType;

	// メッシュのプール
	static std::map<const std::string, MESH*, std::less<>> m_meshPool;

	bool m_bZBuffer = true;
};


// ビルボードレンダラー
class CBillboardRenderer final : public CRenderer
{
private:

public:
	CBillboardRenderer()
	{
		m_mesh = nullptr;

		m_material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_material.Power = 0.0f;

		m_diffuseTexture = nullptr;

		// レンダラータイプ
		m_eRendererType = Renderer::eTransparent;

		// メッシュの取得
		m_mesh = GetBillboardMesh();

	}
	~CBillboardRenderer()
	{

	}

	void Start(void) override
	{
		// ワールドマトリックスを取得
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();

	}

	// ビルボードメッシュ取得
	static MESH* GetBillboardMesh()
	{
		// メッシュが生成されている
		if (m_masterMesh.pVertexBuffer	|| m_masterMesh.pIndexBuffer ||
			m_masterMesh.nNumVertex		|| m_masterMesh.nNumIndex)
		{
			return &m_masterMesh;
		}

		// オブジェクトの頂点配列を生成
		m_masterMesh.nNumVertex = 4;
		VERTEX_3D* pVertexWk = new VERTEX_3D[m_masterMesh.nNumVertex];

		// 頂点配列の中身を埋める
		VERTEX_3D* pVtx = pVertexWk;

		// 頂点座標の設定
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

		// 法線の設定
		pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// 反射光の設定
		pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
		pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
		pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
		pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

		// インデックス配列を生成
		m_masterMesh.nNumIndex = 4;
		int* pIndexWk = new int[m_masterMesh.nNumIndex];

		// インデックス配列の中身を埋める
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		ID3D11Device* pDevice = GetDevice();
		// 頂点バッファ生成
		HRESULT hr = MakeMeshVertex(pDevice, &m_masterMesh, pVertexWk, pIndexWk);

		// 一時配列の解放
		delete[] pIndexWk;
		delete[] pVertexWk;

		m_masterMesh.bLight = false;

		return &m_masterMesh;
	}

	void EarlyDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& mesh = m_mesh;

		if (mesh)
		{
			m_mesh->mtxTexture = m_mtxTexture;
			m_mesh->pMaterial = &m_material;
			m_mesh->pTexture = m_diffuseTexture;

			// Zバッファ無効
			SetZBuffer(false);

			DrawMeshShadow(pDC, mesh);

			// Zバッファ
			SetZBuffer(true);
		}
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& mesh = m_mesh;

		if (mesh)
		{
			// マトリックス作成
			XMMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
			// カメラ行列取得
			XMFLOAT4X4& mtxView = CCamera::GetMainCamera()->GetViewMatrix();
			// ワールドマトリックスの初期化
			mtxWorld = XMMatrixIdentity();
			XMStoreFloat4x4(m_mtxWorld, mtxWorld);

			m_mtxWorld->_11 = mtxView._11;
			m_mtxWorld->_12 = mtxView._21;
			m_mtxWorld->_13 = mtxView._31;
			m_mtxWorld->_21 = mtxView._12;
			m_mtxWorld->_22 = mtxView._22;
			m_mtxWorld->_23 = mtxView._32;
			m_mtxWorld->_31 = mtxView._13;
			m_mtxWorld->_32 = mtxView._23;
			m_mtxWorld->_33 = mtxView._33;

			mtxWorld = XMLoadFloat4x4(m_mtxWorld);

			// スケールを反映
			Vector3 scale = m_pParent->m_pTrans->m_scale;
			mtxScale = XMMatrixScaling(scale->x, scale->y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// 回転
			Vector3 rot = m_pParent->m_pTrans->m_rot;
			mtxRot = XMMatrixRotationZ(rot->z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			// 移動を反映
			Vector3 pos = m_pParent->m_pTrans->m_pos;
			mtxTranslate = XMMatrixTranslation(pos->x, pos->y, pos->z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ワールドマトリックスの設定
			XMStoreFloat4x4(m_mtxWorld, mtxWorld);

			m_mesh->mtxWorld = *m_mtxWorld;

			m_mesh->mtxTexture = m_mtxTexture;
			m_mesh->pMaterial = &m_material;
			m_mesh->pTexture = m_diffuseTexture;

			// Zバッファ無効
			SetZBuffer(false);

			DrawMesh(pDC, mesh);

			// Zバッファ
			SetZBuffer(true);
		}
	}


	//--- マテリアル
	// カラー変更
	void SetDiffuseColor(XMFLOAT4 color) { m_material.Diffuse = color; }

	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const char* filename)
	{
		// テクスチャのセット
		m_diffuseTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// ベースカラーテクスチャのセット
	void SetDiffuseTexture(const wchar_t* filename)
	{
		// テクスチャのセット
		m_diffuseTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ワールドマトリックス
	void SetWorldMatrix(XMFLOAT4X4* pWorld) { m_mtxWorld = pWorld; }

private:
	MESH* m_mesh;
	MATERIAL m_material;

	ID3D11ShaderResourceView* m_diffuseTexture;

	static MESH m_masterMesh;
};
