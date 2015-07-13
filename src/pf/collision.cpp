/* File: collision.cpp
 * Date: 12.09-.010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */


#include "collision.hpp"

using namespace std;
using namespace pf;
using namespace gf;

/* ----------------------------------------------------------------------------
 * cAbstractPrimitive
 */
CAbstractPrimitive::CAbstractPrimitive() : m_position(0.0f,0.0f,0.0f,1.0f){
  m_id = ++m_counter;
}

CAbstractPrimitive::~CAbstractPrimitive(){
  // pass
}

int CAbstractPrimitive::m_counter = 0;

int CAbstractPrimitive::getId() const {
  return m_id;
}

const CVector4f& CAbstractPrimitive::getPosition() const {
  return m_position;
};

void CAbstractPrimitive::setId(int id){
  m_id = id;
}

void CAbstractPrimitive::setPosition(const CVector4f& p){
  m_position = p;
}
// <--

/* ----------------------------------------------------------------------------
 * cCollisionPoint
 */
CCollisionPoint::CCollisionPoint(ICollisionManager* cmgr, const CVector4f& p)
{
  m_cmgr = cmgr;
  setPosition(p);
}

CCollisionPoint::~CCollisionPoint(){
  // pass
};

bool CCollisionPoint::move(gf::CVector4f& v){
  return m_cmgr->move(this, v);
}
// <--

/* ----------------------------------------------------------------------------
 * cCollisionWall
 */
CCollisionWall::CCollisionWall(){
  m_cmgr = 0;
  setPosition(CVector4f(0.0f, 0.0f, 0.0f));
  m_front = CVector4f(0.0f, 0.0f, 1.0f);
  m_up = CVector4f(0.0f, 1.0f, 0.0f, 1.0f);
  m_size = CVector2f(1.0f, 1.0f);
}

CCollisionWall::CCollisionWall(ICollisionManager* cmgr, const CVector4f& p,
    const CVector4f& n, const CVector2f& s)
{
  m_cmgr = cmgr;
  setPosition(p);
  setFront(n);
  setUp(CVector4f(0.0f, 1.0f, 0.0f, 1.0f));
  m_size = s;
}
CCollisionWall::~CCollisionWall(){
  // pass
}

const CVector4f& CCollisionWall::getFront() const {
  return m_front;
}

const CVector4f& CCollisionWall::getUp() const {
  return m_up;
}

const CVector2f& CCollisionWall::getSize() const {
  return m_size;
}

void CCollisionWall::setFront(const CVector4f& f){
  m_front = CVector4f::norm(f);
}

void CCollisionWall::setUp(const CVector4f& u){
  m_up = CVector4f::norm(u);
}

void CCollisionWall::setSize(const CVector2f& s){
  m_size = s;
}
// <--

/* ----------------------------------------------------------------------------
 * cCollisionBox
 */
CCollisionBox::CCollisionBox(ICollisionManager* cmgr, const CVector4f& p,
    const CVector4f& s)
{
  m_cmgr = cmgr;
  setPosition(p);
  m_front = CVector4f(0.0f, 0.0f, 1.0f);
  m_up = CVector4f(0.0f, 1.0f, 0.0f);
  m_size = s;
  m_eval = true;
}

CCollisionBox::~CCollisionBox(){
  // pass
}

const CVector4f& CCollisionBox::getPosition() const {
  return CAbstractPrimitive::getPosition();
}

const CVector4f& CCollisionBox::getFront() const {
  return m_front;
}

const CVector4f& CCollisionBox::getUp() const {
  return m_up;
}

const CVector4f& CCollisionBox::getSize() const {
  return m_size;
}

void CCollisionBox::setPosition(const gf::CVector4f& p){
  CAbstractPrimitive::setPosition(p);
  m_eval = true;
}

void CCollisionBox::setFront(const CVector4f& f){
  m_front = CVector4f::norm(f);
  m_eval = true;
}

void CCollisionBox::setUp(const CVector4f& u){
  m_up = CVector4f::norm(u);
  m_eval = true;
}

void CCollisionBox::setSize(const gf::CVector4f& s){
  m_size = s;
  m_eval = true;
}

const CAbstractWall& CCollisionBox::getWall(int i) const {
  eval();
  return m_wall[i];
}

void CCollisionBox::eval() const {
  if(!m_eval) return;

  const CVector4f& p = getPosition();
  const CVector4f& s = getSize();
  const CVector4f& front = m_front;
  const CVector4f& right = CVector4f::norm(CVector4f::cross(front, m_up));
  const CVector4f& up = CVector4f::norm(CVector4f::cross(right, front));

  m_wall[0].setPosition(p + front*(s.z()/2.0f));
  m_wall[0].setSize(CVector2f(s.x(), s.y()));
  m_wall[0].setFront(front);
  m_wall[0].setUp(up);

  m_wall[1].setPosition(p - front*(s.z()/2.0f));
  m_wall[1].setSize(CVector2f(s.x(), s.y()));
  m_wall[1].setFront(-front);
  m_wall[1].setUp(up);

  m_wall[2].setPosition(p + right*(s.x()/2.0f));
  m_wall[2].setSize(CVector2f(s.z(), s.y()));
  m_wall[2].setFront(right);
  m_wall[2].setUp(up);

  m_wall[3].setPosition(p - right*(s.x()/2.0f));
  m_wall[3].setSize(CVector2f(s.z(), s.y()));
  m_wall[3].setFront(-right);
  m_wall[3].setUp(up);

  m_wall[4].setPosition(p + up*(s.y()/2.0f));
  m_wall[4].setSize(CVector2f(s.x(), s.z()));
  m_wall[4].setFront(up);
  m_wall[4].setUp(-front);

  m_wall[5].setPosition(p - up*(s.y()/2.0f));
  m_wall[5].setSize(CVector2f(s.x(), s.z()));
  m_wall[5].setFront(-up);
  m_wall[5].setUp(-front);

  m_eval = false;
}
// <--

/* ----------------------------------------------------------------------------
 * cCollisionManager
 */
CCollisionManager::CCollisionManager() : m_log(CLog::instance())
{
  // pass
}

CCollisionManager::~CCollisionManager() {
  for(vector<CAbstractPoint*>::iterator i=m_point.begin(); i!=m_point.end(); ++i)
  {
    delete *i;
  }
  for(vector<CAbstractWall*>::iterator i=m_wall.begin(); i!=m_wall.end(); ++i)
  {
    delete *i;
  }
  for(vector<CAbstractBox*>::iterator i=m_box.begin(); i!=m_box.end(); ++i)
  {
    delete *i;
  }
}

CAbstractPoint* CCollisionManager::addPoint(){
  CAbstractPoint* ptr = new CCollisionPoint(this, CVector4f(0.0f, 0.0f, 0.0f));
  m_point.push_back(ptr);
  return ptr;
}

CAbstractWall* CCollisionManager::addWall(const CVector4f& n, const CVector2f& s){
  CAbstractWall* ptr = new CCollisionWall(this, CVector4f(0.0f,0.0f,0.0f), n, s);
  m_wall.push_back(ptr);
  return ptr;
}

CAbstractBox* CCollisionManager::addBox(const CVector4f& s){
  CAbstractBox* ptr = new CCollisionBox(this, CVector4f(0.0f,0.0f,0.0f), s);
  m_box.push_back(ptr);
  return ptr;
}

void CCollisionManager::delPoint(CAbstractPoint* point){
  const int id = point->getId();
  vector<CAbstractPoint*>::iterator itr;
  for(itr=m_point.begin(); itr!=m_point.end(); ++itr){
    if(id == (*itr)->getId()){
      delete *itr;
      m_point.erase(itr);
      break;
    }
  }
}

void CCollisionManager::delWall(CAbstractWall* wall){
  const int id = wall->getId();
  vector<CAbstractWall*>::iterator itr;
  for(itr=m_wall.begin(); itr!=m_wall.end(); ++itr){
    if(id == (*itr)->getId()){
      delete *itr;
      m_wall.erase(itr);
      break;
    }
  }
}

void CCollisionManager::delBox(CAbstractBox* box){
  const int id = box->getId();
  vector<CAbstractBox*>::iterator itr;
  for(itr=m_box.begin(); itr!=m_box.end(); ++itr){
    if(id == (*itr)->getId()){
      delete *itr;
      m_box.erase(itr);
      break;
    }
  }
}

bool CCollisionManager::move(const CAbstractPoint* obj, gf::CVector4f& v)
{
  // Kolizja Punkt-Sciana
  for(vector<CAbstractWall*>::iterator i=m_wall.begin(); i!=m_wall.end(); ++i)
  {
    if(!checkWall(*i, obj->getPosition(), v)){
      return false;
    }
  }

  // Kolizja Punkt-Prostopadloscian
  for(vector<CAbstractBox*>::iterator i=m_box.begin(); i!=m_box.end(); ++i)
  {
    if(!checkBox(*i, obj->getPosition(), v)){
      return false;
    }
  }

  return true;
}

int CCollisionManager::select(const gf::CVector4f& pos, const gf::CVector4f& vec)
{
  int id = 0;
  float length = 0.0f;

  for(vector<CAbstractWall*>::iterator i=m_wall.begin(); i!=m_wall.end(); ++i)
  {
    if(!checkWall(*i, pos, vec) &&
       (id==0 || CVector4f::mod(pos - (*i)->getPosition()) < length))
    {
      id = (*i)->getId();
      length = CVector4f::mod(pos - (*i)->getPosition());
    }
  }

  for(vector<CAbstractBox*>::iterator i=m_box.begin(); i!=m_box.end(); ++i)
  {
    for(int j=0; j<6; ++j){
      const CAbstractWall* wall = &(*i)->getWall(j);

      if(!checkWall(wall, pos, vec) &&
         (id==0 || CVector4f::mod(pos - wall->getPosition()) < length))
      {
        id = (*i)->getId();
        length = CVector4f::mod(pos - wall->getPosition());
      }
    }
  }

  return id;
}

bool CCollisionManager::checkWall(const CAbstractWall* obj, const gf::CVector4f& p1,
    const gf::CVector4f& vec)
{
  const CVector4f& p2 = p1 + vec;
  const CVector4f& p = obj->getPosition();  // Pozycja sciany
  const CVector4f& n = obj->getFront();     // Wektor normalny sciany

  float d = -CVector4f::dot(n,p);
  float d1 = CVector4f::dot(n,p1) + d;
  float d2 = CVector4f::dot(n,p2) + d;

  if(d1*d2 <= 0){
    CVector4f v = p2 - p1;
    CVector4f q = p1 - v*(CVector4f::dot(n, p1)+d)/CVector4f::dot(n,v);
    CVector4f vq = q - p; // Wektor od srodka sciany do punktu przeciecia

    const CVector2f& s = obj->getSize();
    const CVector4f& horizont = CVector4f::cross(n, obj->getUp());
    const CVector4f& vertic = CVector4f::cross(n, horizont);

    if(2*abs(CVector4f::dot(vq, horizont)) <= s.w() &&
       2*abs(CVector4f::dot(vq, vertic)) <= s.h())
    {
      return false;
    }
  }

  return true;
}

bool CCollisionManager::checkBox(const CAbstractBox* obj, const gf::CVector4f& p1,
    gf::CVector4f& vec)
{
  for(int i=0; i<6; ++i){
    const CAbstractWall* wall = &obj->getWall(i);
    if(!checkWall(wall, p1, vec)){
      vec -= wall->getFront()*CVector4f::dot(wall->getFront(), vec);
      checkBox(obj, p1, vec);
      return false;
    }
  }

  return true;
}
// <--

