package com.bean;

public enum State {

	SUCCESS(0),
	INVALID_CONFIG(-1),
	FORK_FAILED(-2),
	PTHREAD_FAILED(-3),
	WAIT_FAILED(-4),
	ROOT_REQUIRED(-5),
	LOAD_SECCOMP_FAILED(-6),
	SETRLIMIT_FAILED(-7),
	DUP2_FAILED(-8),
	SETUID_FAILED(-9),
	EXECVE_FAILED(-10),
	SPJ_ERROR(-11);

	private State(int index) {
		this.code = code;
	}

	private short code;
}
