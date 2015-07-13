/* File: common.cpp
 * Date: 15.08.2010
 * (c) Artur Sobierak <asobierak@gmail.com>
 */

#include "common.hpp"

using namespace gf;
using namespace std;

/* ----------------------------------------------------------------------------
 * cVector2i
 */
CVector2i::CVector2i() : CVector2<int>(0, 0) {
  // pass
}

CVector2i::CVector2i(const int& x, const int& y)
  : CVector2<int>(x,y)
{
  // pass
}
// <--

/* ----------------------------------------------------------------------------
 * cVector2f
 */
CVector2f::CVector2f() : CVector2<float>(0.0, 0.0) {
  // pass
}

CVector2f::CVector2f(const float& x, const float& y)
  : CVector2<float>(x,y)
{
  // pass
}
// <--

/* ----------------------------------------------------------------------------
 * cVector3i
 */
CVector3i::CVector3i() : CVector3<int>()
{
  // pass
}

CVector3i::CVector3i(const int& x, const int& y, const int& z)
  : CVector3<int>(x,y,z)
{
  // pass
}
// <--

/* ----------------------------------------------------------------------------
 * cVector3f
 */
CVector3f::CVector3f() : CVector3<float>()
{
  // pass
}

CVector3f::CVector3f(const float& x, const float& y, const float& z)
  : CVector3<float>(x,y,z)
{
  // pass
}
// <--

/* ----------------------------------------------------------------------------
 * cVector4f
 */
CVector4f::CVector4f(){
  m_data[0] = m_data[1] = m_data[2] = 0.0;
  m_data[3] = 1.0;
}

CVector4f::CVector4f(float x, float y, float z, float w){
  m_data[0] = x;
  m_data[1] = y;
  m_data[2] = z;
  m_data[3] = w;
}

// ----------------------------------------------------------------------------

float& CVector4f::x(){
  return m_data[0];
}

const float& CVector4f::x() const {
  return m_data[0];
}

float& CVector4f::y(){
  return m_data[1];
}

const float& CVector4f::y() const {
  return m_data[1];
}

float& CVector4f::z(){
  return m_data[2];
}

const float& CVector4f::z() const {
  return m_data[2];
}

float& CVector4f::w(){
  return m_data[3];
}

const float& CVector4f::w() const {
  return m_data[3];
}

const float* CVector4f::raw() const {
  return m_data;
}

float& CVector4f::operator[](const unsigned int& i){
  return m_data[i];
}

// ----------------------------------------------------------------------------

CVector4f CVector4f::operator-() const {
  return CVector4f(-x(), -y(), -z(), w());
}

bool CVector4f::operator==(const CVector4f& v) const {
  return (x()==v.x() && y()==v.y() && z()==v.z());
}

bool CVector4f::operator!=(const CVector4f& v) const {
  return (x()!=v.x() || y()!=v.y() || z()!=v.z());
}

CVector4f CVector4f::operator+(const CVector4f& v) const {
  return CVector4f(x()+v.x(), y()+v.y(), z()+v.z());
}

CVector4f CVector4f::operator-(const CVector4f& v) const {
  return CVector4f(x()-v.x(), y()-v.y(), z()-v.z());
}

CVector4f CVector4f::operator*(const float& s) const {
  return CVector4f(x()*s, y()*s, z()*s);
}

CVector4f CVector4f::operator/(const float& s) const{
  return CVector4f(x()/s, y()/s, z()/s);
}

CVector4f& CVector4f::operator+=(const CVector4f& v){
  x() += v.x();
  y() += v.y();
  z() += v.z();
  return *this;
}

CVector4f& CVector4f::operator-=(const CVector4f& v){
  x() -= v.x();
  y() -= v.y();
  z() -= v.z();
  return *this;
}

CVector4f& CVector4f::operator*=(const float& s){
  x() *= s;
  y() *= s;
  z() *= s;
  return *this;
}

CVector4f& CVector4f::operator/=(const float& s){
  x() /= s;
  y() /= s;
  z() /= s;
  return *this;

}

// ----------------------------------------------------------------------------

float CVector4f::mod(const CVector4f& v){
  return sqrt(pow(v.x(),2) + pow(v.y(),2) + pow(v.z(),2));
}

CVector4f CVector4f::norm(const CVector4f& v){
  float d = CVector4f::mod(v);
  return CVector4f(v.x()/d, v.y()/d, v.z()/d, v.w());
}

float CVector4f::angle(const CVector4f& v1, const CVector4f& v2){
  CVector4f m = CVector4f::norm(v1);
  CVector4f n = CVector4f::norm(v2);
  return acos(CVector4f::dot(m,n));
}

float CVector4f::dot(const CVector4f& v1, const CVector4f& v2){
  return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

CVector4f CVector4f::cross(const CVector4f& v1, const CVector4f& v2){
  return CVector4f(
      v1.y()*v2.z() - v1.z()*v2.y(),
      v1.z()*v2.x() - v1.x()*v2.z(),
      v1.x()*v2.y() - v1.y()*v2.x()
  );
}

// ----------------------------------------------------------------------------

void CVector4f::dump() const {
  printf("cVector4d(): x(%g) y(%g) z(%g) w(%g)\n", x(), y(), z(), w());
}
// <--

/* ----------------------------------------------------------------------------
 * cMatrix4f
 */
CMatrix4f::CMatrix4f(){
  for(unsigned int i=0; i<16; ++i) m_data[i] = 0.0;
  at(0,0) = at(1,1) = at(2,2) = at(3,3) = 1.0;
}

CMatrix4f::CMatrix4f(const float* src){
  for(unsigned int i=0; i<16; ++i){
    m_data[i] = src[i];
  }
}

CMatrix4f::CMatrix4f(const CVector4f& vec){
  for(unsigned int i=0; i<16; ++i) m_data[i] = 0.0;
  at(0,0) = at(1,1) = at(2,2) = at(3,3) = 1.0;

  at(3,0) = vec.x();
  at(3,1) = vec.y();
  at(3,2) = vec.z();
}

CMatrix4f::CMatrix4f(float angle, const CVector4f& vec){
  for(unsigned int i=0; i<16; ++i) m_data[i] = 0.0;
  at(0,0) = at(1,1) = at(2,2) = at(3,3) = 1.0;

  // Tylko raz licz wartosc funkcji trygonometrycznych
  float radians = (angle/180.0f)*3.14f;
  float angleSin = sin(radians);
  float angleCos = cos(radians);

  at(0,0) = pow(vec.x(), 2)*(1 - angleCos) + angleCos;
  at(0,1) = vec.x()*vec.y()*(1 - angleCos) + vec.z()*angleSin;
  at(0,2) = vec.x()*vec.z()*(1 - angleCos) - vec.y()*angleSin;

  at(1,0) = vec.x()*vec.y()*(1 - angleCos) - vec.z()*angleSin;
  at(1,1) = pow(vec.y(), 2)*(1 - angleCos) + angleCos;
  at(1,2) = vec.y()*vec.z()*(1 - angleCos) + vec.x()*angleSin;

  at(2,0) = vec.x()*vec.z()*(1 - angleCos) + vec.y()*angleSin;
  at(2,1) = vec.y()*vec.z()*(1 - angleCos) - vec.x()*angleSin;
  at(2,2) = pow(vec.z(),2)*(1 - angleCos) + angleCos;
}

// ----------------------------------------------------------------------------

float& CMatrix4f::at(const int& i, const int& j){
  return m_data[i*4+j];
}

const float& CMatrix4f::at(const int& i, const int& j) const {
  return m_data[i*4+j];
}

float& CMatrix4f::at(const int& i){
  return m_data[i];
}

const float& CMatrix4f::at(const int& i) const {
  return m_data[i];
}

float& CMatrix4f::operator[](const int& i){
  return m_data[i];
}

const float* CMatrix4f::raw() const {
  return m_data;
}

// ----------------------------------------------------------------------------

bool CMatrix4f::operator==(const CMatrix4f& m) const {
  for(unsigned int i=0; i<16; ++i){
    if(at(i) != m.at(i)) return false;
  }
  return true;
}

bool CMatrix4f::operator!=(const CMatrix4f& m) const {
  for(unsigned int i=0; i<16; ++i){
    if(at(i) != m.at(i)) return true;
  }
  return false;
}

CMatrix4f CMatrix4f::operator+(const CMatrix4f& m) const {
  float tmp[16];
  for(unsigned int i=0; i<16; ++i){
    tmp[i] = at(i) + m.at(i);
  }
  return CMatrix4f(tmp);
}

CMatrix4f CMatrix4f::operator-(const CMatrix4f& m) const {
  float tmp[16];
  for(unsigned int i=0; i<16; ++i){
    tmp[i] = at(i) - m.at(i);
  }
  return CMatrix4f(tmp);
}

CMatrix4f CMatrix4f::operator*(const float& s) const {
  float tmp[16];
  for(unsigned int i=0; i<16; ++i){
    tmp[i] = at(i)*s;
  }
  return CMatrix4f(tmp);
}

CVector4f CMatrix4f::operator*(const CVector4f& v) const {
  CVector4f tmp(0,0,0,0);
  for(unsigned int i=0; i<4; ++i){
    tmp[i] = 0;
    for(unsigned int k=0; k<4; ++k){
      tmp[i] += at(k,i)*v.raw()[k];
    }
  }
  return tmp;
}

CMatrix4f CMatrix4f::operator*(const CMatrix4f& m) const {
  float tmp[16];
  for(unsigned int i=0; i<4; ++i){
    for(unsigned int j=0; j<4; ++j){
      tmp[i*4+j] = 0;
      for(unsigned int k=0; k<4; ++k){
        tmp[i*4+j] += at(k,j)*m.at(i,k);
      }
    }
  }
  return CMatrix4f(tmp);
}

CMatrix4f& CMatrix4f::operator+=(const CMatrix4f& m){
  for(unsigned int i=0; i<16; ++i){
    m_data[i] += m.at(i);
  }
  return *this;
}

CMatrix4f& CMatrix4f::operator-=(const CMatrix4f& m){
  for(unsigned int i=0; i<16; ++i){
    m_data[i] -= m.at(i);
  }
  return *this;
}

CMatrix4f& CMatrix4f::operator*=(const float& s){
  for(unsigned int i=0; i<16; ++i){
    m_data[i] *= s;
  }
  return *this;
}

CMatrix4f& CMatrix4f::operator*=(const CMatrix4f& m){
  float tmp[16];
  for(unsigned int i=0; i<4; ++i){
    for(unsigned int j=0; j<4; ++j){
      tmp[i*4+j] = 0;
      for(unsigned int k=0; k<4; ++k){
        tmp[i*4+j] += at(k,j)*m.at(i,k);
      }
    }
  }

  for(unsigned int i=0; i<16; ++i){
    m_data[i] = tmp[i];
  }

  return *this;
}

// ----------------------------------------------------------------------------

CMatrix4f CMatrix4f::transpose(const CMatrix4f& m){
  float tmp[16];
  for(unsigned int i=0; i<4; ++i){
    for(unsigned int j=0; j<4; ++j){
      tmp[i*4+j] = m.at(j,i);
    }
  }
  return CMatrix4f(tmp);
}

// ----------------------------------------------------------------------------

void CMatrix4f::dump() const {
  printf("cMatrix4d(\n");
  for(unsigned int i=0; i<4; ++i){
    for(unsigned int j=0; j<4; ++j){
      printf("%g ", at(j,i));
    }
    printf("\n");
  }
  printf(")\n");
}
// <--
