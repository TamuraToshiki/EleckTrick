#include "Renderer.h"
#include "transform.h"
#include "Math.h"


std::map<const std::string, CAssimpModel*, std::less<>> CAssimpRenderer::m_assimpModelPool;
std::map<const CAssimpModel*, TAssimpMaterial*, std::less<>> CAssimpRenderer::m_materialPool;


std::list<CRenderer*> CRenderer::m_meshList;


std::list<CRenderer*> CRenderer::m_opaqueList;
std::list<CRenderer*> CRenderer::m_UIList;

std::map<const std::string, MESH*, std::less<>> CMeshRenderer::m_meshPool;

MESH CBillboardRenderer::m_masterMesh;



class Plane
{
public:
    Vector3 normal;
    Vector3 position;
    static const int Length = 6;

    float GetDistanceToPoint(Vector3 pos)
    {
        return Vector3::Dot(pos - position, normal.normalized());
    }
};

class CheckFrustum
{
public:

    enum State
    {
        Outside, Inside, Intersect,
    };

    /// 

    /// 対象AABBとProjection Matrixから視錐台内に入っているかの検知を行う
    /// 
    /// AABB対象
    /// Projection Matrix
    /// カメラ位置
    /// カメラのNear
    /// カメラのFar
    /// 
    static State Detect(CTransform* target, XMFLOAT4X4 pmat, CCamera* pCamera, float fNear, float fFar)
    {
        Plane planes[Plane::Length];
        CalculateFrustumPlanes(pmat, pCamera, fNear, fFar, planes);

        State result = State::Inside;

        for (int i = 0; i < planes->Length; i++)
        {
            Vector3 normal = planes[i].normal;
            Vector3 vp = GetPositivePoint(target, normal);
            Vector3 vn = GetNegativePoint(target, normal);

            // (vp - plane.pos)・normal
            float dp = planes[i].GetDistanceToPoint(vp);

            if (dp < 0)
            {
                return State::Outside;
            }

            float dn = planes[i].GetDistanceToPoint(vn);
            if (dn < 0)
            {
                result = State::Intersect;
            }
        }

        return result;
    }


private:

    /// 

    /// 法線から一番近い点を算出する
    /// 
    /// ターゲットとなるAABB
    /// 算出する法線
    /// 
    static Vector3 GetPositivePoint(CTransform* target, Vector3 normal)
    {
        Vector3 result = target->GetMin();

        if (normal->x > 0)
        {
            result->x += target->m_scale->x;
        }
        if (normal->y > 0)
        {
            result->y += target->m_scale->y;
        }
        if (normal->z > 0)
        {
            result->z += target->m_scale->z;
        }

        return result;
    }

    /// 

    /// 法線から一番遠い点を算出する
    /// 
    /// ターゲットとなるAABB
    /// 算出する法線
    /// 
    static Vector3 GetNegativePoint(CTransform* target, Vector3 normal)
    {
        Vector3 result = target->GetMin();

        if (normal->x < 0)
        {
            result->x += target->m_scale->x;
        }
        if (normal->y < 0)
        {
            result->y += target->m_scale->y;
        }
        if (normal->z < 0)
        {
            result->z += target->m_scale->z;
        }

        return result;
    }

    /// 

    /// 指定されたProjection Matricsから視錐台の6面の平面を求める
    /// 
    /// Projection Matrix
    /// カメラ位置
    /// カメラのNear
    /// カメラのFar
    /// 
    static Plane* CalculateFrustumPlanes(XMFLOAT4X4 pmat, CCamera* pCamera, float fNear, float fFar, Plane* pPlane)
    {
        Plane* result = pPlane;

        // 0: Left, 1: Right, 2: Bottm, 3: Top
        for (int i = 0; i < 4; i++)
        {
            float a, b, c, d;
            int r = i / 2;
            if (i % 2 == 0)
            {
                // 平面の方程式
                // ax + by + cz + d = 0
                a = pmat.m[3][0] - pmat.m[r][0];
                b = pmat.m[3][1] - pmat.m[r][1];
                c = pmat.m[3][2] - pmat.m[r][2];
                d = pmat.m[3][3] - pmat.m[r][3];
            }
            else
            {
                a = pmat.m[3][0] + pmat.m[r][0];
                b = pmat.m[3][1] + pmat.m[r][1];
                c = pmat.m[3][2] + pmat.m[r][2];
                d = pmat.m[3][3] + pmat.m[r][3];
            }

            Vector3 normal = Vector3(a, b, c).normalized() * -1;
            //XMVECTOR q = XMQuaternionRotationMatrix(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
            //q = XMQuaternionMultiply(q, XMLoadFloat3(normal.GetFloat3()));

            XMVECTOR q = XMVector3TransformCoord(XMLoadFloat3(normal.GetFloat3()),
                XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix())));
            XMStoreFloat3(normal.GetFloat3(), q);

            result[i] = Plane{ normal, pCamera->GetPos() };
        }

        // for the near plane
        {
            float a = pmat.m[3][0] + pmat.m[2][0];
            float b = pmat.m[3][1] + pmat.m[2][1];
            float c = pmat.m[3][2] + pmat.m[2][2];
            float d = pmat.m[3][3] + pmat.m[2][3];

            Vector3 normal = Vector3(a, b, c).normalized() * -1;
            //XMVECTOR q = XMQuaternionRotationMatrix(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
            //q = XMQuaternionMultiply(q, XMLoadFloat3(normal.GetFloat3()));

            XMVECTOR q = XMVector3TransformCoord(XMLoadFloat3(normal.GetFloat3()),
                XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix())));
            XMStoreFloat3(normal.GetFloat3(), q);

            Vector3 pos = Vector3(pCamera->GetPos()) + (pCamera->GetForward() * fNear);
            result[4] = Plane{ normal, pos };
        }

        // for the far plane
        {
            float a = pmat.m[3, 0] - pmat.m[2, 0];
            float b = pmat.m[3, 1] - pmat.m[2, 1];
            float c = pmat.m[3, 2] - pmat.m[2, 2];
            float d = pmat.m[3, 3] - pmat.m[2, 3];

            Vector3 normal = Vector3(a, b, c).normalized() * -1;
            //XMVECTOR q = XMQuaternionRotationMatrix(XMLoadFloat4x4(&pCamera->GetViewMatrix()));
            //q = XMQuaternionMultiply(q, XMLoadFloat3(normal.GetFloat3()));

            XMVECTOR q = XMVector3TransformCoord(XMLoadFloat3(normal.GetFloat3()),
                XMMatrixTranspose(XMLoadFloat4x4(&pCamera->GetViewMatrix())));
            XMStoreFloat3(normal.GetFloat3(), q);

            Vector3 pos = Vector3(pCamera->GetPos()) + 
                (pCamera->GetForward() * fNear) + (pCamera->GetForward() * fFar);
            result[5] = Plane{ normal, pos };
        }

        return result;
    }
};


//===================== CRenderer ========================
void CRenderer::ListDraw(ID3D11DeviceContext* pDC)
{
    // フラスタムカリング
    //std::for_each(m_meshList.begin(), m_meshList.end(), [&pDC](const auto& mesh)
    //    {
    //        if (mesh)
    //        {
    //        }
    //    });
    Vector3 cameraPos = CCamera::GetMainCamera()->GetPos();
    Vector3 cameraScale = Vector3{ SCREEN_CENTER_X , SCREEN_CENTER_Y , 1 };
    cameraScale *= 1.85f;
    Vector3 pos;
    Vector3 scale;
    for (const auto& mesh : m_meshList)
    {
        if (mesh)
        {
            /*CheckFrustum::State eState = CheckFrustum::Detect(mesh->m_pParent->m_pTrans.get(),
                CCamera::GetMainCamera()->GetProjMatrix(),
                CCamera::GetMainCamera(),
                VIEW_NEAR_Z, VIEW_FAR_Z);*/

            // 内側なら
           // if (eState == CheckFrustum::State::Inside)
            //{
            //    m_opaqueList.push_back(mesh);
            //}


            // レンダラータイプで分ける
            if (mesh->m_eRendererType == Renderer::eUI)
            {
                m_UIList.push_back(mesh);
                continue;
            }
           

            // カメラカリング
            pos   = mesh->m_pParent->m_pTrans->m_pos;
            scale = mesh->m_pParent->m_pTrans->m_scale;
            if (!mesh->m_bCull ||
                (pos->x + scale->x / 2 < cameraPos->x + cameraScale->x &&
                pos->x - scale->x / 2 > cameraPos->x - cameraScale->x &&
                pos->y + scale->y / 2 < cameraPos->y + cameraScale->y &&
                pos->y - scale->y / 2 > cameraPos->y - cameraScale->y))
            {
                m_opaqueList.push_back(mesh);
            }

        }
    }


    // ソート
    //m_meshList.sort(swapR);
    m_opaqueList.sort(swapR);
    m_UIList.sort(swapR);

    // 影描画
    //for (const auto& mesh : m_meshList)
    //std::for_each(m_meshList.begin(), m_meshList.end(), [&pDC](const auto& mesh)
    //{
    //	if (mesh)
    //	{
    //		mesh->EarlyDraw(pDC);

    //	}
    //});

    //for (const auto& mesh : m_meshList)
    //std::for_each(m_meshList.begin(), m_meshList.end(), [&pDC](const auto& mesh)
    //    {
    //        if (mesh)
    //        {
    //            // 半透明処理有効
    //            SetBlendState(mesh->m_eState);

    //            mesh->LateDraw(pDC);

    //            // 半透明処理無効
    //            SetBlendState(BS_NONE);
    //        }
    //    });


    // 不透明描画
    std::for_each(m_opaqueList.begin(), m_opaqueList.end(), [&pDC](const auto& mesh)
        {
            if (mesh)
            {
                // 半透明処理有効
                SetBlendState(mesh->m_eState);

                mesh->LateDraw(pDC);

                // 半透明処理無効
                SetBlendState(BS_NONE);
            }
        });

    m_opaqueList.clear();

    // 半透明描画


    // ポストエフェクト
    DrawPostEffect();

    // UI描画
    std::for_each(m_UIList.begin(), m_UIList.end(), [&pDC](const auto& mesh)
        {
            if (mesh)
            {
                // 半透明処理有効
                SetBlendState(mesh->m_eState);

                mesh->LateDraw(pDC);

                // 半透明処理無効
                SetBlendState(BS_NONE);
            }
        });

    m_UIList.clear();

}