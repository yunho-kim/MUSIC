#ifndef MUSIC_VLPR_H_
#define MUSIC_VLPR_H_

#include "expr_mutant_operator.h"

class VLPR : public ExprMutantOperator
{
public:
	VLPR(const std::string name = "VLPR")
		: ExprMutantOperator(name)
	{}

	virtual bool ValidateDomain(const std::set<std::string> &domain);
	virtual bool ValidateRange(const std::set<std::string> &range);

	// Return True if the mutant operator can mutate this expression
	virtual bool IsMutationTarget(clang::Expr *e, MusicContext *context);

	virtual void Mutate(clang::Expr *e, MusicContext *context);

private:
	void GetRange(clang::Expr *e, MusicContext *context, VarDeclList *range);
};

#endif	// MUSIC_VLPR_H_