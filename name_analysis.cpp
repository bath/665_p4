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
	if( (symTab->isInCurrentScopeAlready(this->ID())) && (!symTab->isCorrectType(this->getTypeNode()))) { // if both errors
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "Bad declaration type (variable or parameter of void type)");
		myReport.fatal(this->ID()->line(), this->ID()->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if(symTab->isInCurrentScopeAlready(this->ID())){ // CCC
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if(!symTab->isCorrectType(this->getTypeNode())) {
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "Bad declaration type (variable or parameter of void type)");
		nameAnalysisOk = false;
	}
	else {
		SemSymbol *s = new SemSymbol('v', this->getTypeNode()->getType(), this->ID());
		symTab->currentScope()->addSymbol(this->ID()->getName(),s);
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;	
}

bool FnDeclNode::nameAnalysis(SymbolTable *symTab){
	bool nameAnalysisOk = true;

	// is an example of an FnDecl: int a(int b, int c, bool d);
	// ? 
	// drew's piazza post implies stmt list?? when does that come into play?
	// i thought declaration didn't have statement nodes in it.. its just a declaration..

	// NOTE: 
	// During name analysis, if a function name is multiply declared you should 
	// still process the formals and the body of the function; don't add a new entry 
	// to the current symbol table for the function, but do add a new hashtable to 
	// the front of the SymTable's list for the names declared in the body (i.e., the 
	// parameters and other local variables of the function).

	// what types of errors can occur from a function declaration?
		// no return type -- were not concerned about that right now i think?
		// there is already a function in the current scope named 
		// what to do about formals and stmt list?

	// errors:
		// ensure function id doesn't already exist in current scope
		// ensure formals don't already exist in the current scope
			// do we call nameanalysis() within this fndeclnode though?

	// we might not need  else stmt bc according to project spec we continue adding to the table??

	// id check
		// 1st check fn name for current scope
		// 2nd check formaldecls -- do we do that in fndecl? or just in formaldeclnode?
		// add to current scope if both valid
		// create new scope for stmts in fn body

	// my main question is when were doing the checks for each thing after, like how does the "walk" through ast work?

	if (symTab->isInCurrentScopeAlready(this->ID())) { // is function id already in scope
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;

		// even if this fails, we should still process the formals and body.. right here though? where??

	}
	else {
		SemSymbol *s = new SemSymbol('f', this->getTypeNode()->getType(), this->ID());
		symTab->currentScope()->addSymbol(this->ID()->getName(), s);
		ScopeTable *newScope = new ScopeTable();
		symTab->addScope(newScope);

		// we don't need to check the myRetType, correct?

		// go through the formals and body?
		nameAnalysisOk = true;

		for (auto formals : *myFormals) {
			nameAnalysisOk = formals->nameAnalysis(symTab);
		}

		for (auto body : *myBody) {
			nameAnalysisOk = body->nameAnalysis(symTab);
		}

	}
	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable *symTab) {

	// check the cond? ... i dont think so?
	// check the body? ... yes ... but do we do that in this defn or in stmt node?
	return false;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	nameAnalysisIsOk = this->myExp->nameAnalysis(symTab);

	return nameAnalysisIsOk;
}

bool CallExpNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	nameAnalysisIsOk = this->myID->nameAnalysis(symTab);

	for(auto args : *myArgs) {
		nameAnalysisIsOk = args->nameAnalysis(symTab);
	}

	return nameAnalysisIsOk;
}

bool BinaryExpNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	nameAnalysisIsOk = this->myExp1->nameAnalysis(symTab);
	nameAnalysisIsOk = this->myExp2->nameAnalysis(symTab);

	return nameAnalysisIsOk;
}

bool PlusNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool MinusNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool TimesNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool DivideNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool AndNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool OrNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool EqualsNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool NotEqualsNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool LessNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool LessEqNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool GreaterNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool GreaterEqNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	// this isn't supposed to do anything?

	return nameAnalysisIsOk;
}

bool UnaryExpNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisIsOk = false;

	nameAnalysisIsOk = this->myExp->nameAnalysis(symTab);

	return nameAnalysisIsOk;
}

bool IfStmtNode::nameAnalysis(SymbolTable *symTab) {
	bool nameAnalysisIsOk = false;

	nameAnalysisIsOk = this->myCond->nameAnalysis(symTab);

	for (auto body : *myBody) {
		nameAnalysisIsOk = body->nameAnalysis(symTab);
	}

	// check the cond? ... i dont think so?
	// check the body? ... yes ... but do we do that in this defn or in stmt node?
	return nameAnalysisIsOk;	
}


bool StmtNode::nameAnalysis(SymbolTable *symTab) {
	// what do we do in parent nodes?
	return false;
}

bool AssignExpNode::nameAnalysis(SymbolTable *symTab)
{
	bool nameAnalysisOk = false;
	nameAnalysisOk = this->myDst->nameAnalysis(symTab);
	nameAnalysisOk = this->mySrc->nameAnalysis(symTab);

	return nameAnalysisOk;
}

bool LValNode::nameAnalysis(SymbolTable *symTab) {
	// are we supposed to do anything here?

	// yes.. ?

	// what / how do we call in here?

	return false;
}

bool FormalDeclNode::nameAnalysis(SymbolTable *symTab) {
	// here i am assuming even though the private variables aren't defined
	// in FormalDeclNode, a myType and myID already exist since it is inheriting from
	// VarDeclNode, which does have them

	bool nameAnalysisOk = false;
	if ((symTab->isInCurrentScopeAlready(this->ID())) && (!symTab->isCorrectType(this->getTypeNode())))
	{ // if both errors
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "Bad declaration type (variable or parameter of void type)");
		myReport.fatal(this->ID()->line(), this->ID()->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if (symTab->isInCurrentScopeAlready(this->ID())) { // CCC
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "More than one declaration of an identifier in a given scope");
		nameAnalysisOk = false;
	}
	else if (!symTab->isCorrectType(this->getTypeNode())) {
		Report myReport;
		myReport.fatal(this->ID()->line(), this->ID()->col(), "Bad declaration type (variable or parameter of void type)");
		nameAnalysisOk = false;
	}
	else {
		SemSymbol *s = new SemSymbol('v', this->getTypeNode()->getType(), this->ID());
		symTab->currentScope()->addSymbol(this->ID()->getName(), s);
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;
}

bool 


} // end name definitions

