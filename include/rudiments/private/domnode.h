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
		void		init(dom *dom, domnode *nullnode);
		domnode	*getNode(domnode *first,
					uint64_t position,
					const char *ns,
					const char *name,
					bool ignorecase,
					uint64_t count) const;
		domnode	*getAttribute(
					const char *name,
					bool ignorecase) const;
		domnode	*getFirstChild(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		domnode	*getFirstChild(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		domnode	*getFirstTagChild(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		domnode	*getFirstTagChild(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		domnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		domnode	*getPreviousTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		domnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		domnode	*getNextTagSibling(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		domnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					bool ignorecase) const;
		domnode	*getFirstTagDescendent(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase) const;
		bool		deleteFirstChild(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteChildren(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		deleteDescendents(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		deleteDescendents(
					const char *ns,
					const char *name,
					const char *attributename,
					const char *attributevalue,
					bool ignorecase);
		bool		unwrapFirstChild(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		unwrapChildren(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		unwrapDescendents(
					const char *ns,
					const char *name,
					bool ignorecase);
		bool		renameFirstChild(
					const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool		renameChildren(
					const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		bool		renameDescendents(
					const char *oldns,
					const char *oldname,
					const char *newns,
					const char *newname,
					bool ignorecase);
		void		write(output *out,
					bool indent,
					uint16_t *indentlevel) const;
		void		safeWrite(output *out, const char *str) const;
		bool		match(domnode *node,
						const char *ns,
						const char *name,
						bool ignorecase) const;
		bool		match(domnode *node,
						const char *ns,
						const char * const *set) const;

		void	setPrivateData(void *privatedata);
		void	*getPrivateData();

				domnode(const domnode &x);
		domnode	&operator=(const domnode &x);

		domnodeprivate	*pvt;
