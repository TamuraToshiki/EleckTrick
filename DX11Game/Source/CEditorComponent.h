//======================================================================
//											CEditorComponent.h
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

#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CObjectManager.h"
//#include <vector>

//-- マクロ定義 --//
#define MAP_SIZE_X	(100)
#define MAP_SIZE_Y	(100)
#define MAP_SIZE	(MAP_SIZE_X * MAP_SIZE_Y)

//===== クラス定義 =====
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

	// エディットモード
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

	// --- 変数宣言 ---
	SMapData m_aMapData[MAP_SIZE] = { 0 };					//配列ポインタ
	//std::vector<int> m_aMapData;		//データ格納用
	int m_cursorPos_X;					//カーソル位置X
	int m_cursorPos_Y;					//カーソル位置Y

	int m_nHeight = 0;					//
	int m_nWidth = 0;					//
	int m_chipsize;						//チップサイズ
	int m_type;							//オブジェクトの種類
	EEditMode m_mode;							// エディットモード

	int m_anObjectCount[MAX_OBJECT];	// 各オブジェクトの数

	// 生成後のオブジェクトをバックアップ
	std::weak_ptr<CObject> m_objBackUp;

	Vector3 m_drawOffset;

	static unsigned int g_nCuurentStage;
public:
	// --- 関数宣言 ---

	// 作成時に呼ばれます
	void Start() override;

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	//===== 使わない時は消してください() =====
	//--- コールバック関数 ---
	// 2D当たり判定
	//void OnCollisionEnter2D(const std::shared_ptr <CCollision2D>& collsion2d) override;

	// 移動
	void Move();

	// 生成
	void Create();

	// デフォルト生成モード
	void DefaultCreateMode();

	// ドアスイッチ生成モード
	void DoorSwitchMode();

	// リフトポイント生成モード
	void LiftPointMode();

	// ダメージトラップ生成モード
	void DamegeTrapMode();

	// 破棄
	void Destroy();

	// マップロード
	void Load(std::string filename);

	// マップセーブ
	void Save(std::string filename);

	//-- 設置オブジェクト切り替え --//
	void ChangeObj(void);

	//-- オブジェクトの種類取得 --//
	int GetType(void);

	//-- チップサイズ切り替え --//
	void ChangeSize(void);

	//-- 配列チェック --//
	void CheckArray(void);

	//-- マップデータ開放
	void ClearMapData(void);

	//-- ステージ数の加算
	static void AddStage() { g_nCuurentStage++; }

	// リセット
	void Reset();

	// ギミックのリスタート
	void GimmickRestart();

	// ステージ数取得
	static int GetCurrentStageNum() { return g_nCuurentStage; }
	// ステージの指定
	static void SetCurrentStageNum(int nNum) { g_nCuurentStage = nNum; }
	// ステージの最大数
	static unsigned int GetMaxStateNum();
};



