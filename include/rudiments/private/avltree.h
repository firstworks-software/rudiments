// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	insert(treenode<valuetype> *top,
				treenode<valuetype> *node,
				treenode<valuetype> **treetop);
		void	clone(const treecollection<valuetype> *tree);
		void	clone(const nodecollection<valuetype> *coll);
		treenode<valuetype>	*cloneNode(treenode<valuetype> *node);

		ssize_t	write(output *out,
				const treenode<valuetype> *node,
				const char *name,
				uint16_t *indentlevel) const;

		treenode<valuetype>	*top;
		treenode<valuetype>	*first;
		treenode<valuetype>	*last;
		uint64_t		length;
