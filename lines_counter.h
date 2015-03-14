#ifndef LINES_COUNTER_H
#define LINES_COUNTER_H

#include <string>
#include <tr1/functional>
#include "common_defines.h"

typedef std::string T_strFileName;

t_linesQnty countLines_STL(T_strFileName &filename);
t_linesQnty countLines_Qt(T_strFileName &filename);

class LinesCounter
{
public:
    typedef std::tr1::function<t_linesQnty (T_strFileName &)> T_funcLinesCount;

    LinesCounter(T_funcLinesCount func);
    ~LinesCounter();

    void setFileName(const T_strFileName &fileName);
    T_strFileName fileName() const;

    void setFuncLinesCount(T_funcLinesCount func);
    t_linesQnty countLines();

private:
    T_strFileName m_filePathName;
    t_linesQnty m_linesCount;
    T_funcLinesCount m_funcCountLines;
};

#endif // LINES_COUNTER_H
