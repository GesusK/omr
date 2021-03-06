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

#ifndef NAMESPACEUDT_HPP
#define NAMESPACEUDT_HPP

#include "ddr/config.hpp"

#include "ddr/std/string.hpp"

#include "ddr/ir/Macro.hpp"
#include "ddr/ir/UDT.hpp"

class NamespaceUDT : public UDT
{
public:
	std::vector<UDT *> _subUDTs;
	std::vector<Macro> _macros;

	NamespaceUDT(unsigned int lineNumber = 0);
	~NamespaceUDT();

	virtual DDR_RC acceptVisitor(TypeVisitor const &visitor);
	virtual void checkDuplicate(Symbol_IR *ir);
	virtual string getSymbolKindName();
	virtual void computeFieldOffsets();
	virtual void addMacro(Macro *macro);
	virtual std::vector<UDT *> * getSubUDTS();
	virtual void renameFieldsAndMacros(FieldOverride fieldOverride, Type *replacementType);

	bool operator==(Type const & rhs) const;
	virtual bool compareToNamespace(NamespaceUDT const &) const;
};

#endif /* NAMESPACEUDT_HPP */
