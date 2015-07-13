/* File: collision.hpp
 * Date: 12.09-.010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#ifndef _COLLISION_HEADER_
#define _COLLISION_HEADER_

#include <cmath>
#include "../gf/common.hpp"
#include "../gf/logger.hpp"
#include "icollision.hpp"

namespace pf
{
  class CCollisionPoint : public cAbstractPoint {
  public:
	CCollisionPoint(iCollisionManager* cmgr, const gf::CVector4f& p);
	virtual ~CCollisionPoint();

	bool move(gf::CVector4f& v);

  protected:
    iCollisionManager* m_cmgr;
  };

  class CCollisionWall : public cAbstractWall {
  public:
	CCollisionWall();
	CCollisionWall(
		iCollisionManager* cmgr, const gf::CVector4f& p,
		const gf::CVector4f& n, const gf::CVector2f& s
    );
	virtual ~CCollisionWall();

	const gf::CVector4f& getFront() const;
	const gf::CVector4f& getUp() const;
	const gf::CVector2f& getSize() const;

	void setFront(const gf::CVector4f& f);
	void setUp(const gf::CVector4f& u);
	void setSize(const gf::CVector2f& s);

  protected:
    iCollisionManager* m_cmgr;

  private:
	gf::CVector4f m_front;
	gf::CVector4f m_up;
	gf::CVector2f m_size;
  };

  class CCollisionBox : public cAbstractBox {
  public:
	CCollisionBox(
		iCollisionManager* cmgr, const gf::CVector4f& p, const gf::CVector4f& s
    );
	virtual ~CCollisionBox();

	const gf::CVector4f& getPosition() const;
	const gf::CVector4f& getFront() const;
	const gf::CVector4f& getUp() const;
	const gf::CVector4f& getSize() const;

	void setPosition(const gf::CVector4f& p);
	void setFront(const gf::CVector4f& f);
	void setUp(const gf::CVector4f& u);
	void setSize(const gf::CVector4f& s);

    const cAbstractWall& getWall(int i) const;

  protected:
    iCollisionManager* m_cmgr;
    mutable bool m_eval;
    void eval() const;

  private:
	gf::CVector4f m_front;
	gf::CVector4f m_up;
	gf::CVector4f m_size;
	mutable CCollisionWall m_wall[6];
  };

  class CCollisionManager : public iCollisionManager {
  public:
	CCollisionManager();
	virtual ~CCollisionManager();

    cAbstractPoint* addPoint();
	cAbstractWall* addWall(const gf::CVector4f& n, const gf::CVector2f& s);
	cAbstractBox* addBox(const gf::CVector4f& s);
    void delPoint(cAbstractPoint* point);
    void delWall(cAbstractWall* wall);
    void delBox(cAbstractBox* box);

	bool move(const cAbstractPoint* obj, gf::CVector4f& v);
	int select(const gf::CVector4f& p, const gf::CVector4f& v);

  protected:
	bool checkWall(const cAbstractWall* obj, const gf::CVector4f& p1, const gf::CVector4f& vec);
	bool checkBox(const cAbstractBox* obj, const gf::CVector4f& p1, gf::CVector4f& vec);

  private:
    gf::iLogger& m_log;
    std::vector<cAbstractPoint*> m_point;
    std::vector<cAbstractWall*> m_wall;
    std::vector<cAbstractBox*> m_box;
  };
}

#endif // _COLLISION_HEADER_
