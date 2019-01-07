#include "music_utility.h"
#include "music_context.h"
#include <algorithm>

MusicContext::MusicContext(
    clang::CompilerInstance *CI, Configuration *config,
    LabelStmtToGotoStmtListMap *label_map, 
    SymbolTable *symbol_table, MutantDatabase &mutant_database)
  : comp_inst_(CI), config_(config),
    label_to_gotolist_map_(label_map), function_id_(-1),
    mutant_database_(mutant_database),
    symbol_table_(symbol_table), stmt_context_(CI)
{
	/*std::string input_filename{config->getInputFilename()};
	mutant_filename.assign(input_filename, 0, input_filename.length()-2);
	mutant_filename += ".MUT";*/
}

bool MusicContext::IsRangeInMutationRange(clang::SourceRange range)
{
	SourceRange mutation_range(*(config_->getStartOfMutationRange()),
														 *(config_->getEndOfMutationRange()));

  vector<int>& excluded_list = config_->getExcludedLines();
  int start_line = GetLineNumber(comp_inst_->getSourceManager(), range.getBegin());
  bool excluded = false;

  if (!excluded_list.empty() &&
      std::find(excluded_list.begin(), excluded_list.end(), start_line) != excluded_list.end())
    excluded = true;

	return !excluded && Range1IsPartOfRange2(range, mutation_range);
}

int MusicContext::getFunctionId()
{
  return function_id_;
}

SymbolTable* MusicContext::getSymbolTable()
{
	return symbol_table_;
}

StmtContext& MusicContext::getStmtContext()
{
	return stmt_context_;
}

Configuration* MusicContext::getConfiguration() const
{
  return config_;
}

void MusicContext::IncrementFunctionId()
{
  function_id_++;
}