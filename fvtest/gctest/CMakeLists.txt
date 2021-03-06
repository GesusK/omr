###############################################################################
#
# (c) Copyright IBM Corp. 2017
#
#  This program and the accompanying materials are made available
#  under the terms of the Eclipse Public License v1.0 and
#  Apache License v2.0 which accompanies this distribution.
#
#      The Eclipse Public License is available at
#      http://www.eclipse.org/legal/epl-v10.html
#
#      The Apache License v2.0 is available at
#      http://www.opensource.org/licenses/apache2.0.php
#
# Contributors:
#    Multiple authors (IBM Corp.) - initial implementation and documentation
###############################################################################

add_executable(omrgctest
	GCConfigObjectTable.cpp
	GCConfigTest.cpp
	gcTestHelpers.cpp
	main.cpp
	StartupManagerTestExample.cpp
)

#TODO this is a real gross, tangled mess
target_link_libraries(omrgctest
	omrGtestGlue
	pugixml
	omrtestutil
	omrcore
	${OMR_HOOK_LIB}
	${OMR_TRACE_LIB}
	${OMR_GC_LIB}
	${OMR_THREAD_LIB}
	${OMR_PORT_LIB}
)

# TODO check linker flags from makefile are ported properly.
# Relevant section from makefile below
#ifeq (linux,$(OMR_HOST_OS))
  #MODULE_SHARED_LIBS += rt pthread
#endif
#ifeq (osx,$(OMR_HOST_OS))
  #MODULE_SHARED_LIBS += iconv pthread
#endif
#ifeq (aix,$(OMR_HOST_OS))
  #MODULE_SHARED_LIBS += iconv perfstat
#endif
#ifeq (win,$(OMR_HOST_OS))
  #MODULE_SHARED_LIBS += ws2_32 shell32 Iphlpapi psapi pdh
#endif

if(OMR_HOST_OS STREQUAL "zos")
	target_link_libraries(omrgctest j9a2e)
endif()


add_test(NAME gctest
	COMMAND omrgctest "--gtest_filter=gcFunctionalTest*"
	WORKING_DIRECTORY "${omr_SOURCE_DIR}")
