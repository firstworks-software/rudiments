// Copyright (c) David Muse
// See the COPYING file for more information.

		websocket(websocket &w) {};
		websocket	&operator=(websocket &w) { return *this; };

	private:
		bool	acceptInternal();
		ssize_t	readInternal(void *buf, size_t size);
		ssize_t	write(const void *buf, size_t size, byte_t opcode);
		ssize_t	writeInternal(const void *buf, size_t size,
							byte_t opcode);
		size_t	copyOut(void *buf, size_t size);
		bool	pong();
		bool	validatePong();

		websocketprivate	*pvt;
