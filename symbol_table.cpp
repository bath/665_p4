#include "symbol_table.hpp"
namespace holeyc{

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>(); // unsure how to deal with the hash map when adding or finding ?
	// i think string is:
		// int my_id;
		// string == my_id
		// then why does SemSymbol have a char kind and IDNode* id; ? what do we need the SemSymbol in the map for? vice versa
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be 
	// entered into the front of the chain and upon exit the 
	// latest scope table will be removed from the front of 
	// the chain.
	// define addScope() ?

	// how to deal with popping things off from the chain?
	// when does this occur?

	scopeTableChain = new std::list<ScopeTable *>();
}

void SymbolTable::addScope(ScopeTable * tab){
	// TODO
	// push tab to front of scopeTableChain
	this->scopeTableChain->push_front(tab);
}

void SymbolTable::dropScope(){
	// TODO
	this->scopeTableChain->pop_front();
	// removing a scope when we get out of a scope
}

// within the current scope, returns whether or not the id is present
bool ScopeTable::isPresent(IDNode *id){
	// look at this-> symbols to see if the id is a match in the hashmap
	// not sure what to do with the hashmap's string component? what is it there for?
	// this->symbols.search(id); // something like this? search the hashmap for the id
}

// lookUp will check ALL open scopes for an identifier.
// Use only for stmts and expressions -- why do we care about if an expression lookUp == true? does clay mean the vars within?
bool SymbolTable::lookUp(IDNode *id)
{
	// See if id has been declared in all open scopes 
	bool isPresent = false;
	for(auto scopeT : *scopeTableChain){
		// scopeT is a ScopeTable
		isPresent = scopeT->isPresent(id); // what happens when there are two instances?
	}
	return isPresent;
}

// isInCurrentScopeAlready will check the CURRENT scope for an identifier. If found, print error in nameAnalysis caller. 
// Use only for decls
bool SymbolTable::isInCurrentScopeAlready(IDNode *id)
{
	// TODO
	// See if id has been declared in all open scopes
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
	// TODO
	this->currentScope()->symbols.insert(s->IdToS(), s); // what does string in hashMap do? i think its the name of the id
	// is there some kind of fuckery going on with IDNode having a 
	// SemSymbol as priv var and then SemSymbol having a IDNode as a priv var?
	// why do we need to have a doubly linked nodeid and semsymbol?
}









// not sure what were doing in here
// p4 starts by taking the root of the ast and walks over the whole thing	
// once we reach a child node, we get to our first element that needs to be added to the symbol table
// once we get to that child node, lets say a IDNode a, we pull the name out of it(the key) "a", it will be a pointer
// which points to  a new semantic symbol which has "the various fields for that guy" 

//  in the symbol table were building out 