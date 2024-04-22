//*****************************************************************************
// filesystem_ns.h          Helper for filesystem namespace. Makes it so that
//                          across GCC 7 --> GCC 8 we can still use stdc++fs
//
// Author: Bryan Womack
// Date: May 13, 2019
// Company: Echodyne Corp.
//
//*****************************************************************************

#ifndef FILESYSTEM_NAMESPACE_HELPER_H
#define FILESYSTEM_NAMESPACE_HELPER_H

// Determine the GNU C/C++ Compiler Features
// Define the namespace "fs" such that the rest of the code base does not have
// to worry about the differences
#ifdef __GNUC__
#   if __GNUC__ >= 8
    #include <filesystem>
    namespace fs = std::filesystem;
#	define HAVE_FILESYSTEM_NAMESPACE
#   elif (__GNUC__ == 7) && (__GNUC_MINOR__ >= 3)
    // TODO: Check that the namespace is right
    #include <experimental/filesystem>
#   define EXPERIMENTAL_FILESYSTEM_IMPL
    namespace fs = std::experimental::filesystem;
#	define HAVE_FILESYSTEM_NAMESPACE
#   else
    // Filesystem is not yet supported
#       error "GNU compiler version does not support std::filesystem!"
#   endif
#endif /* __GNUC__ */

// Check for visual studio
#ifdef _MSC_VER
	#include <filesystem>
	namespace fs = std::filesystem;
	#define HAVE_FILESYSTEM_NAMESPACE
#endif

#ifndef HAVE_FILESYSTEM_NAMESPACE
#error "Compiler not configured for std::filesystem!"
#endif

#endif /* FILESYSTEM_NAMESPACE_HELPER_H */

/* EOF */

