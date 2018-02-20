// Copyright (c) 1999-2017 David Muse
// See the COPYING file for more information

	private:

		bool	parse(bool justvariables,
					stringbuffer *output,
					const char *block,
					uint64_t blocklength,
					blockparser *blockparsers,
					dictionary< const char *,
							const char * > *vars);
		void	replaceVariable(stringbuffer *output,
					char **buffer,
					dictionary< const char *,
							const char * > *vars);
		bool	getBlockName(char **buffer,
					stringbuffer *blockname,
					dictionary< const char *,
							const char * > *vars);
		bool	getBlockLength(char **buffer,
					uint64_t *blocklength);
		bool	getIncludeFilename(char **buffer,
					stringbuffer *filename,
					dictionary< const char *,
							const char * > *vars);
		bool	getName(char **buffer,
					stringbuffer *name,
					dictionary< const char *,
							const char * > *vars,
					const char *end,
					uint16_t endlen);
		bool	parseBlock(stringbuffer *output,
					const char *blockname,
					const char *block,
					uint64_t blocklength,
					blockparser *blockparsers,
					dictionary< const char *,
							const char * > *vars);

		templateengineprivate	*pvt;
