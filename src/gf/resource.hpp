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

#ifndef _RESOURCE_HEADER_
#define _RESOURCE_HEADER_

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include "iresource.hpp"
#include "common.hpp"
#include "logger.hpp"

namespace gf
{
  /**
   * Klasa reprezentujaca teksture
   */
  class CGenericTexture : public CAbstractTexture {
  public:
    /**
     * Konstruktor tworzacy teksture z podanych danych
     * @param width Szerokosc tekstury w pixelach
     * @param height Wysokosc tekstury w pixelach
     * @param bpp Ilosc bitow przypadajacych na pixel
     * @param data Dane grafiki
     * @param name Nazwa zasobu
     * @param texUnit Numer jednostki teksturujacej
     */
	CGenericTexture(unsigned int width, unsigned int height, unsigned int bpp,
        const std::vector<GLubyte>& data, const std::string& name = "unnamed",
        unsigned int texUnit = 0);

    /**
     * Konstruktor tworzacy pusta teksture, rezerwuje miejsce na dane
     * @param width Szerokosc tekstury w pixelach
     * @param height Wysokosc tekstury w pixelach
     * @param bpp Ilosc bitow przypadajacych na pixel
     * @param data Dane grafiki
     * @param name Nazwa zasobu
     * @param texUnit Numer jednostki teksturujacej
     */
	CGenericTexture(unsigned int width, unsigned int height, unsigned int bpp,
        const std::string& name = "unnamed", unsigned int texUnit = 0);

	virtual ~CGenericTexture();

    bool copy(const std::vector<unsigned char>& data);
    bool lock();
    unsigned char* buffer();
    bool unlock();
    void bind() const;

  protected:
    void activeTextureUnit(unsigned int texUnit) const;

  private:
    unsigned int m_oglHandle[2];
    unsigned char* m_buffer;
  };

  /**
   * Tekstura generowanan na podstawie tekstu (Freetype 2)
   */
  class CTextTexture : public CGenericTexture {
  public:
	CTextTexture(unsigned int width, unsigned int height, FT_Face& face,
        const std::string& name, unsigned int texUnit = 0);
	virtual ~CTextTexture();

    virtual const CVector4f& getForegroundColor() const;
    virtual const CVector4f& getBackgroundColor() const;
    virtual int getMargin() const;
    virtual const std::string& getText() const;

    virtual void setForegroundColor(const CVector4f& color);
    virtual void setBackgroundColor(const CVector4f& color);
    virtual void setMargin(int margin); 
    virtual void setText(const std::string& text);

  protected:
    void blit(FT_Bitmap* bm, int x, int y);

  private:
    std::string m_text;
    FT_Face m_face;
    CVector4f m_fgColor;
    CVector4f m_bgColor;
    int m_margin;
  };

  /**
   * Pojemnik na dane opisujace material obiektu
   */
  class CGenericMaterial : public CAbstractMaterial {
  public:
	CGenericMaterial(const std::string& name = "Unnamed");
	virtual ~CGenericMaterial();

    void bind() const;
  };

  /**
   * Klasa reprezentujaca siatke modelu
   */
  class CGenericMesh : public CAbstractMesh {
  public:
	CGenericMesh(
        const unsigned int& vertexCount,
        const std::vector<CVector3f>& vertex,
        const std::vector<CVector3f>& normal,
        const std::vector<CVector2f>& coord,
        const std::string& name = "Unnamed"
    );
	virtual ~CGenericMesh();

    void bind() const;

  private:
    // 0=vertex, 1=normal, 2=coord
    unsigned int m_oglHandle[3];
  };

  /**
   * OpenGL program. Vertex i Fragment program.
   */
  class CGenericProgram : public CAbstractProgram {
  public:
	CGenericProgram(
        const std::string& srcVertexProgram,
        const std::string& srcFragentProgram,
        const std::string& name = "Unnamed"
    );
	virtual ~CGenericProgram();

    virtual bool setUniform1i(const std::string& name, int value) const;
    virtual bool setUniform1f(const std::string& name, float value) const;
    virtual bool setUniform2f(const std::string& name, const CVector2f& v2) const;
    virtual bool setUniform3f(const std::string& name, const CVector3f& v3) const;
    virtual bool setUniform4f(const std::string& name, const CVector4f& v4) const;

    void bind() const;

  protected:
    std::string getShaderInfoLog(unsigned int handle) const;
    std::string getProgramInfoLog() const;

  private:
    unsigned int m_oglHandle[3];
  };

  class CFrameBuffer : public CAbstractFrameBuffer {
  public:
	CFrameBuffer(int width, int height, const std::string& name="unnamed");
	virtual ~CFrameBuffer();

    bool getEnabled() const;
    void setEnabled(bool enabled);

    void clear();
    void bind() const;

  private:
    bool m_enabled;
    // 0-fbo, 1-depht, 2-texture
    unsigned int m_oglHandle[3];
  };

  /**
   * Klasa zarzadzajaca zasobami karty graficznej
   */
  class CResourceFactory: public IResourceFactory {
  public:
	virtual ~CResourceFactory();

	static CResourceFactory& instance();

	std::vector< smart_ptr<CAbstractMesh> > buildMesh(const std::string& path);
	std::vector< smart_ptr<CAbstractMaterial> > buildMaterial(const std::string& path);
	smart_ptr<CAbstractTexture> buildTexture(const std::string& path, int texUnit = 0);
	smart_ptr<CAbstractProgram> buildProgram(const std::string& path);

	smart_ptr<CAbstractMesh> genMesh_Quad(float width, float height);
	smart_ptr<CAbstractMesh> genMesh_Plane(int w, int h);
	smart_ptr<CAbstractTexture> genTexture_Grid(int w, int h, int texUnit);

	smart_ptr<CTextTexture> createTextTexture(unsigned int width, unsigned int height,
        const std::string& font, const std::string& text, unsigned int pt=12);

  protected:
	ILogger &m_log;
    FT_Library m_freetype;

	std::vector< smart_ptr<CAbstractMesh> > fileLoad_OBJ(const std::string& fileName);
	std::vector< smart_ptr<CAbstractMaterial> > fileLoad_MAT(const std::string& fileName);
	std::vector< smart_ptr<CAbstractMaterial> > fileLoad_MTL(const std::string& fileName);
	smart_ptr<CAbstractTexture> fileLoad_TGA(const std::string& fileName, int texUnit);
	smart_ptr<CAbstractProgram> fileLoad_GLSL(const std::string& path);

	std::map< std::string, smart_ptr<CAbstractResource> > m_cache;
	bool addToCache(smart_ptr<CAbstractResource> ptr);
	smart_ptr<CAbstractResource> getFromCache(const std::string& key);

  private:
	CResourceFactory();
	CResourceFactory(const CResourceFactory&);
	CResourceFactory& operator=(const CResourceFactory&);
  };
}

#endif //_RESOURCE_HEADER_
