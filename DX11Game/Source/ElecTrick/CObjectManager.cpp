
#include "CObjectManager.h"

std::list<obj_shared> CObjectManager::m_objectList;
std::list<std::list<obj_shared>::iterator> CObjectManager::m_destroyList;

Vector3 CObjectManager::m_posBuffer;
Vector3 CObjectManager::m_rotBuffer;
Vector3 CObjectManager::m_scaleBuffer;
