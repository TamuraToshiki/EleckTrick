//==================================================================
//								CObjectManager.h
//	オブジェクトマネージャー
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/09/23	オブジェクトマネージャークラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "CObject.h"
#include <algorithm>

//===== マクロ定義 =====


//===== 型変更 =====
typedef std::shared_ptr<CObject> obj_shared;
typedef std::weak_ptr<CObject> obj_weak;


//===== クラス定義 =====
class CObjectManager
{
	friend CObject;
public:
	CObjectManager(){}
	~CObjectManager(){}

	template<class T>
	static obj_shared CreateObject(Vector3 pos = {0.0f, 0.0f, 0.0f}, Vector3 rot = { 0.0f, 0.0f, 0.0f }, Vector3 scale = { 1.0f, 1.0f, 1.0f })
	{
		// 座標バッファに格納
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
				// 既に格納されていた場合
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
				// 既に格納されていた場合
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


	// 更新処理
	static void Update()
	{
		//for (auto obj : m_objectList)
		std::for_each(m_objectList.begin(), m_objectList.end(), [](const auto& obj)
		{
			obj->Update();
		});
	}

	// 後更新処理
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

	//// 描画処理
	//static void Draw()
	//{
	//	for (auto obj : m_objectList)
	//	{
	//		(*obj).Draw();
	//	}
	//}

	// リストクリア
	static void ClearObjectList() 
	{ 
		for (auto itr = m_objectList.begin(); itr != m_objectList.end(); ++itr)
		{
			// 既に格納されていた場合
			const auto& itr2 = std::find(m_destroyList.begin(), m_destroyList.end(), itr);
			if (itr2 != m_destroyList.end()) continue;

			// コンポーネントを削除リストに追加
			m_destroyList.push_back(itr);
			(*itr)->End();
		}
		DestroyUpdate();
		m_objectList.clear();
	}

	// ネームサーチ
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
	// タグサーチ
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
