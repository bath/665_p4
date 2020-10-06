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
	public: 
		SemSymbol(char kind, IDNode* id);
		std::string IdToS()
		{
			return id->getName();
		};
		char getKind() {
			return kind; 
			// b = bool, c == char, i == int, s == string, v == void
			// p sure there are more types -- deal with this later
		}
	private: 
		char kind;
		IDNode* id;
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
		bool isWrongType(IDNode *id);
		bool isPresent(IDNode *id);
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
		bool lookUp(IDNode *id); //  -- ensure that what were calling exists & is defined
		bool isInCurrentScopeAlready(IDNode *id);
		bool isWrongType(IDNode * id);
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc. 
	private:
		std::list<ScopeTable *> * scopeTableChain;
};

	
}

#endif
