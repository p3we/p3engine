/* File: icollision.hpp
 * Date: 12.09-.010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#ifndef _ICOLLISION_HEADER_
#define _ICOLLISION_HEADER_

#include "../gf/common.hpp"

namespace pf
{
  class cAbstractPrimitive {
  public:
    cAbstractPrimitive();
    virtual ~cAbstractPrimitive();

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

  class cAbstractPoint : public cAbstractPrimitive {
  public:
    virtual ~cAbstractPoint(){};

    virtual bool move(gf::CVector4f& v) = 0;
  };

  class cAbstractWall : public cAbstractPrimitive {
  public:
    virtual ~cAbstractWall(){};

    virtual const gf::CVector4f& getFront() const = 0;
    virtual const gf::CVector4f& getUp() const = 0;
    virtual const gf::CVector2f& getSize() const = 0;

    virtual void setFront(const gf::CVector4f& f) = 0;
    virtual void setUp(const gf::CVector4f& u) = 0;
    virtual void setSize(const gf::CVector2f& s) = 0;
  };

  class cAbstractBox : public cAbstractPrimitive {
  public:
    virtual ~cAbstractBox(){};

    virtual const gf::CVector4f& getFront() const = 0;
    virtual const gf::CVector4f& getUp() const = 0;
    virtual const gf::CVector4f& getSize() const = 0;

    virtual void setFront(const gf::CVector4f& f) = 0;
    virtual void setUp(const gf::CVector4f& u) = 0;
    virtual void setSize(const gf::CVector4f& s) = 0;

    virtual const cAbstractWall& getWall(int i) const = 0;
  };

  class iCollisionManager {
  public:
    virtual ~iCollisionManager(){};

    virtual cAbstractPoint* addPoint() = 0;
    virtual cAbstractWall* addWall(const gf::CVector4f& n, const gf::CVector2f& s) = 0;
    virtual cAbstractBox* addBox(const gf::CVector4f& s) = 0;
    virtual void delPoint(cAbstractPoint* point) = 0;
    virtual void delWall(cAbstractWall* wall) = 0;
    virtual void delBox(cAbstractBox* box) = 0;

    virtual bool move(const cAbstractPoint* obj, gf::CVector4f& v) = 0;

    virtual int select(const gf::CVector4f& p, const gf::CVector4f& v) = 0;
  };
}

#endif //_ICOLLISION_HEADER_
