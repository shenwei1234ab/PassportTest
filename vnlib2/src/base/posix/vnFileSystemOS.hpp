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
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

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
			s64 pos = ftello(m_fp);
			fseeko(m_fp, 0, SEEK_END);
			s64 ret = ftello(m_fp);
			fseeko(m_fp, pos, SEEK_SET);
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
                default:
                    return false;
			}
			return fseeko(m_fp, offset, whence) == 0;
		}
        
		virtual s64 tell() {
			return ftello(m_fp);
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
	const char *mode = (readOnly ? "rb" : "r+b");
	FILE *fp = 0;
	if (m_path.empty()) {
		fp = fopen(file.c_str(), mode);
	} else {
		str8 name = m_path + file;
		fp = fopen(name.c_str(), mode);
	}
	if (!fp) {
		return 0;
	}
	return vnnew internal::FileStreamOS(fp);
}

FileStream * FileSystemOS::create(const str8 &file, bool writeOnly) {
	const char *mode = (writeOnly ? "wb" : "w+b");
	FILE *fp = 0;
	if (m_path.empty()) {
		fp = fopen(file.c_str(), mode);
	} else {
		str8 name = m_path + file;
		fp = fopen(name.c_str(), mode);
	}
	if (!fp) {
		return 0;
	}
	return vnnew internal::FileStreamOS(fp);
}

bool FileSystemOS::exist(const str8 &path) {
    if (m_path.empty()) {
        return ::access(path.c_str(), F_OK) == 0;
    }
    str8 name = m_path + path;
    return ::access(name.c_str(), F_OK) == 0;
}

bool FileSystemOS::remove(const str8 &path) {
	if (m_path.empty()) {
		return ::remove(path.c_str()) == 0;
	}
	str8 name = m_path + path;
	return ::remove(name.c_str()) == 0;
}

bool FileSystemOS::createDirectory(const str8 &path) {
	int mode = 0777;
	if (m_path.empty()) {
		return ::mkdir(path.c_str(), mode) == 0;
	}
	str8 name = m_path + path;
	return ::mkdir(name.c_str(), mode) == 0;
}

bool FileSystemOS::removeDirectory(const str8 &path) {
    if (m_path.empty()) {
        return ::rmdir(path.c_str()) == 0;
    }
    str8 name = m_path + path;
    return ::rmdir(name.c_str()) == 0;
}

bool FileSystemOS::list(const str8 &path, std::list<str8> &result) {
	str8 name = m_path + path;
	if (name.empty()) {
		name = "./";
	}
	DIR *dirp = opendir(name.c_str());
	
	if (!dirp) {
		return false;
	}
	result.clear();
	dirent *dp = readdir(dirp);
	while (dp) {
		if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
			str8 t = dp->d_name;
			if (dp->d_type == DT_DIR) {
				t.push_back('/');
                result.push_back(t);
			} else if (dp->d_type == DT_LNK) {
                str8 lnk = name;
                if (lnk.back() != '/') {
                    lnk.push_back('/');
                }
                lnk += t;
                struct stat info;
                if (!::stat(lnk.c_str(), &info)) {
                    if (S_ISDIR(info.st_mode)) {
                        t.push_back('/');
                        result.push_back(t);
                    } else if (S_ISREG(info.st_mode)) {
                        result.push_back(t);
                    }
                }
            } else if (dp->d_type == DT_REG) {
                result.push_back(t);
            }
		}
        dp = readdir(dirp);
    }
    closedir(dirp);
    return true;
}

_vn_end

#endif
