#ifndef LINES_COUNTER_H
#define LINES_COUNTER_H

#include <string>
#include <tr1/functional>
#include "common_defines.h"

t_linesQnty fCountLines_STL(std::string &filename);
t_linesQnty fCountLines_Qt(std::string &filename);

class LinesCounter
{
public:
    typedef std::tr1::function<t_linesQnty (std::string &)> T_funcLinesCount;

    LinesCounter(T_funcLinesCount func);
    ~LinesCounter();

    void setFileName(const std::string &fileName);
    std::string fileName() const;

    void setFuncLinesCount(T_funcLinesCount func);
    t_linesQnty countLines();

private:
    std::string m_filePathName;
    t_linesQnty m_linesCount;
    T_funcLinesCount m_funcCountLines;
};

#endif // LINES_COUNTER_H
