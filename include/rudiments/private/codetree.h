// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		char	getSymbolType(domnode *nt);
		bool	isTag(domnode *nt);
		bool	parseChild(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseConcatenation(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	endOfStringOk(domnode *grammarnode);
		bool	parseAlternation(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseOption(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseRepetition(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseException(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseTerminal(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	compareValue(const char *code,
					const char *value,
					size_t *valuelength,
					const char *casesensitive);
		bool	parseLetter(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseLowerCaseLetter(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseUpperCaseLetter(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseDigit(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseNonPrintableCharacter(
					domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseSet(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseBreak(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);
		bool	parseNonTerminal(domnode *grammarnode,
					domnode *treeparent,
					const char **codeposition,
					stringbuffer *ntbuffer);

		void	pushBreakStack();
		void	popBreakStack();
		bool	parseBreakStack(const char **codeposition);

		bool	writeNode(domnode *node,
					stringbuffer *output);
		void	indent(stringbuffer *output);
		void	writeStartEnd(stringbuffer *output,
					const char *string);

		codetreeprivate		*pvt;
