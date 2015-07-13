/* File: device.hpp
 * Date: 01.06.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#ifndef _DEVICE_HEADER_
#define _DEVICE_HEADER_

#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "common.hpp"
#include "logger.hpp"
#include "resource.hpp"
#include "idevice.hpp"

namespace gf
{
  class CDevice: public IDevice {
  public:
	CDevice();
	virtual ~CDevice();

    bool create(int width, int height, bool fullscreen);

    const CVector2i& getSize() const;
    const std::string& getCaption() const;
    bool getKeyState(SDLKey key) const;
    bool getKeyRepeat() const;
    const CVector2i& getMousePosition() const;
    unsigned char getMouseState() const;
    bool getCursorVisible() const;
    const float& getFrameDelta() const;
    float getFPS() const;

    void setSize(const CVector2i& size);
    void setCaption(const std::string& str);
    void setKeyState(SDLKey key, bool state);
    void setKeyRepeat(bool repeat);
    void setMousePosition(const CVector2i& pos);
    void setMouseState(unsigned char state);
    void setCursorVisible(bool visible);

	smart_ptr<CAbstractProgram> getPostProgram() const ;
	void setPostProgram(smart_ptr<CAbstractProgram> prog);

    bool drawBegin(const CVector3f& clear = CVector3f(0,0,0));
    bool drawEnd();

    bool run();
    bool quit();

    bool event(SDL_Event* ev);

  protected:
    iLogger &m_log;
    bool setupSDL();
    bool setupOpenGL();
    void postprocessing();

  private:
    bool m_run;

    CVector2i m_size;
    bool m_fullscreen;
    mutable std::string m_caption;

    unsigned char* m_keyState;
    CVector2i m_mousePosition;
    unsigned char m_mouseState;

    float m_frameLast;
    float m_frameDelta;
    float m_fpsLastUpdate;
    int m_fpsCounter;
    float m_FPS;

	mutable smart_ptr<CFrameBuffer> m_fb;
	mutable smart_ptr<CAbstractMesh> m_fbQuad;
	mutable smart_ptr<CAbstractProgram> m_fbProg;
  };
}

#endif //_IDEVICE_HEADER_
