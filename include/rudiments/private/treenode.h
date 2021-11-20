// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		virtual void	insert(treenode<valuetype> *node,
					treenode<valuetype> **treetop)=0;
		virtual void	detach(treenode<valuetype> **treetop)=0;

		virtual void	adjustParentHeights(
					treenode<valuetype> *node)=0;

		virtual void	balance(treenode<valuetype> **treetop)=0;

		virtual treenode<valuetype> *leftRightRotate(
					treenode<valuetype> **treetop)=0;
		virtual treenode<valuetype> *rightLeftRotate(
					treenode<valuetype> **treetop)=0;
		virtual treenode<valuetype> *leftRotate(
					treenode<valuetype> **treetop)=0;
		virtual treenode<valuetype> *rightRotate(
					treenode<valuetype> **treetop)=0;

		virtual void	setParent(treenode<valuetype> *node)=0;
		virtual void	setLeftChild(treenode<valuetype> *node)=0;
		virtual void	setRightChild(treenode<valuetype> *node)=0;
		virtual void	setLeftHeight(uint8_t height)=0;
		virtual void	setRightHeight(uint8_t height)=0;
