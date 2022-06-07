//======================================================================
//											CSpriteRenderer.h
//	�X�v���C�g�����_���[
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//
//	2020/12/06	�쐬
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "Renderer.h"
#include "polygon.h"


//===== �\���̒�` =====


//===== �N���X��` =====
class CSpriteRenderer : public CRenderer
{
public:
	// ���C���[
	enum ESpriteLayer
	{
		// �擪
		eTop,

		// �t�F�[�h
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

		// �w�i
		eBG3 = (int)VIEW_FAR_Z + 1,
		eBG2,
		eBG1,

		// �Ō�
		eBottom,

		eMaxLayer,
	};

public:
	CSpriteRenderer()
	{
		m_fLayer = eDefault;
		m_pTexture = nullptr;
		// �F
		m_color = Vector3{ 1,1,1 };
		m_fAlpha = 1.0f;

		// �u�����h�X�e�[�g
		m_eState = BS_ALPHABLEND;

		// �J�����O�I�t
		m_bCull = false;

		// �����_���[�^�C�v
		m_eRendererType = Renderer::eUI;
	}
	~CSpriteRenderer(){}

	// �쐬���ɌĂ΂�܂�
	void Start() override
	{
		// ���[���h�}�g���b�N�X���擾
		m_mtxWorld = m_pParent->m_pTrans->GetWorldMatrix();
	}

	// ���C���[���i�[
	void LateUpdate() override
	{
		// ����Ȃ�

	}

	void EarlyDraw(ID3D11DeviceContext* pDC) override
	{
		// �e�`��Ȃ�
	}

	void LateDraw(ID3D11DeviceContext* pDC) override
	{
		// ����
		const auto& trans = m_pParent->m_pTrans;
		if (!trans) return;

		// Z�o�b�t�@����
		SetZBuffer(false);

		// ���W
		SetPolygonPos(trans->m_pos->x, trans->m_pos->y);
		// �p�x
		SetPolygonAngle(trans->m_rot->z);
		// �T�C�Y
		SetPolygonSize(trans->m_scale->x, trans->m_scale->y);
		// �J���[
		SetPolygonColor(m_color->x, m_color->y, m_color->z);
		// �A���t�@
		SetPolygonAlpha(m_fAlpha);
		// �e�N�X�`�����W
		SetPolygonUV(m_texPos->x, m_texPos->y);
		// �e�N�X�`���T�C�Y
		SetPolygonFrameSize(m_texSize->x, m_texSize->y);
		// �e�N�X�`��
		SetPolygonTexture(m_pTexture);

		// �`��
		DrawPolygon(pDC);

		// Z�o�b�t�@�L��
		SetZBuffer(true);
	}

	//--- �}�e���A��
	// �J���[�ύX
	void SetColor(Vector3 color) { m_color = color; }
	void SetColor(float r, float g, float b) { 
		m_color->x = r; 
		m_color->y = g;
		m_color->z = b;
	}
	void SetAlpha(float alpha) { m_fAlpha = alpha; }

	// �e�N�X�`���̃Z�b�g
	void SetTexture(const char* filename)
	{
		// �e�N�X�`���̃Z�b�g
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);
	}
	// �e�N�X�`���̃Z�b�g
	void SetTexture(const wchar_t* filename)
	{
		// �e�N�X�`���̃Z�b�g
		m_pTexture = CTexture::GetTexture(GetDevice(), filename);
	}

	// ���C���[
	void SetLayer(ESpriteLayer eLayer) { m_fLayer = (float)eLayer; }
	void SetLayer(int nLayer) { m_fLayer = (float)nLayer; }


	// �J���[�擾
	Vector3 GetColor() { return m_color; }
	// �A���t�@�擾
	float GetAlpha() { return m_fAlpha; }
	// ���C���[�擾
	int GetLayer() { return (int)m_fLayer; }

private:
	// �e�N�X�`��
	ID3D11ShaderResourceView* m_pTexture;
	// �J���[
	Vector3 m_color;
	// �A���t�@
	float m_fAlpha;
};


//===== �v���g�^�C�v�錾 =====


