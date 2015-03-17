#include <fstream>
#include <stdexcept>

#include <QFile>
#include <QDebug>

#include "lines_counter.h"

/*
 * Strategy lines counter functions
 */
t_linesQnty f_countLines_STL(std::string &filename)
{
    t_linesQnty lines = 0;
    std::ifstream in( filename.c_str() );
    if (!in.is_open())
        throw std::runtime_error( std::string("f_countLines_STL()\nCannot open file: ") + filename );
    std::string str;
    while (std::getline(in, str)) { ++lines; }
    in.close();
    return lines;
}

t_linesQnty f_countLines_Qt(std::string &filename)
{
    t_linesQnty lines = 0;
    QFile file(QString::fromStdString(filename));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error( std::string("f_countLines_Qt()\nCannot open file: ") + filename );
    while (!file.atEnd()) { file.readLine(); ++lines; }
    file.close();
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

void LinesCounter::setFileName(const std::string &fileName)
{
    if (fileName.empty())
        throw std::runtime_error( std::string("LinesCounter::setFileName()\nFile name is empty") );
    m_filePathName = fileName;
}

std::string LinesCounter::fileName() const
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
