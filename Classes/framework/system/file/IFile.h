//
//  IFile.h
//  framework
//
//  Created by Kennedy on 15/5/28.
//
//

#ifndef __framework__IFile__
#define __framework__IFile__



#include "cocos2d.h"

using namespace std;
using namespace cocos2d;


class IFile
{
public:
    static  void InitDirectory();
    static  bool isFileExist(const string& filename);
    static  bool isFileExist_Writable(const string& filename);
    static  bool writeFile(const string& filename, const string& content);
    static  const char* readFile(const string& filename);
    static  bool createDirectory(const string& pDirName);
    static  bool removeFile(const string& filepath);
    static  bool shareLocalSprite(const string& sSurName, const string& sDirName);
    
    static void writeStringToFile(const string& str, const string& filename);
    

};






#endif /* defined(__framework__IFile__) */
