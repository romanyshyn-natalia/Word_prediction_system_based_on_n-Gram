#include "my_archive.h"
#include <archive_entry.h>
#include <archive.h>

void Archive::initFromMemoryImpl(const std::string &data) const {
    if (archive_read_open_memory(arc, &data[0], data.size()) != ARCHIVE_OK) {
        std::string lib_err_msg{archive_error_string(arc)};
        throw ArchiveErrorFile{"Error while opening archive"};
    }
}

[[maybe_unused]] uint64_t Archive::getFileSizeImpl() const {
    if (archive_entry_size_is_set(entry) == 0) {
        throw ArchiveErrorFile{"File size is not set"};
    }
    return archive_entry_size(entry);
}

std::string Archive::getFileNameImpl() const {
    return archive_entry_pathname(entry);
}

bool Archive::nextFileImpl() {
    res = archive_read_next_header(arc, &entry);
    if (res == ARCHIVE_EOF) {
        return false;
    }
    if (res != ARCHIVE_OK) {
        std::string lib_err_msg{archive_error_string(arc)};
        throw ArchiveErrorFile{"Error going to next file" + lib_err_msg};
    }
    return true;
}

std::string Archive::getWholeFileImpl() const {
    auto file_size = archive_entry_size(entry);
    std::string data(file_size, char{});

    la_ssize_t read_size = archive_read_data(arc, &data[0], data.size());
    if (read_size != file_size) {
        std::string lib_err_msg{archive_error_string(arc)};
        throw ArchiveErrorReadFile{"Error reading file:" + lib_err_msg};
    }
    return data;
}

Archive::Archive() {
    arc = archive_read_new();
    archive_read_support_filter_all(arc);
    archive_read_support_format_all(arc);
}

Archive::~Archive() {
    archive_read_free(arc);
}