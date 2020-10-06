#include "symbol_table.hpp"
namespace holeyc{

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>(); // unsure how to deal with the hash map when adding or finding ?
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.

	scopeTableChain = new std::list<ScopeTable *>();
}

// push a new table into the scopeTableChain
void SymbolTable::addScope(ScopeTable *tab){
	this->scopeTableChain->push_front(tab);
}

// drop the current scopetable 
void SymbolTable::dropScope(){
	// TODO : how to tell when we call dropScope();?
		// need to identify when we are exiting a scope
		// maybe at the end of every nameAnalysis() since they will be recursive?
	this->scopeTableChain->pop_front();
}

// within the current scope, is this id present?
bool ScopeTable::isPresent(IDNode *id)
{
	// TODO : not sure if this is correct
	bool isPresent = true;
	if ((this->symbols->find(id->getName())) == this->symbols->end()) {
		isPresent = false;
	}
	return isPresent;
}

// lookUp will check ALL open scopes for an identifier.
// use only for stmt and exps, we are actually USING the values, so make sure they exist in the first place
bool SymbolTable::lookUp(IDNode *id)
{
	// See if id has been declared in all open scopes
	// TODO -- i think this is correct
	bool isPresent = false;
	for(auto scopeT : *scopeTableChain){
		// scopeT is a ScopeTable
		isPresent = scopeT->isPresent(id); // what happens when there are two instances?
	}
	return isPresent;
}

// isInCurrentScopeAlready will check the CURRENT scope for an identifier. If found, print error in nameAnalysis caller. 
// Use only for decls -- var and fn
bool SymbolTable::isInCurrentScopeAlready(IDNode *id)
{
	// TODO -- i think this is correct
	bool isPresent = false;
	isPresent = this->currentScope()->isPresent(id);
	return isPresent;
}

bool SymbolTable::isWrongType(IDNode *id) { // should there be a version for fn decl and var decl? since var decl can't be void?
	// TODO -- could be some type that isn't defined (i.e. asdf a;)

	// check the type? var decl vs fndecl? how to do so?
	bool isCorrect = true;
	if(id->mySymbol->getKind() == "v"){
		return false;
	}
	return isCorrect;
}

ScopeTable* SymbolTable::currentScope(){
	return this->scopeTableChain.front();
}

// addSymbol will insert a new symbol into the current scope
void SymbolTable::addSymbol(SemSymbol *s){
	// TODO -- i think this is correct
	this->currentScope()->symbols.insert(s->IdToS(), s);
}









// not sure what were doing in here
// p4 starts by taking the root of the ast and walks over the whole thing	
// once we reach a child node, we get to our first element that needs to be added to the symbol table
// once we get to that child node, lets say a IDNode a, we pull the name out of it(the key) "a", it will be a pointer
// which points to  a new semantic symbol which has "the various fields for that guy" 

//  in the symbol table were building out 