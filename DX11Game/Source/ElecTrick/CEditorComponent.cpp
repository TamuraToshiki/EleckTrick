//======================================================================
//											CEditorComponent.cpp
//	�}�b�v�G�f�B�^�̃R���|�[�l���g
//
//======================================================================
//	author :	AT12A 12 �I��
//					  05 �F����
//======================================================================
//	�J������
//	2020/12/03	�쐬
//				�I����������p��/�匴
//				�ꉞ�u���b�N�̐����͂ł��邪
//				�p�ӂ��ꂽ�͈͓��Ƀf�[�^�����܂�Ȃ����
//				�G���[���o��B���I�z��ɗv�ύX�B 
//	2020/12/08	�F���������p��
//	2020/12/09	�X�C�b�`�ƃ��t�g�|�C���g��u����悤��
//
//======================================================================


//===== �C���N���[�h�� =====
#include "CEditorComponent.h"
#include "CBillboardObject.h"
#include "CObjectManager.h"
#include "CSceneManager.h"
#include "input.h"
#include "Camera.h"

#include "CBlock.h"
#include "CDoor.h"
#include "CDoorComponent.h"
#include "CSwitchComponent.h"
#include "CLift.h"
#include "CLiftComponent.h"
#include "CEndElevator.h"
#include "CStartElevator.h"
#include "CSecurityRobo.h"
#include "CCheckPoint.h"
#include "CDamageTrapObj.h"
#include "CDamageTrapObjComponent.h"
#include "CBrokenObject.h"
#include "CDropObject.h"
#include "CFallObject.h"
#include "CMagnetBlock.h"
#include "CBlockComponent.h"

#include "CDropComponent.h"
#include "CFallComponent.h"
#include "CBrokenComponent.h"
#include "CMagnetBlockComponent.h"
#include "CSecurityRoboComponent.h"


#include <direct.h>

//===== �}�N����` =====
#define CHIP_SIZE   (100.0f)
#define CHIP_HALF	(50.0f)	

//#define FILE_PATH "data/map/"
#define MAP_PATH "/map.csv"

#define FILE_PATH [&](unsigned int n){					\
	char szName[256];									\
	sprintf_s(szName, "%s%d", "data/map/stage", n);		\
	return std::string(szName);}


#define MAX_STAGE (19)	//�ŏI�}�b�v

//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====
unsigned int CEditorComponent::g_nCuurentStage = 0;



//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CEditorComponent::Start()
{
	//m_aMapData.resize(MAP_SIZE_X * MAP_SIZE_Y,0);
	//m_aMapData
	m_cursorPos_X = m_pParent->m_pTrans->m_pos->x;
	m_cursorPos_Y = m_pParent->m_pTrans->m_pos->y;
	m_chipsize = CHIP_SIZE;
	m_type = BLOCK;

	m_mode = eDefault;							// �G�f�B�b�g���[�h

	m_nHeight = MAP_SIZE_Y;
	m_nWidth = MAP_SIZE_X;

	m_drawOffset = Vector3{ 0,0,0 };

	// �}�b�v���[�h
	//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
	Load(FILE_PATH(g_nCuurentStage));
}


//===================================
//
//	�X�V�֐�
//
//===================================
void CEditorComponent::Update()
{
	// �I�u�W�F�N�g�̐���
	if (GetKeyTrigger(VK_1))
	{
		Create();
	}

	// �I�u�W�F�N�g�̔j��
	if (GetKeyTrigger(VK_2))
	{
		Destroy();
	}

	// �I�u�W�F�N�g�̐؂�ւ�
	if (GetKeyTrigger(VK_K) || GetKeyTrigger(VK_3))
	{
		CEditorComponent::ChangeObj();
	}

	// �Z�[�u
	if (GetKeyTrigger(VK_8))
	{
		//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
		Save(FILE_PATH(g_nCuurentStage));
	}

	// ���[�h
	if (GetKeyTrigger(VK_9))
	{
		//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
		Load(FILE_PATH(g_nCuurentStage));
	}

	// �}�b�v�f�[�^�̊J��
	if (GetKeyTrigger(VK_0))
	{
		ClearMapData();
	}

	// �J�[�\���ړ�
	Move();

	// ���W����
	m_pParent->m_pTrans->m_pos = Vector3{ (float)m_cursorPos_X, (float)m_cursorPos_Y, 0.0f } + m_drawOffset;
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);


	// �X�e�[�W���̉���
	if (GetKeyTrigger(VK_5))
	{
		g_nCuurentStage--;
	}

	if (GetKeyTrigger(VK_6))
	{
		g_nCuurentStage++;
	}
}


//===================================
//
//	��X�V�֐�
//
//===================================
void CEditorComponent::LateUpdate()
{
	PrintDebugProc("UP   :��\n");
	PrintDebugProc("RIGHT:з�\n");
	PrintDebugProc("LEFT :����\n");
	PrintDebugProc("DOWN :��\n");

	PrintDebugProc("1     :����\n");
	PrintDebugProc("2     :���ޮ\n");
	PrintDebugProc("P     :���خ�\n");
	PrintDebugProc("K,3   :�ض�\n");

	PrintDebugProc("   1  :BLOCK\n");
	PrintDebugProc("   2  :DOOR\n");
	PrintDebugProc("   3  :LiFT\n");
	PrintDebugProc("   4  :START_EV\n");
	PrintDebugProc("   5  :END_EV\n");
	PrintDebugProc("   6  :ROBO\n");
	PrintDebugProc("   7  :CheckPoint\n");
	PrintDebugProc("   8  :DAMEGE_TRAP\n");
	PrintDebugProc("   9  :MAGNET\n");
	PrintDebugProc("  10  :BROKEN\n");
	PrintDebugProc("  11  :DROP\n");
	PrintDebugProc("  12  :FALL\n");
	PrintDebugProc("��ݻ޲:%d\n", m_type);

	PrintDebugProc("Key8:����\n");
	PrintDebugProc("Key9:۰��\n");
	PrintDebugProc("Key0:��γ\n");

	PrintDebugProc("\nStage  :%u\n", g_nCuurentStage);
	PrintDebugProc("Stage++:6�\n");
	PrintDebugProc("Stage--:5\n");
}


//===================================
//
//	�I���֐�
//
//===================================
void CEditorComponent::End()
{
	ClearMapData();

	// �X�e�[�W������
	g_nCuurentStage++;
}


//===================================
//
//	�I�u�W�F�N�g�̐���
//
//===================================
void CEditorComponent::Create()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// ���ɐ�������Ă�����
	//if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	
	// ���[�h�Ő��������𕪊�
	switch (m_mode)
	{
	case CEditorComponent::eDefault:
		DefaultCreateMode();
		break;
	case CEditorComponent::eDoorSwitch:
		DoorSwitchMode();
		break;
	case CEditorComponent::eLiftPoint:
		LiftPointMode();
		break;
	case CEditorComponent::eDamegeTrap:
		DamegeTrapMode();
		break;
	}
}


// �f�t�H���g�������[�h
void CEditorComponent::DefaultCreateMode()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// ���ɐ�������Ă�����
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	//�z��֊i�[
	switch (m_type)
	{
	case BLOCK:
	{
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BLOCK;
		const auto& block = CObjectManager::CreateObject<CBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak = block;

		// �e�N�X�`���̎w��
		int texNo = (2 - hnum % 3) * 3 + wnum % 3;
		block->GetComponent<CBlockComponent>()->SetTexture(texNo);
	}
		break;
	case DOOR:
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DOOR;
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CDoor>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		{
			// ���[�h�ؑ�
			m_mode = eDoorSwitch;
			// ���f���f�[�^�ؑ�
			const auto& render = m_pParent->GetComponent<CAssimpRenderer>();
			render->ModelLoad("data/model/Electrode/Electrode.fbx");
			m_drawOffset = Vector3{ 0,0,0 };
		}

		break;
	case LIFT:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CLift>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = LIFT;

		{
			// ���[�h�ؑ�
			m_mode = eLiftPoint;
			// ���f���f�[�^�ؑ�
			const auto& render = m_pParent->GetComponent<CAssimpRenderer>();
			render->ModelLoad("data/model/Lift_Line/Gear.fbx");
			m_drawOffset = Vector3{ 0,0,0 };
		}

		break;
	case DAMAGE_TRAP:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CDamageTrapObj>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DAMAGE_TRAP;

		{
			// ���[�h�ؑ�
			m_mode = eDamegeTrap;
			// ���f���f�[�^�ؑ�
			const auto& render = m_pParent->GetComponent<CAssimpRenderer>();
			render->ModelLoad("data/model/DamageTrap/DamageTrap.fbx");
			m_drawOffset = Vector3{ 0,0,0 };
		}

		break;
	case START_EV:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CStartElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = START_EV;
		break;
	case END_EV:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CEndElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = END_EV;
		break;
	case ROBO:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CSecurityRobo>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = ROBO;
		break;
	case CHECK_POINT:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CCheckPoint>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = CHECK_POINT;
		break;
	case MAGNET:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CMagnetBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = MAGNET;
		break;
		// �j��u���b�N
	case BROKEN:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CBrokenObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BROKEN;
		break;
	case DROP:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CDropObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DROP;
		break;
	case FALL:
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CFallObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = FALL;
		break;
	}

	// �����I�u�W�F�N�g���o�b�N�A�b�v
	m_objBackUp = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak;
}

// �h�A�X�C�b�`�������[�h
void CEditorComponent::DoorSwitchMode()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// ���ɐ�������Ă�����
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	// �h�A�R���|�[�l���g
	const auto& door = m_objBackUp.lock()->GetComponent<CDoorComponent>();
	if (!door) return;

	// �X�C�b�`�̐���
	door->CreateSwitch({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });

}

// ���t�g�|�C���g�������[�h
void CEditorComponent::LiftPointMode()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// ���ɐ�������Ă�����
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType == 0 || 
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType == LIFT)
	{
		// ���t�g�R���|�[�l���g
		const auto& lift = m_objBackUp.lock()->GetComponent<CLiftComponent>();
		if (!lift) return;

		// ���t�g�|�C���g�̐���
		lift->CreateLiftPoint({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
	}
}

// �_���[�W�g���b�v�������[�h
void CEditorComponent::DamegeTrapMode()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// ���ɐ�������Ă�����
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType == 0 ||
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType == DAMAGE_TRAP)
	{
		// ���t�g�R���|�[�l���g
		const auto& trap = m_objBackUp.lock()->GetComponent<CDamageTrapObjComponent>();
		if (!trap) return;

		// ���t�g�|�C���g�̐���
		trap->CreateTrapPoint({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
	}
}

//===================================
//
//	�I�u�W�F�N�g�̔j��
//
//===================================
void CEditorComponent::Destroy()
{
	//�N���b�N�������W�̋L�^�p�ϐ�
	int imgx = 0;
	int imgy = 0;
	//�N���b�N���ꂽ�z����L�^����ϐ�
	int hnum = 0;
	int wnum = 0;

	//�����ꂽ���W���L�^
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//������`�b�v�T�C�Y�Ŋ��鎖�ŉ����ꂽ�z����m�F
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// �z��͈͊O
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	//������0�ɕς���
	m_aMapData[hnum * m_nWidth + wnum].m_nObjType = 0;

	// �I�u�W�F�N�g���폜
	const auto& obj = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock();
	if (obj)
	{
		if (obj == m_objBackUp.lock()) m_mode = eDefault;

		CObjectManager::DestroyObject(obj);
	}
}


//===================================
//
//	�Z�[�u
//
//===================================
void CEditorComponent::Save(std::string filepath)
{
	if (_mkdir(filepath.c_str()) == 0) {
		// ����

	}
	else {
		// ���s

	}

	FILE *fp;
	fopen_s(&fp, (filepath + MAP_PATH).c_str(), "wt");

	int nDwCnt = 0;
	int nLpCnt = 0;
	int nDtCnt = 0;

	if (fp)
	{
		int nNum;
		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				nNum = m_aMapData[i * m_nWidth + j].m_nObjType;

				fprintf_s(fp, "%d,", nNum);
				if (j >= m_nWidth - 1)
				{
					fprintf_s(fp, "\n");
				}

				// �h�A�X�C�b�`�⃊�t�g�|�C���g��������
				if (nNum == DOOR)
				{
					// ��
					nDwCnt++;

					const auto& door_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CDoorComponent>();
					const auto& aswitch = door_s->GetSwitchContainer();

					// �t�@�C���l�[������
					char szName[256];
					std::string path = filepath + "/Switch";
					sprintf_s(szName, "%s%d.txt", path.c_str(), nDwCnt);

					FILE* fpSwitch;
					fopen_s(&fpSwitch, szName, "wt");

					for (const auto& sw : aswitch)
					{
						Vector3 pos = sw.lock()->m_pParent->m_pTrans->m_pos;
						fprintf_s(fpSwitch, "%f/%f/%f\n", pos->x, pos->y, pos->z);
					}

					fclose(fpSwitch);
				}
				else if (nNum == LIFT)
				{
					// ��
					nLpCnt++;

					const auto& lift_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CLiftComponent>();
					const auto& liftpoint = lift_s->GetLiftPoint();

					// �t�@�C���l�[������
					char szName[256];
					std::string path = filepath + "/LiftPoint";
					sprintf_s(szName, "%s%d.txt", path.c_str(), nLpCnt);

					FILE* fpLift;
					fopen_s(&fpLift, szName, "wt");

					for (const auto& lp : liftpoint)
					{
						Vector3 pos = lp.lock()->m_pTrans->m_pos;
						fprintf_s(fpLift, "%f/%f/%f\n", pos->x, pos->y, pos->z);
					}

					fclose(fpLift);
				}
				else if (nNum == DAMAGE_TRAP)
				{
					// ��
					nDtCnt++;

					const auto& trap_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CDamageTrapObjComponent>();
					const auto& trappoint = trap_s->GetTrapPoint();

					// �t�@�C���l�[������
					char szName[256];
					std::string path = filepath + "/DamageTrap";
					sprintf_s(szName, "%s%d.txt", path.c_str(), nDtCnt);

					FILE* fpTrap;
					fopen_s(&fpTrap, szName, "wt");

					for (int i = 0; i < trappoint.size(); ++i)
					{
						if (i == 0) continue;

						Vector3 pos = trappoint[i].lock()->m_pTrans->m_pos;
						fprintf_s(fpTrap, "%f/%f/%f\n", pos->x, pos->y, pos->z);
					}

					fclose(fpTrap);
				}

			}
		}
		fclose(fp);
	}
	else
	{
		// �G���[

	}
}


//===================================
//
//	���[�h
//
//===================================
void CEditorComponent::Load(std::string filepath)
{
	// �}�b�v�f�[�^�̊J��
	ClearMapData();

	FILE *fp;

	int nDwCnt = 0;
	int nLpCnt = 0;
	int nDtCnt = 0;

	// �t�@�C���I�[�v��
	fopen_s(&fp, (filepath + MAP_PATH).c_str(), "rt");

	if (fp)
	{
		// �}�b�v�̓ǂݍ���
		fseek(fp, 0, SEEK_SET);
		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				fscanf_s(fp, "%d,", &m_aMapData[i * m_nWidth + j]);
			}
		}

		//�}�b�v�`�b�v����
		for (int hnum = 0; hnum < m_nHeight; hnum++) {
			for (int wnum = 0; wnum < m_nWidth; wnum++) {
				//�z��v�f��1�ł��鏊�ɉ摜��\������B
				switch (m_aMapData[hnum * m_nWidth + wnum].m_nObjType)
				{
				case BLOCK:
				{
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BLOCK;
					const auto& block = CObjectManager::CreateObject<CBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak = block;

					// �e�N�X�`���̎w��
					int texNo = (2 - hnum % 3) * 3 + wnum % 3;
					block->GetComponent<CBlockComponent>()->SetTexture(texNo);

					// �����œ����蔻��̍œK����������
					// ���肪�S�ău���b�N�������瓖���蔻��Ȃ���
					if (wnum > 0)
					{
						if (m_aMapData[hnum * m_nWidth + (wnum - 1)].m_nObjType != BLOCK) continue;
					}
					if (wnum + 1 < m_nWidth)
					{
						if (m_aMapData[hnum * m_nWidth + (wnum + 1)].m_nObjType != BLOCK) continue;
					}
					if (hnum > 0)
					{
						if (m_aMapData[(hnum - 1) * m_nWidth + wnum].m_nObjType != BLOCK) continue;
					}
					if (hnum + 1 < m_nHeight)
					{
						if (m_aMapData[(hnum + 1) * m_nWidth + wnum].m_nObjType != BLOCK) continue;
					}

					{
						// �R���|�[�l���g����菜��
						const auto& obj = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock();
						obj->RemoveComponet<CCollision2D>();
						obj->RemoveComponet<CRigidbody2D>();
					}
				}
					break;
				case DOOR:
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DOOR;
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CDoor>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					{
						// ��
						nDwCnt++;
						// �擾
						const auto& door_s = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CDoorComponent>();
						// �X�C�b�`�̔z�u
						// �t�@�C���l�[������
						char szName[256];
						std::string path = filepath + "/Switch";
						sprintf_s(szName, "%s%d.txt", path.c_str(), nDwCnt);
						FILE* fpSwitch;
						fopen_s(&fpSwitch, szName, "rt");
						if (fpSwitch)
						{
							Vector3 pos;
							while (true)
							{
								if (fscanf_s(fpSwitch, "%f/%f/%f", &pos->x, &pos->y, &pos->z) == EOF)
								{
									break;
								}
								pos->z = 0;
								door_s->CreateSwitch(pos);
							}
							fclose(fpSwitch);
						}
					}

					break;
				case LIFT:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CLift>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = LIFT;

					{
						// ��
						nLpCnt++;
						// �擾
						const auto& lift = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CLiftComponent>();
						// �X�C�b�`�̔z�u
						// �t�@�C���l�[������
						char szName[256];
						std::string path = filepath + "/LiftPoint";
						sprintf_s(szName, "%s%d.txt", path.c_str(), nLpCnt);
						FILE* fpLift;
						fopen_s(&fpLift, szName, "rt");
						if (fpLift)
						{
							Vector3 pos;
							while (true)
							{
								if (fscanf_s(fpLift, "%f/%f/%f", &pos->x, &pos->y, &pos->z) == EOF)
								{
									break;
								}
								lift->CreateLiftPoint(pos);
							}
							fclose(fpLift);
						}
					}

					break;

				case DAMAGE_TRAP:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CDamageTrapObj>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DAMAGE_TRAP;

					{
						// ��
						nDtCnt++;
						// �擾
						const auto& trap = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CDamageTrapObjComponent>();
						// �X�C�b�`�̔z�u
						// �t�@�C���l�[������
						char szName[256];
						std::string path = filepath + "/DamageTrap";
						sprintf_s(szName, "%s%d.txt", path.c_str(), nDtCnt);
						FILE* fpTrap;
						fopen_s(&fpTrap, szName, "rt");
						if (fpTrap)
						{
							Vector3 pos;
							while (true)
							{
								if (fscanf_s(fpTrap, "%f/%f/%f", &pos->x, &pos->y, &pos->z) == EOF)
								{
									break;
								}
								pos->z = 0;
								trap->CreateTrapPoint(pos);
							}
							fclose(fpTrap);
						}
					}

					break;
				case START_EV:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CStartElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = START_EV;
					break;
				case END_EV:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CEndElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = END_EV;
					break;
				case ROBO:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CSecurityRobo>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = ROBO;
					break;
				case CHECK_POINT:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CCheckPoint>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = CHECK_POINT;
					break;
				case MAGNET:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CMagnetBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = MAGNET;
					break;
					// �j��u���b�N
				case BROKEN:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CBrokenObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BROKEN;
					break;
				case DROP:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CDropObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DROP;
					break;
				case FALL:
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
						CObjectManager::CreateObject<CFallObject>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = FALL;
					break;
				}
			}
		}

		// �N���[�Y
		fclose(fp);
	}
	else
	{
		// �G���[

		// �f�t�H���g�Ń}�b�v����肽��
		// �X�^�[�g�G���x�[�^�[�𐶐�
		int hnum = 10;
		int wnum = 10;

		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CStartElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = START_EV;
	}

	// ���[�h���f�t�H���g��
	m_mode = eDefault;
}


//----------------------------------------------------------------------//
//	�ݒu�I�u�W�F�N�g�؂�ւ�
//----------------------------------------------------------------------//
void CEditorComponent::ChangeObj(void)
{
	// ���[�h���f�t�H���g��
	m_mode = eDefault;

	int n = m_type;

	n = (n + 1) % MAX_OBJECT;
	if (n == 0) n = 1;

	m_type = n;

	const auto& render = m_pParent->GetComponent<CAssimpRenderer>();

	switch (m_type)
	{
	case CEditorComponent::BLOCK:
		render->ModelLoad("data/model/box.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	case CEditorComponent::DOOR:
		render->ModelLoad("data/model/Door/Door.fbx");
		m_drawOffset = Vector3{ 0,-50,0 };
		break;
	case CEditorComponent::LIFT:
		render->ModelLoad("data/model/Lift/Lift.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	case CEditorComponent::START_EV:
		render->ModelLoad("data/model/Ev/Elevator_Ainm3.fbx");
		m_drawOffset = Vector3{ 0,-50,150 };
		break;
	case CEditorComponent::END_EV:
		render->ModelLoad("data/model/Ev/Elevator_Ainm3.fbx");
		m_drawOffset = Vector3{ 0,-50,150 };
		break;
	case CEditorComponent::ROBO:
		render->ModelLoad("data/model/EnemyA_Mix/EnemyA_Mix.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	case CEditorComponent::CHECK_POINT:
		render->ModelLoad("data/model/Resporn/Resporn.fbx");
		m_drawOffset = Vector3{ 0,-50,0 };
		break;
	case CEditorComponent::MAGNET:
		render->ModelLoad("data/model/Magnet/Magnet.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	case CEditorComponent::DAMAGE_TRAP:
		render->ModelLoad("data/model/DamageTrap/DamageTrap.fbx");
		m_drawOffset = Vector3{ 0,-50,0 };
		break;
	case CEditorComponent::BROKEN:
		render->ModelLoad("data/model/BreakBrock/BreakBrock.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	case CEditorComponent::DROP:
		render->ModelLoad("data/model/FallBrock/FallBrock.fbx");
		m_drawOffset = Vector3{ 0,-50,0 };
		break;
	case CEditorComponent::FALL:
		render->ModelLoad("data/model/EnagyFallBrock/EnagyFallBrock.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	}
}

//----------------------------------------------------------------------//
//	�I�u�W�F�N�g��ގ擾
//----------------------------------------------------------------------//
int CEditorComponent::GetType(void)
{
	return m_type;
}

//----------------------------------------------------------------------//
//	�ݒu�I�u�W�F�N�g�؂�ւ�
//----------------------------------------------------------------------//
void CEditorComponent::ChangeSize(void)
{
	if (m_chipsize == CHIP_SIZE)
	{
		m_chipsize = CHIP_HALF;
	}
	else
	{
		m_chipsize = CHIP_SIZE;
	}
}

//----------------------------------------------------------------------//
//	�z�񐔊m�F
//----------------------------------------------------------------------//
void CEditorComponent::CheckArray(void)
{
	//if(m_aMapData.size() < )
}

//----------------------------------------------------------------------//
//	�}�b�v�f�[�^�J��
//----------------------------------------------------------------------//
void CEditorComponent::ClearMapData()
{
	for (int i = 0; i < MAP_SIZE_X * MAP_SIZE_Y; i++)
	{
		m_aMapData[i].m_nObjType = 0;
		const auto& obj = m_aMapData[i].m_obj_weak.lock();
		if (obj)
		{
			CObjectManager::DestroyObject(obj);
		}
	}

	// ���[�h���f�t�H���g��
	m_mode = eDefault;
}

//===================================
//
//	�J�[�\���̈ړ�
//
//===================================
void CEditorComponent::Move()
{
	//�J�[�\���ʒu�ړ�
	if (GetKeyTrigger(VK_RIGHT))
	{//�E�ړ�
		if (GetKeyTrigger(VK_UP))
		{//�E��ړ�
			m_cursorPos_Y += m_chipsize;
		}
		else if (GetKeyTrigger(VK_DOWN))
		{//�E���ړ�
			m_cursorPos_Y -= m_chipsize;
			if (m_cursorPos_Y < 0)	m_cursorPos_Y = 0;
		}
		m_cursorPos_X += m_chipsize;
	}
	else if (GetKeyTrigger(VK_LEFT))
	{//���ړ�
		if (GetKeyTrigger(VK_UP))
		{//����ړ�
			m_cursorPos_Y += m_chipsize;
		}
		else if (GetKeyTrigger(VK_DOWN))
		{//�����ړ�
			m_cursorPos_Y -= m_chipsize;
			if (m_cursorPos_Y < 0)	m_cursorPos_Y = 0;
		}
		m_cursorPos_X -= m_chipsize;
		if (m_cursorPos_X < 0)	m_cursorPos_X = 0;
	}
	else if (GetKeyTrigger(VK_UP))
	{//��ړ�
		if (GetKeyTrigger(VK_RIGHT))
		{//�E��ړ�
			m_cursorPos_X += m_chipsize;
		}
		else if (GetKeyTrigger(VK_LEFT))
		{//����
			m_cursorPos_X -= m_chipsize;
			if (m_cursorPos_X < 0)	m_cursorPos_X = 0;
		}
		m_cursorPos_Y += m_chipsize;

	}
	else if (GetKeyTrigger(VK_DOWN))
	{
		if (GetKeyTrigger(VK_RIGHT))
		{
			m_cursorPos_X += m_chipsize;
		}
		else if (GetKeyTrigger(VK_LEFT))
		{
			m_cursorPos_X -= m_chipsize;
			if (m_cursorPos_X < 0)	m_cursorPos_X = 0;
		}
		else
		{
			m_cursorPos_Y -= m_chipsize;
			if (m_cursorPos_Y < 0)	m_cursorPos_Y = 0;
		}
	}
}


void CEditorComponent::Reset()
{
	// ���[�h���f�t�H���g��
	m_mode = eDefault;

	m_type = BLOCK;

	const auto& render = m_pParent->GetComponent<CAssimpRenderer>();

	switch (m_type)
	{
	case CEditorComponent::BLOCK:
		render->ModelLoad("data/model/box.fbx");
		m_drawOffset = Vector3{ 0,0,0 };
		break;
	}
}

void CEditorComponent::GimmickRestart()
{
	//�}�b�v�`�b�v����
	for (int hnum = 0; hnum < m_nHeight; hnum++)
	{
		for (int wnum = 0; wnum < m_nWidth; wnum++)
		{
			const auto& obj = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock();
			if (!obj) continue;

			switch (m_aMapData[hnum * m_nWidth + wnum].m_nObjType)
			{
			case CEditorComponent::NONE:
				break;
			case CEditorComponent::BLOCK:
				break;
			case CEditorComponent::DOOR:
				obj->GetComponent<CDoorComponent>()->Restart();
				break;
			case CEditorComponent::LIFT:
				break;
			case CEditorComponent::START_EV:
				break;
			case CEditorComponent::END_EV:
				break;
			case CEditorComponent::ROBO:
				obj->GetComponent<CSecurityRoboComponent>()->Restart();
				break;
			case CEditorComponent::CHECK_POINT:
				break;
			case CEditorComponent::DAMAGE_TRAP:
				break;
			case CEditorComponent::MAGNET:
				obj->GetComponent<CMagnetBlockComponent>()->Restart();
				break;
			case CEditorComponent::BROKEN:
				obj->GetComponent<CBrokenComponent>()->Restart();
				break;
			case CEditorComponent::DROP:
				obj->GetComponent<CDropComponent>()->Restart();
				break;
			case CEditorComponent::FALL:
				obj->GetComponent<CFallComponent>()->Restart();
				break;
			case CEditorComponent::MAX_OBJECT:
				break;
			default:
				break;
			}
		}
	}
}

unsigned int CEditorComponent::GetMaxStateNum()
{
	return MAX_STAGE;
}