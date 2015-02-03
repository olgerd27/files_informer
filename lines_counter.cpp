#include <fstream>
#include <stdexcept>
#include <QDebug>
#include "lines_counter.h"

LinesCounter::LinesCounter()
    : m_linesCount(0)
{
}

LinesCounter::~LinesCounter()
{
}

void LinesCounter::setFileName(const std::string &fileName)
{
    if (fileName.empty())
        throw std::runtime_error( std::string("LinesCounter::setFileName()\nbad file name: ") + fileName );
    m_filePathName = fileName;
}

std::string LinesCounter::fileName() const
{
    return m_filePathName;
}

t_linesCount LinesCounter::countLines()
{
    std::ifstream in(m_filePathName.c_str());
    if (!in.is_open())
        throw std::runtime_error( std::string("LinesCounter::countLines()\ncannot open file: ") + m_filePathName );
    std::string str;
    t_linesCount lines = 0L;
    while (std::getline(in, str)) { ++lines; }
    in.close();
    return lines;
}
