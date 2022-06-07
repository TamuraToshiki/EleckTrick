//==================================================================
//									collision.h
//	�R���W����
//
//==================================================================
//	author :	AT12A 05 �F�����W�V
//==================================================================
//	�J������
//
//	2020/06/24	�R���W����
//
//===================================================================

//====== �C���N���[�h�� ======
#include "CCollision2D.h"
#include "CObject.h"
#include "CRigidbody2D.h"
#include "polygon.h"
#include "Renderer.h"


//===== �}�N����` =====
#define MAX_CELL (1+4+16+64)

//===== �񋓌^ =====
enum ECollider
{
	eRECTANGLE,
	eCIRCLE,

	MAX_COLLISION_TYPE,
};

//===== �N���X��` =====

// �^��`
using col_weak = std::weak_ptr<CCollision2D>;

// �l���؋��
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
	static float m_fUnit_W;		// �ŏ����x����Ԃ̕��P��
	static float m_fUnit_H;		// �ŏ����x����Ԃ̍��P��
	static const unsigned int m_uiLevel = 3;			// �ŉ��ʃ��x��
};

// �����T�C�Y
float CCell::m_fUnit_W = 100.0f;		// �ŏ����x����Ԃ̕��P��
float CCell::m_fUnit_H = 100.0f;		// �ŏ����x����Ԃ̍��P��


//===== �v���g�^�C�v�錾 =====
// �r�b�g�����֐�
DWORD BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

// 2D���[�g����Ԕԍ��Z�o�֐�
WORD Get2DMortonNumber(WORD x, WORD y)
{
	return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
}

// ���W�����`4���ؗv�f�ԍ��ϊ��֐�
DWORD GetPointElem(float pos_x, float pos_y)
{
	// �{���̓t�B�[���h�̑傫���Ƃ�
	return Get2DMortonNumber((WORD)(pos_x / CCell::GetUnitW()), (WORD)(pos_y / CCell::GetUnitH()));
}


//===== �ÓI�����o =====
std::list<std::weak_ptr<CCollision2D>> CCollision2D::m_list;


//========================================
//
//	�R���X�g���N�^
//
//========================================
CCollision2D::CCollision2D()
{

}


//========================================
//
//	�f�X�g���N�^
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
//	�X�^�[�g
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


	// �X�e�[�g�̏�����
	m_bOldState = false;
	m_bCurState = false;

	// �}�b�v�T�C�Y // ���̂���{�ɂȂ��Ă�H�@����9600x9600
	CCell::SetMapSize(100.0f * 8 * 6, 100.0f * 8 * 6);
}

//========================================
//
//	�X�V
//
//========================================
void CCollision2D::Update()
{
	// �ߋ��̏�Ԃ�ۑ�
	m_bOldState = m_bCurState;

	// ���݂̏�Ԃ��X�V
	m_bCurState = false;
}

//========================================
//
//	�����蔻��̍X�V
//
//========================================
void CCollision2D::CollisionUpdate()
{
	// �f�t�H���g�̓����蔻��
	//DefaultCollisionUpdate();

	// �l���؂̓����蔻��
	CellCollisionUpdate();
}


//========================================
//
//	�`��
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

	//	// ���[���h�}�g���b�N�X�̏�����
	//	mtxWorld = XMMatrixIdentity();

	//	// �g��k��
	//	mtxScale = XMMatrixScaling(scale->x, scale->y, scale->z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	//	// ��]�𔽉f
	//	mtxRot = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rot->x),
	//		XMConvertToRadians(rot->y), XMConvertToRadians(rot->z));
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxRot);

	//	// �ړ��𔽉f
	//	mtxTranslate = XMMatrixTranslation(pos->x, pos->y, pos->z);
	//	mtxWorld = XMMatrixMultiply(mtxWorld, mtxTranslate);

	//	// ���[���h�}�g���b�N�X�ݒ�
	//	XMStoreFloat4x4(&col.lock()->m_mtxWorld, mtxWorld);
	//}
}


//========================================
//
//	�m�[�h�ƃ��X�g�̓����蔻��
//
//========================================
void CCollision2D::Collision(const col_weak& main_w, const std::list<col_weak>& m_pSubList)
{
	Vector3 mainPos;
	Vector3 subPos;

	// ���Ԕ���
	const auto& main_s = main_w.lock();
	if (main_s)
	{
		// �����蔻����W
		mainPos = main_s->m_pParent->m_pTrans->m_pos + main_s->m_offset;

		// �T�u���[�v
		for (const auto& sub_w : m_pSubList)
		{
			// ���Ԕ���
			const auto& sub_s = sub_w.lock();
			if (sub_s)
			{
				// ����������
				if (main_s == sub_s) continue;

				// �����蔻����W
				subPos = sub_s->m_pParent->m_pTrans->m_pos + sub_s->m_offset;

				//--- �����蔻�菈�� ---
				if (CheckCollisionRectToRect(mainPos, subPos, main_s->m_scale, sub_s->m_scale))
				{
					// ��Ԃ��X�V
					main_s->m_bCurState = true;
					sub_s->m_bCurState = true;

					// ���W�b�h�{�f�B���m�F
					const auto& main_rb_s = main_s->m_rb.lock();
					const auto& sub_rb_s = sub_s->m_rb.lock();

					// �����o��
					if (main_rb_s && sub_rb_s)
					{
						// �����o������
						main_rb_s->Extrusion(sub_rb_s);
						// ���葤�̉����o������
						sub_rb_s->Extrusion(main_rb_s);
					}

					//--- �����蔻��R�[���o�b�N ---
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


					// ���葤
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
					//// ���葤
					//sub_s->m_pParent->OnCollisionEnter2D(main_s);
				}

				//--- �����蔻��R�[���o�b�N ---
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
//	�f�t�H���g�̓����蔻��
//
//========================================
void CCollision2D::DefaultCollisionUpdate()
{
	Vector3 mainPos;
	Vector3 subPos;

	// ���C�����[�v
	for (const auto& main_w : m_list)
	{
		// ���Ԕ���
		const auto& main_s = main_w.lock();
		if (main_s)
		{
			// �����蔻����W
			mainPos = main_s->m_pParent->m_pTrans->m_pos + main_s->m_offset;

			// �T�u���[�v
			for (const auto& sub_w : m_list)
			{
				// ���Ԕ���
				const auto& sub_s = sub_w.lock();
				if (sub_s)
				{
					// ����������
					if (main_s == sub_s) continue;

					// �����蔻����W
					subPos = sub_s->m_pParent->m_pTrans->m_pos + sub_s->m_offset;

					//--- �����蔻�菈�� ---
					if (CheckCollisionRectToRect(mainPos, subPos, main_s->m_scale, sub_s->m_scale))
					{
						// ��Ԃ��X�V
						main_s->m_bCurState = true;
						sub_s->m_bCurState = true;

						// ���W�b�h�{�f�B���m�F
						const auto& main_rb_s = main_s->m_rb.lock();
						const auto& sub_rb_s = sub_s->m_rb.lock();

						// �����o��
						if (main_rb_s && sub_rb_s)
						{
							// �����o������
							main_rb_s->Extrusion(sub_rb_s);
							// ���葤�̉����o������
							sub_rb_s->Extrusion(main_rb_s);
						}

						//--- �����蔻��R�[���o�b�N ---
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


						// ���葤
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
						//// ���葤
						//sub_s->m_pParent->OnCollisionEnter2D(main_s);
					}

					//--- �����蔻��R�[���o�b�N ---
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
//	�l���؂̓����蔻��
//
//========================================
void CCollision2D::CellCollisionUpdate()
{
	// ��ԃ��x���̐�
	const unsigned int uiLevel = CCell::GetUnitLevel();
	const unsigned int nMaxCell = MAX_CELL;

	// ��Ԃ̍쐬
	CCell mainCell[MAX_CELL];
	CCell subCell[MAX_CELL];
	// ���[�g���ԍ�
	DWORD Def = 0;
	DWORD wLeftTop = 0;
	DWORD wRightDown = 0;
	// ���W�E�X�P�[��
	Vector3 pos;
	Vector3 scale;


	// ���X�g���̈�čX�V
	for (const auto& col_w : m_list)
	{
		// �R���W�����R���|�[�l���g
		const auto& col_s = col_w.lock();

		// ���Ԃ��Ȃ�
		if (!col_s) continue;

		// �g�����X�t�H�[���擾
		//const auto& trans = col_s->m_pParent->m_pTrans;

		// ���W�{�I�t�Z�b�g
		pos = col_s->m_pParent->m_pTrans->m_pos + col_s->m_offset;
		// �X�P�[��
		scale = col_s->m_scale;


		// �����ŋ�Ԃ̓o�^������
		// ����ƉE�����o��
		wLeftTop = GetPointElem(pos->x - scale->x / 2,
			pos->y - scale->y / 2);
		wRightDown = GetPointElem(pos->x + scale->x / 2,
			pos->y + scale->y / 2);
		if (wLeftTop >= nMaxCell - 1 || wRightDown >= nMaxCell - 1)
		{
			continue;
		}

		// XOR���Ƃ�	
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
		SpaceNum += AddNum;	// ���ꂪ��������

		// ��ԊO�͂͂���
		if (SpaceNum > nMaxCell - 1) continue;

		// �������Ԃ̃��C�����X�g�Ɋi�[
		mainCell[SpaceNum].GetList().push_back(col_s);

		// �������Ԃ̐e�̃T�u�Ɋi�[
		while (SpaceNum > 0)
		{
			SpaceNum--;
			SpaceNum /= 4;

			subCell[SpaceNum].GetList().push_back(col_s);
		}

		// �����Ń_�u�����[�v�ŁA���̃L�����Ƒ�������œ����蔻������
		//(*col).Collision(pNode);
	}

	// �����ł��ꂼ��̋�ԓ��ł̂ɓ����蔻������
	for (int i = 0; i < nMaxCell; i++)
	{
		for (const auto& main_s : mainCell[i].GetList())
		{
			// �����蔻������
			Collision(main_s, mainCell[i].GetList());
		}
	}

	// ���ɐe����q�ւ̓����蔻������
	for (int i = 0; i < nMaxCell; i++)
	{
		//for (auto itr = mainCell[i].m_list.begin(); itr != mainCell[i].m_list.end(); ++itr)
		for (const auto& main_s : mainCell[i].GetList())
		{
			// �����蔻������
			Collision(main_s, subCell[i].GetList());
		}
	}
}



// �����̏Փ�
bool CCollision2D::CheckCollisionLine(Vector3 pos1, Vector3 pos2, Vector3 pos3, Vector3 pos4, Vector3* outPos, float *outT1) 
{

	Vector3 v = { pos3->x - pos1->x, pos3->y - pos1->y, 0.0f };
	// �x�N�g��
	Vector3 vec1 = { pos2->x - pos1->x, pos2->y - pos1->y, 0.0f };
	Vector3 vec2 = { pos4->x - pos3->x, pos4->y - pos3->y, 0.0f };

	float Crs_v1_v2 = Vector3::Cross2D(vec1, vec2);
	if (Crs_v1_v2 == 0.0f) {
		// ���s���
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
		// �������Ă��Ȃ�
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
//	��`�Ƌ�`�̓����蔻��
//	
//	����:
//		��`�P�̒��S���W
//		��`�Q�̒��S���W
//		��`�P�̃T�C�Y
//		��`�Q�̃T�C�Y
//
//	�߂�l
//		false�F�ڐG���Ă��Ȃ�
//		true�F�ڐG���Ă���
//
//*******************************
bool CCollision2D::CheckCollisionRectToRect(Vector3 centerPos1, Vector3 centerPos2, Vector3 size1, Vector3 size2)
{
	Vector3 halfSize1 = Vector3{ size1->x / 2.0f, size1->y / 2.0f , 0.0f};
	Vector3 halfSize2 = Vector3{ size2->x / 2.0f, size2->y / 2.0f , 0.0f };

	if ((centerPos2->x - halfSize2->x < centerPos1->x + halfSize1->x) &&		// 2�̍��[ < 1�̉E�[
		(centerPos1->x - halfSize1->x < centerPos2->x + halfSize2->x ))		// 1�̍��[ < 2�̉E�[
	{
		if ((centerPos2->y - halfSize2->y < centerPos1->y + halfSize1->y) &&		// 2�̏�[ < 1�̉��[
			(centerPos1->y - halfSize1->y < centerPos2->y + halfSize2->y))		// 1�̏�[ < 2�̉��[
		{
			return true;
		}
	}



	return false;
}




//*******************************
//
//	�~�Ɖ~�̓����蔻��
//	
//	����:
//		�~�P�̒��S���W
//		�~�Q�̒��S���W
//		�~�P�̔��a
//		�~�Q�̔��a
//
//	�߂�l
//		false�F�ڐG���Ă��Ȃ�
//		true�F�ڐG���Ă���
//
//*******************************
bool CCollision2D::CheckCollisionCircleToCircle(Vector3 centerPos1, Vector3 centerPos2, float radius1, float radius2)
{
	float fX = centerPos1->x - centerPos2->x;
	float fY = centerPos1->y - centerPos2->y;
	float fR = radius1 + radius2;

	if (fX * fX + fY * fY		// (�~1�̒��S���WX - �~2�̒��S���WX)��2�� + (�~1�̒��S���WY - �~2�̒��S���WY)��2��
		<= fR * fR)				// (�~1�̔��a+�~2�̔��a)��2��
	{
		return true;
	}

	return false;
}


//*******************************
//
//	�~�Ƌ�`�̓����蔻��
//	
//	����:
//		�~�P�̒��S���W
//		��`�Q�̒��S���W
//		�~�P�̔��a
//		��`�Q�̑傫��
//
//	�߂�l
//		false�F�ڐG���Ă��Ȃ�
//		true�F�ڐG���Ă���
//
//*******************************
bool CCollision2D::CheckCollisionCircleToRect(Vector3 centerPos1, Vector3 centerPos2, float radius1, Vector3 size2)
{
	// �~
	float x = centerPos1->x;
	float y = centerPos1->y;
	float radius = radius1;

	// ��`
	float L = centerPos2->x - size2->x / 2.0f;
	float R = centerPos2->x + size2->x / 2.0f;
	float T = centerPos2->y - size2->y / 2.0f;
	float B = centerPos2->y + size2->y / 2.0f;


	if (L - radius > x || R + radius < x || T - radius > y || B + radius < y) {//��`�̗̈攻��1
		return false;
	}
	if (L > x && T > y && !((L - x) * (L - x) + (T - y) * (T - y) < radius * radius)) {//����̓����蔻��
		return false;
	}
	if (R < x && T > y && !((R - x) * (R - x) + (T - y) * (T - y) < radius * radius)) {//�E��̓����蔻��
		return false;
	}
	if (L > x && B < y && !((L - x) * (L - x) + (B - y) * (B - y) < radius * radius)) {//�����̓����蔻��
		return false;
	}
	if (R < x && B < y && !((R - x) * (R - x) + (B - y) * (B - y) < radius * radius)) {//�E���̓����蔻��
		return false;
	}
	return true;//���ׂĂ̏������O�ꂽ�Ƃ��ɓ������Ă���
}