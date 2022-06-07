//==================================================================
//								CObject.h
//	オブジェクトベースクラス
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/09/23	オブジェクトクラス作成
//
//===================================================================

#pragma once

//====== インクルード部 ======
#include "CComponent.h"
#include <list>
#include <memory>
#include "transform.h"
#include <string>
#include <algorithm>

//===== マクロ定義 =====


//===== 型変更 =====
//template<class T>
//using com_shaerd = std::shared_ptr<T>;

typedef std::shared_ptr<CComponent> com_shared;
typedef std::weak_ptr<CComponent> com_weak;


//===== クラス定義 =====
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

	// コンポーネントの更新処理
	void Update()
	{
		// ストップ
		if (m_bStop) return;

		//auto buffer = m_ComponentList;
		//for (const auto& com : buffer)
		for_each(m_ComponentList.begin(), m_ComponentList.end(), [](const auto& com)
		{
			com->Update();
		});
	}

	// コンポーネントの後更新処理
	void LateUpdate()
	{
		// ストップ
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

	// コンポーネントの削除
	void DestroyComUpdate()
	{
		// コンポーネントの削除
		for (auto itr = m_DestroyComponentList.begin(); itr != m_DestroyComponentList.end(); ++itr)
		{
			(**itr)->End();
			m_ComponentList.erase(*itr);
		}
		m_DestroyComponentList.clear();
	}


	// コンポーネント追加
	template <class T>
	std::shared_ptr<T> AddComponent()
	{
		// コンポーネントの新規作成
		std::shared_ptr<T> buffer(new T());
		//T* buffer = new T();
		// コンポーネントIDを登録
		buffer->SetTypeID<T>();
		// 親オブジェクトを登録
		//std::shared_ptr<CObject> ptr(this);
		buffer->SetParent(this);
		// リストに格納
		m_ComponentList.push_front(buffer);
		// 初期化
		buffer->Start();

		return buffer;
	}

	// コンポーネントを取得
	template <class T>
	std::shared_ptr<T> GetComponent()
	{
		for (const auto& com : m_ComponentList)
		//std::for_each(m_ComponentList.begin(), m_ComponentList.end(), [](const auto& com)
		{
			// コンポーネントIDを判定
			if (com->GetID() == CUniqueTypeID::Get<T>())
			{
				// 見つかったらキャスト
				//std::shared_ptr<T> buffer;
				//buffer.reset(static_cast<T*>(&(*com)));
				auto buffer = std::static_pointer_cast<T>(com);

				return buffer;
			}
		}
		// なかったら
		return nullptr;
	}

	// コンポーネントを消去
	template <class T>
	bool RemoveComponet()
	{
		for (auto itr = m_ComponentList.begin(); itr != m_ComponentList.end(); ++itr)
		{
			// コンポーネントIDを判定
			if ((*itr)->GetID() == CUniqueTypeID::Get<T>())
			{
				m_DestroyComponentList.push_back(itr);
				return true;
			}
		}
		return false;
	}


	//--- ゲット関数
	std::string GetTag() { return m_tag; }
	std::string GetName() { return m_name; }
	bool GetStop() { return m_bStop; }
	const std::list<com_shared>& GetComponentList() { return m_ComponentList; }

	std::weak_ptr<CObject> tish() { return m_this; }

	//--- セット関数
	void SetStop(bool bStop) { m_bStop = bStop; }
	void SetName(std::string name) { m_name = name; }

	// デフォルトコンポーネント
	std::shared_ptr<CTransform> m_pTrans;

protected:
	// タグ
	std::string m_tag;
	// ネーム
	std::string m_name;
	// ストップ
	bool m_bStop;

private:
	// コンポーネント格納リスト
	std::list<com_shared> m_ComponentList;
	std::list<std::list<com_shared>::iterator> m_DestroyComponentList;

	std::weak_ptr<CObject> m_this;
};


//#include "CObjectManager.h"
//
//CObject::CObject()
//{
//	//// スマートポインタの生成
//	//obj_shared ptr;
//	//ptr.reset(this);
//
//	//// リストに格納
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