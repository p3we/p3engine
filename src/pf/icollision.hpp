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

#ifndef _ICOLLISION_HEADER_
#define _ICOLLISION_HEADER_

#include "../gf/common.hpp"

namespace pf
{
  class CAbstractPrimitive {
  public:
	CAbstractPrimitive();
	virtual ~CAbstractPrimitive();

    virtual int getId() const;
    virtual const gf::CVector4f& getPosition() const;

    virtual void setId(int id);
    virtual void setPosition(const gf::CVector4f& p);

  protected:
    static int m_counter;

  private:
    int m_id;
    gf::CVector4f m_position;
  };

  class CAbstractPoint : public CAbstractPrimitive {
  public:
	virtual ~CAbstractPoint(){}

    virtual bool move(gf::CVector4f& v) = 0;
  };

  class CAbstractWall : public CAbstractPrimitive {
  public:
	virtual ~CAbstractWall(){}

    virtual const gf::CVector4f& getFront() const = 0;
    virtual const gf::CVector4f& getUp() const = 0;
    virtual const gf::CVector2f& getSize() const = 0;

    virtual void setFront(const gf::CVector4f& f) = 0;
    virtual void setUp(const gf::CVector4f& u) = 0;
    virtual void setSize(const gf::CVector2f& s) = 0;
  };

  class CAbstractBox : public CAbstractPrimitive {
  public:
	virtual ~CAbstractBox(){}

    virtual const gf::CVector4f& getFront() const = 0;
    virtual const gf::CVector4f& getUp() const = 0;
    virtual const gf::CVector4f& getSize() const = 0;

    virtual void setFront(const gf::CVector4f& f) = 0;
    virtual void setUp(const gf::CVector4f& u) = 0;
    virtual void setSize(const gf::CVector4f& s) = 0;

	virtual const CAbstractWall& getWall(int i) const = 0;
  };

  class ICollisionManager {
  public:
	virtual ~ICollisionManager(){}

	virtual CAbstractPoint* addPoint() = 0;
	virtual CAbstractWall* addWall(const gf::CVector4f& n, const gf::CVector2f& s) = 0;
	virtual CAbstractBox* addBox(const gf::CVector4f& s) = 0;
	virtual void delPoint(CAbstractPoint* point) = 0;
	virtual void delWall(CAbstractWall* wall) = 0;
	virtual void delBox(CAbstractBox* box) = 0;

	virtual bool move(const CAbstractPoint* obj, gf::CVector4f& v) = 0;

    virtual int select(const gf::CVector4f& p, const gf::CVector4f& v) = 0;
  };
}

#endif //_ICOLLISION_HEADER_
