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
  class CLoggerEntry : public ILoggerEntry{
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
    
	ILoggerEntry& add(const char* arg);
    void clear();
    
	ILoggerEntry& operator<<(const char* arg);
	ILoggerEntry& operator<<(const std::string& arg);
	ILoggerEntry& operator<<(const char& arg);
	ILoggerEntry& operator<<(const int& arg);
	ILoggerEntry& operator<<(const unsigned int& arg);
	ILoggerEntry& operator<<(const long& arg);
	ILoggerEntry& operator<<(const unsigned long& arg);
	ILoggerEntry& operator<<(const float& arg);
	ILoggerEntry& operator<<(const double& arg);
	ILoggerEntry& operator<<(const long double& arg);
	ILoggerEntry& operator<<(const bool& arg);
    
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
    
  class CLogger : public ILogger{
  public:  
    enum OUTPUT{
      OUTPUT_CONSOLE = 0,
      OUTPUT_FILE = 1,
      OUTPUT_ALL
    };
    
	CLogger(OUTPUT output = OUTPUT_ALL, const std::string& filename="default.log");
	virtual ~CLogger();
    
	void append(const ILoggerEntry& entry);
    void flush();
	void msg(const ILoggerEntry& entry);
    
    unsigned int getLevel() const;    
    void setLevel(unsigned int level);
    
  protected:
    void print_console(const std::string& str);
    void print_file(const std::string& str);
    
	std::queue<const ILoggerEntry*> m_buffer;
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
