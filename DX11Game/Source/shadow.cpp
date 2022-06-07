//=============================================================================
//
// �e���� [shadow.cpp]
// Author : USAMI KOSHI
//
//=============================================================================
#include "shadow.h"
#include "Texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	L"data/texture/field002.jpg"	// �e�N�X�`���t�@�C����

#define M_DIFFUSE			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_AMBIENT			XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE			XMFLOAT4(0.0f,0.0f,0.0f,1.0f)


std::list<CShadow::SShadowData>	CShadow::m_shadowDataList;				// ���X�g
ID3D11ShaderResourceView*		CShadow::m_pTexture;				// �e�N�X�`��
MESH							CShadow::m_mesh;					// �\����
MATERIAL						CShadow::m_material;				// �}�e���A��



CShadow::CShadow()
{
}

CShadow::~CShadow()
{
}



HRESULT CShadow::InitShadow(void)
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	m_mesh.pTexture = CTexture::GetTexture(pDevice, TEXTURE_FILENAME);
	if (FAILED(hr))
		return hr;
	XMStoreFloat4x4(&m_mesh.mtxTexture, XMMatrixIdentity());

	// ���_���̍쐬
	m_mesh.nNumVertex = 4;
	VERTEX_3D* pVerWk = new VERTEX_3D[m_mesh.nNumVertex];

	pVerWk[0].vtx = { -0.5f, 0.0f,  0.5f };
	pVerWk[1].vtx = {  0.5f, 0.0f,  0.5f };
	pVerWk[2].vtx = { -0.5f, 0.0f, -0.5f };
	pVerWk[3].vtx = {  0.5f, 0.0f, -0.5f };

	pVerWk[0].diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	pVerWk[1].diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	pVerWk[2].diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	pVerWk[3].diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	pVerWk[0].nor = { 0.0f, 1.0f, 0.0f };
	pVerWk[1].nor = { 0.0f, 1.0f, 0.0f };
	pVerWk[2].nor = { 0.0f, 1.0f, 0.0f };
	pVerWk[3].nor = { 0.0f, 1.0f, 0.0f };

	pVerWk[0].tex = { 0.0f, 0.0f };
	pVerWk[1].tex = { 1.0f, 0.0f };
	pVerWk[2].tex = { 0.0f, 1.0f };
	pVerWk[3].tex = { 1.0f, 1.0f };

	m_mesh.nNumIndex = 4;
	int* pIndex = new int[m_mesh.nNumIndex];



	return hr;
}
void CShadow::UninitShadow(void)
{

}
void CShadow::UpdateShadow(void)
{

}
void CShadow::DrawShadow(void)
{

}
CShadow* CShadow::CreateShadow(XMFLOAT3 pos, float radius)
{

	return nullptr;
}
void CShadow::MoveShadow(XMFLOAT3 pos)
{

}
void CShadow::ReleaseShadow()
{

}