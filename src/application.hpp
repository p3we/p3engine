/* File: application.hpp
 * Date: 29.08.2010
 */

#ifndef _APPLICATION_HEADER_
#define _APPLICATION_HEADER_

#include <cstdlib>
#include <string>
#include "gf/logger.hpp"
#include "gf/device.hpp"
#include "gf/resource.hpp"
#include "gf/iscene.hpp"
#include "pf/icollision.hpp"

class CApplication {
public:
  CApplication(int argc, char* argv[]);
  virtual ~CApplication();

  /**
   * Uruchamianie aplikacji
   */
  virtual int exec();

protected:
  /**
   * Funkcja uruchamiana przed wejsciem do petli glownej programu
   */
  virtual void create() = 0;

  /**
   * Funkcja wywolywana po wysjciu z petli glownej programu
   */
  virtual void destroy() = 0;

  /**
   * Funkcja wywolywana w petli glownej programu, w kazdej klatce
   */
  virtual void frame() = 0;

  void addSceneManager(gf::ISceneManager* smgr);
  void addCollisionManager(pf::iCollisionManager* cmgr);
  gf::ISceneManager* getSceneManager(int i);
  pf::iCollisionManager* getCollisionManager(int i);

  gf::CLogger& m_log;
  gf::CDevice m_dev;
  gf::CResourceFactory& m_res;

private:
  int m_error;
  std::vector< gf::smart_ptr<gf::ISceneManager> > m_scene;
  std::vector< gf::smart_ptr<pf::iCollisionManager> > m_collision;
};

#endif //_APPLICATION_HEADER_
