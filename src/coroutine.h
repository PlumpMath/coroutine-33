#ifndef __COROUTINE_H__
#define __COROUTINE_H__


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "context.h"
#include "co_structre.h"


/**************     coroutine interface     *********************/


/**
 * Initialize the coroutine library.
 *
 * It must be called before other functions in coroutine library.
 * Otherwise, the program will crash with unknown error.
 */
void coroutine_init();


/**
 * Create a new coroutine but not yet run it.
 *
 * @param attr: attributes of coroutine (have no meaning at present).
 * @param start_rtn: function pointer run by coroutine once started.
 * @param arg:  argument of start_rtn.
 *
 * @return: a coroutine context if success, NULL if error.
 */
coroutine_ctx_t* coroutine_create(const void *attr,
    void(*start_rtn)(void*), void *arg);


/**
 * Run a coroutine with given coroutine context.
 * The caller routine will be suspended at once.
 *
 * @param ctx: context of coroutine which you want to run now.
 */
void coroutine_resume(coroutine_ctx_t *ctx);


/**
 * Pause the current coroutine and schedule to anyone if ready.
 */
void coroutine_yield();


/**
 * Return the current running coroutine context.
 *
 * @return: context related to current coroutine.
 */
coroutine_ctx_t* coroutine_running();


/**************     co_operation interface     *********************/


typedef struct co_tcp_s co_tcp_t;


/**
 * Initialize a ip version4 address structure.
 *
 * @param addr: the address structure need initializing.
 * @param ip: a ip string with format "xxx.xxx.xxx.xxx" or NULL with INADDR_ANY.
 * @param port: a value related to port.
 *
 * @return: 0 means ok.
 */
int co_ip4_addr_init(struct sockaddr_in *addr, const char *ip, unsigned short port);
int co_ip6_addr_init(struct sockaddr_in6 *addr, const char *ip, unsigned short port);


/* socket interface
size_t co_send(co_socket_t *sock, const void *buf, size_t len, int flags);
ssize_t co_sendto(int sockfd, const void *buf, size_t len, int flags,
    const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t co_sendmsg(int sockfd, const struct msghdr *msg, int flags);
*/


/* tcp interface */
int co_tcp4_open(co_tcp_t *tcp);
int co_tcp6_open(co_tcp_t *tcp);

int co_tcp_bind(co_tcp_t *tcp, const struct sockaddr *addr);
int co_tcp_listen(co_tcp_t *tcp, int backlog);
int co_tcp_accept(co_tcp_t *tcp, struct sockaddr *addr, socklen_t *len);

int co_tcp_connect(co_tcp_t *tcp, struct sockaddr *addr);

ssize_t co_tcp_read(co_tcp_t *tcp, void *buf, size_t count);
ssize_t co_tcp_write(co_tcp_t *tcp, const void *buf, size_t count);


/* file interface */
int co_file_open(const char *pathname, int flags, ... /* mode_t */);

ssize_t co_read(int fd, void *buf, size_t count);
ssize_t co_write(int fd, const void *buf, size_t count);

ssize_t co_pread(int fd, void *buf, size_t count, off_t offset_t);
ssize_t co_pwrite(int fd, void *buf, size_t count, off_t offset_t);


/* pipe interface */
int co_pipe_open(int pipefd[2]);
int co_pipe2_open(int pipefd[2], int flags);


#endif

