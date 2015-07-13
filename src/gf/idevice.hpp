/* File: idevice.hpp
 * Date: 01.06.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#ifndef _IDEVICE_HEADER_
#define _IDEVICE_HEADER_

#include <string>
#include "resource.hpp"
#include "common.hpp"

namespace gf
{
  class IDevice {
  public:
	virtual ~IDevice(){}

    virtual bool create(int width, int height, bool fullscreen) = 0;

    virtual const CVector2i& getSize() const = 0;
    virtual const std::string& getCaption() const = 0;
    virtual bool getKeyState(SDLKey key) const = 0;
    virtual bool getKeyRepeat() const = 0;
    virtual const CVector2i& getMousePosition() const = 0;
    virtual unsigned char getMouseState() const = 0;
    virtual bool getCursorVisible() const = 0;
    virtual const float& getFrameDelta() const = 0;
    virtual float getFPS() const = 0;

    virtual void setSize(const CVector2i& size) = 0;
    virtual void setKeyState(SDLKey key, bool state) = 0;
    virtual void setKeyRepeat(bool repeat) = 0;
    virtual void setMousePosition(const CVector2i& pos) = 0;
    virtual void setMouseState(unsigned char state) = 0;
    virtual void setCaption(const std::string& str) = 0;
    virtual void setCursorVisible(bool visible) = 0;

    virtual smart_ptr<CAbstractProgram> getPostProgram() const = 0;
    virtual void setPostProgram(smart_ptr<CAbstractProgram> prog) = 0;

    virtual bool drawBegin(const CVector3f& clear = CVector3f(0,0,0)) = 0;
    virtual bool drawEnd() = 0;

    virtual bool run() = 0;
    virtual bool quit() = 0;

    virtual bool event(SDL_Event* ev) = 0;
  };
}

#endif //_IDEVICE_HEADER_
