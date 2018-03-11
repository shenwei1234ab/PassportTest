//
//  vnFileSystemOS.hpp
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#ifndef vnlib_vnFileSystemOS_hpp
#define vnlib_vnFileSystemOS_hpp

#include "vnFileSystem.h"
#include "vnUnicode.h"
#include <Windows.h>
#include <stdio.h>

_vn_begin

namespace internal {
    
	class FileStreamOS : public FileStream {
	public:
		FileStreamOS(FILE *fp) : m_fp(fp) {}
		
		virtual ~FileStreamOS() {
			if (m_fp) {
				fclose(m_fp);
				m_fp = 0;
			}
		}
        
		virtual s64 size() {
			if (!m_fp) {
				return 0;
			}
			s64 pos = _ftelli64(m_fp);
			_fseeki64(m_fp, 0, SEEK_END);
			s64 ret = _ftelli64(m_fp);
			_fseeki64(m_fp, pos, SEEK_SET);
			return ret;
		}
        
		virtual void close() {
			delete this;
		}
        
		virtual size_t read(void *buffer, size_t size) {
			return fread(buffer, 1, size, m_fp);
		}
		virtual size_t write(const void *buffer, size_t size) {
			return fwrite(buffer, 1, size, m_fp);
		}
        
		virtual bool seek(s64 offset, SeekDir way) {
			int whence;
			switch (way) {
				case kBegin:
					whence = SEEK_SET;
					break;
				case kCurrent:
					whence = SEEK_CUR;
					break;
				case kEnd:
					whence = SEEK_END;
					break;
			}
			return _fseeki64(m_fp, offset, whence) == 0;
		}
        
		virtual s64 tell() {
			return _ftelli64(m_fp);
		}
	private:
		FILE *m_fp;
	};
}

FileSystemOS::FileSystemOS(const str8 &path)
: m_path(path) {
	if (!m_path.empty() && m_path.back() != '/') {
		m_path.push_back('/');
	}
}

FileSystemOS::FileSystemOS(str8 &&path)
: m_path(path) {
    if (!m_path.empty() && m_path.back() != '/') {
		m_path.push_back('/');
	}
}

FileStream * FileSystemOS::open(const str8 &file, bool readOnly) {
	const wchar_t *mode = (readOnly ? L"rb" : L"r+b");
	FILE *fp = 0;
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(file.c_str(), file.length(), name);
		fp = _wfsopen((const wchar_t *)name.c_str(), mode, _SH_DENYNO);
		if (!fp) {
			return 0;
		}
	} else {
		str8 name = m_path + file;
		str16 _name;
		Unicode::convert(name.c_str(), name.length(), _name);
		fp = _wfsopen((const wchar_t *)_name.c_str(), mode, _SH_DENYNO);
		if (!fp) {
			return 0;
		}
	}
	return vnnew internal::FileStreamOS(fp);
}

FileStream * FileSystemOS::create(const str8 &file, bool writeOnly) {
	const wchar_t *mode = (writeOnly ? L"wb" : L"w+b");
	FILE *fp = 0;
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(file.c_str(), file.length(), name);
		if (_wfopen_s(&fp, (const wchar_t *)name.c_str(), mode)) {
			return 0;
		}
	}
	else {
		str8 name = m_path + file;
		str16 _name;
		Unicode::convert(name.c_str(), name.length(), _name);
		if (_wfopen_s(&fp, (const wchar_t *)_name.c_str(), mode)) {
			return 0;
		}
	}
	return vnnew internal::FileStreamOS(fp);
}

bool FileSystemOS::exist(const str8 &path) {
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(path.c_str(), path.length(), name);
		return ::_waccess((const wchar_t *)name.c_str(), 0) == 0;
	}
	str8 name = m_path + path;
	str16 _name;
	Unicode::convert(name.c_str(), name.length(), _name);
	return ::_waccess((const wchar_t *)_name.c_str(), 0) == 0;
}

bool FileSystemOS::remove(const str8 &path) {
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(path, name);
		return ::_wremove((const wchar_t *)name.c_str()) == 0;
	}
	str8 name = m_path + path;
	str16 _name;
	Unicode::convert(name, _name);
	return ::_wremove((const wchar_t *)_name.c_str()) == 0;
}

bool FileSystemOS::createDirectory(const str8 &path) {
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(path, name);
		return ::CreateDirectoryW((const wchar_t *)name.c_str(), 0) == TRUE;
	}
	str8 name = m_path + path;
	str16 _name;
	Unicode::convert(name, _name);
	return ::CreateDirectoryW((const wchar_t *)_name.c_str(), 0) == TRUE;
}

bool FileSystemOS::removeDirectory(const str8 &path) {
	if (m_path.empty()) {
		str16 name;
		Unicode::convert(path, name);
		return ::_wrmdir((const wchar_t *)name.c_str()) == 0;
	}
	str8 name = m_path + path;
	str16 _name;
	Unicode::convert(name, _name);
	return ::_wrmdir((const wchar_t *)_name.c_str()) == 0;
}

bool FileSystemOS::list(const str8 &path, std::list<str8> &result) {
	str8 name = m_path + path;
	if (name.empty()) {
		name += "./*";
	} else if (name.back() != '/') {
		name += "/*";
	} else {
		name += '*';
	}
	str16 _name;
	Unicode::convert(name, _name);
	WIN32_FIND_DATAW fd;
	::HANDLE hFind = ::FindFirstFileW((const wchar_t *)_name.c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..")) {
				result.push_back(std::move(str8()));
				str8 &fn = result.back();
				Unicode::convert((c16 *)fd.cFileName, wcslen(fd.cFileName), fn);
				fn.push_back('/');
			}
		}
		else {
			result.push_back(std::move(str8()));
			Unicode::convert((c16 *)fd.cFileName, wcslen(fd.cFileName), result.back());
		}
	} while (::FindNextFileW(hFind, &fd) != FALSE);
    return true;
}

_vn_end

#endif
