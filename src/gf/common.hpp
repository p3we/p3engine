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

#ifndef _COMMON_HEADER_
#define _COMMON_HEADER_

#include <cmath>
#include <cstdio>

namespace gf
{
  /**
   * Klasa reprezentujaca matematyczny wektor 2 elementowy.
   * Wykorzystywany do reprezentacji pozycji w przestrzeni 2D
   */
  template<class T>
  class CVector2 {
  public:
	CVector2(){
      m_data[0] = m_data[1] = 0;
    }
	CVector2(const T& x, const T& y){
      m_data[0] = x;
      m_data[1] = y;
    }

    inline T& x(){ return m_data[0];}
    inline const T& x() const { return m_data[0];}
    inline T& y(){ return m_data[1];}
    inline const T& y() const { return m_data[1];}
    inline T& u(){ return m_data[0];}
    inline const T& u() const { return m_data[0];}
    inline T& v(){ return m_data[1];}
    inline const T& v() const { return m_data[1];}
    inline T& w(){ return m_data[0];}
    inline const T& w() const { return m_data[0];}
    inline T& h(){ return m_data[1];}
    inline const T& h() const { return m_data[1];}

    inline T* raw(){ return m_data;}
    inline const T* raw() const { return m_data;}

    inline T& operator[](int i){ return m_data[i];}
    inline const T& operator[](int i) const { return m_data[i];}

  protected:
    T m_data[2];
  };

  class CVector2i : public CVector2<int> {
  public:
	CVector2i();
	CVector2i(const int& x, const int& y);
  };

  class CVector2f : public CVector2<float> {
  public:
	CVector2f();
	CVector2f(const float& x, const float& y);
  };

  /**
   * Klasa wektora 3 elementowego
   * Wykorzstywany do przechowywania pozycji w przestrzeni 3D i koloru
   */
  template<class T>
  class CVector3 {
  public:
	CVector3(){
      m_data[0] = m_data[1] = m_data[2] = 0;
    }
	CVector3(const T& x, const T& y, const T& z){
      m_data[0] = x;
      m_data[1] = y;
      m_data[2] = z;
    }

    inline T& x(){ return m_data[0];}
    inline const T& x() const { return m_data[0];}
    inline T& y(){ return m_data[1];}
    inline const T& y() const { return m_data[1];}
    inline T& z() { return m_data[2];}
    inline const T& z() const { return m_data[2];}

    inline T& r(){ return m_data[0];}
    inline const T& r() const { return m_data[0];}
    inline T& g(){ return m_data[1];}
    inline const T& g() const { return m_data[1];}
    inline T& b() { return m_data[2];}
    inline const T& b() const { return m_data[2];}

    inline T* raw(){ return m_data;}
    inline const T* raw() const { return m_data;}
    inline T& operator[](int i){ return m_data[i];}
    inline const T& operator[](int i) const { return m_data[i];}

  protected:
    T m_data[3];
  };

  class CVector3i : public CVector3<int> {
  public:
	CVector3i();
	CVector3i(const int& x, const int& y, const int& z);
  };

  class CVector3f : public CVector3<float> {
  public:
	CVector3f();
	CVector3f(const float& x, const float& y, const float& z);
  };

  /**
   * Klasa reprezentujaca matematyczny czteroelementowy wektor
   */
  class CVector4f {
  public:
	CVector4f();
	CVector4f(float x, float y, float z, float w=1.0);

    // Dostep do skladowych wektora
    float& x();
    const float& x() const;
    float& y();
    const float& y() const;
    float& z();
    const float& z() const;
    float& w();
    const float& w() const;
    const float* raw() const;
    float& operator[](const unsigned int& i);

    // Kolekcja operacji arytmetycznych i logicznych wektora
	CVector4f operator-() const;

	bool operator==(const CVector4f& v) const;
	bool operator!=(const CVector4f& v) const;

	CVector4f operator+(const CVector4f& v) const;
	CVector4f operator-(const CVector4f& v) const;
	CVector4f operator*(const float& s) const;
	CVector4f operator/(const float& s) const;

	CVector4f& operator+=(const CVector4f& v);
	CVector4f& operator-=(const CVector4f& v);
	CVector4f& operator*=(const float& s);
	CVector4f& operator/=(const float& s);

    // Funkcje operujace na wektorach
	static float mod(const CVector4f& v);
	static CVector4f norm(const CVector4f& v);
	static float angle(const CVector4f& v1, const CVector4f& v2);
	static float dot(const CVector4f& v1, const CVector4f& v2);
	static CVector4f cross(const CVector4f& v1, const CVector4f& v2);

    // Debug
    void dump() const;

  protected:
    float m_data[4];
  };

  /**
   * Klasa reprezentujaca matematyczna macierz 4x4
   */
  class CMatrix4f {
  public:
	CMatrix4f();
	CMatrix4f(const float* src);
	CMatrix4f(const CVector4f& vec);
	CMatrix4f(float angle, const CVector4f& vec);

    // Dostep do elementow macierzy
    float& at(const int& i, const int& j);
    const float& at(const int& i, const int& j) const;
    float& at(const int& i);
    const float& at(const int& i) const;
    float& operator[](const int& i);
    const float* raw() const;

    // Kolekcja operatowow matemtycznych i logicznych
	bool operator==(const CMatrix4f& m) const;
	bool operator!=(const CMatrix4f& m) const;

	CMatrix4f operator+(const CMatrix4f& m) const;
	CMatrix4f operator-(const CMatrix4f& m) const;
	CMatrix4f operator*(const float& s) const;
	CVector4f operator*(const CVector4f& v) const;
	CMatrix4f operator*(const CMatrix4f& m) const;

	CMatrix4f& operator+=(const CMatrix4f& m);
	CMatrix4f& operator-=(const CMatrix4f& m);
	CMatrix4f& operator*=(const float& s);
	CMatrix4f& operator*=(const CMatrix4f& m);


    // Funkcje operujace na macierzy
	static CMatrix4f transpose(const CMatrix4f& m);

    // Debug
    void dump() const;

  private:
    float m_data[16];
  };

  /**
   * Implementacja wskaznika zliczajacego referencje.
   * Po usunieciu ostatniego odwolania do obiektu zostaje on usuniety.
   */
  template<class T>
  class smart_ptr {
  public:
    explicit smart_ptr(){
      m_entity = 0;
      m_counter = new unsigned int(1);
    }

    explicit smart_ptr(T* ptr, unsigned int counter=1){
      m_entity = ptr;
      m_counter = new unsigned int(counter);
    }

    smart_ptr(const smart_ptr<T>& arg){
      acquire(arg.m_entity, arg.m_counter);
    }

    ~smart_ptr(){
      if(--(*m_counter) <= 0) dispose();
    }

    smart_ptr<T>& operator=(const smart_ptr<T>& arg){
      if(this != &arg){
        if(--(*m_counter) <= 0) dispose();
        acquire(arg.m_entity, arg.m_counter);
      }
      return *this;
    }

    T& operator*(){
      return *m_entity;
    }

    const T& operator*() const {
      return *m_entity;
    }

    T* operator->(){
      return m_entity;
    }

    const T* operator->() const {
      return m_entity;
    }

    bool operator==(const T& arg) const {
      return (m_entity == m_entity);
    }

    bool operator!=(const T& arg) const {
      return (m_entity != m_entity);
    }

    bool operator!(){
      return (m_entity == 0);
    }

    operator bool() const {
      return (m_entity != 0);
    }

    template<class Base> smart_ptr<Base> up_cast(){
      smart_ptr<Base> ptr;
      ptr.dispose();
      ptr.acquire(m_entity, m_counter);
      return ptr;
    }

    template<class Derive> smart_ptr<Derive> down_cast(){
      smart_ptr<Derive> ptr;
      Derive* entity = dynamic_cast<Derive*>(m_entity);
      if(entity){
        ptr.dispose();
        ptr.acquire(entity, m_counter);
      }
      return ptr;
    }

    T* entity(){
      return m_entity;
    }

    unsigned int counter() const {
      return *m_counter;
    }

    void acquire(T* e, unsigned int* c){
      m_entity = e;
      m_counter = c;
      ++(*m_counter);
    }

    void dispose(){
      delete m_counter;
      delete m_entity;
      m_counter = 0;
      m_entity = 0;
    }

  private:
    T* m_entity;
    unsigned int* m_counter;
  };
}

#endif //_COMMON_HEADER_
