#ifndef COMUT_CRCR_H_
#define COMUT_CRCR_H_

#include "expr_mutant_operator.h"

class CRCR : public ExprMutantOperator
{
private:
	std::set<std::string> range_integral_;
	std::set<std::string> range_float_;

public:
	CRCR(const std::string name = "CRCR")
		: ExprMutantOperator(name)
	{}

	virtual bool ValidateDomain(const std::set<std::string> &domain);
	virtual bool ValidateRange(const std::set<std::string> &range);

  virtual void setRange(std::set<std::string> &range);

	// Return True if the mutant operator can mutate this expression
	virtual bool CanMutate(clang::Expr *e, ComutContext *context);

	virtual void Mutate(clang::Expr *e, ComutContext *context);
};

#endif	// COMUT_CRCR_H_