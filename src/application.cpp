/* File: application.cpp
 * Date: 29.08.2010
 */

#include "application.hpp"

using namespace std;
using namespace gf;
using namespace pf;

CApplication::CApplication(int argc, char* argv[])
  : m_log(CLog::instance()), m_res(CResourceFactory::instance())
{
  m_error = 0;

  int width = 800;
  int height = 600;
  bool fullscreen = false;

  for(int i=1; i<argc; ++i){
    string arg(argv[i]);
    if(arg.find("-fullscreen")!=string::npos){
      fullscreen = true;
    }
    if(arg.find("-width:")!=string::npos){
      width = atoi(arg.substr(arg.find("-width:")+7).c_str());
    }
    if(arg.find("-height:")!=string::npos){
      height = atoi(arg.substr(arg.find("-height:")+8).c_str());
    }
  }

  if(m_dev.create(width, height, fullscreen)){
    m_dev.setCursorVisible(false);
    m_dev.setCaption("cApplication");
  }
  else{
    m_log.msg(CLogError() << "Uruchomienie aplikacji nie powiodlo sie");
    m_error = 0x1;
  }
}

CApplication::~CApplication()
{
  // pass
}

int CApplication::exec(){
  if(!m_error){
    // Inicjalizacja
    create();

    while(m_dev.run()){
        // Obliczenia
        frame();

        if(m_dev.drawBegin(CVector3f(0.2f,0.2f,0.2f))){

          vector< smart_ptr<ISceneManager> >::iterator itr;
          for(itr=m_scene.begin(); itr!=m_scene.end(); ++itr){
            (*itr)->render();
          }

          m_dev.drawEnd();
        }
        else{
          m_log.msg(CLogError() << "Blad urzadenia renderujacego");
          break;
        }
    }
  }

  destroy();

  return m_error;
}

void CApplication::addSceneManager(ISceneManager* smgr){
  m_scene.push_back(smart_ptr<ISceneManager>(smgr));
}

void CApplication::addCollisionManager(ICollisionManager* cmgr){
  m_collision.push_back(smart_ptr<ICollisionManager>(cmgr));
}

ISceneManager* CApplication::getSceneManager(int i){
  return m_scene.at(i).entity();
}

ICollisionManager* CApplication::getCollisionManager(int i){
  return m_collision.at(i).entity();
}
