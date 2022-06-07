//==================================================================
//								CObjectManager.h
//	�I�u�W�F�N�g�}�l�[�W���[
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/09/23	�I�u�W�F�N�g�}�l�[�W���[�N���X�쐬
//
//===================================================================

#pragma once

//====== �C���N���[�h�� ======
#include "CObject.h"
#include <algorithm>

//===== �}�N����` =====


//===== �^�ύX =====
typedef std::shared_ptr<CObject> obj_shared;
typedef std::weak_ptr<CObject> obj_weak;


//===== �N���X��` =====
class CObjectManager
{
	friend CObject;
public:
	CObjectManager(){}
	~CObjectManager(){}

	template<class T>
	static obj_shared CreateObject(Vector3 pos = {0.0f, 0.0f, 0.0f}, Vector3 rot = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
	{
		// ���W�o�b�t�@�Ɋi�[
		m_posBuffer = pos;
		m_rotBuffer = rot;
		m_scaleBuffer = scale;

		obj_shared ptr;
		ptr.reset(new T());
		AddList(ptr);

		ptr->m_this = ptr;

		return ptr;
	}

	static void DestroyObject(CObject *pObj)
	{
		for (auto itr = m_objectList.begin(); itr != m_objectList.end(); ++itr)
		{
			if (&(*(*itr)) == pObj)
			{
				// ���Ɋi�[����Ă����ꍇ
				const auto& itr2 = std::find(m_destroyList.begin(), m_destroyList.end(), itr);
				if (itr2 != m_destroyList.end()) break;

				m_destroyList.push_back(itr);
				(*itr)->End();
				break;
			}
		}
	}

	static void DestroyObject(const obj_shared& pObj)
	{
		for (auto itr = m_objectList.begin(); itr != m_objectList.end(); ++itr)
		{
			if (*itr == pObj)
			{
				// ���Ɋi�[����Ă����ꍇ
				const auto& itr2 = std::find(m_destroyList.begin(), m_destroyList.end(), itr);
				if (itr2 != m_destroyList.end()) break;

				m_destroyList.push_back(itr);
				(*itr)->End();
				break;
			}
		}
	}

	static void AddList(const obj_shared& pObj)
	{
		m_objectList.push_back(pObj);
	}


	// �X�V����
	static void Update()
	{
		//for (auto obj : m_objectList)
		std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& obj)
		{
			obj->Update();
		});
	}

	// ��X�V����
	static void LateUpdate()
	{
		//for (auto obj : m_objectList)
		std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& obj)
		{
			obj->LateUpdate();
		});
	}

	static void DestroyUpdate()
	{
		std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& obj)
			{
				obj->DestroyComUpdate();
			});

		std::for_each(m_destroyList.begin(), m_destroyList.end(), [](const auto& itr)
		{
			m_objectList.erase(itr);
		});
		m_destroyList.clear();
	}

	//// �`�揈��
	//static void Draw()
	//{
	//	for (auto obj : m_objectList)
	//	{
	//		(*obj).Draw();
	//	}
	//}

	// ���X�g�N���A
	static void ClearObjectList() 
	{ 
		for (auto itr = m_objectList.begin(); itr != m_objectList.end(); ++itr)
		{
			// ���Ɋi�[����Ă����ꍇ
			const auto& itr2 = std::find(m_destroyList.begin(), m_destroyList.end(), itr);
			if (itr2 != m_destroyList.end()) continue;

			// �R���|�[�l���g���폜���X�g�ɒǉ�
			m_destroyList.push_back(itr);
			(*itr)->End();
		}
		DestroyUpdate();
		m_objectList.clear();
	}

	// �l�[���T�[�`
	static std::weak_ptr<CObject> SearchObjectName(std::string name)
	{
		for (const auto& obj : m_objectList)
		{
			if (obj->GetName() == name)
			{
				return obj;
			}
		}

		return std::weak_ptr<CObject>();
	}
	// �^�O�T�[�`
	static std::weak_ptr<CObject> SearchObjectTag(std::string tag)
	{
		for (const auto& obj : m_objectList)
		{
			if (obj->GetTag() == tag)
			{
				return obj;
			}
		}

		return std::weak_ptr<CObject>();
	}

private:
	static std::list<obj_shared> m_objectList;
	static std::list<std::list<obj_shared>::iterator> m_destroyList;


	static Vector3 m_posBuffer;
	static Vector3 m_rotBuffer;
	static Vector3 m_scaleBuffer;
};
