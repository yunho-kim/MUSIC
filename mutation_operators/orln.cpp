#include "../music_utility.h"
#include "orln.h"

extern set<string> relational_operators;
extern set<string> logical_operators;

bool ORLN::ValidateDomain(const std::set<std::string> &domain)
{
	for (auto it: domain)
  	if (relational_operators.find(it) == relational_operators.end())
    	// cannot find input domain inside valid domain
      return false;

  return true;
}

bool ORLN::ValidateRange(const std::set<std::string> &range)
{
	for (auto it: range)
  	if (logical_operators.find(it) == logical_operators.end())
    	// cannot find input range inside valid range
      return false;

  return true;
}

void ORLN::setDomain(std::set<std::string> &domain)
{
	if (domain.empty())
		domain_ = relational_operators;
	else
		domain_ = domain;
}

void ORLN::setRange(std::set<std::string> &range)
{
	if (range.empty())
		range_ = logical_operators;
	else
		range_ = range;
}

bool ORLN::IsMutationTarget(clang::Expr *e, MusicContext *context)
{
	if (BinaryOperator *bo = dyn_cast<BinaryOperator>(e))
	{
		string binary_operator{bo->getOpcodeStr()};
		SourceLocation start_loc = bo->getOperatorLoc();
		SourceManager &src_mgr = context->comp_inst_->getSourceManager();
		// cout << "cp orln\n";
		SourceLocation end_loc = src_mgr.translateLineCol(
				src_mgr.getMainFileID(),
				GetLineNumber(src_mgr, start_loc),
				GetColumnNumber(src_mgr, start_loc) + binary_operator.length());
		StmtContext &stmt_context = context->getStmtContext();

		// Return True if expr is in mutation range, NOT inside array decl size
		// and NOT inside enum declaration.
		if (context->IsRangeInMutationRange(SourceRange(start_loc, end_loc)) &&
				!stmt_context.IsInArrayDeclSize() &&
				!stmt_context.IsInEnumDecl() &&
				domain_.find(binary_operator) != domain_.end())
			return true;
	}

	return false;
}



void ORLN::Mutate(clang::Expr *e, MusicContext *context)
{
	BinaryOperator *bo;
	if (!(bo = dyn_cast<BinaryOperator>(e)))
		return;

	string token{bo->getOpcodeStr()};
	SourceLocation start_loc = bo->getOperatorLoc();
	SourceManager &src_mgr = context->comp_inst_->getSourceManager();
	// cout << "cp orln\n";
	SourceLocation end_loc = src_mgr.translateLineCol(
			src_mgr.getMainFileID(),
			GetLineNumber(src_mgr, start_loc),
			GetColumnNumber(src_mgr, start_loc) + token.length());

	for (auto mutated_token: range_)
		if (token.compare(mutated_token) != 0)
		{
			context->mutant_database_.AddMutantEntry(name_, start_loc, end_loc, token, mutated_token, context->getStmtContext().getProteumStyleLineNum());
		}
}

