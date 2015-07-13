/* File: iresource.hpp
 * Date: 01.06.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#ifndef _IRESOURCE_HEADER_
#define _IRESOURCE_HEADER_

#include <string>
#include "logger.hpp"
#include "common.hpp"

namespace gf
{
  class CAbstractResource {
  public:
    enum eState {STATE_UNKNOW, STATE_LOADED, STATE_UNLOADED, STATE_ERROR};

	CAbstractResource();
	virtual ~CAbstractResource();

    virtual const eState& getState() const;
    virtual const unsigned int* getHandle() const;
    virtual const std::string& getName() const;

    virtual void setState(const eState& state);
    virtual void setHandle(unsigned int* handle);
    virtual void setName(const std::string& name);

    virtual bool vaild() const;
    virtual void bind() const = 0;

  protected:
    iLogger& m_log;

  private:
    eState m_state;
    unsigned int* m_handle;
    std::string m_name;
  };

  class CAbstractTexture : public CAbstractResource {
  public:
	CAbstractTexture();
	virtual ~CAbstractTexture();

    virtual unsigned int getWidth() const;
    virtual unsigned int getHeight() const;
    virtual unsigned int getBpp() const;
    virtual unsigned int getTextureUnit() const;

    virtual void setWidth(unsigned int width);
    virtual void setHeight(unsigned int height);
    virtual void setBpp(unsigned int bpp);
    virtual void setTextureUnit(unsigned int texUnit);

    virtual bool copy(const std::vector<unsigned char>& data) = 0;

    virtual bool lock() = 0;
    virtual unsigned char* buffer() = 0;
    virtual bool unlock() = 0;

  private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_bpp;
    unsigned int m_texUnit;
  };

  class CAbstractMaterial : public CAbstractResource {
  public:
	CAbstractMaterial();
	virtual ~CAbstractMaterial();

	virtual smart_ptr<CAbstractTexture> getTexture(unsigned int texUnit) const;
    virtual const CVector4f& getAmbient() const;
    virtual const CVector4f& getDiffuse() const;
    virtual const CVector4f& getSpecular() const;
    virtual const float& getReflection() const;

	virtual void setTexture(smart_ptr<CAbstractTexture> texture);
    virtual void setAmbient(const CVector4f& ambient);
    virtual void setDiffuse(const CVector4f& diffuse);
    virtual void setSpecular(const CVector4f& specular);
    virtual void setReflection(const float& reflection);

  private:
	smart_ptr<CAbstractTexture> m_texture[4];
    CVector4f m_ambient;
    CVector4f m_diffuse;
    CVector4f m_specular;
    float m_reflection;
  };

  class CAbstractMesh : public CAbstractResource {
  public:
	CAbstractMesh();
	virtual ~CAbstractMesh();

    virtual unsigned int getVertexCount() const;
    virtual void setVertexCount(unsigned int vertexCount);

  private:
    unsigned int m_vertexCount;
  };

  class CAbstractProgram : public CAbstractResource {
  public:
	CAbstractProgram();
	virtual ~CAbstractProgram();

    virtual bool setUniform1i(const std::string& name, int value) const = 0;
    virtual bool setUniform1f(const std::string& name, float value) const = 0;
    virtual bool setUniform2f(const std::string& name, const CVector2f& v2) const = 0;
    virtual bool setUniform3f(const std::string& name, const CVector3f& v3) const = 0;
    virtual bool setUniform4f(const std::string& name, const CVector4f& v4) const = 0;

    //virtual int getUniform1i(const std::string& name) const = 0;
    //virtual float getUniform1f(const std::string& name) const = 0;
    //virtual const cVector2f& getUniform2f(const std::string& name) const = 0;
    //virtual const cVector3f& getUniform3f(const std::string& name) const = 0;
    //virtual const cVector4f& getUniform4f(const std::string& name) const = 0;
  };

  class CAbstractFrameBuffer : public CAbstractResource {
  public:
	CAbstractFrameBuffer();
	virtual ~CAbstractFrameBuffer();

    virtual bool getEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;

    virtual void clear() = 0;
  };

  class IResourceFactory {
  public:
	virtual ~IResourceFactory(){};

	virtual std::vector< smart_ptr<CAbstractMesh> > buildMesh(const std::string& path) = 0;
	virtual std::vector< smart_ptr<CAbstractMaterial> > buildMaterial(const std::string& path) = 0;
	virtual smart_ptr<CAbstractTexture> buildTexture(const std::string& path, int texUnit = 0) = 0;
	virtual smart_ptr<CAbstractProgram> buildProgram(const std::string& path) = 0;
  };
}

#endif //_IRESOURCE_HEADER_
