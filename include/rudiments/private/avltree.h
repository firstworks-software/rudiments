// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	insert(treenode<valuetype> *top,
				treenode<valuetype> *node,
				treenode<valuetype> **treetop);
		void	clone(treecollection<valuetype> *tree);
		void	clone(nodecollection<valuetype> *coll);
		treenode<valuetype>	*cloneNode(treenode<valuetype> *node);

		ssize_t	writeNodeXml(output *out,
					treenode<valuetype> *node,
					const char *name,
					uint16_t *indentlevel,
					bool details,
					bool indent);
		ssize_t	writeNodeJson(output *out,
					treenode<valuetype> *node,
					uint16_t *indentlevel,
					bool indent);

		treenode<valuetype>	*top;
		treenode<valuetype>	*first;
		treenode<valuetype>	*last;
		uint64_t		count;
