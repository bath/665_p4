#ifndef HOLEYC_SYMBOL_TABLE_HPP
#define HOLEYC_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>

//Use an alias template so that we can use
// "HashMap" and it means "std::unordered_map"
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace holeyc{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols 
// exist for the lifetime of a scope in the 
// symbol table. 
class SemSymbol {
	//TODO add the fields that 
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields

	// what should each symantic symbol track?
		// probably:
			// function vs variable
			// type of id ? int, bool, char, etc?
			// scope ?!?! -- probably not needed?
			// clay had put in IDNode, do we need this? removing for now

	public: 
		SemSymbol(char k, string t) : kind(k), type(t){};
		char getKind()
		{
			return kind;
			// f or v
		}
		std::string getType() { return type; }
	private: 
		char kind;
		std::string type;
};

//A single scope. The symbol table is broken down into a 
// chain of scope tables, and each scope table holds 
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		bool isInCurrentScopeAlready(IDNode *id);
		bool isPresent(IDNode *id);
		void addSymbol(string idName, SemSymbol *s);
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		void addScope(ScopeTable *tab);
		void dropScope();
		ScopeTable* currentScope();
		bool lookUp(IDNode *id); //  -- ensure that what were calling exists & is defined
		bool isInCurrentScopeAlready(IDNode *id);
		bool isFnWrongType(IDNode *id);
		bool isVarWrongType(IDNode *id);
		bool isCorrectType(TypeNode *typeNode, char declType);
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc. 
	private:
		std::list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
