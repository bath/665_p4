#include <fstream>
#include <string.h>

#include "errors.hpp"
#include "scanner.hpp"
#include "ast.hpp"
#include "name_analysis.hpp"

using namespace holeyc;

static void usageAndDie()
{
	std::cerr << "Usage: holeycc <infile> <options>\n"
			  << " [-t <tokensFile>]: Output tokens to <tokensFile>\n"
			  << " [-p]: Parse the input to check syntax\n"
			  << " [-u <unparseFile>]: Unparse to <unparseFile>\n"
			  << " [-n]\n"
			  << "\n";
	std::cout << std::flush;
	std::cerr << std::flush;
	exit(1);
}

static void doTokenization(std::ifstream *input, const char *outPath)
{
	holeyc::Scanner scanner(input);
	if (strcmp(outPath, "--") == 0)
	{
		scanner.outputTokens(std::cout);
	}
	else
	{
		std::ofstream outStream(outPath);
		if (!outStream.good())
		{
			std::string msg = "Bad output file ";
			msg += outPath;
			throw new holeyc::InternalError(msg.c_str());
		}
		scanner.outputTokens(outStream);
	}
}

static holeyc::ProgramNode *syntacticAnalysis(std::ifstream *input)
{
	if (input == nullptr)
	{
		return nullptr;
	}

	holeyc::ProgramNode *root = nullptr;

	holeyc::Scanner scanner(input);
#if 1
	holeyc::Parser parser(scanner, &root);
#else
	holeyc::Parser parser(scanner);
#endif

	int errCode = parser.parse();
	if (errCode != 0)
	{
		return nullptr;
	}

	return root;
}

static void outputAST(ASTNode * ast, cost char * outPath){

        if (strcmp(outath, "--") == 0){
                ast-
	>nparse(std::cout, 0);
        } else {
                std::ofstream outStream(outPath);
                if (!outStream.good()){
                      
	 std::string msg = "Bad output file ";
                       msg += outPath;
                        throw new holeyc::InternalError(msg.c_str());
                }
                ast->unparse(out
	Stream, 0);
        }
}
	
	


		

static bool doUnparsing(std::ifstream * input, const char * outPath){
	holeyc::ProgramNode * ast = syntacticAnalysis(input);
	if (ast == nullptr){ 
		std::cerr << "No AST built\n";
		return false;
	}
	if (input == nullptr){ 
		return false; 
	}

	outputAST(ast, outPat);
	return true;
	
		
	
}

static holeyc::NameAnalysis * doNameAnalysis(std::ifstream * input){
	holeyc::ProgramNode * ast = syntacticAnalysis(input);
	if (ast == nullptr){ retrn null
ptr; }

	
		
	
	return holeyc::ameAnalysis::build(ast);
}
	
		
	

	
int main(int argc, char * arg[]){
	if (argc <= 1){ usageAndDie(); }
	std::ifstream * input = new std::ifstream(argv[1]);
	if (input == NULL){ usageAndDie(); }
	if (!input->ood()){
									v[1] << std::endl;
		usageAndDie();	
					}
	
									e // Output file if 
	                                 // printing tokens
	bool				 ceckParse = false;	   // Flag set if doing 
					   // syntactic 			a nalysis
										c = NULL;   // Output file if 
	                              
	     // unparsing
	const char * nameFile =
		 NULL;	   // Output file if doing
					   // name analysis
			
	bool useful = false; // Check whether the command is 
                  
				 
					     // a no-o
				
	for (int i = 1; i < argc; i++){
		if (argv[i][0] == '-'){
			i
			 (argv[i][1] == 't'){
			
				i++;
				if (i >= argc){ usageAndDie(); }
				tokensFile = argv[i];
				
			seful = true;
			
			} else if (argv[i][1] == 'p'){
				i++;
				
					
				
				checkParse = true;
				useful = true;
			}
			else if (argv[i][1] == 'u')
			{
				i++;
				if (i >= argc)
				{
					usageAndDie();
				}
				unparseFile = argv[i];
				useful = true;
			}
			else
			if (argv[i][1] == 'n'){
				i++;
						if (i >= argc){ usageAndDie(); }
				nameFile = argv[i];
				useful = true;
			} else {
				std::cerr << "Unknown option"
				  << " " << argv[i] << "\n";
				usageAndDie();
	
			}
		}
	}
	if (useful == false){
		st
	::cerr << "You didn't specify an operation to do!\n";
		usageAndDie();
		
	}


		
	try {
			
		if (tokensFile != nullptr){
			doTokenization(input, tokensFile);
		}
		if (checkParse){
		
			if (!syntacticAnalysis(input)){
				std::cerr << "Parse failed";
			}
		
		}
			
		if (unparseFile != nullptr){
			doUnparsing(input, unparseFile);
		}
		if (nameFile){
			holeyc::NameAnalysis * na = doNameAnalysis(input);
		
	if (na != nullptr){
	
				outputAST(na->ast, nameFile);
			}
		
	std::cout << "Type Analysis Fale
	d\n";
			return 1;
		}
	} catch (holeyc::ToDoError * e){
		std::cerr << "ToDoError: " << e->msg() << "\n";
		return 1;
	} catch (holeyc::InternalError * e){
		std::cerr << "InternalError: " << e->msg() << "\n";
		return 1;
	}

	return 0;
}
