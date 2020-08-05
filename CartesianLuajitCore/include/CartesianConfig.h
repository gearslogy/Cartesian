//
// Created by admin on 2020/7/31.
//

#ifndef CP_01_EXPORT_DLL_CARTESIANCONFIG_H
#define CP_01_EXPORT_DLL_CARTESIANCONFIG_H




#if defined _WIN32 || defined __CYGWIN__
    #ifdef CARTESIAN_BUILDING_DLL
        #define CARTESIAN_API _declspec(dllexport)
        #pragma message("[-----Windows Building DLL-------]" )
    #else
        #define CARTESIAN_API _declspec(dllimport)
        #pragma message("[-----Windows Importing DLL-------]" )
    #endif

#else
    #ifdef CARTESIAN_BUILDING_DLL
       #define CARTESIAN_API __attribute__ ((dllexport))
       #pragma message("[-----Linux Building DLL-------]" )
    #else
       #define CARTESIAN_API __attribute__ ((dllimport))
       #pragma message("[-----Linux Importing DLL-------]" )
    #endif
#endif

#if defined _WIN32 || defined __CYGWIN__
#define CARTESIAN_EXPORT _declspec(dllexport)
#else
#define CARTESIAN_EXPORT __attribute__ ((visibility("default")))
#endif









#endif //CP_01_EXPORT_DLL_CETERSIANCONFIG_H
