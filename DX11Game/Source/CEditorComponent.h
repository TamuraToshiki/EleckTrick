//======================================================================
//											CEditorComponent.h
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

#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CObjectManager.h"
//#include <vector>

//-- �}�N����` --//
#define MAP_SIZE_X	(100)
#define MAP_SIZE_Y	(100)
#define MAP_SIZE	(MAP_SIZE_X * MAP_SIZE_Y)

//===== �N���X��` =====
class CEditorComponent : public CComponent
{
private:
	enum EObjectType
	{
		NONE,

		BLOCK = 1,
		DOOR,
		LIFT,
		START_EV,
		END_EV,
		ROBO,
		CHECK_POINT,
		DAMAGE_TRAP,
		MAGNET,
		BROKEN,
		DROP,
		FALL,

		MAX_OBJECT,
	};

	// �G�f�B�b�g���[�h
	enum EEditMode
	{
		eDefault,
		eDoorSwitch,
		eLiftPoint,
		eDamegeTrap,

		eMaxEditMode,
	};


	struct SMapData
	{
		int m_nObjType;
		std::weak_ptr<CObject> m_obj_weak;
	};

	// --- �ϐ��錾 ---
	SMapData m_aMapData[MAP_SIZE] = { 0 };					//�z��|�C���^
	//std::vector<int> m_aMapData;		//�f�[�^�i�[�p
	int m_cursorPos_X;					//�J�[�\���ʒuX
	int m_cursorPos_Y;					//�J�[�\���ʒuY

	int m_nHeight = 0;					//
	int m_nWidth = 0;					//
	int m_chipsize;						//�`�b�v�T�C�Y
	int m_type;							//�I�u�W�F�N�g�̎��
	EEditMode m_mode;							// �G�f�B�b�g���[�h

	int m_anObjectCount[MAX_OBJECT];	// �e�I�u�W�F�N�g�̐�

	// ������̃I�u�W�F�N�g���o�b�N�A�b�v
	std::weak_ptr<CObject> m_objBackUp;

	Vector3 m_drawOffset;

	static unsigned int g_nCuurentStage;
public:
	// --- �֐��錾 ---

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �������ɌĂ΂�܂�
	void End() override;

	//===== �g��Ȃ����͏����Ă�������() =====
	//--- �R�[���o�b�N�֐� ---
	// 2D�����蔻��
	//void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	// �ړ�
	void Move();

	// ����
	void Create();

	// �f�t�H���g�������[�h
	void DefaultCreateMode();

	// �h�A�X�C�b�`�������[�h
	void DoorSwitchMode();

	// ���t�g�|�C���g�������[�h
	void LiftPointMode();

	// �_���[�W�g���b�v�������[�h
	void DamegeTrapMode();

	// �j��
	void Destroy();

	// �}�b�v���[�h
	void Load(std::string filename);

	// �}�b�v�Z�[�u
	void Save(std::string filename);

	//-- �ݒu�I�u�W�F�N�g�؂�ւ� --//
	void ChangeObj(void);

	//-- �I�u�W�F�N�g�̎�ގ擾 --//
	int GetType(void);

	//-- �`�b�v�T�C�Y�؂�ւ� --//
	void ChangeSize(void);

	//-- �z��`�F�b�N --//
	void CheckArray(void);

	//-- �}�b�v�f�[�^�J��
	void ClearMapData(void);

	//-- �X�e�[�W���̉��Z
	static void AddStage() { g_nCuurentStage++; }

	// ���Z�b�g
	void Reset();

	// �M�~�b�N�̃��X�^�[�g
	void GimmickRestart();

	// �X�e�[�W���擾
	static int GetCurrentStageNum() { return g_nCuurentStage; }
	// �X�e�[�W�̎w��
	static void SetCurrentStageNum(int nNum) { g_nCuurentStage = nNum; }
	// �X�e�[�W�̍ő吔
	static unsigned int GetMaxStateNum();
};



