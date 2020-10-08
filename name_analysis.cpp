#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace holeyc{

//TODO here is a subset of the nodes needed to do nameAnalysis, 
// you should add the rest to allow for a complete treatment
// of any AST

// should be 4 instances of creating new scopes

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
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
		SemSymbol *s = new SemSymbol('v', this);
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

		ScopeTable *newScope = new ScopeTable();
		symTab->addScope(newScope);

		for (auto formals : *myFormals)
		{
			nameAnalysisOk = formals->nameAnalysis(symTab);
		}

		for (auto body : *myBody)
		{
			nameAnalysisOk = body->nameAnalysis(symTab);
		}
	}
	else { // is a valid function declaration
		
		// add function id to scope
		SemSymbol *s = new SemSymbol('f', this);
		symTab->currentScope()->addSymbol(this->ID()->getName(), s);

		// create new scope for formals and body
		ScopeTable *newScope = new ScopeTable();
		symTab->addScope(newScope);

		nameAnalysisOk = true;

		for (auto formals : *myFormals)
		{
			nameAnalysisOk = formals->nameAnalysis(symTab);
		}

		for (auto body : *myBody)
		{
			nameAnalysisOk = body->nameAnalysis(symTab);
		}

		symTab->dropScope();
	}
	return nameAnalysisOk;
}

bool IDNode::nameAnalysis(SymbolTable *symTab) { // make sure this is right
	if(symTab->lookUp(this)) {
		return true;
	}
	else {
		Report myReport;
		myReport.fatal(line(), col(), "Use of an undeclared identifier");
		return false;
	}
}

bool RefNode::nameAnalysis(SymbolTable *symTab) {
	if (myID->nameAnalysis(symTab))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DerefNode::nameAnalysis(SymbolTable *symTab)
{
	if (myID->nameAnalysis(symTab))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IndexNode::nameAnalysis(SymbolTable *symTab)
{
	if ((myBase->nameAnalysis(symTab)) && (myOffset->nameAnalysis(symTab)))
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool WhileStmtNode::nameAnalysis(SymbolTable *symTab) // correct
{
	bool one, two;
	one = this->myCond->nameAnalysis(symTab);
	ScopeTable *scope = new ScopeTable();
	symTab->addScope(scope);
	for (auto body : *myBody) // the variable's name is myBody for WhileStmtNode -- see line 22 on name_analysis.cpp
	{
		two = this->body->nameAnalysis(symTab);
	}
	symTab->dropScope();
	return (one && two);
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

bool PlusNode::nameAnalysis(SymbolTable *symTab) {
	bool one,two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool MinusNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool TimesNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool DivideNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool AndNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool OrNode::nameAnalysis(SymbolTable *symTab) {
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool EqualsNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool NotEqualsNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool LessNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool LessEqNode::nameAnalysis(SymbolTable *symTab)
{
	bool one,two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool GreaterNode::nameAnalysis(SymbolTable* symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool GreaterEqNode::nameAnalysis(SymbolTable *symTab)
{
	bool one, two;
	one = this->myExp1->nameAnalysis(symTab);
	two = this->myExp2->nameAnalysis(symTab);
	return (one && two);
}

bool NegNode::nameAnalysis(SymbolTable * symTab)
{
	bool one;
	one = this->myExp->nameAnalysis(symTab);
	return (one);
}

bool NotNode::nameAnalysis(SymbolTable * symTab)
{
	bool one;
	one = this->myExp->nameAnalysis(symTab);
	return (one);
}

bool VoidTypeNode::nameAnalysis(SymbolTable * symTab) {

	return true;

}

bool IntTypeNode::nameAnalysis(SymbolTable * symTab)
{

	return true;
}

bool BoolTypeNode::nameAnalysis(SymbolTable * symTab)
{

	return true;
}

bool CharTypeNode::nameAnalysis(SymbolTable * symTab)
{

	return true;
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab) { 
	// this isn't right? we need to verify that the 
	// use the callexpnode to get the id and ensure that it is already existing, if it is 
	// then 
	bool one;
	one = this->myCallExp->nameAnalysis(symTab);
	return one;
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
	bool one,two;
	one = this->myDst->nameAnalysis(symTab);
	two = this->mySrc->nameAnalysis(symTab);

	return (one && two);
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
		SemSymbol *s = new SemSymbol('v', this);
		symTab->currentScope()->addSymbol(this->ID()->getName(), s);
		nameAnalysisOk = true;
	}
	return nameAnalysisOk;
}

} // end name definitions

