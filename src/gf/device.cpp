/* File: device.cpp
 * Date: 01.06.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#include "device.hpp"

using namespace std;
using namespace gf;

/* ----------------------------------------------------------------------------
 * cDevice
 */
CDevice::CDevice() : m_log(CLog::instance())
{
  m_run = false;
  m_mouseState = 0;
  m_frameLast = m_frameDelta = 0.0;
  m_FPS = 0.0f;
  m_fpsCounter = 0;
  m_fpsLastUpdate = 0.0f;

  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
    m_log.msg(CLogError() << "Blad inicjalizacji biblioteki SDL: " << SDL_GetError());
  }
  else {
    const SDL_version *ver = SDL_Linked_Version();
    m_log.msg(CLogMessage()<<"Zainicjowano SDL: "<< ver->major << "."
        << ver->minor << "." << ver->patch);
    m_run = true;
  }
}

CDevice::~CDevice() {
  SDL_Quit();
  m_log.msg(CLogMessage() << "Koniec");
}

bool CDevice::create(int width, int height, bool fullscreen) {
  // Store settings
  m_size.x() = width;
  m_size.y() = height;
  m_fullscreen = fullscreen;

  // Setup SDL window
  if (!setupSDL())
    return false;

  // Setup OpenGL library
  if (!setupOpenGL())
    return false;

  m_fb = smart_ptr<CFrameBuffer>(new CFrameBuffer(m_size.w(),m_size.h()));
  m_fbQuad = CResourceFactory::instance().genMesh_Quad((float)m_size.w(), (float)m_size.h());

  m_log.msg(CLogMessage() << "Pomyslnie utworzono obiekt urzadzenia");

  return true;
}

bool CDevice::setupSDL() {
  if (!m_run)
    return false;

  const SDL_VideoInfo* video;
  video = SDL_GetVideoInfo();
  if (video == 0) {
    m_log.msg(CLogError() << "SDL can't get video mode information: " << SDL_GetError());
    m_run = false;
    return false;
  }

  char vdriver[16];
  SDL_VideoDriverName(vdriver, 16);

  m_log.msg(
      CLogMessage() << "Driver: " << vdriver << " Mode: " << m_size.x()
      << "x" << m_size.y() << "x" << video->vfmt->BitsPerPixel
  );

  unsigned int flags = SDL_OPENGL;
  if (m_fullscreen)
    flags = flags | SDL_FULLSCREEN;
  //Selse
  //    flags = flags|SDL_RESIZABLE;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); // V-sync

  if (SDL_SetVideoMode(m_size.x(), m_size.y(), video->vfmt->BitsPerPixel, flags)
      == 0)
  {
    m_log.msg(CLogError() << "SDL surface initialization problem: " << SDL_GetError());
    m_run = false;
    return false;
  }

  setKeyRepeat(true);

  return true;
}

bool CDevice::setupOpenGL() {
  m_log.msg(CLogMessage() << "Ladowanie rozszerzen OpenGL");

  if(glewInit() || !glewIsSupported("GL_VERSION_2_1 GL_ARB_framebuffer_object")){
    m_log.msg(CLogError() << "Nie znaleziono wymaganych rozszerzen");
    return false;
  }

  int param = 0;
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &param);
  if(param < 4){
    m_log.msg(CLogError() << "Karta nie posida 4 jednostek teksturujacych");
    return false;
  }

  m_log.msg(
    CLogMessage()
    << "OpenGL " << (char*) glGetString(GL_VERSION) << " "
    << "GLSL " << (char*) glGetString(GL_SHADING_LANGUAGE_VERSION)
  );

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_GREATER,0.1f);

  return true;
}

const CVector2i& CDevice::getSize() const {
  return m_size;
}

const string& CDevice::getCaption() const {
  char* ptr;
  SDL_WM_GetCaption(&ptr, 0);
  m_caption = ptr;
  return m_caption;
}

bool CDevice::getKeyState(SDLKey key) const {
  return (m_keyState[key] != 0);
}

bool CDevice::getKeyRepeat() const {
  return false;
}

const CVector2i& CDevice::getMousePosition() const {
  return m_mousePosition;
}

unsigned char CDevice::getMouseState() const {
  return m_mouseState;
}

bool CDevice::getCursorVisible() const {
  if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
    return true;
  else
    return false;
}

const float& CDevice::getFrameDelta() const {
  return m_frameDelta;
}

float CDevice::getFPS() const {
  return m_FPS;
}

void CDevice::setSize(const CVector2i& size){
  m_size = size;
}

void CDevice::setCaption(const string& str) {
  m_caption = str;
  SDL_WM_SetCaption(m_caption.c_str(), 0);
}

void CDevice::setCursorVisible(bool visible){
  if (visible)
    SDL_ShowCursor(SDL_ENABLE);
  else
    SDL_ShowCursor(SDL_DISABLE);
}

void CDevice::setKeyState(SDLKey key, bool state){
  m_keyState[key] = state;
}

void CDevice::setKeyRepeat(bool repeat){
  if(repeat){
    SDL_EnableKeyRepeat(50, SDL_DEFAULT_REPEAT_INTERVAL);
  }
  else{
    SDL_EnableKeyRepeat(0, 0);
  }
}

void CDevice::setMousePosition(const CVector2i& pos){
  m_mousePosition = pos;
  SDL_WarpMouse(pos.x(), pos.y());
}

void CDevice::setMouseState(unsigned char state){
  m_mouseState = state;
}

bool CDevice::quit() {
  m_run = false;
  return true;
}

bool CDevice::run() {
  SDL_PumpEvents(); // Sprawdza stan wszystkich zrodel zdarzen

  float now = SDL_GetTicks()/1000.0f;

  m_frameDelta = now - m_frameLast;
  m_frameLast = now;

  ++m_fpsCounter;
  if((now-m_fpsLastUpdate) > 2.0f){
    m_FPS = m_fpsCounter/(now-m_fpsLastUpdate);
    m_fpsCounter = 0;
    m_fpsLastUpdate = now;
  }

  m_keyState = SDL_GetKeyState(0);
  m_mouseState = SDL_GetMouseState(&m_mousePosition.x(), &m_mousePosition.y());

  return m_run;
}

bool CDevice::event(SDL_Event* ev) {
  if (SDL_PollEvent(ev)) {
    switch (ev->type) {
    case SDL_QUIT:
      m_run = false;
      break;
    default:
      break;
    }
    return true;
  } else {
    ev = 0;
    return false;
  }
}

smart_ptr<CAbstractProgram> CDevice::getPostProgram() const {
  return m_fbProg;
}

void CDevice::setPostProgram(smart_ptr<CAbstractProgram> prog){
  m_fbProg = prog;
}

bool CDevice::drawBegin(const CVector3f& clear) {
  if(!m_fb || !m_fbQuad || !m_fbProg){
    return false;
  }

  glClearColor(clear.r(), clear.g(), clear.b(), 0);
  m_fb->setEnabled(true);
  m_fb->clear();

  return true;
}

bool CDevice::drawEnd() {
  m_fb->setEnabled(false);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, m_size.w(), 0, m_size.h(), 0.1, 20.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef((float)m_size.w()/2.0f, (float)m_size.h()/2.0f, -10.0f);

  if(m_fb && m_fbQuad && m_fbQuad->vaild()){
    if(m_fbProg){
      m_fbProg->bind();
      m_fbProg->setUniform1f("fb.width", (float)m_size.w());
      m_fbProg->setUniform1f("fb.height", (float)m_size.h());
    }
    else{
      glUseProgram(0);
    }

    m_fb->bind();
    m_fbQuad->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_fbQuad->getVertexCount());
  }

  SDL_GL_SwapBuffers();
  return true;
}
