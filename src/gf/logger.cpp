/* File: logger.cpp
 * Date: 10.03.2010
 */

#include "logger.hpp"

using namespace std;
using namespace gf;

/* -----------------------------------------------------------------------------
 * cLoggerEntry
 */
CLoggerEntry::CLoggerEntry(CLoggerEntry::TYPE type){
  m_type = type;
  m_time = std::time(0);
      
  char str[16];
  strftime(str, 16, "[%H:%M:%S]", localtime(&m_time));
  m_content.append(str);

  switch(m_type){
    case TYPE_MESSAGE: m_content.append("(MESSAGE) "); break;
    case TYPE_WARNING: m_content.append("(WARNING) "); break;
    case TYPE_ERROR: m_content.append("(ERROR) "); break;
    default: m_content.append(" "); break;    
  }
}

const unsigned int& CLoggerEntry::getType() const{
  return (unsigned int&)m_type;
}

void CLoggerEntry::setType(unsigned int type){
  m_type = (CLoggerEntry::TYPE)type;
}

const time_t& CLoggerEntry::getTime() const{
  return m_time;
}

void CLoggerEntry::setTime(time_t time){
  m_time = time;
}

const string& CLoggerEntry::getContent() const{
  return m_content;
}

void CLoggerEntry::setContent(const string& content){
  m_content = content;
}

ILoggerEntry& CLoggerEntry::add(const char* arg){
    m_content.append(arg);
    return *this;
}

void CLoggerEntry::clear(){
  m_content.clear();
}

ILoggerEntry& CLoggerEntry::operator<<(const char* arg){
    return add(arg);
}

ILoggerEntry& CLoggerEntry::operator<<(const std::string& arg){
    m_content.append(arg);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const char& arg){
    m_content.append(&arg, 1);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const int& arg){
    char buff[16];
    sprintf(buff, "%d", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const unsigned int& arg){
    char buff[16];
    sprintf(buff, "%u", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const long& arg){
    char buff[24];
    sprintf(buff, "%ld", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const unsigned long& arg){
    char buff[24];
    sprintf(buff, "%lu", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const float& arg){
    char buff[16];
    sprintf(buff, "%f", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const double& arg){
    char buff[24];
    sprintf(buff, "%g", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const long double& arg){
    char buff[24];
    sprintf(buff, "%Lg", arg);
    m_content.append(buff);
    return *this;
}

ILoggerEntry& CLoggerEntry::operator<<(const bool& arg){
    arg ? m_content.append("TRUE") : m_content.append("FALSE");
    return *this;
}
// <--

/* -----------------------------------------------------------------------------
 * cLogError
 */
CLogError::CLogError() : CLoggerEntry(CLoggerEntry::TYPE_ERROR){
  // pass
}
// <--

/* -----------------------------------------------------------------------------
 * cLogWarningr
 */
CLogWarning::CLogWarning() : CLoggerEntry(CLoggerEntry::TYPE_WARNING){
  // pass
}
// <--

/* -----------------------------------------------------------------------------
 * cLogMessage
 */
CLogMessage::CLogMessage() : CLoggerEntry(CLoggerEntry::TYPE_MESSAGE){
  // pass
}
// <--

/* -----------------------------------------------------------------------------
 * cLogger
 */
CLogger::CLogger(CLogger::OUTPUT output, const string& filename){
  m_level = 4;
  m_output = output;
  if(m_output==CLogger::OUTPUT_FILE || m_output==CLogger::OUTPUT_ALL){
    m_file = fopen(filename.c_str(), "w");
  }
  else{
    m_file = 0;
  }
}

CLogger::~CLogger(){
  if(m_file){
    fclose(m_file);
  }
}

void CLogger::append(const ILoggerEntry& entry){
  m_buffer.push(&entry);
}

void CLogger::flush(){
  while(!m_buffer.empty()){
    if(m_buffer.front()->getType() < m_level){
      switch(m_output){
        case OUTPUT_CONSOLE: 
          print_console(m_buffer.front()->getContent());
          break;
        case OUTPUT_FILE:
          print_file(m_buffer.front()->getContent());
          break;
        default:
          print_console(m_buffer.front()->getContent());
          print_file(m_buffer.front()->getContent());
          break;
      }
    }
    m_buffer.pop();
  }
  fflush(stdout);
}

void CLogger::msg(const ILoggerEntry& entry){
  append(entry);;
  flush();
}

unsigned int CLogger::getLevel() const{
  return m_level;
}

void CLogger::setLevel(unsigned int level){
  m_level = level;
}

void CLogger::print_console(const std::string& str){
  printf("%s\n", str.c_str());
}

void CLogger::print_file(const std::string& str){
  if(m_file){
    fprintf(m_file, "%s\n", str.c_str());
  }
  else{
    printf("[LOGGER ERROR] Output file unaviable\n");
  }
}
// <--

/* -----------------------------------------------------------------------------
 * cLog
 */
CLog::CLog( CLogger::OUTPUT output, const std::string& filename):
  CLogger(output, filename)
{
  // pass
}

CLog& CLog::instance(CLogger::OUTPUT output, const std::string& filename){
  static CLog object(output, filename);
  return object;
}

// <--
