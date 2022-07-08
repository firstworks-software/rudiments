// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		virtual	ssize_t	writeNodeXml(output *out,
						const treenode<valuetype> *node,
						const char *name,
						uint16_t *indentlevel,
						bool includedetails) const=0;
