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

// check type of functions and varDecls, since you can only have certain types be void for ex.. see project details / errors
bool VarDeclNode::nameAnalysis(SymbolTable *symTab) {
	// only two problems may arise with varDecl
		// 1. name exists in current scope already
		// 2. wrong type (void a;)
		// else valid
	// TODO : not sure this function definition is correct
	bool nameAnalysisOk = false;
	// this == the current VarDeclNode
	// current scope will return the current scope table?
	if (symTab->isInCurrentScopeAlready(this->myID)){
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if(symTab->isVarWrongType(this->myID)) {
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "Bad declaration type (variable or parameter of void type)");
		nameAnalysisOk = false;
	}
	else {
			// IDK HOW TO GET A TYPENODE'S TYPE IN STRING? 
			// like do we call name analysis on the typenode? that doesn't make sense
			// not sure we should even define anything for the typenode nameanalysis(), let alone call it
		SemSymbol *s = new SemSymbol("v",this->myType->getType(), this->myID); // check myType, need to return a char currently sending TypeNode
		symTab->currentScope()->addSymbol(s);
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable *symTab){
	bool nameAnalysisOk = true;

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
