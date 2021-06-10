#ifndef WORD_COUNTING_ARCHIVE_H
#define WORD_COUNTING_ARCHIVE_H

#include <string>
#include <archive.h>
#include <archive_entry.h>
#include <stdexcept>

class ArchiveErrorFile : public std::runtime_error {
public:
    ArchiveErrorFile(const std::string &text) : std::runtime_error(text) {}
};

class ArchiveErrorReadFile : public ArchiveErrorFile {
public:
    ArchiveErrorReadFile(const std::string &text) : ArchiveErrorFile(text) {}
};

class Archive {
    archive_entry *entry{};
    int res{};
    struct archive *arc;
public:
    void initFromMemoryImpl(const std::string &data) const;

    std::string getWholeFileImpl() const;

    bool nextFileImpl();

    uint64_t getFileSizeImpl() const;

    std::string getFileNameImpl() const;

    Archive();

    ~Archive();
};

#endif //WORD_COUNTING_ARCHIVE_H
