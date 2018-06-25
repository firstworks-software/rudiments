// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

		void		setType(domnodetype type);
		void		setNamespace(const char *ns);
		void		setName(const char *name);
		void		setValue(const char *value);
		void		setParent(domnode *parent);
		void		setPreviousSibling(domnode *previous);
		void		setNextSibling(domnode *next);
		domnode		*clone();
		domnode		*clone(dom *dom);

	protected:
		bool		insertNode(domnode *node,
					uint64_t position,
					domnodetype type,
					domnode **first,
					domnode **last,
					uint64_t *count);
		bool		deleteNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count);
		domnode		*unlinkNode(domnode *node,
					uint64_t position,
					const char *name,
					domnode **first,
					domnode **last,
					uint64_t *count);

	private:
				cursordomnode(const cursordomnode &x);
		cursordomnode	&operator=(const cursordomnode &x);

		cursordomnodeprivate	*pvt;
