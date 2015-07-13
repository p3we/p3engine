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

#ifndef ILOGGER_HEADER
#define ILOGGER_HEADER

#include <ctime>
#include <string>

namespace gf
{
  /**
   * Interface for objects representing logger entry.
   */
  class ILoggerEntry{
  public:
	virtual ~ILoggerEntry(){};

    virtual const unsigned int& getType() const = 0;
    virtual void setType(unsigned int type) = 0;
    
    virtual const std::time_t& getTime() const = 0;
    virtual void setTime(time_t time) = 0;
    
    virtual const std::string& getContent() const = 0;
    virtual void setContent(const std::string& content) = 0;
    
	virtual ILoggerEntry& add(const char* arg) = 0;
    virtual void clear() = 0;
    
	virtual ILoggerEntry& operator<<(const char* arg) = 0;
	virtual ILoggerEntry& operator<<(const std::string& arg) = 0;
	virtual ILoggerEntry& operator<<(const char& arg) = 0;
	virtual ILoggerEntry& operator<<(const int& arg) = 0;
	virtual ILoggerEntry& operator<<(const unsigned int& arg) = 0;
	virtual ILoggerEntry& operator<<(const long& arg) = 0;
	virtual ILoggerEntry& operator<<(const unsigned long& arg) = 0;
	virtual ILoggerEntry& operator<<(const float& arg) = 0;
	virtual ILoggerEntry& operator<<(const double& arg) = 0;
	virtual ILoggerEntry& operator<<(const long double& arg) = 0;
	virtual ILoggerEntry& operator<<(const bool& arg) = 0;
  };
  
  /**
   * Interface for objects that provide logging feature.
   */
  class ILogger{
  public:
	virtual ~ILogger(){};

	virtual void append(const ILoggerEntry& entry) = 0;
    virtual void flush() = 0;
	virtual void msg(const ILoggerEntry& entry) = 0;
    
    virtual unsigned int getLevel() const = 0;    
    virtual void setLevel(unsigned int level) = 0;
  };
}

#endif //ILOGGER_HEADER
