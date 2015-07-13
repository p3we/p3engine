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

#ifndef _SCENE_HEADER_
#define _SCENE_HEADER_

#include <cstdio>
#include <vector>
#include <cmath>
#include <SDL/SDL_video.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#include "device.hpp"
#include "common.hpp"
#include "resource.hpp"
#include "logger.hpp"
#include "iscene.hpp"

namespace gf
{
  /**
   * Podstawowy typ wezla sceny.
   * Implementuje funkcje potrzebne do wyrenderowania modelu na scenie.
   */
  class CSceneNode : public CAbstractNode{
  public:
	CSceneNode(
		smart_ptr<CAbstractMesh> mesh,
		smart_ptr<CAbstractMaterial> material,
		smart_ptr<CAbstractProgram> program
    );
	virtual ~CSceneNode();

    // Podmiot renderowania
	smart_ptr<CAbstractMesh> getMesh();
	smart_ptr<CAbstractMaterial> getMaterial();
	smart_ptr<CAbstractProgram> getProgram();
	void setMesh(smart_ptr<CAbstractMesh> mesh);
	void setMaterial(smart_ptr<CAbstractMaterial> material);
	void setProgram(smart_ptr<CAbstractProgram> program);

    const eRenderMethod& getRenderMethod() const;
    void setRenderMethod(const eRenderMethod& rm);
    void render() const;

  private:
    // Podmiot renderowania
	smart_ptr<CAbstractMesh> m_mesh;
	smart_ptr<CAbstractMaterial> m_material;
	smart_ptr<CAbstractProgram> m_program;
    eRenderMethod m_renderMethod;
  };

  /**
   * Swiatlo kierunkowe
   */
  class CDirLight : public CAbstractLight{
  public:
	CDirLight();
    void setPosition(const CVector4f& pos);
  };

  /**
   * Swiatlo punktowe
   */
  class CPointLight : public CAbstractLight{
  public:
	CPointLight();
    virtual const float& getConstAtt();
    virtual const float& getLineralAtt();
    virtual const float& getQuadraticAtt();
    virtual void setConstAtt(const float& value);
    virtual void setLinearAtt(const float& value);
    virtual void setQuadraticAtt(const float& value);

  private:
    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
  };

  /**
   * Kamera o rzucie perspektywicznym o dowolnej orientacji w przestrzeni 3D.
   */
  class CPerspectiveCamera : public CAbstractCamera {
  public:
	CPerspectiveCamera(
        const CVector4f& pos,                   /**< Pozycja kamery */
        const CVector4f& at,                    /**< Punkt na ktory skierowana jest kamera*/
        const CVector4f& up = CVector4f(0,1,0), /**< Wektor wskazujacy gore */
        float aspect = 1.3333,                 /**< Stosunek szerokosc/wysokosc */
        float fov = 60.0,                      /**< Field Of View */
        float far = 10000.0,                   /**< Daleka plaszczyzna obcinania */
        float near = 0.1                       /**< Bliska plaszczyzna obcinania */
    );
	virtual ~CPerspectiveCamera();

    virtual const float& getFar() const;
    virtual const float& getNear() const;
    virtual const float& getAcpect() const;
    virtual const float& getFOV() const;

    virtual void setFar(const float& far);
    virtual void setNear(const float& near);
    virtual void setAspect(const float& aspect);
    virtual void setFOV(const float& fov);

    virtual void moveCamera(const CVector4f& vect);
    virtual void rotateCamera(const float& angle, const CVector4f& vect=CVector4f(0,1,0));

  protected:
    // Evaluacja macierzy
    void viewLazyEval() const;
    void projLazyEval() const;

  private:
    // Parametry rzutowania
    float m_far;
    float m_near;
    float m_aspect;
    float m_fov;
  };

  /**
   * Kamera z widoku pierwszej osoby.
   * Sterowanie WSAD + myszka
   */
  class CFPSCamera : public CPerspectiveCamera {
  public:
	CFPSCamera(
        const CVector4f& pos,
        const CVector4f& at,
        float aspect = 1.3333,
        float fov = 60.0
    );

    virtual const float& getCameraSpeed() const;
    virtual const float& getMouseSpeed() const;
    virtual void setCameraSpeed(const float& speed);
    virtual void setMouseSpeed(const float& speed);

    void moveCamera(const CVector4f& vect);
	virtual void warp(IDevice& dev);

  private:
    // Parametry poruszania
    float m_cameraSpeed;
    float m_mouseSpeed;
  };

  /**
   * Kamera o rzucie ortogonalnym
   */
  class COrthoCamera : public CAbstractCamera {
  public:
	COrthoCamera(
        const CVector4f& pos,                   /**< Pozycja kamery */
        const CVector4f& at,                    /**< Punkt na ktory skierowana jest kamera*/
        const CVector4f& up = CVector4f(0,1,0), /**< Wektor wskazujacy gore */
        float width = 640.0f,                   /**< Szerokosc kadru */
        float height = 480.0f,                  /**< Wysokosc kadru */
        float far = 1000.0,                     /**< Daleka plaszczyzna obcinania */
        float near = 0.1                        /**< Bliska plaszczyzna obcinania */
    );
	virtual ~COrthoCamera();

    virtual const float& getWidth() const;
    virtual const float& getHeight() const;
    virtual const float& getFar() const;
    virtual const float& getNear() const;

    virtual void setWidth(const float& width);
    virtual void setHeight(const float& width);
    virtual void setFar(const float& width);
    virtual void setNear(const float& width);

  private:
    // Evaluacja macierzy
    void viewLazyEval() const;
    void projLazyEval() const;

    // Parametry rzutowania
    float m_width;
    float m_height;
    float m_far;
    float m_near;
  };

  /**
   * Kamera o rzucie prostokatnym w przestrzeni 2D.
   */
  class CFree2DCamera : public COrthoCamera{
  public:
	CFree2DCamera(
        const CVector2f& pos, /**< Pozycja s*/
        float width,          /**< Szerokosc kadru */
        float height,         /**< Wysokosc kadru */
        float far = 1000.0,   /**< Daleka plaszczyzna obcinania */
        float near = 0.1      /**< Bliska plaszczyzna obcinania */
    );
	virtual ~CFree2DCamera();

    virtual void moveCamera(const CVector2f& v);
    virtual void rotateCamera(const float& angle);
  };

  /**
   *
   */
  class CSceneManager : public ISceneManager {
  public:
	CSceneManager(IDevice& device);
	virtual ~CSceneManager();

    bool getEnabled() const ;
    void setEnabled(bool enabled);

	CAbstractCamera* addPerspectiveCamera(float fov);
	CAbstractCamera* addOrthoCamera(float width, float height);
	CAbstractCamera* add2DCamera();
	CAbstractCamera* addFPSCamera(float fov = 60.0f);
	const CAbstractCamera* getActiveCamera() const;
	void setActiveCamera(const CAbstractCamera* camera);

	CAbstractLight* addDirLight();
	CAbstractLight* addPointLight();
    const CVector4f& getGlobalAmbient() const;
    void setGlobalAmbient(const CVector4f& globamAmbient);

	CAbstractNode* addSceneNode(const std::string& obj, const std::string& mat,
        const std::string& prog);
	CAbstractNode* addSceneNode(smart_ptr<CAbstractMesh> mesh, smart_ptr<CAbstractMaterial> mat,
		smart_ptr<CAbstractProgram> prog);
	CAbstractNode* addSceneNode(smart_ptr<CAbstractTexture> tex, smart_ptr<CAbstractProgram> prog);
	void delSceneNode(const CAbstractNode* node);

    void render() const;

  protected:
    void postprocessing() const;

  private:
	CLogger& m_log;
	IDevice& m_dev;
    CResourceFactory& m_res;
    bool m_enabled;

    // Globalny ambient
    CVector4f m_globalAmbient;

    // Kontenery przechowujace obiekty sceny
	std::vector<CAbstractNode*> m_sceneNode;
	std::vector<CAbstractLight*> m_sceneLight;
	std::vector<CAbstractCamera*> m_sceneCamera;
	CAbstractCamera* m_activeCamera;
  };
}

#endif //_SCNE_HEADER_
