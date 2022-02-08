// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	private:
		int32_t		countPairs(const char *paramstring);
		const char	*parseName(const char *data, char **outbuffer);
		const char	*parseValue(const char *data, char **outbuffer);
		const char	*parsePart(int32_t len,
						char delimiter,
						const char *data,
						char **outbuffer,
						int32_t quotes,
						int32_t escapedchars);
		int32_t	parseNameLength(const char *data);
		int32_t	parseValueLength(const char *data);
		int32_t	parsePartLength(const char *data,
						char delimiter,
						int32_t quotes,
						int32_t escapedchars);
		
		parameterstringprivate	*pvt;
