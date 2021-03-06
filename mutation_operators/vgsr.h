#ifndef MUSIC_VGSR_H_
#define MUSIC_VGSR_H_

#include "expr_mutant_operator.h"

class VGSR : public ExprMutantOperator
{
public:
	VGSR(const std::string name = "VGSR")
		: ExprMutantOperator(name)
	{}

	virtual bool ValidateDomain(const std::set<std::string> &domain);
	virtual bool ValidateRange(const std::set<std::string> &range);

	// Return True if the mutant operator can mutate this expression
	virtual bool IsMutationTarget(clang::Expr *e, MusicContext *context);

	virtual void Mutate(clang::Expr *e, MusicContext *context);
};

#endif	// MUSIC_VGSR_H_