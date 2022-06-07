//======================================================================
//											CDoorComponent.h
//	�h�A�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/11/20	�쐬
//		 11/30	�啝�ύX�B
//				�h�A�̈ړ�������Y���Œ肩��A�V�[���Ń|�W�V��������͂���v���O�����ɕύX
//				ALLCHECK��PIECESCHECK��ǉ�
//				ALLCHECK���S�ẴX�C�b�`��true�ɂȂ邱�Ƃō쓮����
//				PIECESCHECK��true�ɂȂ����������^�[�������B�ʂɂ݂邱�Ƃ͏o���Ȃ�
//				�V�[���œ��͂���ۂ�->Doortype(type,pos)��n��
//				����Ɏ����������߁A������[�_�[�Ƒ��k
//
//======================================================================
#pragma once


//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CDoor.h"
#include "CSwitch.h"
#include "CSwitchComponent.h"
#include "CObjectManager.h"
#include "debugproc.h"


//===== �N���X��` =====
class CDoorComponent : public CComponent
{
private:
	// --- �ϐ��錾 ---
	std::weak_ptr<CRigidbody2D> m_rb;
	std::vector<std::weak_ptr<CSwitchComponent>> aSwitchCom;
	std::weak_ptr<CTransform> m_tr;
	std::vector<Vector3> DoorMovePoint;
	Vector3 vec;
	float fDoorSpd;
	int MaxSwitch;
	int CurrentRoot;
	int nMaxPoint;
	int m_nDoorType;
	Vector3 m_initPos;

	bool m_bIsMove;

	// �v���C���[
	std::weak_ptr<CObject> m_player;

	bool AllCheckSwitch();

	int PiecesCheckSwitch();

	std::string m_energyOnTexture;		// �ʓd���̃e�N�X�`��
	std::string m_energyOffTexture;		// ��ʓd���̃e�N�X�`��

public:
	// --- �֐��錾 ---
	const auto& GetSwitch(unsigned int nElement);

	// �쐬���ɌĂ΂�܂�
	void Start() override;

	void Restart();

	// ���t���[���Ă΂�܂�
	void Update() override;

	// ���t���[���̍X�V��ɌĂ΂�܂�
	void LateUpdate() override;

	// �������ɌĂ΂�܂�
	void End() override;

	// �X�C�b�`�쐬�֐�(����:�X�C�b�`�|�W�V����)
	void CreateSwitch(Vector3 pos);

	// �h�A����(����:�h�A�^�C�v�A�h�A�̈ړ��͈�)
	void Doortype(int nType, Vector3 pos);

	std::vector<std::weak_ptr<CSwitchComponent>>& GetSwitchContainer();

	// ===== �񋓑̐錾 =====
	enum DOORTYPE
	{
		ALLCHECK = 1,
		PIECESCHECK,

		MOVE_END,

		MAX_DOOR_TYPE,
	};
};



