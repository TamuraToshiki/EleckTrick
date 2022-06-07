//======================================================================
//											CEditorComponent.cpp
//	マップエディタのコンポーネント
//
//======================================================================
//	author :	AT12A 12 紀平
//					  05 宇佐美
//======================================================================
//	開発履歴
//	2020/12/03	作成
//				紀平から引き継ぎ/榊原
//				一応ブロックの生成はできるが
//				用意された範囲内にデータが収まらなければ
//				エラーが出る。動的配列に要変更。 
//	2020/12/08	宇佐美が引継ぎ
//	2020/12/09	スイッチとリフトポイントを置けるように
//
//======================================================================


//===== インクルード部 =====
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

//===== マクロ定義 =====
#define CHIP_SIZE   (100.0f)
#define CHIP_HALF	(50.0f)	

//#define FILE_PATH "data/map/"
#define MAP_PATH "/map.csv"

#define FILE_PATH [&](unsigned int n){					\
	char szName[256];									\
	sprintf_s(szName, "%s%d", "data/map/stage", n);		\
	return std::string(szName);}


#define MAX_STAGE (19)	//最終マップ

//===== プロトタイプ宣言 =====


//===== グローバル変数 =====
unsigned int CEditorComponent::g_nCuurentStage = 0;



//===================================
//
//	スタート関数
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

	m_mode = eDefault;							// エディットモード

	m_nHeight = MAP_SIZE_Y;
	m_nWidth = MAP_SIZE_X;

	m_drawOffset = Vector3{ 0,0,0 };

	// マップロード
	//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
	Load(FILE_PATH(g_nCuurentStage));
}


//===================================
//
//	更新関数
//
//===================================
void CEditorComponent::Update()
{
	// オブジェクトの生成
	if (GetKeyTrigger(VK_1))
	{
		Create();
	}

	// オブジェクトの破棄
	if (GetKeyTrigger(VK_2))
	{
		Destroy();
	}

	// オブジェクトの切り替え
	if (GetKeyTrigger(VK_K) || GetKeyTrigger(VK_3))
	{
		CEditorComponent::ChangeObj();
	}

	// セーブ
	if (GetKeyTrigger(VK_8))
	{
		//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
		Save(FILE_PATH(g_nCuurentStage));
	}

	// ロード
	if (GetKeyTrigger(VK_9))
	{
		//std::string path = FILE_PATH + CSceneManager::Get()->GetCurrentScene()->GetSceneName();
		Load(FILE_PATH(g_nCuurentStage));
	}

	// マップデータの開放
	if (GetKeyTrigger(VK_0))
	{
		ClearMapData();
	}

	// カーソル移動
	Move();

	// 座標同期
	m_pParent->m_pTrans->m_pos = Vector3{ (float)m_cursorPos_X, (float)m_cursorPos_Y, 0.0f } + m_drawOffset;
	CCamera::GetMainCamera()->SetCameraTarget(m_pParent->m_pTrans);


	// ステージ数の加減
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
//	後更新関数
//
//===================================
void CEditorComponent::LateUpdate()
{
	PrintDebugProc("UP   :ｳｴ\n");
	PrintDebugProc("RIGHT:ﾐｷﾞ\n");
	PrintDebugProc("LEFT :ﾋﾀﾞﾘ\n");
	PrintDebugProc("DOWN :ｼﾀ\n");

	PrintDebugProc("1     :ｾｲｾｲ\n");
	PrintDebugProc("2     :ｻｸｼﾞｮ\n");
	PrintDebugProc("P     :ｼｭｳﾘｮｳ\n");
	PrintDebugProc("K,3   :ｷﾘｶｴ\n");

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
	PrintDebugProc("ｹﾞﾝｻﾞｲ:%d\n", m_type);

	PrintDebugProc("Key8:ｾｰﾌﾞ\n");
	PrintDebugProc("Key9:ﾛｰﾄﾞ\n");
	PrintDebugProc("Key0:ｶｲﾎｳ\n");

	PrintDebugProc("\nStage  :%u\n", g_nCuurentStage);
	PrintDebugProc("Stage++:6ﾞ\n");
	PrintDebugProc("Stage--:5\n");
}


//===================================
//
//	終了関数
//
//===================================
void CEditorComponent::End()
{
	ClearMapData();

	// ステージを次へ
	g_nCuurentStage++;
}


//===================================
//
//	オブジェクトの生成
//
//===================================
void CEditorComponent::Create()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// 既に生成されていたら
	//if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	
	// モードで生成処理を分岐
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


// デフォルト生成モード
void CEditorComponent::DefaultCreateMode()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// 既に生成されていたら
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	//配列へ格納
	switch (m_type)
	{
	case BLOCK:
	{
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BLOCK;
		const auto& block = CObjectManager::CreateObject<CBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak = block;

		// テクスチャの指定
		int texNo = (2 - hnum % 3) * 3 + wnum % 3;
		block->GetComponent<CBlockComponent>()->SetTexture(texNo);
	}
		break;
	case DOOR:
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = DOOR;
		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CDoor>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		{
			// モード切替
			m_mode = eDoorSwitch;
			// モデルデータ切替
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
			// モード切替
			m_mode = eLiftPoint;
			// モデルデータ切替
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
			// モード切替
			m_mode = eDamegeTrap;
			// モデルデータ切替
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
		// 破壊ブロック
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

	// 生成オブジェクトをバックアップ
	m_objBackUp = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak;
}

// ドアスイッチ生成モード
void CEditorComponent::DoorSwitchMode()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// 既に生成されていたら
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType != 0) return;

	// ドアコンポーネント
	const auto& door = m_objBackUp.lock()->GetComponent<CDoorComponent>();
	if (!door) return;

	// スイッチの生成
	door->CreateSwitch({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });

}

// リフトポイント生成モード
void CEditorComponent::LiftPointMode()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// 既に生成されていたら
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType == 0 || 
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType == LIFT)
	{
		// リフトコンポーネント
		const auto& lift = m_objBackUp.lock()->GetComponent<CLiftComponent>();
		if (!lift) return;

		// リフトポイントの生成
		lift->CreateLiftPoint({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
	}
}

// ダメージトラップ生成モード
void CEditorComponent::DamegeTrapMode()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	// 既に生成されていたら
	if (m_aMapData[hnum * m_nWidth + wnum].m_nObjType == 0 ||
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType == DAMAGE_TRAP)
	{
		// リフトコンポーネント
		const auto& trap = m_objBackUp.lock()->GetComponent<CDamageTrapObjComponent>();
		if (!trap) return;

		// リフトポイントの生成
		trap->CreateTrapPoint({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
	}
}

//===================================
//
//	オブジェクトの破棄
//
//===================================
void CEditorComponent::Destroy()
{
	//クリックした座標の記録用変数
	int imgx = 0;
	int imgy = 0;
	//クリックされた配列を記録する変数
	int hnum = 0;
	int wnum = 0;

	//押された座標を記録
	imgx = m_cursorPos_X; imgy = m_cursorPos_Y;
	//それをチップサイズで割る事で押された配列を確認
	wnum = imgx / m_chipsize; hnum = imgy / m_chipsize;

	// 配列範囲外
	if (hnum < 0 || hnum > MAP_SIZE_Y - 1) return;
	if (wnum < 0 || wnum > MAP_SIZE_X - 1) return;

	//そこを0に変える
	m_aMapData[hnum * m_nWidth + wnum].m_nObjType = 0;

	// オブジェクトを削除
	const auto& obj = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock();
	if (obj)
	{
		if (obj == m_objBackUp.lock()) m_mode = eDefault;

		CObjectManager::DestroyObject(obj);
	}
}


//===================================
//
//	セーブ
//
//===================================
void CEditorComponent::Save(std::string filepath)
{
	if (_mkdir(filepath.c_str()) == 0) {
		// 成功

	}
	else {
		// 失敗

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

				// ドアスイッチやリフトポイントだったら
				if (nNum == DOOR)
				{
					// 数
					nDwCnt++;

					const auto& door_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CDoorComponent>();
					const auto& aswitch = door_s->GetSwitchContainer();

					// ファイルネーム生成
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
					// 数
					nLpCnt++;

					const auto& lift_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CLiftComponent>();
					const auto& liftpoint = lift_s->GetLiftPoint();

					// ファイルネーム生成
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
					// 数
					nDtCnt++;

					const auto& trap_s = m_aMapData[i * m_nWidth + j].m_obj_weak.lock()->GetComponent<CDamageTrapObjComponent>();
					const auto& trappoint = trap_s->GetTrapPoint();

					// ファイルネーム生成
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
		// エラー

	}
}


//===================================
//
//	ロード
//
//===================================
void CEditorComponent::Load(std::string filepath)
{
	// マップデータの開放
	ClearMapData();

	FILE *fp;

	int nDwCnt = 0;
	int nLpCnt = 0;
	int nDtCnt = 0;

	// ファイルオープン
	fopen_s(&fp, (filepath + MAP_PATH).c_str(), "rt");

	if (fp)
	{
		// マップの読み込み
		fseek(fp, 0, SEEK_SET);
		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				fscanf_s(fp, "%d,", &m_aMapData[i * m_nWidth + j]);
			}
		}

		//マップチップ生成
		for (int hnum = 0; hnum < m_nHeight; hnum++) {
			for (int wnum = 0; wnum < m_nWidth; wnum++) {
				//配列要素が1である所に画像を表示する。
				switch (m_aMapData[hnum * m_nWidth + wnum].m_nObjType)
				{
				case BLOCK:
				{
					m_aMapData[hnum * m_nWidth + wnum].m_nObjType = BLOCK;
					const auto& block = CObjectManager::CreateObject<CBlock>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
					m_aMapData[hnum * m_nWidth + wnum].m_obj_weak = block;

					// テクスチャの指定
					int texNo = (2 - hnum % 3) * 3 + wnum % 3;
					block->GetComponent<CBlockComponent>()->SetTexture(texNo);

					// ここで当たり判定の最適化をしたい
					// 周りが全てブロックだったら当たり判定なしに
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
						// コンポーネントを取り除く
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
						// 数
						nDwCnt++;
						// 取得
						const auto& door_s = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CDoorComponent>();
						// スイッチの配置
						// ファイルネーム生成
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
						// 数
						nLpCnt++;
						// 取得
						const auto& lift = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CLiftComponent>();
						// スイッチの配置
						// ファイルネーム生成
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
						// 数
						nDtCnt++;
						// 取得
						const auto& trap = m_aMapData[hnum * m_nWidth + wnum].m_obj_weak.lock()->GetComponent<CDamageTrapObjComponent>();
						// スイッチの配置
						// ファイルネーム生成
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
					// 破壊ブロック
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

		// クローズ
		fclose(fp);
	}
	else
	{
		// エラー

		// デフォルトでマップを作りたい
		// スタートエレベーターを生成
		int hnum = 10;
		int wnum = 10;

		m_aMapData[hnum * m_nWidth + wnum].m_obj_weak =
			CObjectManager::CreateObject<CStartElevator>({ (float)wnum * m_chipsize,(float)hnum * m_chipsize,0.0f });
		m_aMapData[hnum * m_nWidth + wnum].m_nObjType = START_EV;
	}

	// モードをデフォルトに
	m_mode = eDefault;
}


//----------------------------------------------------------------------//
//	設置オブジェクト切り替え
//----------------------------------------------------------------------//
void CEditorComponent::ChangeObj(void)
{
	// モードをデフォルトに
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
//	オブジェクト種類取得
//----------------------------------------------------------------------//
int CEditorComponent::GetType(void)
{
	return m_type;
}

//----------------------------------------------------------------------//
//	設置オブジェクト切り替え
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
//	配列数確認
//----------------------------------------------------------------------//
void CEditorComponent::CheckArray(void)
{
	//if(m_aMapData.size() < )
}

//----------------------------------------------------------------------//
//	マップデータ開放
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

	// モードをデフォルトに
	m_mode = eDefault;
}

//===================================
//
//	カーソルの移動
//
//===================================
void CEditorComponent::Move()
{
	//カーソル位置移動
	if (GetKeyTrigger(VK_RIGHT))
	{//右移動
		if (GetKeyTrigger(VK_UP))
		{//右上移動
			m_cursorPos_Y += m_chipsize;
		}
		else if (GetKeyTrigger(VK_DOWN))
		{//右下移動
			m_cursorPos_Y -= m_chipsize;
			if (m_cursorPos_Y < 0)	m_cursorPos_Y = 0;
		}
		m_cursorPos_X += m_chipsize;
	}
	else if (GetKeyTrigger(VK_LEFT))
	{//左移動
		if (GetKeyTrigger(VK_UP))
		{//左上移動
			m_cursorPos_Y += m_chipsize;
		}
		else if (GetKeyTrigger(VK_DOWN))
		{//左下移動
			m_cursorPos_Y -= m_chipsize;
			if (m_cursorPos_Y < 0)	m_cursorPos_Y = 0;
		}
		m_cursorPos_X -= m_chipsize;
		if (m_cursorPos_X < 0)	m_cursorPos_X = 0;
	}
	else if (GetKeyTrigger(VK_UP))
	{//上移動
		if (GetKeyTrigger(VK_RIGHT))
		{//右上移動
			m_cursorPos_X += m_chipsize;
		}
		else if (GetKeyTrigger(VK_LEFT))
		{//左上
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
	// モードをデフォルトに
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
	//マップチップ生成
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