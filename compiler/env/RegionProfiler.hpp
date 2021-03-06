/*******************************************************************************
   *
   * (c) Copyright IBM Corp. 2017, 2017
   *
   *  This program and the accompanying materials are made available
   *  under the terms of the Eclipse Public License v1.0 and
   *  Apache License v2.0 which accompanies this distribution.
   *
   *      The Eclipse Public License is available at
   *      http://www.eclipse.org/legal/epl-v10.html
   *
   *      The Apache License v2.0 is available at
   *      http://www.opensource.org/licenses/apache2.0.php
   *
   * Contributors:
   *    Multiple authors (IBM Corp.) - initial implementation and documentation
   ******************************************************************************/

#ifndef OMR_REGION_PROFILER_HPP
#define OMR_REGION_PROFILER_HPP

#pragma once

#include "env/Region.hpp"
#include "env/SegmentProvider.hpp"
#include "compile/Compilation.hpp"

namespace TR {

/**
 * @class
 * @brief The RegionProfiler class provides a mechanism for profiling between
 * two points of execution determined lexically by the lifetime of the profiler
 * object.
 *
 * This class makes use of the compiler's debug counter facility to record the
 * difference in memory usage for a region and its segment provider between the
 * two points of execution determined by the invocation of its constructor and
 * the invocation of its destructor. The lifetime of the region tracked by the
 * profiler object must comprehend the lifetime of the profiler itself. The
 * implementation requires a compilation object in order to determine whether
 * or not the facility is active.
 */

class RegionProfiler
   {
public:
   RegionProfiler(TR::Region &region, TR::Compilation &compilation, const char *identifier) :
      _region(region),
      _initialRegionSize(_region.bytesAllocated()),
      _initialSegmentProviderSize(_region._segmentProvider.bytesAllocated()),
      _compilation(compilation),
      _identifier(identifier ? identifier : "[ANONYMOUS]")
      {
      }

   ~RegionProfiler()
      {
      if (_compilation.getOption(TR_ProfileMemoryRegions))
         {
         TR::DebugCounter::incStaticDebugCounter(
            &_compilation,
            TR::DebugCounter::debugCounterName(
               &_compilation,
               "kbytesAllocated.details/%s/%s",
               _identifier,
               _compilation.getHotnessName(_compilation.getMethodHotness())
               ),
            (_region.bytesAllocated() - _initialRegionSize) / 1024
            );
         TR::DebugCounter::incStaticDebugCounter(
            &_compilation,
            TR::DebugCounter::debugCounterName(
               &_compilation,
               "segmentAllocation.details/%s/%s",
                _identifier,
                _compilation.getHotnessName(_compilation.getMethodHotness())
                ),
            (_region._segmentProvider.bytesAllocated() - _initialSegmentProviderSize) / 1024
            );
         }
      }

private:
   TR::Region &_region;
   size_t const _initialRegionSize;
   size_t const _initialSegmentProviderSize;
   TR::Compilation &_compilation;
   const char * const _identifier;
   };

}

#endif
