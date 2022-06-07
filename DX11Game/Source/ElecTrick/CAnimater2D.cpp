//==================================================================
//									Animator.h
//	�R���W����
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/07/08	�A�j���[�^�[�N���X�̍쐬
//
//===================================================================

//====== �C���N���[�h�� ======
#include "CAnimater2D.h"
#include "CAnimation2D.h"
#include "CSpriteRenderer.h"
#include "InstancingMeshRenderer.h"


//===== �}�N����` =====


//===== �N���X��` =====



//===== �v���g�^�C�v�錾 =====



//========================================
//
//	�R���X�g���N�^
//
//========================================
CAnimator2D::CAnimator2D()
{
	m_nCurrentAnimNo = 0;
	m_nFrame = 0;
	m_nSpeed = 0;
	m_nSplitX = 0;
	m_nSplitY = 0;
}


//========================================
//
//	�f�X�g���N�^
//
//========================================
CAnimator2D::~CAnimator2D()
{
	m_animationPool.clear();
}

//========================================
//
//	�X�V
//
//========================================
void CAnimator2D::Start()
{
	// �����_���[�擾
	const auto& bill = m_pParent->GetComponent<CBillboardRenderer>();
	m_renderer = bill;
	if (!bill)
	{
		// �X�v���C�g
		const auto& spr = m_pParent->GetComponent<CSpriteRenderer>();
		m_renderer = spr;
		if (!spr)
		{
			// ���b�V��
			const auto& mesh = m_pParent->GetComponent<CMeshRenderer>();
			m_renderer = mesh;
			if (!mesh)
			{
				// �A�V���v
				const auto& assimp = m_pParent->GetComponent<CAssimpRenderer>();
				m_renderer = assimp;
				if (!assimp)
				{
					// �C���X�^���X�V���O
					const auto& instancing = m_pParent->GetComponent<InstancingMeshRenderer>();
					m_renderer = instancing;
				}
			}
		}
	}

	// ���g
	m_this = m_pParent->GetComponent<CAnimator2D>();
}


//========================================
//
//	�X�V
//
//========================================
void CAnimator2D::Update()
{
	// ���݂̃A�j���[�V�������擾
	const auto& pAnimation = m_pCurrentAnimation.lock();

	// ��Ȃ��΂�
	if (!pAnimation) return;

	Vector3 texPos;
	Vector3 texSize = m_renderer.lock()->GetTexSize();

	// �A�j���[�V�����̃R�}�̍X�V
	if (0 == m_nFrame % m_nSpeed)
	{
		// �e�N�X�`�����W�̍X�V
		texPos->x = texSize->x * (pAnimation->GetAnimNoArray()[m_nCurrentAnimNo] % m_nSplitX);
		texPos->y = texSize->y * (pAnimation->GetAnimNoArray()[m_nCurrentAnimNo] / m_nSplitX);
		m_renderer.lock()->SetTexPos(texPos);
		m_renderer.lock()->UpdateTexMatrix();

		// ���݂̃R�}�ԍ��̍X�V
		m_nCurrentAnimNo++;
		m_nFrame = 0;
	}

	//  �t���[���̍X�V
	m_nFrame++;

	// �A�j���[�V�����̏I��(�ŏI�t���[��)
	if (m_nCurrentAnimNo >= pAnimation->GetArraySize() && m_nFrame >= m_nSpeed)
	{
		// �A�j���[�V�����I���̃R�[���o�b�N�֐�
		const auto& list = m_pParent->GetComponentList();
		std::for_each(list.begin(), list.end(), [this](const auto& com)
		{
			com->OnAnimationExit2D(m_this.lock());
		});

		m_nCurrentAnimNo = 0;
	}



}


//========================================
//
//	�A�j���[�^�[�ɃA�j���[�V�����̃Z�b�g
//
//========================================
void CAnimator2D::SetAnimation(std::string name)
{
	// �A�j���[�V�����̌���
	const auto& itr = m_animationPool.find(name);

	// ���݂�����
	if (m_animationPool.end() != itr)
	{
		const auto& curAnim = m_pCurrentAnimation.lock();
		// �A�j���[�V�������ς���ĂȂ��Ȃ�
		if (curAnim == itr->second) return;

		m_pCurrentAnimation = itr->second;
		m_nFrame = 0;
		m_nCurrentAnimNo = 0;
		return;
	}
}

//========================================
//
//	�A�j���[�^�[�ɃA�j���[�V�����̃Z�b�g
//
//========================================
void CAnimator2D::SetAnimation(const std::string name, int nSpeed)
{
	SetAnimation(name);
	if(nSpeed > 0)
		m_nSpeed = nSpeed;
}

//========================================
//
//	�A�j���[�^�[�ɃA�j���[�V�����̃Z�b�g
//
//========================================
void CAnimator2D::SetAnimation(const std::string name, int nSpeed, int nSplitX, int nSplitY)
{
	SetAnimation(name, nSpeed);
	m_nSplitX = nSplitX;
	m_nSplitY = nSplitY;

	Vector3 texSize;
	texSize->x = 1.0f / nSplitX;
	texSize->y = 1.0f / nSplitY;

	m_renderer.lock()->SetTexSize(texSize);
}


//========================================
//
//	�A�j���[�V�����̒ǉ�
//
//========================================
void CAnimator2D::AddAnimationSelect(const std::string name, int nMaxFrame, ...)
{
	// �ψ������X�g
	va_list args;
	int nCount;

	// ���I�ɃA�j���[�V�����N���X�̊m��
	CAnimation2D *pAnim = new CAnimation2D;

	// �ψ������X�g�쐬
	va_start(args, nMaxFrame);

	// �ψ����̐����v�Z
	nCount = nMaxFrame;

	// �ψ������������m��
	pAnim->m_pAnimNoArray = new int[nCount];

	// �z���nAnimNo�����ԂɊi�[
	for (int i = 0; i < nCount; i++)
	{
		pAnim->m_pAnimNoArray[i] = va_arg(args, int);
	}

	// �ψ������X�g�I��
	va_end(args);

	// �A�j���[�V�����̖��O�̊i�[
	pAnim->m_name = name;
	// �z��(�A�j���[�V�����̃R�})�̐����i�[
	pAnim->m_nArraySize = nCount;

	// �Ō�ɃA�j���[�^�[�̃��X�g�ɐV����������A�j���[�V�������i�[
	m_animationPool.emplace(name, pAnim);

}

//========================================
//
//	���� (�����ŃR�}�ԍ��U�蕪��)
//
//========================================
void CAnimator2D::AddAnimationDefault(const std::string name, int nMaxFrame)
{
	// ���I�ɃA�j���[�V�����N���X�̊m��
	CAnimation2D *pAnim = new CAnimation2D;

	// �������m��
	pAnim->m_pAnimNoArray = new int[nMaxFrame];

	// �z���nAnimNo�����ԂɊi�[
	for (int i = 0; i < (int)nMaxFrame; i++)
	{
		pAnim->m_pAnimNoArray[i] = i;
	}

	// �A�j���[�V�����̖��O�̊i�[
	pAnim->m_name = name;
	// �z��(�A�j���[�V�����̃R�})�̐����i�[
	pAnim->m_nArraySize = nMaxFrame;

	// �Ō�ɃA�j���[�^�[�̃��X�g�ɐV����������A�j���[�V�������i�[
	m_animationPool.emplace(name, pAnim);

}