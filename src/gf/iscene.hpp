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

#ifndef _ISCENE_HEADER_
#define _ISCENE_HEADER_

#include <vector>
#include "common.hpp"
#include "resource.hpp"

namespace gf
{
  /**
   * Abstrakcyjna klasa wezla sceny.
   * Implementuje funkcje skladowe zwiazane z pozycja na scenie i jej struktura.
   */
  class CAbstractNode {
  public:
	CAbstractNode();
	virtual ~CAbstractNode();

    enum eRenderMethod {
      RENDER_WIRED=0,RENDER_SOLID=1,
      RENDER_SHADED=2, RENDER_TEXTURED=3
    };

    // Struktura sceny
	virtual const CAbstractNode* getParent() const;
	virtual void setParent(CAbstractNode* parent);
	virtual bool appendChild(CAbstractNode* child);
	virtual bool removeChild(CAbstractNode* child);
	virtual const std::vector<CAbstractNode*>& getChildren() const;
    virtual void clearChildren();

    // Podmiot renderowania
	virtual smart_ptr<CAbstractMesh> getMesh() = 0;
	virtual smart_ptr<CAbstractMaterial> getMaterial() = 0;
	virtual smart_ptr<CAbstractProgram> getProgram() = 0;
	virtual void setMesh(smart_ptr<CAbstractMesh> mesh) = 0;
	virtual void setMaterial(smart_ptr<CAbstractMaterial> material) = 0;
	virtual void setProgram(smart_ptr<CAbstractProgram> program) = 0;

    // Pozycja na scenie
    virtual const CVector4f& getPosition() const;
    virtual const CVector4f& getScale() const;
    virtual const CMatrix4f& getModelMatrix() const;
    virtual const float& getRotationAngle() const;
    virtual const CVector4f& getRotationVector() const;
    virtual const CVector4f& getRotationPoint() const;
    virtual void setPosition(const CVector4f& vect);
    virtual void setRotation(const float& angle, const CVector4f& vect,
        const CVector4f& point = CVector4f(0,0,0));
    virtual void setScale(const CVector4f& v);
    virtual void setModelMatrix(const CMatrix4f& m) const;

    // Renderowanie wezla
    virtual const eRenderMethod& getRenderMethod() const = 0;
    virtual void setRenderMethod(const eRenderMethod& rm) = 0;
    virtual void render() const = 0;

  protected:
	ILogger& m_log;

  private:
    // Skojarzone wezly
	CAbstractNode* m_parent;
	std::vector<CAbstractNode*> m_children;

    // Macierz modelu
    void matrixLazyEval() const;      /**< Pozna ewaulacja macierzy */
    mutable bool m_modelMatrixValid;  /**< Flaga poprawnosci macierzy */
    mutable CMatrix4f m_modelMatrix;  /**< Macierz modelu */

    // Dane o pozycji w przestrzenis
    CVector4f m_position;      /**< Aktualna pozycja obiektu */
    CVector4f m_scale;         /**< Skala */
    float m_rotationAngle;     /**< Kat obrotu w stopniach */
    CVector4f m_rotationVect;  /**< Wektor obrotu */
    CVector4f m_rotationPoint; /**< Punkt wzgledem ktorego wykonywany jest obrot */
  };


  /**
   * Abstrakcyjne zrodlo swiatla
   */
  class CAbstractLight {
  public:
	CAbstractLight();
	virtual ~CAbstractLight();

    virtual const CVector4f& getPosition();
    virtual const CVector4f& getAmbient();
    virtual const CVector4f& getDiffuse();
    virtual const CVector4f& getSpecular();

    virtual void setPosition(const CVector4f& pos);
    virtual void setAmbient(const CVector4f& ambient);
    virtual void setDiffuse(const CVector4f& diffuse);
    virtual void setSpecular(const CVector4f& specular);

  private:
    CVector4f m_position;
    CVector4f m_ambient;
    CVector4f m_diffuse;
    CVector4f m_specular;
  };


  /**
   * Abstrakcyjna kamera
   */
  class CAbstractCamera {
  public:
	CAbstractCamera();
	virtual ~CAbstractCamera();

    virtual const CVector4f& getPosition() const;
    virtual const CVector4f& getLookAt() const;
    virtual const CVector4f& getUp() const;
    virtual const CMatrix4f& getViewMatrix() const;
    virtual const CMatrix4f& getProjMatrix() const;

    virtual void setPosition(const CVector4f& pos);
    virtual void setLookAt(const CVector4f& at);
    virtual void setUp(const CVector4f& up);
    virtual void setViewMatrix(const CMatrix4f& m);
    virtual void setProjMatrix(const CMatrix4f& m);

  protected:
    virtual void viewLazyEval() const = 0;
    virtual void projLazyEval() const = 0;

    mutable bool m_viewValid;
    mutable bool m_projValid;

    mutable CMatrix4f m_viewMatrix;
    mutable CMatrix4f m_projMatrix;

  private:
    // Orientacja kamery w globalnym ukladzie wspolrzednych
    CVector4f m_camPosition;
    CVector4f m_camLookAt;
    CVector4f m_camUp;
  };

  /**
   *
   */
  class ISceneManager {
  public:
	virtual ~ISceneManager(){};

    virtual bool getEnabled() const = 0;
    virtual void setEnabled(bool enabled) = 0;

    // Kamera
	virtual CAbstractCamera* addPerspectiveCamera(float fov) = 0;
	virtual CAbstractCamera* addOrthoCamera(float width, float height) = 0;
	virtual CAbstractCamera* add2DCamera() = 0;
	virtual CAbstractCamera* addFPSCamera(float fov = 60.0f) = 0;
	virtual const CAbstractCamera* getActiveCamera() const = 0;
	virtual void setActiveCamera(const CAbstractCamera* camera) = 0;

    // Swiatlo
	virtual CAbstractLight* addDirLight() = 0;
	virtual CAbstractLight* addPointLight() = 0;
    virtual const CVector4f& getGlobalAmbient() const = 0;
    virtual void setGlobalAmbient(const CVector4f& globamAmbient) = 0;

    // Akcja ;]
	virtual CAbstractNode* addSceneNode(const std::string& obj, const std::string& mat,
      const std::string& prog) = 0;
	virtual CAbstractNode* addSceneNode(smart_ptr<CAbstractMesh> mesh,
	  smart_ptr<CAbstractMaterial> mat, smart_ptr<CAbstractProgram> prog) = 0;
	virtual CAbstractNode* addSceneNode(smart_ptr<CAbstractTexture> tex,
		smart_ptr<CAbstractProgram> prog) = 0;
	virtual void delSceneNode(const CAbstractNode* node) = 0;

    // Renderuj scene
    virtual void render() const = 0;
  };
}

#endif //_ISCENE_HEADER_
