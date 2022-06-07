//======================================================================
//											CDoorComponent.h
//	ドアコンポーネント
//
//======================================================================
//	author :	AT12A 18 田村敏基
//======================================================================
//	開発履歴
//	2020/11/20	作成
//		 11/30	大幅変更。
//				ドアの移動方向をY軸固定から、シーンでポジションを入力するプログラムに変更
//				ALLCHECKとPIECESCHECKを追加
//				ALLCHECK→全てのスイッチがtrueになることで作動する
//				PIECESCHECK→trueになった数がリターンされる。個別にみることは出来ない
//				シーンで入力する際は->Doortype(type,pos)を渡す
//				勝手に実装したため、後日リーダーと相談
//
//======================================================================
#pragma once


//====== インクルード部 ======
#include "CObject.h"
#include "CRigidbody2D.h"
#include "CDoor.h"
#include "CSwitch.h"
#include "CSwitchComponent.h"
#include "CObjectManager.h"
#include "debugproc.h"


//===== クラス定義 =====
class CDoorComponent : public CComponent
{
private:
	// --- 変数宣言 ---
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

	// プレイヤー
	std::weak_ptr<CObject> m_player;

	bool AllCheckSwitch();

	int PiecesCheckSwitch();

	std::string m_energyOnTexture;		// 通電時のテクスチャ
	std::string m_energyOffTexture;		// 非通電時のテクスチャ

public:
	// --- 関数宣言 ---
	const auto& GetSwitch(unsigned int nElement);

	// 作成時に呼ばれます
	void Start() override;

	void Restart();

	// 毎フレーム呼ばれます
	void Update() override;

	// 毎フレームの更新後に呼ばれます
	void LateUpdate() override;

	// 消去時に呼ばれます
	void End() override;

	// スイッチ作成関数(引数:スイッチポジション)
	void CreateSwitch(Vector3 pos);

	// ドア生成(引数:ドアタイプ、ドアの移動範囲)
	void Doortype(int nType, Vector3 pos);

	std::vector<std::weak_ptr<CSwitchComponent>>& GetSwitchContainer();

	// ===== 列挙体宣言 =====
	enum DOORTYPE
	{
		ALLCHECK = 1,
		PIECESCHECK,

		MOVE_END,

		MAX_DOOR_TYPE,
	};
};



