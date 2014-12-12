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
    if(fileName.empty())
        throw std::runtime_error(std::string("LinesCounter: bad file name: ") + fileName);
    else
        m_filePathName = fileName;
}

std::string LinesCounter::fileName() const
{
    return m_filePathName;
}

t_linesCount LinesCounter::getLinesQuantity()
{
    std::ifstream in(m_filePathName.c_str());
    if(!in.is_open())
        throw std::runtime_error(std::string("LinesCounter: cannot open file: ") + m_filePathName);
    std::string str;
    t_linesCount lines = 0;
    while(std::getline(in, str)) { ++lines; }
    in.close();
    return lines;
}
