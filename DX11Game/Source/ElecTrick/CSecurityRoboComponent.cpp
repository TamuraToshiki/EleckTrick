//======================================================================
//											CSecurityRoboComponent.h
//	�x�����{�R���|�[�l���g
//
//======================================================================
//	author :	AT12A 18 �c���q��
//======================================================================
//	�J������
//	2020/12/04	�쐬(�D��x�����������ߌ��)
//
//======================================================================

//====== �C���N���[�h�� ======
#include "CObject.h"
#include "CObjectManager.h"
#include "CSecurityRoboComponent.h"
#include "CVisible.h"
#include "Sound.h"
#include "CGearCaterpillarComponent.h"

#include "CEffectComponent.h"


//===== �}�N����` =====


//===== �v���g�^�C�v�錾 =====


//===== �O���[�o���ϐ� =====


//===== �񋓑̐錾 =====

//===== �萔��` =====
#define EyeScale	Vector3{ 200.0f,50.0f,100.0f }
#define EyeOffset	Vector3{ (EyeScale->x * -1),50.0f,0.0f }
#define U_turn_OFFSET	Vector3{ -70.0f,50.0f,0.0f}


//===================================
//
//	�X�^�[�g�֐�
//
//===================================
void CSecurityRoboComponent::Start()
{
	CEnemyBase::Start();

	// �x�����{�ɖځA�X�^���h�A�Z���T�[���Z�b�g����v���O����
	m_visi.lock()->SetTargetEnemy(m_pParent->GetComponent<CSecurityRoboComponent>(), EyeScale);
	m_stand.lock()->SetStandStick(m_pParent->GetComponent<CSecurityRoboComponent>());
	m_blsen.lock()->SetBlocksensorComponent(m_pParent->GetComponent<CSecurityRoboComponent>());

	npt_cnt = 0;										// �p�^�[���J�E���g
	m_bDelayStand = true;								// �R�Ŕ��]����t���O
	spd = 2;											// �G�l�~�[�̑���
	m_rb = m_pParent->GetComponent<CRigidbody2D>();		// ���W�b�g�{�f�B�Z�b�g
	vec = Vector3{ -1.0f,0.0f,0.0f };					// �x�N�g��
	Eye = EyeOffset;									// �ڂ̕���
	m_U_turn = U_turn_OFFSET;
	m_pParent->m_pTrans->m_rot->y = 90;					// �������{�b�g�̌����Ă����
	m_visi.lock()->SetDir(Eye);
	m_blsen.lock()->SetDir(m_U_turn);

	// �A�j���[�V����
	m_renderer = m_pParent->GetComponent<CAssimpRenderer>();
	m_renderer.lock()->StopAnim();

	m_energyOffTexture = "data/model/EnemyA_Mix/EnemyA_Tex4.png";
	m_energyOnTexture  = "data/model/EnemyA_Mix/EnemyA_Tex4.png";

	// �������W
	m_initPos = m_pParent->m_pTrans->m_pos;


	//�@�M�A�A�L���^�s��
	// �L���^�s���̐���
	const auto& cater = CObjectManager::CreateObject<CAssimpObject>();
	cater->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_caterpollar = cater->GetComponent<CAssimpRenderer>();
	m_caterpollar.lock()->ModelLoad("data/model/Enemy_Anim_Blender/Enemy_Caterpillar_Anim.fbx");
	m_caterpollar.lock()->SetDiffuseTexture("data/model/EnemyA_Mix/EnemyA_Tex4.png");
	m_caterpollar.lock()->SetAnimIndex(0);
	m_caterpollar.lock()->SetAnimSpeed(2);
	m_caterpollar.lock()->PlayAnim();

	// �M�A�̐���
	const auto& gear = CObjectManager::CreateObject<CAssimpObject>();
	gear->AddComponent<CGearCaterpillarComponent>()->SetPlayer(m_pParent->m_pTrans);
	m_gear = gear->GetComponent<CAssimpRenderer>();
	m_gear.lock()->ModelLoad("data/model/Enemy_Anim_Blender/Gear_Anim_Blender.fbx");
	m_gear.lock()->SetAnimIndex(1);
	m_gear.lock()->SetAnimSpeed(2);
	m_gear.lock()->PlayAnim();
}

//===================================
//
//	���X�^�[�g
//
//===================================
void CSecurityRoboComponent::Restart()
{
	spd = 2;
	vec = Vector3{ -1.0f,0.0f,0.0f };					// �x�N�g��

	m_pParent->m_pTrans->m_pos = m_initPos;

}

//===================================
//
//	��X�V
//
//===================================
void CSecurityRoboComponent::LateUpdate()
{
	// �A�j���[�V�����X�V����

	// ���x
	Vector3 velo = m_rb.lock()->GetVelocity();

	// �A�j���[�V����
	float animSpeed = velo->x + velo->z;
	animSpeed = fabsf(animSpeed);

	m_caterpollar.lock()->SetAnimSpeed(animSpeed);
	m_gear.lock()->SetAnimSpeed(animSpeed);
}

//===================================
//
//	�I��
//
//===================================
void CSecurityRoboComponent::End()
{
	const auto& cata = m_caterpollar.lock();
	if (cata)
		CObjectManager::DestroyObject(cata->m_pParent);

	const auto& gaer = m_gear.lock();
	if (gaer)
		CObjectManager::DestroyObject(gaer->m_pParent);

	CEnemyBase::End();
}

//===================================
//
//	���֐�
//
//===================================
void CSecurityRoboComponent::EngagementEnemy()
{

	if (++npt_cnt == 1)// 1f�@���������Ƃ���
	{
		m_rb.lock()->SetForce({ 0.0f,2.0f,0.0f });	// ��яオ��
		// �T�E���h
		CSound::PlaySE("EnemyDiscovery.wav");
	}
	if (npt_cnt >= 30)// 0.5�b��ɒǂ�������
	{
		vec = m_player.lock()->m_pTrans->GetPos() - m_pParent->m_pTrans->GetPos();	// ���� = �v���C���[ - ���g 
		vec = Vector3::Normalize(vec);	// �����𐳋K��
		m_rb.lock()->SetForceX((spd * 2) * vec->x);	// X���ɑ��� * 2 �Œǂ�������
		// �A�j���[�V����
		m_renderer.lock()->PlayAnim();
	}
}

//===================================
//
//	�ʏ�ړ��֐�
//
//===================================
void CSecurityRoboComponent::NormalEnemy()
{
	if (m_rb.lock()->GetHitDir() == DIRECTION::DOWN)	// �n�ʂɐڂ��Ă�Ȃ�
	{
		m_rb.lock()->SetForceX(vec->x * spd);	// ���� * ����
		if (m_bStandflg)	// �X�^���h���n�ʂɐڂ��Ă�Ȃ�
		{
			m_bDelayStand = false;	// ���]���ĂȂ����Ƃɂ���
			npt_cnt = 0;	// �p�^�[��������������
			if (m_bBlockflg)	// �Z���T�[�ǂɐڂ��Ă�Ȃ�
			{
				m_pParent->m_pTrans->m_rot->y *= -1;	// �G�l�~�[�̌����𔽓]����
				vec->x *= -1;	// �ړ������𔽓]���܂�
				Eye->x *= -1;	// �ڂ̕����𔽓]���܂�
				m_U_turn->x *= -1;
				m_visi.lock()->SetDir(Eye);	// ���]�����ڂ̕������Z�b�g���܂�
				m_blsen.lock()->SetDir(m_U_turn);
			}
		}
		else
		{
			if (!m_bDelayStand)
			{
				m_pParent->m_pTrans->m_rot->y *= -1;// �G�l�~�[�̌����𔽓]����
				vec->x *= -1;	// �ړ������𔽓]���܂�
				Eye->x *= -1;	// �ڂ̕����𔽓]���܂�
				m_U_turn->x *= -1;
				m_visi.lock()->SetDir(Eye);	// ���]�����ڂ̕������Z�b�g���܂�
				m_blsen.lock()->SetDir(m_U_turn);
				m_bDelayStand = true;	// ���]�������Ƃɂ���
			}
		}

		// �x�����{�̌������h�~�v���O����
		if (vec->x > 0 && m_pParent->m_pTrans->m_rot->y > 0)
		{
			vec->x *= -1;
		}
		else if (vec->x < 0 && m_pParent->m_pTrans->m_rot->y < 0)
		{
			vec->x *= -1;
		}
	}

	// �A�j���[�V����
	m_renderer.lock()->StopAnim();
}

//===================================
//
//	�@�\��~�֐�
//
//===================================
void CSecurityRoboComponent::Outage()
{
	m_rb.lock()->SetForce({ 0,0,0 });
	m_fUseResource = 0.1f;

	// �G�t�F�N�g�̐���
	Vector3 pos = m_pParent->m_pTrans->m_pos + 
		Vector3{(float)(rand() % 100) - 50, (float)(rand() % 100) - 50, 0};

	const auto& obj = CObjectManager::CreateObject<CObject>(pos);
	obj->AddComponent<CEffectComponent>()->SetEffect(CEffectComponent::eSmokeEffect);
	const auto& rb = obj->AddComponent<CRigidbody2D>();

	rb->SetGravityForce(0.0f);
	rb->SetForceY(2.0f);

}
