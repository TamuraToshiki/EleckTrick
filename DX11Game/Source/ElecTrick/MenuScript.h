//==================================================================
//								MenuScript.h
//	���j���[�R���|�[�l���g�̈ڐA
//
//==================================================================
//	author :	AT13C 12 �匴����
//==================================================================
//	�J������
//
//	2021/06/17	�ڐA�J�n
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "../Engine/ECSCompoent/Script.h"


//===== �}�N����` =====
#define MAX_MENU (3)

//===== �N���X��` =====
namespace ECS
{
	class MenuScript : public Script
	{
	private:
		// ----- �����o -----
		bool m_bStick;					//�R���g���[���[�̃X�e�B�b�N����
		bool m_bOpen;					//���j���[���\�����ꂽ��
		bool m_bButton;					//�{�^�������͂��ꂽ��
		int m_nMenuIndex;				//�I����
		float m_fChoiseColor;			//�I�����̃n�C���C�g
		float m_fTimer;					//�A�����͖h�~
		std::weak_ptr<SpriteRenderer>	//�X�v���C�g�󂯎��p
			m_SpriteMenu[MAX_MENU];

	protected:
		// ----- ���\�b�h -----

		// �J�n���ɌĂ΂�܂�
		void Start() override;
		// ���t���[���Ă΂�܂�
		void Update() override;
		// ���t���[���X�V��ɌĂ΂�܂�
		void LateUpdate() override;
		// �I�����ɌĂ΂�܂�
		void End() override;

		//�C���f�b�N�X�̎擾
		int GetIndex();

		//�\���t���O�̎擾
		bool GetOpenFrag();

		//�\���t���O�̃Z�b�g
		void SetOpenFlag(bool);

		// ----- �R�[���o�b�N�֐� -----

		// ����������
		void OnCollisionEnter(Collider* collider) override;
		// �������Ă����
		void OnCollisionStay (Collider* collider) override;
		// ���ꂽ��
		void OnCollisionExit (Collider* collider) override;

		// ECS
		// ����������
		void OnDeltaCollisionEnter(DeltaCollider* collider) override;
		// �������Ă����
		void OnDeltaCollisionStay(DeltaCollider* collider) override;
		// ���ꂽ��
		void OnDeltaCollisionExit(DeltaCollider* collider) override;
	};
}
