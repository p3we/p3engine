/* File: ilogger.hpp
 * Date: 10.03.2010
 */

#ifndef ILOGGER_HEADER
#define ILOGGER_HEADER

#include <ctime>
#include <string>

namespace gf
{
  /**
   * Interface for objects representing logger entry.
   */
  class iLoggerEntry{
  public:
    virtual ~iLoggerEntry(){};

    virtual const unsigned int& getType() const = 0;
    virtual void setType(unsigned int type) = 0;
    
    virtual const std::time_t& getTime() const = 0;
    virtual void setTime(time_t time) = 0;
    
    virtual const std::string& getContent() const = 0;
    virtual void setContent(const std::string& content) = 0;
    
    virtual iLoggerEntry& add(const char* arg) = 0;
    virtual void clear() = 0;
    
    virtual iLoggerEntry& operator<<(const char* arg) = 0;
    virtual iLoggerEntry& operator<<(const std::string& arg) = 0;
    virtual iLoggerEntry& operator<<(const char& arg) = 0;
    virtual iLoggerEntry& operator<<(const int& arg) = 0;
    virtual iLoggerEntry& operator<<(const unsigned int& arg) = 0;
    virtual iLoggerEntry& operator<<(const long& arg) = 0;
    virtual iLoggerEntry& operator<<(const unsigned long& arg) = 0;
    virtual iLoggerEntry& operator<<(const float& arg) = 0;
    virtual iLoggerEntry& operator<<(const double& arg) = 0;
    virtual iLoggerEntry& operator<<(const long double& arg) = 0;
    virtual iLoggerEntry& operator<<(const bool& arg) = 0;
  };
  
  /**
   * Interface for objects that provide logging feature.
   */
  class iLogger{
  public:
    virtual ~iLogger(){};

    virtual void append(const iLoggerEntry& entry) = 0;  
    virtual void flush() = 0;
    virtual void msg(const iLoggerEntry& entry) = 0;
    
    virtual unsigned int getLevel() const = 0;    
    virtual void setLevel(unsigned int level) = 0;
  };
}

#endif //ILOGGER_HEADER
