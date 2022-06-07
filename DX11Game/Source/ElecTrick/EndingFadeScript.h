//==================================================================
//								EndingFadeScript.h
//	�G���f�B���O���o�p�X�N���v�g
//
//==================================================================
//	author :	AT13C 12 �匴����
//==================================================================
//	�J������
//
//	2021/06/18	�ڐA�J�n
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "../Engine/ECSCompoent/Script.h"


//===== �}�N����` =====


//===== �N���X��` =====
namespace ECS
{
	class EndingFadeScript : public Script
	{
	private:
		// ----- �����o -----
		float m_fTimer;			//�^�C�}�[
		float m_fAlpha;			//���l�p
		float m_fAlpha2;		//���l�p2
		float m_fAlpha3;		//���l�p3
		float m_fHighlight;		//�n�C���C�g�p

		bool m_bMoveEnd;		//�ړ��I��������
		bool m_bFadeEnd;		//�t�F�[�h�I��������

		std::weak_ptr<SpriteRenderer>
			m_EndingSprite[5];	//�����_���[

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
