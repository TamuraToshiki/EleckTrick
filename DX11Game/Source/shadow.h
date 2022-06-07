
// �ۉe

#pragma once

#include "main.h"
#include <list>
#include "mesh.h"


class CShadow
{
public:
	CShadow();
	~CShadow();

	static HRESULT InitShadow(void);
	static void UninitShadow(void);
	void UpdateShadow(void);
	void DrawShadow(void);
	CShadow* CreateShadow(XMFLOAT3 pos, float radius);
	void MoveShadow(XMFLOAT3 pos);
	void ReleaseShadow();

	struct SShadowData
	{
		int nState;
		XMFLOAT3 pos;
		float radius;
	};

private:
	static ID3D11ShaderResourceView*	m_pTexture;				// �e�N�X�`��
	static MESH							m_mesh;					// �\����
	static MATERIAL						m_material;				// �}�e���A��



	static std::list<SShadowData> m_shadowDataList;
};

