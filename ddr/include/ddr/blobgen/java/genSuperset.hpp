/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2017
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

#ifndef GENSUPERSET_HPP
#define GENSUPERSET_HPP

#include "ddr/config.hpp"

#include "ddr/std/sstream.hpp"
#include <stdio.h>
#include <string.h>

#include "ddr/ir/ClassType.hpp"
#include "ddr/ir/EnumMember.hpp"
#include "ddr/ir/Field.hpp"
#include "ddr/ir/Symbol_IR.hpp"
#include "ddr/std/unordered_map.hpp"

using std::string;
using std::stringstream;

class SupersetVisitor;

class JavaSupersetGenerator: public SupersetGenerator
{
private:
	set<string> _baseTypedefSet; /* Set of types renamed to "[U/I][SIZE]" */
	unordered_map<string, string> _baseTypedefMap; /* Types remapped for assembled type names. */
	unordered_map<string, string> _baseTypedefReplace; /* Type names which are replaced everywhere. */
	set<string> _baseTypedefIgnore; /* Set of types to not rename when found as a typedef */
	intptr_t _file;
	OMRPortLibrary *_portLibrary;
	bool _printEmptyTypes;

	void initBaseTypedefSet();
	void convertJ9BaseTypedef(Type *type, string *name);
	void replaceBaseTypedef(Type *type, string *name);
	DDR_RC getFieldType(Field *f, string *assembledTypeName, string *simpleTypeName);
	DDR_RC printFieldMember(Field *field, string prefix);
	void printConstantMember(string name);

	string replace(string str, string subStr, string newStr);

	friend class SupersetVisitor;
	friend class SupersetFieldVisitor;

public:
	JavaSupersetGenerator(bool printEmptyTypes);

	DDR_RC printSuperset(OMRPortLibrary *portLibrary, Symbol_IR *ir, const char *supersetFile);
};

#endif /* GENSUPERSET_HPP */
