/* File: scene.cpp
 * Date: 01.06.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#include "scene.hpp"

using namespace std;
using namespace gf;

/* ----------------------------------------------------------------------------
 * cAbstractNode
 */
CAbstractNode::CAbstractNode()
  : m_log(CLog::instance()), m_scale(1.0, 1.0, 1.0)
{
  m_modelMatrixValid = true;
  m_rotationAngle = 0.0;
  m_parent = 0;
}

CAbstractNode::~CAbstractNode(){
  // pass
}

const CAbstractNode* CAbstractNode::getParent() const {
  return m_parent;
}

void CAbstractNode::setParent(CAbstractNode* parent){
  m_parent = parent;
}

bool CAbstractNode::appendChild(CAbstractNode* child){
  if(child){
    child->setParent(this);
    m_children.push_back(child);
    return true;
  }
  else
    return false;
}

bool CAbstractNode::removeChild(CAbstractNode* child){
  vector<CAbstractNode*>::iterator itr;
  for(itr=m_children.begin(); itr!=m_children.end(); ++itr){
    if((*itr) == child){
      child->setParent(0);
      m_children.erase(itr);
      return true;
    }
  }
  return false;
}

const vector<CAbstractNode*>& CAbstractNode::getChildren() const {
  return m_children;
}

void CAbstractNode::clearChildren(){
  m_children.clear();
}

const CVector4f& CAbstractNode::getPosition() const {
  return m_position;
}

const CVector4f& CAbstractNode::getScale() const {
  return m_scale;
}

const float& CAbstractNode::getRotationAngle() const {
  return m_rotationAngle;
}

const CVector4f& CAbstractNode::getRotationVector() const {
  return m_rotationVect;
}

const CVector4f& CAbstractNode::getRotationPoint() const {
  return m_rotationPoint;
}

const CMatrix4f& CAbstractNode::getModelMatrix() const {
  matrixLazyEval();
  return m_modelMatrix;
}

void CAbstractNode::setPosition(const CVector4f& vect){
  m_position = vect;
  vector<CAbstractNode*>::const_iterator itr;
  for(itr=m_children.begin(); itr!=m_children.end(); ++itr){
    (*itr)->setPosition((*itr)->getPosition()+m_position);
  }

  m_modelMatrixValid = false;
}

void CAbstractNode::setScale(const CVector4f& v){
  m_scale = v;
  vector<CAbstractNode*>::const_iterator itr;
  for(itr=m_children.begin(); itr!=m_children.end(); ++itr){
    (*itr)->setScale(CVector4f((*itr)->getScale().x()*m_scale.x(),
                               (*itr)->getScale().y()*m_scale.y(),
                               (*itr)->getScale().z()*m_scale.z()));
  }

  m_modelMatrixValid = false;
}

void CAbstractNode::setRotation(const float& angle, const CVector4f& vect,
    const CVector4f& point)
{
  m_rotationAngle = angle;
  m_rotationVect = vect;
  m_rotationPoint = point;
  vector<CAbstractNode*>::const_iterator itr;
  for(itr=m_children.begin(); itr!=m_children.end(); ++itr){
    (*itr)->setRotation(
        (*itr)->getRotationAngle()+m_rotationAngle,
        (*itr)->getRotationVector()+m_rotationVect,
        (*itr)->getRotationPoint()+m_rotationPoint);
  }

  m_modelMatrixValid = false;
}

void CAbstractNode::setModelMatrix(const CMatrix4f& m) const{
  m_modelMatrix = m;

  // Update pozycji potomkow
  vector<CAbstractNode*>::const_iterator itr;
  for(itr=m_children.begin(); itr!=m_children.end(); ++itr){
    (*itr)->setModelMatrix((*itr)->getModelMatrix()*m_modelMatrix);
  }
}

void CAbstractNode::matrixLazyEval() const {
  if(m_modelMatrixValid) return;

  // Macierz translacji pozycji
  CMatrix4f translate;
  translate.at(3,0) = m_position.x();
  translate.at(3,1) = m_position.y();
  translate.at(3,2) = m_position.z();

  // Macierz skalowania
  CMatrix4f scale;
  scale.at(0,0) = m_scale.x();
  scale.at(1,1) = m_scale.y();
  scale.at(2,2) = m_scale.z();

  // Macierz obrotu wokol wektora
  CMatrix4f rotate;

  float radians = (m_rotationAngle*3.14f)/180.0f;
  float angleSin = std::sin(radians);
  float angleCos = std::cos(radians);

  rotate.at(0,0) = pow(m_rotationVect.x(), 2)*(1 - angleCos) + angleCos;
  rotate.at(0,1) = m_rotationVect.x()*m_rotationVect.y()*(1 - angleCos)
      + m_rotationVect.z()*angleSin;
  rotate.at(0,2) = m_rotationVect.x()*m_rotationVect.z()*(1 - angleCos)
      - m_rotationVect.y()*angleSin;

  rotate.at(1,0) = m_rotationVect.x()*m_rotationVect.y()*(1 - angleCos)
      - m_rotationVect.z()*angleSin;
  rotate.at(1,1) = pow(m_rotationVect.y(), 2)*(1 - angleCos) + angleCos;
  rotate.at(1,2) = m_rotationVect.y()*m_rotationVect.z()*(1 - angleCos)
      + m_rotationVect.x()*angleSin;

  rotate.at(2,0) = m_rotationVect.x()*m_rotationVect.z()*(1 - angleCos)
      + m_rotationVect.y()*angleSin;
  rotate.at(2,1) = m_rotationVect.y()*m_rotationVect.z()*(1 - angleCos)
      - m_rotationVect.x()*angleSin;
  rotate.at(2,2) = pow(m_rotationVect.z(),2)*(1 - angleCos) + angleCos;

  CMatrix4f rotatePoint;
  rotatePoint.at(3,0) = m_rotationPoint.x();
  rotatePoint.at(3,1) = m_rotationPoint.y();
  rotatePoint.at(3,2) = m_rotationPoint.z();

  rotate = rotatePoint * rotate;

  rotatePoint.at(3,0) = -m_rotationPoint.x();
  rotatePoint.at(3,1) = -m_rotationPoint.y();
  rotatePoint.at(3,2) = -m_rotationPoint.z();

  rotate *= rotatePoint;

  m_modelMatrix = CMatrix4f();
  m_modelMatrix = translate * scale * rotate;

  m_modelMatrixValid = true;
}
// <--

/* ----------------------------------------------------------------------------
 * cSceneNode
 */
CSceneNode::CSceneNode(smart_ptr<CAbstractMesh> mesh, smart_ptr<CAbstractMaterial> material,
    smart_ptr<CAbstractProgram> program)
{
  m_mesh = mesh;
  if(!(m_mesh && m_mesh->vaild())){
    m_log.msg(CLogWarning() << "Wezel sceny nie posiada siatki");
  }

  m_material = material;
  if(!m_material){
    m_log.msg(CLogWarning() << "Wezel sceny nie posiada materialu");
  }

  m_program = program;
  if(!(m_program && m_program->vaild())){
    m_log.msg(CLogWarning() << "Wezel sceny nie posiada programu cieniujacego");
  }

  if(m_material->getTexture(0)){
    m_renderMethod = RENDER_TEXTURED;
  }
  else{
    m_renderMethod = RENDER_SHADED;
  }
}

CSceneNode::~CSceneNode(){
  // pass
}

smart_ptr<CAbstractMesh> CSceneNode::getMesh(){
  return m_mesh;
}

smart_ptr<CAbstractMaterial> CSceneNode::getMaterial(){
  return m_material;
}

smart_ptr<CAbstractProgram> CSceneNode::getProgram(){
  return m_program;
}

void CSceneNode::setMesh(smart_ptr<CAbstractMesh> mesh){
  m_mesh = mesh;
}

void CSceneNode::setMaterial(smart_ptr<CAbstractMaterial> material){
  m_material = material;
}

void CSceneNode::setProgram(smart_ptr<CAbstractProgram> program){
  m_program = program;
}

const CAbstractNode::eRenderMethod& CSceneNode::getRenderMethod() const {
  return m_renderMethod;
}

void CSceneNode::setRenderMethod(const eRenderMethod& rm){
  m_renderMethod = rm;
  vector<CAbstractNode*>::const_iterator itr;
  for(itr=getChildren().begin(); itr!=getChildren().end(); ++itr){
    (*itr)->setRenderMethod(rm);
  }
}

void CSceneNode::render() const {

  glPushMatrix();
  // Sent matrix to GPU vertex processor unit
  glMultMatrixf(getModelMatrix().raw());

  if(m_mesh && m_mesh->vaild() && m_program && m_program->vaild()){
    // Niektore karty wymagaja by program byl zbindowany przed
    // przesylaniem uniformow, wiec binduje go na poczatku
    m_program->bind();

    if(m_material){
      m_program->setUniform1i("render", (int)getRenderMethod());
      m_program->setUniform4f("Material.ambient", m_material->getAmbient());
      m_program->setUniform4f("Material.diffuse", m_material->getDiffuse());
      m_program->setUniform4f("Material.specular", m_material->getSpecular());
      m_program->setUniform1f("Material.reflection", m_material->getReflection());
    }

    if(m_renderMethod != RENDER_WIRED && m_material){
      for(int i=0; i<4; ++i){
        smart_ptr<CAbstractTexture> tex = m_material->getTexture(i);
        if(tex && tex->vaild()) tex->bind();
      }
    }

    m_mesh->bind();
    if(m_renderMethod == RENDER_WIRED){
      for(unsigned int i=0; i<m_mesh->getVertexCount(); i+=3){
        glDrawArrays(GL_LINE_STRIP, i, 3);
      }
    }
    else {
      glDrawArrays(GL_TRIANGLES, 0, m_mesh->getVertexCount());
    }
  }

  glPopMatrix();
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractCamera
 */
CAbstractCamera::CAbstractCamera(){
  m_viewValid = m_projValid = false;
}

CAbstractCamera::~CAbstractCamera(){
  // pass
}

const CVector4f& CAbstractCamera::getPosition() const {
  return m_camPosition;
}

const CVector4f& CAbstractCamera::getLookAt() const {;
  return m_camLookAt;
}

const CVector4f& CAbstractCamera::getUp() const {
  return m_camUp;
}

const CMatrix4f& CAbstractCamera::getViewMatrix() const{
  viewLazyEval();
  return m_viewMatrix;
}

const CMatrix4f& CAbstractCamera::getProjMatrix() const{
  projLazyEval();
  return m_projMatrix;
}

void CAbstractCamera::setPosition(const CVector4f& pos){
  m_camPosition = pos;
  m_viewValid = false;
}

void CAbstractCamera::setLookAt(const CVector4f& at){
  m_camLookAt = at;
  m_viewValid = false;
}

void CAbstractCamera::setUp(const CVector4f& up){
  m_camUp = up;
  m_viewValid = false;
}

void CAbstractCamera::setViewMatrix(const CMatrix4f& m){
  m_viewMatrix = m;
  m_viewValid = false;
}

void CAbstractCamera::setProjMatrix(const CMatrix4f& m){
  m_projMatrix = m;
  m_projValid = false;
}
// <--

/* ----------------------------------------------------------------------------
 * cPerspectiveCamera
 */
CPerspectiveCamera::CPerspectiveCamera(const CVector4f& pos, const CVector4f& at,
    const CVector4f& up, float aspect, float fov, float far, float near)
{
  setPosition(pos);
  setLookAt(at);
  setUp(up);

  m_far = far;
  m_near = near;
  m_aspect = aspect;
  m_fov = (fov/180.0f)*3.14f;
}

CPerspectiveCamera::~CPerspectiveCamera(){
  // pass
}

const float& CPerspectiveCamera::getFar() const {
  return m_far;
}

const float& CPerspectiveCamera::getNear() const {
  return m_near;
}

const float& CPerspectiveCamera::getAcpect() const {
  return m_aspect;
}

const float& CPerspectiveCamera::getFOV() const {
  return m_fov;
}

void CPerspectiveCamera::setFar(const float& far){
  m_far = far;
  m_projValid = false;
}

void CPerspectiveCamera::setNear(const float& near){
  m_near = near;
  m_projValid = false;

}

void CPerspectiveCamera::setAspect(const float& aspect){
  m_aspect = aspect;
  m_projValid = false;

}

void CPerspectiveCamera::setFOV(const float& fov){
  m_fov = (fov/180.0f)*3.14f;;
  m_projValid = false;
}

void CPerspectiveCamera::moveCamera(const CVector4f& vect){
  // Translacja wektora z ukladu kamery do globalnego ukladu wspol.
  CVector4f v = CMatrix4f::transpose(getViewMatrix()) * vect;
  setPosition(getPosition() + v);
  setLookAt(getLookAt() + v);

  m_viewValid = false;
}

void CPerspectiveCamera::rotateCamera(const float& angle, const CVector4f& vect){
  // Znajdz wektor obrotu w globalnym ukladzie wspolrzednych
  // (1) Macierz m_viewMatrix jest ortogonalna, jej odwrotnosc=transpozycja
  CVector4f v = CMatrix4f::transpose(getViewMatrix()) * vect;

  // Znajdz macierz obrotu dla wyliczonego wektora
  CMatrix4f euler(angle, v);

  // Znajdz wektor kierunku kamery
  CVector4f forward = getPosition() - getLookAt();

  // Obruc wektor kierunku
  forward = euler * forward;

  // Oblicz nowy punk LookAt
  setLookAt(getPosition() - forward);

  // Zglos koniecznosc przeliczenia macierzy widoku
  m_viewValid = false;
}

void CPerspectiveCamera::viewLazyEval() const {
  if(m_viewValid) return;

  const CVector4f& position = getPosition();
  const CVector4f& at = getLookAt();
  const CVector4f& up = getUp();

  CVector4f zAxis = -CVector4f::norm(at - position);
  CVector4f xAxis = CVector4f::norm(CVector4f::cross(up, zAxis));
  CVector4f yAxis = CVector4f::cross(zAxis, xAxis);

  m_viewMatrix = CMatrix4f();

  m_viewMatrix.at(0,0) = xAxis.x();
  m_viewMatrix.at(1,0) = xAxis.y();
  m_viewMatrix.at(2,0) = xAxis.z();
  m_viewMatrix.at(3,0) = -CVector4f::dot(xAxis, position);

  m_viewMatrix.at(0,1) = yAxis.x();
  m_viewMatrix.at(1,1) = yAxis.y();
  m_viewMatrix.at(2,1) = yAxis.z();
  m_viewMatrix.at(3,1) = -CVector4f::dot(yAxis, position);

  m_viewMatrix.at(0,2) = zAxis.x();
  m_viewMatrix.at(1,2) = zAxis.y();
  m_viewMatrix.at(2,2) = zAxis.z();
  m_viewMatrix.at(3,2) = -CVector4f::dot(zAxis, position);

  m_viewValid = true;
}

void CPerspectiveCamera::projLazyEval() const {
  if(m_projValid) return;

  float angleCtg = 1/tan(m_fov/2);

  m_projMatrix = CMatrix4f();
  m_projMatrix.at(0,0) = angleCtg/m_aspect;
  m_projMatrix.at(1,1) = angleCtg;
  m_projMatrix.at(2,2) = (m_far+m_near)/(m_near-m_far);
  m_projMatrix.at(3,3) = 0.0;
  m_projMatrix.at(2,3) = -1.0;
  m_projMatrix.at(3,2) = (2*m_far*m_near)/(m_near-m_far);

  m_projValid = true;
}
// <--

/* ----------------------------------------------------------------------------
 * cFPSCamera
 */
CFPSCamera::CFPSCamera(const CVector4f& pos, const CVector4f& at, float aspect,
    float fov)
  : CPerspectiveCamera(pos, at, CVector4f(0,1,0), aspect, fov)
{
  m_cameraSpeed = 8.0f;
  m_mouseSpeed = 2.0f;
}

const float& CFPSCamera::getCameraSpeed() const {
   return m_cameraSpeed;
}

const float& CFPSCamera::getMouseSpeed() const {
   return m_mouseSpeed;
}

void CFPSCamera::setCameraSpeed(const float& speed){
  m_cameraSpeed = speed;
}

void CFPSCamera::setMouseSpeed(const float& speed){
  m_mouseSpeed = speed;
}

void CFPSCamera::moveCamera(const CVector4f& vect){
  // Korzystamy z macierzy widoku, trzeba byc pewnym ze jest aktualna
  viewLazyEval();

  // Translacja wektora z ukladu kamery do globalnego ukladu wspol.
  CVector4f v = CMatrix4f::transpose(m_viewMatrix) * vect;
  v.y() = 0.0; // Kamera FPS olewa przesuniecie w lokalnym y
  setPosition(getPosition() + v);
  setLookAt(getLookAt() + v);

  m_viewValid = false;
}


void CFPSCamera::warp(IDevice& dev){
  // Obrot
  CVector2i size = dev.getSize();
  CVector2i mouse = dev.getMousePosition();
  CVector2f delta;

  delta.x() = (float)(mouse.x() - size.w()/2);
  delta.y() = (float)(mouse.y() - size.h()/2);

  if(delta.x()!=0){
    rotateCamera(-delta.x()*getMouseSpeed()*dev.getFrameDelta(), CVector4f(0,1,0));
  }
  if(delta.y()!=0){
    rotateCamera(-delta.y()*getMouseSpeed()*dev.getFrameDelta(), CVector4f(1,0,0));
  }

  dev.setMousePosition(CVector2i(size.x()/2, size.y()/2));

  // Pozycja
  if(dev.getKeyState(SDLK_w))
    moveCamera(CVector4f(0, 0, -getCameraSpeed()*dev.getFrameDelta()));
  if(dev.getKeyState(SDLK_s))
    moveCamera(CVector4f(0, 0, getCameraSpeed()*dev.getFrameDelta()));
  if(dev.getKeyState(SDLK_a))
    moveCamera(CVector4f(-getCameraSpeed()*dev.getFrameDelta(), 0, 0));
  if(dev.getKeyState(SDLK_d))
    moveCamera(CVector4f(getCameraSpeed()*dev.getFrameDelta(), 0, 0));
}
// <--

/* ----------------------------------------------------------------------------
 * cOrthoCamera
 */
COrthoCamera::COrthoCamera(const CVector4f& pos, const CVector4f& at,
    const CVector4f& up, float width, float height, float far, float near)
{
  setPosition(pos);
  setLookAt(at);
  setUp(up);

  m_width = width;
  m_height = height;
  m_far = far;
  m_near = near;
}

COrthoCamera::~COrthoCamera(){
  // pass
}

const float& COrthoCamera::getWidth() const {
  return m_width;
}

const float& COrthoCamera::getHeight() const {
  return m_height;
}

const float& COrthoCamera::getFar() const {
  return m_far;
}

const float& COrthoCamera::getNear() const {
  return m_near;
}

void COrthoCamera::setWidth(const float& width){
  m_width = width;
  m_viewValid = false;
}

void COrthoCamera::setHeight(const float& height){
  m_height = height;
  m_viewValid = false;
}

void COrthoCamera::setFar(const float& far){
  m_far = far;
  m_viewValid = false;
}

void COrthoCamera::setNear(const float& near){
  m_near = near;
  m_viewValid = false;
}

void COrthoCamera::viewLazyEval() const {
  if(m_viewValid) return;

  const CVector4f& position = getPosition();
  const CVector4f& at = getLookAt();
  const CVector4f& up = getUp();

  CVector4f zAxis = -CVector4f::norm(at - position);
  CVector4f xAxis = CVector4f::norm(CVector4f::cross(up, zAxis));
  CVector4f yAxis = CVector4f::cross(zAxis, xAxis);

  m_viewMatrix = CMatrix4f();

  m_viewMatrix.at(0,0) = xAxis.x();
  m_viewMatrix.at(1,0) = xAxis.y();
  m_viewMatrix.at(2,0) = xAxis.z();
  m_viewMatrix.at(3,0) = -CVector4f::dot(xAxis, position);

  m_viewMatrix.at(0,1) = yAxis.x();
  m_viewMatrix.at(1,1) = yAxis.y();
  m_viewMatrix.at(2,1) = yAxis.z();
  m_viewMatrix.at(3,1) = -CVector4f::dot(yAxis, position);

  m_viewMatrix.at(0,2) = zAxis.x();
  m_viewMatrix.at(1,2) = zAxis.y();
  m_viewMatrix.at(2,2) = zAxis.z();
  m_viewMatrix.at(3,2) = -CVector4f::dot(zAxis, position);

  m_viewValid = true;
}

void COrthoCamera::projLazyEval() const {
  if(m_projValid) return;

  float left = -m_width/2;
  float right = m_width/2;
  float top = m_height/2;
  float bottom = -m_height/2;

  m_projMatrix = CMatrix4f();
  m_projMatrix.at(0,0) = 2/(right-left);
  m_projMatrix.at(1,1) = 2/(top-bottom);
  m_projMatrix.at(2,2) = -2/(m_far-m_near);
  m_projMatrix.at(3,0) = -(right+left)/(right-left);
  m_projMatrix.at(3,1) = -(top+bottom)/(top-bottom);
  m_projMatrix.at(3,2) = -(m_far+m_near)/(m_far-m_near);

  m_projValid = true;
}
// <--


/* ----------------------------------------------------------------------------
 * cFree2DCamera
 */
CFree2DCamera::CFree2DCamera(const CVector2f& pos, float width, float height,
    float far, float near)
  : COrthoCamera(CVector4f(pos.x(), pos.y(), 100.0f), CVector4f(pos.x(), pos.y(), -100.0f),
                 CVector4f(0.0f, 1.0f, 0.0f), width, height, far, near)
{
  // pass
}

CFree2DCamera::~CFree2DCamera(){
  // pass
}

void CFree2DCamera::moveCamera(const CVector2f& v){
  setPosition(getPosition() + CVector4f(v.x(), v.y(), 0));
  m_viewValid = false;
}

void CFree2DCamera::rotateCamera(const float& angle){
  float radians = (angle/180.0f)*3.14f;
  float angleSin = sin(radians);
  float angleCos = cos(radians);

  // Macierz obracajaca wektor w osi OZ
  CMatrix4f euler;
  euler.at(0,0) = angleCos;
  euler.at(0,1) = angleSin;
  euler.at(1,0) = -angleSin;
  euler.at(1,1) = angleCos;

  setUp(euler * getUp());

  m_viewValid = false;
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractLight
 */
CAbstractLight::CAbstractLight()
  : m_ambient(1,1,1,1), m_diffuse(1,1,1,1), m_specular(1,1,1,1)
{
  // pass
}

CAbstractLight::~CAbstractLight(){
  // pass
}

const CVector4f& CAbstractLight::getPosition(){
  return m_position;
}

const CVector4f& CAbstractLight::getAmbient(){
  return m_ambient;
}

const CVector4f& CAbstractLight::getDiffuse(){
  return m_diffuse;
}

const CVector4f& CAbstractLight::getSpecular(){
  return m_specular;
}

void CAbstractLight::setPosition(const CVector4f & pos){
  m_position = pos;
}

void CAbstractLight::setAmbient(const CVector4f & ambient){
  m_ambient = ambient;
}

void CAbstractLight::setDiffuse(const CVector4f & diffuse){
  m_diffuse = diffuse;
}

void CAbstractLight::setSpecular(const CVector4f & specular){
  m_specular = specular;
}
// <--

/* ----------------------------------------------------------------------------
 * cDirLight
 */
CDirLight::CDirLight(){
  CAbstractLight::setPosition(CVector4f(0.0, 0.0, 0.0, 0.0));
}

void CDirLight::setPosition(const CVector4f& pos){
  CAbstractLight::setPosition(CVector4f(pos.x(), pos.y(), pos.z(), 0.0));
}
// <--

/* ----------------------------------------------------------------------------
 * cPointLight
 */
CPointLight::CPointLight(){
  setPosition(CVector4f(0,0,0,1));
  m_constantAttenuation = 0.5f;
  m_linearAttenuation = 0.05f;
  m_quadraticAttenuation = 0.01f;
}

const float& CPointLight::getConstAtt(){
  return m_constantAttenuation;
}

const float& CPointLight::getLineralAtt(){
  return m_linearAttenuation;
}

const float& CPointLight::getQuadraticAtt(){
  return m_quadraticAttenuation;
}

void CPointLight::setConstAtt(const float& value){
  m_constantAttenuation = value;
}

void CPointLight::setLinearAtt(const float& value){
  m_linearAttenuation = value;
}

void CPointLight::setQuadraticAtt(const float& value){
  m_quadraticAttenuation = value;
}
// <--

/* ----------------------------------------------------------------------------
 * cSceneManager
 */
CSceneManager::CSceneManager(IDevice& dev)
  : m_log(CLog::instance()), m_dev(dev), m_res(CResourceFactory::instance())
{
  m_enabled = true;
  m_globalAmbient = CVector4f(0.05f, 0.05f, 0.05f, 0.05f);
  m_activeCamera = 0;
}

CSceneManager::~CSceneManager(){
  for(vector<CAbstractCamera*>::iterator itr=m_sceneCamera.begin();
      itr!=m_sceneCamera.end(); ++itr){
    delete *itr;
  }
  for(vector<CAbstractLight*>::iterator itr=m_sceneLight.begin();
      itr!=m_sceneLight.end(); ++itr){
    delete *itr;
  }
  for(vector<CAbstractNode*>::iterator itr=m_sceneNode.begin();
      itr!=m_sceneNode.end(); ++itr){
    delete *itr;
  }
}

bool CSceneManager::getEnabled() const {
  return m_enabled;
}

void CSceneManager::setEnabled(bool enabled){
  m_enabled = enabled;
}

CAbstractCamera* CSceneManager::addPerspectiveCamera(float fov){
  const CVector2i& s = m_dev.getSize();
  CAbstractCamera* cam = 0;
  cam = new CPerspectiveCamera(CVector4f(0.0f,0.0f,0.0f), CVector4f(0.0f,0.0f,-1.0f),
                               CVector4f(0.0f,1.0f,0.0f), (float)s.x()/(float)s.y(), fov);
  m_sceneCamera.push_back(cam);
  return cam;
}

CAbstractCamera* CSceneManager::addOrthoCamera(float width, float height){
  CAbstractCamera* cam = 0;
  cam = new COrthoCamera(CVector4f(0.0f,0.0f,0.0f), CVector4f(0.0f,0.0f,-1.0f),
                         CVector4f(0.0f,1.0f,0.0f), width, height);
  m_sceneCamera.push_back(cam);
  return cam;
}

CAbstractCamera* CSceneManager::add2DCamera(){
  const CVector2i& s = m_dev.getSize();

  CAbstractCamera* cam = 0;
  cam = new CFree2DCamera(CVector2f(0.0f,0.0f), (float)s.w(), (float)s.h());
  m_sceneCamera.push_back(cam);

  return cam;
}

CAbstractCamera* CSceneManager::addFPSCamera(float fov){
  const CVector2i& s = m_dev.getSize();
  CAbstractCamera* cam = 0;
  cam = new CFPSCamera(CVector4f(0.0f,0.0f,0.0f), CVector4f(0.0f,0.0f,-1.0f),
                       (float)s.x()/(float)s.y(), fov);
  m_sceneCamera.push_back(cam);
  return cam;
}

CAbstractLight* CSceneManager::addDirLight(){
  CAbstractLight* light = new CDirLight();

  m_sceneLight.push_back(light);

  return light;
}

CAbstractLight* CSceneManager::addPointLight(){
  CAbstractLight* light = new CPointLight();

  m_sceneLight.push_back(light);

  return light;
}

const CVector4f& CSceneManager::getGlobalAmbient() const {
  return m_globalAmbient;
}

void CSceneManager::setGlobalAmbient(const CVector4f& globamAmbient){
  m_globalAmbient = globamAmbient;
}

CAbstractNode* CSceneManager::addSceneNode(smart_ptr<CAbstractMesh> mesh,
    smart_ptr<CAbstractMaterial> mat, smart_ptr<CAbstractProgram> prog)
{
  CAbstractNode* node = new CSceneNode(mesh, mat, prog);
  m_sceneNode.push_back(node);

  return node;
}

CAbstractNode* CSceneManager::addSceneNode(const string& obj, const string& mat,
    const string& prog)
{
  // Wczytywanie zasobow
  vector<smart_ptr<CAbstractMesh> > mesh = m_res.buildMesh(obj);
  vector<smart_ptr<CAbstractMaterial> > material = m_res.buildMaterial(mat);
  smart_ptr<CAbstractProgram> program = m_res.buildProgram(prog);

  vector<CAbstractNode*> model;

  for(unsigned int i=0; i<mesh.size(); ++i){
    if(!mesh[i]) continue;
    string meshGroup = mesh[i]->getName().substr(mesh[i]->getName().rfind(':')+1);

    for(unsigned j=0; j<material.size(); ++j){
      if(!material[j]) continue;
      string materialGroup =
          material[j]->getName().substr(material[j]->getName().rfind(':')+1);

      if(meshGroup == materialGroup){
        CAbstractNode* node = new CSceneNode(mesh[i], material[j], program);
        model.push_back(node);
        m_sceneNode.push_back(node);
        break;
      }
    }
  }

  for(unsigned int i=1; i<model.size(); ++i){
    model[i-1]->appendChild(model[i]);
    //model[0]->appendChild(model[i]);
  }

  m_log.msg(
      CLogMessage()
      << "addSceneNode("<<obj<<","<<mat<<","<<prog<<"): "<< model.size()
  );

  if(model.size()>0)
    return model.front();
  else
    return 0;
}

CAbstractNode* CSceneManager::addSceneNode(smart_ptr<CAbstractTexture> tex,
    smart_ptr<CAbstractProgram> prog)
{
  if(tex && prog){
    smart_ptr<CAbstractMaterial> mat(new CGenericMaterial());
    mat->setTexture(tex);
    smart_ptr<CAbstractMesh> mesh = m_res.genMesh_Quad((float)tex->getWidth(),(float)tex->getHeight());
    return addSceneNode(mesh,mat,prog);
  }
  else{
    return 0;
  }
}

void CSceneManager::delSceneNode(const CAbstractNode* node){
  std::vector<CAbstractNode*>::iterator i;
  for(i=m_sceneNode.begin(); i!=m_sceneNode.end(); ++i){
    if(node == *i){
      std::vector<CAbstractNode*>::const_iterator j;
      for(j=(*i)->getChildren().begin(); j!=(*i)->getChildren().end(); ++j){
        delSceneNode(*j);
      }
      delete *i;
      m_sceneNode.erase(i);
      break;
    }
  }
}

const CAbstractCamera* CSceneManager::getActiveCamera() const {
  return m_activeCamera;
}

void CSceneManager::setActiveCamera(const CAbstractCamera* camera){
  m_activeCamera = const_cast<CAbstractCamera*>(camera);
}

void CSceneManager::render() const {
  if(!m_activeCamera){
    m_log.msg(CLogWarning() << "Brak kamery na scenie");
    return;
  }

  if(!m_enabled) return;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf(m_activeCamera->getProjMatrix().raw());

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(m_activeCamera->getViewMatrix().raw());

  vector<CAbstractNode*>::const_iterator itr;
  for(itr=m_sceneNode.begin(); itr!=m_sceneNode.end(); ++itr){
    smart_ptr<CAbstractProgram> prog = (*itr)->getProgram();

    if(!prog) continue;

    // Przed przeslaniem uniformow binduje program
    prog->bind();

    prog->setUniform4f("LightModel.ambient", m_globalAmbient);

    for(unsigned int i=0; i<m_sceneLight.size(); ++i){
      CAbstractLight* Light = m_sceneLight[i];

      stringstream stream;
      stream << "Light[" << i << "]";
      string str = stream.str();

      CVector4f pos = m_activeCamera->getViewMatrix()*Light->getPosition();
      prog->setUniform4f(str+".position", pos);
      prog->setUniform4f(str+".ambient", Light->getAmbient());
      prog->setUniform4f(str+".diffuse", Light->getDiffuse());
      prog->setUniform4f(str+".specular", Light->getSpecular());

      CPointLight* pointLight = dynamic_cast<CPointLight*>(Light);
      if(pointLight){
        prog->setUniform1f(str+".constAtt", pointLight->getConstAtt());
        prog->setUniform1f(str+".linearAtt", pointLight->getLineralAtt());
        prog->setUniform1f(str+".quadraticAtt", pointLight->getQuadraticAtt());
      }
    }

    (*itr)->render();
  }
}
// <--
