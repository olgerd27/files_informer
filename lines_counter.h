#ifndef LINES_COUNTER_H
#define LINES_COUNTER_H

#include <string>
#include "common_defines.h"

class LinesCounter
{
public:
    LinesCounter();
    ~LinesCounter();

    void setFileName(const std::string &fileName);
    std::string fileName() const;

    t_linesQnty countLines();

private:
    std::string m_filePathName;
    t_linesQnty m_linesCount;
};

#endif // LINES_COUNTER_H
