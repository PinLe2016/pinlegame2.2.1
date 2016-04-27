//
//  IFile.cpp
//  framework
//
//  Created by Kennedy on 15/5/28.
//
//

#include "IFile.h"



bool IFile::isFileExist(const string& filename)
{
    return FileUtils::getInstance()->isFileExist(filename);
}

bool IFile::isFileExist_Writable(const string& filename)
{
    string path = FileUtils::getInstance()->getWritablePath() + filename;
    return FileUtils::getInstance()->isFileExist(path);
}


bool IFile::writeFile(const string& filename, const string& content)
{
    string wirtePath = FileUtils::getInstance()->getWritablePath() + filename;
    FILE* openFile	 = fopen(wirtePath.c_str(),"wb+");	//打开文件 有即使用 无即创建
    if(openFile)
    {
        fwrite(content.c_str(), content.size(), 1, openFile);
        fclose(openFile);
        return true;
    }
    return false;
}
//将本地图片写入到沙盒内（sSurName－本地图片路径，sDirName－保存沙盒路径）
bool IFile::shareLocalSprite(const string& sSurName, const string& sDirName)
{
    if(!IFile::isFileExist_Writable(sDirName))
    {
        ssize_t size = 0;
        unsigned char* titlech = FileUtils::getInstance()->getFileData(sSurName, "r", &size);
        if(size > 0)
        {
            std::string load_str = std::string((const char*)titlech, size);
            IFile::writeFile(sDirName, load_str);
            return true;
        }
        return false;
    }
    return true;
}

const char* IFile::readFile(const string& filename)
{
    string wirtePath = FileUtils::getInstance()->getWritablePath() + filename;
    __String * m_pString = __String::createWithContentsOfFile(wirtePath);
    return m_pString->getCString();
}



bool IFile::createDirectory(const string& pDirName)
{
    if(pDirName.empty())
        return false;
    string _downloadPath = FileUtils::getInstance()->getWritablePath();
    _downloadPath.append(pDirName);
    _downloadPath += "/";
    FileUtils::getInstance()->createDirectory(_downloadPath.c_str());
    return true;
}

//FileUtils::getInstance()->removeFile(const std::string &filepath)


bool IFile::removeFile(const string& filepath)
{
    if(filepath.empty())
        return false;
    return FileUtils::getInstance()->removeFile(filepath);
}


void IFile::writeStringToFile(const string& str, const string& filename)
{
    FILE* openFile	 = fopen(filename.c_str(),"wb+");	//打开文件 有即使用 无即创建
    if(openFile)
    {
        fwrite(str.c_str(), str.size(), 1, openFile);
        fclose(openFile);
    }
    
    return;
}



