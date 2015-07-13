/* File: example.hpp
 * Date: 28.08.2010
 */


#ifndef _EXAMPLE_HEADER_
#define _EXAMPLE_HEADER_

#include <map>
#include <cstdlib>
#include <algorithm>
#include "application.hpp"
#include "gf/scene.hpp"
#include "pf/collision.hpp"

class CPlayer {
public:
  CPlayer(gf::CAbstractCamera* camera, pf::cAbstractPoint* point);
  virtual ~CPlayer();

  virtual gf::CAbstractCamera* getCamera();
  virtual pf::cAbstractPoint* getPoint();
  virtual float getSpeed() const;
  virtual float getAlcohol() const;

  virtual void setCamera(gf::CAbstractCamera* camera);
  virtual void setPoint(pf::cAbstractPoint* point);
  virtual void setSpeed(float speed);
  virtual void setAlcohol(float alcohol);

  virtual bool move(const gf::CVector4f& v);
  virtual void rotate(float angle, const gf::CVector4f& vec);
  virtual void warp(gf::IDevice& dev);

private:
  gf::iLogger& m_log;
  gf::CAbstractCamera* m_camera;
  pf::cAbstractPoint* m_point;

  float m_cameraSpeed;
  float m_mouseSpeed;
  float m_alcohol;
};

class CBottle {
public:
  CBottle(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);
  virtual ~CBottle();

  virtual const std::string& getName() const;
  virtual float getAlcohol() const;
  virtual const gf::CVector4f& getPosition() const;

  virtual void setName(const std::string& name);
  virtual void setAlcohol(float alcohol);
  virtual void setPosition(const gf::CVector4f& p);

  virtual const gf::CAbstractNode* getSceneNode() const;
  virtual const pf::cAbstractBox* getCollisionBox() const;

protected:
  gf::CAbstractNode* m_sceneNode;
  pf::cAbstractBox* m_collisionBox;

private:
  gf::ISceneManager* m_smgr;
  pf::iCollisionManager* m_cmgr;
  std::string m_name;
  float m_alcohol;
  gf::CVector4f m_position;
};

class CBottleHeineken : public CBottle {
public:
  CBottleHeineken(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);
  virtual ~CBottleHeineken();
};

class CBottleWine : public CBottle {
public:
  CBottleWine(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);
  virtual ~CBottleWine();
};

class CExample : public CApplication {
public:
  CExample(int argc, char* argv[]);
  virtual ~CExample();

protected:
  void create();
  void destroy();
  void frame();

  void focus(int id);
  void click(int id);

private:
  // Obiekt gracza
  gf::smart_ptr<CPlayer> m_player;

  // Pomieszczenie
  void build_room(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);

  // Alkohole
  std::vector< gf::smart_ptr<CBottle> > m_bottle;
  bool addBottle(CBottle* bottle);
  bool delBottle(CBottle* bottle);
  CBottle* getBottle(int id);
  void build_alcohol(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);
  CBottle* random_bottle(gf::ISceneManager* smgr, pf::iCollisionManager* cmgr);

  // HUD
  gf::CAbstractNode* m_cursorNode;
  gf::smart_ptr<gf::CAbstractTexture> m_cursor[2];
  gf::smart_ptr<gf::CTextTexture> m_labelFps;
  gf::smart_ptr<gf::CTextTexture> m_labelHint;
  gf::smart_ptr<gf::CTextTexture> m_labelStat;
  void build_hud(gf::ISceneManager* smgr);

  // Pauza
  bool m_pause;
  int m_pauseId;
  gf::smart_ptr<gf::CTextTexture> m_labelPause[5];
  void build_pause(gf::ISceneManager* smgr);

  // Ewaluacja
  gf::smart_ptr<gf::CAbstractProgram> m_postprocess[2];
  void stat_eval();

  // Helpers
  gf::CAbstractNode* m_lara;
  int m_focusId;
  bool m_pitch;
  float m_pitchAngle;

  void game_begin();
  void game_end();
  void pause(bool enabled);
};

#endif //_EXAMPLE_HEADER_
