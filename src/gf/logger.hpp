/* File: ilogger.hpp
 * Date: 10.03.2010
 */

#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <cstdio>
#include <ctime>
#include <string>
#include <queue>
#include "ilogger.hpp"

namespace gf
{  
  /**  
   */
  class CLoggerEntry : public iLoggerEntry{
  public:
    enum TYPE{
      TYPE_ERROR = 1,
      TYPE_WARNING = 2,
      TYPE_MESSAGE = 3,
      TYPE_UNKNOW
    };

	CLoggerEntry(TYPE type = TYPE_UNKNOW);
    
    const unsigned int& getType() const;
    void setType(unsigned int type);
    const std::time_t& getTime() const;
    void setTime(time_t time);
    const std::string& getContent() const;
    void setContent(const std::string& content);
    
    iLoggerEntry& add(const char* arg);
    void clear();
    
    iLoggerEntry& operator<<(const char* arg);
    iLoggerEntry& operator<<(const std::string& arg);
    iLoggerEntry& operator<<(const char& arg);
    iLoggerEntry& operator<<(const int& arg);
    iLoggerEntry& operator<<(const unsigned int& arg);
    iLoggerEntry& operator<<(const long& arg);
    iLoggerEntry& operator<<(const unsigned long& arg);
    iLoggerEntry& operator<<(const float& arg);
    iLoggerEntry& operator<<(const double& arg);
    iLoggerEntry& operator<<(const long double& arg);
    iLoggerEntry& operator<<(const bool& arg);
    
  private:
    TYPE m_type;
    std::time_t m_time;
    std::string m_content;
  };
  
  /**
   */
  class CLogError : public CLoggerEntry{
  public:
	CLogError();
  };
  
  /**
   */
  class CLogWarning : public CLoggerEntry{
  public:
	CLogWarning();
  };  
  
  /**
   */
  class CLogMessage : public CLoggerEntry{
  public:
	CLogMessage();
  };
    
  class CLogger : public iLogger{
  public:  
    enum OUTPUT{
      OUTPUT_CONSOLE = 0,
      OUTPUT_FILE = 1,
      OUTPUT_ALL
    };
    
	CLogger(OUTPUT output = OUTPUT_ALL, const std::string& filename="default.log");
	virtual ~CLogger();
    
    void append(const iLoggerEntry& entry);
    void flush();
    void msg(const iLoggerEntry& entry);
    
    unsigned int getLevel() const;    
    void setLevel(unsigned int level);
    
  protected:
    void print_console(const std::string& str);
    void print_file(const std::string& str);
    
    std::queue<const iLoggerEntry*> m_buffer;
    unsigned int m_level;
    OUTPUT m_output;
    std::FILE* m_file;
  };
  
  class CLog : public CLogger{
  public:
	static CLog& instance(CLogger::OUTPUT output = CLogger::OUTPUT_ALL,
                          const std::string& filename = "log.txt");
  
  private:
	CLog(CLogger::OUTPUT output = CLogger::OUTPUT_ALL,
         const std::string& filename="default.log");
	CLog(const CLog&);
	CLog& operator=(const CLog&);
  };
}

#endif //LOGGER_HEADER
