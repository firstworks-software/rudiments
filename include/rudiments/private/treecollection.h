// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		virtual	ssize_t	writeNode(output *out,
					const treenode<valuetype> *node,
					const char *name,
					uint16_t *indentlevel) const=0;
