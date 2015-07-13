/*
 * (C) Copyright 2010 Artur Sobierak <asobierak@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
  void addCollisionManager(pf::ICollisionManager* cmgr);
  gf::ISceneManager* getSceneManager(int i);
  pf::ICollisionManager* getCollisionManager(int i);

  gf::CLogger& m_log;
  gf::CDevice m_dev;
  gf::CResourceFactory& m_res;

private:
  int m_error;
  std::vector< gf::smart_ptr<gf::ISceneManager> > m_scene;
  std::vector< gf::smart_ptr<pf::ICollisionManager> > m_collision;
};

#endif //_APPLICATION_HEADER_
