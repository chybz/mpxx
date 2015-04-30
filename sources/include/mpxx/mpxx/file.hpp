#ifndef __MPXX_FILE_H__
#define __MPXX_FILE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

#include <ios>
#include <string>
#include <fstream>
#include <stdexcept>

#include <mpxx/msgpack.hpp>

namespace mpxx {

template <typename T>
class file
{
public:
    bool load(T& v, const std::string& path) const
    {
        struct stat st;

        if (!exists(path, st)) {
            return false;
        }

        std::size_t sz = st.st_size;
        msgpack::sbuffer sbuf(sz);

        slurp(path, sbuf.data(), sz);

        msgpack::unpacked u;
        msgpack::unpack(&u, sbuf.data(), sz);

        msgpack::object obj = u.get();
        obj.convert(&v);

        return true;
    }

    void save(const T& v, const std::string& path) const
    {
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, v);

        expel(path, sbuf.data(), sbuf.size());
    }

private:
    bool exists(const std::string& path, struct stat& st) const
    {
        bool ret = false;

        if (stat(path.c_str(), &st) == 0) {
            ret = S_ISREG(st.st_mode);
        }

        return ret;
    }

    void slurp(
        const std::string& path,
        char* buffer, std::size_t size
    ) const
    {
        std::ifstream ifs(path, std::ios::binary);

        if (!ifs.is_open()) {
            throw std::runtime_error(
                "failed to open " + path
            );
        }

        ifs.read(buffer, size);
        ifs.close();
    }

    void expel(
        const std::string& path,
        const char* buffer, std::size_t size
    ) const
    {
        struct stat st;
        auto old = path + ".old";

        bool here = exists(path, st);

        if (here) {
            if (rename(path.c_str(), old.c_str()) != 0) {
                throw std::runtime_error(
                    "failed to rename " + path + " to " + old
                );
            }
        }

        std::ofstream ofs(path, std::ios::binary | std::ios::trunc);

        try {
            if (!ofs.is_open()) {
                throw std::runtime_error(
                    "failed to open " + path
                );
            }

            ofs.write(buffer, size);
            ofs.close();
        } catch (...) {
            unlink(path.c_str());

            if (here) {
                if (rename(old.c_str(), path.c_str()) != 0) {
                    throw std::runtime_error(
                        "WHOAA: failed to roolback "
                        + old + " to " + path
                    );
                }
            }

            throw;
        }
    }
};

} // namespace mpxx

#endif // __MPXX_FILE_H__
