// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		virtual	ssize_t	writeNodeXml(output *out,
						treenode<valuetype> *node,
						const char *name,
						uint16_t *indentlevel,
						bool details,
						bool indent)=0;
		virtual	ssize_t	writeNodeJson(output *out,
						treenode<valuetype> *node,
						uint16_t *indentlevel,
						bool indent)=0;
