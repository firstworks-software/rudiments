// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		virtual	bool		insertNode(domnode *node,
							uint64_t position,
							domnodetype type,
							domnode **first,
							domnode **last,
							uint64_t *count);
		virtual	bool		deleteNode(domnode *node,
							uint64_t position,
							const char *name,
							domnode **first,
							domnode **last,
							uint64_t *count);
		virtual	domnode		*unlinkNode(domnode *node,
							uint64_t position,
							const char *name,
							domnode **first,
							domnode **last,
							uint64_t *count);

	friend class codetree;
	friend class codetreegrammar;
	friend class domevents;
	private:
		domnode(domnode &x);
		domnode	&operator=(domnode &x);

		void	construct(dom *dom);
		domnode	*getNode(domnode *first,
					uint64_t position,
					const char *ns,
					const char *name,
					bool ignorecase,
					uint64_t count);
		domnode	*getAttribute(const char *name,
					bool ignorecase);
		domnode	*getFirstChild(const char *ns,
					const char *name,
					bool ignorecase);
		domnode	*getFirstChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		domnode	*getFirstTagChild(const char *ns,
					const char *name,
					bool ignorecase);
		domnode	*getFirstTagChild(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		domnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase);
		domnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		domnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase);
		domnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		domnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					bool ignorecase);
		domnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool	deleteFirstChild(const char *ns,
					const char *name,
					bool ignorecase);
		bool	deleteChildren(const char *ns,
					const char *name,
					bool ignorecase);
		bool	deleteChildren(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool	deleteDescendents(const char *ns,
					const char *name,
					bool ignorecase);
		bool	deleteDescendents(const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool	unwrapFirstChild(const char *ns,
					const char *name,
					bool ignorecase);
		bool	unwrapChildren(const char *ns,
					const char *name,
					bool ignorecase);
		bool	unwrapDescendents(const char *ns,
					const char *name,
					bool ignorecase);
		bool	renameFirstChild(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool	renameChildren(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool	renameDescendents(const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		void	write(output *out,
					bool indent,
					uint16_t *indentlevel);
		void	safeWrite(output *out, const char *str);
		bool	match(domnode *node,
					const char *ns,
					const char *name,
					bool ignorecase);
		bool	match(domnode *node,
					const char *ns,
					const char * const *set);

		void	setPrivateData(void *privatedata);
		void	*getPrivateData();

		domnodeprivate	*pvt;
