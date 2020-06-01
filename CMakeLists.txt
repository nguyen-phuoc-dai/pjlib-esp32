# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# add some necessary defnitions
add_definitions(
		-D__ESP32__
		-D__XTENSA__
		
		-DPJ_HAS_ASSERT_H
		-DPJ_HAS_SETJMP_H
		-DPJ_HAS_STRING_H
		-DPJ_HAS_CTYPE_H
		-DPJ_HAS_TIME_H
		-DPJ_HAS_SYS_TIME_H
		-DPJ_HAS_ERRNO_H
		-DPJ_HAS_SYS_TIMEB_H
		-DPJ_HAS_STDARG_H
		-DPJ_HAS_LIMITS_H
		-DPJ_HAS_SEMAPHORE_H
		-DPJ_HAS_FLOATING_POINT
		-DPJ_HAS_UNISTD_H
    )
    
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(pjproject)