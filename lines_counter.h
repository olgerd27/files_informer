#ifndef LINES_COUNTER_H
#define LINES_COUNTER_H

#include <string>
#include <qglobal.h>

typedef qlonglong t_linesCount;

class LinesCounter
{
public:
    LinesCounter();
    ~LinesCounter();

    /* maybe need to use the "File" class, that stored all needing information about file instead of std::string
     * or use both
     */
    void setFileName(const std::string &fileName);
    std::string fileName() const;

    t_linesCount countLines();

private:
    std::string m_filePathName;
    t_linesCount m_linesCount;
};

#endif // LINES_COUNTER_H
