# Symbols, Symbol References, and Aliasing in Testarossa

## Introduction

This document explains and documents the concepts and implementation of symbols,
symbol references, and aliasing in Testarossa.

## Symbols and Symbol References

The original, conceptual distinction between symbols and symbol references (symrefs)
was actually quite straightforward. However, over time, implementation details have
led to the definition of the two to become very blurry and somewhat unspecified.
This document therefore offers the original definitions, as well as some implementation
details and subtleties.

### Symbols

A symbol represents a memory location. It holds the minimum information required to
directly translate a method's source code or IL representation, without consideration
for optimization. It stores basic information (e.g. data type) needed to determine
what memory access operations can/must be done.

Different kinds of symbols exist to represent the different kinds of memory access
requirements. Each kind of symbol stores information that is specific to its kind (type).
The different symbol kinds are:

* static
   * represents a static memory address
   * contains the address of the memory location
* auto
   * represents a variable mapped on the stack
      * may come from local method variables or compiler allocated stack objects
   * contains the stack offset of the variable
* method
   * represents a method (static, virtual)
* resolved method
* parameter
* register mapped
* label
* shadow
   * represents indirection off a base address
      * e.g. accessing a class field using the class base address plus an offset
   * contains offset from some base address

#### int shadow

An int shadow represents the common case of an integer type. In the case of Java,
for example, an int shadow is created for a field or array access.

#### generic int shadow

A generic int shadow is created by the optimizer in some (relatively rare) cases to
refer to a field that wasn't referred to in the bytecodes/source. If the optimizer wants
to synthesize a load or store, but doesn't have a symbol for it in the original program,
it can create one and call it a generic int shadow.

For example, an escape analysis optimization might want to generate a store to a location
within an object that hasn't been read in this method in order to initialize the location
to zero.

A Generic int shadow has different aliasing depending on the context where it is created
(i.e. it could alias different kinds of other shadows).

### Symbol References

A symbol reference is (or is supposed to be, rather) a wrapper around the different kinds
of symbols. It encapsulates the additional information required by an optimizing compiler,
including:

* aliasing
* owning method (method who's IL created the symbol, which may be different from the
  current method because of inlining)

All symrefs for a given compilation are stored in the "symbol reference table" (symreftab).

Symrefs are numbered, and sets of symrefs are almost always represented as bit vectors where
each bit specifies whether a corresponding symref is a member of the set or not.

## Aliasing

### Basic concept

Aliasing information is associated with symrefs. However, the structural definition of
aliasing used in Testarossa differs from the classical definition of aliasing. Specifically,
it also is used as an indicator of side-effects, rather than to mean that multiple names are
used to access the same memory. For example, a method symref may alias a static symref,
signifying that a call to the method kills the value of the static.

### Asymmetric aliasing

This alternative definition of aliasing permits aliasing relationships to be asymmetric.
To illustrate using the above example, although a method may alias a static, the static
*does not* alias the method because killing the value of the static *does not* affect
the method itself (though it may affect the result returned by a call to the method).

### Low level implementation

At the lowest level, aliasing relations are represented using bit vectors. The bit
vector represents a set of symrefs that alias a particular symref.

The two main APIs for getting a set of aliased symrefs are
`TR::SymbolReference::getUseDefAliasesBV()` and `TR::SymbolReference::getUseonlyAliasesBV`.
When called on a particular symref, they return a bit vector specifying the
aliased symrefs.

The bit vector returned is constructed from various other, more specific bit vectors.
Each of these specifies the symrefs that alias a symref matching certain queries.
Mostly, these queries have to do with the kind of symbol wrapped by the symref. The
associated bit vectors themselves are stored in the `TR::AliasBuilder` class. For a
given symref, each query is successively performed. Every time a query returns a
positive result, the associated bit vector is "ORed" into the final alias bit vector.

### TR::AliasBuilder

The `TR::AliasBuilder` class is responsible for storing the bit vectors of all symrefs
that alias a symref matching a given query. When a new symref is created, its corresponding
bit must be set in all the bit vectors associated with queries that match the new symref.

These bit vectors are only present for primitive queries. Bit vectors for complex queries
are constructed by `TR:AliasBuilder` by "ORing" together the various bit vectors
associated with queries that compose the larger query.

### TR_AliasSetInterface

The `TR_AliasSetInterface` class is a wrapper around a bit vector representing aliased
symrefs. It provides various methods for performing common operations on these bit vectors.
[The CRTP](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) is used
to achieve static polymorphism.

One key interface method that *must* be overridden by sub-classes is `getTRAliases()`.
This is the method responsible for returning the bit vector of aliases.

It is worth noting that some of these operations are templated. So, theoretically, they
can be used on bit vector types different from `TR_BitVector`. However, the type is
required to satisfy the (unspecified) interface of `TR_BitVector`.

Another noteworthy point is that some of the operations rely on calls to functions in
the derived class, as is common with the CRTP. However, because of C++ lazy template
instantiation, these operations will only be materialized if they are called. Hence,
derived classes are not strictly required to define these called functions, as long
as the operations that require them are never called on an instance of this class.

There two classes that inherit from this base class, namely `TR_SymAliasSetInterface`
and `TR_NodeAliasSetInterface`.

### TR_SymAliasSetInterface

`TR_SymAliasSetInterface` acts as a wrapper around a symref and provides an API for
dealing with aliases of the symref. As should be expected, its `getTRAliases()`
method returns the set of aliases of the symref.

Being a class template, it has two specializations with different implementations
of `getTRAliases()`. The non-type template parameter is one of the two values of
the `TR_AliasSetType` enum: `useDefAliasSet` or `UseOnlyAliasSet`. 

### TR_NodeAliasSetInterface

The `TR_NodeAliasSetInterface` is a wrapper around a node (instance of `TR::Node`).
It provides an API for dealing with aliases of the symref *contained* by the node.
That is, its `getTRAliases()` method returns the set of aliases of the symref
contained by the node.

It too is a template class with two specializations having different implementations
of `getTRAliases()`. In this case, the non-type template parameter is one of the two
values of the `TR_NodeAliasSetType` enum: `mayUseAliasSet` or `mayKillAliasSet`.

It should be noted that this class mostly exists for historic reasons. Once upon a time,
aliasing could apply to instances of `TR::Node` as well as symrefs. However, since this
is no longer the case, the class is only used as a shortcut to access the aliases of the
contained symref.

