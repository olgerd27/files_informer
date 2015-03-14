#include <fstream>
#include <stdexcept>
#include <QDebug>
#include "lines_counter.h"

/*
 * Strategy lines counter functions
 */
t_linesQnty countLines_STL(T_strFileName &filename)
{
    t_linesQnty lines = 0;
    std::ifstream in( filename.c_str() );
    if (!in.is_open())
        throw std::runtime_error( std::string("countLines_STL()\nCannot open file: ") + filename.c_str() );
    std::string str;
    while (std::getline(in, str)) { ++lines; }
    in.close();
    return lines;
}

t_linesQnty countLines_Qt(T_strFileName &filename)
{
    t_linesQnty lines = 0;
    return lines;
}

/*
 * LinesCounter
 */
LinesCounter::LinesCounter(T_funcLinesCount func)
    : m_linesCount(0)
    , m_funcCountLines(func)
{
}

LinesCounter::~LinesCounter()
{
}

void LinesCounter::setFileName(const T_strFileName &fileName)
{
    if (fileName.empty())
        throw std::runtime_error( std::string("LinesCounter::setFileName()\nFile name is empty") );
    m_filePathName = fileName;
}

T_strFileName LinesCounter::fileName() const
{
    return m_filePathName;
}

void LinesCounter::setFuncLinesCount(LinesCounter::T_funcLinesCount func)
{
    m_funcCountLines = func;
}

t_linesQnty LinesCounter::countLines()
{
    return m_funcCountLines(m_filePathName);
}
