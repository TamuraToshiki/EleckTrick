//==================================================================
//									animator.h
//	�A�j���[�^�[
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/08	�A�j���[�^�[�N���X�쐬
//
//===================================================================
#pragma once

//====== �C���N���[�h�� ======
#include "CComponent.h"
#include "Renderer.h"


//===== �}�N����` =====


//===== �N���X��` =====
class CAnimation2D;

// �����F �ėp�^�ɂ���
class CAnimator2D : public CComponent
{
public:
	CAnimator2D();
	~CAnimator2D();
	void Start()  override;
	void Update() override;

	// �A�j���[�V�������X�g�Ǘ�
	void AddAnimationSelect(const std::string name, int nMaxFrame, ...);
	void AddAnimationDefault(const std::string name, int nMaxFrame);
	//void DestroyAnimation(const std::string name);
	void SetAnimation(const std::string name);
	void SetAnimation(const std::string name, int nSpeed);
	void SetAnimation(const std::string name, int nSpeed, int nSplitX, int nSplitY);

	// �Z�b�g�֐�
	void SetSplit(int nSplitX, int nSplitY) {
		m_nSplitX = nSplitX; m_nSplitY = m_nSplitY; 
		Vector3 texSize;
		texSize->x = 1.0f / nSplitX;
		texSize->y = 1.0f / nSplitY;
		m_renderer.lock()->SetTexSize(texSize);
	}
	void SetAnimationSpeed(int nSpeed) { m_nSpeed = nSpeed; }
	void SetStartFrame(int nStart) { m_nFrame = nStart; }
	void SetStartAnimNo(int nAnimNo) { m_nCurrentAnimNo = nAnimNo; }

	//void SetCurAnimNo(int nAnimNo) { m_nCurrentAnimNo = nAnimNo; }

	// �Q�b�g�֐�
	std::weak_ptr<CAnimation2D> GetCurrentAnimation() { return m_pCurrentAnimation; }

private:
	// �v�[��
	std::map<const std::string, std::shared_ptr<CAnimation2D>> m_animationPool;

	// this
	std::weak_ptr<CAnimator2D> m_this;
	// �����_���[
	std::weak_ptr<CRenderer> m_renderer;

	// �A�j���[�V�����̕ϐ�
	int m_nSpeed;
	int m_nSplitX;
	int m_nSplitY;
	int m_nFrame;
	int m_nCurrentAnimNo;
	std::weak_ptr<CAnimation2D> m_pCurrentAnimation;
};

