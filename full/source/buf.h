#ifndef BUF_H
#define BUF_H

/*
    Buf class for buffer manipulations
    Harri Pesonen 2002-12-04
*/

#ifndef __STDC_WANT_SECURE_LIB__
#define __STDC_WANT_SECURE_LIB__ 0
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif


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

//#include <stdio.h>
#include <string.h>

#define Buf_Increment 0x2000

class Buf {
private:
    LPTSTR   buffer;
    size_t   buflen;
    size_t   bufsize;

    void SetStringLen(LPCTSTR p, size_t len) {
        Free();
        buflen = len;
        bufsize = buflen + 1;
        buffer = new _TCHAR[bufsize];
        if (p) {
            memcpy(buffer, p, len*sizeof(_TCHAR));
            buffer[buflen] = __T('\0');
        }
        else
            memset(buffer, 0, bufsize*sizeof(_TCHAR));
    }
    void SetString(LPCTSTR p) {
        Free();
        if (p)
            buflen = _tcslen(p);
        else
            buflen = 0;
        bufsize = buflen + 1;
        buffer = new _TCHAR[bufsize];
        if (p)
            _tcscpy(buffer, p);
        else
            buffer[0] = __T('\0');
    }
#if defined(_UNICODE) || defined(UNICODE)
    void SetStringLen(const char * p, size_t len) {
        Free();
        buflen = len;
        bufsize = buflen + 1;
        buffer = new _TCHAR[bufsize];
        if (p) {
            while( len ) {
                --len;
                buffer[len] = (_TCHAR)p[len];
            }
            buffer[buflen] = __T('\0');
        }
        else
            memset(buffer, 0, bufsize*sizeof(_TCHAR));
    }
    void SetString(const char * p) {
        size_t len;
        Free();
        if (p) {
            buflen = strlen(p);
            bufsize = buflen + 1;
            buffer = new _TCHAR[bufsize];
            len = buflen;
            while( len ) {
                --len;
                buffer[len] = (_TCHAR)p[len];
            }
            buffer[buflen] = __T('\0');;
        }
    }
#endif

public:
    ~Buf() {
        if (buffer)
            delete [] buffer;
    }
    Buf() {
        buffer = 0;
        buflen = 0;
        bufsize = 0;
    }
    Buf(LPCTSTR p) {
        buffer = 0;
        SetString(p);
    }
    Buf(LPCTSTR p, size_t len) {
        buffer = 0;
        SetStringLen(p, len);
    }
#if defined(_UNICODE) || defined(UNICODE)
    Buf(const char * p) {
        buffer = 0;
        SetString(p);
    }
    Buf(const char * p, size_t len) {
        buffer = 0;
        SetStringLen(p, len);
    }
#endif
    Buf(size_t alloclen) {
        buffer = 0;
        Free();
        bufsize = alloclen;
        buffer = new _TCHAR[bufsize];
    }
    void Clear() {
        buflen = 0;
        if (!bufsize) {
            buffer = new _TCHAR[Buf_Increment];
            bufsize = Buf_Increment;
        }
        buffer[0] = __T('\0');
    }
    void Free() {
        buflen = 0;
        if (buffer) {
            delete [] buffer;
            buffer = 0;
            bufsize = 0;
        }
    }
    void Clone(const Buf &buf) {
        if (buf.bufsize) {
            Alloc(buf.buflen + 1);
            if (buf.buflen)
                memcpy(buffer, buf.buffer, buf.buflen*sizeof(_TCHAR));

            buflen = buf.buflen;
            buffer[buflen] = __T('\0');
        }
        else
            Free();
    }
    Buf(const Buf &buf) {
        Clear();
        Clone(buf);
    }
    Buf & operator=( const Buf &buf ) {
        Clone(buf);
        return *this;
    }
    void Move(Buf &buf) {
        Free();
        buffer = buf.buffer;
        bufsize = buf.bufsize;
        buflen = buf.buflen;
        buf.buffer = 0;
        buf.bufsize = 0;
        buf.buflen = 0;
    }
    void Alloc(size_t size) {
        if (size > bufsize) {
            bufsize = (size + (Buf_Increment - 1)) & (-1 ^ (Buf_Increment - 1));// 0x7FFFE000;
            LPTSTR newbuffer = new _TCHAR[bufsize];
            if (buffer) {
                memcpy(newbuffer, buffer, (buflen + 1)*sizeof(_TCHAR));
                delete [] buffer;
            }
            buffer = newbuffer;
        }
    }
    void AllocExact(size_t size) {
        if (size > bufsize) {
            bufsize = size;
            LPTSTR newbuffer = new _TCHAR[bufsize];
            if (buffer) {
                memcpy(newbuffer, buffer, (buflen + 1)*sizeof(_TCHAR));
                delete [] buffer;
            }
            buffer = newbuffer;
        }
    }
    void Add(LPCTSTR text, size_t textlen) {
        if (text) {
            size_t newlen = buflen + textlen;
            Alloc(newlen + 1);
            if (textlen)
                memcpy(buffer + buflen, text, textlen*sizeof(_TCHAR));
            buffer[newlen] = __T('\0');
            buflen = newlen;
        }
    }
    void Add(LPCTSTR text) {
        if (text)
            Add(text, _tcslen(text));
        else
            Add(__T(""), 0);
    }
    void Add(_TCHAR ch) {
        Add(&ch, 1);
    }
    void Add(const Buf &buf) {
        Add(buf.buffer, buf.buflen);
    }
#if defined(_UNICODE) || defined(UNICODE)
    void Add(const char * text, size_t textlen) {
        if (text && textlen) {
            size_t newlen = buflen + textlen;
            Alloc(newlen + 1);
            while( textlen ) {
                textlen--;
                buffer[buflen+textlen] = (_TCHAR)text[textlen];
            }
            buffer[newlen] = __T('\0');
            buflen = newlen;
        }
    }
    void Add(const char * text) {
        if (text)
            Add(text, strlen(text));
    }
    void Add(char ch) {
        Add(&ch, 1);
    }
#endif
    LPTSTR Get() {
        return buffer;
    }
    size_t Length() const {
        return buflen;
    }
    LPTSTR GetTail() {
        return buffer + buflen;
    }
    void Expand(size_t size) {
        Alloc(buflen + size);
    }
    void Adjust() {
        if ( buffer )
            buflen += _tcslen(buffer + buflen);
    }
    void SetLength() {
        if (buffer)
            buflen = _tcslen(buffer);
    }
    void SetLength(size_t newlen) {
        if (newlen < bufsize)
            buflen = newlen;
    }
    void Remove() {
        if (buflen)
            buflen--;
    }
    void Remove(size_t position) {
        if (buffer) {
            if (position < buflen) {
                do {
                    buffer[position] = buffer[position+1];
                    position++;
                } while (position < buflen);
                buflen--;
            }
        }
    }
    operator LPCTSTR () const{
        return buffer;
    }
    void operator=(LPCTSTR p) {
        if (buffer) {
            Clear();
            Add(p);
        }
        else
            SetString(p);
    }
#if defined(_UNICODE) || defined(UNICODE)
    void operator=(const char * p) {
        if (buffer) {
            Clear();
            Add(p);
        }
        else
            SetString(p);
    }
#endif

    void operator=(_TCHAR c) {
        if (buffer) {
            Clear();
        }
        Add(c);
    }
};

#endif
