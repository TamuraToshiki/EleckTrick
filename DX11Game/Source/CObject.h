//==================================================================
//								CObject.h
//	�I�u�W�F�N�g�x�[�X�N���X
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/09/23	�I�u�W�F�N�g�N���X�쐬
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "CComponent.h"
#include <list>
#include <memory>
#include "transform.h"
#include <string>
#include <algorithm>

//===== �}�N����` =====


//===== �^�ύX =====
//template<class T>
//using com_shaerd = std::shared_ptr<T>;

typedef std::shared_ptr<CComponent> com_shared;
typedef std::weak_ptr<CComponent> com_weak;


//===== �N���X��` =====
class CObjectManager;

class CObject
{
	friend CObjectManager;
public:
	CObject();
	virtual ~CObject();

	void Start()
	{
		for (const auto& com : m_ComponentList)
		{
			com->Start();
		}
	}

	// �R���|�[�l���g�̍X�V����
	void Update()
	{
		// �X�g�b�v
		if (m_bStop) return;

		//auto buffer = m_ComponentList;
		//for (const auto& com : buffer)
		for_each(m_ComponentList.begin(), m_ComponentList.end(), [](const auto& com)
		{
			com->Update();
		});
	}

	// �R���|�[�l���g�̌�X�V����
	void LateUpdate()
	{
		// �X�g�b�v
		if (m_bStop) return;

		//auto buffer = m_ComponentList;
		//for (const auto& com : buffer)
		for_each(m_ComponentList.begin(), m_ComponentList.end(), [](const auto& com)
		{
			com->LateUpdate();
		});
	}

	void End()
	{
		for (const auto& com : m_ComponentList)
		{
			com->End();
		}
	}

	// �R���|�[�l���g�̍폜
	void DestroyComUpdate()
	{
		// �R���|�[�l���g�̍폜
		for (auto itr = m_DestroyComponentList.begin(); itr != m_DestroyComponentList.end(); ++itr)
		{
			(**itr)->End();
			m_ComponentList.erase(*itr);
		}
		m_DestroyComponentList.clear();
	}


	// �R���|�[�l���g�ǉ�
	template <class T>
	std::shared_ptr<T> AddComponent()
	{
		// �R���|�[�l���g�̐V�K�쐬
		std::shared_ptr<T> buffer(new T());
		//T* buffer = new T();
		// �R���|�[�l���gID��o�^
		buffer->SetTypeID<T>();
		// �e�I�u�W�F�N�g��o�^
		//std::shared_ptr<CObject> ptr(this);
		buffer->SetParent(this);
		// ���X�g�Ɋi�[
		m_ComponentList.push_front(buffer);
		// ������
		buffer->Start();

		return buffer;
	}

	// �R���|�[�l���g���擾
	template <class T>
	std::shared_ptr<T> GetComponent()
	{
		for (const auto& com : m_ComponentList)
		//std::for_each(m_ComponentList.begin(), m_ComponentList.end(), [](const auto& com)
		{
			// �R���|�[�l���gID�𔻒�
			if (com->GetID() == CUniqueTypeID::Get<T>())
			{
				// ����������L���X�g
				//std::shared_ptr<T> buffer;
				//buffer.reset(static_cast<T*>(&(*com)));
				auto buffer = std::static_pointer_cast<T>(com);

				return buffer;
			}
		}
		// �Ȃ�������
		return nullptr;
	}

	// �R���|�[�l���g������
	template <class T>
	bool RemoveComponet()
	{
		for (auto itr = m_ComponentList.begin(); itr != m_ComponentList.end(); ++itr)
		{
			// �R���|�[�l���gID�𔻒�
			if ((*itr)->GetID() == CUniqueTypeID::Get<T>())
			{
				m_DestroyComponentList.push_back(itr);
				return true;
			}
		}
		return false;
	}


	//--- �Q�b�g�֐�
	std::string GetTag() { return m_tag; }
	std::string GetName() { return m_name; }
	bool GetStop() { return m_bStop; }
	const std::list<com_shared>& GetComponentList() { return m_ComponentList; }

	std::weak_ptr<CObject> tish() { return m_this; }

	//--- �Z�b�g�֐�
	void SetStop(bool bStop) { m_bStop = bStop; }
	void SetName(std::string name) { m_name = name; }

	// �f�t�H���g�R���|�[�l���g
	std::shared_ptr<CTransform> m_pTrans;

protected:
	// �^�O
	std::string m_tag;
	// �l�[��
	std::string m_name;
	// �X�g�b�v
	bool m_bStop;

private:
	// �R���|�[�l���g�i�[���X�g
	std::list<com_shared> m_ComponentList;
	std::list<std::list<com_shared>::iterator> m_DestroyComponentList;

	std::weak_ptr<CObject> m_this;
};


//#include "CObjectManager.h"
//
//CObject::CObject()
//{
//	//// �X�}�[�g�|�C���^�̐���
//	//obj_shared ptr;
//	//ptr.reset(this);
//
//	//// ���X�g�Ɋi�[
//	//CObjectManager::AddList(ptr);
//
//
//	
//}
//
//CObject::~CObject()
//{
//	
//}