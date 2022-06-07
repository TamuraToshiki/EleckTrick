//==================================================================
//									collision.h
//	コリジョン
//
//==================================================================
//	author :	AT12A 05 宇佐美晃之
//==================================================================
//	開発履歴
//
//	2020/06/24	コリジョン
//
//===================================================================

//====== インクルード部 ======
#include "CCollision2D.h"
#include "CObject.h"
#include "CRigidbody2D.h"
#include "polygon.h"
#include "Renderer.h"


//===== マクロ定義 =====
#define MAX_CELL (1+4+16+64)

//===== 列挙型 =====
enum ECollider
{
	eRECTANGLE,
	eCIRCLE,

	MAX_COLLISION_TYPE,
};

//===== クラス定義 =====

// 型定義
using col_weak = std::weak_ptr<CCollision2D>;

// 四分木空間
class CCell
{
public:
	CCell()
	{

	}
	~CCell()
	{
		m_list.clear();
	}

	void ClearList()
	{
		m_list.clear();
	}

	std::list<col_weak>& GetList() { return m_list; }

	static void SetMapSize(float width, float height)
	{
		m_fUnit_W = width / (float)(1 << (m_uiLevel - 1));
		m_fUnit_H = height / (float)(1 << (m_uiLevel - 1));
	}

	static float GetUnitW() { return m_fUnit_W; }
	static float GetUnitH() { return m_fUnit_H; }
	static unsigned int GetUnitLevel() { return m_uiLevel; }

	std::list<col_weak> m_list;

private:
	static float m_fUnit_W;		// 最小レベル空間の幅単位
	static float m_fUnit_H;		// 最小レベル空間の高単位
	static const unsigned int m_uiLevel = 3;			// 最下位レベル
};

// 初期サイズ
float CCell::m_fUnit_W = 100.0f;		// 最小レベル空間の幅単位
float CCell::m_fUnit_H = 100.0f;		// 最小レベル空間の高単位


//===== プロトタイプ宣言 =====
// ビット分割関数
DWORD BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

// 2Dモートン空間番号算出関数
WORD Get2DMortonNumber(WORD x, WORD y)
{
	return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
}

// 座標→線形4分木要素番号変換関数
DWORD GetPointElem(float pos_x, float pos_y)
{
	// 本当はフィールドの大きさとか
	return Get2DMortonNumber((WORD)(pos_x / CCell::GetUnitW()), (WORD)(pos_y / CCell::GetUnitH()));
}


//===== 静的メンバ =====
std::list<std::weak_ptr<CCollision2D>> CCollision2D::m_list;


//========================================
//
//	コンストラクタ
//
//========================================
CCollision2D::CCollision2D()
{

}


//========================================
//
//	デストラクタ
//
//========================================
CCollision2D::~CCollision2D()
{
	for (auto itr = m_list.begin(); itr != m_list.end(); ++itr)
	{
		if (itr->lock() == m_this.lock())
		{
			m_list.erase(itr);
			break;
		}
	}
}


//========================================
//
//	スタート
//
//========================================
void CCollision2D::Start()
{
	m_rb = m_pParent->GetComponent<CRigidbody2D>();
	m_this = m_pParent->GetComponent<CCollision2D>();

	m_offset = Vector3{ 0.0f, 0.0f, 0.0f };
	m_scale = m_pParent->m_pTrans->GetScale();

	m_list.push_back(m_this);

	const auto& rb = m_rb.lock();
	if (rb)
	{
		rb->SetCol(m_this.lock());
	}

#ifdef _DEBUG
	//const auto& r = m_pParent->AddComponent<CMeshRenderer>();
	//r->MakeCube("Collision2D");
	//r->SetDiffuseColor({ 1.0f, 1.0f, 0.0f, 0.2f });
	//r->SetWorldMatrix(&m_mtxWorld);
	//r->SetTranslucentType(eTranslucentType::TT_TRANSLUCENTONLY);
	////r->SetPrimitiveType(ePrimitiveType::PT_LINE);
	//r->SetMeshBlendState(BS_ALPHABLEND);
#endif // _DEBUG


	// ステートの初期化
	m_bOldState = false;
	m_bCurState = false;

	// マップサイズ // 何故か二倍になってる？　現状9600x9600
	CCell::SetMapSize(100.0f * 8 * 6, 100.0f * 8 * 6);
}

//========================================
//
//	更新
//
//========================================
void CCollision2D::Update()
{
	// 過去の状態を保存
	m_bOldState = m_bCurState;

	// 現在の状態を更新
	m_bCurState = false;
}

//========================================
//
//	当たり判定の更新
//
//========================================
void CCollision2D::CollisionUpdate()
{
	// デフォルトの当たり判定
	//DefaultCollisionUpdate();

	// 四分木の当たり判定
	CellCollisionUpdate();
}


//========================================
//
//	描画
//
//========================================
void CCollision2D::CollisionDraw()
{
	//Vector3 pos;
	//Vector3 scale;
	//Vector3 rot;
	//

	//for (const auto& col : m_list)
	//{
	//	pos = col.lock()->m_pParent->m_pTrans->GetPos() + col.lock()->GetOffSet();
	//	scale = col.lock()->GetScale();
	//	rot = col.lock()->m_rot;

	//	XMMATRIX mtxWorld, mtxScale, mtxRot, mtxTranslate;

	//	// ワールドマトリックスの初期化
	//	mtxWorld = XMMatrixIdentity();

	//	// 拡大縮小
	//	mtxScale = XMMatrixScaling(scale->x, scale->y, scale->z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	//	// 回転を反映
	//	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rot->x),
	//		XMConvertToRadians(rot->y), XMConvertToRadians(rot->z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// 移動を反映
	//	mtxTranslate = XMMatrixTranslation(pos->x, pos->y, pos->z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ワールドマトリックス設定
	//	XMStoreFloat4x4(&col.lock()->m_mtxWorld, mtxWorld);
	//}
}


//========================================
//
//	ノードとリストの当たり判定
//
//========================================
void CCollision2D::Collision(const col_weak& main_w, const std::list<col_weak>& m_pSubList)
{
	Vector3 mainPos;
	Vector3 subPos;

	// 実態判定
	const auto& main_s = main_w.lock();
	if (main_s)
	{
		// 当たり判定座標
		mainPos = main_s->m_pParent->m_pTrans->m_pos + main_s->m_offset;

		// サブループ
		for (const auto& sub_w : m_pSubList)
		{
			// 実態判定
			const auto& sub_s = sub_w.lock();
			if (sub_s)
			{
				// 同じだった
				if (main_s == sub_s) continue;

				// 当たり判定座標
				subPos = sub_s->m_pParent->m_pTrans->m_pos + sub_s->m_offset;

				//--- 当たり判定処理 ---
				if (CheckCollisionRectToRect(mainPos, subPos, main_s->m_scale, sub_s->m_scale))
				{
					// 状態を更新
					main_s->m_bCurState = true;
					sub_s->m_bCurState = true;

					// リジッドボディを確認
					const auto& main_rb_s = main_s->m_rb.lock();
					const auto& sub_rb_s = sub_s->m_rb.lock();

					// 押し出し
					if (main_rb_s && sub_rb_s)
					{
						// 押し出し処理
						main_rb_s->Extrusion(sub_rb_s);
						// 相手側の押し出し処理
						sub_rb_s->Extrusion(main_rb_s);
					}

					//--- 当たり判定コールバック ---
					// Enter
					if (main_s->m_bCurState && !main_s->m_bOldState)
					{
						const auto& main_list = main_s->m_pParent->GetComponentList();
						for (const auto& com : main_list)
						{
							com->OnCollisionEnter2D(sub_s);
						}
					}
					// Stay
					else if (main_s->m_bCurState && main_s->m_bOldState)
					{
						const auto& main_list = main_s->m_pParent->GetComponentList();
						for (const auto& com : main_list)
						{
							com->OnCollisionStay2D(sub_s);
						}
					}


					// 相手側
					// Enter
					if (sub_s->m_bCurState && !sub_s->m_bOldState)
					{
						const auto& sub_list = sub_s->m_pParent->GetComponentList();
						for (const auto& com : sub_list)
						{
							com->OnCollisionEnter2D(main_s);
						}
					}
					// Stay
					else if (sub_s->m_bCurState && sub_s->m_bOldState)
					{
						const auto& sub_list = sub_s->m_pParent->GetComponentList();
						for (const auto& com : sub_list)
						{
							com->OnCollisionStay2D(main_s);
						}
					}

					//main_s->m_pParent->OnCollisionEnter2D(sub_s);
					//// 相手側
					//sub_s->m_pParent->OnCollisionEnter2D(main_s);
				}

				//--- 当たり判定コールバック ---
				// Exit
				if (!main_s->m_bCurState && main_s->m_bOldState)
				{
					const auto& main_list = main_s->m_pParent->GetComponentList();
					for (const auto& com : main_list)
					{
						com->OnCollisionExit2D(sub_s);
					}
				}
				// Enter
				if (!sub_s->m_bCurState && sub_s->m_bOldState)
				{
					const auto& sub_list = sub_s->m_pParent->GetComponentList();
					for (const auto& com : sub_list)
					{
						com->OnCollisionExit2D(main_s);
					}
				}

			}
		}
	}

}


//========================================
//
//	デフォルトの当たり判定
//
//========================================
void CCollision2D::DefaultCollisionUpdate()
{
	Vector3 mainPos;
	Vector3 subPos;

	// メインループ
	for (const auto& main_w : m_list)
	{
		// 実態判定
		const auto& main_s = main_w.lock();
		if (main_s)
		{
			// 当たり判定座標
			mainPos = main_s->m_pParent->m_pTrans->m_pos + main_s->m_offset;

			// サブループ
			for (const auto& sub_w : m_list)
			{
				// 実態判定
				const auto& sub_s = sub_w.lock();
				if (sub_s)
				{
					// 同じだった
					if (main_s == sub_s) continue;

					// 当たり判定座標
					subPos = sub_s->m_pParent->m_pTrans->m_pos + sub_s->m_offset;

					//--- 当たり判定処理 ---
					if (CheckCollisionRectToRect(mainPos, subPos, main_s->m_scale, sub_s->m_scale))
					{
						// 状態を更新
						main_s->m_bCurState = true;
						sub_s->m_bCurState = true;

						// リジッドボディを確認
						const auto& main_rb_s = main_s->m_rb.lock();
						const auto& sub_rb_s = sub_s->m_rb.lock();

						// 押し出し
						if (main_rb_s && sub_rb_s)
						{
							// 押し出し処理
							main_rb_s->Extrusion(sub_rb_s);
							// 相手側の押し出し処理
							sub_rb_s->Extrusion(main_rb_s);
						}

						//--- 当たり判定コールバック ---
						// Enter
						if (main_s->m_bCurState && !main_s->m_bOldState)
						{
							const auto& main_list = main_s->m_pParent->GetComponentList();
							for (const auto& com : main_list)
							{
								com->OnCollisionEnter2D(sub_s);
							}
						}
						// Stay
						else if (main_s->m_bCurState && main_s->m_bOldState)
						{
							const auto& main_list = main_s->m_pParent->GetComponentList();
							for (const auto& com : main_list)
							{
								com->OnCollisionStay2D(sub_s);
							}
						}


						// 相手側
						// Enter
						if (sub_s->m_bCurState && !sub_s->m_bOldState)
						{
							const auto& sub_list = sub_s->m_pParent->GetComponentList();
							for (const auto& com : sub_list)
							{
								com->OnCollisionEnter2D(main_s);
							}
						}
						// Stay
						else if (sub_s->m_bCurState && sub_s->m_bOldState)
						{
							const auto& sub_list = sub_s->m_pParent->GetComponentList();
							for (const auto& com : sub_list)
							{
								com->OnCollisionStay2D(main_s);
							}
						}

						//main_s->m_pParent->OnCollisionEnter2D(sub_s);
						//// 相手側
						//sub_s->m_pParent->OnCollisionEnter2D(main_s);
					}

					//--- 当たり判定コールバック ---
					// Exit
					if (!main_s->m_bCurState && main_s->m_bOldState)
					{
						const auto& main_list = main_s->m_pParent->GetComponentList();
						for (const auto& com : main_list)
						{
							com->OnCollisionExit2D(sub_s);
						}
					}
					// Enter
					if (!sub_s->m_bCurState && sub_s->m_bOldState)
					{
						const auto& sub_list = sub_s->m_pParent->GetComponentList();
						for (const auto& com : sub_list)
						{
							com->OnCollisionExit2D(main_s);
						}
					}

				}
			}
		}

	}
}

//========================================
//
//	四分木の当たり判定
//
//========================================
void CCollision2D::CellCollisionUpdate()
{
	// 空間レベルの数
	const unsigned int uiLevel = CCell::GetUnitLevel();
	const unsigned int nMaxCell = MAX_CELL;

	// 空間の作成
	CCell mainCell[MAX_CELL];
	CCell subCell[MAX_CELL];
	// モートン番号
	DWORD Def = 0;
	DWORD wLeftTop = 0;
	DWORD wRightDown = 0;
	// 座標・スケール
	Vector3 pos;
	Vector3 scale;


	// リスト内の一斉更新
	for (const auto& col_w : m_list)
	{
		// コリジョンコンポーネント
		const auto& col_s = col_w.lock();

		// 実態がない
		if (!col_s) continue;

		// トランスフォーム取得
		//const auto& trans = col_s->m_pParent->m_pTrans;

		// 座標＋オフセット
		pos = col_s->m_pParent->m_pTrans->m_pos + col_s->m_offset;
		// スケール
		scale = col_s->m_scale;


		// ここで空間の登録をする
		// 左上と右下を出す
		wLeftTop = GetPointElem(pos->x - scale->x / 2,
			pos->y - scale->y / 2);
		wRightDown = GetPointElem(pos->x + scale->x / 2,
			pos->y + scale->y / 2);
		if (wLeftTop >= nMaxCell - 1 || wRightDown >= nMaxCell - 1)
		{
			continue;
		}

		// XORをとる	
		Def = wLeftTop ^ wRightDown;
		unsigned int HiLevel = 0;
		unsigned int i;
		for (i = 0; i < uiLevel; i++)
		{
			DWORD Check = (Def >> (i * 2)) & 0x3;
			if (Check != 0)
				HiLevel = i + 1;
		}
		DWORD SpaceNum = wRightDown >> (HiLevel * 2);
		int nPow4 = 1;
		for (i = 0; i < uiLevel - HiLevel; i++) nPow4 *= 4;
		DWORD AddNum = (nPow4 - 1) / 3;
		SpaceNum += AddNum;	// これが今いる空間

		// 空間外ははじく
		if (SpaceNum > nMaxCell - 1) continue;

		// 今いる空間のメインリストに格納
		mainCell[SpaceNum].GetList().push_back(col_s);

		// 今いる空間の親のサブに格納
		while (SpaceNum > 0)
		{
			SpaceNum--;
			SpaceNum /= 4;

			subCell[SpaceNum].GetList().push_back(col_s);
		}

		// ここでダブルループで、他のキャラと総当たりで当たり判定を取る
		//(*col).Collision(pNode);
	}

	// ここでそれぞれの空間内でのに当たり判定を取る
	for (int i = 0; i < nMaxCell; i++)
	{
		for (const auto& main_s : mainCell[i].GetList())
		{
			// 当たり判定を取る
			Collision(main_s, mainCell[i].GetList());
		}
	}

	// 次に親から子への当たり判定を取る
	for (int i = 0; i < nMaxCell; i++)
	{
		//for (auto itr = mainCell[i].m_list.begin(); itr != mainCell[i].m_list.end(); ++itr)
		for (const auto& main_s : mainCell[i].GetList())
		{
			// 当たり判定を取る
			Collision(main_s, subCell[i].GetList());
		}
	}
}



// 線分の衝突
bool CCollision2D::CheckCollisionLine(Vector3 pos1, Vector3 pos2, Vector3 pos3, Vector3 pos4, Vector3* outPos, float *outT1) 
{

	Vector3 v = { pos3->x - pos1->x, pos3->y - pos1->y, 0.0f };
	// ベクトル
	Vector3 vec1 = { pos2->x - pos1->x, pos2->y - pos1->y, 0.0f };
	Vector3 vec2 = { pos4->x - pos3->x, pos4->y - pos3->y, 0.0f };

	float Crs_v1_v2 = Vector3::Cross2D(vec1, vec2);
	if (Crs_v1_v2 == 0.0f) {
		// 平行状態
		return false;
	}

	float Crs_v_v1 = Vector3::Cross2D(v, vec1);
	float Crs_v_v2 = Vector3::Cross2D(v, vec2);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	if(outT1)
		*outT1 = t2;

	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// 交差していない
		return false;
	}

	if (outPos)
	{
		//*outPos = seg1.s + seg1.v * t1;
		(*outPos)->x = pos1->x + vec1->x * t1;
		(*outPos)->y = pos1->y + vec1->y * t1;
		(*outPos)->z = 0;
	}
	return true;
}



//*******************************
//
//	矩形と矩形の当たり判定
//	
//	引数:
//		矩形１の中心座標
//		矩形２の中心座標
//		矩形１のサイズ
//		矩形２のサイズ
//
//	戻り値
//		false：接触していない
//		true：接触している
//
//*******************************
bool CCollision2D::CheckCollisionRectToRect(Vector3 centerPos1, Vector3 centerPos2, Vector3 size1, Vector3 size2)
{
	Vector3 halfSize1 = Vector3{ size1->x / 2.0f, size1->y / 2.0f , 0.0f};
	Vector3 halfSize2 = Vector3{ size2->x / 2.0f, size2->y / 2.0f , 0.0f };

	if ((centerPos2->x - halfSize2->x < centerPos1->x + halfSize1->x) &&		// 2の左端 < 1の右端
		(centerPos1->x - halfSize1->x < centerPos2->x + halfSize2->x ))		// 1の左端 < 2の右端
	{
		if ((centerPos2->y - halfSize2->y < centerPos1->y + halfSize1->y) &&		// 2の上端 < 1の下端
			(centerPos1->y - halfSize1->y < centerPos2->y + halfSize2->y))		// 1の上端 < 2の下端
		{
			return true;
		}
	}



	return false;
}




//*******************************
//
//	円と円の当たり判定
//	
//	引数:
//		円１の中心座標
//		円２の中心座標
//		円１の半径
//		円２の半径
//
//	戻り値
//		false：接触していない
//		true：接触している
//
//*******************************
bool CCollision2D::CheckCollisionCircleToCircle(Vector3 centerPos1, Vector3 centerPos2, float radius1, float radius2)
{
	float fX = centerPos1->x - centerPos2->x;
	float fY = centerPos1->y - centerPos2->y;
	float fR = radius1 + radius2;

	if (fX * fX + fY * fY		// (円1の中心座標X - 円2の中心座標X)の2乗 + (円1の中心座標Y - 円2の中心座標Y)の2乗
		<= fR * fR)				// (円1の半径+円2の半径)の2乗
	{
		return true;
	}

	return false;
}


//*******************************
//
//	円と矩形の当たり判定
//	
//	引数:
//		円１の中心座標
//		矩形２の中心座標
//		円１の半径
//		矩形２の大きさ
//
//	戻り値
//		false：接触していない
//		true：接触している
//
//*******************************
bool CCollision2D::CheckCollisionCircleToRect(Vector3 centerPos1, Vector3 centerPos2, float radius1, Vector3 size2)
{
	// 円
	float x = centerPos1->x;
	float y = centerPos1->y;
	float radius = radius1;

	// 矩形
	float L = centerPos2->x - size2->x / 2.0f;
	float R = centerPos2->x + size2->x / 2.0f;
	float T = centerPos2->y - size2->y / 2.0f;
	float B = centerPos2->y + size2->y / 2.0f;


	if (L - radius > x || R + radius < x || T - radius > y || B + radius < y) {//矩形の領域判定1
		return false;
	}
	if (L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < radius * radius)) {//左上の当たり判定
		return false;
	}
	if (R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < radius * radius)) {//右上の当たり判定
		return false;
	}
	if (L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < radius * radius)) {//左下の当たり判定
		return false;
	}
	if (R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < radius * radius)) {//右下の当たり判定
		return false;
	}
	return true;//すべての条件が外れたときに当たっている
}