#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace holeyc{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	// astnode nameAnalysis decl has no param name, is this going to be a problem in compile time?

	// WTF does this function do?

	// we want to create a new symbolTable?
	// for every { we access the scopeTableChain and add something to symbols
	// not sure what would go here b/c idk what this node do?
	// should we push new scope onto the symTab? addScope()?
	return false;
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	ScopeTable * mainScope = new ScopeTable();
	symTab->addScope(mainScope);
	bool res = true;
	for (auto global : *myGlobals){
		// global could be VarDeclNode or FnDeclNode
		res = global->nameAnalysis(symTab) && res; // for every node in myGlobals, execute nameAnalysis and pass in the current symTab
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisOk = false;
	if (symTab->isInCurrentScopeAlready(this->myID)){ // CCC
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if(!symTab->isCorrectType(this->getTypeNode(), 'v')) {
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "Bad declaration type (variable or parameter of void type)");
		nameAnalysisOk = false;
	}
	else {
		SemSymbol *s = new SemSymbol('v', this->getTypeNode()->getType());
		symTab->currentScope()->addSymbol(this->ID()->getName(),s);
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable *symTab){
	bool nameAnalysisOk = true;

	// NOTE: 
	// During name analysis, if a function name is multiply declared you should 
	// still process the formals and the body of the function; don't add a new entry 
	// to the current symbol table for the function, but do add a new hashtable to 
	// the front of the SymTable's list for the names declared in the body (i.e., the 
	// parameters and other local variables of the function).

	// what types of errors can occur from a function declaration?
		// no return type -- were not concerned about that right now i think?
		// there is already a function in the current scope named 

	// we might not need  else stmt bc according to project spec we continue adding to the table??
	if (symTab->isInCurrentScopeAlready(this->myID)) { // fndecl errors are handled special(PITA). dont fuck w/ rn
		// already exists
	}
	else if(){
		// can function decl have a bad type? i don't think so
	}
	else {
		SemSymbol *s = new SemSymbol(this->myRetType, this->myID);
		ScopeTable *newScope = new ScopeTable();
		symTab->addScope(newScope);
		symTab->
	}
	throw new ToDoError("[DELETE ME] I'm an fnDecl."
		" you should add and make current a new"	
		" scope table for my body"
	);
	return nameAnalysisOk;
}

}
