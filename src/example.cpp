/* File: example.cpp
 * Date: 28.08.2010
 */

#include "example.hpp"

using namespace std;
using namespace gf;
using namespace pf;

/* ----------------------------------------------------------------------------
 * CPlayer
 */
CPlayer::CPlayer(gf::CAbstractCamera* camera, pf::cAbstractPoint* point)
: m_log(gf::CLog::instance()), m_camera(camera), m_point(point)
{
  m_cameraSpeed = 10.0f;
  m_mouseSpeed = 2.0f;
  m_alcohol = 0.0f;
}

CPlayer::~CPlayer()
{
  // pass
}

CAbstractCamera* CPlayer::getCamera(){
  return m_camera;
}

cAbstractPoint* CPlayer::getPoint(){
  return m_point;
}

float CPlayer::getSpeed() const {
  return m_cameraSpeed;
}

float CPlayer::getAlcohol() const {
  return m_alcohol;
}

void CPlayer::setCamera(CAbstractCamera* camera){
  m_camera = camera;
}

void CPlayer::setPoint(cAbstractPoint* point){
  m_point = point;
}

void CPlayer::setSpeed(float speed){
  m_cameraSpeed = speed;
}

void CPlayer::setAlcohol(float alcohol){
  m_alcohol = alcohol;
}

bool CPlayer::move(const CVector4f& vec){
  CVector4f v = CMatrix4f::transpose(m_camera->getViewMatrix()) * vec;
  v = CVector4f(v.x(), 0.0f, v.z());
  m_point->move(v);
  m_camera->setPosition(m_camera->getPosition() + v);
  m_camera->setLookAt(m_camera->getLookAt() + v);
  m_point->setPosition(m_camera->getPosition());

  return false;
}

void CPlayer::rotate(float angle, const gf::CVector4f& vec){
  const CVector4f& v = CMatrix4f::transpose(m_camera->getViewMatrix()) * vec;

  CMatrix4f euler(angle, v);
  CVector4f forward = m_camera->getPosition() - m_camera->getLookAt();
  forward = euler * forward;
  m_camera->setLookAt(m_camera->getPosition() - forward);
}

void CPlayer::warp(IDevice& dev){
  // Obrot
  CVector2i size = dev.getSize();
  CVector2i mouse = dev.getMousePosition();
  CVector2f delta;

  delta.x() = (float)(mouse.x() - size.w()/2);
  delta.y() = (float)(mouse.y() - size.h()/2);

  if(delta.x()!=0){
    rotate(-delta.x()*m_mouseSpeed*dev.getFrameDelta(), CVector4f(0,1,0));
  }
  if(delta.y()!=0){
    rotate(-delta.y()*m_mouseSpeed*dev.getFrameDelta(), CVector4f(1,0,0));
  }

  dev.setMousePosition(CVector2i(size.x()/2, size.y()/2));

  // Pozycja
  if(dev.getKeyState(SDLK_w))
    move(CVector4f(0, 0, -m_cameraSpeed*dev.getFrameDelta()));
  if(dev.getKeyState(SDLK_s))
    move(CVector4f(0, 0, m_cameraSpeed*dev.getFrameDelta()));
  if(dev.getKeyState(SDLK_a))
    move(CVector4f(-m_cameraSpeed*dev.getFrameDelta(), 0, 0));
  if(dev.getKeyState(SDLK_d))
    move(CVector4f(m_cameraSpeed*dev.getFrameDelta(), 0, 0));
}
// <--

/* ----------------------------------------------------------------------------
 * CBottle
 */
CBottle::CBottle(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr){
  m_smgr = smgr;
  m_cmgr = cmgr;
  m_sceneNode = 0;
  m_collisionBox = 0;
  m_name = "Unnamed alcohol, mayby poison !";
  m_alcohol = 0.0f;
}

CBottle::~CBottle(){
  if(m_sceneNode){
    m_smgr->delSceneNode(m_sceneNode);
    m_sceneNode = 0;
  }

  if(m_collisionBox){
    m_cmgr->delBox(m_collisionBox);
    m_collisionBox = 0;
  }
}

const string& CBottle::getName() const {
  return m_name;
}

float CBottle::getAlcohol() const {
  return m_alcohol;
}

const CVector4f& CBottle::getPosition() const {
  return m_position;
}

void CBottle::setName(const string& name){
  m_name = name;
}

void CBottle::setAlcohol(float alcohol){
  m_alcohol = alcohol;
}

void CBottle::setPosition(const CVector4f& p){
  m_position = p;
  if(m_sceneNode) m_sceneNode->setPosition(m_position);
  if(m_collisionBox) m_collisionBox->setPosition(m_position);
}

const CAbstractNode* CBottle::getSceneNode() const {
  return m_sceneNode;
}

const cAbstractBox* CBottle::getCollisionBox() const {
  return m_collisionBox;
}
// <--

/* ----------------------------------------------------------------------------
 * CBottleHeineken
 */
CBottleHeineken::CBottleHeineken(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr)
: CBottle(smgr,cmgr)
{
  m_sceneNode = smgr->addSceneNode(
    "data/alcohol/bottle_beer.obj",
    "data/alcohol/heineken.mtl",
    "data/glsl/phong-2"
  );
  m_collisionBox = cmgr->addBox(CVector4f(0.4f, 1.4f, 0.4f));

//  CAbstractNode* node = smgr->addSceneNode(
//	CResourceFactory::instance().genMesh_Quad(0.4f, 1.4f),
//	smart_ptr<CAbstractMaterial>(new CGenericMaterial()),
//	CResourceFactory::instance().buildProgram("data/glsl/phong-2")
//  );
//  if(node){
//	node->setRenderMethod(CAbstractNode::RENDER_WIRED);
//	m_sceneNode->appendChild(node);
//  }

  setName("Beer");
  setAlcohol(5.2f);
}

CBottleHeineken::~CBottleHeineken(){}
// <--

/* ----------------------------------------------------------------------------
 * CBottleWine
 */
CBottleWine::CBottleWine(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr)
: CBottle(smgr,cmgr)
{
  m_sceneNode = smgr->addSceneNode(
    "data/alcohol/bottle_wine.obj",
    "data/alcohol/wine.mtl",
    "data/glsl/phong-2"
  );
  m_collisionBox = cmgr->addBox(CVector4f(0.4f, 2.0f, 0.4f));
  setName("Wine");
  setAlcohol(10.1f);
}

CBottleWine::~CBottleWine(){}
// <--

/* ----------------------------------------------------------------------------
 * CExample
 */
CExample::CExample(int argc, char* argv[])
  : CApplication(argc,argv)
{
  m_pause = false;
  m_pauseId = 0;

  m_focusId = 0;
  m_lara = 0;

  m_pitch = false;
  m_pitchAngle = 0.0f;

  srand(time(0));
}

CExample::~CExample() {
  // pass
}

bool CExample::addBottle(CBottle* bottle){
  if(bottle && getBottle(bottle->getCollisionBox()->getId())!=0) return false;

  m_bottle.push_back(smart_ptr<CBottle>(bottle));

  return true;
}

bool CExample::delBottle(CBottle* bottle){
  const int id = bottle->getCollisionBox()->getId();
  vector< smart_ptr<CBottle> >::iterator itr;
  for(itr=m_bottle.begin(); itr!=m_bottle.end(); ++itr){
    if(id == (*itr)->getCollisionBox()->getId()){
      m_bottle.erase(itr);
      return true;
    }
  }
  return false;
}

CBottle* CExample::getBottle(int id){
  vector< smart_ptr<CBottle> >::iterator itr;
  for(itr=m_bottle.begin(); itr!=m_bottle.end(); ++itr){
    if(id == (*itr)->getCollisionBox()->getId()){
      return (*itr).entity();
    }
  }
  return 0;
}

void CExample::create(){
  m_dev.setCaption("Example Application v1.0");
  m_dev.setMousePosition(CVector2i(m_dev.getSize().x()/2, m_dev.getSize().y()/2));
  m_postprocess[0] = m_res.buildProgram("data/glsl/default");
  m_postprocess[1] = m_res.buildProgram("data/glsl/gauss_blur-25");
  m_dev.setPostProgram(m_postprocess[0]);

  // Dodaj menadzery
  ISceneManager* smgr = 0;
  iCollisionManager* cmgr = 0;

  // Room
  addSceneManager((smgr = new CSceneManager(m_dev)));
  addCollisionManager((cmgr = new CCollisionManager()));

  // Gracz = Kamera + Punkt wykrywajacy kolizje
  CAbstractCamera* cam = smgr->addPerspectiveCamera(60.0f);
  smgr->setActiveCamera(cam);
  cAbstractPoint* point = cmgr->addPoint();
  point->setPosition(CVector4f(0.0f, 5.0f, 12.0f));
  m_player = smart_ptr<CPlayer>(new CPlayer(cam,point));

  // Swiatlo
  CAbstractLight* light = 0;
  light = smgr->addPointLight();
  light->setPosition(CVector4f(0.0f,6.0f,8.0f));
  light = smgr->addPointLight();
  light->setPosition(CVector4f(0.0f,6.0f,-8.0f));

  build_room(smgr,cmgr);

  // Alkohole
  addCollisionManager((cmgr = new CCollisionManager()));

  // Dodaje do aplikacji kolejny mendzer sceny z kamera 2d
  addSceneManager((smgr = new CSceneManager(m_dev)));
  smgr->setActiveCamera(smgr->add2DCamera());
  build_hud(smgr);

  // Ekran pauzy
  addSceneManager((smgr = new CSceneManager(m_dev)));
  smgr->setActiveCamera(smgr->add2DCamera());
  build_pause(smgr);

  // Ustaw alkohol
  game_begin();
}

void CExample::destroy(){
	// pass
}

void CExample::game_begin(){
  // Reset gracza
  m_player->setAlcohol(0.0f);
  m_player->getCamera()->setPosition(CVector4f(0.0f, 5.0f, 12.0f));
  m_player->getCamera()->setLookAt(CVector4f(0.0f,5.0f,0.0f));

  // Dodanie alkoholu
  build_alcohol(getSceneManager(0),getCollisionManager(1));

  // Update statystyk
  stat_eval();
}

void CExample::game_end(){
  vector< smart_ptr<CBottle> >::iterator i;
  for(i=m_bottle.begin(); i!=m_bottle.end(); ++i){
    m_player->setAlcohol(m_player->getAlcohol() + (*i)->getAlcohol());
  }
  m_bottle.clear();
  m_bottle.resize(0);
  stat_eval();
}

void CExample::pause(bool enabled){
  if(enabled){
    m_pause = true;
    m_pauseId = 0;
    m_dev.setCursorVisible(true);
    getSceneManager(2)->setEnabled(true);
    m_dev.setKeyRepeat(false);
    m_dev.setPostProgram(m_postprocess[0]);
  }
  else{
    m_pause = false;
    m_dev.setCursorVisible(false);
    getSceneManager(2)->setEnabled(false);
    m_dev.setMousePosition(
        CVector2i(m_dev.getSize().w()/2.0f,m_dev.getSize().h()/2.0f)
    );
    m_dev.setKeyRepeat(true);
    stat_eval();
  }
}

void CExample::build_room(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr){
  CAbstractNode* sceneNode = 0;
  cAbstractPrimitive* collisionNode = 0;

  // Pomieszczenie
  sceneNode = smgr->addSceneNode("data/floor.obj","data/floor.mtl","data/glsl/phong-2");
  sceneNode = smgr->addSceneNode("data/wall.obj","data/wall.mtl","data/glsl/phong-2");
  sceneNode = smgr->addSceneNode("data/beam.obj","data/beam.mtl","data/glsl/phong-2");
  collisionNode = cmgr->addBox(CVector4f(28.0f, 8.0f, 28.0f));
  collisionNode->setPosition(CVector4f(0.0f, 4.0f, 0.0f));

  // Regaly
  sceneNode = smgr->addSceneNode("data/bookcase.obj","data/bookcase.mtl","data/glsl/phong-2");
  collisionNode = cmgr->addBox(CVector4f(4.0f, 8.0f, 10.4f));
  collisionNode->setPosition(CVector4f(8.2f, 4.0f, 4.7f));
  collisionNode = cmgr->addBox(CVector4f(4.0f, 8.0f, 10.4f));
  collisionNode->setPosition(CVector4f(-8.2f, 4.0f, 4.7f));

  // Stoly
  sceneNode = smgr->addSceneNode("data/table.obj", "data/table.mtl","data/glsl/phong-2");
  collisionNode = cmgr->addBox(CVector4f(9.0f, 8.0f, 4.8f));
  collisionNode->setPosition(CVector4f(6.9f, 4.0f, -9.0f));
  collisionNode = cmgr->addBox(CVector4f(9.0f, 8.0f, 4.8f));
  collisionNode->setPosition(CVector4f(-6.9f, 4.0f, -9.0f));
}

void CExample::build_alcohol(ISceneManager* smgr, iCollisionManager* cmgr)
{
  unsigned int slot_count = 92;
  CVector4f slot[] = {
    // Stolik prawy (28)
    CVector4f(4.0f, 2.9f, -7.5f), CVector4f(4.0f, 2.9f, -8.5f),
    CVector4f(4.0f, 2.9f, -9.5f), CVector4f(4.0f, 2.9f, -10.5f),
    CVector4f(5.0f, 2.9f, -7.5f), CVector4f(5.0f, 2.9f, -8.5f),
    CVector4f(5.0f, 2.9f, -9.5f), CVector4f(5.0f, 2.9f, -10.5f),
    CVector4f(6.0f, 2.9f, -7.5f), CVector4f(6.0f, 2.9f, -8.5f),
    CVector4f(6.0f, 2.9f, -9.5f), CVector4f(6.0f, 2.9f, -10.5f),
    CVector4f(7.0f, 2.9f, -7.5f), CVector4f(7.0f, 2.9f, -8.5f),
    CVector4f(7.0f, 2.9f, -9.5f), CVector4f(7.0f, 2.9f, -10.5f),
    CVector4f(8.0f, 2.9f, -7.5f), CVector4f(8.0f, 2.9f, -8.5f),
    CVector4f(8.0f, 2.9f, -9.5f), CVector4f(8.0f, 2.9f, -10.5f),
    CVector4f(9.0f, 2.9f, -7.5f), CVector4f(9.0f, 2.9f, -8.5f),
    CVector4f(9.0f, 2.9f, -9.5f), CVector4f(9.0f, 2.9f, -10.5f),
    CVector4f(10.0f, 2.9f, -7.5f), CVector4f(10.0f, 2.9f, -8.5f),
    CVector4f(10.0f, 2.9f, -9.5f), CVector4f(10.0f, 2.9f, -10.5f),

    // Stolik lewy (28)
    CVector4f(-4.0f, 2.9f, -7.5f), CVector4f(-4.0f, 2.9f, -8.5f),
    CVector4f(-4.0f, 2.9f, -9.5f), CVector4f(-4.0f, 2.9f, -10.5f),
    CVector4f(-5.0f, 2.9f, -7.5f), CVector4f(-5.0f, 2.9f, -8.5f),
    CVector4f(-5.0f, 2.9f, -9.5f), CVector4f(-5.0f, 2.9f, -10.5f),
    CVector4f(-6.0f, 2.9f, -7.5f), CVector4f(-6.0f, 2.9f, -8.5f),
    CVector4f(-6.0f, 2.9f, -9.5f), CVector4f(-6.0f, 2.9f, -10.5f),
    CVector4f(-7.0f, 2.9f, -7.5f), CVector4f(-7.0f, 2.9f, -8.5f),
    CVector4f(-7.0f, 2.9f, -9.5f), CVector4f(-7.0f, 2.9f, -10.5f),
    CVector4f(-8.0f, 2.9f, -7.5f), CVector4f(-8.0f, 2.9f, -8.5f),
    CVector4f(-8.0f, 2.9f, -9.5f), CVector4f(-8.0f, 2.9f, -10.5f),
    CVector4f(-9.0f, 2.9f, -7.5f), CVector4f(-9.0f, 2.9f, -8.5f),
    CVector4f(-9.0f, 2.9f, -9.5f), CVector4f(-9.0f, 2.9f, -10.5f),
    CVector4f(-10.0f, 2.9f, -7.5f), CVector4f(-10.0f, 2.9f, -8.5f),
    CVector4f(-10.0f, 2.9f, -9.5f), CVector4f(-10.0f, 2.9f, -10.5f),

    // Regal prawy (18)
    CVector4f(8.5f, 2.6f, 1.0f), CVector4f(8.5f, 2.6f, 2.5f),
    CVector4f(8.5f, 2.6f, 4.0f), CVector4f(8.5f, 4.1f, 1.0f),
    CVector4f(8.5f, 4.1f, 2.5f), CVector4f(8.5f, 4.1f, 4.0f),
    CVector4f(8.5f, 5.6f, 1.0f), CVector4f(8.5f, 5.6f, 2.5f),
    CVector4f(8.5f, 5.6f, 4.0f), CVector4f(8.5f, 2.6f, 5.5f),
    CVector4f(8.5f, 2.6f, 7.0f), CVector4f(8.5f, 2.6f, 8.5f),
    CVector4f(8.5f, 4.1f, 5.5f), CVector4f(8.5f, 4.1f, 7.0f),
    CVector4f(8.5f, 4.1f, 8.5f), CVector4f(8.5f, 5.6f, 5.5f),
    CVector4f(8.5f, 5.6f, 7.0f), CVector4f(8.5f, 5.6f, 8.5f),

    // Regal lewy (18)
    CVector4f(-8.5f, 2.6f, 1.0f), CVector4f(-8.5f, 2.6f, 2.5f),
    CVector4f(-8.5f, 2.6f, 4.0f), CVector4f(-8.5f, 4.1f, 1.0f),
    CVector4f(-8.5f, 4.1f, 2.5f), CVector4f(-8.5f, 4.1f, 4.0f),
    CVector4f(-8.5f, 5.6f, 1.0f), CVector4f(-8.5f, 5.6f, 2.5f),
    CVector4f(-8.5f, 5.6f, 4.0f), CVector4f(-8.5f, 2.6f, 5.5f),
    CVector4f(-8.5f, 2.6f, 7.0f), CVector4f(-8.5f, 2.6f, 8.5f),
    CVector4f(-8.5f, 4.1f, 5.5f), CVector4f(-8.5f, 4.1f, 7.0f),
    CVector4f(-8.5f, 4.1f, 8.5f), CVector4f(-8.5f, 5.6f, 5.5f),
    CVector4f(-8.5f, 5.6f, 7.0f), CVector4f(-8.5f, 5.6f, 8.5f)
  };


  // Losowanie
  for(int i=0; i<60; ++i){
	CBottle* ptr = random_bottle(smgr,cmgr);
    int n = rand()%slot_count;
    if(ptr){
      ptr->setPosition(slot[n]);
      slot[n] = slot[--slot_count];
      addBottle(ptr);
    }
  }
}

void CExample::build_hud(ISceneManager* smgr){
  CAbstractNode* node = 0;

  // Kursor 
  m_cursor[0] = m_res.buildTexture("data/aim.tga");
  m_cursor[1] = m_res.buildTexture("data/hand.tga");
  m_cursorNode= smgr->addSceneNode(m_cursor[0], m_res.buildProgram("data/glsl/default"));

  // Etykieta wyswietlajaca nazwe wskazywanego alkoholu
  m_labelHint = m_res.createTextTexture(256, 32, "data/DejaVuSansMono.ttf","HUD.HINT", 16);
  if(m_labelHint){
    m_labelHint->setBackgroundColor(CVector4f(0.2f, 0.2f, 0.2f, 0.3f));
    m_labelHint->setForegroundColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_labelHint->setText("none");
  }
  node = smgr->addSceneNode(m_labelHint.up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 0.0f);
    p.x() = -((m_dev.getSize().w()/2.0f)-(m_labelHint->getWidth()/2.0f+10.0f));
    p.y() = -((m_dev.getSize().h()/2.0f)-(m_labelHint->getHeight()/2.0f+10.0f));
    node->setPosition(p);
  }

  // Etykieta wyswietlajaca dane gracza
  m_labelStat = m_res.createTextTexture(256, 32, "data/DejaVuSansMono.ttf","HUD.STAT", 16);
  if(m_labelStat){
    m_labelStat->setBackgroundColor(CVector4f(0.2f, 0.2f, 0.2f, 0.3f));
    m_labelStat->setForegroundColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_labelStat->setText("?");
  }
  node = smgr->addSceneNode(m_labelStat.up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 0.0f);
    p.x() = -((m_dev.getSize().w()/2.0f)-(m_labelHint->getWidth()/2.0f+10.0f));
    p.y() = -((m_dev.getSize().h()/2.0f)-(m_labelHint->getHeight()/2.0f+44.0f));
    node->setPosition(p);
  }

  // Licznik FPS
  m_labelFps = m_res.createTextTexture(128, 32, "data/DejaVuSansMono.ttf","HUD.FPS", 16);
  if(m_labelFps){
    m_labelFps->setBackgroundColor(CVector4f(0.3f, 0.3f, 0.3f, 0.3f));
    m_labelFps->setForegroundColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
    m_labelFps->setText("FPS");
  }
  node = smgr->addSceneNode(m_labelFps.up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 0.0f);
    p.x() = -((m_dev.getSize().w()/2.0f)-(m_labelFps->getWidth()/2.0f+10.0f));
    p.y() = (m_dev.getSize().h()/2.0f)-(m_labelFps->getHeight()/2.0f+10.0f);
    node->setPosition(p);
  }
}

void CExample::build_pause(ISceneManager* smgr){
  CAbstractNode* node = 0;

  // Tlo
  smart_ptr<CAbstractMesh> pause_bg_mesh(
      m_res.genMesh_Quad(float(m_dev.getSize().w())/2.0f, float(m_dev.getSize().h()))
  );
  smart_ptr<CAbstractMaterial> pause_bg_mat(new CGenericMaterial("PAUSE.BG.MATERIAL"));
  pause_bg_mat->setAmbient(CVector4f(0.0f, 0.0f, 0.0f, 0.5f));
  smart_ptr<CAbstractProgram> pause_bg_prog(m_res.buildProgram("data/glsl/phong-2"));
  node = smgr->addSceneNode(pause_bg_mesh, pause_bg_mat, pause_bg_prog);
  if(node){
    node->setRenderMethod(CAbstractNode::RENDER_SOLID);
    node->setPosition(CVector4f(float(m_dev.getSize().w())/4.0f, 0.0f, 1.0));
  }

  // Resume
  m_labelPause[0] = m_res.createTextTexture(256, 32, "data/DejaVuSansMono.ttf","PAUSE.RESUME", 16);
  if(m_labelPause[0]){
	m_labelPause[0]->setText("Back");
  }
  node = smgr->addSceneNode(m_labelPause[0].up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 2.0f);
    p.x() = float(m_labelPause[0]->getWidth()/2.0f)+20.0f;
    p.y() = -float(m_labelPause[0]->getHeight());
    node->setPosition(p);
  }

  // Restart
  m_labelPause[1] = m_res.createTextTexture(256, 32, "data/DejaVuSansMono.ttf","PAUSE.RESTART", 16);
  if(m_labelPause[1]){
	m_labelPause[1]->setText("Restart");
  }
  node = smgr->addSceneNode(m_labelPause[1].up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 2.0f);
    p.x() = float(m_labelPause[1]->getWidth()/2.0f)+20.0f;
    p.y() = -float(m_labelPause[1]->getHeight())*2.0f;
    node->setPosition(p);
  }

  // Quit
  m_labelPause[2] = m_res.createTextTexture(256, 32, "data/DejaVuSansMono.ttf","PAUSE.QUIT", 16);
  if(m_labelPause[2]){
	m_labelPause[2]->setText("End");
  }
  node = smgr->addSceneNode(m_labelPause[2].up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 2.0f);
    p.x() = float(m_labelPause[2]->getWidth()/2.0f)+20.0f;
    p.y() = -float(m_labelPause[2]->getHeight())*3.0f;
    node->setPosition(p);
  }

  // Etykieta wyswietlajaca informacje pauzy
  m_labelPause[3] = m_res.createTextTexture(256, 64, "data/DejaVuSansMono.ttf","PAUSE.INFO", 32);
  if(m_labelPause[3]){
    m_labelPause[3]->setForegroundColor(CVector4f(0.8f, 0.8f, 0.8f, 0.8f));
	m_labelPause[3]->setText("-P-A-U-S-E-");
  }
  node = smgr->addSceneNode(m_labelPause[3].up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 2.0f);
    p.x() = float(m_labelPause[3]->getWidth()/2.0f)+20.0f;
    p.y() = float(m_dev.getSize().h()/2.0f-m_labelPause[3]->getHeight());
    node->setPosition(p);
  }

  // Etykieta wyswietlajaca informacje o autorach
  m_labelPause[4] = m_res.createTextTexture(256, 64, "data/DejaVuSansMono.ttf","PAUSE.INFO", 12);
  if(m_labelPause[4]){
    m_labelPause[4]->setForegroundColor(CVector4f(0.8f, 0.8f, 0.8f, 0.8f));
	m_labelPause[4]->setText("(c) Artur Sobierak");
  }
  node = smgr->addSceneNode(m_labelPause[4].up_cast<CAbstractTexture>(),
                            m_res.buildProgram("data/glsl/default"));
  if(node){
    CVector4f p(0.0f, 0.0f, 2.0f);
    p.x() = float(m_labelPause[4]->getWidth()/2.0f)+20.0f;
    p.y() = float(m_dev.getSize().h()/2.0f-m_labelPause[3]->getHeight()*2.0f);
    node->setPosition(p);
  }

  smgr->setEnabled(false);
}

CBottle* CExample::random_bottle(ISceneManager* smgr, iCollisionManager* cmgr){
  CBottle* ptr = 0;
  unsigned int id = rand()%2;

  switch(id){
	case 0: ptr = new CBottleHeineken(smgr,cmgr); break;
	case 1: ptr = new CBottleWine(smgr,cmgr); break;
    default: break;
  }

  return ptr;
}

void CExample::frame(){
  // Update FPS
  if(m_labelFps){
    char buff[12];
    sprintf(buff,"FPS: %4.1f", m_dev.getFPS());
    m_labelFps->setText(buff);
  }

  if(m_pause)
  {
    // Odrysuj menu
    for(int i=0; i<3; ++i){
      if(i == m_pauseId){
        m_labelPause[i]->setForegroundColor(CVector4f(1.0f,0.6f,0.0f));
      }
      else{
        m_labelPause[i]->setForegroundColor(CVector4f(1.0f,1.0f,1.0f));
      }
      m_labelPause[i]->setText(m_labelPause[i]->getText());
    }

    if(m_dev.getKeyState(SDLK_UP) || m_dev.getKeyState(SDLK_DOWN)){
      if(m_dev.getKeyState(SDLK_UP) && m_pauseId>0){
        m_dev.setKeyState(SDLK_UP, false);
        --m_pauseId;
      }
      if(m_dev.getKeyState(SDLK_DOWN) && m_pauseId<2){
        m_dev.setKeyState(SDLK_DOWN, false);
        ++m_pauseId;
      }
    }

    if(m_dev.getKeyState(SDLK_RETURN)){
      switch(m_pauseId){
        case 1:
          m_bottle.clear();
          game_begin();
          break;
        case 2:
          m_dev.quit();
          break;
        default: break;
      }
      pause(false);
    }

    // czity
    if(m_dev.getKeyState(SDLK_0)){
      game_end();
      pause(false);
    }
  }
  else{
    // Update pozycji kamerki
    m_player->warp(m_dev);

    // Interakcja ze swiatem
    iCollisionManager* cmgr = getCollisionManager(1);
    const CVector4f& pos = m_player->getCamera()->getPosition();
    const CVector4f& at = m_player->getCamera()->getLookAt();
    int id = cmgr->select(pos, CVector4f::norm(at-pos)*3.0f);
    if(id != m_focusId){
      m_focusId = id;
      focus(m_focusId);
    }
    if(m_focusId!=0 && m_dev.getMouseState() == 1){
      click(m_focusId);
    }

    // Kolysanie
    if(m_pitch){
      CVector4f up(m_player->getCamera()->getLookAt()-m_player->getCamera()->getPosition());
      up = CVector4f::norm(CVector4f::cross(CVector4f(0.0f,1.0f,0.0f),up));
      up *= sin(m_pitchAngle)*0.1f*tanh(m_player->getAlcohol()/200.0f);
      up += CVector4f(0.0f,1.0f,0.0f);

      m_player->getCamera()->setUp(up);

      m_pitchAngle += m_dev.getFrameDelta();
    }
    else{
      m_player->getCamera()->setUp(CVector4f(0.0f,1.0f,0.0f));
    }

    if(m_dev.getKeyState(SDLK_ESCAPE)) pause(true);
  }
}

void CExample::focus(int id){
  CBottle* bottle = getBottle(id);
  if(bottle){
    m_labelHint->setText(bottle->getName());
    m_cursorNode->getMaterial()->setTexture(m_cursor[1]);
  }
  else{
    m_labelHint->setText("none");
    m_cursorNode->getMaterial()->setTexture(m_cursor[0]);
  }
}

void CExample::click(int id){
  CBottle* bottle = getBottle(id);
  if(bottle){
    m_log.msg(CLogMessage() << "Wypito: " << bottle->getName()
        << " (" << bottle->getAlcohol() <<")");
    m_player->setAlcohol(m_player->getAlcohol() + bottle->getAlcohol());
    delBottle(bottle);

    stat_eval();
  }
}

void CExample::stat_eval(){
  // Predkosc poruszania sie
  m_player->setSpeed(10.0f/(m_player->getAlcohol()/40.0f + 1.0f));

  // Zmiana FOV
  CPerspectiveCamera* cam = dynamic_cast<CPerspectiveCamera*>(m_player->getCamera());
  if(cam){
    cam->setFOV(60.0f + min<float>((m_player->getAlcohol()/5.0f), 100.0f));
  }

  // Kolysanie
  if(m_player->getAlcohol()>40.0f)
    m_pitch = true;
  else
    m_pitch = false;

  // Rozmycie gaussa
  if(m_player->getAlcohol()>80.0f)
    m_dev.setPostProgram(m_postprocess[1]);
  else
    m_dev.setPostProgram(m_postprocess[0]);

  // Wyswietla aktualna zawartosc alkoholu
  if(m_labelStat){
    char buff[64];
    sprintf(buff, "Alkohol: %5.2f", m_player->getAlcohol());
    m_labelStat->setText(buff);
  }

  if(m_bottle.size() == 0){
    m_labelHint->setText("Gratulacje !");
  }
  else{
    m_labelHint->setText("none");
  }
}
