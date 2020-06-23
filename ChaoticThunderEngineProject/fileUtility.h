#pragma once
#include <string>
#include <windows.h>
namespace FileUtility {
    const wchar_t pathSeperator = '\\'; //May vary between operations systems

    template<int maxCharBufferSize>
    std::wstring currentDirectory() {
        std::wstring tmp(maxCharBufferSize, 0);
        unsigned long len = GetCurrentDirectory(maxCharBufferSize, &tmp[0]);
        if (len == 0 || len >= maxCharBufferSize) { throw std::runtime_error("GetCurrentDirectory failed."); }
        tmp.resize(len);
        return tmp;
    }

    std::wstring getParent(std::wstring path) {
        size_t index = path.find_last_of(pathSeperator);
        return path.substr(0, index);
    }

    std::wstring combinePath(std::wstring path1, std::wstring path2) {
        return path1.append(&pathSeperator).append(path2);
    }

    std::wstring combinePath(std::wstring path1, std::string path2) {
        return path1.append(&pathSeperator).append(std::wstring(path2.begin(), path2.end()));
    }
}
