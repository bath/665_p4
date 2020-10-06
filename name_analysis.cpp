#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace holeyc{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	// we want to create a new symbolTable?
	// for every { we access the scopeTableChain and add something to symbols
	// not sure what would go here b/c idk what this node do?
	// should we push new scope onto the symTab? addScope()?
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
bool VarDeclNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	// this == the current VarDeclNode
	// current scope will return the current scope table?
	if (symTab->isInCurrentScopeAlready(this->myID)){ // error: varDecl name already in scope
		// std::cerr << "FATAL [" << this->myID->line() << "," << this->myID->col() << "]: Multiply declared identifier" << std::endl;
		// also in errors.hpp reports.fatal(this->myID->line(), this->myID->col(), "custom message here")
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if(symTab->isWrongType(this->myID)) { // not the correct type (i.e void a; or func a;)
		Report myReport;
		myReport.fatal(this->myID->line(), this->myID->col(), "Bad declaration type (variable or parameter of void type)");
		nameAnalysisOk = false;
	}
	else {
		SemSymbol * s = new SemSymbol(this->myType, this->myID); // check myType, need to return a char currently sending TypeNode
		symTab->currentScope()->addSymbol(s);
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
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
		SemSymbol *s = new SemSymbol(this->myType, this->myID);
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
