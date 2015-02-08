#ifndef FILES_INFO_DATA_IMPL_H
#define FILES_INFO_DATA_IMPL_H

#include "common_defines.h"

class FilesInfoDataImpl
{
public:

    FilesInfoDataImpl()
        : m_files(0)
        , m_size(0)
        , m_lines(0)
        , m_density(0)
    {
    }

    FilesInfoDataImpl(int filesQnty, long size, long lines, float density)
        : m_files(filesQnty)
        , m_size(size)
        , m_lines(lines)
        , m_density(density)
    {
    }

    inline t_filesQnty filesQnty() const { return m_files; }
    inline void setFilesQnty(int v) { m_files = v; }

    inline t_filesSize filesSize() const { return m_size; }
    inline void setFilesSize(t_filesSize v) { m_size = v; }

    inline t_linesQnty filesLines() const { return m_lines; }
    inline void setFilesLines(t_linesQnty v) { m_lines = v; }

    inline t_density filesDensity() const { return m_density; }
    inline void setFilesDensity(t_density v) { m_density = v; }

private:
    t_filesQnty m_files;
    t_filesSize m_size;
    t_linesQnty m_lines;
    t_density   m_density;
};

#endif // FILES_INFO_DATA_IMPL_H
