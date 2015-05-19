//
//  FileUtilsIOS.m
//  redstarinternational
//
//  Created by Camellia_Mac on 15/2/12.
//
//

#import <Foundation/Foundation.h>
#include "FileUtilsIOS.h"



bool FileUtilsIOS::createDirectory(std::string path)
{
    NSString *nsPath = [NSString stringWithUTF8String:path.c_str()];
    return [[NSFileManager defaultManager] createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:nil error:nil];
}
bool FileUtilsIOS::createFile(std::string absFilepath)
{
    NSString *nsPath = [NSString stringWithUTF8String:absFilepath.c_str()];
    return [[NSFileManager defaultManager] createFileAtPath:nsPath contents:nil attributes:nil];
}