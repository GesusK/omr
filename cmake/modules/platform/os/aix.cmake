##############################################################################
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

include(OmrAssert)

omr_assert(TEST DEFINED OMR_ARCH_POWER MESSAGE "OMR_HOST_OS configured as AIX but OMR_ARCH_POWER is not defined")

set(OMR_PLATFORM_DEFINITIONS
	-DRS6000
	-DAIXPPC
	-D_LARGE_FILES
	-D_ALL_SOURCE
)
