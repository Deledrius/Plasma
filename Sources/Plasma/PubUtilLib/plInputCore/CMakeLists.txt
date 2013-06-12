include_directories("../../CoreLib")
include_directories("../../NucleusLib/inc")
include_directories("../../NucleusLib")
include_directories("../../PubUtilLib")

if(WIN32)
    include_directories(${DirectX_INCLUDE_DIR})
endif(WIN32)

set(plInputCore_SOURCES
    plAvatarInputInterface.cpp
    plDebugInputInterface.cpp
    plInputDevice.cpp
    plInputInterface.cpp
    plInputInterfaceMgr.cpp
    plInputManager.cpp
    plSceneInputInterface.cpp
    plTelescopeInputInterface.cpp
)

set(plDInputCore_SOURCES
    plDInputDevice.cpp
    plDInputManager.cpp
)

set(plInputCore_HEADERS
    plAvatarInputInterface.h
    plDebugInputInterface.h
    plInputCoreCreatable.h
    plInputDevice.h
    plInputInterface.h
    plInputInterfaceMgr.h
    plInputManager.h
    plSceneInputInterface.h
    plTelescopeInputInterface.h
)

set(plDInputCore_HEADERS
    plDInputDevice.h
    plDInputManager.h
)

if(WIN32)
    add_library(plInputCore STATIC ${plInputCore_SOURCES} ${plInputCore_HEADERS} ${plDInputCore_SOURCES} ${plDInputCore_HEADERS})
else()
    add_library(plInputCore STATIC ${plInputCore_SOURCES} ${plInputCore_HEADERS})
endif(WIN32)

source_group("Source Files" FILES ${plInputCore_SOURCES})
source_group("Header Files" FILES ${plInputCore_HEADERS})

if(WIN32)
    source_group("DX\\Source Files" FILES ${plDInputCore_SOURCES})
    source_group("DX\\Header Files" FILES ${plDInputCore_HEADERS})
endif()

