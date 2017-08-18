#include "../comut_utility.h"
#include "vgtf.h"

bool VGTF::ValidateDomain(const std::set<std::string> &domain)
{
	return domain.empty();
}

bool VGTF::ValidateRange(const std::set<std::string> &range)
{
	return range.empty();
}

// Return True if the mutant operator can mutate this expression
bool VGTF::CanMutate(clang::Expr *e, ComutContext *context)
{
	if (CallExpr *ce = dyn_cast<CallExpr>(e))
	{
		SourceLocation start_loc = ce->getLocStart();

    // getRParenLoc returns the location before the right parenthesis
    SourceLocation end_loc = ce->getRParenLoc();
    end_loc = end_loc.getLocWithOffset(1);

    // Return True if expr is in mutation range, NOT inside enum decl
    // and is structure type.
		return (Range1IsPartOfRange2(
				SourceRange(start_loc, end_loc), 
				SourceRange(*(context->userinput->getStartOfMutationRange()),
										*(context->userinput->getEndOfMutationRange()))) &&
						!context->is_inside_enumdecl &&
						ExprIsStruct(e));
	}

	return false;
}

// Return True if the mutant operator can mutate this statement
bool VGTF::CanMutate(clang::Stmt *s, ComutContext *context)
{
	return false;
}

void VGTF::Mutate(clang::Expr *e, ComutContext *context)
{
	CallExpr *ce;
	if (!(ce = dyn_cast<CallExpr>(e)))
		return;

	SourceLocation start_loc = ce->getLocStart();

  // getRParenLoc returns the location before the right parenthesis
  SourceLocation end_loc = ce->getRParenLoc();
  end_loc = end_loc.getLocWithOffset(1);

  Rewriter rewriter;
	SourceManager &src_mgr = context->comp_inst->getSourceManager();
	rewriter.setSourceMgr(src_mgr, context->comp_inst->getLangOpts());

	string token{rewriter.ConvertToString(e)};

	// cannot mutate variable in switch condition to a floating-type variable
  bool skip_float_vardecl = LocationIsInRange(
  		start_loc, *(context->switchstmt_condition_range));

  string struct_type{
  		getStructureType(e->getType().getCanonicalType())};

  for (auto vardecl: *(context->global_struct_vardecl_list))
  {
    if (skip_float_vardecl && IsVarDeclFloating(vardecl))
        continue;

    string mutated_token{GetVarDeclName(vardecl)};

    if (token.compare(mutated_token) != 0 &&
        struct_type.compare(getStructureType(vardecl->getType())) == 0)
    {
      GenerateMutantFile(context, start_loc, end_loc, mutated_token);
      WriteMutantInfoToMutantDbFile(context, start_loc, end_loc, token, 
                                    mutated_token);
    }
  }
}

void VGTF::Mutate(clang::Stmt *s, ComutContext *context)
{}
