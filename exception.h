#pragma once
#include <exception>
#include <string>
enum except_type { MemoryAllocateError, IndexOutOfRange, SizeBelowZero, NegativeRange, 
	IncorrectComparator, IncorrectRange, IncorrectValue, IncorrectInputFormat, IncorrectIterator, 
	NoRequiredArgument, UnknownFlag, EmptySet, UnknownCommand, MutuallyExclusive, KeyNotExist, EmptySequence, NoSuchElement
};
#include<iostream>

class SetException {
//private:
	//except_type id;
public:
	except_type id;

	SetException(except_type id) { this->id = id; }
	const char* message() {
		if (id == MemoryAllocateError) return "Cannot allocate memory";
		if (id == IndexOutOfRange) return "Subscirpt index out of range";
		if (id == SizeBelowZero) return "Size below zero";
		if (id == NegativeRange) return "Range is less than zero";
		if (id == IncorrectComparator) return "Incorrect sorting comparator";
		if (id == IncorrectRange) return "The size of sequence isn't a power of 2";
		if (id == IncorrectValue) return "Not allowed parameter value";
		if (id == NoRequiredArgument) return "No required argument for command";
		if (id == UnknownFlag) return "Unknown flag name for this command";
		if (id == UnknownCommand) return "Unknown command name,gandon";
		if (id == IncorrectInputFormat) return "Incorrect Input format";
		if (id == MutuallyExclusive) return "There are two or more mutually exclusive arguments";
		if (id == KeyNotExist) return "Key is not in the tree now";
		if (id == EmptySequence) return "Your sequence is empty";
		if (id == NoSuchElement) return "No such element in RBTree";
		if (id == EmptySet) return "No such element in ISet";


		
	}
};
