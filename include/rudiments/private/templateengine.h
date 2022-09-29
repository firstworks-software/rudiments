// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	private:
		templateengine(templateengine &t) {};
		templateengine	&operator=(templateengine &t) { return *this; };

		bool	parse(bool justvariables,
					output *out,
					const char *block,
					uint64_t blocklength,
					blockparser *blockparsers,
					dictionary< const char *,
							const char * > *vars);
		void	replaceVariable(output *out,
					char **buffer,
					dictionary< const char *,
							const char * > *vars,
					bool escq, bool escdq);
		bool	getBlockName(char **buffer,
					stringbuffer *blockname,
					dictionary< const char *,
							const char * > *vars);
		bool	getBlockLength(const char *blockname,
					char **buffer,
					uint64_t *blocklength);
		bool	getIncludeFileName(char **buffer,
					stringbuffer *filename,
					dictionary< const char *,
							const char * > *vars);
		bool	getName(char **buffer,
					stringbuffer *name,
					dictionary< const char *,
							const char * > *vars,
					const char *end,
					uint16_t endlen);
		bool	parseBlock(output *out,
					const char *blockname,
					const char *block,
					uint64_t blocklength,
					blockparser *blockparsers,
					dictionary< const char *,
							const char * > *vars);

		templateengineprivate	*pvt;
