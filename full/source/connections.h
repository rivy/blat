#if !defined __CONNECTIONS_H__
#define __CONNECTIONS_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#if defined(_MSC_VER) && (_MSC_VER < 1300)
// For VC6, disable warnings from various standard Windows headers
// NOTE: #pragma warning(push) ... #pragma warning(pop) is broken/unusable for MSVC 6 (re-enables multiple other warnings)
#pragma warning(disable : 4068) // DISABLE: unknown pragma warning
#pragma warning(disable : 4035) // DISABLE: no return value warning
#endif
#include <windows.h>
#if defined(_MSC_VER) && (_MSC_VER < 1300)
#pragma warning(default : 4068) // RESET: unknown pragma warning
#pragma warning(default : 4035) // RESET: no return value warning
#endif

#include <winsock.h>

#include "gensock.h"

//
// ---------------------------------------------------------------------------
// container for a buffered SOCK_STREAM.

class connection {
private:
    SOCKET              the_socket;
    char *              pInBuffer;
    char *              pOutBuffer;
    unsigned int        in_index;
    unsigned int        out_index;
    unsigned int        in_buffer_total;
    unsigned int        out_buffer_total;
    unsigned int        last_winsock_error;
    int                 buffer_size;
    TASK_HANDLE_TYPE    owner_task;
    fd_set              fds;
    struct timeval      timeout;

public:

    connection (void);
    ~connection (void);

    int                 get_connected (struct _COMMON_DATA & CommonData, LPTSTR hostname, LPTSTR service);
    SOCKET              get_socket(void) { return(the_socket);}
    TASK_HANDLE_TYPE    get_owner_task(void) { return(owner_task);}
    int                 get_buffer(struct _COMMON_DATA & CommonData, int wait);
    int                 close (void);
    int                 getachar (struct _COMMON_DATA & CommonData, int wait, LPTSTR ch);
    int                 put_data (struct _COMMON_DATA & CommonData, LPTSTR pData, unsigned long length);
#if defined(_UNICODE) || defined(UNICODE)
    int                 put_data (struct _COMMON_DATA & CommonData, char * pData, unsigned long length);
#endif
    int                 put_data_buffered (struct _COMMON_DATA & CommonData, LPTSTR pData, unsigned long length);
    int                 put_data_flush (struct _COMMON_DATA & CommonData);
};


//
//---------------------------------------------------------------------------
// we keep lists of connections in this class

class connection_list {
private:
    connection *      data;
    connection_list * next;

public:
    connection_list   (void);
    ~connection_list  (void);
    void push         (connection & conn);

    // should really use pointer-to-memberfun for these
    connection * find (SOCKET sock);
    int how_many_are_mine (struct _COMMON_DATA & CommonData);

    void remove       (socktag sock);
};

#endif // __CONNECTIONS_H__
