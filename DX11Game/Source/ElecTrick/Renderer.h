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
		// ���X�g�Ɋi�[
		m_meshList.push_back(this);

		// ���[���h�}�g���b�N�X
		m_mtxWorld = nullptr;

		// �e�N�X�`�����W
		m_texPos = Vector3(0.0f, 0.0f, 0.0f);
		m_texSize = Vector3(1.0f, 1.0f, 1.0f);
		XMMATRIX mTex = XMMatrixScaling(m_texSize->x, m_texSize->y, 0.0f);
		mTex *= XMMatrixTranslation(m_texPos->x, m_texPos->y, 1.0f);
		XMStoreFloat4x4(&m_mtxTexture, mTex);

		// �u�����h�X�e�[�g
		m_eState = BS_NONE;

		// �����_���[�^�C�v
		m_eRendererType = Renderer::eOpaque;

		// �J�����O
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

	// �J��������̋������i�[�p
	void LateUpdate() override
	{
		// �J�������W
		Vector3 cameraPos = CCamera::GetMainCamera()->GetPos();
		// �������i�[
		m_fLayer = Vector3::Length(m_pParent->m_pTrans->m_pos - cameraPos);
	}

	virtual void EarlyDraw(ID3D11DeviceContext* pDC) = 0;
	virtual void LateDraw(ID3D11DeviceContext* pDC) = 0;

	// �\�[�g�p
	static bool swapR(const CRenderer* lhs, const CRenderer* rhs) {
		return lhs->m_fLayer > rhs->m_fLayer; // �~���ɂ��Ă݂�
	}

	// �`��
	static void ListDraw(ID3D11DeviceContext* pDC);


	// �e�N�X�`���}�g���b�N�X�̍X�V
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

	// �u�����h�X�e�[�g
	void SetMeshBlendState(EBlendState eBlendState) { m_eState = eBlendState; }
	EBlendState GetMeshBlendState() { return m_eState; }

	// �J�����O
	void SetCullMode(bool bCull) { m_bCull = bCull; }

	// 

private:
	static std::list<CRenderer*> m_meshList;


	static std::list<CRenderer*> m_opaqueList;
	static std::list<CRenderer*> m_UIList;


protected:
	XMFLOAT4X4* m_mtxWorld;					// ���[���h�}�g���b�N�X
	XMFLOAT4X4 m_mtxTexture;				// �e�N�X�`�� �}�g���b�N�X

	Vector3 m_texPos;
	Vector3 m_texSize;

	// �u�����h�X�e�[�g
	EBlendState m_eState;

	// �����_���[�^�C�v
	Renderer::ERendererType m_eRendererType;

	float m_fLayer;

	// �J�����O�t���O
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

		// �v�[�����猟��
		const auto& itr = m_assimpModelPool.find(filename);

		// ���Ƀv�[���ɂ�������
		if (m_assimpModelPool.end() != itr)
		{
			m_model = itr->second;
			m_filename = filename;
			m_pTexture = GetMaterial()->pTexture;

			return true;
		}

		// �Ȃ�������V������������
		const auto& am = new CAssimpModel();

		// ���f���̃��[�h
		bool b = am->Load(pDevice, pDC, filename);

		// �v�[���ɓo�^
		m_assimpModelPool.emplace(filename, am);

		// �i�[
		m_model = am;
		m_filename = filename;
		m_pTexture = GetMaterial()->pTexture;

		// ���C�g�ƃJ����
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
			// �J���[�E�e�N�X�`���ύX
			TAssimpMaterial* pMat = m_model->GetMaterial();
			if (pMat)
			{
				pMat->Kd = m_diffuseColor;
				pMat->pTexture = m_pTexture;
			}

			bool bExitFlg = model->Draw(pDC, *m_mtxWorld, eNoAffect, false);


			// �����ŃR�[���o�b�N�֐��̏����H
			if (bExitFlg)
			{
				// �R�[���o�b�N�֐�
				for (const auto& com : m_pParent->GetComponentList())
				{
					// �A�j���[�V�����I��
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

		// �v�[�����猟��
		const auto& itr = m_assimpModelPool.find(m_filename);

		// �v�[���ɂ�������
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

	//--- �}�e���A��
	TAssimpMaterial* GetMaterial()
	{
		// ���f�����Ȃ�������
		if (!m_model) return nullptr;

		//// �}�e���A������������
		//if (m_model->GetMeshList()->back().GetMaterial())
		//{
		//	m_pTexture = m_model->GetMeshList()->back().GetMaterial()->pTexture;
		//	return m_model->GetMeshList()->back().GetMaterial();
		//}
		if (m_model->GetMaterial()) return m_model->GetMaterial();
		

		// �}�e���A������
		const auto& itr = m_materialPool.find(m_model);

		// �}�e���A�����v�[���ɂ�������
		if (m_materialPool.end() != itr)
		{
			return itr->second;
		}

		// �V�K�쐬
		auto pMat = new TAssimpMaterial();

		// ���f���Ɋi�[
		m_model->SetMaterial(pMat);

		// �v�[���Ɋi�[
		m_materialPool.emplace(m_model, pMat);

		return pMat;
	}

	// �J���[�ύX
	void SetDiffuseColor(XMFLOAT4 color) 
	{
		m_diffuseColor = color;

		//// �}�e���A���擾
		//auto pMat = GetMaterial();
		//
		//// �F�̕ύX
		//if (pMat)
		//{
		//	pMat->Kd = color;
		//}
	}

	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const char* filename)
	{
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);

		//// �}�e���A���擾
		//auto pMat = GetMaterial();
		//// �e�N�X�`��
		//if (pMat)
		//	pMat->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const wchar_t* filename)
	{
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);

		//// �}�e���A���擾
		//auto pMat = GetMaterial();
		//// �e�N�X�`��
		//if (pMat)
		//	pMat->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}


	//CAssimpModel* GetModel()
	//{
	//	return m_model;
	//}

	// �A�j���[�V�����C���f�b�N�X
	void SetAnimIndex(int nIndex)
	{
		if (m_model)
		{
			m_model->SetAnimIndex(nIndex);
		}
	}

	// ���݂̃A�j���[�V�����C���f�b�N�X���擾
	int GetCurrentAnimIndex()
	{
		if (m_model)
		{
			return m_model->GetCurrentAnimIndex();
		}
		return -1;
	}

	// �A�j���[�V�����X�s�[�h
	void SetAnimSpeed(double dSpeed)
	{
		if (m_model)
		{
			m_model->SetAnimSpeed(dSpeed);
		}
	}
	// �A�j���[�V�����̍Đ��ꏊ
	void SetStartAnimTime(double dTime)
	{
		if (m_model)
		{
			m_model->SetStartAnimTime(dTime);
		}
	}

	// �A�j���[�V�����̍Đ�
	void PlayAnim() {
		if (m_model) {
			m_model->SetStopAnim(false);
		}
	}
	// �A�j���[�V�����̃X�g�b�v
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
	ID3D11ShaderResourceView* m_pTexture = nullptr;		// �g�U�e�N�X�`��

	std::weak_ptr<CAssimpRenderer> m_this;

	// AssimpModel�v�[��
	static std::map<const std::string, CAssimpModel*, std::less<>> m_assimpModelPool;
	// �}�e���A���v�[��
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

		// �����_���[�^�C�v
		m_eRendererType = Renderer::eOpaque;
	}
	~CMeshRenderer()
	{

	}

	void Start(void) override
	{
		// ���[���h�}�g���b�N�X���擾
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();
	}

	// ���ʃ��b�V���̐���
	HRESULT MakePlane(const std::string tag, int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ,
		float fTexSizeX = 1.0f, float fTexSizeZ = 1.0f)
	{
		// ���b�V���̌���
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// �V�K�쐬
		m_mesh = new MESH();


		// �v�[���Ɋi�[
		m_meshPool.emplace(tag, m_mesh);

		// �v���~�e�B�u��ʐݒ�
		m_mesh->primitiveType = PT_TRIANGLESTRIP;
		// ���_���̐ݒ�
		m_mesh->nNumVertex = (nNumBlockX + 1) * (nNumBlockZ + 1);
		// �C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
		m_mesh->nNumIndex = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;
		// ���_�z��̍쐬
		VERTEX_3D* pVertexWk = new VERTEX_3D[m_mesh->nNumVertex];
		// �C���f�b�N�X�z��̍쐬
		int* pIndexWk = new int[m_mesh->nNumIndex];
		// ���_�z��̒��g�𖄂߂�
		VERTEX_3D* pVtx = pVertexWk;
		for (int z = 0; z < nNumBlockZ + 1; ++z) {
			for (int x = 0; x < nNumBlockX + 1; ++x) {

				// ���_���W�̐ݒ�
				pVtx->vtx.x = x * fSizeBlockX - (nNumBlockX * 0.5f) * fSizeBlockX;
				pVtx->vtx.y = 0.0f;
				pVtx->vtx.z = -z * fSizeBlockZ + (nNumBlockZ * 0.5f) * fSizeBlockZ;
				// �@���̐ݒ�
				pVtx->nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
				// ���ˌ��̐ݒ�
				pVtx->diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				// �e�N�X�`�����W�̐ݒ�
				pVtx->tex.x = fTexSizeX * x;
				pVtx->tex.y = fTexSizeZ * z;
				++pVtx;
			}
		}
		//�C���f�b�N�X�z��̒��g�𖄂߂�
		int* pIdx = pIndexWk;
		for (int z = 0; z < nNumBlockZ; ++z) {
			if (z > 0) {
				// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				*pIdx++ = (z + 1) * (nNumBlockX + 1);
			}
			for (int x = 0; x < nNumBlockX + 1; ++x) {
				*pIdx++ = (z + 1) * (nNumBlockX + 1) + x;
				*pIdx++ = z * (nNumBlockX + 1) + x;
			}
			if (z < nNumBlockZ - 1) {
				// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				*pIdx++ = z * (nNumBlockX + 1) + nNumBlockX;
			}
		}

		ID3D11Device* pDevice = GetDevice();
		// ���_�o�b�t�@/�C���f�b�N�X �o�b�t�@����
		HRESULT hr = MakeMeshVertex(pDevice, m_mesh, pVertexWk, pIndexWk);
		// �ꎞ�z��̉��
		delete[] pVertexWk;
		delete[] pIndexWk;
		return hr;
	}

	// �L���[�u
	HRESULT MakeBlock(const std::string tag)
	{
		// ���b�V���̌���
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// �V�K�쐬
		m_mesh = new MESH();


		// �v�[���Ɋi�[
		m_meshPool.emplace(tag, m_mesh);


#define	SIZE_X			(0.5f)											// �����̂̃T�C�Y(X����)
#define	SIZE_Y			(0.5f)											// �����̂̃T�C�Y(Y����)
#define	SIZE_Z			(0.5f)											// �����̂̃T�C�Y(Z����)

#define CUBE_VERTEX		(24)
#define CUBE_INDEX		(36)

		// �v���~�e�B�u�ݒ�
		m_mesh->primitiveType = PT_TRIANGLE;

		VERTEX_3D	vertexWk[CUBE_VERTEX];	// ���_���i�[���[�N
		int			indexWk[CUBE_INDEX];	// �C���f�b�N�X�i�[���[�N

		// ���_��
		m_mesh->nNumVertex = CUBE_VERTEX;

		// ���_���W�̐ݒ�
		// �O
		vertexWk[0].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[1].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[3].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		// �E
		vertexWk[4].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[5].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[6].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[7].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		// ��
		vertexWk[8].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[9].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[10].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[11].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		// ��
		vertexWk[12].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[13].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[14].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[15].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		// ��
		vertexWk[16].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[17].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[18].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[19].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		// ��
		vertexWk[20].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[21].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[22].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[23].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);


		// �@���x�N�g���̐ݒ�
		// �O
		vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		// �E
		vertexWk[4].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[5].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[6].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[7].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		// ��
		vertexWk[8].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[9].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[10].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[11].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		// ��
		vertexWk[12].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[13].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[14].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[15].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		// ��
		vertexWk[16].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[17].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[18].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[19].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// ��
		vertexWk[20].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[21].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[22].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[23].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);


		// �g�U���ˌ��̐ݒ�
		for (int i = 0; i < CUBE_VERTEX; i++)
		{
			vertexWk[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�̐ݒ�
		float texSize = 1.0f / 6;
		for (int i = 0; i < CUBE_VERTEX; i += 4)
		{
			vertexWk[0 + i].tex = XMFLOAT2(texSize * (i / 4)			, 0.0f);	// ����
			vertexWk[1 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 0.0f);	// �E��

			if (i / 4 == 2 || i / 4 == 5)
			{
				vertexWk[2 + i].tex = XMFLOAT2(texSize * (i / 4)			, 1.0f * 3);	// ����
				vertexWk[3 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 1.0f * 3);	// �E��
			}
			else
			{
				vertexWk[2 + i].tex = XMFLOAT2(texSize * (i / 4)			, 1.0f);	// ����
				vertexWk[3 + i].tex = XMFLOAT2(texSize * (i / 4) + texSize	, 1.0f);	// �E��
			}
		}

		// �C���f�b�N�X��
		m_mesh->nNumIndex = CUBE_INDEX;

		// �C���f�b�N�X�z��̐ݒ�
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

	// �u���b�N
	HRESULT MakeCube(const std::string tag)
	{
		// ���b�V���̌���
		const auto& itr = m_meshPool.find(tag);
		if (m_meshPool.end() != itr)
		{
			m_mesh = itr->second;
			return S_OK;
		}

		// �V�K�쐬
		m_mesh = new MESH();


		// �v�[���Ɋi�[
		m_meshPool.emplace(tag, m_mesh);


#define	SIZE_X			(0.5f)											// �����̂̃T�C�Y(X����)
#define	SIZE_Y			(0.5f)											// �����̂̃T�C�Y(Y����)
#define	SIZE_Z			(0.5f)											// �����̂̃T�C�Y(Z����)

#define CUBE_VERTEX		(24)
#define CUBE_INDEX		(36)

		// �v���~�e�B�u�ݒ�
		m_mesh->primitiveType = PT_TRIANGLE;

		VERTEX_3D	vertexWk[CUBE_VERTEX];	// ���_���i�[���[�N
		int			indexWk[CUBE_INDEX];	// �C���f�b�N�X�i�[���[�N

		// ���_��
		m_mesh->nNumVertex = CUBE_VERTEX;

		// ���_���W�̐ݒ�
		// �O
		vertexWk[0].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[1].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[2].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[3].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		// �E
		vertexWk[4].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[5].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[6].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[7].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		// ��
		vertexWk[8].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[9].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[10].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[11].vtx = XMFLOAT3(SIZE_X, SIZE_Y, -SIZE_Z);
		// ��
		vertexWk[12].vtx = XMFLOAT3(SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[13].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[14].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[15].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		// ��
		vertexWk[16].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, SIZE_Z);
		vertexWk[17].vtx = XMFLOAT3(-SIZE_X, SIZE_Y, -SIZE_Z);
		vertexWk[18].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[19].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);
		// ��
		vertexWk[20].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[21].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, SIZE_Z);
		vertexWk[22].vtx = XMFLOAT3(SIZE_X, -SIZE_Y, -SIZE_Z);
		vertexWk[23].vtx = XMFLOAT3(-SIZE_X, -SIZE_Y, -SIZE_Z);


		// �@���x�N�g���̐ݒ�
		// �O
		vertexWk[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertexWk[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		// �E
		vertexWk[4].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[5].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[6].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertexWk[7].nor = XMFLOAT3(1.0f, 0.0f, 0.0f);
		// ��
		vertexWk[8].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[9].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[10].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		vertexWk[11].nor = XMFLOAT3(0.0f, 1.0f, 0.0f);
		// ��
		vertexWk[12].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[13].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[14].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertexWk[15].nor = XMFLOAT3(0.0f, 0.0f, 1.0f);
		// ��
		vertexWk[16].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[17].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[18].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertexWk[19].nor = XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// ��
		vertexWk[20].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[21].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[22].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertexWk[23].nor = XMFLOAT3(0.0f, -1.0f, 0.0f);


		// �g�U���ˌ��̐ݒ�
		for (int i = 0; i < CUBE_VERTEX; i++)
		{
			vertexWk[i].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}



		// �e�N�X�`�����W�̐ݒ�
		for (int i = 0; i < CUBE_VERTEX; i += 4)
		{
			vertexWk[0 + i].tex = XMFLOAT2(0.0f, 0.0f);
			vertexWk[1 + i].tex = XMFLOAT2(1.0f, 0.0f);
			vertexWk[2 + i].tex = XMFLOAT2(0.0f, 1.0f);
			vertexWk[3 + i].tex = XMFLOAT2(1.0f, 1.0f);
		}

		// �C���f�b�N�X��
		m_mesh->nNumIndex = CUBE_INDEX;

		// �C���f�b�N�X�z��̐ݒ�
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


	//--- �}�e���A��
	// �J���[�ύX
	void SetDiffuseColor(XMFLOAT4 color) { m_material.Diffuse = color; }

	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const char* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const wchar_t* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// �m�[�}���e�N�X�`���̃Z�b�g
	void SetNormalTexture(const char* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pNormalTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// �m�[�}���e�N�X�`���̃Z�b�g
	void SetNormalTexture(const wchar_t* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pNormalTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ���e�N�X�`���̃Z�b�g
	void SetAmbientTexture(const char* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pAmbientTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ���e�N�X�`���̃Z�b�g
	void SetAmbientTexture(const wchar_t* filename)
	{
		// ���b�V���擾
		auto pMesh = m_mesh;
		// �e�N�X�`��
		if (pMesh)
			pMesh->pAmbientTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// �`��^�C�v
	void SetTranslucentType(eTranslucentType eType) { m_eTranslucentType = eType; }
	// �v���~�e�B�u
	void SetPrimitiveType(ePrimitiveType eType) { if (m_mesh) m_mesh->primitiveType = eType; }

	void SetWorldMatrix(XMFLOAT4X4* pWorld) { m_mtxWorld = pWorld; }


	// Z�o�b�t�@�[
	void SetZBufferFlg(bool bZBuffer)
	{
		m_bZBuffer = bZBuffer;
	}

private:
	MESH* m_mesh;
	MATERIAL m_material;

	// �`��^�C�v
	eTranslucentType m_eTranslucentType;

	// ���b�V���̃v�[��
	static std::map<const std::string, MESH*, std::less<>> m_meshPool;

	bool m_bZBuffer = true;
};


// �r���{�[�h�����_���[
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

		// �����_���[�^�C�v
		m_eRendererType = Renderer::eTransparent;

		// ���b�V���̎擾
		m_mesh = GetBillboardMesh();

	}
	~CBillboardRenderer()
	{

	}

	void Start(void) override
	{
		// ���[���h�}�g���b�N�X���擾
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();

	}

	// �r���{�[�h���b�V���擾
	static MESH* GetBillboardMesh()
	{
		// ���b�V������������Ă���
		if (m_masterMesh.pVertexBuffer	|| m_masterMesh.pIndexBuffer ||
			m_masterMesh.nNumVertex		|| m_masterMesh.nNumIndex)
		{
			return &m_masterMesh;
		}

		// �I�u�W�F�N�g�̒��_�z��𐶐�
		m_masterMesh.nNumVertex = 4;
		VERTEX_3D* pVertexWk = new VERTEX_3D[m_masterMesh.nNumVertex];

		// ���_�z��̒��g�𖄂߂�
		VERTEX_3D* pVtx = pVertexWk;

		// ���_���W�̐ݒ�
		pVtx[0].vtx = XMFLOAT3(-1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[1].vtx = XMFLOAT3(-1.0f / 2, 1.0f / 2, 0.0f);
		pVtx[2].vtx = XMFLOAT3(1.0f / 2, -1.0f / 2, 0.0f);
		pVtx[3].vtx = XMFLOAT3(1.0f / 2, 1.0f / 2, 0.0f);

		// �@���̐ݒ�
		pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = XMFLOAT2(0.0f, 1.0f);
		pVtx[1].tex = XMFLOAT2(0.0f, 0.0f);
		pVtx[2].tex = XMFLOAT2(1.0f, 1.0f);
		pVtx[3].tex = XMFLOAT2(1.0f, 0.0f);

		// �C���f�b�N�X�z��𐶐�
		m_masterMesh.nNumIndex = 4;
		int* pIndexWk = new int[m_masterMesh.nNumIndex];

		// �C���f�b�N�X�z��̒��g�𖄂߂�
		pIndexWk[0] = 0;
		pIndexWk[1] = 1;
		pIndexWk[2] = 2;
		pIndexWk[3] = 3;

		ID3D11Device* pDevice = GetDevice();
		// ���_�o�b�t�@����
		HRESULT hr = MakeMeshVertex(pDevice, &m_masterMesh, pVertexWk, pIndexWk);

		// �ꎞ�z��̉��
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

			// Z�o�b�t�@����
			SetZBuffer(false);

			DrawMeshShadow(pDC, mesh);

			// Z�o�b�t�@
			SetZBuffer(true);
		}
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		const auto& mesh = m_mesh;

		if (mesh)
		{
			// �}�g���b�N�X�쐬
			XMMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;
			// �J�����s��擾
			XMFLOAT4X4& mtxView = CCamera::GetMainCamera()->GetViewMatrix();
			// ���[���h�}�g���b�N�X�̏�����
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

			// �X�P�[���𔽉f
			Vector3 scale = m_pParent->m_pTrans->m_scale;
			mtxScale = XMMatrixScaling(scale->x, scale->y, 1.0f);
			mtxWorld = XMMatrixMultiply(mtxScale, mtxWorld);

			// ��]
			Vector3 rot = m_pParent->m_pTrans->m_rot;
			mtxRot = XMMatrixRotationZ(rot->z);
			mtxWorld = XMMatrixMultiply(mtxRot, mtxWorld);

			// �ړ��𔽉f
			Vector3 pos = m_pParent->m_pTrans->m_pos;
			mtxTranslate = XMMatrixTranslation(pos->x, pos->y, pos->z);
			mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

			// ���[���h�}�g���b�N�X�̐ݒ�
			XMStoreFloat4x4(m_mtxWorld, mtxWorld);

			m_mesh->mtxWorld = *m_mtxWorld;

			m_mesh->mtxTexture = m_mtxTexture;
			m_mesh->pMaterial = &m_material;
			m_mesh->pTexture = m_diffuseTexture;

			// Z�o�b�t�@����
			SetZBuffer(false);

			DrawMesh(pDC, mesh);

			// Z�o�b�t�@
			SetZBuffer(true);
		}
	}


	//--- �}�e���A��
	// �J���[�ύX
	void SetDiffuseColor(XMFLOAT4 color) { m_material.Diffuse = color; }

	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const char* filename)
	{
		// �e�N�X�`���̃Z�b�g
		m_diffuseTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// �x�[�X�J���[�e�N�X�`���̃Z�b�g
	void SetDiffuseTexture(const wchar_t* filename)
	{
		// �e�N�X�`���̃Z�b�g
		m_diffuseTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ���[���h�}�g���b�N�X
	void SetWorldMatrix(XMFLOAT4X4* pWorld) { m_mtxWorld = pWorld; }

private:
	MESH* m_mesh;
	MATERIAL m_material;

	ID3D11ShaderResourceView* m_diffuseTexture;

	static MESH m_masterMesh;
};
