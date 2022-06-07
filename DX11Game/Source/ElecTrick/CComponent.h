//==================================================================
//								CComponent.h
//	�R���|�[�l���g�x�[�X�N���X
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/09/23	�R���|�[�l���g�N���X�쐬
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "uniqueTypeID.h"
#include <memory>
#include "Math.h"

//===== �}�N����` =====


//===== �N���X��` =====

//===== �O��` =====
class CObject;
class CCollision2D;
class CAnimator2D;
class CAssimpRenderer;

class CComponent
{
public:
	CComponent() 
	{
		// ������
		m_nTypeID = -1;
		m_pParent = nullptr;
	}
	virtual ~CComponent() {}

	// ���z�֐�
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void End() {}

	// �e�I�u�W�F�N�g
	void SetParent(CObject* pObjct) { m_pParent = pObjct; }

	// ID
	// ID�̓o�^
	template <class T>
	void SetTypeID()
	{
		m_nTypeID = CUniqueTypeID::Get<T>();
	}
	// ID�̎擾
	int GetID()
	{
		return m_nTypeID;
	}
	CObject* GetParent() { return m_pParent; }

	//--- �R�[���o�b�N�֐�
	// �����蔻��R�[���o�b�N�֐�
	virtual void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	virtual void OnCollisionStay2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	virtual void OnCollisionExit2D(const std::shared_ptr <CCollision2D>& collsion2d) {}
	// 2D�A�j���[�V�����R�[���o�b�N�֐�
	virtual void OnAnimationEnter2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	virtual void OnAnimationStay2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	virtual void OnAnimationExit2D(const std::shared_ptr <CAnimator2D>& animator2d) {}
	// 3D�A�j���[�V�����R�[���o�b�N�֐�
	virtual void OnAnimationExit3D(const std::shared_ptr <CAssimpRenderer>& assimp){}
	
	CObject* m_pParent;

protected:

private:
	int m_nTypeID;

};

