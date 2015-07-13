/* File: resource.cpp
 * Date: 02.06.2010
 */

#include "resource.hpp"

using namespace std;
using namespace gf;

/* ----------------------------------------------------------------------------
 * cAbstractResource
 */
CAbstractResource::CAbstractResource()
  : m_log(CLog::instance()), m_name("unnamed")
{
  m_state = STATE_UNKNOW;
  m_handle = 0;
}

CAbstractResource::~CAbstractResource(){
  // pass
}

const CAbstractResource::eState& CAbstractResource::getState() const {
  return m_state;
}

const unsigned int* CAbstractResource::getHandle() const {
  return m_handle;
}

const string& CAbstractResource::getName() const {
  return m_name;
}

void CAbstractResource::setState(const eState& state){
  m_state = state;
}

void CAbstractResource::setHandle(unsigned int* handle){
  m_handle = handle;
}

void CAbstractResource::setName(const string& name){
  m_name = name;
}

bool CAbstractResource::vaild() const {
  return (m_state == STATE_LOADED);
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractTexture
 */
CAbstractTexture::CAbstractTexture(){
  m_width = m_height = m_bpp = m_texUnit = 0;
}

CAbstractTexture::~CAbstractTexture(){}

unsigned int CAbstractTexture::getWidth() const {
  return m_width;
}

unsigned int CAbstractTexture::getHeight() const {
  return m_height;
}

unsigned int CAbstractTexture::getBpp() const {
  return m_bpp;
}

unsigned int CAbstractTexture::getTextureUnit() const {
  return m_texUnit;
}

void CAbstractTexture::setWidth(unsigned int width){
  m_width = width;
}

void CAbstractTexture::setHeight(unsigned int height){
  m_height = height;
}

void CAbstractTexture::setBpp(unsigned int bpp){
  m_bpp = bpp;
}

void CAbstractTexture::setTextureUnit(unsigned int texUnit){
  m_texUnit = texUnit;
}
// <--

/* ----------------------------------------------------------------------------
 * cGenericTexture
 */
CGenericTexture::CGenericTexture(unsigned int width, unsigned int height,
    unsigned int bpp, const vector<GLubyte>& data, const string& name,
    unsigned int texUnit)
{
  setState(STATE_ERROR);
  setHandle(m_oglHandle);
  setName(name);

  m_oglHandle[0] = m_oglHandle[1] = 0;
  m_buffer = 0;

  // Ladowanie danych do karty graficznej
  if(data.size() == width*height*(bpp/8))
  {
    setTextureUnit(texUnit);
    setWidth(width);
    setHeight(height);
    setBpp(bpp);

    glGetError();

    glGenBuffers(1, &m_oglHandle[0]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_oglHandle[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*(bpp/8), &data.front(), GL_STREAM_DRAW);

    activeTextureUnit(getTextureUnit());

    glGenTextures(1, &m_oglHandle[1]);
    glBindTexture(GL_TEXTURE_2D, m_oglHandle[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, getWidth(), getHeight(), 0, GL_BGRA,
        GL_UNSIGNED_BYTE, 0);

    if(glGetError() == GL_NO_ERROR){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      setState(STATE_LOADED);
    }
  }

  if(!vaild()){
    m_log.msg(CLogError() << "[cGenericTexture] Blad podczas ladowania tekstury");
  }
}

CGenericTexture::CGenericTexture(unsigned int width, unsigned int height,
    unsigned int bpp, const string& name, unsigned int texUnit)
{
  setState(STATE_ERROR);
  setHandle(m_oglHandle);
  setName(name);

  m_oglHandle[0] = m_oglHandle[1] = 0;
  m_buffer = 0;

  // Ladowanie danych do karty graficznej
  if(width!=0 && height!=0 && bpp!=0)
  {
    setTextureUnit(texUnit);
    setWidth(width);
    setHeight(height);
    setBpp(bpp);

    glGetError();

    glGenBuffers(1, &m_oglHandle[0]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_oglHandle[0]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, width*height*(bpp/8), 0, GL_STREAM_DRAW);

    glGenTextures(1, &m_oglHandle[1]);
    glBindTexture(GL_TEXTURE_2D, m_oglHandle[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, getWidth(), getHeight(), 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, 0);

    if(glGetError() == GL_NO_ERROR){
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      setState(STATE_LOADED);
    }
  }

  if(!vaild()){
    m_log.msg(CLogError() << "[cGenericTexture] Blad podczas ladowania tekstury");
  }
}

CGenericTexture::~CGenericTexture() {
  if(vaild()){
    activeTextureUnit(getTextureUnit());
    glDeleteTextures(1, &m_oglHandle[1]);
    glDeleteBuffers(1, &m_oglHandle[0]);
    setState(STATE_UNLOADED);
  }
}

void CGenericTexture::activeTextureUnit(unsigned int texUnit) const {
  switch(texUnit){
    case 0: glActiveTexture(GL_TEXTURE0); break;
    case 1: glActiveTexture(GL_TEXTURE1); break;
    case 2: glActiveTexture(GL_TEXTURE2); break;
    case 3: glActiveTexture(GL_TEXTURE3); break;
    default: break;
  }
}

bool CGenericTexture::copy(const std::vector<unsigned char>& data){
  if(lock() && data.size()==getWidth()*getHeight()*(getBpp()/8)){
    unsigned char* texture = buffer();
    for(unsigned int i=0; i<data.size(); ++i){
      texture[i] = data[i];
    }
    return unlock();
  }
  else{
    return false;
  }
}

bool CGenericTexture::lock(){
  if(!vaild()) return false;

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_oglHandle[0]);
  m_buffer = static_cast<GLubyte*>(glMapBuffer(GL_PIXEL_UNPACK_BUFFER,GL_READ_WRITE));
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  if(m_buffer == 0){
    m_log.msg(CLogWarning() << "Mapowanie tekstury nie powiodlo sie");
    return false;
  }

  return true;
}

bool CGenericTexture::unlock(){
  if(!vaild() || m_buffer == 0) return false;

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_oglHandle[0]);
  glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  m_buffer = 0;
  bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getWidth(), getHeight(),
      GL_BGRA, GL_UNSIGNED_BYTE, 0);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  return true;
}

unsigned char* CGenericTexture::buffer(){
  return m_buffer;
}

void CGenericTexture::bind() const {
  if(vaild()){
    activeTextureUnit(getTextureUnit());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_oglHandle[1]);
  }
}
// <--

/* ----------------------------------------------------------------------------
 * cTextTexture
 */
CTextTexture::CTextTexture(unsigned int width, unsigned int height, FT_Face& face,
    const string& name, unsigned int texUnit)
  : CGenericTexture(width,height,32,name,texUnit), m_fgColor(1.0f, 1.0f, 1.0f, 1.0f),
    m_bgColor(0.0f, 0.0f, 0.0f, 0.0f)
{
  m_face = face;
  m_margin = 4;
}

CTextTexture::~CTextTexture(){

}

const CVector4f& CTextTexture::getForegroundColor() const {
  return m_fgColor;
}

const CVector4f& CTextTexture::getBackgroundColor() const {
  return m_bgColor;
}

int CTextTexture::getMargin() const {
  return m_margin;
}

const string& CTextTexture::getText() const {
  return m_text;
}

void CTextTexture::setForegroundColor(const CVector4f& color){
  m_fgColor = color;
}

void CTextTexture::setBackgroundColor(const CVector4f& color){
  m_bgColor = color;
}

void CTextTexture::setMargin(int margin){
  m_margin = margin;
}

void CTextTexture::setText(const string& text){
  m_text = text;

  if(lock())
  {
    // Zapomnij o poprzedniej zawartosci
    glBufferData(GL_PIXEL_UNPACK_BUFFER, getWidth()*getHeight()*4, 0, GL_STREAM_DRAW);

    // Wypelnienie tekstury kolorem tla
    for(unsigned int i=0; i<getWidth()*getHeight()*4; ++i){
      buffer()[i] = static_cast<unsigned char>(m_bgColor[i%4]*255.0f);
    }

    FT_GlyphSlot  slot = m_face->glyph;

    CVector2i pen(getMargin(), getHeight()-(getMargin()+1));

    for(unsigned int i=0; i<text.size(); ++i){
      FT_UInt glyph_index = FT_Get_Char_Index(m_face,text[i]);
      FT_Load_Glyph(m_face, glyph_index, FT_LOAD_DEFAULT);
      FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);

      if(text[i]=='\n' || text[i]=='\r'){
        pen.x() = getMargin();
        pen.y() -= slot->bitmap_top + slot->bitmap.rows;
      }
      else{
        int x = pen.x() + slot->bitmap_left;
        int y = pen.y() + (slot->bitmap_top - (slot->linearVertAdvance>>16));
        blit(&slot->bitmap, x, y);
        pen.x() += (m_face->glyph->advance.x >> 6);
        pen.y() += (m_face->glyph->advance.y >> 6);
      }
    }

    unlock();
  }
}

void CTextTexture::blit(FT_Bitmap* bm, int x, int y){
  if(x<0 || x+(bm->width+getMargin())>getWidth() ||
     y-(bm->rows+getMargin())<0 || y>getHeight() ||
     bm->pixel_mode != FT_PIXEL_MODE_GRAY)
  {
    return;
  }

  unsigned char* src = bm->buffer;
  unsigned char* dst = &buffer()[y*getWidth()*4 + x*4];
  for(int i=0; i<bm->rows; ++i){
    for(int j=0; j<bm->width; ++j){
      if(src[j]>128){
        dst[0] = static_cast<unsigned char>(m_fgColor[2]*src[j]);
        dst[1] = static_cast<unsigned char>(m_fgColor[1]*src[j]);
        dst[2] = static_cast<unsigned char>(m_fgColor[0]*src[j]);
        dst[3] = static_cast<unsigned char>(m_fgColor[3]*255.0f);
      }
      dst+=4;
    }
    dst -=(getWidth()+bm->width)*4;
    src += bm->pitch;
  }
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractMaterial
 */
CAbstractMaterial::CAbstractMaterial(){}

CAbstractMaterial::~CAbstractMaterial(){}

smart_ptr<CAbstractTexture> CAbstractMaterial::getTexture(unsigned int texUnit) const {
  return m_texture[texUnit];
}

const CVector4f& CAbstractMaterial::getAmbient() const {
  return m_ambient;
}

const CVector4f& CAbstractMaterial::getDiffuse() const {
  return m_diffuse;
}

const CVector4f& CAbstractMaterial::getSpecular() const {
  return m_specular;
}

const float& CAbstractMaterial::getReflection() const {
  return m_reflection;
}

void CAbstractMaterial::setTexture(smart_ptr<CAbstractTexture> texture){
  if(texture && texture->getTextureUnit()<4){
    m_texture[texture->getTextureUnit()] = texture;
  }
}

void CAbstractMaterial::setAmbient(const CVector4f& ambient){
  m_ambient = ambient;
}

void CAbstractMaterial::setDiffuse(const CVector4f& diffuse){
  m_diffuse = diffuse;
}

void CAbstractMaterial::setSpecular(const CVector4f& specular){
  m_specular = specular;
}

void CAbstractMaterial::setReflection(const float& reflection){
  m_reflection = reflection;
}
// <--

/* ----------------------------------------------------------------------------
 * cGenericMaterial
 */
CGenericMaterial::CGenericMaterial(const string& name)
{
  setName(name);
  setState(STATE_LOADED);
  setAmbient(CVector4f(1.0f,1.0f,1.0f,1.0f));
  setDiffuse(CVector4f(0.2f,0.2f,0.2f,1.0f));
  setSpecular(CVector4f(0.0f,0.0f,0.0f,1.0f));
  setReflection(8.0f);
}

CGenericMaterial::~CGenericMaterial(){}

void CGenericMaterial::bind() const {
  m_log.msg(CLogWarning() << "cGenericMaterial::bind() Pusta funkcja !");
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractMesh
 */
CAbstractMesh::CAbstractMesh(){
  m_vertexCount = 0;
}

CAbstractMesh::~CAbstractMesh(){}

unsigned int CAbstractMesh::getVertexCount() const {
  return m_vertexCount;
}

void CAbstractMesh::setVertexCount(unsigned int vertexCount){
  m_vertexCount = vertexCount;
}
// <--

/* ----------------------------------------------------------------------------
 * cGenericMesh
 */
CGenericMesh::CGenericMesh(const unsigned int& vertexCount,
    const std::vector<CVector3f>& vertex, const std::vector<CVector3f>& normal,
    const std::vector<CVector2f>& coord, const string& name)
{
  setState(STATE_ERROR);
  setHandle(m_oglHandle);
  setName(name);
  setVertexCount(vertexCount);
  for(int i=0; i<3; ++i) m_oglHandle[i] = 0;

  glGetError();

  // Tablica wierzcholkow
  if(getVertexCount() != 0 && vertex.size() == getVertexCount()){
    glGenBuffers(1, &m_oglHandle[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[0]);
    glBufferData(GL_ARRAY_BUFFER, vertex.size()*sizeof(GLfloat)*3,
        reinterpret_cast<const float*>(&vertex.front()), GL_STATIC_DRAW);
  }
  else{
    m_log.msg(CLogError() << "[cGenericMesh] Brak wierzcholkow siatki");
  }

  // Wektory normalne wierzcholkow
  if(normal.size() == getVertexCount()){
    glGenBuffers(1, &m_oglHandle[1]);
    glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size()*sizeof(GLfloat)*3,
        reinterpret_cast<const float*>(&normal.front()), GL_STATIC_DRAW);
  }
  else{
    m_log.msg(CLogWarning() << "[cGenericMesh] Brak wektorow normalnych");
  }

  // Wspolrzedne tekstur
  if(coord.size() == getVertexCount()){
    glGenBuffers(1, &m_oglHandle[2]);
    glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[2]);
    glBufferData(GL_ARRAY_BUFFER, coord.size()*sizeof(GLfloat)*2,
        reinterpret_cast<const float*>(&coord.front()), GL_STATIC_DRAW);
  }
  else{
    m_log.msg(CLogWarning() << "[cGenericMesh] Brak wspolrzednych tekstury 0");
  }

  if(glGetError() == GL_NO_ERROR){
    setState(STATE_LOADED);
  }

  if(!vaild()){
    m_log.msg(CLogError() << "[cGenericMesh] Blad podczas ladowania siatki");
  }
}

CGenericMesh::~CGenericMesh() {
  if(vaild()){
    for(int i=0; i<3; ++i){
      if(m_oglHandle[i] != 0) glDeleteBuffers(1, &m_oglHandle[i]);
    }
    setState(STATE_UNLOADED);
  }
}

void CGenericMesh::bind() const {
  if(vaild())
  {
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if(m_oglHandle[1] != 0){
      glEnableClientState(GL_NORMAL_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[1]);
      glNormalPointer(GL_FLOAT, 0, 0);
    }

    if(m_oglHandle[2] != 0){
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, m_oglHandle[2]);
      glClientActiveTexture(GL_TEXTURE0);
      glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractProgram
 */
CAbstractProgram::CAbstractProgram(){}

CAbstractProgram::~CAbstractProgram(){}
// <--

/* ----------------------------------------------------------------------------
 * cGenericProgram
 */
CGenericProgram::CGenericProgram( const string& srcVertexProgram,
    const string& srcFragmentProgram, const string& name)
{
  setHandle(m_oglHandle);
  setName(name);
  m_oglHandle[0] = m_oglHandle[1] = m_oglHandle[2] = 0;
  setState(STATE_ERROR);

  if(srcVertexProgram.length()>0 && srcFragmentProgram.length()>0){
    // Generuj uchwyty dla shaderow
    m_oglHandle[0] = glCreateShader(GL_VERTEX_SHADER);
    m_oglHandle[1] = glCreateShader(GL_FRAGMENT_SHADER);
    // Ustaw zrodla
    const char* v = srcVertexProgram.data();
    const char* f = srcFragmentProgram.data();
    glShaderSource(m_oglHandle[0], 1, &v, 0);
    glShaderSource(m_oglHandle[1], 1, &f, 0);
    // Kompuluj
    glCompileShader(m_oglHandle[0]);
    glCompileShader(m_oglHandle[1]);
    
	string msg;
	msg = getShaderInfoLog(m_oglHandle[0]);
	m_log.msg(CLogWarning() << "Vertex Program Info:\n" << msg);
	msg = getShaderInfoLog(m_oglHandle[1]);
    m_log.msg(CLogWarning() << "Fragment Program Info:\n" << msg);
	
    // Utworz obiekt programu
    m_oglHandle[2] = glCreateProgram();
    glAttachShader(m_oglHandle[2], m_oglHandle[0]); // Vertex Program
    glAttachShader(m_oglHandle[2], m_oglHandle[1]); // Fragment Program
    // Linkuj program
    glLinkProgram(m_oglHandle[2]);

    // Sprawdzenie stanu
    int status = 0;
    glGetProgramiv(m_oglHandle[2], GL_LINK_STATUS, &status);
    if(status == GL_TRUE){
      setState(STATE_LOADED);
    }
  }

  if(!vaild()){
    m_log.msg(CLogError() << "Blad podczs tworzenie shadera:\n" << getProgramInfoLog());
  }
}

CGenericProgram::~CGenericProgram(){
  if(vaild()){
    glDetachShader(m_oglHandle[2], m_oglHandle[0]);
    glDetachShader(m_oglHandle[2], m_oglHandle[1]);
    glDeleteProgram(m_oglHandle[2]);
  }
}

string CGenericProgram::getShaderInfoLog(unsigned int handle) const {
  int lenght = 0;

  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &lenght);

  if(lenght > 0){
    char *buff = new char[lenght];
    glGetShaderInfoLog(handle, lenght, &lenght, buff);
    string str(buff, lenght);
    delete[] buff;
    return str;
  }
  else{
    return string();
  }
}

string CGenericProgram::getProgramInfoLog() const {
  int lenght = 0;

  glGetProgramiv(m_oglHandle[2], GL_INFO_LOG_LENGTH, &lenght);

  if(lenght > 0){
    char *buff = new char[lenght];
    glGetProgramInfoLog(m_oglHandle[2], lenght, &lenght, buff);
    string str(buff, lenght);
    delete[] buff;
    return str;
  }
  else{
    return string();
  }
}

bool CGenericProgram::setUniform1i(const string& name, int value) const {
  if(!vaild()) return false;

  int location = glGetUniformLocation(m_oglHandle[2], name.c_str());
  if(location<0) return false;

  glUniform1i(location, value);

  return true;
}

bool CGenericProgram::setUniform1f(const string& name, float value) const {
  if(!vaild()) return false;

  int location = glGetUniformLocation(m_oglHandle[2], name.c_str());
  if(location<0) return false;

  glUniform1f(location, value);

  return true;
}

bool CGenericProgram::setUniform2f(const string& name, const CVector2f& v2) const {
  if(!vaild()) return false;

  int location = glGetUniformLocation(m_oglHandle[2], name.c_str());
  if(location<0) return false;

  glUniform2f(location, v2.u(), v2.v());

  return true;
}

bool CGenericProgram::setUniform3f(const string& name, const CVector3f& v3) const {
  if(!vaild()) return false;

  int location = glGetUniformLocation(m_oglHandle[2], name.c_str());
  if(location<0) return false;

  glUniform3f(location, v3.x(), v3.y(), v3.z());

  return true;
}

bool CGenericProgram::setUniform4f(const string& name, const CVector4f& v4) const {
  if(!vaild()) return false;

  int location = glGetUniformLocation(m_oglHandle[2], name.c_str());
  if(location<0) return false;

  bind();
  glUniform4f(location, v4.x(), v4.y(), v4.z(), v4.w());

  return true;
}

void CGenericProgram::bind() const {
  if(vaild()){
    glUseProgram(m_oglHandle[2]);
  }
}
// <--

/* ----------------------------------------------------------------------------
 * cAbstractFrameBuffer
 */
CAbstractFrameBuffer::CAbstractFrameBuffer(){}

CAbstractFrameBuffer::~CAbstractFrameBuffer(){}
// <--

/* ----------------------------------------------------------------------------
 * cFrameBuffer
 */
CFrameBuffer::CFrameBuffer(int width, int height, const string& name)
{
  setState(STATE_ERROR);
  setHandle(m_oglHandle);
  setName(name);
  m_enabled = false;

  glGenRenderbuffers(1, &m_oglHandle[1]);
  glBindRenderbuffer(GL_RENDERBUFFER, m_oglHandle[1]);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  glGenTextures(1, &m_oglHandle[2]);
  glBindTexture(GL_TEXTURE_2D, m_oglHandle[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,  width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmapEXT(GL_TEXTURE_2D);

  glGenFramebuffers(1, &m_oglHandle[0]);
  glBindFramebuffer(GL_FRAMEBUFFER, m_oglHandle[0]);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_oglHandle[1]);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_oglHandle[2], 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    m_log.msg(CLogError() << "Blad podczas tworzenia FBO");
  }
  else{
    setState(STATE_LOADED);
    m_log.msg(CLogMessage() << "Utworzono cFrameBuffer: " << width << "x" << height);
  }
}

CFrameBuffer::~CFrameBuffer(){
  if(m_oglHandle[1]) glDeleteRenderbuffers(1, &m_oglHandle[1]);
  if(m_oglHandle[2]) glDeleteTextures(1, &m_oglHandle[2]);
  if(m_oglHandle[0]) glDeleteFramebuffers(1, &m_oglHandle[0]);
}

bool CFrameBuffer::getEnabled() const {
  return m_enabled;
}

void CFrameBuffer::setEnabled(bool enabled){
  m_enabled = enabled;
  if(m_enabled && m_oglHandle[0]){
    glBindFramebuffer(GL_FRAMEBUFFER, m_oglHandle[0]);
  }
  else{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
}

void CFrameBuffer::clear(){
  if(!m_enabled) setEnabled(true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CFrameBuffer::bind() const {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_oglHandle[2]);
  glGenerateMipmap(GL_TEXTURE_2D);
}
// <--


/* ----------------------------------------------------------------------------
 * cResourceFactory
 */
CResourceFactory::CResourceFactory()
  : m_log(CLog::instance())
{
  if(FT_Init_FreeType(&m_freetype)){
    m_log.msg(CLogError() << "Blad podczas inicjalzacji FreeType");
  }
}

CResourceFactory::~CResourceFactory(){
  FT_Done_FreeType(m_freetype);
}

CResourceFactory& CResourceFactory::instance(){
  static CResourceFactory object;
  return object;
}

vector< smart_ptr<CAbstractMesh> >
CResourceFactory::buildMesh(const string& path)
{
  smart_ptr<CAbstractMesh> ptr;
  string ext = path.substr(path.rfind('.')+1);
  if(ext == "OBJ" || ext == "obj"){
    return fileLoad_OBJ(path);
  }
  else{
    m_log.msg(CLogError() << "Nieznany format pliku: " << path);
	return vector< smart_ptr<CAbstractMesh> >();
  }
}

vector< smart_ptr<CAbstractMaterial> >
CResourceFactory::buildMaterial(const string& path)
{
  smart_ptr<CAbstractMaterial> ptr;
  string ext = path.substr(path.rfind('.')+1);
  if(ext == "MTL" || ext == "mtl"){
    return fileLoad_MTL(path);
  }
  else if(ext == "MAT" || ext == "mat"){
    return fileLoad_MAT(path);
  }
  else{
    m_log.msg(CLogError() << "Nieznany format pliku: " << path);
	return vector< smart_ptr<CAbstractMaterial> >();
  }
}

smart_ptr<CAbstractTexture>
CResourceFactory::buildTexture(const string& path, int texUnit){
  smart_ptr<CAbstractTexture> ptr;
  string ext = path.substr(path.rfind('.')+1);
  if(ext == "TGA" || ext == "tga"){
    return fileLoad_TGA(path, texUnit);
  }
  else{
    m_log.msg(CLogError() << "Nieznany format pliku: " << path);
	return smart_ptr<CAbstractTexture>();
  }
}

smart_ptr<CAbstractProgram>
CResourceFactory::buildProgram(const string& path){
  return fileLoad_GLSL(path);
}

bool CResourceFactory::addToCache(smart_ptr<CAbstractResource> ptr){
  if(ptr && ptr->vaild() && m_cache.find(ptr->getName())==m_cache.end()){
    m_cache[ptr->getName()] = ptr;
    return true;
  }
  else
    return false;
}

smart_ptr<CAbstractResource> CResourceFactory::getFromCache(const string& key){
  smart_ptr<CAbstractResource> ptr;
  if(m_cache.find(key)!=m_cache.end()){
    ptr = m_cache[key];
  }
  return ptr;
}

vector< smart_ptr<CAbstractMesh> >
CResourceFactory::fileLoad_OBJ(const string& fileName)
{
  vector< smart_ptr<CAbstractMesh> > container;

  fstream file(fileName.c_str(), fstream::in);
  char buff[256];

  if(!file.is_open()){
    m_log.msg(CLogError() << "Blad otwarcia pliku: " << fileName);
    return container;
  }

  // Dane werteksow odczytane z pliku
  vector<CVector3f> objVertex, objNormal;
  vector<CVector2f> objCoord;

  unsigned int lineNumber = 0;

  /* Pierwsze parsowanie pliku
   * Znajduje wszystkie deklaracje dotyczace wierzcholkow
   */
  file.seekg(0,ios::beg);
  while(!file.getline(buff,256).eof() && !file.bad()){
    ++lineNumber;

    stringstream stream(buff);
    string marker;
    stream >> marker;

    if(marker.length()<1 || marker == "#" || marker == "f" || marker == "g" ||
       marker == "o" || marker == "mtllib" || marker == "usemtl" )
    {
      // Znane znaki ktore nalezy pominac
      continue;
    }

    if(marker == "v"){
      objVertex.push_back(CVector3f());
      stream >> objVertex.back().x() >> objVertex.back().y() >> objVertex.back().z();
      continue;
    }

    if(marker == "vt"){
      objCoord.push_back(CVector2f());
      stream >> objCoord.back().u() >> objCoord.back().v();
      continue;
    }

    if(marker == "vn"){
      objNormal.push_back(CVector3f());
      stream >> objNormal.back().x() >> objNormal.back().y() >> objNormal.back().z();
      continue;
    }

    m_log.msg(CLogWarning()
        << "Podczas parsowania pliku natrafiono na nieznany symbol "
        << "(" << fileName << ":"  << lineNumber << ")"
    );
  }

  /* Drugie parsowanie pliku
   * Wszystkie wierzcholki zostaly wczytane wiec mozna zbudowac siatke
   */
  vector<CVector3f> v, n;
  vector<CVector2f> c;
  unsigned int vertexCount = 0;
  unsigned int faceCount = 0;
  string materialName;

  file.clear();
  file.seekg(0,ios::beg);
  lineNumber = 0;

  while(!file.eof() && !file.bad()){
    file.getline(buff,256).eof();
    ++lineNumber;

    stringstream stream(buff);
    string marker;
    stream >> marker;

    // Ladowanie siatki
    if(vertexCount>0 && (file.eof() || marker=="usemtl"))
    {
      string name = fileName+":"+materialName;
	  smart_ptr<CAbstractMesh> ptr;

	  ptr = getFromCache(name).down_cast<CAbstractMesh>();
      if(!ptr){
        m_log.msg(CLogMessage() << "Ladowanine siatki: " << name);

		CAbstractMesh* mesh = 0;
        mesh = new CGenericMesh(vertexCount, v, n, c, name);
		ptr = smart_ptr<CAbstractMesh>(mesh);
		addToCache(ptr.up_cast<CAbstractResource>());
      }

      container.push_back(ptr);

      // Czysc dane pomocnicze
      materialName.clear();
      v.clear();
      n.clear();
      c.clear();
      vertexCount = faceCount = 0;
    }

    if(marker == "usemtl"){
      // Zapisz nazwe nowgo obiektu
      stream >> materialName;
      continue;
    }

    // Deklaracja fejsa
    if(marker == "f")
    {
      ++faceCount;

      for(unsigned int i=0; i<3 && !stream.eof(); ++i){
        unsigned int id[] = {0,0,0};

        // ID wspolrzednych wierzcholka
        stream >> id[0];

        if(stream.peek() == '/'){
          stream.ignore(1);

          // ID wspolrzednych tekstury wierzcholka
          if(stream.peek() != '/' && stream.peek() != ' '){
            stream >> id[1];
          }

          // ID wspolrzednych normalnej wierzcholka
          if(stream.peek()=='/'){
            stream.ignore(1);
            stream >> id[2];
          }
        }

        // Zapisz dane werteksa
        if(!stream.fail()){
          if(id[0]>0 && id[0]<=objVertex.size()){
            v.push_back(objVertex.at(id[0]-1));
          }
          if(id[1]>0 && id[1]<=objCoord.size()){
            c.push_back(objCoord.at(id[1]-1));
          }
          if(id[2]>0 && id[2]<=objNormal.size()){
            n.push_back(objNormal.at(id[2]-1));
          }

          ++vertexCount;
        }
        else{
          m_log.msg(CLogWarning()
              << "Blad danych (" << fileName << ":" << lineNumber << ")"
          );
        }
      }
    }
  }

  file.close();

  return container;
}

vector< smart_ptr<CAbstractMaterial> >
CResourceFactory::fileLoad_MAT(const std::string& fileName){
  vector< smart_ptr<CAbstractMaterial> > container;

  fstream file(fileName.c_str(), fstream::in);

  if(!file.is_open()){
    m_log.msg(CLogError() << "Blad otwarcia pliku: " << fileName);
  }
  else{
    string materialName, textureName[4];
    CVector4f ambient, diffuse, specular;
    float reflection;
    bool isData[6] = {false};

    char buff[256];
    while((!file.eof() && !file.bad()) || isData[0]){
      file.getline(buff, 256);

      stringstream stream(buff);
      string marker;
      stream >> marker;

      if(isData[0] && (file.eof() || marker == "mat")){
        string name = fileName.substr(fileName.rfind('/')+1)+":"+materialName;
        string dir = fileName.substr(0, fileName.rfind('/')+1);

		smart_ptr<CAbstractMaterial> ptr;
		ptr = getFromCache(name).down_cast<CAbstractMaterial>();

        if(!ptr){
          m_log.msg(CLogMessage() << "Ladowanie materialu: " << dir << name);

		  ptr = smart_ptr<CAbstractMaterial>(new CGenericMaterial(name));

          if(isData[1]) ptr->setAmbient(ambient);

          if(isData[2]) ptr->setDiffuse(diffuse);

          if(isData[3]) ptr->setSpecular(specular);

          if(isData[4]) ptr->setReflection(reflection);

          if(isData[5]){
            for(unsigned int i=0; i<4; ++i){
              if(textureName[i].size()>0){
                ptr->setTexture(fileLoad_TGA(dir+textureName[i],i));
                textureName[i].clear();
              }
            }
          }

		  addToCache(ptr.up_cast<CAbstractResource>());
        }

        container.push_back(ptr);

        materialName.clear();
        for(unsigned int i=0; i<6; ++i) isData[i] = false;
      }

      if(marker == "#") continue;

      if(marker == "mat"){
        stream >> materialName;
        continue;
      }

      if(marker == "a"){
        stream >> ambient.x() >> ambient.y() >> ambient.z() >> ambient.w();
        isData[0] = isData[1] = true;
        continue;
      }

      if(marker == "d"){
        stream >> diffuse.x() >> diffuse.y() >> diffuse.z() >> diffuse.w();
        isData[0] = isData[2] = true;
        continue;
      }

      if(marker == "s"){
        stream >> specular.x() >> specular.y() >> specular.z() >> specular.w();
        isData[0] = isData[3] = true;
        continue;
      }

      if(marker == "r"){
        stream >> reflection;
        isData[0] = isData[4] = true;
        continue;
      }

      if(marker == "t"){
        unsigned int texUnit;
        stream >> texUnit;
        if(texUnit < 4){
          stream >> textureName[texUnit];
          isData[0] = isData[5] = true;
        }
        continue;
      }
    }

    file.close();
  }

  return container;
}

vector< smart_ptr<CAbstractMaterial> >
CResourceFactory::fileLoad_MTL(const std::string& fileName){
  vector< smart_ptr<CAbstractMaterial> > container;

  fstream file(fileName.c_str(), fstream::in);

  if(!file.is_open()){
    m_log.msg(CLogError() << "Blad otwarcia pliku: " << fileName);
  }
  else{
    string materialName, textureName;
    CVector4f ambient, diffuse, specular;
    float  alpha, reflection;
    bool isData[7] = {false};

    char buff[256];
    while((!file.eof() && !file.bad()) || isData[0]){
      file.getline(buff, 256);

      stringstream stream(buff);
      string marker;
      stream >> marker;

      if(isData[0] && (file.eof() || marker == "newmtl")){
        string name = fileName.substr(fileName.rfind('/')+1)+":"+materialName;
        string dir = fileName.substr(0, fileName.rfind('/')+1);

		smart_ptr<CAbstractMaterial> ptr;
		ptr = getFromCache(name).down_cast<CAbstractMaterial>();

        if(!ptr){
          m_log.msg(CLogMessage() << "Ladowanie materialu: " << dir << name);

		  ptr = smart_ptr<CAbstractMaterial>(new CGenericMaterial(name));

          if(isData[1]) ptr->setAmbient(ambient);

          if(isData[2]) ptr->setDiffuse(diffuse);

          if(isData[3]) ptr->setSpecular(specular);

          if(isData[4]) ptr->setReflection(reflection);

          if(isData[5]){
            CVector4f tmp;
            tmp = ptr->getAmbient();
            tmp.w() = alpha;
            ptr->setAmbient(tmp);
            tmp = ptr->getDiffuse();
            tmp.w() = alpha;
            ptr->setDiffuse(tmp);
            tmp = ptr->getSpecular();
            tmp.w() = alpha;
            ptr->setSpecular(tmp);
          }

          if(isData[6] && textureName.size()>0){
			smart_ptr<CAbstractTexture> tex = buildTexture(dir+textureName);
            ptr->setTexture(tex);
          }

		  addToCache(ptr.up_cast<CAbstractResource>());
        }

        container.push_back(ptr);

        materialName.clear();
        for(unsigned int i=0; i<7; ++i) isData[i] = false;
      }

      if(marker == "#") continue;

      if(marker == "newmtl"){
        stream >> materialName;
        continue;
      }

      if(marker == "Ka"){
        stream >> ambient.x() >> ambient.y() >> ambient.z() >> ambient.w();
        isData[0] = isData[1] = true;
        continue;
      }

      if(marker == "Kd"){
        stream >> diffuse.x() >> diffuse.y() >> diffuse.z() >> diffuse.w();
        isData[0] = isData[2] = true;
        continue;
      }

      if(marker == "Ks"){
        stream >> specular.x() >> specular.y() >> specular.z() >> specular.w();
        isData[0] = isData[3] = true;
        continue;
      }

      if(marker == "Ns"){
        stream >> reflection;
        isData[0] = isData[4] = true;
        continue;
      }

      if(marker == "Tr" || marker == "d"){
        stream >> alpha;
        isData[0] = isData[5] = true;
        continue;
      }

      if(marker == "map_Ka" || marker == "map_Kd"){
        stream >> textureName;
        isData[0] = isData[6] = true;
        continue;
      }
    }

    file.close();
  }

  return container;
}

smart_ptr<CAbstractTexture>
CResourceFactory::fileLoad_TGA(const string& fileName, int texUnit)
{
  smart_ptr<CAbstractTexture> ptr;
  ptr = getFromCache(fileName).down_cast<CAbstractTexture>();

  if(!ptr){

    m_log.msg(CLogMessage() << "Ladowanie tekstury: " << fileName);

    // --- Naglowek pliku TGA v1.0 (18 bajtow) ---------------------------------
    // 00h  Size of Image ID field (Pole zaczyna sie w 12h)
    // 01h  Color map type (1 wykorzystuje palete kolorow, 0 nie wykorzystuje)
    // 02h  Image type code (2 - truecolor bez palety, 10 - z konpresja RLE)
    // 03h  Color map origin
    // 05h  Color map length
    // 07h  Depth of color map entries
    // 08h  X origin of image
    // 0Ah  Y origin of image
    // 0Ch  Width of image
    // 0Eh  Height of image
    // 10h  Image pixel size (32 = 8r + 8g + 8b + 8a)
    // 11h  Image descriptor byte
    // -------------------------------------------------------------------------

    fstream file(fileName.c_str(), ios::in | ios::binary);

    // Wczytaj naglowek
    unsigned char header[18];
    file.read((char*) header, 18);

    // Sprawdz format pliku, TrueColor, bez palety, 32 bity na pixel !
    if(file.is_open() && header[1] == 0 && (header[2] == 2 || header[2] == 10)
       && header[16] == 32)
    {
      unsigned int width = header[13] * 256 + header[12];
      unsigned int height = header[15] * 256 + header[14];
      unsigned int bpp = header[16];
      unsigned int size = width * height * bpp/8; // bajty

      // Alokacja bufora
      vector<unsigned char> data(size, 0);

      file.seekg(18 + header[0], ios::beg);
      if(header[2] == 10){
        GLubyte pc;
        GLubyte pix[4];
        GLuint counter = 0;
        while(counter < size && !file.eof()){
          file.read((char*) &pc, 1);
          if((pc & 0x80) == 0x80){
            // RLE
            pc = (pc & 0x7f) + 1;
            file.read((char*) &pix, 4);
            for(unsigned int i = 0; i < pc; ++i){
              unsigned int id = counter + i * 4;
              for(unsigned int j = 0; j < 4; ++j)
                data[id + j] = pix[j];
            }
          }
          else{
            // NRLE
            pc += 1;
            for(unsigned int i = 0; i < pc; ++i){
              file.read((char*) &data[counter + i * 4], 4);
            }
          }
          counter += pc * 4;
        }
      }
      else{
        file.read((char*)&data[0], size);
      }

      file.close();

      CGenericTexture* tex = 0;
      tex = new CGenericTexture(width, height, bpp, data, fileName, texUnit);
	  ptr = smart_ptr<CAbstractTexture>(tex);
	  addToCache(ptr.up_cast<CAbstractResource>());
    }
    else{
      m_log.msg(CLogError() << "Blad podczas wczytywania tekstury: " << fileName);
    }
  }

  return ptr;
}

smart_ptr<CAbstractProgram> CResourceFactory::fileLoad_GLSL(const string& path)
{
  smart_ptr<CAbstractProgram> ptr = getFromCache(path).down_cast<CAbstractProgram>();

  if(!ptr){
    m_log.msg(CLogMessage() << "Ladowanie programu cieniujacego: " << path);

    // Vertex Shader Program / Fragment Shader Program
    string vsp, fsp;

    fstream file;
    char buff[256];

    do{
      file.open(string(path+".vsp").c_str(), fstream::in);
      if(!file.is_open()){
        m_log.msg(CLogError() << "Blad otwarcia pliku: " << path << ".vsp");
        vsp.clear();
        continue;
      }
      else{
        while(!file.getline(buff,256).eof()){
          vsp.append(buff);
          vsp.append("\n");
        }
        file.close();
      }

      file.clear();
      file.open(string(path+".fsp").c_str(), fstream::in);
      if(!file.is_open()){
        m_log.msg(CLogError() << "Blad otwarcia pliku: " << path << ".fsp");
        fsp.clear();
        continue;
      }
      else{
        while(!file.getline(buff,256).eof()){
          fsp.append(buff);
          fsp.append("\n");
        }
        file.close();
      }
    }
    while(false);

    if(vsp.length()>0 && fsp.length()>0){
	  CAbstractProgram* prog = 0;
      prog = new CGenericProgram(vsp, fsp, path);
	  ptr = smart_ptr<CAbstractProgram>(prog);
	  addToCache(ptr.up_cast<CAbstractResource>());
    }
    else{
      m_log.msg(CLogError() << "Blad odczytu programu cieniujacego");
    }
  }

  return ptr;
}

smart_ptr<CAbstractMesh> CResourceFactory::genMesh_Quad(float width, float height){
  // Generuj klucz
  stringstream stream;
  stream << "quad_" << width << "_" << height;
  string name = stream.str();

  smart_ptr<CAbstractMesh> ptr = getFromCache(name).down_cast<CAbstractMesh>();

  if(!ptr){
    m_log.msg(CLogMessage() << "Generowanie siatki: " << name);

    vector<CVector3f> v(6),n(6);
    vector<CVector2f> c(6);

    v[0].x() = v[4].x() = v[5].x() = -width/2;
    v[1].x() = v[2].x() = v[3].x() = width/2;
    v[0].y() = v[1].y() = v[5].y() = -height/2;
    v[2].y() = v[3].y() = v[4].y() = height/2;
    n[0].z() = n[1].z() = n[2].z() = n[3].z() = n[4].z() = n[5].z() = 1.0;
    c[0].u() = c[4].u() = c[5].u() = 0.0;
    c[1].u() = c[2].u() = c[3].u() = 1.0;
    c[0].v() = c[1].v() = c[5].v() = 0.0;
    c[2].v() = c[3].v() = c[4].v() = 1.0;

	CAbstractMesh* mesh = 0;
    mesh = new CGenericMesh(6, v, n, c, name);
	ptr = smart_ptr<CAbstractMesh>(mesh);
	addToCache(ptr.up_cast<CAbstractResource>());
  }

  return ptr;
}

smart_ptr<CAbstractTexture>
CResourceFactory::genTexture_Grid(int w, int h, int texUnit)
{
  // Generuj nazwe
  stringstream stream;
  stream << "GridTexture(" << w << "," << h << "," << 32 << ")";
  string name = stream.str();

  smart_ptr<CAbstractTexture> ptr = getFromCache(name).down_cast<CAbstractTexture>();

  if(!ptr){
    m_log.msg(CLogMessage() << "Generowanie tekstury: " << name);

    // Grafika 32 bitowa o rozmiarze w x h
    vector<unsigned char> data(w*h*4, 0);
    // Kolory kraty
    unsigned char color[] = {200, 80};

    for(int i=0; i<h; ++i){
      for(int j=0; j<w; ++j){
        int q = (i*h + j) * 4;

        if((i<h/2 && j<w/2) || (i>=h/2 && j>=w/2))
        {
          data[q] = color[0];
          data[q+1] = color[0];
          data[q+2] = color[0];
        }
        else{
          data[q] = color[1];
          data[q+1] = color[1];
          data[q+2] = color[1];
        }
      }
    }

	CAbstractTexture* tex = 0;
    tex = new CGenericTexture(w, h, 32, data, name, texUnit);
	smart_ptr<CAbstractTexture> ptr(tex);
	addToCache(ptr.up_cast<CAbstractResource>());
  }

  return ptr;
}


smart_ptr<CAbstractMesh> CResourceFactory::genMesh_Plane(int w, int h)
{
  // Generuja nazwe
  stringstream stream;
  stream << "PlaneMesh(" << w << "," << h << ")";
  string name = stream.str();

  smart_ptr<CAbstractMesh> ptr = getFromCache(name).down_cast<CAbstractMesh>();

  if(!ptr){
    m_log.msg(CLogMessage() << "Generowanie siatki: " << name);

    unsigned int count = abs(w*h*6);
    vector<CVector3f> v(count), n(count, CVector3f(0,1,0));
    vector<CVector2f> c(count);

    int q = 0;
    for(int i=0; i<h; ++i){
      for(int j=0; j<w; ++j){
        v[q] = CVector3f((float)j, 0.0f, (float)i);
        c[q] = CVector2f(0.0f ,1.0f);
        ++q;
        v[q] = CVector3f((float)j, 0.0f, (float)i+1.0f);
        c[q] = CVector2f(0.0f, 0.0f);
        ++q;
        v[q] = CVector3f((float)j+1.0f, 0.0f, (float)i+1.0f);
        c[q] = CVector2f(1.0f, 0.0f);
        ++q;
        v[q] = v[q-1];
        c[q] = c[q-1];
        ++q;
        v[q] = CVector3f((float)j+1.0f, 0.0f, (float)i);
        c[q] = CVector2f(1.0f, 1.0f);
        ++q;
        v[q] = v[q-5];
        c[q] = c[q-5];
        ++q;
      }
    }

	CAbstractMesh* mesh = 0;
    mesh = new CGenericMesh(count, v, n, c, name);
	smart_ptr<CAbstractMesh> ptr(mesh);
	addToCache(ptr.up_cast<CAbstractResource>());
  }

  return ptr;
}

smart_ptr<CTextTexture> CResourceFactory::createTextTexture(unsigned int width,
    unsigned int height, const std::string& font, const std::string& text,
    unsigned int pt)
{
  FT_Face face;
  if(FT_New_Face(m_freetype, font.c_str(), 0, &face)){
    m_log.msg(CLogError() << "Blad podczas tworzenia czcionki");
    return smart_ptr<CTextTexture>();
  }

  FT_Set_Char_Size(face, 0, pt*64, 72, 72);

  return smart_ptr<CTextTexture>(new CTextTexture(width,height,face,font+text,0));
}
// <--

