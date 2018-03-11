//
//  vnFileManager.cpp
//  vnlib
//
//  Created by Wayne on 5/12/14.
//  Copyright (c) 2014 viichi.com. All rights reserved.
//

#include "vnFileManager.h"

#include <algorithm>

_vn_begin

FileManager::FileManager() {
	m_systems[0] = vnnew FileSystemOS();
}

FileStream * FileManager::open(const vn::FilePath &path, bool readOnly) {
    auto it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->open(path.fileName, readOnly);
	}
	return 0;
}

FileStream * FileManager::create(const vn::FilePath &path, bool writeOnly) {
    auto it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->create(path.fileName, writeOnly);
	}
	return 0;
}

bool FileManager::exist(const FilePath &path) {
    auto it = m_systems.find(path.fsId);
    if (it != m_systems.end()) {
        return it->second->exist(path.fileName);
    }
    return false;
}

bool FileManager::remove(const FilePath &path) {
	auto it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->remove(path.fileName);
	}
	return false;
}

bool FileManager::list(const FilePath &path, std::list<str8> &result) {
	auto it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->list(path.fileName, result);
	}
	return false;
}

bool FileManager::createDirectory(const FilePath &path) {
	auto it = m_systems.find(path.fsId);
	if (it != m_systems.end()) {
		return it->second->createDirectory(path.fileName);
	}
	return false;
}

bool FileManager::removeDirectory(const FilePath &path) {
    auto it = m_systems.find(path.fsId);
    if (it != m_systems.end()) {
        return it->second->removeDirectory(path.fileName);
    }
    return false;
}


FileSystemPtr FileManager::findFileSystem(u32 fsId) {
	auto it = m_systems.find(fsId);
	if (it != m_systems.end()) {
		return it->second;
	}
	return nullptr;
}

void FileManager::setFileSystem(u32 fsId, const FileSystemPtr &fs) {
	if (fs) {
		m_systems[fsId] = fs;
	} else {
        m_systems.erase(fsId);
    }
}

void FileManager::setFileSystem(u32 fsId, FileSystemPtr &&fs) {
	if (fs) {
        m_systems[fsId] = std::move(fs);
	} else {
        m_systems.erase(fsId);
    }
}

std::vector<u32> FileManager::listFileSystemId() {
	std::vector<u32> result;
    result.reserve(m_systems.size());
	for (auto &i : m_systems) {
		result.push_back(i.first);
	}
    std::sort(result.begin(), result.end());
    return std::move(result);
}


_vn_end
