// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		valuetype	&getValue();

		void	detach(treenode<valuetype> **treetop);

		void	adjustParentHeights(treenode<valuetype> *node);

		void	balance(treenode<valuetype> **treetop);

		treenode<valuetype> *leftRightRotate(
					treenode<valuetype> **treetop);
		treenode<valuetype> *rightLeftRotate(
					treenode<valuetype> **treetop);
		treenode<valuetype> *leftRotate(
					treenode<valuetype> **treetop);
		treenode<valuetype> *rightRotate(
					treenode<valuetype> **treetop);

		void	setParent(treenode<valuetype> *node);
		void	setLeftChild(treenode<valuetype> *node);
		void	setRightChild(treenode<valuetype> *node);
		void	setLeftHeight(uint8_t height);
		void	setRightHeight(uint8_t height);

	protected:
		valuetype		value;
		treenode<valuetype>	*parent;
		treenode<valuetype>	*left;
		treenode<valuetype>	*right;
		uint8_t			leftheight;
		uint8_t			rightheight;

	private:
		void	print(const char *name, uint16_t *indentlevel) const;
