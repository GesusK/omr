/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2016, 2017
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
 *******************************************************************************/

#ifndef MODIFIERS_HPP
#define MODIFIERS_HPP

#include "ddr/std/string.hpp"
#include <vector>

using std::string;
using std::vector;

class Modifiers
{
public:
	vector<size_t> _arrayLengths;
	int _modifierFlags;
	size_t _offset;
	size_t _pointerCount;
	size_t _referenceCount;

	static const int MODIFIER_FLAGS = 31;
	static const int NO_MOD = 0;
	static const int CONST_TYPE = 1;
	static const int VOLATILE_TYPE = 2;
	static const int UNALIGNED_TYPE = 4;
	static const int RESTRICT_TYPE = 8;
	static const int SHARED_TYPE = 16;
	static const string MODIFIER_NAMES[];

	Modifiers();
	~Modifiers();

	string getPointerType();
	string getModifierNames();
	void addArrayDimension(size_t length);
	bool isArray();
	size_t getArrayLength(size_t i);
	size_t getArrayDimensions();
	size_t getSize(size_t typeSize);
	bool operator==(Modifiers const& type) const;
};

#endif /* MODIFIERS_HPP */
