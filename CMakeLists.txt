cmake_minimum_required (VERSION 3.10)

project (cscrypto)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(libsodium_dir ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/libsodium/)

if(WIN32)
  if(${MSVC_TOOLSET_VERSION} MATCHES 142)
    set(libsodium_patch ${libsodium_dir}/../patches/msvc142_retargeting.patch)
  else()
    if(${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION} MATCHES 10.0.17763.0)
	  set(libsodium_patch ${libsodium_dir}/../patches/msvs141_retargeting17763.patch)
    else()
      set(libsodium_patch ${libsodium_dir}/../patches/msvs141_retargeting17134.patch)
    endif()
  endif()
  execute_process(COMMAND git apply --ignore-space-change ${libsodium_patch}
                  WORKING_DIRECTORY ${libsodium_dir})
				  
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(msbuild_property "/p:Configuration=Debug")
  else()
    set(msbuild_property "/p:Configuration=Release")
  endif()
  
  set(msbuild_parallel "/m")
  set(msbuild_platform "/p:platform=x64")
  set(proj_to_build "libsodium.sln")
  set(disable_post_build "/p:PostBuildEventUseInBuild=false")
  
  execute_process(COMMAND ${CMAKE_VS_MSBUILD_COMMAND} ${msbuild_parallel} ${proj_to_build} ${msbuild_property} ${msbuild_platform} ${disable_post_build}
				  WORKING_DIRECTORY ${libsodium_dir})
				  
  execute_process(COMMAND git apply --ignore-space-change ${libsodium_patch} -R
                  WORKING_DIRECTORY ${libsodium_dir})
else()

  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(libsodium_lib_dir ${libsodium_dir}/Build/Debug/x64/)
    if(EXISTS ${libsodium_lib_dir}/lib/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
      message(STATUS "Libsodium lib has already been built")
    else()
      execute_process(COMMAND bash ./autogen.sh
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND ./configure --prefix=${libsodium_lib_dir}
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make check
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make install 
                      WORKING_DIRECTORY ${libsodium_dir})
    endif()
  else()
    set(libsodium_lib_dir ${libsodium_dir}/Build/Release/x64/)
    if(EXISTS ${libsodium_lib_dir}/lib/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
      message(STATUS "Libsodium lib has already been built")
    else()
      execute_process(COMMAND bash ./autogen.sh
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND ./configure --prefix=${libsodium_lib_dir}
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make check
                      WORKING_DIRECTORY ${libsodium_dir})
      execute_process(COMMAND make install 
                      WORKING_DIRECTORY ${libsodium_dir})
    endif()
  endif()

    
endif()

if(WIN32)
  set(lb_lib_folder .)
else()
  set(lb_lib_folder lib)
endif()

add_library(libsodium STATIC IMPORTED GLOBAL)
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_RELEASE
             ${libsodium_dir}/Build/Release/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_RELWITHDEBINFO
             ${libsodium_dir}/Build/Release/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_DEBUG
             ${libsodium_dir}/Build/Debug/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})

set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_RELSPAM
             ${libsodium_dir}/Build/Release/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_RELMONITOR
             ${libsodium_dir}/Build/Release/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})
set_property(TARGET libsodium PROPERTY IMPORTED_LOCATION_RELWALLET
             ${libsodium_dir}/Build/Release/x64/${lb_lib_folder}/libsodium${CMAKE_STATIC_LIBRARY_SUFFIX})


add_library(${PROJECT_NAME} STATIC
  include/cscrypto/cscrypto.hpp
  include/cscrypto/cryptotypes.hpp
  include/cscrypto/memoryprotection.hpp
  include/cscrypto/privatekey.hpp
  include/cscrypto/cryptoconstants.hpp
  include/cscrypto/maincryptofunctions.hpp
  src/maincryptofunctions.cpp
  src/privatekey.cpp
  )

target_link_libraries (${PROJECT_NAME} blake2 libsodium)
target_compile_definitions(${PROJECT_NAME} PUBLIC SODIUM_STATIC)
if(WIN32)
  target_compile_options(${PROJECT_NAME} PUBLIC /wd4324)
endif()
  
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/blake2/sse
	${CMAKE_CURRENT_SOURCE_DIR}/../third-party/libsodium/src/libsodium/include)