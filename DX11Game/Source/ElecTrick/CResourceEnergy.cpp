//======================================================================
//											CResourceEnergy.cpp
//	�G�l���M�[���\�[�X
//
//======================================================================
//	author :	AT12A 15 �匴����
//======================================================================
//	�J������
//	2020/12/11	�쐬
//				�w�b�_�̊֐��Ȃǂ��������
//======================================================================


//===== �C���N���[�h�� =====
#include "CResourceEnergy.h"

//===== �}�N����` =====

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====

//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CResourceEnergy::Start()
{
	m_fResource = 0.0f;
	m_enLevel = ZERO;
	m_bFlag = false;
	m_bEnergyUse = false;
}

//===================================
//
//	�X�V�֐�
//
//===================================
void CResourceEnergy::Update()
{
	AddEnergy();
	SubEnergy();
	CheckUse();		// ���\�[�X���g���Ă���̂��̔��f
	DecideCharged();
	DecideLevel();
}

//===================================
//
//	��X�V�֐�
//
//===================================
void CResourceEnergy::LateUpdate()
{
	CheckStop();
}


//===================================
//
//	�I���֐�
//
//===================================
void CResourceEnergy::End()
{

}

//--------------------------------------------------
//	�G�l���M�[�ʂ��擾
//--------------------------------------------------
float CResourceEnergy::GetResource(void)
{
	DecideLevel();
	return m_fResource;
}

//--------------------------------------------------
//	�G�l���M�[���x�����擾
//--------------------------------------------------
int CResourceEnergy::GetLevel(void)
{
	return m_enLevel;
}

//--------------------------------------------------
//	�ѓd��Ԃ��擾
//--------------------------------------------------
bool CResourceEnergy::GetFlag(void)
{
	return m_bFlag;
}

//--------------------------------------------------
//	�G�l���M�[���x���𔻕�
//--------------------------------------------------
void CResourceEnergy::DecideLevel(void)
{

	if (m_fResource < 10)	m_enLevel = ZERO;
	else if (m_fResource < 20)	m_enLevel = ONE;
	else if (m_fResource < 30)	m_enLevel = TWO;
	else if (m_fResource <= 30)	m_enLevel = THREE;

	//if (m_fResource < 20)	m_enLevel = ZERO;
	//if (m_fResource > 20)	m_enLevel = ONE;
	//if (m_fResource > 40)	m_enLevel = TWO;
	//if (m_fResource > 60)	m_enLevel = THREE;

}

//--------------------------------------------------
//  �G�l���M�[�̃Z�b�g	
//--------------------------------------------------
void CResourceEnergy::SetEnergy(float setAmount)
{
	m_fResource = setAmount;
}

//--------------------------------------------------
//  �G�l���M�[�̉��Z	
//--------------------------------------------------
void CResourceEnergy::AddEnergy(float addAmount)
{
	m_fResource += addAmount;
	if (m_fResource > m_fMaxResource)
	{
		m_fResource = m_fMaxResource;
	}
}

//--------------------------------------------------
//	�G�l���M�[�̌��Z
//--------------------------------------------------
void CResourceEnergy::SubEnergy(float subAmount)
{
	m_fResource -= subAmount;
	if (m_fResource < m_fMinResource)
	{
		m_fResource = m_fMinResource;
	}

}

//--------------------------------------------------
//	�ѓd����
//	���ʒl�͌�
//--------------------------------------------------
void CResourceEnergy::DecideCharged(void)
{
	if (m_enLevel > 0)	m_bFlag = true;
	else m_bFlag = false;
}

//--------------------------------------------------------------------//
//	�G�l���M�[�̎g�p����
//--------------------------------------------------------------------//
void CResourceEnergy::CheckUse(void)
{
	if (!m_bEnergyUse) // �G�l���M�[���g���Ă��Ȃ����̔��f
	{
		if (m_fResource <= m_nEnergyStop)	// �G�l���M�[���\�[�X���X�g�b�p�[�������悤�Ȃ�
		{
			m_fResource = m_nEnergyStop;	// ���\�[�X���X�g�b�p�[�̒l�ɂ���
		}
	}
}

//--------------------------------------------------------------------//
//	�G�l���M�[�̃X�g�b�v����
//--------------------------------------------------------------------//
void CResourceEnergy::CheckStop(void)
{
	if (m_fResource >= m_fMaxResource)
		m_nEnergyStop = m_fMaxResource;
	// Update�ɓ����ƁA�X�g�b�p�[�����ʂ𔭊����Ȃ��Ȃ邽��LateUpdate��
	m_nEnergyStop = m_fResource / 10;	// �����_�ȉ���؂�̂ĂĐ����^�ɑ��
	m_nEnergyStop *= 10;				// 10�̒P�ʂɂ��邽��
}