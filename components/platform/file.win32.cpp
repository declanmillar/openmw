#include "platform.internal.hpp"
#include "file.hpp"

#if PLATFORM_TYPE == PLATFORM_TYPE_WIN32

#include <components/windows.hpp>
#include <string>
#include <boost/locale.hpp>

namespace Platform::File {

    static auto getNativeHandle(Handle handle)
    {
        return reinterpret_cast<HANDLE>(static_cast<intptr_t>(handle));
    }

    static int getNativeSeekType(SeekType seek)
    {
        if (seek == SeekType::Begin)
            return FILE_BEGIN;
        if (seek == SeekType::Current)
            return FILE_CURRENT;
        if (seek == SeekType::End)
            return FILE_END;
        return -1;
    }

    Handle open(const char* filename)
    {
        std::wstring wname = boost::locale::conv::utf_to_utf<wchar_t>(filename);
        HANDLE handle = CreateFileW(wname.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        if (handle == INVALID_HANDLE_VALUE)
        {
            std::ostringstream os;
            os << "Failed to open '" << filename << "' for reading: " << GetLastError();
            throw std::runtime_error(os.str());
        }
        return static_cast<Handle>(reinterpret_cast<intptr_t>(handle));
    }

    void close(Handle handle)
    {
        auto nativeHandle = getNativeHandle(handle);
        CloseHandle(nativeHandle);
    }

    void seek(Handle handle, size_t position, SeekType type /*= SeekType::Begin*/)
    {
        const auto nativeHandle = getNativeHandle(handle);
        const auto nativeSeekType = getNativeSeekType(type);

        if (SetFilePointer(nativeHandle, static_cast<LONG>(position), nullptr, nativeSeekType) == INVALID_SET_FILE_POINTER)
        {
            if (auto errCode = GetLastError(); errCode != ERROR_SUCCESS)
            {
                throw std::runtime_error("An fseek() call failed: " + std::to_string(errCode));
            }
        }
    }

    size_t size(Handle handle)
    {
        auto nativeHandle = getNativeHandle(handle);

        assert(isValidHandle(mHandle));

        BY_HANDLE_FILE_INFORMATION info;

        if (!GetFileInformationByHandle(nativeHandle, &info))
            throw std::runtime_error("A query operation on a file failed.");

        if (info.nFileSizeHigh != 0)
            throw std::runtime_error("Files greater that 4GB are not supported.");

        return info.nFileSizeLow;
    }

    size_t tell(Handle handle)
    {
        auto nativeHandle = getNativeHandle(handle);

        DWORD value = SetFilePointer(nativeHandle, 0, nullptr, SEEK_CUR);
        if (value == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
            throw std::runtime_error("A query operation on a file failed.");

        return value;
    }

    size_t read(Handle handle, void* data, size_t size)
    {
        auto nativeHandle = getNativeHandle(handle);

        DWORD bytesRead{};

        if (!ReadFile(nativeHandle, data, static_cast<DWORD>(size), &bytesRead, nullptr))
            throw std::runtime_error("A read operation on a file failed: " + std::to_string(GetLastError()));

        return bytesRead;
    }
}

#endif
