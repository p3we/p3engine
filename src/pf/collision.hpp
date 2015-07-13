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

#ifndef _COLLISION_HEADER_
#define _COLLISION_HEADER_

#include <cmath>
#include "../gf/common.hpp"
#include "../gf/logger.hpp"
#include "icollision.hpp"

namespace pf
{
  class CCollisionPoint : public CAbstractPoint {
  public:
	CCollisionPoint(ICollisionManager* cmgr, const gf::CVector4f& p);
	virtual ~CCollisionPoint();

	bool move(gf::CVector4f& v);

  protected:
	ICollisionManager* m_cmgr;
  };

  class CCollisionWall : public CAbstractWall {
  public:
	CCollisionWall();
	CCollisionWall(
		ICollisionManager* cmgr, const gf::CVector4f& p,
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
	ICollisionManager* m_cmgr;

  private:
	gf::CVector4f m_front;
	gf::CVector4f m_up;
	gf::CVector2f m_size;
  };

  class CCollisionBox : public CAbstractBox {
  public:
	CCollisionBox(
		ICollisionManager* cmgr, const gf::CVector4f& p, const gf::CVector4f& s
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

	const CAbstractWall& getWall(int i) const;

  protected:
	ICollisionManager* m_cmgr;
    mutable bool m_eval;
    void eval() const;

  private:
	gf::CVector4f m_front;
	gf::CVector4f m_up;
	gf::CVector4f m_size;
	mutable CCollisionWall m_wall[6];
  };

  class CCollisionManager : public ICollisionManager {
  public:
	CCollisionManager();
	virtual ~CCollisionManager();

	CAbstractPoint* addPoint();
	CAbstractWall* addWall(const gf::CVector4f& n, const gf::CVector2f& s);
	CAbstractBox* addBox(const gf::CVector4f& s);
	void delPoint(CAbstractPoint* point);
	void delWall(CAbstractWall* wall);
	void delBox(CAbstractBox* box);

	bool move(const CAbstractPoint* obj, gf::CVector4f& v);
	int select(const gf::CVector4f& p, const gf::CVector4f& v);

  protected:
	bool checkWall(const CAbstractWall* obj, const gf::CVector4f& p1, const gf::CVector4f& vec);
	bool checkBox(const CAbstractBox* obj, const gf::CVector4f& p1, gf::CVector4f& vec);

  private:
	gf::ILogger& m_log;
	std::vector<CAbstractPoint*> m_point;
	std::vector<CAbstractWall*> m_wall;
	std::vector<CAbstractBox*> m_box;
  };
}

#endif // _COLLISION_HEADER_
