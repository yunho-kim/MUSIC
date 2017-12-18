#ifndef COMUT_OEBA_H_
#define COMUT_OEBA_H_

#include "expr_mutant_operator.h"

class OEBA : public ExprMutantOperator
{
public:
	OEBA(const std::string name = "OEBA")
		: ExprMutantOperator(name)
	{}

	virtual bool ValidateDomain(const std::set<std::string> &domain);
	virtual bool ValidateRange(const std::set<std::string> &range);

	virtual void setDomain(std::set<std::string> &domain);
  virtual void setRange(std::set<std::string> &range);

	// Return True if the mutant operator can mutate this expression
	virtual bool CanMutate(clang::Expr *e, ComutContext *context);

	virtual void Mutate(clang::Expr *e, ComutContext *context);
};

#endif	// COMUT_OEBA_H_