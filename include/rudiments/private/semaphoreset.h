// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		semaphoreset(semaphoreset &s) {};
		semaphoreset	&operator=(semaphoreset &s) { return *this; };

	private:
		void	createOperations();

		int32_t	semGet(key_t key, int32_t nsems,
					int32_t semflg, const int32_t *values);
		int32_t	semControl(semaphoresetprivate *pvt, int32_t semnum,
						int32_t cmd, semun *semctlun);
		bool	semTimedOp(struct sembuf *sops,
						int32_t seconds,
						int32_t nanoseconds);
		bool	semOp(struct sembuf *sops);

		bool	forceRemove();

		semaphoresetprivate	*pvt;
