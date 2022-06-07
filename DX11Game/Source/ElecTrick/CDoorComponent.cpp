//======================================================================
//											CDoorComponent.cpp
//	�R���|�[�l���g�쐬�p�̃x�[�X���C�A�E�g
//
//======================================================================
//	author :	AT12A 05 �F�����W�V
//======================================================================
//	�J������
//	2020/11/16	�쐬
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CDoorComponent.h"
#include "Camera.h"

#include "Sound.h"

//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CDoorComponent::Start()
{
	MaxSwitch = 0;
	nMaxPoint = 0;
	CurrentRoot = 0;
	m_nDoorType = 0;
	fDoorSpd = 1.5f;
	vec = Vector3{ 0,0,0 };
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_bIsMove = false;

	// �h�A�̊p�x����ړ��ʒu���擾
	float m_angle = m_pParent->m_pTrans->m_rot->z;
	Vector3 pos = m_pParent->m_pTrans->m_pos;

	if (m_angle == 0)		pos->y += 200;
	else if(m_angle ==  90)	pos->x -= 200;
	else if(m_angle == 180)	pos->y -= 200;
	else if(m_angle == 270)	pos->x += 200;

	Doortype(ALLCHECK, pos);

	// �v���C���[�̎擾
	m_player = CObjectManager::SearchObjectTag("Player");

	m_energyOffTexture = "data/model/Door/Door.png";
	m_energyOnTexture = "data/model/Door/Door_Change.png";

	// �������W
	m_initPos = m_pParent->m_pTrans->m_pos;
}

//===================================
//
//	���X�^�[�g
//
//===================================
void CDoorComponent::Restart()
{
	m_bIsMove = false;
	CurrentRoot = 0;
	m_nDoorType = ALLCHECK;
	m_pParent->m_pTrans->m_pos = m_initPos;
	// �e�N�X�`���̕ύX
	m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOffTexture.c_str());

	// �X�C�b�`
	for (const auto& switch_w : aSwitchCom)
	{
		const auto& switch_s = switch_w.lock();
		if (switch_s)
		{
			switch_s->SetSwitchFlg(false);
			switch_s->ResetResource();
		}
	}
}

//===================================
//
//	�X�V�֐�
//
//===================================
void CDoorComponent::Update()
{

}


//===================================
//
//	��X�V�֐�
//
//===================================
void CDoorComponent::LateUpdate()
{
	// �h�A����
	switch (m_nDoorType)
	{
	case ALLCHECK:
		if (AllCheckSwitch())
		{
			// �J��������
			CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);
			// �v���C���[��~
			if (!m_player.expired())
			{
				m_player.lock()->SetStop(true);
			}

			if (!m_bIsMove)
			{
				m_bIsMove = true;
				// �ړ���
				CSound::PlaySE("DoorMove.wav");
				// �e�N�X�`���̕ύX
				m_pParent->GetComponent<CAssimpRenderer>()->SetDiffuseTexture(m_energyOnTexture.c_str());
			}

			// �x�N�g������
			vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();

			// �ړ��I����
			if (Vector3::Length(vec) <= 1.0f)
			{
				// �ړ��ʂ��[��
				vec = Vector3{ 0,0,0 };
				// �ړ�������Ԃ�
				m_nDoorType = MOVE_END;
				// ����SE
				CSound::PlaySE("DoorStop.wav");
				// �ړ�����~
				CSound::StopSE("DoorMove.wav");


				// �v���C���[
				if (!m_player.expired())
				{
					// �v���C���[�̒�o����
					m_player.lock()->SetStop(false);
					// �J���������I�� �v���C���[�ɖ߂�
					CCamera::GetMainCamera()->SetCameraTarget(m_player.lock()->m_pTrans);
				}
			}

			vec = Vector3::Normalize(vec);

			m_rb.lock()->SetForce(vec * fDoorSpd);
		}
		break;

	case PIECESCHECK:
		if (PiecesCheckSwitch())
		{
			vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();
			if (Vector3::Length(vec) <= 1.0f)
			{
				if (CurrentRoot != nMaxPoint - 1)
				{
					CurrentRoot++;
					vec = DoorMovePoint[CurrentRoot] - m_pParent->m_pTrans->GetPos();
				}
				else
				{
					vec = Vector3{ 0,0,0 };
				}
			}
			vec = Vector3::Normalize(vec);

			m_rb.lock()->SetForce(vec * fDoorSpd);
		}
		break;

		// �ړ��I��
	case MOVE_END:


		break;

	default:
		break;
	}
}


//===================================
//
//	�I���֐�
//
//===================================
void CDoorComponent::End()
{
	for (const auto& sw_com : aSwitchCom)
	{
		const auto& sw_s = sw_com.lock()->m_pParent->tish().lock();
		if (sw_s)
		{
			CObjectManager::DestroyObject(sw_s);
		}
	}
	aSwitchCom.clear();
}


void CDoorComponent::CreateSwitch(Vector3 pos)
{
	MaxSwitch++;

	const auto& swi = CObjectManager::CreateObject<CSwitch>(pos);
	aSwitchCom.push_back(swi->GetComponent<CSwitchComponent>());
}

void CDoorComponent::Doortype(int nType, Vector3 pos)
{
	m_nDoorType = nType;
	nMaxPoint++;
	DoorMovePoint.push_back(pos);
}

bool CDoorComponent::AllCheckSwitch()
{
	if (aSwitchCom.size() == 0) return false;

	bool b = true;

	for (const auto& sw : aSwitchCom)
	{
		const auto& s = sw.lock();
		if (s)
		{
			if (!s->GetSwitchflg())
			{
				b = false;
			}
		}
	}

	return b;
}

int CDoorComponent::PiecesCheckSwitch()
{
	int n = 0;

	for (const auto& sw : aSwitchCom)
	{
		const auto& s = sw.lock();
		if (s)
		{
			if (s->GetSwitchflg())
			{
				n++;
			}
		}
	}

	return n;
}

const auto & CDoorComponent::GetSwitch(unsigned int nElement)
{
	if (MaxSwitch > nElement)
		return aSwitchCom[nElement];
	else {
		aSwitchCom.end();
	}
}

std::vector<std::weak_ptr<CSwitchComponent>>& CDoorComponent::GetSwitchContainer()
{
	return aSwitchCom;
}
